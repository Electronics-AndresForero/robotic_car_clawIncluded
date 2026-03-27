#include <Arduino.h>
#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// --- Screen Configuration (Reverted to your working pins!) ---
#define TFT_CS   15 
#define TFT_DC   2  
#define TFT_RST  4  
#define TFT_BLK  32 

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// --- Servo Configuration ---
const int SERVO_PIN = 26; 
Servo clawServo;

const int CLAW_OPEN_ANGLE = 20;  
const int CLAW_CLOSED_ANGLE = 90; 
int currentAngle = CLAW_OPEN_ANGLE;

void setup() {
  Serial.begin(115200);
  
  // Wait a second for the monitor to connect so we don't miss the prints
  delay(1000); 
  Serial.println("\n--- Starting Boot Sequence ---");

  // 1. Turn on the screen backlight
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, HIGH);

  // 2. Initialize the ST7789 Screen
  tft.init(135, 240);
  tft.setRotation(1); // Landscape mode
  tft.fillScreen(ST77XX_BLACK);
  
  // 3. Draw the UI
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK); 
  tft.setTextSize(3); 
  tft.setCursor(10, 20);
  tft.print("Claw Tester");
  
  tft.setTextSize(4); 
  tft.setCursor(10, 70);
  tft.print("Ang:");
  
  tft.setCursor(110, 70); 
  tft.print(currentAngle);

  // 4. Initialize Servo safely
  ESP32PWM::allocateTimer(0);
  clawServo.setPeriodHertz(50);
  clawServo.write(currentAngle);
  clawServo.attach(SERVO_PIN, 500, 2400); 
  
  Serial.println("System Ready! Type an angle in the monitor.");
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming text until the Enter key is pressed
    String incomingText = Serial.readStringUntil('\n');
    
    // Clean up the string by removing invisible line endings (\r) or spaces
    incomingText.trim(); 

    // Make sure we actually received something before trying to move
    if (incomingText.length() > 0) {
      int targetAngle = incomingText.toInt();
      
      // Protect the claw mechanism
      int safeAngle = constrain(targetAngle, CLAW_OPEN_ANGLE, CLAW_CLOSED_ANGLE);
      
      // Move servo
      clawServo.write(safeAngle);
      currentAngle = safeAngle;
      
      // Update Screen
      tft.setCursor(110, 70); 
      tft.print("    ");       // Clear old number
      tft.setCursor(110, 70);  
      tft.print(currentAngle); // Print new number
      
      Serial.print("Command received. Claw moved to: ");
      Serial.println(currentAngle);
    }
  }
}