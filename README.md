# GestureControl

## Table of contents
* [General info](#general-info)
* [Technology](#technology)
* [Features](#features)
* [Initialization](#initialization)
* [To-Do](#to-Do)
* [Transmitter](#transmitter)
* [Receiver](#receiver)


## General info
Gesture Controlled 4 wheel drive which is connected with the RC using 433 MHz Transmitter Receiver module or NRF24L01 Transreceiver module.
This project uses MPU6050 as an IMU. The IMU is attached to the transmitter end. Readings from the MPU is send over to the receiver end which will perform a directional motion based on the incoming readings.

## Technology
Project is created with:
* C/C++
* 433MHz Transmitter Receiver module
* NRF24L01 Transreceiver module
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

## Transmitter
* Consists of:
  * AtMega328/MCU
  * MPU6050
  * Wireless module (433MHz Transmitter Receiver,NRF24L01 Transreceiver module)

## Receiver
* Consists of:
  * AtMega328/MCU
  * Motor driver module
  * Wireless module (433MHz Transmitter Receiver,NRF24L01 Transreceiver module)
