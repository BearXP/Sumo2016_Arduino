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

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

typedef struct {
  byte x;
  byte y;
  byte cbut;
  byte zbut;
  unsigned long int receivingDataIndex;
  bool allFieldsReceived;
  unsigned long int timeLastReceived;
} WiiChuckData;
WiiChuckData controller;

enum Pins {
  p_arm = 10,
  p_motorLeftFwd = 11,
  p_motorLeftBkd = 12,
  p_motorRightFwd = 11,
  p_motorRightBkd = 12
};

//WiiChuckData control = {};

void replyData(byte data) {
    Mirf.setTADDR((byte *)"clie1");
    Mirf.send(&data);
}

void SerialPrint(char input, int value) {
  return;
  Serial.print("Received:\t");
  Serial.print(input);
  Serial.print("\t@:\t");
  Serial.println(value);
}

void MotorDrive(int dir) {
  const byte full_power_l = 255;
  const byte full_power_r = 255;
  const byte turning_power_l = full_power_l/2;
  const byte turning_power_r = full_power_r/2;
  switch( max(min(dir,9),0) ) {
    case 1:                                           // Back left
      analogWrite(p_motorLeftFwd,  turning_power_l);
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorRightFwd, 0              );
      analogWrite(p_motorRightBkd, full_power_r   );  break;
    case 2:                                           // Back
      analogWrite(p_motorLeftFwd,  0              );
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorRightFwd, 0              );
      analogWrite(p_motorRightBkd, full_power_r   );  break;
    case 3:                                           // Back right
      analogWrite(p_motorLeftFwd,  0              );
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorRightFwd, turning_power_r);
      analogWrite(p_motorRightBkd, full_power_r   );  break;
    case 4:                                           // Hard left
      analogWrite(p_motorLeftFwd,  0              );
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorRightFwd, full_power_r   );
      analogWrite(p_motorRightBkd, 0              );  break;
    case 5:                                           // No Movement
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorRightFwd, full_power_r   );
      analogWrite(p_motorRightBkd, full_power_r   );  break;
    case 6:                                           // Hard right
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorLeftBkd,  0              );
      analogWrite(p_motorRightFwd, 0              );
      analogWrite(p_motorRightBkd, full_power_r   );  break;
    case 7:                                           // Forward left
      analogWrite(p_motorLeftFwd,  turning_power_l);
      analogWrite(p_motorLeftBkd,  0              );
      analogWrite(p_motorRightFwd, full_power_r   );
      analogWrite(p_motorRightBkd, 0              );  break;
    case 8:                                           // Forward
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorLeftBkd,  0              );
      analogWrite(p_motorRightFwd, full_power_r   );
      analogWrite(p_motorRightBkd, 0              );  break;
    case 9:                                           // Forward right
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorLeftBkd,  0              );
      analogWrite(p_motorRightFwd, turning_power_r);
      analogWrite(p_motorRightBkd, 0              );  break;
  }
}

void setup(){
  // Setup radio
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();
  // Setup pins
  pinMode(10, OUTPUT);
  Serial.println("Listening..."); 
}

void loop(){
  /******************* GET DATA FROM CONTROLLER *******************/
  byte data[Mirf.payload];
  unsigned long int timeNow = millis()+5000;
  if( !Mirf.isSending() && Mirf.dataReady() ){
    // Reset timer
    controller.timeLastReceived = timeNow;
    Mirf.getData(data);
    controller.receivingDataIndex = controller.receivingDataIndex + 1;
    if( controller.receivingDataIndex > 4 ) {
      controller.allFieldsReceived = true;
    }
    // Get the LSB
    switch ( *data - ((byte)(*data/10))*10 )
    {
      case 1: // x
        controller.x = *data / 10;
        SerialPrint('x', controller.x);  break;
      case 2: // y
        controller.y = *data / 10;
        SerialPrint('y', controller.y);  break;
      case 3: // c
        controller.cbut = *data / 10;
        SerialPrint('c', controller.cbut);  break;
      case 4: // z
        controller.zbut = *data / 10;
        SerialPrint('z', controller.zbut);  break;
    }
    replyData(*data);
  }
  /********************** CONTROLLER TIMEOUT **********************/
  if( controller.timeLastReceived < timeNow - 5000 ){
    controller.receivingDataIndex = 0;
    controller.allFieldsReceived = false;
    Serial.print("-----TIMEOUT---- Last Rec: ");
    Serial.print(controller.timeLastReceived, DEC);
    Serial.print("\tNow: ");
    Serial.println(timeNow, DEC);
  }
  /********************* SEND DATA TO SPINNER *********************/
  if( controller.allFieldsReceived ){
    if( controller.cbut ){
      analogWrite(p_arm, 255);
    }
    else {
      analogWrite(p_arm, 0);
    }
  }
  else {
    analogWrite(p_arm, 0);
  }
  /************************** MOVE MOTOR **************************/
  int x_dir = map(controller.x, 0, 25, -1, 1);
  int y_dir = map(controller.y, 0, 25, -1, 1);
  MotorDrive( x_dir + 3*y_dir + 5 );
  /**************************** DELAY *****************************/
  delay(1);
}
