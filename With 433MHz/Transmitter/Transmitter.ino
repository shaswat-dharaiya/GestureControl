/*
 * This project is created by Shaswat Dhariaya
 * It uses VirtualWire
 * Is uses 433MHz Transmitter Receiver wireless module
 *
 *
 * This part of the code is for transmitter.
 */
#include <SPI.h>
#include "Wire.h"
#include "MPU.h"
#include <VirtualWire.h>

MPU mpuAccel(true);   //true will activate Accelerometer.
MPU mpuGyro(false);   //false will activate Gyroscope.

char axis[] = {'X','Y','Z'};
float accel[3];
float gyro[3];

struct package
{
  int dir = 0;
};
typedef struct package Package;
Package data;

void setup()
{
  pinMode(2,OUTPUT);  //Acts as VCC
  digitalWrite(2,HIGH);

  Serial.begin(9600);
  Serial.println("Initializing..");
  pinMode(13,OUTPUT);

  Wire.begin();
  mpuAccel.startMPU();
  mpuGyro.startMPU();

  vw_set_tx_pin(3);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);       // Bits per sec
}
void loop()
{
  mpuAccel.startReadMPU();
  mpuGyro.startReadMPU();

  mpuAccel.readMPU(accel);
  mpuGyro.readMPU(gyro);

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

  Serial.println("Data: "+String(data.dir));
  vw_send((uint8_t *)&data, sizeof(data));
  vw_wait_tx(); // Wait until the whole message is gone
  delay(5);
}
