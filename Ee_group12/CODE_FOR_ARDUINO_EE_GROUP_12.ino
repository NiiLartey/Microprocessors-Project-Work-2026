// ============================================================
//  SMART WATER SPRINKLER — Physical Build
//  LCD 16x2 (direct) + LDR + Soil Moisture + Low Trigger Relay
//  Activates at sunrise + dry soil → runs for 30 minutes
// ============================================================

#include <LiquidCrystal.h>

// RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// ---------- PIN DEFINITIONS ----------
const int LDR_PIN      = A0;  // LDR voltage divider midpoint
const int MOISTURE_PIN = A1;  // Soil moisture sensor AOUT
const int RELAY_PIN    = 8;   // Relay IN (low level trigger)
const int LED_PIN      = 13;  // Status LED

// ---------- THRESHOLDS ----------
const int LIGHT_THRESHOLD    = 500;  // Above = sunrise detected
const int MOISTURE_THRESHOLD = 400;  // Below = soil is dry

// ---------- TIMING ----------
const unsigned long SPRINKLER_DURATION = 1UL * 60UL * 1000UL; // 1 minutes

// ---------- STATE ----------
bool sprinklerActive = false;8u 62
unsigned long startTime = 0;

// ============================================================
void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN,   OUTPUT);

  // LOW TRIGGER relay — HIGH = OFF at startup
  // Prevents pump firing accidentally on boot
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN,   LOW);

  // Start LCD
  lcd.begin(16, 2);

  // Startup message
  lcd.setCursor(0, 0);
  lcd.print(" Water Sprinkler");
  lcd.setCursor(0, 1);
  lcd.print(" Initializing...");
  delay(2000);
  lcd.clear();

  Serial.println("=== Smart Sprinkler Ready ===");
}

// ============================================================
void loop() {
  int lightValue    = analogRead(LDR_PIN);
  int moistureValue = analogRead(MOISTURE_PIN);

  bool isSunrise = (lightValue    <= LIGHT_THRESHOLD);
  bool isDry     = (moistureValue <= MOISTURE_THRESHOLD);

  Serial.print("Light: ");       Serial.print(lightValue);
  Serial.print(" | Moisture: "); Serial.print(moistureValue);
  Serial.print(" | Sunrise: ");  Serial.print(isSunrise ? "YES" : "no");
  Serial.print(" | Dry: ");      Serial.println(isDry ? "YES" : "no");

  // --- TURN ON ---
  // Low sun detected + soil dry + not already running
  if (isSunrise && isDry && !sprinklerActive) {
    sprinklerActive = true;
    startTime = millis();

    digitalWrite(RELAY_PIN, LOW);  // LOW = relay ON (low trigger)
    digitalWrite(LED_PIN,  HIGH);  // LED ON
    Serial.println(">>> PUMP ON — 1 min cycle started <<<");
  }

  // --- TURN OFF ---
  // 1 minute timer expired
  if (sprinklerActive && (millis() - startTime >= SPRINKLER_DURATION)) {
    sprinklerActive = false;

    digitalWrite(RELAY_PIN, HIGH); // HIGH = relay OFF (low trigger)
    digitalWrite(LED_PIN,   LOW);  // LED OFF
    Serial.println(">>> PUMP OFF — cycle complete <<<");
  }

  // --- UPDATE LCD ---
  updateLCD(isSunrise, isDry);

  delay(500);
}

// ============================================================
//  LCD DISPLAY LOGIC
//  Row 0: Status message
//  Row 1: Condition info or countdown timer
// ============================================================
void updateLCD(bool isSunrise, bool isDry) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (sprinklerActive) {
    // Pump is running — show countdown
    lcd.print("  Watering...   ");
    lcd.setCursor(0, 1);

    unsigned long elapsed   = millis() - startTime;
    unsigned long remaining = (SPRINKLER_DURATION - elapsed) / 1000;

    unsigned long mins = remaining / 60;
    unsigned long secs = remaining % 60;

    lcd.print("Done in: ");
    lcd.print(mins);
    lcd.print("m ");
    if (secs < 10) lcd.print("0"); // Leading zero e.g. 4m 05s
    lcd.print(secs);
    lcd.print("s  ");

  } else {
    // Pump is idle — show why it's waiting
    if (!isSunrise && !isDry) {
      lcd.print("Status: Waiting ");
      lcd.setCursor(0, 1);
      lcd.print("Day & Moist   ");
    } else if (!isSunrise) {
      lcd.print("Status: Waiting ");
      lcd.setCursor(0, 1);
      lcd.print("Sun is high ");
    } else if (!isDry) {
      lcd.print("Status: Waiting ");
      lcd.setCursor(0, 1);
      lcd.print("Soil is Moist   ");
    } else {
      lcd.print(" Sun is low + Dry! ");
      lcd.setCursor(0, 1);
      lcd.print(" Cycle Complete ");
    }
  }
}
