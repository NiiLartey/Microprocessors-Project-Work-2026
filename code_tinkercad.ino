//variation of code for Tinkercad simulation using a generic gas sensor.
#include <LiquidCrystal_I2C.h>// adding the functions  for the lcd


/* assigning various pins on the arduino for the 
 buzzer, push button, gas sensor*/
int buzzerpin = 3; 
int pushbutton = 11;
int gasSensorpin = A0;
int buzzpinread = 13;


int alarmmute = 0; // 0 = alarm active, 1 = alarm muted
int delaytime = 1000;


/*initializing variables to store read data*/
int pushdetect;
int readbuzzpin;

/* setting adjustable gas concentration(ppm) thresholds*/
float threshold1 = 0.4;
float threshold2 = 1;

LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
 Serial.begin(9600);
 pinMode(pushbutton, INPUT);
 pinMode(buzzerpin, OUTPUT);
 pinMode(buzzpinread, INPUT);
 digitalWrite(pushbutton, HIGH);

 /*inititializing lcd*/
 lcd.init();
 lcd.backlight();
 lcd.clear();
}

void loop() {
  lcd.setCursor(0,1);
  float smoke = (5./1023.)*analogRead(gasSensorpin);//read smoke(test element) concentration(ppm)

/*When the smoke concentraion is less than the first threshold and the alarm
 has not been reset to mute, "SAFE" is displayed on the lcd and the alarm is
 off, otherwise when the smoke concentration is greater than the first threshold
 and less than the second, the alarm comes on and the lcd displays "WARNING". When the
 smoke is above the third threshold then alarm turns on with the lcd displaying "DANGER!!!" */

 if(smoke < threshold1 && alarmmute == 0){
   lcd.clear();
   lcd.print("SAFE");
   digitalWrite(buzzerpin, LOW);
   } 
 else if(smoke >= threshold1 && smoke < threshold2 && alarmmute == 0) {
   lcd.clear();
   digitalWrite(buzzerpin, HIGH);
   lcd.print("WARNING");  
   }
 else if (smoke >= threshold2 && alarmmute == 0){
    lcd.clear();
    digitalWrite(buzzerpin, HIGH);
    lcd.print("DANGER!!!");
   }

   /* digital reads from push button and alarm network respectively*/
   pushdetect = digitalRead(pushbutton);
   readbuzzpin = digitalRead(buzzpinread);

  /* decision flow for resetting the alarm network*/
  if(pushdetect == 0 && readbuzzpin == 1 ){
     alarmmute = 1;
     digitalWrite(buzzerpin, LOW);
     lcd.clear();
     delay(delaytime);
   }
  else if (pushdetect == 0 && readbuzzpin == 0){
    alarmmute = 0;
   }
   delay(delaytime);
}