/*
  example.ino - Demo code for controlling all aspects of the robot.
  Created by Team Wall-R-Us, March 4, 2016.
*/

#include <wally.h>

Wally wally; // Initialize wally robot
int firstLoop = 1;

void setup() {
//  /* Wait for Button */
  Serial.begin(115200); // Initialize serial 
}

void loop() {
  Serial.println("Wally Example");  // Output serial string
  wally.waitButton();
  
  /* Read Ultrasonics */
  Serial.print("Front Sensor: ");
  Serial.println(wally.readUltrasonic(0));
  Serial.print("Rear Sensor: ");
  Serial.println(wally.readUltrasonic(1));

  /* Read Accelerometer */
  XYZ acc = wally.readAccelerometer();
  Serial.print("Acceleration: x=");
  Serial.print(acc.x);
  Serial.print(" y=");
  Serial.print(acc.y);
  Serial.print(" z=");
  Serial.println(acc.z);  

  /* Drive Motors Back and Forth */
  wally.setMotors(50, 50);
  delay(5000);
  wally.setMotors(-50, -50);
  delay(5000);
  wally.setMotors(0, 0);
}
