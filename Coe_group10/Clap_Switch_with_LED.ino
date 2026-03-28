// Clap Controlled LED\

const int soundSensor = 3;   // Sound sensor pin
const int ledPin = 13;       // LED pin

int sensorState = 0;         // Current sensor reading
int lastSensorState = 0;     // Previous sensor reading
bool ledState = false;       // LED state (ON/OFF)

void setup() {
  pinMode(soundSensor, INPUT);   // Set sensor as input
  pinMode(ledPin, OUTPUT);       // Set LED as output
  digitalWrite(ledPin, LOW);     // Start with LED off
}

void loop() {

  // Read the sound sensor
  sensorState = digitalRead(soundSensor);

  // If a clap is detected (LOW -> HIGH)
  if (sensorState == HIGH && lastSensorState == LOW) {

    ledState = !ledState;        // Toggle LED
    digitalWrite(ledPin, ledState);

    delay(500);                  // Prevent multiple triggers
  }

  // Save current state for next check
  lastSensorState = sensorState;
}