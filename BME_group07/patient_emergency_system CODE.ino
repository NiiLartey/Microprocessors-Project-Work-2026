#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buttonPin = 7;
const int buzzerPin = 8;const int ledPin = 13;

int lastButtonState = LOW; // Tracks the previous button state

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Initial screen setup
  lcd.print("System Ready");
}

void loop() {
  int currentButtonState = digitalRead(buttonPin);

  // ONLY run this if the button has changed (pressed or released)
  if (currentButtonState != lastButtonState) {
    
    if (currentButtonState == HIGH) { // Just pressed
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1000); 
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("!!! EMERGENCY !!!");
      lcd.setCursor(0, 1);
      lcd.print("  HELP NEEDED   ");
    } 
    else { // Just released
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("System Ready");
    }
    
    // Update the tracker so we don't repeat this until the next change
    lastButtonState = currentButtonState;
    delay(50); // Small debounce delay to prevent "double clicks"
  }
}