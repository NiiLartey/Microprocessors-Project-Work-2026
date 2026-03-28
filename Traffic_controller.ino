int red = 2;
int yellow = 3;
int green = 4;
int buzzer = 12;
int button = 13;

int segPins[7] = {5, 6, 7, 8, 9, 10, 11};
byte numbers[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1}
};

bool checkButtonDelay(int ms) {
  for (int i = 0; i < ms / 20; i++) {
    if (digitalRead(button) == HIGH) {
      delay(50); 
      if (digitalRead(button) == HIGH) {
        pedestrianMode(); 
        return true; 
      }
    }
    delay(20);
  }
  return false;
}

void displayNumber(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], numbers[num][i]);
  }
}

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT);

  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }
}

void loop() {
  normalTraffic();
}

void normalTraffic() {
  digitalWrite(green, HIGH);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
  for (int i = 9; i >= 0; i--) {
    displayNumber(i);
    if (checkButtonDelay(1000)) return; 
  }

  digitalWrite(green, LOW);
  digitalWrite(yellow, HIGH);
  digitalWrite(red, LOW);
  for (int i = 3; i >= 0; i--) {
    displayNumber(i);
    if (checkButtonDelay(1000)) return;
  }

  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  for (int i = 9; i >= 0; i--) {
    displayNumber(i);
    if (checkButtonDelay(1000)) return;
  }
} 

void pedestrianMode() {
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  digitalWrite(buzzer, HIGH);
  
  for(int i = 9; i >= 0; i--) {
    displayNumber(i);
    delay(1000);}
    
   digitalWrite(buzzer, LOW);
     delay(500);   
  }







