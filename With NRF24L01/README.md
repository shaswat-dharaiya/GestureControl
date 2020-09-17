# GestureControl with NRF24L01 Transreceiver Wireless module

## Table of contents
* [General info](#general-info)
* [Technology](#technology)
* [Features](#features)
* [Initialization](#initialization)
* [To-Do](#to-Do)
* [Libraries](#libraries)
* [Transmitter](#transmitter)
* [Receiver](#receiver)


## General info
Gesture Controlled 4 wheel drive which is connected with the RC using NRF24L01 Transreceiver Wireless module.
This project uses MPU6050 as an IMU. The IMU is attached to the transmitter end. Readings from the MPU is send over to the receiver end which will perform a directional motion based on the incoming readings.

## Technology
Project is created with:
* C/C++
* NRF24L01 Transreceiver Wireless module
* MPU6050

## Features
* Gesture Control
* Wireless communication

## To-Do
* Move forward
* Move reverse
* Move left
* Move right
* Stay/Stop Motion.

## Libraries
* [MPU](https://github.com/shaswat-dharaiya/Arduino-Libraries/tree/master/MPU)
* [Motion](https://github.com/shaswat-dharaiya/Arduino-Libraries/tree/master/Motion)

## Transmitter
* Consists of:
  * AtMega328/MCU
  * MPU6050
  * Wireless module (NRF24L01 Transreceiver Wireless module)
* Initialization
```
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include "MPU.h"
```
1. Create an object for MPU
  ```
  MPU mpuAccel(true);   //true will activate Accelerometer.
  MPU mpuGyro(false);   //false will activate Gyroscope.

  char axis[] = {'X','Y','Z'};
  float accel[3]; // For storing data from Accelerometer.
  float gyro[3];  // For storing data from Gyroscope.
  ```
2. Initialize RF24 & create const byte for channel selection.
  ```
  RF24 radio(7,8);

  const byte rxAddr[6] = "00001";
  ```

* In void setup():<br>
1. Initialize MPU
  ```
  Wire.begin();
  mpuAccel.startMPU();
  mpuGyro.startMPU();
  ```
2. Initialize the NRF24L01 module
  ```
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
  ```
* In void loop():<br>
1. Read and store data from MPU
  ```
  mpuAccel.startReadMPU();
  mpuGyro.startReadMPU();

  mpuAccel.readMPU(accel);  store Accelerometer readings in float accel[]

  // I have not used Gyroscope readings but I still called it for future purpose.

  mpuGyro.readMPU(gyro);  store Gyroscope readings in float gyro[]
  ```
2. Decode the Accelerometer readings & store it in the Package
  ```  
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
  ```
3. Send the package to receiver.
  ```
  radio.write(&x, sizeof(x));
  ```

## Receiver
* Consists of:
  * AtMega328/MCU
  * Motor driver module
  * Wireless module (NRF24L01 Transreceiver Wireless module)

* Initialization
```
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include "Motion.h";
```
1. Create an object for MPU
  ```
  //Use your own pins
  int motxf = 22;
  int motxr = 24;
  int motyf = 26;
  int motyr = 28;

  int motx = 2;
  int moty = 3;

  // Create an object of Motion
  Motion motion(motx,moty,motxf,motyf,motxr,motyr);
  ```
2. Initialize RF24 & create const byte for channel selection.
  ```
  RF24 radio(7,8);

  const byte rxAddr[6] = "00001";
  ```

* In void setup():<br>
1. Initialize Motion
  ```
  motion.setupMotion();
  ```
2. Initialize the NRF24L01 module
  ```
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();
  ```
* In void loop():<br>
1. If Receiver is receiving
  ```
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
  ```
