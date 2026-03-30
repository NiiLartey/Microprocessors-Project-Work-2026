
#define BLYNK_PRINT Serial  // <--- ADD THIS AT LINE 1
#define BLYNK_TEMPLATE_ID "TMPL2joVSxDOp"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation System"
#define BLYNK_AUTH_TOKEN "q-1cG4QIjvrz1Wihd-cmFwA95SOy5M7z"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST"; 
char pass[] = "";

// Hardware Pins
#define SOIL_PIN 34
#define RELAY_PIN 2
#define TRIG_PIN 5
#define ECHO_PIN 18

// System Thresholds
const int TANK_EMPTY_DIST = 50; 
const int TANK_FULL_DIST = 5;   
const int SOIL_DRY_THRESHOLD = 30; 

// State Variables
int moisturePercent;
int tankLevel;
bool manualMode = false;
bool isRaining = false;
bool isPumping = false;

// Timing Management
unsigned long prevSensorMillis = 0;
unsigned long pumpStartTime = 0;
unsigned long pumpDuration = 0;
const long sensorInterval = 2000; 

// Blynk Virtual Pin Handlers
BLYNK_WRITE(V2) { 
  manualMode = true; 
  int val = param.asInt();
  digitalWrite(RELAY_PIN, val);
  isPumping = (val == 1);
}

BLYNK_WRITE(V3) { 
  manualMode = param.asInt();
  if(!manualMode) digitalWrite(RELAY_PIN, LOW); 
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Blynk.begin(auth, ssid, pass);
  Serial.println("System Online.");

}

// Ultrasonic Sensor Logic
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // 30ms timeout to prevent CPU hang on sensor failure
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  return (duration == 0) ? 400 : (duration * 0.034 / 2);
}

void loop() {
  Blynk.run();
  unsigned long currentMillis = millis();

  // Sensor Polling Loop (Non-blocking)
  if (currentMillis - prevSensorMillis >= sensorInterval) {
    prevSensorMillis = currentMillis;

    // Analog to Percentage Conversion
    int moistureRaw = analogRead(SOIL_PIN);
    moisturePercent = map(moistureRaw, 4095, 0, 0, 100);
    
    // Tank Level Calculation with Clamped Values
    float distance = getDistance();
    float constrainedDist = constrain(distance, TANK_FULL_DIST, TANK_EMPTY_DIST);
    tankLevel = map(constrainedDist, TANK_EMPTY_DIST, TANK_FULL_DIST, 0, 100);

    isRaining = random(0, 10) > 8;

    // Terminal Feedback
    Serial.print("Moisture: "); Serial.print(moisturePercent);
    Serial.print("% | Tank: "); Serial.print(tankLevel);
    Serial.print("% | Rain: "); Serial.println(isRaining ? "YES" : "NO");

    // Cloud Data Sync
    Blynk.virtualWrite(V0, moisturePercent);
    Blynk.virtualWrite(V1, tankLevel);
    Blynk.virtualWrite(V4, isRaining ? 1 : 0);

    // Automation Logic
    if (!manualMode && !isPumping) {
      if (isRaining || tankLevel < 20) {
        digitalWrite(RELAY_PIN, LOW);
      } 
      else if (moisturePercent < SOIL_DRY_THRESHOLD) {
        isPumping = true;
        pumpStartTime = millis();
        // Variable watering duration based on severity
        pumpDuration = (moisturePercent < 15) ? 6000 : 3000; 
        digitalWrite(RELAY_PIN, HIGH);
      }
    }
  }

  // Independent Pump Watchdog
  if (isPumping && !manualMode) {
    if (currentMillis - pumpStartTime >= pumpDuration) {
      digitalWrite(RELAY_PIN, LOW);
      isPumping = false;
    }
  }
}