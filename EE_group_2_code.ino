const int pin1 = 2;
const int pin2 = 3;
const int buzzerPin = 9; // Buzzer pin
const int ledPin = 8;    // LED pin
const int relayPin = 11; // Relay pin

volatile unsigned long pulseCount1 = 0; 
volatile unsigned long pulseCount2 = 0;

unsigned long previousTime = 0;

float flow_rate1 = 0.0;
float flow_rate2 = 0.0;

// Filtering variables
float f1_filtered = 0.0;
float f2_filtered = 0.0;
const float alpha = 0.3;

// Leak detection variables
int leakCounter = 0;
const int leakTriggerCount = 1;

// Leak state flag
bool leakDetected = false;

// ISR for sensor 1
void countPulse1()
{
  pulseCount1++;
}

// ISR for sensor 2
void countPulse2()
{
  pulseCount2++;
}

void setup()
{
  Serial.begin(9600);

  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);

  digitalWrite(relayPin, LOW);

  attachInterrupt(digitalPinToInterrupt(pin1), countPulse1, FALLING);
  attachInterrupt(digitalPinToInterrupt(pin2), countPulse2, FALLING);

  previousTime = millis();
}

void loop()
{
  if (millis() - previousTime >= 1000)
  {
    noInterrupts();
    unsigned long pulses1 = pulseCount1;
    unsigned long pulses2 = pulseCount2;

    pulseCount1 = 0;
    pulseCount2 = 0;
    interrupts();

    flow_rate1 = pulses1 / 7.5;
    flow_rate2 = pulses2 / 7.5;

    f1_filtered = alpha * flow_rate1 + (1 - alpha) * f1_filtered;
    f2_filtered = alpha * flow_rate2 + (1 - alpha) * f2_filtered;

    float flow_diff = f1_filtered - f2_filtered;

    float percent_diff = 0;
    if (f1_filtered > 0.001)
    {
      percent_diff = flow_diff / f1_filtered;
    }

    // Leak detection logic
    if (f1_filtered > 0.5)
    {
      if (flow_diff > 0.3 && percent_diff > 0.1)
      {
        leakCounter++;
      }
      else
      {
        leakCounter = 0;
        leakDetected = false;
      }

      if (leakCounter >= leakTriggerCount && !leakDetected)
      {
        Serial.println("⚠️ Leak Detected!");
        leakDetected = true;
      }
    }

    // Buzzer and LED control
    if (leakDetected)
    {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(ledPin, HIGH);

      // Turn OFF pump
      digitalWrite(relayPin, HIGH); // OFF (active LOW relay)
    }
    else
    {
      digitalWrite(buzzerPin, LOW);
      digitalWrite(ledPin, LOW);

      // Keep pump ON
      digitalWrite(relayPin, LOW);
    }

    // Debug output
    Serial.print("Flow1: ");
    Serial.print(flow_rate1, 2);
    Serial.print(" | Flow2: ");
    Serial.print(flow_rate2, 2);

    Serial.print(" | F1_filt: ");
    Serial.print(f1_filtered, 2);
    Serial.print(" | F2_filt: ");
    Serial.print(f2_filtered, 2);

    Serial.print(" | Diff: ");
    Serial.print(flow_diff, 2);

    Serial.print(" | %Diff: ");
    Serial.print(percent_diff * 100, 1);
    Serial.println("%");

    previousTime += 1000;
  }
}