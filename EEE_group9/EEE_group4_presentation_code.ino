// Pin definitions
const int trigPin = 5;
const int echoPin = 6;
const int relayPin = 7;

// Trigger Distance (in cm)
const int Threshold = 10;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  long duration;
  int distance;

  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo time
  duration = pulseIn(echoPin, HIGH);

  // Convert to distance in cm
  distance = duration * 0.034 / 2;

 
  if  (distance < Threshold && distance > 0) {
    // object is close, turn on pump
    
    digitalWrite(relayPin, LOW);
  } else {
    // no object detected, turn off pump
    
    digitalWrite(relayPin, HIGH);
  }

  delay(2000); // Small delay to reduce bouncing
}