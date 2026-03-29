// --------------------
// LED Pins
// --------------------
int led1 = 15;
int led2 = 6;
int led3 = 5;

// --------------------
// Button Pins
// --------------------
int btn1 = 4;
int btn2 = 3;
int btn3 = 2;

// --------------------
// Buzzer
// --------------------
int buzzer = A0;

// --------------------
// 7 Segment Pins
// --------------------
int segA = 10;
int segB = 9;
int segC = 11;
int segD = 12;
int segE = 13;
int segF = 7;
int segG = 8;


// --------------------
// Setup
// --------------------
void setup(){

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);

  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
}


// --------------------
// Display Functions
// --------------------

void display1(){
  digitalWrite(segA, LOW);
  digitalWrite(segB, HIGH);
  digitalWrite(segC, HIGH);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
}

void display2(){
  digitalWrite(segA, HIGH);
  digitalWrite(segB, HIGH);
  digitalWrite(segC, LOW);
  digitalWrite(segD, HIGH);
  digitalWrite(segE, HIGH);
  digitalWrite(segF, LOW);
  digitalWrite(segG, HIGH);
}

void display3(){
  digitalWrite(segA, HIGH);
  digitalWrite(segB, HIGH);
  digitalWrite(segC, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, HIGH);
}

void clearDisplay(){
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
}


// --------------------
// Main Loop
// --------------------
void loop(){

  int b1 = digitalRead(btn1);
  int b2 = digitalRead(btn2);
  int b3 = digitalRead(btn3);

  // Player 1 pressed
  if(b1 == LOW){

    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);

    display1();
    tone(buzzer,10000);
  }

  // Player 2 pressed
  else if(b2 == LOW){

    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);

    display2();
    tone(buzzer,7500);
  }

  // Player 3 pressed
  else if(b3 == LOW){

    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);

    display3();
    tone(buzzer,5000);
  }

  // No button pressed
  else{

    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);

    clearDisplay();
    noTone(buzzer);
  }

}