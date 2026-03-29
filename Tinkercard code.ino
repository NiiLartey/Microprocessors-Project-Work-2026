// 7-segment pins (a-g) connected to Arduino pins 13 to 7
int segPins[] = {11, 10, 7, 8, 9, 12, 13}; 

// LED pins
int ledPins[] = {14, 15, 16}; 

// Button pins
int buttonPins[] = {6, 5, 4}; // buttons 1,2,3

// Buzzer pin
int buzzerPin = 3;

// 7-segment encoding for common cathode
// Index = number to display
byte numbers[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void setup() {
  // Set segment pins as OUTPUT
  for(int i = 0; i < 7; i++){
    pinMode(segPins[i], OUTPUT);
  }

  // Set LED pins as OUTPUT
  for(int i = 0; i < 3; i++){
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // start OFF
  }

  // Set button pins as INPUT_PULLUP
  for(int i = 0; i < 3; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Set buzzer pin as OUTPUT
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // start OFF
}

void loop() {
  // Check each button
  bool buttonPressed = false;
  for(int i = 0; i < 3; i++){
    if(digitalRead(buttonPins[i]) == LOW){ // button pressed (connected to GND)
      buttonPressed = true;

      // Display number i+1 on 7-segment
      for(int j = 0; j < 7; j++){
        digitalWrite(segPins[j], numbers[i+1][j] ? HIGH : LOW);
      }

      // Turn ON corresponding LED
      for(int k = 0; k < 3; k++){
        digitalWrite(ledPins[k], (k == i) ? HIGH : LOW);
      }

      // Turn ON buzzer
      digitalWrite(buzzerPin, HIGH);
      break; // only handle one button at a time
    }
  }

  if(!buttonPressed){
    // No button pressed → turn everything OFF
    for(int j = 0; j < 7; j++) digitalWrite(segPins[j], LOW);
    for(int k = 0; k < 3; k++) digitalWrite(ledPins[k], LOW);
    digitalWrite(buzzerPin, LOW);
  }
}