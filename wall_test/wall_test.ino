/*
  wall_test.ino - Tests the profile for climbing up the wall.
  Created by Team Wall-R-Us, March 4, 2016.
*/

#include "wally.h"

Wally* wally;
XYZ acc;

void setup() {
  wally = new Wally();
  pinMode(13, OUTPUT);
  wally->waitButton();
}

void loop() {
  /* Drive Motors Back and Forth */
  if (wally->readIR()) {
    wally->setMotors(20, 20);
      acc = wally->readAccelerometer();
  } else {
//    delay(150);
//    wally->setMotorsFR(100, -100);
//    delay(75);
//    wally->setMotorsFR(100, 30);
    delay(2000);
    wally->setMotorsFR(0, 0);
    wally->waitButton();
  }
}
