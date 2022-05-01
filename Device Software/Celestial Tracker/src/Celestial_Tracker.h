#ifndef CELESTIAL_TRACKER_H
#define CELESTIAL_TRACKER_H

class CelestialTracker {
public:
    CelestialTracker(float[3]);
    ~CelestialTracker();

    void update(float[3], float[2], float[2]);

    float PI  = 3.14159265f;
    float D2R = 0.01745329f;
    float R2D = 57.29577951f;

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