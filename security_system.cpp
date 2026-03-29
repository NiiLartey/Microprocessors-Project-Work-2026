#include <Keypad.h>

// KEYPAD CONFIGURATION
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 6, 7, 8};
byte colPins[COLS] = {9, 10, 11, 12};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// PIN DEFINITIONS
#define PIR_PIN 2
#define TRIG_PIN 3
#define ECHO_PIN 4
#define BUZZER1 13
#define BUZZER2 A0
#define RED A1
#define GREEN A2
#define BLUE A3

// SYSTEM VARIABLES
String correctPIN = "1234";
String enteredPIN = "";
int attempts = 0;

enum SystemState {
  SYS_IDLE,
  SYS_AUTH,
  SYS_ACCESS_GRANTED,
  SYS_ALERT_MODE,
  SYS_ALARM
};

SystemState currentState = SYS_IDLE;

// FUNCTIONS
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

void setColor(bool r, bool g, bool b) {
  digitalWrite(RED, r);
  digitalWrite(GREEN, g);
  digitalWrite(BLUE, b);
}

void alarmRGB() {
  setColor(1,0,0); delay(200);
  setColor(0,0,1); delay(200);
  setColor(1,0,1); delay(200);
  setColor(0,1,1); delay(200);
  setColor(1,1,0); delay(200);
}

// SETUP
void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  setColor(0,0,0);
}

// LOOP
void loop() {
  float distance = getDistance();

  // IDLE STATE
  if (currentState == SYS_IDLE) {
    if (distance <= 30) {
      Serial.println("User detected");
      setColor(0,0,1); // BLUE
      currentState = SYS_AUTH;
      attempts = 0;
      enteredPIN = "";
      delay(500);
    }
  }

  // AUTHENTICATION STATE
  else if (currentState == SYS_AUTH) {
    char key = keypad.getKey();
    if (key) {
      Serial.print("Key: ");
      Serial.println(key);

      if (key == '#') {
        if (enteredPIN == correctPIN) {
          Serial.println("Access Granted");
          setColor(0,1,0); // GREEN
          digitalWrite(BUZZER1, HIGH);
          delay(200);
          digitalWrite(BUZZER1, LOW);
          currentState = SYS_ACCESS_GRANTED;
        } else {
          Serial.println("Wrong PIN");
          setColor(1,0,0); // RED
          digitalWrite(BUZZER1, HIGH);
          delay(200);
          digitalWrite(BUZZER1, LOW);
          attempts++;
          enteredPIN = "";

          if (attempts >= 2) {
            currentState = SYS_ALERT_MODE;
          }
        }
      } 
      else if (key == '*') {
        enteredPIN = "";
      } 
      else {
        enteredPIN += key;
      }
    }
  }

  // ACCESS GRANTED STATE
  else if (currentState == SYS_ACCESS_GRANTED) {
    Serial.println("Enter now...");
    delay(5000);
    currentState = SYS_IDLE;
    setColor(0,0,0);
  }

  // ALERT MODE STATE
  else if (currentState == SYS_ALERT_MODE) {
    Serial.println("ALERT MODE");
    digitalWrite(BUZZER1, HIGH);
    delay(200);
    digitalWrite(BUZZER1, LOW);
    delay(800);

    if (digitalRead(PIR_PIN) == HIGH) {
      currentState = SYS_ALARM;
    }
  }

  // ALARM STATE
  else if (currentState == SYS_ALARM) {
    Serial.println("INTRUDER ALERT!");
    alarmRGB();
    digitalWrite(BUZZER2, HIGH);

    char key = keypad.getKey();
    if (key) {
      if (key == '#') {
        if (enteredPIN == correctPIN) {
          Serial.println("System Reset");
          digitalWrite(BUZZER2, LOW);
          setColor(0,1,0);
          delay(1000);
          currentState = SYS_IDLE;
          enteredPIN = "";
        } else {
          enteredPIN = "";
        }
      } else {
        enteredPIN += key;
      }
    }
  }
}
