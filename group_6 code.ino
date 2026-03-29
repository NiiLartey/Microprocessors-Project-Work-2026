#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(9); // connect servo signal to pin 9
}

void loop() {
  myServo.write(0);   // move to 0 degrees
  delay(1000);

  myServo.write(90);  // move to 90 degrees
  delay(1000);

  myServo.write(180); // move to 180 degrees
  delay(1000);
}