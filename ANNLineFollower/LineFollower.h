#ifndef LINEFOLLOWER_H_
#define LINEFOLLOWER_H_

#include <Motor.h>

#define DEFAULT_THRESHOLD 350
#define DEFAULT_WEIGHT 1000
#define DEFAULT_SPEED 180
#define BAUD 9600
#define DEBUG_DELAY 1000

//Check if white lines on black or black on whites
#ifdef INVERSE_LOGIC
  #define INSIDE_LINE 0
  #define OUTSIDE_LINE 1
#else
  #define INSIDE_LINE 1
  #define OUTSIDE_LINE 0
#endif

#define RX 10
#define TX 11

#include <SoftwareSerial.h>

SoftwareSerial bluetooth(RX, TX);

class Robot {
  private:
    //All Motor
    Motor motors;

    //All sensor
    uint *sensors;
    uint numberOfSensors;

    //Placeholder for digital reading and analog reading
    uint *digitalReading;
    int *analogReading;

    int *thresholds;
    int threshold;
    bool singleThreshold;
    uint activeSensors;
    int weight;

    uint leftSpeed;
    uint rightSpeed;

    //far left and far right sensor
    uint sideSensors[2];
    uint sideAnalogReading[2];
    uint sideDigitalReading[2];

  public:
    Robot (uint *lm, uint *rm, uint *s, uint num);
    void begin (void);
    void read_line (void);
    void read_line (bool differentThresholds);
    int get_position (void);
    int get_position (bool differentThresholds);
    void differential_drive (void);
    void set_thresholds (int *t);
    void set_threshold (int t);
    void set_weight(int w);
    void debug(int _delay);
    void debug(bool via_bluetooth);
    void debug_with_different_thresholds(int _delay);
    void set_speed(uint lspeed, uint rspeed);

    //Side sensors
    void set_side_sensors(uint leftIr, uint rightIr);
    void side_read_line(void);
    int get_left_weight(void);
    int get_right_weight(void);

    //Controls
    void forward(uint lspeed, uint rspeed);
    void backward(uint lspeed, uint rspeed);
    void left(uint rspeed);
    void right(uint lspeed);
    void clockwise(uint lspeed, uint rspeed);
    void anticlockwise(uint lspeed, uint rspeed);
    void stop(void);
    void control_via_bluetooth(void);
};

Robot::Robot(uint *lm, uint *rm, uint *s, uint num) : singleThreshold(true) , activeSensors(0), threshold(DEFAULT_THRESHOLD), weight(DEFAULT_WEIGHT), leftSpeed(DEFAULT_SPEED), rightSpeed(DEFAULT_SPEED) {
  motors.setMotorPins(lm, rm);
  //Allocating memory
  sensors = new uint[num];
  digitalReading = new uint[num];
  analogReading = new int[num];
  thresholds = new int[num];
  numberOfSensors = num;
  for (uint i = 0; i < num; i++){
    sensors[i] = s[i];
  }
}

void Robot::begin(void){
  for (uint i = 0; i < numberOfSensors; i++) {
    pinMode(sensors[i], INPUT);
    digitalReading[i] = OUTSIDE_LINE;
    analogReading[i] = OUTSIDE_LINE;
  }
  motors.begin();
  Serial.begin(BAUD);
  bluetooth.begin(BAUD);
}

void Robot::read_line(void){
  activeSensors = 0;
  for (uint i = 0; i < numberOfSensors; i++){
    analogReading[i] = analogRead(sensors[i]);
    if (analogReading[i] > threshold) {
      digitalReading[i] = INSIDE_LINE;
      activeSensors++;
    } else digitalReading[i] = OUTSIDE_LINE;
  }
}

void Robot::read_line(bool differentThreshold){
  activeSensors = 0;
  if (differentThreshold){
    for (uint i = 0; i < numberOfSensors; i++){
      analogReading[i] = analogRead(sensors[i]);
      if (analogReading[i] > thresholds[i]){
        digitalReading[i] = INSIDE_LINE;
        activeSensors++;
      } else digitalReading[i] = OUTSIDE_LINE;
    }
  } else read_line();
}

int Robot::get_position(void){
  int weighted_value = 0;
  read_line();
  for (uint i = 0; i < numberOfSensors; i++){
    weighted_value += (i + 1) * weight * digitalReading[i];
  }
  return (weighted_value / activeSensors);
}

int Robot::get_position(bool differentThreshold){
  int weighted_value = 0;
  read_line(differentThreshold);
  for (uint i = 0; i < numberOfSensors; i++){
    weighted_value += (i + 1) * weight * digitalReading[i];
  }
  return (weighted_value / activeSensors);
}

void Robot::set_thresholds(int *t){
  for (uint i = 0; i < numberOfSensors; i++) thresholds[i] = t[i];
}

void Robot::set_threshold(int t) { threshold = t; }

void Robot::set_weight(int w) { weight = w; }

void Robot::debug(int _del = DEBUG_DELAY){
  read_line();
  side_read_line();
  Serial.println(" === ANALOG VALUE BEGIN ==== ");
  for (int i = 0; i < numberOfSensors; i++){
    Serial.println("IR [ " + String(i) + " ] = " + String(analogReading[i]));
  }
  Serial.println(" === ANALOG VALUE END === ");
  delay(_del);

  Serial.println(" === DIGITAL VALUE BEGIN === ");
  for (int i = 0; i < numberOfSensors; i++) Serial.print(String(digitalReading[i]) + " ");
  Serial.println();
  Serial.println(" === DIGITAL VALUE END === ");
  Serial.println("Position: " + String(get_position()));
  Serial.println(" === SIDE SENSOR BEGIN == ");
  for (int i = 0; i < 2; i++) Serial.println("Side IR [ " + String(i) + " ] = " + String(sideAnalogReading[i]));
  delay(_del);
}

void Robot::debug(bool via_bluetooth){
  int _del = DEBUG_DELAY;
  read_line();
  if (via_bluetooth){
    bluetooth.println(" === ANALOG VALUE BEGIN ==== ");
    for (int i = 0; i < numberOfSensors; i++){
      bluetooth.println("IR [ " + String(i) + " ] = " + String(analogReading[i]));
    }
    bluetooth.println(" === ANALOG VALUE END === ");
    delay(_del);
  
    bluetooth.println(" === DIGITAL VALUE BEGIN === ");
    for (int i = 0; i < numberOfSensors; i++) bluetooth.print(String(digitalReading[i]) + " ");
    bluetooth.println();
    bluetooth.println(" === DIGITAL VALUE END === ");
    bluetooth.println("Position: " + String(get_position()));
    delay(_del);
  } else {
    debug(_del);
  }
}

void Robot::debug_with_different_thresholds(int _del = DEBUG_DELAY){
  read_line(true);
  debug(_del);
}

void Robot::forward(uint lspeed, uint rspeed){
  motors.go(lspeed, rspeed, FORWARD);
}

void Robot::backward(uint lspeed, uint rspeed){
  motors.go(lspeed, rspeed, BACKWARD);
}

void Robot::left(uint rspeed){
  motors.go(0, rspeed, LEFT);
}

void Robot::right(uint lspeed){
  motors.go(lspeed, 0, RIGHT);
}

void Robot::clockwise(uint lspeed, uint rspeed){
  motors.go(lspeed, rspeed, CLOCKWISE);
}

void Robot::anticlockwise(uint lspeed, uint rspeed){
  motors.go(lspeed, rspeed, ANTICLOCKWISE);
}

void Robot::stop(void){
  motors.go(0, 0, NOWHERE);
}

void Robot::set_speed(uint lspeed, uint rspeed){
  leftSpeed = lspeed;
  rightSpeed = rspeed;
}

void Robot::control_via_bluetooth(void){
  char command = '~';
  if (bluetooth.available() > 0){
    command = bluetooth.read();
    switch(command){
      case 'w':
        forward(leftSpeed, rightSpeed);
        break;
      case 's':
        backward(leftSpeed, rightSpeed);
        break;
      case 'a':
        left(rightSpeed);
        break;
      case 'd':
        right(leftSpeed);
        break;
      case 'r':
        debug(true);
        break;
      default:
        stop();
        break;
    }
  }
}

void Robot::differential_drive(void){
  int pos = get_position();
  if (pos == 2500) forward(180, 180); // 0110
  else if (pos > 1500 && pos < 2500) left(90); //0100
  else if (pos > 1000 && pos <= 1500) left(95); //1100
  else if (pos > 500 && pos <= 1000) left(100); //1000
  else if (pos >= 3000 && pos < 3500) right(90); //0010
  else if (pos >= 3500 && pos < 4000) right(95); //0011
  else if (pos >= 4000 && pos < 5000) right(100); //0001
  else if (pos < 0) stop();
}


void Robot::set_side_sensors(uint leftIr, uint rightIr){
  sideSensors[0] = leftIr;
  sideSensors[1] = rightIr;
  for (uint i = 0; i < 2; i++) pinMode(sideSensors[i], INPUT);
}

void Robot::side_read_line(void){
  for (uint i = 0; i < 2; i++){
    sideAnalogReading[i] = analogRead(sideSensors[i]);
    if (sideAnalogReading[i] > threshold) sideDigitalReading[i] = INSIDE_LINE;
    else sideDigitalReading[i] = OUTSIDE_LINE;
  }
}

#endif