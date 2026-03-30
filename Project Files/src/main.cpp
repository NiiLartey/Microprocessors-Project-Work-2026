#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// code for defining pin connections
const int pot_pin = A0;
const int IRpin = 2; 
const int buzzer_pin = 8;
const int servo_pin = 9;

// code for initialising variables
Servo clampservo;
LiquidCrystal_I2C lcd(0x27,16,2);

// volatile dropCount = 0;
volatile unsigned long last_dropTime = 0;
volatile float actualRate = 0;
unsigned long last_alarmCheck = 0;
int current_servoPosition = 90;
int targetRate = 0;
unsigned long currentTime;

// function to calculate the rate based on the time between drops
void countDrops();
void countDrops(){
    unsigned long now = millis(); // now variable allows us to use local time t
    unsigned long timeDiff = now - last_dropTime;

    if(timeDiff > 150){ // code to prevent false readings by ensuring at least 150ms between drops
        // dropCount++;
        actualRate = 60000 / timeDiff; // code to calculate the rate in drops per minute
        last_dropTime = now; // code to reset last drop time
    }
}

void setup(){
    // initialising IR sensor pin and setting up interrupt for counting drops
    pinMode(IRpin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IRpin), countDrops, FALLING); // code to set up interrupt for counting drops

    // initialising buzzer pin and setting it as output
    pinMode(buzzer_pin, OUTPUT); 

    // initialising servo motor pin and setting initial position
    clampservo.attach(servo_pin);
    lcd.init(); // code to initialise LCD
    lcd.backlight(); // code to turn on LCD backlight
    lcd.setCursor(0,0);
    lcd.print("Target: 0 dpm    ");
    clampservo.write(current_servoPosition); // code to set initial servo position to 90 degrees   
}

void loop(){
    currentTime = millis();

    // code to read target rate input from potentiometer and map it to a range of 0-100 drops per minute
    int potValue = analogRead(pot_pin);
    targetRate = map(potValue, 0, 1023, 0, 100);

    // Proportional Control Logic
    // code to limit servo movement to every 1000ms to prevent excessive adjustments
    static unsigned long last_servoUpdate = 0;
    if(currentTime - last_servoUpdate > 1000){
        // code to adjust servo position based on the difference between actual rate and target rate
        int error = targetRate - (int)actualRate;
        if(abs(error) > 5){ // code to only adjust servo position if error is greater than 5 drops per minute
            current_servoPosition += error / 5; // code to adjust servo position based on error (proportional control)
            current_servoPosition = constrain(current_servoPosition, 20, 160); // code to constrain servo position between 20 and 160 degrees
            clampservo.write(current_servoPosition); // code to set servo position
        }

        last_servoUpdate = currentTime; // code to reset last servo update time}
    }

    // code to sound alarm when flow rate status viers from normal
    if(actualRate > (targetRate + 15) && targetRate > 0){ // code to check if actual rate is above target rate and ensure alarm sounds at most every 500ms
        if(currentTime - last_alarmCheck > 500){
            tone(buzzer_pin, 700, 200); // code to sound buzzer at 700Hz for 200ms for the overflow alarm
            last_alarmCheck = currentTime; 
        }
    } else if(targetRate > 0 && (currentTime - last_dropTime) > 12000){
        if(currentTime - last_alarmCheck > 500){
            tone(buzzer_pin, 1000, 200); // code to sound buzzer at 1000Hz for 200ms for the occlusion alarm
            last_alarmCheck = currentTime; // code to reset time since no drops are falling
            actualRate = 0; // code to reset actual rate to 0 if no drops are falling for 12 seconds
        }
    } else {
        noTone(buzzer_pin); // code to turn off buzzer if actual rate is not above target rate
    }
    
    // code to display target rate and actual rate on LCD
    lcd.setCursor(0,0);
    lcd.print("Target: "); lcd.print(targetRate); lcd.print(" dpm    ");
    lcd.setCursor(0,1);
    lcd.print("Actual: "); lcd.print((int)actualRate); lcd.print(" dpm    ");

    delay(100); // code to add a small delay to prevent excessive LCD updates
}