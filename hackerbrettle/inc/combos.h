    combos {
        compatible = "zmk,combos";
        
        COMBO(cmb_caps, 4 5, &caps_word)
        COMBO(cmb_tab, 12 13, &kp TAB)
        COMBO(cmb_sfttab, 2 3, &kp LS(TAB))
        COMBO(cmb_bksp, 17 18, &kp BSPC)
        COMBO(cmb_del, 7 8, &kp DEL)
        COMBO(cmb_delword, 27 28, &delword)
        COMBO(cmb_delline, 28 29, &delline)
        COMBO(cmb_ctlx, 20 21, &kp LC(X))
        COMBO(cmb_ctlc, 21 22, &kp LC(C))
        COMBO(cmb_ctlv, 22 23, &kp LC(V))
    };
