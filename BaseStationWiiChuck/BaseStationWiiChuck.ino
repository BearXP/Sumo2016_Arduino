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
 * nRF24L01 - CSN -> 7
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
    Mirf.cePin = 7;
    Mirf.csnPin = 8;
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"clie1");   // Config receiving address
    Mirf.payload = sizeof(byte); // Payload must be the same
    Mirf.channel = 10;
    Mirf.config();
    
    Serial.print("WiiChuckDemo ready\n");
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
  Mirf.send((byte *)170   );   while(Mirf.isSending());
  Mirf.send((byte *)&y   );   while(Mirf.isSending());
  Mirf.send((byte *)&y   );   while(Mirf.isSending());
  Mirf.send((byte *)&zbut);   while(Mirf.isSending());
  Mirf.send((byte *)&cbut);   while(Mirf.isSending());
  // Send over serial
  Serial.print("X: ");      Serial.print(  x,   DEC);
  Serial.print("\ty: ");    Serial.print(  y,   DEC);
  Serial.print("\tzbut: "); Serial.print(  zbut,DEC);
  Serial.print("\tcbut: "); Serial.println(cbut,DEC);
  // Delay
  delay(100);
}
