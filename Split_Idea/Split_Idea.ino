#include <rolling_median.h>
#include <wally.h>
#include <wally_control.h>

Wally* wally; // Initialize wally robot
WallyControl* wally_c;

double front_reading = 201;
double left_reading = 201;
double right_reading = 201;
int left_speed = 20;
int right_speed = 20;
int delay_speed = 20;
int turn_speed = 30;
int forwards_speed = 30;
int right_turn_delay = 350;
int left_turn_delay = 350;

void setup() {
  /* Wait for Button */
  wally->waitButton();
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  wally = new Wally();
  wally_c = new WallyControl(wally, 1, 1, 1, 5);
  wally_c->begin();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
}

double differentialLeft(double reading) {
  // negative is away
  // positive is towards
  double value = left_reading - reading;
  left_reading = reading;
  return value;
}

double differentialRight(double reading) {
  // negative is away
  // positive is towards
  double value = right_reading - reading;
  right_reading = reading;
  return value;
}

double differentialFront(double reading) {
  // negative is away
  // positive is towards
  double value = front_reading - reading;
  front_reading = reading;
  return value;
}

int delayTime(double reading, int speed) {
  int value = (int)reading;
  return value / speed * 100;
}

int notCloseToBase() {
  wally_c->update();
  if(wally_c->readUltrasonic(0) < 15) {
    return 0;
  }
  return 1;
}

void loop() {
  wally->setMotors(left_speed,right_speed);
  while(notCloseToBase()) {
//    wally_c->update();
    if(differentialLeft(wally_c->readUltrasonic(2)) > 30) {
      wally->setMotors(delay_speed,delay_speed);
      delay(delayTime(wally_c->readUltrasonic(2),delay_speed));
      wally->setMotors(0,0);
      delay(100);
      wally->setMotors(-turn_speed,turn_speed);
      delay(left_turn_delay);
      wally->setMotors(0,0);
      delay(100);

      wally->setMotors(forwards_speed,forwards_speed);
      while(notCloseToBase()) {
//        wally_c->update(); 
      }
      wally->setMotors(0,0);
      wally->waitButton();
    }
    if(differentialRight(wally_c->readUltrasonic(1)) > 30) {
      wally->setMotors(delay_speed,delay_speed);
      delay(delayTime(wally_c->readUltrasonic(1),delay_speed));
      wally->setMotors(0,0);
      delay(100);
      wally->setMotors(turn_speed,-turn_speed);
      delay(right_turn_delay);
      wally->setMotors(0,0);
      delay(100);

      wally->setMotors(forwards_speed,forwards_speed);
      while(notCloseToBase()) {
//        wally_c->update(); 
      }
      wally->setMotors(0,0);
      wally->waitButton();
    }
    if(differentialLeft(wally_c->readUltrasonic(2)) < 0) {
      if(left_speed > 10) {
        left_speed = left_speed - 1;
        right_speed = right_speed + 1;
      }
    }
    if(differentialLeft(wally_c->readUltrasonic(2)) > 0) {
      if(right_speed > 10) {
        left_speed = left_speed + 1;
        right_speed = right_speed - 1;
      }
    }
    wally->setMotors(left_speed,right_speed);
  }
}
