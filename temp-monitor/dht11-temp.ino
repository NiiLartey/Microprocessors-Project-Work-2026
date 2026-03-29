#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Pins
#define DHTPIN 4       
#define DHTTYPE DHT11   
#define BUZZER_PIN 18
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Initialize objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const float threshold = 37.5; 

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  // DHT11 is slow, reading every 2 seconds is best
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Check if reading failed
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // --- OLED Update ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("DHT11 HEALTH MONITOR");
  
  display.setCursor(0, 15);
  display.print("Humidity: ");
  display.print(h, 1);
  display.println("%");

  display.setTextSize(2);
  display.setCursor(0, 35);
  display.print(t, 1);
  display.print(" C");

  // --- Alert Logic ---
  if (t >= threshold) {
    display.setTextSize(1);
    display.setCursor(0, 55);
    display.println("ALERT: HIGH TEMP!");
    
    // Pulse the buzzer
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  display.display();
  delay(2000); 
}