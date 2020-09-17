/*
 * This project is created by Shaswat Dhariaya
 * It uses VirtualWire
 * Is uses 433MHz transmitter receiver wireless module
 *
 * This part of the code is for transmitter.
 */
#include <SPI.h>
#include <VirtualWire.h>
#include <Wire.h>
#include "Motion.h";

int motxf = 22;
int motxr = 24;
int motyf = 26;
int motyr = 28;

int motx = 2;
int moty = 3;

Motion motion(motx,moty,motxf,motyf,motxr,motyr);

struct package
{
  int dir = 0;
};

typedef struct package Package;
Package data;

void setup()
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);

  pinMode(48,OUTPUT);   //VCC
  digitalWrite(48,HIGH);

  Serial.println("Initialization..");

  motion.setupMotion();

  vw_set_rx_pin(49);
  vw_setup(2000);   // Bits per sec
  vw_rx_start();
}

void loop()
{
  uint8_t buf[sizeof(data)];
  uint8_t buflen = sizeof(data);

  if (vw_have_message())  // Is there a packet for us?
  {
    vw_get_message(buf, &buflen);
    memcpy(&data,&buf,buflen);
    Serial.println(data.dir);
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
  else{
    motion.stay();
  }
  delay(5);
}


void reverse(){
    digitalWrite(motxf, HIGH);
    digitalWrite(motyf, HIGH);
    analogWrite(motx, 1023);
    delay(2);
    analogWrite(moty, 1023);
    delay(2);
    digitalWrite(motxr, LOW);
    digitalWrite(motyr, LOW);
    Serial.println("Reverse");

}
void forward(){
    digitalWrite(motxr, HIGH);
    digitalWrite(motyr, HIGH);
    analogWrite(moty, 1023);
    delay(2);
    analogWrite(motx, 1023);
    delay(2);
    digitalWrite(motxf, LOW);
    digitalWrite(motyf, LOW);
    Serial.println("Forward");
}
void right(){
    digitalWrite(motxf, LOW);
    digitalWrite(motyf, HIGH);
    analogWrite(motx, 1023);
    delay(2);
    analogWrite(moty, 1023);
    delay(2);
    digitalWrite(motxr, HIGH);
    digitalWrite(motyr, LOW);
    Serial.println("Right");
}
void left(){
    digitalWrite(motxf, HIGH);
    digitalWrite(motyf, LOW);
    analogWrite(motx, 1023);
    delay(2);
    analogWrite(moty, 1023);
    delay(2);
    digitalWrite(motxr, LOW);
    digitalWrite(motyr, HIGH);
    Serial.println("Left");
}
void stay(){
    digitalWrite(motxf, LOW);
    digitalWrite(motyf, LOW);
    analogWrite(moty, 0);
    delay(2);
    digitalWrite(motxr, LOW);
    digitalWrite(motyr, LOW);
    analogWrite(motx, 0);
    delay(2);
    Serial.println("Stay");
}
