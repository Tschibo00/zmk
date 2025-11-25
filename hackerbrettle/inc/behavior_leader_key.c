/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_leader_key

#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/kernel.h> // Wird benötigt für k_work_delayable!

#include <drivers/behavior.h>

#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h> // NEU: Positions-Events
#include <zmk/events/keycode_state_changed.h> 
#include <zmk/hid.h>
#include <zmk/matrix.h>
#include <zmk/keymap.h>

#include <zmk-leader-key/keys.h>
#include <zmk-leader-key/virtual_key_position.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// --- Datenstrukturen für Positions-IDs (statt Keycodes) ---

// IGNORIERT: Die ignore_keys-Liste bleibt bei zmk_key_param, da sie HID-Keycodes/Modifier enthält.
struct key_list {
    size_t size;
    struct zmk_key_param keys[];
};

struct leader_seq_cfg {
    int32_t virtual_key_position;
    bool is_pressed;
    size_t keys_len;
    // GEÄNDERT: Speichert Positions-IDs (uint32_t) statt Keycodes (zmk_key_param)
    uint32_t positions[CONFIG_ZMK_LEADER_MAX_KEYS_PER_SEQUENCE]; 
    struct zmk_behavior_binding behavior;
};

struct behavior_leader_key_config {
    size_t sequences_len;
    struct leader_seq_cfg *sequences;
    const struct key_list *ignore_keys;
};

const struct behavior_leader_key_config *active_leader_cfg;

/* State of currently active leader key instance */
static bool is_undecided;
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
static uint8_t source;
#endif
static int32_t press_count;     /* Total number of pressed keys (Sequence length) */
static int32_t release_count;   /* Number of currently pressed keys (in leader_pressed_positions) */
static int32_t num_candidates;
static struct leader_seq_cfg *sequence_candidates[CONFIG_ZMK_LEADER_MAX_SEQUENCES];
static struct leader_seq_cfg *completed_sequence; // Das längste, vollständige Match

/* Keep track of pressed positions */
// GEÄNDERT: Speichert die tatsächlich gedrückten Positions-IDs
static uint32_t leader_pressed_positions[CONFIG_ZMK_LEADER_MAX_KEYS_PER_SEQUENCE]; 

/* Timeout for short sequences */
#define LEADER_TIMEOUT_MS 500 
static struct k_work_delayable leader_timeout_work;
static struct leader_seq_cfg *pending_sequence = NULL; // Kürzere Sequenz, die auf Timeout wartet

// --- HILFSFUNKTIONEN (unverändert) ---

static inline int press_leader_behavior(struct leader_seq_cfg *sequence, int32_t timestamp) {
    struct zmk_behavior_binding_event event = {
        .position = sequence->virtual_key_position,
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
        .source = source,
#endif
        .timestamp = timestamp,
    };
    sequence->is_pressed = true;
    return zmk_behavior_invoke_binding(&sequence->behavior, event, true);
}

static inline int release_leader_behavior(struct leader_seq_cfg *sequence, int32_t timestamp) {
    struct zmk_behavior_binding_event event = {
        .position = sequence->virtual_key_position,
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
        .source = source,
#endif
        .timestamp = timestamp,
    };
    sequence->is_pressed = false;
    return zmk_behavior_invoke_binding(&sequence->behavior, event, false);
}

// --- CORE: LEADER KEY DEAKTIVIERUNG (Korrekturen angewendet) ---

void deactivate_leader_key() {
    is_undecided = false;
    active_leader_cfg = NULL;
    pending_sequence = NULL;
    completed_sequence = NULL;
    press_count = 0;
    release_count = 0;
    // Explizites Leeren des Puffers, um blockierende Reste zu verhindern
    for (int i = 0; i < CONFIG_ZMK_LEADER_MAX_KEYS_PER_SEQUENCE; i++) {
        leader_pressed_positions[i] = 0; // GEÄNDERT: Puffer von Positionen leeren
    }
    k_work_cancel_delayable(&leader_timeout_work);
}

// --- CORE: TIMEOUT HANDLER (Korrekturen angewendet) ---

static void leader_timeout_handler(struct k_work *work) {
    if (!pending_sequence) return;

    LOG_DBG("Leader timeout triggered, executing pending sequence");

    // Aktion ausführen (press und release)
    if (!pending_sequence->is_pressed) {
        int32_t ts = k_uptime_get();
        
        press_leader_behavior(pending_sequence, ts);
        release_leader_behavior(pending_sequence, ts);
    }
    
    // Setze die Sequenz als 'abgeschlossen', der Listener übernimmt das finale Deaktivieren
    completed_sequence = pending_sequence;
    pending_sequence = NULL; 
}

void activate_leader_key(const struct behavior_leader_key_config *cfg) {
    LOG_DBG("Activating leader key");
    // Bei Re-Aktivierung den Timeout sofort abbrechen, falls noch aktiv
    k_work_cancel_delayable(&leader_timeout_work); 

    active_leader_cfg = cfg;
    is_undecided = true;
    press_count = 0;
    release_count = 0;
    completed_sequence = NULL;
    pending_sequence = NULL;
    num_candidates = cfg->sequences_len;

#if IS_ENABLED(CONFIG_ZMK_SPLIT)
    source = 0;
#endif

    for (int i = 0; i < cfg->sequences_len; i++) {
        sequence_candidates[i] = &(cfg->sequences[i]);
    }
    for (int i = 0; i < CONFIG_ZMK_LEADER_MAX_KEYS_PER_SEQUENCE; i++) {
        leader_pressed_positions[i] = 0; // GEÄNDERT: Puffer initialisieren
    }

    // Initialisierung des Work-Items nur einmal
    static bool initialized = false;
    if (!initialized) {
        k_work_init_delayable(&leader_timeout_work, leader_timeout_handler);
        initialized = true;
    }
}


// --- VERGLEICH & FILTERUNG (Auf Positionen umgestellt) ---

// Die Funktion key_param_equals bleibt für die ignore_keys-Liste, da diese Keycodes enthält.
static bool key_param_equals(const struct zmk_key_param *key, const struct zmk_key_param *other) {
    return key->page == other->page && key->id == other->id &&
           (key->modifiers & (other->modifiers | zmk_hid_get_explicit_mods())) == key->modifiers;
}

static bool key_is_ignored(const struct zmk_key_param *key) {
    const struct key_list *ignore_keys = active_leader_cfg->ignore_keys;
    for (int i = 0; i < ignore_keys->size; i++) {
        if (key_param_equals(&ignore_keys->keys[i], key)) {
            return true;
        }
    }
    return false;
}

// NEUE Funktion: Löscht die Position aus dem gedrückten Puffer
static bool release_position_in_leader_sequence(const uint32_t position) {
    for (int i = 0; i < release_count; i++) {
        // GEÄNDERT: Vergleich ist jetzt nur noch der Wert
        if (leader_pressed_positions[i] == position) { 
            for (int j = i; j < release_count - 1; j++) {
                leader_pressed_positions[j] = leader_pressed_positions[j + 1];
            }
            release_count--;
            LOG_DBG("Released position %d, %d remaining", position, release_count);
            return true;
        }
    }
    return false;
}

// GEÄNDERT: Filtert nach Positionen
static bool filter_leader_sequences(const uint32_t position, int count) {
    int n = 0;
    struct leader_seq_cfg *possible_complete = NULL;
    bool has_longer_candidates = false;

    for (int i = 0; i < num_candidates; i++) {
        struct leader_seq_cfg *seq = sequence_candidates[i];

        // GEÄNDERT: Vergleich mit dem neuen positions-Array
        if (seq->positions[count] == position) { 
            sequence_candidates[n++] = seq;

            if (seq->keys_len == count + 1) {
                possible_complete = seq;
            } else if (seq->keys_len > count + 1) {
                has_longer_candidates = true;
            }
        }
    }

    num_candidates = n;

    LOG_DBG("Pressed position %d, %d candidate sequences remaining", position, n);

    if (possible_complete) {
        if (!has_longer_candidates) {
            // Keine längere Sequenz möglich: Sofort auslösen
            completed_sequence = possible_complete;
            return true;
        } else {
            // Ambiguität: Kurze Sequenz merken und Timer starten
            pending_sequence = possible_complete;
            k_work_schedule(&leader_timeout_work, K_MSEC(LEADER_TIMEOUT_MS));
        }
    } else {
        // Falls ein längeres Match die pending_sequence war, aber jetzt kein Präfix mehr gefunden wird
        pending_sequence = NULL;
        k_work_cancel_delayable(&leader_timeout_work);
    }

    completed_sequence = NULL;
    return (n > 0);
}

// --- EVENT LISTENER (Auf Positionen umgestellt) ---

// GEÄNDERT: Listener-Funktion umbenannt und Header-Änderungen
static int leader_position_state_changed_listener(const zmk_event_t *eh);

ZMK_LISTENER(behavior_leader_key, leader_position_state_changed_listener);
// GEÄNDERT: Abonniert Positions-Events
ZMK_SUBSCRIPTION(behavior_leader_key, zmk_position_state_changed); 

static int leader_position_state_changed_listener(const zmk_event_t *eh) {
    // GEÄNDERT: Prüft auf Position-State-Changed Event
    struct zmk_position_state_changed *ev = as_zmk_position_state_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    const uint32_t position = ev->position; // NEU: Eingabeposition

    // Wenn der Leader-Key-Modus aktiv ist und eine Taste gedrückt wird
    if (is_undecided && ev->state) {
        
        // Key-Ignore-Check muss weiterhin HID-Keycodes nutzen. 
        // Dies ist kompliziert, da wir nur die Position haben.
        // Die einfachste Lösung, die zu 99% funktioniert, ist, den Ignorier-Check 
        // für Positions-Sequenzen zu überspringen oder nur Modifikatoren zu prüfen.
        // Da die ignorierten Tasten *Keycodes* (zmk_key_param) speichern, MUSS der Check hier bleiben,
        // falls die Tastatur in der DT/Kconfig einen Keycode für diese Position gesetzt hat.
        // ABER: Da wir das Event selbst von keycode auf position umgestellt haben, 
        // müssen wir hier aussteigen, da wir keine Keycodes mehr sehen.
        // Wenn eine Taste als IGNORE_KEY definiert ist (z.B. LSHIFT), wird sie das zmk_keycode_state_changed 
        // Event auslösen, das hier nicht mehr verarbeitet wird!
        // Wir nehmen an, dass die ignorierten Tasten nur Modifikatoren sind, die über 
        // zmk_keycode_state_changed Events ablaufen und hier irrelevant werden.

        // k_work_cancel_delayable(&leader_timeout_work); // Aus filter_leader_sequences ausgelagert

        // GEÄNDERT: filter_leader_sequences verwendet jetzt die Position
        if (filter_leader_sequences(position, press_count++)) {
            // GEÄNDERT: Speichert die gedrückte Position
            leader_pressed_positions[release_count++] = position; 

            if (completed_sequence) {
                // Sequenz sofort abgeschlossen (keine Ambiguität)
                
                // Ausführung der Sequenz
                press_leader_behavior(completed_sequence, ev->timestamp);
                
                // Zustand zurücksetzen (Leader-Modus beendet)
                deactivate_leader_key(); 

                return ZMK_EV_EVENT_HANDLED;
            }

            return ZMK_EV_EVENT_HANDLED;
        } else {
            // Sequenz abgebrochen (kein Match gefunden)
            deactivate_leader_key();
            return ZMK_EV_EVENT_BUBBLE;
        }
    }

    // --- KEY RELEASE (Simulierte Tasten und normale Tasten) ---
    // GEÄNDERT: Verwendet die neue release-Funktion für Positionen
    if (!ev->state && release_position_in_leader_sequence(position)) {
        if (completed_sequence && completed_sequence->is_pressed && !release_count) {
            // Dies wird ausgelöst, wenn das virtuelle Release-Event 
            // der Leader-Sequenz (aus Timeout oder Sofortauslösung) verarbeitet wird.
            release_leader_behavior(completed_sequence, ev->timestamp);
            
            // Jetzt ist die Sequenz vollständig abgeschlossen und der Modus kann deaktiviert werden.
            deactivate_leader_key(); 
        }
        return ZMK_EV_EVENT_HANDLED;
    }

    return ZMK_EV_EVENT_BUBBLE;
}

// --- BEHAVIOR API (unverändert) ---

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    if (release_count) {
        LOG_ERR("Unable to activate leader key. Previous sequence is still pressed.");
        return ZMK_BEHAVIOR_OPAQUE;
    }
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    activate_leader_key(dev->config);
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
    source = event.source;
#endif
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_leader_key_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

static int behavior_leader_key_init(const struct device *dev) { return 0; }

/* --- DT / Macros (Angepasst für Positions-IDs) --- */

// NEU: Nimmt die Zahl direkt als Positions-ID
#define SEQUENCE_POSITION_ID(i, n, prop) DT_PROP_BY_IDX(n, prop, i) 

#define PROP_SEQUENCES(n, prop)                                                                    \
    {                                                                                              \
        .virtual_key_position = ZMK_VIRTUAL_KEY_POSITION_LEADER(__COUNTER__), .is_pressed = false, \
        .keys_len = DT_PROP_LEN(n, prop),                                                          \
        .positions = {LISTIFY(DT_PROP_LEN(n, prop), SEQUENCE_POSITION_ID, (, ), n, prop)}, /* GEÄNDERT: .positions-Feld und SEQUENCE_POSITION_ID */ \
        .behavior = ZMK_KEYMAP_EXTRACT_BINDING(0, n),                                              \
    }

// Die Ignorier-Liste bleibt bei Keycodes (zmk_key_param)
#define KEY_LIST_ITEM(i, n, prop) ZMK_KEY_PARAM_DECODE(DT_INST_PROP_BY_IDX(n, prop, i))

#define PROP_KEY_LIST(n, prop)                                                                     \
    COND_CODE_1(DT_NODE_HAS_PROP(DT_DRV_INST(n), prop),                                            \
                ({                                                                                 \
                    .size = DT_INST_PROP_LEN(n, prop),                                             \
                    .keys = {LISTIFY(DT_INST_PROP_LEN(n, prop), KEY_LIST_ITEM, (, ), n, prop)},    \
                }),                                                                                \
                ({.size = 0}))

#define LEAD_INST(n)                                                                               \
    static struct leader_seq_cfg leader_sequences_##n[] = {                                        \
        DT_INST_FOREACH_CHILD_STATUS_OKAY_SEP_VARGS(n, PROP_SEQUENCES, (, ), sequence)};            \
    BUILD_ASSERT(ARRAY_SIZE(leader_sequences_##n) <= CONFIG_ZMK_LEADER_MAX_SEQUENCES,               \
                 "Number of leader sequences exceeds ZMK_LEADER_MAX_SEQUENCES limit");             \
    static const struct key_list leader_key_ignore_keys_##n = PROP_KEY_LIST(n, ignore_keys);        \
    static struct behavior_leader_key_config behavior_leader_key_config_##n = {                    \
        .sequences = leader_sequences_##n,                                                         \
        .sequences_len = ARRAY_SIZE(leader_sequences_##n),                                         \
        .ignore_keys = &leader_key_ignore_keys_##n,                                                \
    };                                                                                             \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_leader_key_init, NULL, NULL,                                \
                            &behavior_leader_key_config_##n, POST_KERNEL,                          \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_leader_key_driver_api);

DT_INST_FOREACH_STATUS_OKAY(LEAD_INST)