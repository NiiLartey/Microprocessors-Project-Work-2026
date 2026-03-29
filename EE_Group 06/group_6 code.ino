#include <Servo.h>

Servo myServo;

// Pins
const int rainSensorPin = A0;
const int servoPin = 6;

// Sensor calibration (adjust these after testing)
int sensorDry = 700;   // fully dry reading
int sensorWet = 400;   // fully wet reading

// Servo range
const int servoMin = 0;   // folded
const int servoMax = 180;  // fully open

// Smoothing
const int numReadings = 5;

// Track roof state
enum RoofState {FOLDED, OPEN};
RoofState roofState = FOLDED;

// Servo movement speed
const int moveDelay = 60; 

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  myServo.write(servoMin); // start folded
  roofState = FOLDED;
}

// Smooth sensor reading
int readSensor() {
  long sum = 0;
  for(int i=0; i<numReadings; i++){
    sum += analogRead(rainSensorPin);
    delay(100);
  }
  return sum / numReadings;
}

// Smooth servo movement
void moveServo(int targetAngle) {
  int current = myServo.read();
  if(current == targetAngle) return;

  int step = (current < targetAngle) ? 1 : -1;
  while(current != targetAngle){
    current += step;
    myServo.write(current);
    delay(moveDelay);
  }
}

void loop() {
  int sensorValue = readSensor();
  Serial.println(sensorValue);

  // --- State logic ---
  if(roofState == FOLDED && sensorValue < sensorDry - 10){
    // Rain detected → fully open
    moveServo(servoMax);
    roofState = OPEN;
  }
  else if(roofState == OPEN && sensorValue > sensorDry){
    // Rain stopped → fully fold
    moveServo(servoMin);
    roofState = FOLDED;
  }

  delay(50);
}
