#ifndef CELESTIAL_TRACKER_H
#define CELESTIAL_TRACKER_H

#define CT_PI              3.14159265f    // pi
#define CT_D2R             0.01745329f    // degrees to radians
#define CT_R2D             57.29577951f   // radians to degrees
#define CT_EARTH_RADIUS_FT 6378.13699997f // earth major radius, km
#define CT_EARTH_ECC2      0.00669438f    // earth eccentricity squared

class CelestialTracker {
public:
    CelestialTracker() {}
    ~CelestialTracker() {}

    void update(float[3], float[3], float[2]);
    float getCmdAzDeg() const;
    float getCmdElDeg() const;

private:
    void ecef2enu(float, float, float[3][3]);
    void lla2ecef(float, float, float, float[3]);
    void multiply33x31(float[3], float[3][3], float[3]);
    void calcSatDir();
    void calcCmdDir();

    float _satDir[2]{}; // satellite direction angles [az, el], deg
    float _trkDir[2]{}; // tracker   direction angles [az, el], deg
    float _cmdDir[2]{}; // command   direction angles [az, el], deg

    float _satPosEcef[3]{}; // satellite position vector ECEF, km
    float _trkPosEcef[3]{}; // tracker   position vector ECEF, km

    float _satLatDeg{}; // satellite latitude , deg
    float _satLonDeg{}; // satellite longitude, deg
    float _satAltFt{};  // satellite altitude , km (HAE? AGL? MSL?....I have no idea)

    float _trkLatDeg{}; // tracker latitude , deg
    float _trkLonDeg{}; // tracker longitude, deg
    float _trkAltFt{};  // tracker altitude , km 

    float _relPosEcef[3]{};     // relative position vector from tracker to satellite ECEF, km
    float _relPosEcefUnit[3]{}; // unit vector form of the vector above
    float _relPosMag{};         // magnitude of the relative position difference, km

    float _relPosEnuUnit[3]{}; // unit relative position vector from tracker to satellite ENU (East-North-Up)

    float _dcmEcefToEnu[3][3]{}; // direction cosine matrix for ECEF to ENU
};

#endif