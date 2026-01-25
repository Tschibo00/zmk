    katana_layout: katana_layout {
        compatible = "zmk,physical-layout";
        display-name = "katana";
        keys =    //             w   h   x     y  rot rx ry
            <&key_physical_attrs 200 100   0   0    0 0 0>,
            <&key_physical_attrs 100 100 200   0    0 0 0>,
            <&key_physical_attrs 100 100 300   0    0 0 0>,
            <&key_physical_attrs 100 100 400   0    0 0 0>,
            <&key_physical_attrs 100 100 500   0    0 0 0>,
            <&key_physical_attrs 100 100 600   0    0 0 0>,
            <&key_physical_attrs 100 100 700   0    0 0 0>,
            <&key_physical_attrs 100 100 800   0    0 0 0>,
            <&key_physical_attrs 100 100 900   0    0 0 0>,
            <&key_physical_attrs 200 100 1000  0    0 0 0>,

            <&key_physical_attrs 150 100   0  100   0 0 0>,
            <&key_physical_attrs 100 100 150  100   0 0 0>,
            <&key_physical_attrs 100 100 250  100   0 0 0>,
            <&key_physical_attrs 100 100 350  100   0 0 0>,
            <&key_physical_attrs 100 100 450  100   0 0 0>,
            <&key_physical_attrs 100 100 650  100   0 0 0>,
            <&key_physical_attrs 100 100 750  100   0 0 0>,
            <&key_physical_attrs 100 100 850  100   0 0 0>,
            <&key_physical_attrs 100 100 950  100   0 0 0>,
            <&key_physical_attrs 150 100 1050 100   0 0 0>,

            <&key_physical_attrs 100 100   0  200   0 0 0>,
            <&key_physical_attrs 100 100 100  200   0 0 0>,
            <&key_physical_attrs 100 100 200  200   0 0 0>,
            <&key_physical_attrs 100 100 300  200   0 0 0>,
            <&key_physical_attrs 100 100 400  200   0 0 0>,
            <&key_physical_attrs 100 100 700  200   0 0 0>,
            <&key_physical_attrs 100 100 800  200   0 0 0>,
            <&key_physical_attrs 100 100 900  200   0 0 0>,
            <&key_physical_attrs 100 100 1000 200   0 0 0>,
            <&key_physical_attrs 100 100 1100 200   0 0 0>,

            <&key_physical_attrs 300 100 250  300   0 0 0>,
            <&key_physical_attrs 100 100 550  300   0 0 0>,
            <&key_physical_attrs 300 100 650  300   0 0 0>,

            <&key_physical_attrs 100 100   0  400   0 0 0>,
            <&key_physical_attrs 100 100 100  400   0 0 0>,
            <&key_physical_attrs 100 100 200  400   0 0 0>,
            <&key_physical_attrs 100 100 300  400   0 0 0>,
            <&key_physical_attrs 100 100 400  400   0 0 0>,
            <&key_physical_attrs 100 100 500  400   0 0 0>,
            <&key_physical_attrs 100 100 600  400   0 0 0>,
            <&key_physical_attrs 100 100 700  400   0 0 0>,
            <&key_physical_attrs 100 100 800  400   0 0 0>,
            <&key_physical_attrs 100 100 900  400   0 0 0>,
            <&key_physical_attrs 100 100 1000 400   0 0 0>,
            <&key_physical_attrs 100 100 1100 400   0 0 0>;
        transform = <&katana_transform>;
    };
