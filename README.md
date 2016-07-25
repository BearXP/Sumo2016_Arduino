# Sumo2016_Arduino


DESIGN DESCRIPTION:
 This design has a remote control, separate to the actual robot.


REMOTE DESIGN DESCRIPTION:
 Driven using a wii remote.
 Wii remote is plugged in to an Arduino Uno, using the $1 adapters on ebay
  Wii remote clock -> A?
  Wii remote data  -> A?
  Wii remote Vpos  -> A?
  Wii remote Vneg  -> A?
 Data is parsed, and send to a nRF24L01 2.4GHz tranceiver module, and is sent using the MiRF library.


SUMO DESIGN DESCRIPTION:
 Receives the data from the remote.
 Rest is a secret.