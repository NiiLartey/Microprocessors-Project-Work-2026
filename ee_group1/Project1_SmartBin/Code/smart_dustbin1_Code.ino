#include <Servo.h>
#include <LiquidCrystal.h>

// ============ PIN DEFINITIONS ============
// Level sensor (senses garbage inside bin)
const int trigLevel = 3;
const int echoLevel = 4;

// Proximity sensor (senses person approaching)
const int trigProx = 5;
const int echoProx = 6;

// Servo motor (lid control)
Servo lidServo;
const int servoPin = 9;

// LEDs
const int greenLED = 7;   // 0-50% full
const int yellowLED = 8;  // 50-80% full
const int redLED = 12;    // 80-100% full

// Buzzer
const int buzzer = 13; //Makes sound when bin is >90%full

// LCD Display
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0); //LCD connected to analog pins

// ============ VARIABLES ============
int fillPercent = 0;			//how full the bin is (0%-100%)
unsigned long lidTimer = 0;		//how long the lid is opened
bool lidOpen = false;			// is the lid currently open?
unsigned long lastBeepTime =0;	//last time buzzer sounded

// ============ FUNCTION: GET DISTANCE ============
float getDistance(int trig, int echo) {
  digitalWrite(trig, LOW); //Clear the trigger pin
  delayMicroseconds(2); 		//Wait 2 microseconds
  digitalWrite(trig, HIGH);		//Send pulse
  delayMicroseconds(10);  		//Pulse lasts 10us 
  digitalWrite(trig, LOW);		//Stop pulse
  
  //pulseIn waits for echo to go HIGH
  //Times how long it stays HIGH
  //Returns 0 if no echo after 30ms(30,000us)
  long duration = pulseIn(echo, HIGH, 30000); 
  float distance = duration * 0.034 / 2; // divide by 2since sound travels there and back
  return distance;					// in cm
}

// ==================== SETUP =====================
void setup() {
  Serial.begin(9600);   //start serial communication
  
  // Initializing pins
  pinMode(trigLevel, OUTPUT);
  pinMode(echoLevel, INPUT);
  pinMode(trigProx, OUTPUT);
  pinMode(echoProx, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // Turn everything off
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(buzzer, LOW);
  
  // Initialize servo
  lidServo.attach(servoPin);
  lidServo.write(0);  // Start with lid closed
  
  // Initialize LCD
  lcd.begin(16, 2);			//LCD is 16col x 2rows
  lcd.print("Smart Dustbin"); //Print title
  lcd.setCursor(0, 1);		//Move cursor to second line
  lcd.print("Ready!");		//Show Ready message
  delay(2000);				//Wait 2s
  lcd.clear();				//Clear Screen
  
  Serial.println("=== SMART DUSTBIN READY ===");
}

// ============ MAIN LOOP ============
void loop() {
  
  // ===== 1. READ PROXIMITY SENSOR (PERSON DETECTION) =====
  float personDist = getDistance(trigProx, echoProx);
  
  //If person is within 20cm
  if (personDist > 0 && personDist < 20) {
    if (lidOpen == false) {
      for (int angle=0; angle <=90; angle++){
        lidServo.write(90); // Open lid
      	delay(50);
      }
      lidOpen = true;		//remeber lid is OPEN
      Serial.println("Lid OPEN - Person detected");
    }
    // Reset timer(person still there)
    lidTimer = millis();    //record current time person was there
  }
  
  else{
    if(lidOpen){
      lidServo.write(0);
      lidOpen =false;
      Serial.println("Lid CLOSED");
    
    }
  }
  
  
  // ===== 2. READ LEVEL SENSOR (GARBAGE LEVEL) =====
  float garbageDistance = getDistance(trigLevel, echoLevel);
  
  // Calculate fill percentage (bin height = 30cm)
  if (garbageDistance > 0 && garbageDistance <= 30) {
    // When distance = 30cm → 0%, distance = 0cm → 100%
    fillPercent = (1 - (garbageDistance / 30)) * 100;
    
    // Keep within 0-100%
    if (fillPercent > 100) fillPercent = 100;
    if (fillPercent < 0) fillPercent =0;
  }
  else if (garbageDistance > 30) {	// More than 30cm means an Empty bin
    fillPercent = 0;   				// set fillPercent to 0
  }
  else {				//less than 30cm means bin is full
    fillPercent = 100; // set fillPercent to 100
  }
  
  // ===== 3. UPDATE LEDs =====
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  
  if (fillPercent < 50) {
    digitalWrite(greenLED, HIGH);
  }
  else if (fillPercent < 80) {
    digitalWrite(yellowLED, HIGH);
  }
  else {
    digitalWrite(redLED, HIGH);
  }
  
  // ===== 4. BUZZER ALERT =====
  if (fillPercent >= 85) {		//if bin is 85% full or more buzzer beeps
    if (millis() - lastBeepTime>5000){	//if 5seconds has passed
      tone(buzzer, 1000);				//buzzer beeps for 1000Hz
      delay(2000);						//beeps for 2s
      noTone(buzzer);					//stops
      lastBeepTime = millis();         //records current time and starts again
      Serial.println("Bin is FULL!!!");
    }
  }
  else {
    lastBeepTime =0;			//reset last beepTime to 0
  }
  
  // ===== 5. UPDATE LCD DISPLAY =====
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 500) {
    lastUpdate = millis();				//update time after 500ms 
    
    lcd.clear();
    
    // Show Fill percentage
    lcd.setCursor(0, 0);
    lcd.print("FillPercent: ");
    lcd.print(fillPercent);
    lcd.print("%");
    
    
    // Status
    lcd.setCursor(0, 1);
    if (fillPercent < 50) {
      lcd.print("Status: EMPTY");
    }
    else if (fillPercent < 80) {
      lcd.print("Status: HALF");
    }
    else {
      lcd.print("Status: FULL!");
    }
  }
  
  // ===== 6. SERIAL MONITOR DEBUG =====
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 1000) {
    lastDebug = millis();
    Serial.print("GarbageLevel: ");
    Serial.print(garbageDistance);
    Serial.print("cm | Fill: ");
    Serial.print(fillPercent);
    Serial.print("% | Person: ");
    Serial.print(personDist);
    Serial.println("cm");
  }
  
  delay(50);		//Waits 50ms before repeating
}
      
     