#include <Arduino.h>
#include <AccelStepper.h>

#define FULLSTEP 4
#define HALFSTEP 8
#define STEP_AZ_A 2 // Connected pin for Az motor, IN1
#define STEP_AZ_B 3 // Connected pin for Az motor, IN3
#define STEP_AZ_C 4 // Connected pin for Az motor, IN2
#define STEP_AZ_D 5 // Connected pin for Az motor, IN4

#define STEP_EL_A 8 // Connected pin for El motor, IN1
#define STEP_EL_B 9 // Connected pin for El motor, IN3
#define STEP_EL_C 10 // Connected pin for El motor, IN2
#define STEP_EL_D 11 // Connected pin for El motor, IN4


#define REPORT_TIME 1000
unsigned long next_loop = 0;

AccelStepper Stepper_Az(HALFSTEP,STEP_AZ_A,STEP_AZ_B,STEP_AZ_C,STEP_AZ_D);
AccelStepper Stepper_El(HALFSTEP,STEP_EL_A,STEP_EL_B,STEP_EL_C,STEP_EL_D);

void setup() {

  Serial.begin(9600);
  
  // Setup Steppers
    // Azimuth
    Serial.print("Setting up Az motor");
    Stepper_Az.setMaxSpeed(1000.0);
    Stepper_Az.setAcceleration(4000);
    Stepper_Az.setSpeed(1000);
    Stepper_Az.moveTo(2000);
    Serial.println(".....Success!");
    // Elevation
    Serial.print("Setting up El motor");
    Stepper_El.setMaxSpeed(1000.0);
    Stepper_El.setAcceleration(4000);
    Stepper_El.setSpeed(1000);
    Stepper_El.moveTo(2000);
    Serial.println(".....Success!");

  // Homing
  next_loop = millis() + REPORT_TIME;
}

void loop() {
// Change direction once the motor reaches target position
	if (Stepper_Az.distanceToGo() == 0) {
		Stepper_Az.moveTo(-Stepper_Az.currentPosition());
    Serial.println("Changing Az Direction");
  }
  if (Stepper_El.distanceToGo() == 0) {
		Stepper_El.moveTo(-Stepper_El.currentPosition());
    Serial.println("Changing El Direction");
  }

  if ( millis() >= next_loop ) {
    Serial.print("Current Position (Az, El): ");
    Serial.print(Stepper_Az.currentPosition());
    Serial.print(" , ");
    Serial.print(Stepper_El.currentPosition());
    Serial.println();
    next_loop = millis() + REPORT_TIME;
  }

	// Move the motor one step
	Stepper_Az.run();
  Stepper_El.run();
  

}