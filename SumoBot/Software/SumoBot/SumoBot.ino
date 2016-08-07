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

typedef enum Pins {
  arm = 10
} pins;

//WiiChuckData control = {};

void replyData(byte data) {
    Mirf.setTADDR((byte *)"clie1");
    Mirf.send(&data);
}

void SerialPrint(char input, int value) {
  //return;
  Serial.print("Received:\t");
  Serial.print(input);
  Serial.print("\t@:\t");
  Serial.println(value);
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
  /*************** GET DATA FROM CONTROLLER ***********************/
  byte data[Mirf.payload];
  if( !Mirf.isSending() && Mirf.dataReady() ){
    // Reset timer
    controller.timeLastReceived = millis();
    Mirf.getData(data);
    controller.receivingDataIndex = controller.receivingDataIndex + 1;
    if( controller.receivingDataIndex > 4 )
      controller.allFieldsReceived = true;
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
  /*************** CONTROLLER TIMEOUT ***********************/
  if( controller.timeLastReceived + 1000 < millis() ){
    //controller.receivingDataIndex = 0;
    //controller.allFieldsReceived = false;
  }
  /******************* SEND DATA TO SPINNER ***********************/
  if( controller.allFieldsReceived ){
    if( controller.cbut ){
      digitalWrite(10, HIGH);
      Serial.println("Writing high to pin: 10");
      //Serial.println(pins.arm, DEC);
    }
    else {
      analogWrite(10, 0);
    }
  }
  else {
    analogWrite(10, 0);
  }
  
  /******************* DELAY ***********************/
  delay(1);
}
