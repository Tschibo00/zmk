#include <zmk-helpers/helper.h>

/* Create a ZMK Behavior without having to specify the name three times */
#define ZMK_BEHAVIOR(NAME, ...) \
    / { \
        behaviors { \
            NAME: NAME { \
                __VA_ARGS__ \
            }; \
        }; \
    };

#define ZMK_HOLD_TAP(name, ...) ZMK_BEHAVIOR(name, hold_tap, __VA_ARGS__)
#define ZMK_MOD_MORPH(name, ...) ZMK_BEHAVIOR(name, mod_morph, __VA_ARGS__)
    
/* Mod-Morph helper */
#define MOD_MORPH(NAME, STANDARD, MORPHED, MODS, ...) \
    ZMK_BEHAVIOR(NAME, \
        compatible = "zmk,behavior-mod-morph"; \
        #binding-cells = <0>; \
        bindings = <STANDARD>, <MORPHED>; \
        mods = <(MODS)>; \
        __VA_ARGS__ \
    )

/* Mod-Morph that sends another key when Shift is held */
#define SHIFT_MORPH(NAME, LOWER, UPPER) \
    MOD_MORPH(NAME, LOWER, UPPER, MOD_LSFT|MOD_RSFT)

/* Combo helper */
#define ZMK_COMBO(NAME, TIMEOUT_MS, KEY_POSITIONS, LAYERS, BINDINGS) \
    NAME { \
        timeout-ms = <TIMEOUT_MS>; \
        key-positions = <KEY_POSITIONS>; \
        layers = <LAYERS>; \
        bindings = <BINDINGS>; \
    }; \
/* Limit to base layers and use defined timer for most Combos */
#define COMBO(NAME, KEY_POSITIONS, BINDINGS) \
    ZMK_COMBO(NAME, COMBO_TIMEOUT_MS, KEY_POSITIONS, BASE_LAYERS, BINDINGS)

/* Wrap ZMK_MACRO to avoid needing to specify Macro node */
#define ZMKMACRO(NAME, ...) \
    / { \
        macros { \
            ZMK_MACRO(NAME, __VA_ARGS__) \
        }; \
    };
/* Instant key press Macro helper using defined timers */
#define FAST_MACRO(NAME, BINDINGS) \
    NAME: NAME { \
        compatible = "zmk,behavior-macro"; \
        #binding-cells = <0>; \
        tap-ms = <FAST_MACRO_TAP_MS>; \
        wait-ms = <FAST_MACRO_WAIT_MS>; \
        bindings = <BINDINGS>; \
    };

// reuse basic mod-morph scheme
#define SIMPLE_MORPH(NAME, MOD, BINDING1, BINDING2) \
    ZMK_MOD_MORPH(NAME, \
        mods = <(MOD_L ## MOD|MOD_R ## MOD)>; \
        bindings = <BINDING1>, <BINDING2>; \
    )

// HRM shift-morphed as seen on https://github.com/urob/zmk-config/blob/main/config/base.keymap
#define MY_HOLD_TAP(NAME, TAP, HOLD) \
    / { \
        behaviors { \
            NAME: NAME { \
                compatible = "zmk,behavior-hold-tap"; \
                #binding-cells = <2>; \
                flavor = "balanced"; \
                tapping-term-ms = <200>; \
                quick-tap-ms = <0>; \
                bindings = <HOLD>, <TAP>; \
            }; \
        }; \
    };

#define ZMK_LEADER_SEQUENCE(name, leader_bindings, leader_sequence) \
    / { \
        behaviors { \
            leader: leader { \
                compatible = "zmk,behavior-leader-key"; \
                #binding-cells = <0>; \
                ignore-keys = <LSHFT RSHFT>; \
                leader_sequence_ ## name { \
                    bindings = <leader_bindings>; \
                    sequence = <leader_sequence>; \
                }; \
            }; \
        }; \
    };
