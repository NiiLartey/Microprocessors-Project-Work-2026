#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------- PIN DEFINITIONS ----------
const int LDR_PIN      = A0;
const int MOISTURE_PIN = A1;
const int MOTOR_PIN    = 7;
const int LED_PIN      = 13;

// ---------- THRESHOLDS ----------
const int LIGHT_THRESHOLD    = 600;  // Below = DARK
const int MOISTURE_THRESHOLD = 400;  // Below = DRY

// ---------- TIMING ----------
const unsigned long SPRINKLER_DURATION = 1UL * 60UL * 1000UL;

// ---------- STATE ----------
bool dispensing = false;
unsigned long startTime = 0;

// ============================================================
void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_PIN,   OUTPUT);

  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(LED_PIN,   LOW);

  lcd.init();
  lcd.backlight();

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

  bool isDark = (lightValue < LIGHT_THRESHOLD);   // 🌙 DARK CONDITION
  bool isDry  = (moistureValue <= MOISTURE_THRESHOLD);

  Serial.print("Light: ");      Serial.print(lightValue);
  Serial.print(" | Moisture: "); Serial.print(moistureValue);
  Serial.print(" | Dark: ");    Serial.print(isDark ? "YES" : "no");
  Serial.print(" | Dry: ");     Serial.println(isDry ? "YES" : "no");

  // --- TURN ON ---
  if (isDark && isDry && !dispensing) {
    dispensing = true;
    startTime  = millis();

    digitalWrite(MOTOR_PIN, HIGH);
    digitalWrite(LED_PIN,   HIGH);
    Serial.println(">>> SPRINKLER ON (Night + Dry) <<<");
  }

  // --- TURN OFF ---
  if (dispensing && (millis() - startTime >= SPRINKLER_DURATION)) {
    dispensing = false;

    digitalWrite(MOTOR_PIN, LOW);
    digitalWrite(LED_PIN,   LOW);
    Serial.println(">>> SPRINKLER OFF <<<");
  }

  updateLCD(isDark, isDry);

  delay(500);
}

// ============================================================
void updateLCD(bool isDark, bool isDry) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (dispensing) {
    lcd.print("  Watering...   ");
    lcd.setCursor(0, 1);

    unsigned long elapsed   = millis() - startTime;
    unsigned long remaining = (SPRINKLER_DURATION - elapsed) / 1000;

    unsigned long mins = remaining / 60;
    unsigned long secs = remaining % 60;

    lcd.print("Done in: ");
    lcd.print(mins);
    lcd.print("m ");
    if (secs < 10) lcd.print("0");
    lcd.print(secs);
    lcd.print("s  ");

  } else {
    if (!isDark && !isDry) {
      lcd.print("Status: Waiting ");
      lcd.setCursor(0, 1);
      lcd.print("Day & Moist     ");
    } else if (!isDark) {
      lcd.print("Status: Waiting ");
      lcd.setCursor(0, 1);
      lcd.print("Too Bright      ");
    } else if (!isDry) {
      lcd.print("Status: Waiting ");
      lcd.setCursor(0, 1);
      lcd.print("Soil is Moist   ");
    } else {
      lcd.print(" Night + Dry!   ");
      lcd.setCursor(0, 1);
      lcd.print(" Cycle Complete ");
    }
  }
}