
#include <cmath>
#include "Celestial_Tracker.h"

CelestialTracker::CelestialTracker(float trkLLA[3]) {

    _trkLatDeg = trkLLA[0];
    _trkLonDeg = trkLLA[1];
    _trkAltFt  = trkLLA[2];

    return;
}

CelestialTracker::~CelestialTracker() {

    /* NEEDS CODE */

    return;
}

void CelestialTracker::update(float satLLA[3], float trkDir[2], float cmdDir[2]){
    
    _satLatDeg = satLLA[0];
    _satLonDeg = satLLA[1];
    _satAltFt  = satLLA[2];

    _trkDir[0] = trkDir[0];
    _trkDir[1] = trkDir[1];

    calcSatDir();
    calcCmdDir();

    cmdDir[0] = _cmdDir[0];
    cmdDir[1] = _cmdDir[1];

    return;
}

void CelestialTracker::ecef2enu(float latDeg, float lonDeg, float dcmEcefToEnu[3][3]) {
    // Trigonometry
    float sinLat = sin(latDeg * D2R);
    float sinLon = sin(lonDeg * D2R);

    float cosLat = cos(latDeg * D2R);
    float cosLon = cos(lonDeg * D2R);

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

void CelestialTracker::lla2ecef(float latDeg, float lonDeg, float altFt, float posEcef[3]) {
    // WGS84 Constants
    float radius = 20925646.32537562f;     // earth major radius [ft]
    float ellip  = 1.0f / 298.25722356f;   // earth ellipticity
    float ecc2   = ellip * (2.0f - ellip); // earth eccentricity squared

    // Trigonometry
    float sinLat = sin(latDeg * D2R);
    float sinLon = sin(lonDeg * D2R);

    float cosLat = cos(latDeg * D2R);
    float cosLon = cos(lonDeg * D2R);

    // Radii of Curvature
    float r0 = radius / sqrt(1.0f - (ecc2 * sinLat * sinLat)); // normal radius of curvature
    float r1 = r0 + altFt;
    float r2 = r0 * (1.0f - ecc2) + altFt; // meridian radius of curvature

    // ECEF Coordinates
    posEcef[0] = -r1 * (-cosLon * cosLat);
    posEcef[1] = -r1 * (-sinLon * cosLat);
    posEcef[2] = -r2 * (-sinLat);

    return;
}

void CelestialTracker::calcSatDir() {

     /* NEEDS CODE */

    return;
}

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
    azSat = _satDir[0] * D2R; // radians
    elSat = _satDir[1] * D2R; // radians

    azTrk = _trkDir[0] * D2R; // radians
    elTrk = _trkDir[1] * D2R; // radians

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

    az2 = (float)fmod((az1 - PI) * R2D, 360.0) * D2R; // radians
    el2 = (float)fmod((PI - el1) * R2D, 360.0) * D2R; // radians

    // Compute nearest approach angles
    cost1 = fabs(az1 - azTrk) + fabs(el1 - elTrk);
    cost2 = fabs(az2 - azTrk) + fabs(el2 - elTrk);

    if (cost1 < cost2) {
        azCmd = az1 * R2D; // degrees
        elCmd = el1 * R2D; // degrees
    }
    else {
        azCmd = az2 * R2D; // degrees
        elCmd = el2 * R2D; // degrees
    }

    _cmdDir[0] = azCmd;
    _cmdDir[1] = elCmd;

    return;
}