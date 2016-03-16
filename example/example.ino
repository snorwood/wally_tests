 /*
  example.ino - Demo code for controlling all aspects of the robot.
  Created by Team Wall-R-Us, March 4, 2016.
*/

#include "wally.h"
#include "wally_control.h"
Wally* wally; // Initialize wally robot
WallyControl* wallyControl;;

int loopCount = 0;

void setup() {
  Serial.begin(115200); // Initialize serial 
  wally = new Wally();
  wally->calibrateAccelerometer();
  wallyControl = new WallyControl(wally);
  wallyControl->begin();  
}

void loop() {
  loopCount += 1;
//  Serial.println("Wally Example");  // Output serial string
  delay(1000);
  /* Wait for button press */
//  wally->waitButton();
//  Serial.println("Button Pressed");
  
  for (int i = 0; i < 10; i++) {
    wallyControl->update();
  }

  /* Read Ultrasonics */
//  Serial.print("Front Sensor: ");
//  Serial.println(wallyControl->readUltrasonic(0));
//  Serial.print("Right Sensor: ");
//  Serial.println(wallyControl->readUltrasonic(1));
//  Serial.print("Left Sensor: ");
//  Serial.println(wallyControl->readUltrasonic(2));

  /* Read Accelerometer */
  XYZ acc = wallyControl->readAccelerometer();
  Serial.print("Acceleration:\nx=");
  Serial.println(acc.x);
  Serial.print("y=");
  Serial.println(acc.y);
  Serial.print("z=");
  Serial.println(acc.z);
  Serial.print("orientation=");
  Serial.println(wally->getOrientation(acc));  
  Serial.print("theta=");
  Serial.println(wally->getTheta    (acc));  

  /* Drive Motors Back and Forth */
//  wally->setMotors(50, 50);
//  delay(5000);
//  wally->setMotors(-50, -50);
//  delay(5000);
//  wally->setMotors(0, 0);
}
