#ifndef CELESTIAL_TRACKER_H
#define CELESTIAL_TRACKER_H

#define CT_PI              3.14159265f        // pi
#define CT_D2R             0.01745329f        // degrees to radians
#define CT_R2D             57.29577951f       // radians to degrees
#define CT_EARTH_RADIUS_FT 20925646.32537562f // earth major radius, ft
#define CT_EARTH_ECC2      0.00669438f        // earth eccentricity squared

class CelestialTracker {
public:
    CelestialTracker(float[3]);
    ~CelestialTracker();

    void update(float[3], float[2], float[2]);

private:
    void ecef2enu(float, float, float[3][3]);
    void lla2ecef(float, float, float, float[3]);
    void calcSatDir();
    void calcCmdDir();

    float _satDir[2]{};
    float _trkDir[2]{};
    float _cmdDir[2]{};

    float _satPosEcef[3]{};
    float _trkPosEcef[3]{};

    float _satLatDeg{};
    float _satLonDeg{};
    float _satAltFt{};

    float _trkLatDeg{};
    float _trkLonDeg{};
    float _trkAltFt{};

    float _relPosEcef[3]{};
    float _relPosEnu[3]{};

    float _dcmEcefToEnu[3][3]{};
};

#endif