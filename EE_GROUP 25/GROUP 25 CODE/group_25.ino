// C++ code
//
// 1. Define Pins
const int trigPin = 7;
const int echoPin = 2;
const int ledPin = 13;
const int buzzerPin = 8;

// 2. Define Variables
long duration;
int distance;
int threshold = 25; // The "Poor Posture" limit in cm

void setup() {
  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  Serial.begin(9600); // For debugging on your computer
}

void loop() {
  // 3. Trigger the Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 4. Read the Echo
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Formula to get cm

  // 5. Logic Check
  if (distance >= threshold) {
    // POOR POSTURE: Turn everything ON
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000); // Beep at 1000Hz
  } else {
    // GOOD POSTURE: Turn everything OFF
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  }

  delay(100); // Wait a bit before checking again
}