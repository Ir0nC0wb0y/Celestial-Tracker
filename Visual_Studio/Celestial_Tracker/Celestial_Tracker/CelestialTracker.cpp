//////////////////////////////////////////////////////////////////////////////////
// @file: CelestialTracker.cpp
//
// @brief: Celestial Tracker algorithm source
//
// @details:
//
// @ingroup: Celestial Tracker
//
// @author: Austin M. Ottaway
//////////////////////////////////////////////////////////////////////////////////
//********************************************************************************
// Include Files
#include <cmath>
#include "CelestialTracker.h"

// Macro Definitions                                                            


// Global Variables


//********************************************************************************

//------------------------------------------------------------
// function: CelestialTracker::CelestialTracker
//
// brief: CelestialTracker constructor
//
// details:
//------------------------------------------------------------
CelestialTracker::CelestialTracker(float trkLLA[3]) {
    // Initialize Tracker LLA
    _trkLatDeg = trkLLA[0]; // tracker latitude, deg
    _trkLonDeg = trkLLA[1]; // tracker longitude, deg
    _trkAltFt  = trkLLA[2]; // tracker altitude, ft

    // Initialize Tracker Position ECEF, ft
    lla2ecef(_trkLatDeg, _trkLonDeg, _trkAltFt, _trkPosEcef);

    return;
}

//------------------------------------------------------------
// function: CelestialTracker::update
//
// brief:
//
// details:
//------------------------------------------------------------
void CelestialTracker::update(float satLLA[3], float trkDir[2], float cmdDir[2]) {
    // Update Satellite LLA
    _satLatDeg = satLLA[0]; // tracker latitude, deg
    _satLonDeg = satLLA[1]; // tracker longitude, deg
    _satAltFt  = satLLA[2]; // tracker altitude, ft

    // Update Tracker Direction
    _trkDir[0] = trkDir[0]; // tracker azimuth, deg
    _trkDir[1] = trkDir[1]; // tracker elevation, deg

    // Update Satellite Direction
    calcSatDir();

    // Update Commanded Direction (where tracker needs to go)
    calcCmdDir();

    cmdDir[0] = _cmdDir[0]; // commanded azimuth, deg
    cmdDir[1] = _cmdDir[1]; // commanded elevation, deg

    return;
}

//------------------------------------------------------------
// function: CelestialTracker::getSatAzDeg
//
// brief:
//
// details:
//------------------------------------------------------------
float CelestialTracker::getSatAzDeg() const {
    return _satDir[0]; // output sat azimuth, deg
}

//------------------------------------------------------------
// function: CelestialTracker::getSatElDeg
//
// brief:
//
// details:
//------------------------------------------------------------
float CelestialTracker::getSatElDeg() const {
    return _satDir[1]; // output sat elevation, deg
}

//------------------------------------------------------------
// function: CelestialTracker::multiply33x31
//
// brief:
//
// details:
//------------------------------------------------------------
void CelestialTracker::multiply33x31(float out31[3], float in33[3][3], float in31[3]){
    //
    // OUT31 = IN33 x IN31
    //
    out31[0] = (in33[0][0] * in31[0]) + (in33[0][1] * in31[1]) + (in33[0][2] * in31[2]);
    out31[1] = (in33[1][0] * in31[0]) + (in33[1][1] * in31[1]) + (in33[1][2] * in31[2]);
    out31[2] = (in33[2][0] * in31[0]) + (in33[2][1] * in31[1]) + (in33[2][2] * in31[2]);

    return;
}

//------------------------------------------------------------
// function: CelestialTracker::ecef2enu
//
// brief:
//
// details:
//------------------------------------------------------------
void CelestialTracker::ecef2enu(float latDeg, float lonDeg, float dcmEcefToEnu[3][3]) {
    // Trig Functions
    float sinLat = sin(latDeg * CT_D2R);
    float sinLon = sin(lonDeg * CT_D2R);

    float cosLat = cos(latDeg * CT_D2R);
    float cosLon = cos(lonDeg * CT_D2R);

    // ECEF To ENU Direction Cosine Matrix
    dcmEcefToEnu[0][0] = -sinLon;
    dcmEcefToEnu[1][0] = -sinLat * cosLon;
    dcmEcefToEnu[2][0] =  cosLat * cosLon;

    dcmEcefToEnu[0][1] =  cosLon;
    dcmEcefToEnu[1][1] = -sinLat * sinLon;
    dcmEcefToEnu[2][1] =  cosLat * sinLon;

    dcmEcefToEnu[0][2] =  0.0f;
    dcmEcefToEnu[1][2] =  cosLat;
    dcmEcefToEnu[2][2] =  sinLat;

    return;
}

//------------------------------------------------------------
// function: CelestialTracker::lla2ecef
//
// brief:
//
// details:
//------------------------------------------------------------
void CelestialTracker::lla2ecef(float latDeg, float lonDeg, float altFt, float posEcef[3]) {
    // Trig Functions
    float sinLat = sin(latDeg * CT_D2R);
    float sinLon = sin(lonDeg * CT_D2R);

    float cosLat = cos(latDeg * CT_D2R);
    float cosLon = cos(lonDeg * CT_D2R);

    // Radii of Curvature
    float r0 = CT_EARTH_RADIUS_FT / sqrt(1.0f - (CT_EARTH_ECC2 * sinLat * sinLat)); // normal radius of curvature
    float r1 = r0 + altFt;
    float r2 = r0 * (1.0f - CT_EARTH_ECC2) + altFt; // meridian radius of curvature

    // ECEF Coordinates
    posEcef[0] = -r1 * (-cosLon * cosLat);
    posEcef[1] = -r1 * (-sinLon * cosLat);
    posEcef[2] = -r2 * (-sinLat);

    return;
}

//------------------------------------------------------------
// function: CelestialTracker::calcSatDir
//
// brief:
//
// details:
//------------------------------------------------------------
void CelestialTracker::calcSatDir() {
    // Convert Satellite LLA to ECEF
    lla2ecef(_satLatDeg, _satLonDeg, _satAltFt, _satPosEcef);

    // Compute Relative Position Unit Vector ECEF
    _relPosEcef[0] = _satPosEcef[0] - _trkPosEcef[0];
    _relPosEcef[1] = _satPosEcef[1] - _trkPosEcef[1];
    _relPosEcef[2] = _satPosEcef[2] - _trkPosEcef[2];

    _relPosMag = sqrt(_relPosEcef[0] * _relPosEcef[0] +
                      _relPosEcef[1] * _relPosEcef[1] +
                      _relPosEcef[2] * _relPosEcef[2]);

    _relPosEcefUnit[0] = _relPosEcef[0] / _relPosMag;
    _relPosEcefUnit[1] = _relPosEcef[1] / _relPosMag;
    _relPosEcefUnit[2] = _relPosEcef[2] / _relPosMag;

    // Compute ECEF To ENU Direction Cosine Matrix
    ecef2enu(_trkLatDeg, _trkLonDeg, _dcmEcefToEnu);

    // Compute Relative Position Unit Vector ENU
    multiply33x31(_relPosEnuUnit, _dcmEcefToEnu, _relPosEcefUnit);

    _satDir[0] = atan2(_relPosEnuUnit[0], _relPosEnuUnit[1]) * CT_R2D;
    _satDir[1] = atan(_relPosEnuUnit[2] / sqrt(_relPosEnuUnit[0] * _relPosEnuUnit[0] + 
                                               _relPosEnuUnit[1] * _relPosEnuUnit[1])) * CT_R2D;

    return;
}

//------------------------------------------------------------
// function: CelestialTracker::calcCmdDir
//
// brief:
//
// details:
//------------------------------------------------------------
void CelestialTracker::calcCmdDir() {
    // [IN]  satDir - satellite direction angles [az, el], degrees
    // [IN]  trkDir - tracker   direction angles [az, el], degrees
    // [OUT] cmdDir - command   direction angles [az, el], degrees

    // Local variables
    float azSat, elSat = 0.0f;
    float azTrk, elTrk = 0.0f;
    float azCmd, elCmd = 0.0f;
    float uSatX, uSatY, uSatZ = 0.0f;

    float sinAz, sinEl = 0.0f;
    float cosAz, cosEl = 0.0f;

    float az1, el1 = 0.0f;
    float az2, el2 = 0.0f;

    float cost1, cost2 = 0.0f;

    // Extract azimuth and elevation
    azSat = _satDir[0] * CT_D2R; // radians
    elSat = _satDir[1] * CT_D2R; // radians

    azTrk = _trkDir[0] * CT_D2R; // radians
    elTrk = _trkDir[1] * CT_D2R; // radians

    // Calculate satellite direction vector
    sinAz = sin(azSat);
    sinEl = sin(elSat);

    cosAz = cos(azSat);
    cosEl = cos(elSat);

    uSatX = sinAz * cosEl;
    uSatY = cosAz * cosEl;
    uSatZ = sinEl;

    // Compute azimuth and elevation angles (TWO ANSWERS)
    az1 = atan2(uSatX, uSatY);      // radians
    el1 = asin(uSatZ);              // radians

    az2 = (float)fmod((az1 - CT_PI) * CT_R2D, 360.0) * CT_D2R; // radians
    el2 = (float)fmod((CT_PI - el1) * CT_R2D, 360.0) * CT_D2R; // radians

    // Compute nearest approach angles
    cost1 = fabs(az1 - azTrk) + fabs(el1 - elTrk);
    cost2 = fabs(az2 - azTrk) + fabs(el2 - elTrk);

    if (cost1 < cost2) {
        azCmd = az1 * CT_R2D; // degrees
        elCmd = el1 * CT_R2D; // degrees
    }
    else {
        azCmd = az2 * CT_R2D; // degrees
        elCmd = el2 * CT_R2D; // degrees
    }

    _cmdDir[0] = azCmd;
    _cmdDir[1] = elCmd;

    return;
}