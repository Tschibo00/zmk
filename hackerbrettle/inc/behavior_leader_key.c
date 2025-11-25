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
#include <zephyr/kernel.h>

#include <drivers/behavior.h>

#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>
#include <zmk/matrix.h>
#include <zmk/keymap.h>

#include <zmk-leader-key/keys.h>
#include <zmk-leader-key/virtual_key_position.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

struct key_list {
    size_t size;
    struct zmk_key_param keys[];
};

struct leader_seq_cfg {
    int32_t virtual_key_position;
    bool is_pressed;
    size_t keys_len;
    struct zmk_key_param keys[CONFIG_ZMK_LEADER_MAX_KEYS_PER_SEQUENCE];
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
static int32_t press_count;   /* Total number of pressed keys */
static int32_t release_count; /* Number of currently pressed keys */
static int32_t num_candidates;
static struct leader_seq_cfg *sequence_candidates[CONFIG_ZMK_LEADER_MAX_SEQUENCES];
static struct leader_seq_cfg *completed_sequence;

/* Keep track of pressed keys */
static struct zmk_key_param leader_pressed_keys[CONFIG_ZMK_LEADER_MAX_KEYS_PER_SEQUENCE];

/* Timeout for short sequences */
#define LEADER_TIMEOUT_MS 300
static struct k_work_delayable leader_timeout_work;
static struct leader_seq_cfg *pending_sequence = NULL;
/* Globales Work-Item für die asynchrone Ausführung der Sequenzaktion */
static struct k_work leader_action_work;
static struct leader_seq_cfg *action_sequence = NULL;

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

static void leader_action_handler(struct k_work *work) {
    if (!action_sequence) return;

    LOG_DBG("Async action triggered for sequence at %p", action_sequence);
    
    // Aktion ausführen
    if (!action_sequence->is_pressed) {
        int32_t ts = k_uptime_get();
        press_leader_behavior(action_sequence, ts);
        release_leader_behavior(action_sequence, ts);
    }
    
    // Zustandsbereinigung (wie in der letzten Korrektur, aber atomar hier)
    active_leader_cfg = NULL;
    is_undecided = false;
    completed_sequence = NULL;
    pending_sequence = NULL;
    press_count = 0;
    release_count = 0;
    for (int i = 0; i < CONFIG_ZMK_LEADER_MAX_KEYS_PER_SEQUENCE; i++) {
        leader_pressed_keys[i] = (struct zmk_key_param){0};
    }
    
    action_sequence = NULL;
}

static void leader_timeout_handler(struct k_work *work) {
    if (!pending_sequence) return;

    LOG_DBG("Leader timeout triggered, scheduling async action");
    
    // Die Aktion in das separate Work-Item verschieben
    action_sequence = pending_sequence;
    k_work_submit(&leader_action_work); 

    // Sofortiger, minimalistischer Reset, um Race-Conditions im Haupt-Loop zu vermeiden
    pending_sequence = NULL;
    // NICHT den vollen Zustand (active_leader_cfg, press_count etc.) hier zurücksetzen, 
    // das macht der async Worker.
}

void activate_leader_key(const struct behavior_leader_key_config *cfg) {
    LOG_DBG("Activating leader key");
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
        leader_pressed_keys[i] = (struct zmk_key_param){0};
    }

    k_work_init_delayable(&leader_timeout_work, leader_timeout_handler);
    k_work_init(&leader_action_work, leader_action_handler); // <--- HINZUGEFÜGT
}

void deactivate_leader_key() {
    is_undecided = false;
    active_leader_cfg = NULL; // Wichtig: Zustand muss beendet werden
    pending_sequence = NULL;
    completed_sequence = NULL; // Optional, aber sicherer
    press_count = 0;           // <--- HINZUGEFÜGT
    release_count = 0;         // <--- HINZUGEFÜGT
    for (int i = 0; i < CONFIG_ZMK_LEADER_MAX_KEYS_PER_SEQUENCE; i++) {
        leader_pressed_keys[i] = (struct zmk_key_param){0};
    }
    k_work_cancel_delayable(&leader_timeout_work);
}

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

static bool release_key_in_leader_sequence(const struct zmk_key_param *key) {
    for (int i = 0; i < release_count; i++) {
        if (key_param_equals(&leader_pressed_keys[i], key)) {
            for (int j = i; j < release_count - 1; j++) {
                leader_pressed_keys[j] = leader_pressed_keys[j + 1];
            }
            release_count--;
            LOG_DBG("Released key 0x%02X - 0x%02X, %d remaining", key->page, key->id, release_count);
            return true;
        }
    }
    return false;
}

static bool filter_leader_sequences(const struct zmk_key_param *key, int count) {
    int n = 0;
    struct leader_seq_cfg *possible_complete = NULL;
    bool has_longer_candidates = false;

    for (int i = 0; i < num_candidates; i++) {
        struct leader_seq_cfg *seq = sequence_candidates[i];

        if (key_param_equals(&seq->keys[count], key)) {
            sequence_candidates[n++] = seq;

            if (seq->keys_len == count + 1) {
                possible_complete = seq;
            } else if (seq->keys_len > count + 1) {
                has_longer_candidates = true;
            }
        }
    }

    num_candidates = n;

    LOG_DBG("Pressed key 0x%02X - 0x%02X, %d candidate sequences remaining", key->page, key->id, n);

    if (possible_complete) {
        if (!has_longer_candidates) {
            completed_sequence = possible_complete;
            return true;
        } else {
            /* Ambiguität: kurze Sequenz merken und Timer starten */
            pending_sequence = possible_complete;
            k_work_schedule(&leader_timeout_work, K_MSEC(LEADER_TIMEOUT_MS));
        }
    }

    completed_sequence = NULL;
    return (n > 0);
}

static int leader_keycode_state_changed_listener(const zmk_event_t *eh);

ZMK_LISTENER(behavior_leader_key, leader_keycode_state_changed_listener);
ZMK_SUBSCRIPTION(behavior_leader_key, zmk_keycode_state_changed);

static int leader_keycode_state_changed_listener(const zmk_event_t *eh) {
    struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    const struct zmk_key_param key = {
        .modifiers = ev->implicit_modifiers,
        .page = ev->usage_page,
        .id = ev->keycode,
    };

    if (is_undecided && ev->state) {
        if (key_is_ignored(&key)) {
            LOG_DBG("Ignoring key 0x%02X - 0x%02X", key.page, key.id);
            return ZMK_EV_EVENT_BUBBLE;
        }

        if (filter_leader_sequences(&key, press_count++)) {
            leader_pressed_keys[release_count++] = key;

            if (completed_sequence) {
                 k_work_cancel_delayable(&leader_timeout_work);
                 pending_sequence = NULL;
 
                 // Ausführung der Sequenz
                 press_leader_behavior(completed_sequence, ev->timestamp);
 
                 // Zustand zurücksetzen (Leader-Modus beendet)
                 deactivate_leader_key(); // <-- Ruft jetzt die korrigierte Funktion auf
 
                 return ZMK_EV_EVENT_HANDLED;
             }
             
             // ... Rest des Codes bleibt gleich
            return ZMK_EV_EVENT_HANDLED;
        } else {
            deactivate_leader_key();
            return ZMK_EV_EVENT_BUBBLE;
        }
    }

    if (!ev->state && release_key_in_leader_sequence(&key)) {
        if (completed_sequence && completed_sequence->is_pressed && !release_count) {
            release_leader_behavior(completed_sequence, ev->timestamp);
        }
        return ZMK_EV_EVENT_HANDLED;
    }

    return ZMK_EV_EVENT_BUBBLE;
}

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

/* --- DT / Macros unten unverändert, keine Änderungen nötig --- */
#define SEQUENCE_ITEM(i, n, prop) ZMK_KEY_PARAM_DECODE(DT_PROP_BY_IDX(n, prop, i))

#define PROP_SEQUENCES(n, prop)                                                                    \
    {                                                                                              \
        .virtual_key_position = ZMK_VIRTUAL_KEY_POSITION_LEADER(__COUNTER__), .is_pressed = false, \
        .keys_len = DT_PROP_LEN(n, prop),                                                          \
        .keys = {LISTIFY(DT_PROP_LEN(n, prop), SEQUENCE_ITEM, (, ), n, prop)},                     \
        .behavior = ZMK_KEYMAP_EXTRACT_BINDING(0, n),                                              \
    }

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
        DT_INST_FOREACH_CHILD_STATUS_OKAY_SEP_VARGS(n, PROP_SEQUENCES, (, ), sequence)};           \
    BUILD_ASSERT(ARRAY_SIZE(leader_sequences_##n) <= CONFIG_ZMK_LEADER_MAX_SEQUENCES,              \
                 "Number of leader sequences exceeds ZMK_LEADER_MAX_SEQUENCES limit");             \
    static const struct key_list leader_key_ignore_keys_##n = PROP_KEY_LIST(n, ignore_keys);       \
    static struct behavior_leader_key_config behavior_leader_key_config_##n = {                    \
        .sequences = leader_sequences_##n,                                                         \
        .sequences_len = ARRAY_SIZE(leader_sequences_##n),                                         \
        .ignore_keys = &leader_key_ignore_keys_##n,                                                \
    };                                                                                             \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_leader_key_init, NULL, NULL,                               \
                            &behavior_leader_key_config_##n, POST_KERNEL,                          \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_leader_key_driver_api);

DT_INST_FOREACH_STATUS_OKAY(LEAD_INST)
