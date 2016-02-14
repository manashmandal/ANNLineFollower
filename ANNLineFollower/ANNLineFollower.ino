//#define __BLUETOOTH_CONTROLLED__
//#define __LINE_FOLLOWER__
//
//#include <Motor.h>
//
//#include "LineFollower.h"
//
//#define RX 10
//#define TX 11
//#define BAUD 9600
//#define lspeed 180
//#define rspeed 180
//
//motorPins pins = {3, 5, 6, 9};
//
//Motor motor(pins);
//
//char command = 'g';
//
//
//
//void setup(void){
//  Serial.begin(BAUD);
//  bluetooth.begin(BAUD);
//  motor.begin();
//  bluetooth.println("Initialized");
//}
//
//void controlRobot(void){
//  if (bluetooth.available() > 0){
//    command = bluetooth.read();
//    switch (command){
//      case 'w':
//        motor.go(lspeed, rspeed, FORWARD);
//        Serial.println("Forward");
//        break;
//      case 's':
//        motor.go(lspeed, rspeed, BACKWARD);
//        Serial.println("Backward");
//        break;
//      case 'a':
//        motor.go(lspeed, rspeed, ANTICLOCKWISE);
//        Serial.println("Anticlockwise");
//        break;
//      case 'd':
//        motor.go(lspeed, rspeed, CLOCKWISE);
//        Serial.println("Clockwise");
//        break;
//      case 'r':
//        
//      
//      default:
//        motor.go(0, 0, NOWHERE);
//        Serial.println("Stop");
//        break;
//    }
//  }
//}
//
//#ifdef __BLUETOOTH_CONTROLLED__
//
//void loop(void){
//  controlRobot();
//}
//
//#else
//
//void loop(void){
//  
//}
//
//#endif


#include "LineFollower.h"

#define spd 180
#define THRESHOLD 300

uint leftMotorPins[] = {3, 5};
uint rightMotorPins[] = {6, 9};


uint sensors[] = {A7, A6, A5, A4, A3, A2, A1, A0};
uint sensors_less[] = {A5, A4, A3, A2};


Robot robot(leftMotorPins, rightMotorPins, sensors_less, 4);

void setup(){
  robot.begin();
//  robot.forward(spd, spd);
}

void loop() {
//  robot.debug(true);
//  robot.control_via_bluetooth();
  robot.differential_drive();
}
