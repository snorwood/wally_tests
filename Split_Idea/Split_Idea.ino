#include <rolling_median.h>
#include <wally.h>
#include <wally_control.h>

Wally* wally; // Initialize wally robot
WallyControl* wally_c;

double front_reading = 201;
double left_reading = 201;
double right_reading = 201;
double left_speed = 25;
double right_speed = 25;
int delay_speed = 30;
int turn_speed = 30;
int forwards_speed = 30;
double max_speed = 40;
double min_speed = 10;
int right_turn_delay = 400;
int left_turn_delay = 400;
int base_speed = 40;
double Kd = 0.7;
double diff_left;
double diff_right;

void setup() {
  /* Wait for Button */
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  wally = new Wally();
  wally->waitButton();
  wally_c = new WallyControl(wally, 1, 1, 1, 3);
  wally_c->begin();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  wally_c->update();
  left_reading = wally_c->readUltrasonic(2);
  right_reading = wally_c->readUltrasonic(1);
  
}

double differentialLeft(double reading) {
  // negative is away
  // positive is towards
//  if(reading == 201.00) {
//    return left_reading;
//  }
  double value = left_reading - reading;
  left_reading = reading;
  return value;
}

double differentialRight(double reading) {
  // negative is away
  // positive is towards
//  if(reading == 201.00) {
//    return right_reading;
//  }
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
  return (int)(value / speed * 25);
}

int notCloseToBase() {
  wally_c->update();
//  Serial.print("1");
  if(wally_c->readUltrasonic(0) < 15.00) {
    return 0;
  }
  return 1;
}

void loop() {
  wally->setMotors(left_speed,right_speed);
  while(notCloseToBase()) {
    diff_left = differentialLeft(wally_c->readUltrasonic(2));
    diff_right = differentialRight(wally_c->readUltrasonic(1));
//    wally_c->update();
    if(diff_left > 40) {
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
      wally->setMotors(forwards_speed,forwards_speed);
      while(wally->getOrientation(wally_c->readAccelerometer()) == FLAT) {
        wally_c->update();
      }
      while(wally->getOrientation(wally_c->readAccelerometer()) != FLAT) {
        wally_c->update();
      }
      wally->setMotors(0,0);
      wally->waitButton();
    }
    if(diff_right > 40) {
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
      wally->setMotors(forwards_speed,forwards_speed);
      while(wally->getOrientation(wally_c->readAccelerometer()) == FLAT) {
        wally_c->update();
      }
      while(wally->getOrientation(wally_c->readAccelerometer()) != FLAT) {
        wally_c->update();
      }
      wally->setMotors(0,0);
      wally->waitButton();
    }
//    if(differentialRight(wally_c->readUltrasonic(1)) > 40) {
//      wally->setMotors(delay_speed,delay_speed);
//      delay(delayTime(wally_c->readUltrasonic(1),delay_speed));
//      wally->setMotors(0,0);
//      delay(100);
//      wally->setMotors(turn_speed,-turn_speed);
//      delay(right_turn_delay);
//      wally->setMotors(0,0);
//      delay(100);
//
//      wally->setMotors(forwards_speed,forwards_speed);
//      while(notCloseToBase()) {
////        wally_c->update(); 
//      }
//      wally->setMotors(0,0);
//      wally->waitButton();
//    }
//    if(diff < 0) {
//      if(left_speed > 10) {
//        left_speed = left_speed - diff * Kd;
//        right_speed = right_speed - diff * Kd;
//      }
//      Serial.println("Correcting to Left");
//      Serial.println(diff);
//      Serial.print(left_speed);
//      Serial.print(" <-> ");
//      Serial.println(right_speed);
//    }
//    if(diff > 0) {
//      if(right_speed > 10) {
//        left_speed = left_speed + diff * Kd;
//        right_speed = right_speed - diff * Kd;
//      }
//      Serial.println("Correcting to Right");
//      Serial.println(diff);
//      Serial.print(left_speed);
//      Serial.print(" <-> ");
//      Serial.println(right_speed);
//    }
    
    left_speed = left_speed + diff_left * Kd;
    right_speed = right_speed - diff_left * Kd;
    if(left_speed > max_speed) {
      left_speed = max_speed;
    }
    if(left_speed < min_speed) {
      left_speed = min_speed;
    }
    if(right_speed > max_speed) {
      right_speed = max_speed;
    }
    if(right_speed < min_speed) {
      right_speed = min_speed;
    }
    wally->setMotors(left_speed,right_speed);
  }
  wally->setMotors(forwards_speed,forwards_speed);
  while(wally->getOrientation(wally_c->readAccelerometer()) == FLAT) {
    wally_c->update();
  }
  while(wally->getOrientation(wally_c->readAccelerometer()) != FLAT) {
    wally_c->update();
  }
  wally->setMotors(0,0);
  wally->waitButton();
}
