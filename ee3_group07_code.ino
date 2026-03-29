// Pin Definitions
int irPin = 2;
int buzzerPin = 8;
int pump = 7;

// State Variables
bool lastState = HIGH;
bool alarmTriggered = false;

void setup() {
  pinMode(irPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pump, OUTPUT);
  Serial.begin(9600);

  digitalWrite(pump, HIGH);      // Pump ON by default
  digitalWrite(buzzerPin, LOW);  // Buzzer OFF by default
}

void loop() {
  int sensorValue = digitalRead(irPin);

  // Hand just placed (HIGH to LOW transition)
  if (sensorValue == LOW && lastState == HIGH) {
    digitalWrite(pump, LOW);         // Pump OFF
    digitalWrite(buzzerPin, HIGH);   // Buzzer ON

    delay(500);                      // Buzzer lasts 0.5 seconds
    digitalWrite(buzzerPin, LOW);    // Buzzer OFF

    delay(700);                      // Pump stays OFF for total 1 second
    digitalWrite(pump, HIGH);        // Pump back ON

    alarmTriggered = true;
  }

  // Hand removed (LOW to HIGH transition)
  if (sensorValue == HIGH && lastState == LOW) {
    alarmTriggered = false;          // Reset alarm
  }

  // Default state - no object detected
  if (sensorValue == HIGH) {
    digitalWrite(pump, HIGH);
    digitalWrite(buzzerPin, LOW);
  }

  lastState = sensorValue;          // Save current state
  delay(50);                         // Debounce delay
}