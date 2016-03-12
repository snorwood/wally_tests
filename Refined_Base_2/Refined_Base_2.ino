/*
  Refined_Base_2 some code to test target finding algorithm
  Created by Team Wall-R-Us, March 11, 2016.
*/

/*
 * Basic Parameter Use
 * limit is value used to set acceptable operation
 *    scanning - max distance allowed
 *    driving - min distance allowed
 * 
 * diff is difference between base and pole
 * 
 * range is +- tolerance for diff
 * 
 * timeout is loop ticks before timing out
*/

#include <wally.h>

Wally* wally; // Initialize wally robot

double readings_back[5];
double readings_front[5];
int rate = 28;


void setup() {
//  /* Wait for Button */
  wally = new Wally();
  Serial.begin(115200); // Initialize serial 
  for(int i = 0;i < (sizeof(readings_back)/sizeof(double));i++) {
    readings_back[i] = 0;
    readings_front[i] = 0;
  }
}

double filteredFront(wally) {
  int sum = 0;
  int counter = 0;
  for(int i = 0;i < (sizeof(readings_front)/sizeof(double)) - 1;i++) {
    readings_front[i] = readings_front[i + 1];
  }
  readings_front[(sizeof(readings_front)/sizeof(double)) - 1] = wally->readUltrasonic(0);
  for(int i = 0;i < (sizeof(readings_front)/sizeof(double)) - 1;i++) {
    if(readings_front[i] != 5) {
      sum = sum + readings_front[i]
      counter = counter + 1;
    }
  }

  if(counter > 0) {
    return sum / counter;
  }
  return 0;
}

double filteredBack(wally) {
  int sum = 0;
  int counter = 0;
  for(int i = 0;i < (sizeof(readings_back)/sizeof(double)) - 1;i++) {
    readings_back[i] = readings_back[i + 1];
  }
  readings_back[(sizeof(readings_back)/sizeof(double)) - 1] = wally->readUltrasonic(1);
  for(int i = 0;i < (sizeof(readings_back)/sizeof(double)) - 1;i++) {
    if(readings_back[i] != 5) {
      sum = sum + readings_back[i]
      counter = counter + 1;
    }
  }

  if(counter > 0) {
    return sum / counter;
  }
  return 0;
}

int driveToTarget(wally, limit, diff, range) {
  int value_front = 0;
  int value_back = 0;
  wally->setMotors(0, 0);
  delay(250);
  value_front = filteredFront(wally);
  value_back = filteredBack(wally);

  wally->setMotors(rate, rate);
  while(value_front >= limit && value_back > limit && (value_back - value_front) < (diff + range) && (value_back - value_front) > (diff - range)) {
    value_front = filteredFront(wally);
    value_back = filteredBack(wally);
    delay(30);
  }
  wally->setMotors(0, 0);
  delay(250);
  if ((value_back - value_front) < (diff + range) && (value_back - value_front) > (diff - range)) {
    return 1;
  }
  return 0;
}

int scanAndCenter(wally, limit, diff, range, timeout) {
  int value_front = 0;
  int value_back = 0;
  int counter = 0;
  int tick = 0;
  wally->setMotors(0, 0);
  delay(250);
  value_front = filteredFront(wally);
  value_back = filteredBack(wally);
  
  if(value_front <= limit && value_back <= limit && (value_back - value_front) < (diff + range) && (value_back - value_front) > (diff - range)) {
    return 1;
  }

  wally->setMotors(rate, -rate);
  while((value_back >= limit || (value_back - value_front) > (diff + range) || (value_back - value_front) < (diff - range)) && tick < timeout) {
    tick = tick + 1;
    value_front = filteredFront(wally);
    value_back = filteredBack(wally);
    delay(30);
  }
  wally->setMotors(0, 0);
  if(tick >= timeout) {
    return 0;
  }
  delay(250);

  wally->setMotors(rate, -rate);
  while(value_front <= limit && value_back <= limit && (value_back - value_front) < (diff + range) && (value_back - value_front) > (diff - range)) {
    counter = counter + 1;
    value_front = filteredFront(wally);
    value_back = filteredBack(wally);
    delay(30);
  }
  wally->setMotors(0, 0);
  delay(250);

  wally->setMotors(-rate, rate);
  while(counter > 0) {
    counter = counter - 1;
    value_front = filteredFront(wally);
    value_back = filteredBack(wally);
    delay(30);
  }
  wally->setMotors(0, 0);
  delay(250);

  return 1;
}

void loop() {
  Serial.println("Wally Target Finder Test");  // Output serial string
  wally->waitButton();

  while(1) {
    if(scanAndCenter(wally, 100, 30, 3, 10000) = 1) {
      if(driveToTarget(wally, 30, 30, 3) = 1) {
        // WOOOOOOOOOOOOOOOOOOOOOOOO
        wally->waitButton();
      }
    } else {
      if (filteredFront(wally) > 50) {
        wally->setMotors(rate, rate);
        delay(250);
        wally->setMotors(0, 0);
      } else {
        wally->setMotors(-rate, -rate);
        delay(250);
        wally->setMotors(0, 0);
      }
    }
  }
}
