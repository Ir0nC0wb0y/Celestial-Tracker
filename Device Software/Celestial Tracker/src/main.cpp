#include <Arduino.h>
#include <WiFiManager.h>
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
  // Setup Wifi
  WiFiManager wm;
  bool res;
    res = wm.autoConnect("CelestialTracker","pass1234"); // password protected ap
    if(!res) {
      Serial.println("Failed to connect");
      // ESP.restart();
    } 
    else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
    }
  
  // Setup Steppers
    // Azimuth
    Stepper_Az.setMaxSpeed(1000.0);
    Stepper_Az.setAcceleration(50.0);
    Stepper_Az.setSpeed(200);
    // Elevation
    Stepper_El.setMaxSpeed(1000.0);
    Stepper_El.setAcceleration(50.0);
    Stepper_El.setSpeed(200);

  // Homing

}

void loop() {
  // put your main code here, to run repeatedly:
}