#include <Arduino.h>
#include <WiFiManager.h>


#include "Move.h"
#include "TrackerApi.h"
#include "PinDef.h"
#include "homing.h"
#include "GetData.h"

AccelStepper Stepper_Az(HALFSTEP,PIN_AZ_A,PIN_AZ_B,PIN_AZ_C,PIN_AZ_D);
AccelStepper Stepper_El(HALFSTEP,PIN_EL_A,PIN_EL_B,PIN_EL_C,PIN_EL_D);

#define MAX_POSITION 16384
#define BASE_SPEED 800
#define MAX_SPEED  1000
float Az_angle = 0;
float El_angle = 0;

#define REPORT_TIME 1000
#define MOVEMENT_TIME 2000
unsigned long report_loop = 0;
unsigned long movement_loop = 0;

#define HOME_OFF_AZ 0.0  // Offset in homing for Az
#define HOME_OFF_EL 0.0  // Offset in homing for El

Position Pointer;
Homing home;

WiFiManager wm;
bool wm_nonblocking = true; //change if this causes issues

float satLLA[3]{};
float trkLLA[3]{};
float trkDir[2]{};
float cmdAz{};
float cmdEl{};

void setup() {

  Serial.begin(115200);

  // Setup WiFi
  WiFi.mode(WIFI_STA);
  if (wm_nonblocking) wm.setConfigPortalBlocking(false);
  wm.setConnectTimeout(20);
  wm.setConfigPortalTimeout(30);
  wm.setMinimumSignalQuality(20);
  bool res;
  res = wm.autoConnect("CelestialTracker"); 
  //res = wm.autoConnect("CelestialTracker","optional_pass"); // the same as above, but with an optional password

  if (res) {
    Serial.println("WiFi Connected Successfully!");
  }
  
  lla WhereISS;
  WhereISS = WhereIsTheISS();

  // Setup Endstop Pins
  pinMode(PIN_H_AZ,INPUT_PULLUP);
  pinMode(PIN_H_EL,INPUT_PULLUP);

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
  home.goHome();
  while (!home.isHomed()) {
    home.runHoming();
  }
  // set current position to 0 (not sure what functions this uses for now)
  Pointer.setPosition(HOME_OFF_AZ,HOME_OFF_EL);
  
  report_loop = millis() + REPORT_TIME;
  movement_loop = millis() + REPORT_TIME;
}

void loop() {
// Change direction once the motor reaches target position
	if (Stepper_Az.distanceToGo() == 0 && Stepper_El.distanceToGo() == 0 && movement_loop <= millis()) {
    //delay(1000);
    /*
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
    int Az_speed = BASE_SPEED;
    int El_speed = BASE_SPEED * abs((float)El_move/(float)Az_move);
    if ( El_speed > MAX_SPEED) {
      Az_speed = BASE_SPEED*(MAX_SPEED/(float)El_speed);
      El_speed = MAX_SPEED;
    }
    Stepper_Az.setMaxSpeed(Az_speed);
    Stepper_El.setMaxSpeed(El_speed);
    
    Serial.print("Changing to new positions (Az,EL): "); Serial.print(Az_pos_new); Serial.print(" , "); Serial.println(El_pos_new);
    Serial.print("Az Move, El Move, Az Speed, El Speed: "); Serial.print(Az_move); Serial.print(" , "); Serial.print(El_move); Serial.print(" , "); Serial.print(Az_speed); Serial.print(" , "); Serial.println(El_speed);
    */
   
  // Move Random Angle (not go to)
  Stepper_Az.setCurrentPosition(0);
  Stepper_El.setCurrentPosition(0);
  int az_angle = random(0,5);
  int el_angle = random(0,5);
  Pointer.MoveDirect(az_angle,el_angle);
  Stepper_Az.moveTo(Pointer.getStepsAz());
  Stepper_El.moveTo(Pointer.getStepsEl());
  Stepper_Az.setMaxSpeed(Pointer.getSpeedAz());
  Stepper_El.setMaxSpeed(Pointer.getSpeedEl());
  Serial.print("Az Angle, El Angle, Az Steps, El Steps, Az Speed, El Speed: ");
  Serial.print(az_angle); Serial.print(" , ");
  Serial.print(el_angle); Serial.print(" , ");
  Serial.print(Pointer.getStepsAz()); Serial.print(" , ");
  Serial.print(Pointer.getStepsEl()); Serial.print(" , ");
  Serial.print(Pointer.getSpeedAz()); Serial.print(" , ");
  Serial.print(Pointer.getSpeedEl()); Serial.println();

  movement_loop = MOVEMENT_TIME + millis();
  }

  if ( millis() >= report_loop ) {
    Serial.print("Current Position (Az, El): ");
    Serial.print(Stepper_Az.currentPosition());
    Serial.print(" , ");
    Serial.print(Stepper_El.currentPosition());
    Serial.println();
    report_loop = millis() + REPORT_TIME;
  }

	// Move the motor one step
	Stepper_Az.run();
  Stepper_El.run();
  
  // Test Tracker API
  trkLLA[0] = 0.0f;   // tracker latitude, deg
  trkLLA[1] = 0.0f;   // tracker longitude, deg
  trkLLA[2] = 0.0f;   // tracker altitude, km

  satLLA[0] = -2.0f;   // satellite latitude, deg
  satLLA[1] = 0.1f;   // satellite longitude, deg
  satLLA[2] = 400.0f; // satellite altitude, km

  trkDir[0] = 0.0f;   // tracker azimuth, deg
  trkDir[1] = 45.0f;  // tracker elevation, deg

  trackerApiUpdate(trkLLA, satLLA, trkDir);
  cmdAz = trackerApiGetAzimuth();
  cmdEl = trackerApiGetElevation();
}
