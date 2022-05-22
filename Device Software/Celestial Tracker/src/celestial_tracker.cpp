//////////////////////////////////////////////////////////////////////////////////
// @file: CelestialTracker.cpp
//
// @brief: Celestial Tracker algorithm source
//
// @details: Algorithm utilized for tracking a celestial body (particularly a
//           man-made satellite). During each update call, the algorithm is
//           provided with the tracker and satellite locations as well as the 
//           tracker az/el. The commanded azimuth and elevation angles (tracker 
//           needs to go to) are computed and then allocated to be retrieved.
//
// @ingroup: Celestial Tracker
//
// @author: Austin M. Ottaway
//////////////////////////////////////////////////////////////////////////////////
//********************************************************************************
// Include Files
#include <cmath>
#include "celestial_tracker.h"
//********************************************************************************

//------------------------------------------------------------
// @function: CelestialTracker::Update
//
// @brief: Update tracker data
//
// @details: Computes commanded az/el for the tracker to go
//------------------------------------------------------------
void CelestialTracker::Update(float trkLLA[3], float satLLA[3], float trk_dir[2]) {
    // Initialize Tracker LLA
    _trk_lat_deg = trkLLA[0]; // tracker latitude, deg
    _trk_lon_deg = trkLLA[1]; // tracker longitude, deg
    _trk_alt_km  = trkLLA[2]; // tracker altitude, km

    // Update Satellite LLA
    _sat_lat_deg = satLLA[0]; // tracker latitude, deg
    _sat_lon_deg = satLLA[1]; // tracker longitude, deg
    _sat_alt_km  = satLLA[2]; // tracker altitude, km

    // Update Tracker Direction
    _trk_dir[0] = trk_dir[0]; // tracker azimuth, deg
    _trk_dir[1] = trk_dir[1]; // tracker elevation, deg

    // Update Satellite Direction
    _CalcSatDir();

    // Update Commanded Direction (where tracker needs to go)
    _CalcCmdDir();

    return;
}

//------------------------------------------------------------
// @function: CelestialTracker::getCmdAzDeg
//
// @brief: Azimuth getter
//
// @details: Gets commanded azimuth (deg) for tracker to go
//------------------------------------------------------------
float CelestialTracker::GetCmdAzDeg() const {
    return _cmd_dir[0]; // output command azimuth, deg
}

//------------------------------------------------------------
// @function: CelestialTracker::getCmdElDeg
//
// @brief: Elevation getter
//
// @details: Gets commanded elevation (deg) for tracker to go
//------------------------------------------------------------
float CelestialTracker::GetCmdElDeg() const {
    return _cmd_dir[1]; // output command elevation, deg
}

//------------------------------------------------------------
// @function: CelestialTracker::multiply33x31
//
// @brief: Basic matrix-vector multiplication
//
// @details: Outputs a 3x1 vector from multiplying a 3x1
//           vector by a 3x3 matrix
//------------------------------------------------------------
void CelestialTracker::_Multiply33x31(float out31[3], float in33[3][3], float in31[3]){
    //
    // OUT31 = IN33 x IN31
    //
    out31[0] = (in33[0][0] * in31[0]) + (in33[0][1] * in31[1]) + (in33[0][2] * in31[2]);
    out31[1] = (in33[1][0] * in31[0]) + (in33[1][1] * in31[1]) + (in33[1][2] * in31[2]);
    out31[2] = (in33[2][0] * in31[0]) + (in33[2][1] * in31[1]) + (in33[2][2] * in31[2]);

    return;
}

//------------------------------------------------------------
// @function: CelestialTracker::ecef2enu
//
// @brief: ECEF to ENU transformation
//
// @details: Uses know latitude and longitude to compute a
//           direction cosine matrix for converting ECEF to
//           ENU coordinates
//------------------------------------------------------------
void CelestialTracker::_ConvertEcefToEnu(float latDeg, float lonDeg, float dcmEcefToEnu[3][3]) {
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
// @function: CelestialTracker::lla2ecef
//
// @brief: LLA to ECEF position
//
// @details: Uses known latitude, longitude and altitude to
//           compute position in ECEF coordinates
//------------------------------------------------------------
void CelestialTracker::_Convertlla2ecef(float latDeg, float lonDeg, float altKm, float posEcef[3]) {
    // Trig Functions
    float sinLat = sin(latDeg * CT_D2R);
    float sinLon = sin(lonDeg * CT_D2R);

    float cosLat = cos(latDeg * CT_D2R);
    float cosLon = cos(lonDeg * CT_D2R);

    // Radii of Curvature
    float r0 = CT_EARTH_RADIUS_KM / sqrt(1.0f - (CT_EARTH_ECC2 * sinLat * sinLat)); // normal radius of curvature
    float r1 = r0 + altKm;
    float r2 = r0 * (1.0f - CT_EARTH_ECC2) + altKm; // meridian radius of curvature

    // ECEF Coordinates
    posEcef[0] = -r1 * (-cosLon * cosLat);
    posEcef[1] = -r1 * (-sinLon * cosLat);
    posEcef[2] = -r2 * (-sinLat);

    return;
}

//------------------------------------------------------------
// @function: CelestialTracker::calcSatDir
//
// @brief: Satellite az/el
//
// @details: Computes the azimuth and elevation angles of the
//           satellite being tracked, relative to the tracker
//           in ENU (east-north-up) coordinate frame
//------------------------------------------------------------
void CelestialTracker::_CalcSatDir() {
    // Convert Satellite LLA to ECEF
    _Convertlla2ecef(_trk_lat_deg, _trk_lon_deg, _trk_alt_km, _trk_pos_ecef);
    _Convertlla2ecef(_sat_lat_deg, _sat_lon_deg, _sat_alt_km, _sat_pos_ecef);

    // Compute Relative Position Unit Vector ECEF
    _rel_pos_ecef[0] = _sat_pos_ecef[0] - _trk_pos_ecef[0];
    _rel_pos_ecef[1] = _sat_pos_ecef[1] - _trk_pos_ecef[1];
    _rel_pos_ecef[2] = _sat_pos_ecef[2] - _trk_pos_ecef[2];

    _rel_pos_mag = sqrt(_rel_pos_ecef[0] * _rel_pos_ecef[0] +
                        _rel_pos_ecef[1] * _rel_pos_ecef[1] +
                        _rel_pos_ecef[2] * _rel_pos_ecef[2]);

    _rel_pos_ecef_unit[0] = _rel_pos_ecef[0] / _rel_pos_mag;
    _rel_pos_ecef_unit[1] = _rel_pos_ecef[1] / _rel_pos_mag;
    _rel_pos_ecef_unit[2] = _rel_pos_ecef[2] / _rel_pos_mag;

    // Compute ECEF To ENU Direction Cosine Matrix
    _ConvertEcefToEnu(_trk_lat_deg, _trk_lon_deg, _dcm_ecef_to_enu);

    // Compute Relative Position Unit Vector ENU
    _Multiply33x31(_rel_pos_enu_unit, _dcm_ecef_to_enu, _rel_pos_ecef_unit);

    _sat_dir[0] = atan2(_rel_pos_enu_unit[0], _rel_pos_enu_unit[1]) * CT_R2D;
    _sat_dir[1] = atan(_rel_pos_enu_unit[2] / sqrt(_rel_pos_enu_unit[0] * _rel_pos_enu_unit[0] + 
                                                   _rel_pos_enu_unit[1] * _rel_pos_enu_unit[1])) * CT_R2D;

    return;
}

//------------------------------------------------------------
// @function: CelestialTracker::calcCmdDir
//
// @brief: Command az/el
//
// @details: Computes the azimuth and elevation angles the
//           tracker must go to in order to point directly at
//           the satellite of interest
//------------------------------------------------------------
void CelestialTracker::_CalcCmdDir() {
    // [IN]  satDir - satellite direction angles [az, el], degrees
    // [IN]  trk_dir - tracker   direction angles [az, el], degrees
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
    azSat = _sat_dir[0] * CT_D2R; // radians
    elSat = _sat_dir[1] * CT_D2R; // radians

    azTrk = _trk_dir[0] * CT_D2R; // radians
    elTrk = _trk_dir[1] * CT_D2R; // radians

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

    _cmd_dir[0] = azCmd;
    _cmd_dir[1] = elCmd;

    return;
}