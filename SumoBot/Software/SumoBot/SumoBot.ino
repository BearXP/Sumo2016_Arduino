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
#define DEBUG


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
  p_motorLeftFwd = 3,
  p_motorLeftBkd = 5,
  p_motorRightFwd = 10,
  p_motorRightBkd = 6
};

//WiiChuckData control = {};

void replyData(byte data) {
    Mirf.setTADDR((byte *)"clie1");
    Mirf.send(&data);
}

void SerialPrint(char input, int value) {
  return;
  #ifdef DEBUG
    Serial.print("Received:\t");
    Serial.print(input);
    Serial.print("\t@:\t");
    Serial.println(value);
  #endif
}
  int lastDir;
  
void MotorDrive(int dir) {
  const byte full_power_l = 225;
  const byte full_power_r = 255;
  const byte turning_power_l = full_power_l/2;
  const byte turning_power_r = full_power_r/2;


  if( dir != lastDir ) {
    lastDir = dir;
    Serial.println(dir);
  }
  
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
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorLeftBkd,  0              );
      analogWrite(p_motorRightFwd, 0              );
      analogWrite(p_motorRightBkd, full_power_r   );  break;
    case 5:                                           // No Movement
      analogWrite(p_motorLeftFwd,  full_power_l   );
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorRightFwd, full_power_r   );
      analogWrite(p_motorRightBkd, full_power_r   );  break;
    case 6:                                           // Hard right
      analogWrite(p_motorLeftFwd,  0              );
      analogWrite(p_motorLeftBkd,  full_power_l   );
      analogWrite(p_motorRightFwd, full_power_r   );
      analogWrite(p_motorRightBkd, 0              );  break;
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
  #ifdef DEBUG
	  Serial.begin(9600);
    Serial.println("Listening..."); 
  #endif
  // Setup radio
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();
  // Setup pins
  pinMode(p_motorLeftFwd,  OUTPUT);
  pinMode(p_motorLeftBkd,  OUTPUT);
  pinMode(p_motorRightFwd, OUTPUT);
  pinMode(p_motorRightBkd, OUTPUT);
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
        SerialPrint(' x', controller.x);  break;
      case 2: // y
        controller.y = *data / 10;
        SerialPrint('  y', controller.y);  break;
      case 3: // c
        controller.cbut = *data / 10;
        SerialPrint('   c', controller.cbut);  break;
      case 4: // z
        controller.zbut = *data / 10;
        SerialPrint('    z', controller.zbut);  break;
    }
    replyData(*data);
  }
  /********************** CONTROLLER TIMEOUT **********************/
  if( controller.timeLastReceived < timeNow - 5000 ){
    controller.receivingDataIndex = 0;
    controller.allFieldsReceived = false;
    #ifdef DEBUG
      Serial.print("-----TIMEOUT---- Last Rec: ");
      Serial.print(controller.timeLastReceived, DEC);
      Serial.print("\tNow: ");
      Serial.println(timeNow, DEC);
    #endif
  }
  /************************** MOVE MOTOR **************************/
  if( controller.allFieldsReceived ){
	  int x_dir = min( max( map(controller.x+2, 10, 16, -1, 1), -1), 1);
	  int y_dir = min( max( map(controller.y+2, 10, 16, -1, 1), -1), 1);
	  MotorDrive( x_dir + 3*y_dir + 5 );
  } else {
	  MotorDrive(5);
  }
  /**************************** DELAY *****************************/
  delay(1);
}
