#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define WATER_SENSOR A0
#define RELAY 7

// RGB LED pins
#define RED_PIN 10
#define GREEN_PIN 9
#define BLUE_PIN 6

// Calibrated values
int dryValue = 0;    // raw value in open air
int wetValue = 530;  // raw value fully submerged

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(WATER_SENSOR, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(RELAY, HIGH);
  setColor(0, 0, 0);

  lcd.setCursor(0, 0);
  lcd.print("Water Level Sys");
  lcd.setCursor(0, 1);
  lcd.print("System Ready!  ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // --- Read Water Sensor ---
  int sensorValue = analogRead(WATER_SENSOR);

  // Map using calibrated values
  int percentage = map(sensorValue, dryValue, wetValue, 0, 100);
  percentage = constrain(percentage, 0, 100);

  // --- Display on LCD ---
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(percentage);
  lcd.print("%   ");

  lcd.setCursor(0, 1);

  // --- Control Logic ---
  if (percentage >= 90) {
    // FULL — pump off, green light
    lcd.print("Status: FULL    ");
    digitalWrite(RELAY, HIGH);  // Pump OFF
    setColor(0, 255, 0);        // Green

  } else if (percentage >= 60) {
    // FILLING — pump on, blue light
    lcd.print("Status: FILLING ");
    digitalWrite(RELAY, LOW);   // Pump ON
    setColor(0, 0, 255);        // Blue

  } else if (percentage >= 30) {
    // LOW — pump on, yellow light
    lcd.print("Status: LOW     ");
    digitalWrite(RELAY, LOW);   // Pump ON
    setColor(255, 100, 0);      // Yellow

  } else {
    // CRITICAL — pump on, red light
    lcd.print("Status: CRITICAL");
    digitalWrite(RELAY, LOW);   // Pump ON
    setColor(255, 0, 0);        // Red
  }

  // --- Serial Monitor ---
  Serial.print("Raw: ");
  Serial.print(sensorValue);
  Serial.print(" | Level: ");
  Serial.print(percentage);
  Serial.print("% | Status: ");

  if (percentage >= 90) {
    Serial.println("FULL - Pump OFF");
  } else if (percentage >= 60) {
    Serial.println("FILLING - Pump ON");
  } else if (percentage >= 30) {
    Serial.println("LOW - Pump ON");
  } else {
    Serial.println("CRITICAL - Pump ON");
  }

  delay(300);
}
