/*
  control_test.ino - Test environment for control logic.
  Created by Team Wall-R-Us, March 4, 2016.
*/
#include "wally.h"
#include "wally_control.h"

Wally* wally;
WallyControl* wallyControl;

unsigned long start_time, loop_time, down_time;
const unsigned long TRIAL_TIME = 15e6;
const unsigned long MIN_DOWN_TIME = 1e6;
const int SAMPLE_SIZE = 10;

int state;
  
void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  wally = new Wally();
  wallyControl = new WallyControl(wally, 350, 0 , 1, 5);
  state = 0;
}

void loop() {
  /* Drive Motors Back and Forth */
  if (state == 0){
    wally->stop();
    digitalWrite(13, HIGH);
    delay(500);
    wally->waitButton();
    start_time = micros();
    wally->calibrateAccelerometer();
    wallyControl->begin();    
    for (int i = 0; i < 5; i++)
      wallyControl->update();
      
    state = -2;
  }

  loop_time = micros();
  wallyControl->update(); 
  XYZ acc = wallyControl->readAccelerometer();
//  Serial.print(loop_time);
//  Serial.print(" ");
//  Serial.print(acc.x);
//  Serial.print(" ");
//  Serial.print(acc.y);
//  Serial.print(" ");
//  Serial.print(acc.z);
//  Serial.print(" ");
//  Serial.print(wally->readUltrasonic(0));
//  Serial.print(" ");
//  Serial.println(wally->readUltrasonic(1));

  if (state == -2) {
    wally->setMotors(50, 50);
    if (wallyControl->readUltrasonic(0) < 75) {
      state = -1;
      wally->stop();
      delay(50);
    }
  }

  if (state == -1) {
    wally->setMotors(30, -30);
    delay(400);
    state = 1;
  }
  
  if (state == 1)  {
    wallyControl->horizontalControl(0);
    if (wallyControl->readUltrasonic(0) < 10){
      wally->setMotors(25, 25);
      state = 2;
    }
  }
  
  if (state == 2)  {
    wally->setMotors(75, 75);
    if (wally->readIR() && wally->getOrientation(wallyControl->readAccelerometer()) == UP){
      state = 3;
    }
  }
  
  if (state == 3) {
    digitalWrite(13, LOW);
    wallyControl->verticalControl(90, 0);
//    wally->setMotors(100, 100);
    if(!wally->readIR()){
      while(!wally->readIR());
      state = 4;
    }
  }

  if (state == 4) {
    digitalWrite(13, HIGH);
    wally->setMotors(50, 50);
    if (!wally->readIR()){
      while(!wally->readIR());
      state = 5 ;
    }
  }

  if (state == 5) {
    digitalWrite(13, LOW);
    wally->setMotors(25, 25);
    if (!wally->readIR()){
      down_time = micros();
      state = 6;
    }
  }

  if (state == 6) {
    digitalWrite(13, HIGH);
    wally->setMotors(0,0);
    if (wallyControl->readUltrasonic(0) < 10 && loop_time - down_time > MIN_DOWN_TIME){
      delay(500);
      state = 7;
    }
  }

  if (state == 7) {
    digitalWrite(13, LOW);
    wally->setMotors(100,100);
    delay(95);
    wally->stop();
    delay(750);
    if (wally->getOrientation(wally->readAccelerometer()) == FLAT){
      state = 0;
    }
  }
  
  if (wally->readButton() || (loop_time - start_time > TRIAL_TIME)){
    state = 0;
  }
}
