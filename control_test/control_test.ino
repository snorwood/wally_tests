/*
  control_test.ino - Test environment for control logic.
  Created by Team Wall-R-Us, March 4, 2016.
*/
#include "wally.h"
#include "rolling_median.h"
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
    wally->stop();
    digitalWrite(13, HIGH);
    delay(500);
    wally->waitButton();
    delay(500);
    wallyControl->begin();
    start_time = micros();
    state = 1;
  }
  
  wallyControl->update();
  if (state == 1)  {
    wallyControl->horizontalControl(0);
    if (wally->readUltrasonic(0) < 6){
      wally->setMotors(25, 25);
      delay(1000); 
      state = 2;
    }
  }
  
  if (state == 2)  {
    wally->setMotors(75, 75);
//    if (wally->getOrientation(wally->readAccelerometer()) == UP){
//      state = 0;
//    }
  }
  
  if (state == 3) {
    digitalWrite(13, LOW);
    wallyControl->verticalControl(75, 0);
    delay(10000);
  } else {
    digitalWrite(13, HIGH);
  }
  
  if (wally->readButton() || (loop_time - start_time > TRIAL_TIME)){
    state = 0;
  }
}
