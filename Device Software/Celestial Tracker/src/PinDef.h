#pragma once
#include <AccelStepper.h>


#define ENV_1 // The one mounted to the white box
//#define ENV_2 // the one mounted to the brown box

#define DRIVER   1
#define FULLSTEP 4
#define HALFSTEP 8


#ifdef ENV_1
  #define PIN_AZ_A 16 // Connected pin for Az motor, IN1
  #define PIN_AZ_B 17 // Connected pin for Az motor, IN3
  #define PIN_AZ_C 18 // Connected pin for Az motor, IN2
  #define PIN_AZ_D 19 // Connected pin for Az motor, IN4

  #define PIN_EL_A 27 // Connected pin for El motor, IN1
  #define PIN_EL_B 26 // Connected pin for El motor, IN3
  #define PIN_EL_C 25 // Connected pin for El motor, IN2
  #define PIN_EL_D 33 // Connected pin for El motor, IN4

  #define PIN_H_AZ 35 // Connected to SIG pin of Az Endstop
  #define PIN_H_EL 34 // Connected to SIG pin of El Endstop

  #define HOME_OFF_AZ   0.0  // Offset in homing for Az
  #define HOME_OFF_EL -90.0  // Offset in homing for El
#endif

#ifdef ENV_2
  #define PIN_AZ_A 16 // Connected pin for Az motor, IN1
  #define PIN_AZ_B 17 // Connected pin for Az motor, IN3
  #define PIN_AZ_C 18 // Connected pin for Az motor, IN2
  #define PIN_AZ_D 19 // Connected pin for Az motor, IN4

  #define PIN_EL_A 27 // Connected pin for El motor, IN1
  #define PIN_EL_B 26 // Connected pin for Az motor, IN3
  #define PIN_EL_C 25 // Connected pin for Az motor, IN2
  #define PIN_EL_D 33 // Connected pin for Az motor, IN4
  
  #define PIN_H_AZ 35 // Connected to SIG pin of Az Endstop
  #define PIN_H_EL 34 // Connected to SIG pin of El Endstop

  #define HOME_OFF_AZ 0.0  // Offset in homing for Az
  #define HOME_OFF_EL 0.0  // Offset in homing for El
#endif

extern AccelStepper Stepper_Az;
extern AccelStepper Stepper_El;