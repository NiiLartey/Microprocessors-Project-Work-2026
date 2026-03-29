
// include the library code:
#include <LiquidCrystal.h>

//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
  LiquidCrystal lcd(13, 12, 11, 10,  9,  8);

// include the DHT11 library code:
#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to

dht DHT;

void setup() 
{
  pinMode(2,INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  lcd.setCursor(0,0);
  lcd.print("  WEATHER STATION    ");
}

void loop() 
{
  int S2 = analogRead(A1);
  int S3 = analogRead(A2);
  int Rain = digitalRead(2);

  DHT.read11(dht_apin);
  
  //Temperature Sensing
  int Temp = DHT.temperature;
  lcd.setCursor(0,1);
  lcd.print("  TM:");
  lcd.print(Temp);
  lcd.print("C  ");

  //Humidity Sennsing
  int Humidity = DHT.humidity;
  lcd.setCursor(11,1);
  lcd.print("HM:");
  lcd.print(Humidity);
  lcd.print("%  ");
  
  //Light Intensity
  lcd.setCursor(11,2);
  lcd.print("LI:");
  int LI=S2*2.1;
  lcd.print(LI);
  lcd.print("Lx  ");
  
  //Air Moisture Sensor
  lcd.setCursor(0,2);
  lcd.print("  AM:");
  int AM = map(S3,0,1023,0,100);
  lcd.print(AM);
  lcd.print("% ");

  //Rain Sensor
  if(Rain == HIGH)
  {
    lcd.setCursor(0,3);
    lcd.print("  RAINING    ");
    delay(50);
    lcd.setCursor(0,3);
    lcd.print("  RAINING.    ");
    delay(50);
    lcd.setCursor(0,3);
    lcd.print("  RAINING..   ");
    delay(50);
    lcd.setCursor(0,3);
    lcd.print("  RAINING...   ");
    delay(50);
  }
  else
  {
    lcd.setCursor(0,3);
    lcd.print("  NO RAIN     ");
  }
  delay(10);
}

