    macros {
        FAST_MACRO(delword, &macro_press &kp LSFT &macro_press &kp LCTL &macro_tap &kp LEFT &macro_release &kp LCTL &macro_release &kp LSFT &macro_tap &kp DEL)
        FAST_MACRO(delline, &macro_tap &kp END &macro_press &kp LSFT &macro_tap &kp HOME &macro_release &kp LSFT &macro_tap &kp DELETE &macro_tap &kp DELETE)
        FAST_MACRO(mc_caret, &macro_tap &kp CARET &macro_tap &kp SPACE)
        FAST_MACRO(mc_tilde, &macro_tap &kp TILDE &macro_tap &kp SPACE)
        FAST_MACRO(mc_apos, &macro_tap &kp APOS &macro_tap &kp SPACE)
        
        // one-key sequences
        FAST_MACRO(u_q,  &macro_press &kp LGUI
                &macro_tap &kp L
                &macro_release &kp LGUI)
        FAST_MACRO(u_e,  &macro_tap &kp LSHFT &kp LSHFT)
        FAST_MACRO(u_r,  &macro_tap &kp HOME &kp HOME &kp SLASH &kp SLASH)
        FAST_MACRO(u_t,  &macro_press &kp LCTL
                &macro_tap &kp X
                &macro_release &kp LCTL
                &macro_tap &kp SLASH &kp STAR &kp RET
                &macro_press &kp LCTL
                &macro_tap &kp V
                &macro_release &kp LCTL
                &macro_tap &kp STAR &kp SLASH &kp RET)
        FAST_MACRO(u_y,  &macro_press &kp LCTL
                &macro_press &kp LSHFT
                &macro_tap &kp KP_SLASH
                &macro_release &kp LSHFT
                &macro_release &kp LCTL)
        FAST_MACRO(u_u,  &macro_tap &kp GRAVE &kp GRAVE &kp GRAVE &kp SPACE
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
        FAST_MACRO(u_i,  &macro_tap &kp END
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
        FAST_MACRO(u_s,  &macro_press &kp LGUI
                &macro_tap &kp R
                &macro_release &kp LGUI)
        FAST_MACRO(u_d,  &macro_tap &kp PRINTSCREEN)
        FAST_MACRO(u_f,  &macro_tap &macro_press &kp LGUI
                &macro_press &kp LALT
                &macro_tap &kp K
                &macro_release &kp LALT
                &macro_release &kp LGUI)
        FAST_MACRO(u_h,  &macro_tap &kp MINUS &kp SPACE &kp LBKT &kp SPACE &kp RBKT &kp SPACE)
        FAST_MACRO(u_j,  &macro_tap &kp LPAR &kp RPAR &kp LEFT)
        FAST_MACRO(u_k,  &macro_tap &kp LBKT &kp RBKT &kp LEFT)
        FAST_MACRO(u_l,  &macro_tap &kp LBRC &kp RBRC &kp LEFT)
        FAST_MACRO(u_p,  &macro_tap &kp GRAVE &kp GRAVE &kp LEFT)
        FAST_MACRO(u_z,  &macro_press &kp LCTL
                &macro_tap &kp Z
                &macro_release &kp LCTL)
        FAST_MACRO(u_x,  &macro_press &kp LCTL
                &macro_tap &kp Y
                &macro_release &kp LCTL)
        FAST_MACRO(u_c,  &macro_press &kp LCTL
                &macro_tap &kp C
                &macro_release &kp LCTL)
        FAST_MACRO(u_v,  &macro_press &kp LCTL
                &macro_tap &kp X
                &macro_release &kp LCTL)
        FAST_MACRO(u_b,  &macro_press &kp LCTL
                &macro_tap &kp V
                &macro_release &kp LCTL)
        FAST_MACRO(u_n,  &macro_press &kp LCTL
                &macro_press &kp LALT
                &macro_tap &kp V
                &macro_release &kp LALT
                &macro_release &kp LCTL)
        FAST_MACRO(u_m,  &macro_press &kp LCTL
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
        FAST_MACRO(u_rgt,  &macro_press &kp LGUI
                &macro_press &kp LSHFT
                &macro_tap &kp RIGHT
                &macro_release &kp LSHFT
                &macro_release &kp LGUI)
        
        // two-key sequences
        // status messages
        FAST_MACRO(u_sm, &macro_tap &kp HASH &kp G &kp O &kp O &kp D
                &macro_press &kp LSHFT
                &macro_tap &kp M
                &macro_release &kp LSHFT
                &macro_tap &kp O &kp R &kp N &kp I &kp N &kp G)

         // emojis
        FAST_MACRO(u_en, &macro_tap &kp COLON &kp LPAR )
        FAST_MACRO(u_eb, &macro_tap &kp COLON &kp RPAR)
    };
 