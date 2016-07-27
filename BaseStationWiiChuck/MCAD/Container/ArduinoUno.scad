MountingHolePos = [[14,2.5,0],
                   [15.3,50.7,0],
                   [66.1,7.6,0],
                   [66.1,35.5,0]];

module ArduinoUno() {
    union() {
        // uno board
        difference() {
            linear_extrude(height = 1.6)
            polygon([[0,    0    ],
                    [66.1, 0    ],
                    [68.6, 2    ],
                    [68.6, 37   ],
                    [66.1, 39   ],
                    [66.1, 53.3 ],
                    [0,    53.3 ]
                    ]);
            for( a=[0:3] ) {
                translate( MountingHolePos[a] )
                cylinder(h=4,d=3.175,center=true);
            }
        }
        // Headers
        translate([20,  1,    1.6])     cube([40,5,10]);
        translate([20,  47.3, 1.6])     cube([40,5,10]);
        // USB Conn
        translate([-10, 30,   1.6])     cube([20,15,15]);
        // Power Conn
        translate([-3,  5,    1.6])     cube([15, 10, 10]);
    }
}

module ArduinoUnoMount(height = 5) {
    union() {
        for( a=[0:3] ) {
                translate( MountingHolePos[a] ) {
                    translate([0,0,height/2]) {
                    cylinder(h=height,d=4,center=true);
                    translate([0,0,height/2+4/2])
                    cylinder(h=4,d=2.5,center=true);
                }
            }
        }
    }
    
}

//ArduinoUno();
//ArduinoUnoMount(height=5);
