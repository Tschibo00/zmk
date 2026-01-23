    combos {
        compatible = "zmk,combos";
        
        COMBO(cmb_caps,      4  5,  &caps_word)
        COMBO(cmb_tab,       13 14, &kp TAB)
        COMBO(cmb_sfttab,    2  3,  &kp LS(TAB))
        COMBO(cmb_bksp_left, 12 13, &kp BSPC)
        COMBO(cmb_bksp,      18 19, &kp BSPC)
        COMBO(cmb_del_left,  1  2,  &kp DEL)
        COMBO(cmb_del,       7  8,  &kp DEL)
        COMBO(cmb_delword,   28 29, &delword)
        COMBO(cmb_delline,   29 30, &delline)
        COMBO(cmb_ctlx,      21 22, &kp LC(X))
        COMBO(cmb_ctlc,      22 23, &kp LC(C))
        COMBO(cmb_ctlv,      23 24, &kp LC(V))
    };
