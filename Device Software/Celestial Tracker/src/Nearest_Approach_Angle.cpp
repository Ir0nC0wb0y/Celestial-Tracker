
#include <cmath>

void getDirectionCommands(float[2], float[2], float[2]);


float satDir[2] = { 0.0f };
float trkDir[2] = { 0.0f };
float cmdDir[2] = { 0.0f };


int main(){

    float azSat, elSat = 0.0f;
    float azTrk, elTrk = 0.0f;

    azSat = 180.0; // ISS direction, degrees
    elSat = 60.0;

    azTrk = 0.0;   // Tracker direction, degrees
    elTrk = 45.0;

    satDir[0] = azSat;
    satDir[1] = elSat;

    trkDir[0] = azTrk;
    trkDir[1] = elTrk;

    getDirectionCommands(satDir, trkDir, cmdDir);

    return 0;
}

void getDirectionCommands(float satDir[2], float trkDir[2], float cmdDir[2]){

    // [IN]  satDir - satellite direction angles [az, el], degrees
    // [IN]  trkDir - tracker   direction angles [az, el], degrees
    // [OUT] cmdDir - command   direction angles [az, el], degrees

    // Local variables
    float PI  = 3.14159265f;
    float D2R = 0.01745329f;
    float R2D = 57.29577951f;

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
    azSat = satDir[0] * D2R; // radians
    elSat = satDir[1] * D2R; // radians

    azTrk = trkDir[0] * D2R; // radians
    elTrk = trkDir[1] * D2R; // radians

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

    az2 = (float)fmod((az1 + PI) * R2D, 360.0) * D2R; // radians
    el2 = (float)fmod((PI - el1) * R2D, 360.0) * D2R; // radians

    // Compute nearest approach angles
    cost1 = fabs(az1 - azTrk) + fabs(el1 - elTrk);
    cost2 = fabs(az2 - azTrk) + fabs(el2 - elTrk);

    if (cost1 < cost2) {
        azCmd = az1;
        elCmd = el1;
    }
    else {
        azCmd = az2;
        elCmd = el2;
    }

    cmdDir[0] = azCmd * R2D; // degrees
    cmdDir[1] = elCmd * R2D; // degrees

    return;
}
