/**
 * WiiChuckDemo -- 
 * 2008 Tod E. Kurt, http://thingm.com/
 * An Mirf example which copies back the data it recives.
 *
 * Pins:
 * nRF24L01 - MISO -> 12
 * nRF24L01 - MOSI -> 11
 * nRF24L01 - SCK  -> 13
 * WiiChuck - Vneg -> A2
 * WiiChuck - Vpos -> A3
 * WiiChuck - Data -> A4
 * WiiChuck - Clk  -> A5
 *
 * Configurable:
 * nRF24L01 - CE -> 8
 * nRF24L01 - CSN -> 9
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <Wire.h>
#include "nunchuck_funcs.h"

int loop_cnt=0;

byte x,y,zbut,cbut;

void setup()
{
    //Serial.begin(19200);
    Serial.begin(9600);
    // Setup Nunchuck
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
    // Setup nRF24L01
    //Mirf.cePin = 8;
    //Mirf.csnPin = 9;
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"clie1");   // Config receiving address
    Mirf.payload = sizeof(unsigned long); // Payload must be the same
    //Mirf.channel = 10;
    Mirf.config();
}

void send_data(unsigned long data) {
  Mirf.setTADDR((byte *)"serv1");
  Mirf.send((byte *)&data);
  while(Mirf.isSending()){
  }
  delay(3);
  unsigned long time = millis();
  while(!Mirf.dataReady()){
    if ( ( millis() - time ) > 1000 ) {
      Serial.println("Timeout on response from server!");
      return;
    }
    delay(1);
  }
  byte data_returned;
  Mirf.getData((byte *)&data_returned);
}


void loop()
{
  // Update the nunchuck
  nunchuck_get_data();
  x =    (byte) nunchuck_joyx();
  y =    (byte) nunchuck_joyy();
  zbut = (byte) nunchuck_zbutton();
  cbut = (byte) nunchuck_cbutton();
  // Send the data
  send_data(((byte)(x/10))*10+1   );  delay(2);
  send_data(((byte)(y/10))*10+2   );  delay(2);
  send_data(cbut*10          +3   );  delay(2);
  send_data(zbut*10          +4   );  delay(2);
  // Delay
  delay(40);
}


