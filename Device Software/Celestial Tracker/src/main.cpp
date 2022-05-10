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

#define REPORT_TIME 5000
//#define MOVEMENT_TIME 2000
unsigned long report_loop = 0;
unsigned long movement_loop = 0;

#define TRACKER_LATITUDE_DEG 30.481320f
#define TRACKER_LONGITUDE_DEG -86.410710f
#define TRACKER_ALT_HAE_KM 0.0f

Position Pointer;
Homing home;

WiFiManager wm;
bool wm_nonblocking = false; //change if this causes issues

float satLLA[3]{};
float trkLLA[3]{};
float trkDir[2]{};
float cmdAz{};
float cmdEl{};


#define ISS_UPDATE_TIME 2000
unsigned long ISS_update = 0;
lla WhereISS;


void setup() {

  Serial.begin(115200);

  // Setup WiFi
  WiFi.mode(WIFI_STA);
  if (wm_nonblocking) wm.setConfigPortalBlocking(false);
  wm.setConnectTimeout(20);
  wm.setConfigPortalTimeout(300);
  //wm.setMinimumSignalQuality(20);
  bool res;
  res = wm.autoConnect("CelestialTracker"); 
  if (res) {
    Serial.println("WiFi Connected Successfully!");
  } else {
    Serial.println("Failed to Connect WiFi!");
    ESP.restart();
  }
  
  
  /////////////////////////////////////////////////////////////
  // Tracker API Setup
  trkLLA[0] = TRACKER_LATITUDE_DEG;   // tracker latitude, deg
  trkLLA[1] = TRACKER_LONGITUDE_DEG;  // tracker longitude, deg
  trkLLA[2] = TRACKER_ALT_HAE_KM;     // tracker altitude, km
  

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
  
  // Pointer Setup
  Pointer.SetAccumulation(5.0,5.0);
  Pointer.MoveTo(HOME_OFF_AZ,HOME_OFF_EL);
  Pointer.setZeroPosition(0,0);
  
  report_loop = millis();
  //movement_loop = millis();
  ISS_update = millis();

}

void loop() {
// Change direction once the motor reaches target position
  if ( millis() >= ISS_update ) {
    // Get ISS Location
    WhereISS = WhereIsTheISS(); // uncomment when below section exists

    //////////////////////////////////////////////////////////////////////
    // Tracker API
    satLLA[0] = WhereISS.latitude;      // satellite latitude, deg
    satLLA[1] = WhereISS.longitude;     // satellite longitude, deg
    satLLA[2] = WhereISS.altitude;      // satellite altitude, km

    trkDir[0] = Pointer.getCurrentAz(); // tracker azimuth, deg
    trkDir[1] = Pointer.getCurrentEl(); // tracker elevation, deg

    trackerApiUpdate(trkLLA, satLLA, trkDir);

    float move_Az = trackerApiGetAzimuth();   // degrees
    float move_El = trackerApiGetElevation(); // degrees
    //////////////////////////////////////////////////////////////////////

    //float move_Az = Pointer.getCurrentAz() + (float)random(15,15); // push the Az location to move to here
    //float move_El = Pointer.getCurrentEl() + (float)random(15,15); // push the El location to move to here
    Serial.print("move Az,El: "); Serial.print(move_Az); Serial.print(" , "); Serial.println(move_El);
    Pointer.AccumulateMove(move_Az,move_El);
    ISS_update = millis() + ISS_UPDATE_TIME;
  }
  
  if ( millis() >= report_loop ) {
    Serial.print("Current Position (Az, El): ");
    Serial.print(Pointer.getCurrentAz());
    Serial.print(" , ");
    Serial.print(Pointer.getCurrentEl());
    Serial.println();
    report_loop = millis() + REPORT_TIME;
  }

  // Move the motor one step
  Stepper_Az.run();
  Stepper_El.run();
  
}
