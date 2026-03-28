/* * ============================================================
 * SECTION 1: LIBRARIES & SYSTEM HARDWARE SETUP
 * ============================================================
 */
#include <LiquidCrystal_I2C.h>

// Initialize two LCDs on the shared I2C bus (SDA: A4, SCL: A5)
LiquidCrystal_I2C lcd1(0x27, 16, 2); // Primary Status Display
LiquidCrystal_I2C lcd2(0x26, 16, 2); // Owner Notification Display

const int BUZZER_PIN = 8; // Digital pin for the Piezo Alarm

/* * ============================================================
 * SECTION 2: GEOFENCE COORDINATE CONSTANTS
 * ============================================================
 * Defining the rectangular boundary for the KNUST campus.
 */
#define LAT_MIN 6.6720
#define LAT_MAX 6.6775
#define LON_MIN -1.5720
#define LON_MAX -1.5630

/* * ============================================================
 * SECTION 3: 20-STEP SIMULATED GPS ROUTE
 * ============================================================
 */
const float latCoords[20] = {
  6.6730, 6.6735, 6.6740, 6.6745,
  6.6750, 6.6755, 6.6760, 6.6764,
  6.6768, 6.6771, 6.6773, 6.6774,
  6.6778, 6.6784, 6.6792, 6.6802,
  6.6815, 6.6830, 6.6848, 6.6868
};

const float lonCoords[20] = {
  -1.5700, -1.5693, -1.5686, -1.5679,
  -1.5672, -1.5665, -1.5658, -1.5651,
  -1.5645, -1.5640, -1.5636, -1.5632,
  -1.5626, -1.5612, -1.5596, -1.5578,
  -1.5557, -1.5532, -1.5504, -1.5472
};

const int TOTAL_COORDS = 20;
int coordIndex = 0;

/* * ============================================================
 * SECTION 4: SIREN SWEEP FUNCTION
 * ============================================================
 * This function creates a rising and falling pitch to simulate 
 * a real security siren when a breach is detected.
 */
void soundSiren() {
  // Rising pitch sweep
  for (int freq = 500; freq <= 2000; freq += 20) {
    tone(BUZZER_PIN, freq);
    delay(15);
  }
  // Falling pitch sweep
  for (int freq = 2000; freq >= 500; freq -= 20) {
    tone(BUZZER_PIN, freq);
    delay(15);
  }
}

/* * ============================================================
 * SECTION 5: CORE EVALUATION & FORMATTING FUNCTIONS
 * ============================================================
 */
bool isInsideSafeZone(float lat, float lon) {
  return (lat >= LAT_MIN && lat <= LAT_MAX &&
          lon >= LON_MIN && lon <= LON_MAX);
}

String fmtCoord(float val) {
  int whole = (int)val;
  int dec = abs((int)round((val - whole) * 10000));
  String pad = (dec < 1000) ? "0" : "";
  return String(whole) + "." + pad + String(dec);
}

/* * ============================================================
 * SECTION 6: INITIALIZATION (SETUP)
 * ============================================================
 */
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN); // Ensure the system starts silently

  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();

  // Startup splash messages to verify I2C connectivity
  lcd1.setCursor(0, 0); lcd1.print("Anti-Theft Sys");
  lcd1.setCursor(0, 1); lcd1.print("Initialising...");
  lcd2.setCursor(0, 0); lcd2.print("Group 8 | COE");
  lcd2.setCursor(0, 1); lcd2.print("GPS Tracking ON");
  
  delay(2000); 
  lcd1.clear();
  lcd2.clear();
}

/* * ============================================================
 * SECTION 7: MAIN OPERATIONAL LOOP
 * ============================================================
 */
void loop() {
  if (coordIndex >= TOTAL_COORDS) {
    return; // System holds final ALERT state after 20 steps
  }

  float lat = latCoords[coordIndex];
  float lon = lonCoords[coordIndex];
  bool safe = isInsideSafeZone(lat, lon);

  // Step A: Display current coordinates on LCD 1
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Lat:" + fmtCoord(lat));
  lcd1.setCursor(0, 1);
  lcd1.print("Lon:" + fmtCoord(lon));
  delay(1200);

  lcd1.clear();
  lcd1.setCursor(0, 0);

  if (safe) {
    // STATE: SAFE - Normal Monitoring
    lcd1.print("Status: SAFE ");
    lcd1.setCursor(0, 1);
    lcd1.print("Inside Zone ");
    noTone(BUZZER_PIN); // Stop the siren
    
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print("Zone: ACTIVE ");
    lcd2.setCursor(0, 1);
    lcd2.print("Vehicle OK ");
  } else {
    // STATE: ALERT - Geofence Breach Detected!
    lcd1.print("!! ALERT !! ");
    lcd1.setCursor(0, 1);
    lcd1.print("Outside Zone! ");
    
    soundSiren(); // Execute the audible siren sweep
    
    // Update owner notification on LCD 2
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print("OWNER ALERT! ");
    lcd2.setCursor(0, 1);
    lcd2.print("Veh left zone ");
    
    delay(1000); 
    
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print("Lat:" + fmtCoord(lat));
    lcd2.setCursor(0, 1);
    lcd2.print("Lon:" + fmtCoord(lon));
  }

  coordIndex++;
  delay(800); 
}