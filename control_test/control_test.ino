#include "wally.h"

Wally* wally;
unsigned long start_time, loop_time;
const unsigned long TRIAL_TIME = 3e6;
XYZ acc;

void setup() {
  Serial.begin(115200);
  wally = new Wally();
  wally->waitButton();
  start_time = micros();
}

void loop() {
  /* Drive Motors Back and Forth */
  loop_time = micros();
  if (loop_time - start_time < TRIAL_TIME) {
    wally->setMotors(50, 50);
    acc = wally->readAccelerometer();
    Serial.print(loop_time);
    Serial.print(",");
    Serial.print(acc.x);
    Serial.print(",");
    Serial.print(acc.y);
    Serial.print(",");
    Serial.println(acc.z);
  } else {
    wally.setMotors(0, 0);
  }
}
