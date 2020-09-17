/*
 * This project is created by Shaswat Dhariaya
 * It uses VirtualWire
 * Is uses NRF24L01 Transreceiver wireless module
 *
 * This part of the code is for receiver.
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include "Motion.h";

RF24 radio(7,8);

const byte rxAddr[6] = "00001";

int motxf = 24;
int motxr = 25;
int motyf = 28;
int motyr = 29;

int motx = 2;
int moty = 3;

Motion motion(motx,moty,motxf,motyf,motxr,motyr);

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  //radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  motion.setupMotion();
}

void loop()
{

  if(radio.available())
  {
    int x = {0};
    radio.read(&x, sizeof(x));
//    Serial.println(x);

    if(x == 1)
      motion.forward(true);
    else if(x == 2)
      motion.reverse(true);
    else if(x == 3)
      motion.left(true);
    else if(x == 4)
      motion.right(true);
    else if(x == 5)
      motion.stay(true);
   }
 delay(5);
}
