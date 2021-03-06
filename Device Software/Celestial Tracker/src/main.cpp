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

#define MAX_POSITION 16384
float Az_angle = 0;
float El_angle = 0;

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
    //Stepper_Az.setSpeed(1000);
    Stepper_Az.moveTo(0);
    Serial.println(".....Success!");
    // Elevation
    Serial.print("Setting up El motor");
    Stepper_El.setMaxSpeed(1000.0);
    Stepper_El.setAcceleration(4000);
    //Stepper_El.setSpeed(1000);
    Stepper_El.moveTo(0);
    Serial.println(".....Success!");

  // Homing
  next_loop = millis() + REPORT_TIME;
}

void loop() {
// Change direction once the motor reaches target position
	if (Stepper_Az.distanceToGo() == 0 && Stepper_El.distanceToGo() == 0) {
    delay(1000);
    // Create new positions
    int Az_pos_last = Stepper_Az.currentPosition();
    int El_pos_last = Stepper_El.currentPosition();
    int Az_pos_new = random(-MAX_POSITION/2,MAX_POSITION/2);
    int El_pos_new = Az_pos_new + random(-MAX_POSITION/2,MAX_POSITION/2);
    Stepper_Az.moveTo(Az_pos_new);
    Stepper_El.moveTo(El_pos_new);
    // Calculate Speeds
    int Az_move = Az_pos_new - Az_pos_last;
    int El_move = El_pos_new - El_pos_last;
    int Az_speed = 500;
    int El_speed = 500 * abs((float)El_move/(float)Az_move);
    if ( El_speed > 1000) {
      Az_speed = 500*(1000/(float)El_speed);
      El_speed = 1000;
    }
    Stepper_Az.setMaxSpeed(Az_speed);
    Stepper_El.setMaxSpeed(El_speed);


    Serial.print("Changing to new positions (Az,EL): "); Serial.print(Az_pos_new); Serial.print(" , "); Serial.println(El_pos_new);
    Serial.print("Az Move, El Move, Az Speed, El Speed: "); Serial.print(Az_move); Serial.print(" , "); Serial.print(El_move); Serial.print(" , "); Serial.print(Az_speed); Serial.print(" , "); Serial.println(El_speed);
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