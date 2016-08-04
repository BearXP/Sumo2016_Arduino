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
  int x;
  int y;
  int cbut;
  int zbut;
  int receivingDataIndex;
} WiiChuckData;

WiiChuckData control = {};

void replyData(byte data) {
    Mirf.setTADDR((byte *)"clie1");
    Mirf.send(&data);
}

void SerialPrint(char input, int value) {
  Serial.print("Received:\t");
  Serial.print(input);
  Serial.print("\t@:\t");
  Serial.println(value);
}

void setup(){
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();
  
  Serial.println("Listening..."); 
}

void loop(){
  byte data[Mirf.payload];
  if(!Mirf.isSending() && Mirf.dataReady()){
    Mirf.getData(data);
    control.receivingDataIndex = (control.receivingDataIndex + 1) % 4;
    if( *data == 170 )
      control.receivingDataIndex = 0;
    else {
      switch (control.receivingDataIndex)
      {
        case 0: // x
          control.x = *data;
          SerialPrint('x', control.x);  break;
        case 1: // y
          control.y = *data;
          SerialPrint('y', control.y);  break;
        case 2: // c
          control.cbut = *data;
          SerialPrint('c', control.cbut);  break;
        case 3: // z
          control.zbut = *data;
          SerialPrint('z', control.zbut);  break;
      }
    }
    replyData(*data);
  }
  delay(1);
}
