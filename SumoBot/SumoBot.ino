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

void setup(){
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = sizeof(byte);
  Mirf.config();
  
  Serial.println("Listening..."); 
}

void loop(){
  byte data[Mirf.payload];
  if(!Mirf.isSending() && Mirf.dataReady()){
    Serial.println("Got packet");
    Mirf.getData(data);
    if( data[0] = (byte)170 )
      control.receivingDataIndex = 0;
    else {
      switch (control.receivingDataIndex)
      {
        case 0: // x
          control.x = data[0];       break;
        case 1: // y
          control.y = data[0];       break;
        case 2: // c
          control.cbut = data[0];    break;
        case 3: // z
          control.zbut = data[0];    break;
      }
    }
  }
}
