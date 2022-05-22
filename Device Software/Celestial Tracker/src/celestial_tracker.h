//////////////////////////////////////////////////////////////////////////////////
// @file: CelestialTracker.h
//
// @brief: Celestial Tracker algorithm header
//
// @details: 
//
// @ingroup: Celestial Tracker
//
// @author: Austin M. Ottaway
//////////////////////////////////////////////////////////////////////////////////
#ifndef CELESTIAL_TRACKER_H
#define CELESTIAL_TRACKER_H
//********************************************************************************
// Macro Definitions
#define CT_PI              3.14159265f    // pi
#define CT_D2R             0.01745329f    // degrees to radians
#define CT_R2D             57.29577951f   // radians to degrees
#define CT_EARTH_RADIUS_KM 6378.13699997f // earth major radius, km
#define CT_EARTH_ECC2      0.00669438f    // earth eccentricity squared
//********************************************************************************

//------------------------------------------------------------
// @class: CelestialTracker
//
// @brief:
//
// @details: 
//------------------------------------------------------------
class CelestialTracker {
public:
    CelestialTracker() {}
    ~CelestialTracker() {}

    void Update(float[3], float[3], float[2]);
    float GetCmdAzDeg() const;
    float GetCmdElDeg() const;

private:
    void _ConvertEcefToEnu(float, float, float[3][3]);
    void _Convertlla2ecef(float, float, float, float[3]);
    void _Multiply33x31(float[3], float[3][3], float[3]);
    void _CalcSatDir();
    void _CalcCmdDir();

    float _sat_dir[2]{}; // satellite direction angles [az, el], deg
    float _trk_dir[2]{}; // tracker   direction angles [az, el], deg
    float _cmd_dir[2]{}; // command   direction angles [az, el], deg

    float _sat_pos_ecef[3]{}; // satellite position vector ECEF, km
    float _trk_pos_ecef[3]{}; // tracker   position vector ECEF, km

    float _sat_lat_deg{}; // satellite latitude , deg
    float _sat_lon_deg{}; // satellite longitude, deg
    float _sat_alt_km{};  // satellite altitude , km (HAE? AGL? MSL?....I have no idea)

    float _trk_lat_deg{}; // tracker latitude , deg
    float _trk_lon_deg{}; // tracker longitude, deg
    float _trk_alt_km{};  // tracker altitude , km 

    float _rel_pos_ecef[3]{};     // relative position vector from tracker to satellite ECEF, km
    float _rel_pos_ecef_unit[3]{}; // unit vector form of the vector above
    float _rel_pos_mag{};         // magnitude of the relative position difference, km

    float _rel_pos_enu_unit[3]{}; // unit relative position vector from tracker to satellite ENU (East-North-Up)

    float _dcm_ecef_to_enu[3][3]{}; // direction cosine matrix for ECEF to ENU
};
#endif