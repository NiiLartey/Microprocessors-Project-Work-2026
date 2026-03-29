int sensorPin = A0;
int ledPin = 4;
int buzzer = 4;  // FIX: use a different pin

int sensorValue = 0;
int angle = 0;

// Adjust these after testing
int minVal = 256;  // straight
int maxVal = 59;  // bent

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  sensorValue = analogRead(sensorPin);

  // Convert to angle
  angle = map(sensorValue, minVal, maxVal, 0, 180);

  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print("  Angle: ");
  Serial.println(angle);

  // Trigger posture warning
  if (angle > 20)   // adjust this threshold
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzer, LOW);
  }

  delay(200);
}