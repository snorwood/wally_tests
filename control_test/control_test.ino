/*
  control_test.ino - Test environment for control logic.
  Created by Team Wall-R-Us, March 4, 2016.
*/
#include "wally.h"
#include "rolling_median.h"
#include "wally_control.h"

Wally* wally;
WallyControl* wallyControl;

unsigned long start_time, loop_time, down_time;
const unsigned long TRIAL_TIME = 15e6;
const unsigned long MIN_DOWN_TIME = 2e6;

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

  loop_time = micros();
  XYZ acc =wally->readAccelerometer();
  Serial.print(loop_time);
  Serial.print(" ");
  Serial.print(acc.x);
  Serial.print(" ");
  Serial.print(acc.y);
  Serial.print(" ");
  Serial.print(acc.z);
  Serial.print(" ");
  Serial.print(wally->readUltrasonic(0));
  Serial.print(" ");
  Serial.println(wally->readUltrasonic(1));
  
  
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
    wallyControl->verticalControl(80, 0);
//      if (wally->readAccelerometer().z < -6) {
      if(!wally->readIR()){
        while(wally->readIR());
        delay(50);
        state = 3;
      }
  }

  if (state == 3) {
    digitalWrite(13, HIGH);
    wally->setMotors(80, 80);
//    if(wally->readAccelerometer().y < -4) {
    if (wally->readIR()){
      while(wally->readIR());
      delay(10);
      state = 4;
    }
  }

  if (state == 4) {
    digitalWrite(13, LOW);
    wally->setMotors(25, 25);
    if (wally->readIR()){
      while(wally->readIR());
      delay(25);
      down_time = micros();
      state = 5;
    }
  }

  if (state == 5) {
    digitalWrite(13, HIGH);
    wally->setMotors(0,0);
    if (wally->readUltrasonic(1) < 20 && loop_time - down_time > MIN_DOWN_TIME){
      delay(100);
      state = 6;
    }
  }

  if (state == 6) {
    digitalWrite(13, LOW);
    wally->setMotors(100,100);
    if (wally->getOrientation(acc) == FLAT){
      delay(250);
      state = 0;
    }
  }
  
  if (wally->readButton() || (loop_time - start_time > TRIAL_TIME)){
    state = 0;
  }
}
