#include <Arduino.h>
#include <AccelStepper.h>

#define FULLSTEP 4
#define STEP_AZ_A 17 // Connected pin for Az motor, IN1
#define STEP_AZ_B 18 // Connected pin for Az motor, IN3
#define STEP_AZ_C 19 // Connected pin for Az motor, IN2
#define STEP_AZ_D 21 // Connected pin for Az motor, IN4

#define STEP_EL_A 25 // Connected pin for El motor, IN1
#define STEP_EL_B 26 // Connected pin for Az motor, IN3
#define STEP_EL_C 27 // Connected pin for Az motor, IN2
#define STEP_EL_D 32 // Connected pin for Az motor, IN4


AccelStepper Stepper_Az(FULLSTEP,STEP_AZ_A,STEP_AZ_B,STEP_AZ_C,STEP_AZ_D);
AccelStepper Stepper_El(FULLSTEP,STEP_EL_A,STEP_EL_B,STEP_EL_C,STEP_EL_D);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}