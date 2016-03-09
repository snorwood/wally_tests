/*
  control_test.ino - Test environment for control logic.
  Created by Team Wall-R-Us, March 4, 2016.
*/
#include "wally.h"
#include "wally_control.h"

Wally* wally;
WallyControl* wallyControl;

unsigned long start_time, loop_time;
const unsigned long TRIAL_TIME = 8e6;
int state;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  wally = new Wally();
  wallyControl = new WallyControl(wally);
  state = 0;
}

void loop() {
  /* Drive Motors Back and Forth */
  if (state == 0){
    digitalWrite(13, HIGH);
    delay(500);
    wally->waitButton();
    delay(500);
    wallyControl->begin();
    start_time = micros();
    state = 1;
  }
  
  loop_time = micros();
  if (state == 1)  {
    wallyControl->horizontalControl(loop_time, 0);

    if (wally->getOrientation(wally->readAccelerometer()) == UP){
      state = 0;
    }
  }
  
  if (state == 2) {
    digitalWrite(13, LOW);
    wallyControl->verticalControl(loop_time, 75, 0);
    delay(10000);
  } else {
    digitalWrite(13, HIGH);
  }
  
  if (wally->readButton() || (loop_time - start_time > TRIAL_TIME)){
    wally->stop();
    state = 0;
  }
}
