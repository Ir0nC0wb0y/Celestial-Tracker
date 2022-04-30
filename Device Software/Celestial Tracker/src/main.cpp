#include <Arduino.h>
//#include <WiFiManager.h>
#include <AccelStepper.h>

#define FULLSTEP 4
#define STEP_AZ_A 16 // Connected pin for Az motor, IN1
#define STEP_AZ_B 17 // Connected pin for Az motor, IN3
#define STEP_AZ_C 18 // Connected pin for Az motor, IN2
#define STEP_AZ_D 19 // Connected pin for Az motor, IN4

#define STEP_EL_A 21 // Connected pin for El motor, IN1
#define STEP_EL_B 26 // Connected pin for Az motor, IN3
#define STEP_EL_C 25 // Connected pin for Az motor, IN2
#define STEP_EL_D 33 // Connected pin for Az motor, IN4

#define REPORT_TIME 1000

AccelStepper Stepper_Az(FULLSTEP,STEP_AZ_A,STEP_AZ_B,STEP_AZ_C,STEP_AZ_D);
AccelStepper Stepper_El(FULLSTEP,STEP_EL_A,STEP_EL_B,STEP_EL_C,STEP_EL_D);

void setup() {
  Serial.begin(115200);

  // Setup Steppers
  Serial.print("Setting up motors");
    // Azimuth
    Stepper_Az.setMaxSpeed(1000.0);
    Stepper_Az.setAcceleration(50);
    Stepper_Az.moveTo(8000);
    // Elevation
    Stepper_El.setMaxSpeed(1000.0);
    Stepper_El.setAcceleration(50);
    Stepper_El.moveTo(8000);
    
  Serial.println(".......Success!");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Stepper_Az.distanceToGo() == 0) {
		Stepper_Az.moveTo(-Stepper_Az.currentPosition());
    Serial.println("Changing Az direction");
  }
  if (Stepper_El.distanceToGo() == 0) {
		Stepper_El.moveTo(-Stepper_El.currentPosition());
    Serial.println("Changing El direction");
  }

	// Move the motor one step
	Stepper_Az.run();
  Stepper_El.run();
 
}