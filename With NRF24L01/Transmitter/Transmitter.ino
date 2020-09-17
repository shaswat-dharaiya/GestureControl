/*
 * This project is created by Shaswat Dhariaya
 * It uses VirtualWire
 * Is uses NRF24L01 Transreceiver wireless module
 *
 * This part of the code is for transmitter.
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include "MPU.h"

RF24 radio(7,8);

const byte rxAddr[6] = "00001";

MPU mpuAccel(true);   //true will activate Accelerometer.
MPU mpuGyro(false);   //false will activate Gyroscope.

char axis[] = {'X','Y','Z'};
float accel[3];
float gyro[3];

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  //radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Wire.begin();
  mpuAccel.startMPU();
  mpuGyro.startMPU();
}

void loop()
{
  mpuAccel.startReadMPU();
  mpuGyro.startReadMPU();

  mpuAccel.readMPU(accel);
  mpuGyro.readMPU(gyro);
  delay(5);
  int x = 0;
  float offSet = 0.65;  //Adjust the offSet as per your needs.
                        //Also adjust the Accelerometer axis by using different index of accel.
  if(accel[0]<=-1*offSet){ //Forward
    x = 1;

  else if(accel[0]>=offSet)  //Reverse
    x = 2;

  else if(accel[1]>=offSet)  //Left or CCW
    x = 3;

  else if(accel[1]<=-1*offSet)   //Right or CW
    x = 4;

  else if(accel[0]>-1*offSet && accel[0]<offSet && accel[1]>-1*offSet && accel[1]<offSet)    //Stay
    x= 5;

    radio.write(&x, sizeof(x));
}
