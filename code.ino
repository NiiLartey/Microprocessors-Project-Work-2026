#include <Servo.h>

// Pin Configuration
const int trigPin = 9;
const int echoPin = 10;
const int redLed = 3;
const int greenLed = 5;
const int servoPin = 11;

Servo dispenserServo;


// Function to calculate distance in cm
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout for stability
  if (duration == 0) return 100; // If no pulse, return a safe "far" distance
  return duration * 0.034 / 2;
}


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  
  dispenserServo.attach(servoPin);
  dispenserServo.write(0); // Ensure motor starts at 0
  
  Serial.begin(9600);
  Serial.println("System Online: Ultrasonic Dispenser Ready");
}


void loop() {
  long distance = getDistance();
  
  // Debugging: See what the sensor sees
  Serial.print("Distance: ");
  Serial.println(distance);

  // TRIGGER LOGIC: 3cm to 12cm is usually the "Sweet Spot"
  if (distance > 3 && distance < 6) { 
    Serial.println(">>> HAND DETECTED! <<<");

    // 1. Switch LEDs
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    
    // 2. Move Servo
    dispenserServo.write(0);     
    delay(1000);                  
    dispenserServo.write(18\13240);      
    
    // 3. Reset LEDs immediately
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, HIGH);

    // 4. WAIT: Stay here until the hand is removed (> 15cm)
    // This stops it from re-triggering constantly at 8cm
    while (getDistance() < 15) {
      delay(100); 
    }
    
    Serial.println("Hand removed. Ready.");
    delay(500); 
  } 
  else {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
  }
  delay(100); 
}