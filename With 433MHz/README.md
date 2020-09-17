# GestureControl with 433MHz Transmitter Receiver Wireless module

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
Gesture Controlled 4 wheel drive which is connected with the RC using 433 MHz Transmitter Receiver module.
This project uses MPU6050 as an IMU. The IMU is attached to the transmitter end. Readings from the MPU is send over to the receiver end which will perform a directional motion based on the incoming readings.

## Technology
Project is created with:
* C/C++
* 433MHz Transmitter Receiver module
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
  * Wireless module (433MHz Transmitter Receiver)
* Initialization
```
#include <SPI.h>
#include "Wire.h"
#include "MPU.h"
#include <VirtualWire.h>
```
1. Create an object for MPU
  ```
  MPU mpuAccel(true);   //true will activate Accelerometer.
  MPU mpuGyro(false);   //false will activate Gyroscope.

  char axis[] = {'X','Y','Z'};
  float accel[3]; // For storing data from Accelerometer.
  float gyro[3];  // For storing data from Gyroscope.
  ```
2. Create package that is to be sent over to Receiver.
  ```
  struct package
  {
    int dir = 0;
  };
  typedef struct package Package;
  Package data;
  ```

* In void setup():<br>
1. Initialize MPU
  ```
  Wire.begin();
  mpuAccel.startMPU();
  mpuGyro.startMPU();
  ```
2. Initialize the 433MHz module
  ```
  vw_set_tx_pin(3);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);       // Bits per sec
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
    data.dir = 1;
    Serial.print("  Forward  ");
  }
  else if(accel[0]>=offSet){  //Resverse
    data.dir = 2;
    Serial.print("  Reverse   ");
  }
  else if(accel[1]>=offSet){  //Left or CCW
    data.dir = 3;
    Serial.print("  Left   ");
  }
  else if(accel[1]<=-1*offSet){   //Right or CW
    data.dir = 4;
    Serial.print("  Right   ");
  }
  else if(accel[0]>-1*offSet && accel[0]<offSet && accel[1]>-1*offSet && accel[1]<offSet){    //Stay
    data.dir = 5;
    Serial.print("  Stay   ");
  }
  ```
3. Send the package to receiver.
  ```
  vw_send((uint8_t *)&data, sizeof(data));
  vw_wait_tx(); // Wait until the whole message is gone
  ```

## Receiver
* Consists of:
  * AtMega328/MCU
  * Motor driver module
  * Wireless module (433MHz Transmitter Receiver)

* Initialization
```
#include <SPI.h>
#include "Wire.h"
#include "MPU.h"
#include <VirtualWire.h>
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
2. Create package that is to be sent over to Receiver.
  ```
  struct package
  {
    int dir = 0;
  };
  typedef struct package Package;
  Package data;
  ```

* In void setup():<br>
1. Initialize Motion
  ```
  motion.setupMotion();
  ```
2. Initialize the 433MHz module
  ```
  vw_set_rx_pin(49);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);       // Bits per sec
  ```
* In void loop():<br>
1. If Receiver is receiving
  ```
  uint8_t buf[sizeof(data)];
  uint8_t buflen = sizeof(data);

  if (vw_have_message())  // Is receiver receiving anything?
  {
    vw_get_message(buf, &buflen);
    memcpy(&data,&buf,buflen);    

    if(data.dir == 3)
      motion.forward();
    else if(data.dir == 4)
      motion.reverse();
    else if(data.dir == 1)
      motion.right();
    else if(data.dir == 2)
      motion.left();
    else if(data.dir == 5 || data.dir == 0)
      motion.stay();
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
  }
  ```
2. If not then stay
  ```  
  else{
    motion.stay();
  }
  ```
