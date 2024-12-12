    macros {
        FAST_MACRO(delword, &macro_press &kp LSFT &macro_press &kp LCTL &macro_tap &kp LEFT &macro_release &kp LCTL &macro_release &kp LSFT &macro_tap &kp DEL)
        FAST_MACRO(delline, &macro_tap &kp END &macro_press &kp LSFT &macro_tap &kp HOME &macro_release &kp LSFT &macro_tap &kp DELETE &macro_tap &kp DELETE)
        FAST_MACRO(mc_caret, &macro_tap &kp CARET &macro_tap &kp SPACE)
        FAST_MACRO(mc_tilde, &macro_tap &kp TILDE &macro_tap &kp SPACE)
        FAST_MACRO(mc_apos, &macro_tap &kp APOS &macro_tap &kp SPACE)
        
        // UNICORN (workaround for missing/non-functioning leader key in ZMK)
        // one-key sequences
        FAST_MACRO(u_q,  &macro_press &kp LGUI
                &macro_tap &kp L
                &macro_release &kp LGUI)
        FAST_MACRO(u_f,  &macro_tap &kp LSHFT &kp LSHFT)
        FAST_MACRO(u_p,  &macro_tap &kp HOME &kp HOME &kp SLASH &kp SLASH)
        FAST_MACRO(u_b,  &macro_press &kp LCTL
                &macro_tap &kp X
                &macro_release &kp LCTL
                &macro_tap &kp SLASH &kp STAR &kp RET
                &macro_press &kp LCTL
                &macro_tap &kp V
                &macro_release &kp LCTL
                &macro_tap &kp STAR &kp SLASH &kp RET)
        FAST_MACRO(u_j,  &macro_press &kp LCTL
                &macro_press &kp LSHFT
                &macro_tap &kp KP_SLASH
                &macro_release &kp LSHFT
                &macro_release &kp LCTL)
        FAST_MACRO(u_l,  &macro_tap &kp GRAVE &kp GRAVE &kp GRAVE &kp SPACE
                &macro_press &kp LSHFT
                &macro_tap &kp RET
                &macro_release &kp LSHFT
                &macro_press &kp LCTL
                &macro_tap &kp V
                &macro_release &kp LCTL
                &macro_press &kp LSHFT
                &macro_tap &kp RET
                &macro_release &kp LSHFT
                &macro_tap &kp GRAVE &kp GRAVE &kp GRAVE &kp SPACE)
        FAST_MACRO(u_u,  &macro_tap &kp END
                &macro_press &kp LSHFT
                &macro_tap &kp HOME &kp HOME
                &macro_release &kp LSHFT
                &macro_press &kp LCTL
                &macro_tap &kp C
                &macro_release &kp LCTL
                &macro_tap &kp END &kp RET &kp HOME &kp SLASH &kp SLASH &kp SPACE
                &macro_press &kp LCTL
                &macro_tap &kp V
                &macro_release &kp LCTL
                &macro_tap &kp HOME &kp UP)
        FAST_MACRO(u_a,  &macro_tap &kp MINUS &kp GT)
        FAST_MACRO(u_r,  &macro_press &kp LGUI
                &macro_tap &kp R
                &macro_release &kp LGUI)
        FAST_MACRO(u_s,  &macro_tap &kp PRINTSCREEN)
        FAST_MACRO(u_t,  &macro_tap &macro_press &kp LGUI
                &macro_press &kp LALT
                &macro_tap &kp K
                &macro_release &kp LALT
                &macro_release &kp LGUI)
        FAST_MACRO(u_m,  &macro_tap &kp MINUS &kp SPACE &kp LBKT &kp SPACE &kp RBKT &kp SPACE)
        FAST_MACRO(u_n,  &macro_tap &kp LPAR &kp RPAR &kp LEFT)
        FAST_MACRO(u_e,  &macro_tap &kp LBKT &kp RBKT &kp LEFT)
        FAST_MACRO(u_i,  &macro_tap &kp LBRC &kp RBRC &kp LEFT)
        FAST_MACRO(u_o,  &macro_tap &kp GRAVE &kp GRAVE &kp LEFT)
        FAST_MACRO(u_z,  &macro_press &kp LCTL
                &macro_tap &kp Z
                &macro_release &kp LCTL)
        FAST_MACRO(u_x,  &macro_press &kp LCTL
                &macro_tap &kp Y
                &macro_release &kp LCTL)
        FAST_MACRO(u_c,  &macro_press &kp LCTL
                &macro_tap &kp C
                &macro_release &kp LCTL)
        FAST_MACRO(u_d,  &macro_press &kp LCTL
                &macro_tap &kp X
                &macro_release &kp LCTL)
        FAST_MACRO(u_v,  &macro_press &kp LCTL
                &macro_tap &kp V
                &macro_release &kp LCTL)
        FAST_MACRO(u_k,  &macro_press &kp LCTL
                &macro_press &kp LALT
                &macro_tap &kp V
                &macro_release &kp LALT
                &macro_release &kp LCTL)
        FAST_MACRO(u_h,  &macro_press &kp LCTL
                &macro_press &kp LSHFT
                &macro_tap &kp V
                &macro_release &kp LSHFT
                &macro_release &kp LCTL)
        FAST_MACRO(u_apo,  &macro_tap &kp DQT &kp SPACE &kp DQT &kp SPACE &kp LEFT)
        FAST_MACRO(u_lft,  &macro_press &kp LGUI
                &macro_press &kp LSHFT
                &macro_tap &kp LEFT
                &macro_release &kp LSHFT
                &macro_release &kp LGUI)
        FAST_MACRO(u_inlft,  &macro_press &kp LALT
                &macro_tap &kp TAB
                &macro_release &kp LALT)
        FAST_MACRO(u_inrgt,  &macro_press &kp LALT
                &macro_press &kp LSHFT
                &macro_tap &kp TAB
                &macro_release &kp LSHFT
                &macro_release &kp LALT)
        FAST_MACRO(u_rgt,  &macro_press &kp LGUI
                &macro_press &kp LSHFT
                &macro_tap &kp RIGHT
                &macro_release &kp LSHFT
                &macro_release &kp LGUI)
    };
 