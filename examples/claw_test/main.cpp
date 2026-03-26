#include <Arduino.h>
#include <ESP32Servo.h>

// --- Configuration ---
const int SERVO_PIN = 18; // Connect SG92R signal wire (orange/yellow) to GPIO 18
Servo clawServo;

// Mechanical limits of the 3D printed claw (Adjust these during testing!)
// Start with a narrow range to prevent breaking the printed gears.
const int CLAW_OPEN_ANGLE = 20;  
const int CLAW_CLOSED_ANGLE = 90; 

int currentAngle = CLAW_OPEN_ANGLE;

void setup() {
  // Initialize serial communication for the Serial Monitor
  Serial.begin(115200);
  
  // Standard PWM parameters for SG92R
  ESP32PWM::allocateTimer(0);
  clawServo.setPeriodHertz(50); // Standard 50hz servo
  
  // Attach the servo and set to initial safe position
  clawServo.attach(SERVO_PIN, 500, 2400); 
  clawServo.write(currentAngle);
  
  Serial.println("\n--- Robotic Claw Controller ---");
  Serial.println("Send an angle between " + String(CLAW_OPEN_ANGLE) + " and " + String(CLAW_CLOSED_ANGLE));
}

void loop() {
  // Check if data is available in the Serial Monitor
  if (Serial.available() > 0) {
    // Read the incoming integer
    int targetAngle = Serial.parseInt();
    
    // Read the newline character to clear the buffer
    if (Serial.read() == '\n') {
      
      // Constrain the input to protect the physical claw mechanism
      int safeAngle = constrain(targetAngle, CLAW_OPEN_ANGLE, CLAW_CLOSED_ANGLE);
      
      if (targetAngle != safeAngle) {
        Serial.println("Warning: Angle out of bounds. Constrained to " + String(safeAngle));
      }

      // Move the servo
      clawServo.write(safeAngle);
      currentAngle = safeAngle;
      
      Serial.print("Claw moved to: ");
      Serial.print(currentAngle);
      Serial.println(" degrees.");
    }
  }
}