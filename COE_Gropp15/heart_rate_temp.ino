/*
 * Combined Heart Rate + LM35 Temperature Monitor
 * - PulseSensor on Analog Pin A0
 * - LM35 on Analog Pin A1
 * - 16x2 LCD in 4-bit mode
 * - LED blink on Pin 13 with heartbeat
 * - Buzzer on Pin 8 for Tachycardia / Bradycardia alarm
 */

#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <LiquidCrystal.h>

// ── Pin Definitions ──────────────────────────────────────
const int PULSE_PIN   = 0;   // PulseSensor → A0
const int TEMP_PIN    = 1;   // LM35        → A1
const int LED_PIN     = 13;  // Onboard LED
const int BUZZER_PIN  = 8;   // Buzzer      → D8

// ── Alarm Thresholds ─────────────────────────────────────
const int TACHY_THRESHOLD = 100;  // Above 100 BPM = Tachycardia
const int BRADY_THRESHOLD = 60;   // Below  60 BPM = Bradycardia

// ── Buzzer Timing ─────────────────────────────────────────
const unsigned long BEEP_INTERVAL = 400;
const unsigned long BEEP_DURATION = 150;
unsigned long lastBuzzerBeep = 0;
bool buzzerState = false;

// ── PulseSensor Setup ─────────────────────────────────────
int Threshold = 550;
PulseSensorPlayground pulseSensor;

// ── LCD Setup (RS, E, D4, D5, D6, D7) ────────────────────
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// ── Globals ───────────────────────────────────────────────
int currentBPM  = 0;
unsigned long lastLCDUpdate = 0;
const unsigned long LCD_INTERVAL = 1000; // refresh LCD every 1s

// Custom heart character for LCD
byte heartChar[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

// Custom bell character for alarm
byte bellChar[8] = {
  0b00100,
  0b01110,
  0b01110,
  0b01110,
  0b11111,
  0b00000,
  0b00100,
  0b00000
};

void setup() {
  Serial.begin(9600);

  // Configure buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // LCD init
  lcd.begin(16, 2);
  lcd.createChar(0, heartChar);
  lcd.createChar(1, bellChar);

  // Splash screen
  lcd.setCursor(0, 0);
  lcd.print("  Health Monitor");
  lcd.setCursor(0, 1);
  lcd.print("  Initializing..");
  delay(2000);
  lcd.clear();

  // Static labels
  lcd.setCursor(0, 0);
  lcd.print("BPM:");
  lcd.setCursor(0, 1);
  lcd.print("Temp:");

  // PulseSensor init
  pulseSensor.analogInput(PULSE_PIN);
  pulseSensor.blinkOnPulse(LED_PIN);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("PulseSensor started OK.");
  }
}

void loop() {
  // ── Read Heart Rate ──────────────────────────────────────
  if (pulseSensor.sawStartOfBeat()) {
    currentBPM = pulseSensor.getBeatsPerMinute();
    Serial.print("BPM: ");
    Serial.println(currentBPM);
    Serial.println("♥ A HeartBeat Happened!");
  }

  // ── Read Temperature ─────────────────────────────────────
  // LM35: 10mV per °C, Arduino ADC reference = 5V (5000mV over 1024 steps)
  int rawTemp   = analogRead(TEMP_PIN);
  float voltage = rawTemp * (5000.0 / 1024.0); // millivolts
  float tempC   = voltage / 10.0;              // LM35: 10mV/°C

  // ── Buzzer Alarm (non-blocking) ───────────────────────────
  unsigned long now = millis();
  bool alarmActive = (currentBPM > TACHY_THRESHOLD ||
                     (currentBPM > 0 && currentBPM < BRADY_THRESHOLD));

  if (alarmActive) {
    if (!buzzerState && (now - lastBuzzerBeep >= BEEP_INTERVAL)) {
      digitalWrite(BUZZER_PIN, HIGH);
      buzzerState    = true;
      lastBuzzerBeep = now;
    }
    if (buzzerState && (now - lastBuzzerBeep >= BEEP_DURATION)) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzerState = false;
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerState = false;
  }

  // ── Update LCD every LCD_INTERVAL ms ─────────────────────
  if (now - lastLCDUpdate >= LCD_INTERVAL) {
    lastLCDUpdate = now;

    if (alarmActive) {
      // ── Row 0: Heart Attack warning ──────────────────────
      lcd.setCursor(0, 0);
      lcd.write(byte(1));           // bell icon
      lcd.print("HEART ATTACK! ");
      lcd.write(byte(1));           // bell icon

      // ── Row 1: BPM + HIGH or LOW indicator ───────────────
      lcd.setCursor(0, 1);
      lcd.print("BPM:");
      lcd.print(currentBPM);
      lcd.print(currentBPM > TACHY_THRESHOLD ? " HIGH  " : " LOW   ");

    } else {
      // ── Row 0: BPM ───────────────────────────────────────
      lcd.setCursor(4, 0);
      if (currentBPM > 0) {
        lcd.print(currentBPM);
        lcd.print(" ");             // clear stale digits
        lcd.write(byte(0));         // heart symbol
        lcd.print("  ");
      } else {
        lcd.print("---   ");
      }

      // ── Row 1: Temperature ───────────────────────────────
      lcd.setCursor(5, 1);
      lcd.print(tempC, 1);          // 1 decimal place
      lcd.print((char)223);         // degree symbol
      lcd.print("C  ");
    }

    // Also print to Serial Monitor
    Serial.print("Temp: ");
    Serial.print(tempC, 1);
    Serial.println(" C");
    Serial.print("Status: ");
    Serial.println(alarmActive ? "HEART ATTACK" : "Normal");
  }

  delay(20); // required by PulseSensor library
}