/*
  control_test.ino - Test environment for control logic.
  Created by Team Wall-R-Us, March 4, 2016.
*/
#include "wally.h"
#include "wally_control.h"

Wally* wally;
WallyControl* wallyControl;

unsigned long start_time, loop_time;
const unsigned long TRIAL_TIME = 5e6;

void setup() {
  Serial.begin(115200);
  wally = new Wally();
  wallyControl = new WallyControl(wally);
  wally->waitButton();
  delay(1000);
  wallyControl->begin();
  start_time = micros();
}

void loop() {
  /* Drive Motors Back and Forth */
  loop_time = micros();
  wallyControl->verticalControl();
  if ((loop_time - start_time > TRIAL_TIME) || wally->readButton()){
    wally->stop();
    delay(1000); 
    setup();
  }
}
