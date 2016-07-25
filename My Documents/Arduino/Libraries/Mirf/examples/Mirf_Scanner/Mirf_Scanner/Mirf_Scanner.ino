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

const uint8_t num_channels = 126;
uint8_t values[num_channels];

void setup_channel(int channel) {
  Mirf.channel = channel;
  Mirf.init();
  Mirf.channel = channel;
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();
}

void setup(){
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  setup_channel(0);
  
  Serial.println("Listening..."); 
}

void loop(){
   
  byte data[Mirf.payload];

  for( int i=0; i<num_channels; i++) {
    Serial.print("Testing channel ");
    Serial.println(i);
    setup_channel(i);
    delay(100);
    if(Mirf.dataReady()){
      Serial.print("  Got packet on channel ");
      Serial.println(i);
      Mirf.getData(data);
      Mirf.setTADDR((byte *)"clie1");
      Mirf.send(data);
      Serial.println("Reply sent.");
    }
  }
}
