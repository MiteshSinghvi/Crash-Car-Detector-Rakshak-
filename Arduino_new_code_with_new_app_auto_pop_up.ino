#include <SoftwareSerial.h>
#include <Wire.h> 

// --- SET UP BLUETOOTH ---
// RX on Pin 2, TX on Pin 3
SoftwareSerial btSerial(2, 3); 

// --- SENSOR PINS & VARIABLES ---
int VIB_PIN = 10;           // SW-420 Vibration Sensor
const int MPU_ADDR = 0x68;  // I2C address of the MPU-6050
float crashThreshold = 1.8; // Your custom G-force threshold!

// --- ALARM PINS ---
int BUZZER_PIN = A0;        // Active Buzzer
int BUTTON_PIN = 13;        // False-Alarm Cancel Button

// --- MOTOR PINS ---
int IN1 = 6;
int IN2 = 7;
int IN3 = 8;
int IN4 = 9;

void setup() {
  Serial.begin(9600);   
  btSerial.begin(9600); 
  
  // Initialize Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize Vibration Sensor
  pinMode(VIB_PIN, INPUT); 

  // Initialize Buzzer and Button
  pinMode(BUZZER_PIN, OUTPUT);      
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // --- WAKE UP THE MPU-6050 ---
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Power management register
  Wire.write(0);    // Set to 0 to wake up the MPU-6050
  Wire.endTransmission(true);

  stopMotors();
  
  Serial.println("Rakshak Dual-Sensor System Ready!");
  Serial.println("MPU-6050 and SW-420 Active. Drive safe!");
}

void loop() {
  // ==========================================
  // 1. CRASH DETECTION (Vibration + G-Force)
  // ==========================================
  
  int vibCrash = digitalRead(VIB_PIN);
  float currentG = getGForce();

  if (vibCrash == HIGH || currentG > crashThreshold) { 
    stopMotors(); 
    Serial.println("\n⚠️ EMERGENCY BRAKES APPLIED! ⚠️");
    
    Serial.print("Impact G-Force was: ");
    Serial.println(currentG);

    if (vibCrash == HIGH) { Serial.println("Triggered by: Vibration Sensor (SW-420)"); }
    if (currentG > crashThreshold) { Serial.println("Triggered by: Accelerometer (MPU-6050)"); }

    // --- UPDATED BLUETOOTH APP NOTIFIER CODE ---
    // Sends the exact trigger word "WARNING" and the split phrase "Location: "
    btSerial.print("WARNING! Severe impact detected. Are you okay? Location: ");
    btSerial.println("https://maps.google.com/?q=22.52168400935758,72.91670644829883"); // Dummy coordinates for Anand
    
    Serial.println("10 Second Warning Started... Press Button to Cancel.");
    bool falseAlarm = false;

    for (int i = 0; i < 10; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
      delay(500);

      if (digitalRead(BUTTON_PIN) == LOW) { 
        falseAlarm = true;
        break; 
      }
    }

    if (falseAlarm) {
      Serial.println("Cancel Button Pressed! Crisis Averted.");
      btSerial.println("SAFE"); 
    } else {
      Serial.println("Countdown Expired! Driver Unresponsive.");
      
      // ADD THIS NEW LINE RIGHT HERE:
      btSerial.println("FORCE_MAP"); 
    }
    
    delay(3000);  
    Serial.println("\nResuming normal driving...");
  }

  // ==========================================
  // 2. NORMAL BLUETOOTH DRIVING LOGIC
  // ==========================================
  if (btSerial.available() > 0) {
    char command = btSerial.read();

    if (command == 'F' || command == 'f') { moveForward(); } 
    else if (command == 'B' || command == 'b') { moveBackward(); } 
    else if (command == 'L' || command == 'l') { turnLeft(); } 
    else if (command == 'R' || command == 'r') { turnRight(); } 
    else if (command == 'S' || command == 's') { stopMotors(); }
  }
}

// ==========================================
//          MPU-6050 MATH FUNCTION
// ==========================================

float getGForce() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); 

  int16_t AcX = Wire.read() << 8 | Wire.read();
  int16_t AcY = Wire.read() << 8 | Wire.read();
  int16_t AcZ = Wire.read() << 8 | Wire.read();

  float gX = (float)AcX / 16384.0;
  float gY = (float)AcY / 16384.0;
  float gZ = (float)AcZ / 16384.0;

  float totalG = sqrt((gX * gX) + (gY * gY) + (gZ * gZ));
  return totalG; 
}

// ==========================================
//          MOTOR LOGIC FUNCTIONS
// ==========================================

void moveForward() { digitalWrite(IN4, LOW); digitalWrite(IN3, HIGH); digitalWrite(IN2, HIGH); digitalWrite(IN1, LOW); }
void moveBackward() { digitalWrite(IN4, HIGH); digitalWrite(IN3, LOW); digitalWrite(IN2, LOW); digitalWrite(IN1, HIGH); }
void turnLeft() { digitalWrite(IN4, LOW); digitalWrite(IN3, HIGH); digitalWrite(IN2, LOW); digitalWrite(IN1, HIGH); }
void turnRight() { digitalWrite(IN4, HIGH); digitalWrite(IN3, LOW); digitalWrite(IN2, HIGH); digitalWrite(IN1, LOW); }
void stopMotors() { digitalWrite(IN4, LOW); digitalWrite(IN3, LOW); digitalWrite(IN2, LOW); digitalWrite(IN1, LOW); }