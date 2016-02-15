//#define BLUETOOTH


#include "LineFollower.h"

#define spd 180
#define THRESHOLD 300
#define baud 19200

uint leftMotorPins[] = {3, 5};
uint rightMotorPins[] = {6, 9};


uint sensors[] = {A7, A6, A5, A4, A3, A2, A1, A0};
uint sensors_less[] = {A5, A4, A3, A2};


Robot robot(leftMotorPins, rightMotorPins, sensors_less, 4);

void setup(){
  robot.begin();
//  robot.forward(spd, spd);
}

#ifdef BLUETOOTH

void loop() {

  robot.control_via_bluetooth();
}

#else 

void loop() {robot.differential_drive();}

#endif
