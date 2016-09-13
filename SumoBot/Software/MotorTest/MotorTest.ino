/**
 * An Mirf example which copies back the data it recives.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 */
#define DEBUG

enum Pins {
  p_motorLeftFwd = 3,
  p_motorLeftBkd = 5,
  p_motorRightFwd = 10,
  p_motorRightBkd = 6
};

void MotorDrive(int dir) {
  const byte full_power_l = 150;
  const byte full_power_r = 255;
  const byte turning_power_l = full_power_l/2;
  const byte turning_power_r = full_power_r/2;
  switch( max(min(dir,9),0) ) {
    case 1:                                           // Back left
      analogWrite(p_motorLeftBkd,  turning_power_l);
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorRightBkd, 0              );
      analogWrite(p_motorRightFwd, full_power_r   );  break;
    case 2:                                           // Back
      analogWrite(p_motorLeftBkd,  0              );
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorRightBkd, 0              );
      analogWrite(p_motorRightFwd, full_power_r   );  break;
    case 3:                                           // Back right
      analogWrite(p_motorLeftBkd,  0              );
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorRightBkd, turning_power_r);
      analogWrite(p_motorRightFwd, full_power_r   );  break;
    case 4:                                           // Hard left
      analogWrite(p_motorLeftFwd,  0              );
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorRightFwd, full_power_r   );
      analogWrite(p_motorRightBkd, 0              );  break;
    case 5:                                           // No Movement
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorRightFwd, full_power_r   );
      analogWrite(p_motorRightBkd, 255   );  break;
    case 6:                                           // Hard right
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorLeftBkd,  0              );
      analogWrite(p_motorRightFwd, 0              );
      analogWrite(p_motorRightBkd, full_power_r   );  break;
    case 7:                                           // Forward left
      analogWrite(p_motorLeftBkd,  turning_power_l);
      analogWrite(p_motorLeftFwd,  0              );
      analogWrite(p_motorRightBkd, full_power_r   );
      analogWrite(p_motorRightFwd, 0              );  break;
    case 8:                                           // Forward
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorLeftFwd,  0              );
      analogWrite(p_motorRightBkd, full_power_r   );
      analogWrite(p_motorRightFwd, 0              );  break;
    case 9:                                           // Forward right
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorLeftFwd,  0              );
      analogWrite(p_motorRightBkd, turning_power_r);
      analogWrite(p_motorRightFwd, 0              );  break;
  }
}

void setup(){
  // Setup radio
  //#ifdef DEBUG
    Serial.begin(9600);
  //#endif
  // Setup pins
  pinMode(p_motorLeftFwd,  OUTPUT);
  pinMode(p_motorLeftBkd,  OUTPUT);
  pinMode(p_motorRightFwd, OUTPUT);
  pinMode(p_motorRightBkd, OUTPUT);
}

int testType = 1;

void loop(){
  if( testType == 1 ) {
    for( int i = 1; i < 10; i++ ) {
      //#ifdef DEBUG
        Serial.println( i );
      //#endif
      MotorDrive( i );
      delay(2000);
    }
  } else {
    /************************** MOVE MOTOR **************************/
    #ifdef DEBUG
      Serial.println("Moving forward");
    #endif
    MotorDrive( 6 );
    /**************************** DELAY *****************************/
    delay(2000);
    /************************** MOVE MOTOR **************************/
    #ifdef DEBUG
      Serial.println("Stopping");
    #endif
    MotorDrive( 5 );
    /**************************** DELAY *****************************/
    delay(2000);
  }
}









