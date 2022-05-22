// Include Files
#include <Arduino.h>
#include <WiFiManager.h>
#include "pointer.h"
#include "tracker_api.h"
#include "pin_def.h"
#include "homing.h"
#include "get_data.h"

// Macro Definitions
#define MAX_POSITION 16384
#define BASE_SPEED 800
#define MAX_SPEED  1000

#define REPORT_TIME 5000
//#define MOVEMENT_TIME 2000

#define TRACKER_LATITUDE_DEG 30.481320f
#define TRACKER_LONGITUDE_DEG -86.410710f
#define TRACKER_ALT_HAE_KM 0.0f

#define ISS_UPDATE_TIME 2000

// Global Variables
AccelStepper stepper_az(HALFSTEP, PIN_AZ_A, PIN_AZ_B, PIN_AZ_C, PIN_AZ_D);
AccelStepper stepper_el(HALFSTEP, PIN_EL_A, PIN_EL_B, PIN_EL_C, PIN_EL_D);

Pointer pointer;
Homing home;

WiFiManager wm;
bool wm_nonblocking = false; //change if this causes issues

PositionLLA where_ISS;
unsigned long ISS_update = 0;

float az_angle = 0;
float el_angle = 0;

unsigned long report_loop = 0;
unsigned long movement_loop = 0;

float sat_lla[3]{};
float trk_lla[3]{};
float trk_dir[2]{};
float cmd_az{};
float cmd_el{};

// Arduino Setup Routine
void setup() {
    Serial.begin(115200);

    // Setup WiFi
    WiFi.mode(WIFI_STA);
    if (wm_nonblocking) 
        wm.setConfigPortalBlocking(false);

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
    trk_lla[0] = TRACKER_LATITUDE_DEG;   // tracker latitude, deg
    trk_lla[1] = TRACKER_LONGITUDE_DEG;  // tracker longitude, deg
    trk_lla[2] = TRACKER_ALT_HAE_KM;     // tracker altitude, km

    // Setup Endstop Pins
    pinMode(PIN_H_AZ, INPUT_PULLUP);
    pinMode(PIN_H_EL, INPUT_PULLUP);

    // Setup Steppers
    // Azimuth
    Serial.print("Setting up Az motor");
    stepper_az.setMaxSpeed(1000.0);
    stepper_az.setAcceleration(4000);
    //Stepper_Az.setSpeed(1000);
    stepper_az.moveTo(0);
    Serial.println(".....Success!");

    // Elevation
    Serial.print("Setting up El motor");
    stepper_el.setMaxSpeed(1000.0);
    stepper_el.setAcceleration(4000);
    //Stepper_El.setSpeed(1000);
    stepper_el.moveTo(0);
    Serial.println(".....Success!");

    // Homing
    home.GoHome();
    while (!home.IsHomed()) {
        home.RunHoming();
    }

    // Pointer Setup
    pointer.SetAccumulation(5.0, 5.0);
    pointer.MoveTo(HOME_OFF_AZ, HOME_OFF_EL);
    pointer.SetZeroPosition(0,0);

    report_loop = millis();
    //movement_loop = millis();
    ISS_update = millis();
}

// Arduino Loop Routine
void loop() {
    // Change direction once the motor reaches target position
    if ( millis() >= ISS_update ) {
        // Get ISS Location
        where_ISS = WhereIsTheISS(); // uncomment when below section exists

        //////////////////////////////////////////////////////////////////////
        // Tracker API
        sat_lla[0] = where_ISS.latitude;      // satellite latitude, deg
        sat_lla[1] = where_ISS.longitude;     // satellite longitude, deg
        sat_lla[2] = where_ISS.altitude;      // satellite altitude, km

        trk_dir[0] = pointer.GetCurrentAz(); // tracker azimuth, deg
        trk_dir[1] = pointer.GetCurrentEl(); // tracker elevation, deg

        TrackerApiUpdate(trk_lla, sat_lla, trk_dir);

        float move_Az = TrackerApiGetAzimuth();   // degrees
        float move_El = TrackerApiGetElevation(); // degrees
        //////////////////////////////////////////////////////////////////////

        //float move_Az = Pointer.getCurrentAz() + (float)random(15,15); // push the Az location to move to here
        //float move_El = Pointer.getCurrentEl() + (float)random(15,15); // push the El location to move to here
        Serial.print("move Az,El: "); Serial.print(move_Az); Serial.print(" , "); Serial.println(move_El);
        pointer.AccumulateMove(move_Az, move_El);
        ISS_update = millis() + ISS_UPDATE_TIME;
    }

    if ( millis() >= report_loop ) {
        Serial.print("Current Position (Az, El): ");
        Serial.print(pointer.GetCurrentAz());
        Serial.print(" , ");
        Serial.print(pointer.GetCurrentEl());
        Serial.println();
        report_loop = millis() + REPORT_TIME;
    }

    // Move the motor one step
    stepper_az.run();
    stepper_el.run();
}