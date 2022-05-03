#include "../Tracker_DLL/Tracker_DLL/TrackerApi.h"

int main(){
    float trkLLA[3]{};
    float satLLA[3]{};
    float trkDir[2]{};
    float cmdAz{};
    float cmdEl{};

    trkLLA[0] = 0.0f;   // tracker latitude, deg
    trkLLA[1] = 0.0f;   // tracker longitude, deg
    trkLLA[2] = 0.0f;   // tracker altitude, km

    satLLA[0] = 0.0f;   // satellite latitude, deg
    satLLA[1] = 0.0f;   // satellite longitude, deg
    satLLA[2] = 400.0f; // satellite altitude, km

    trkDir[0] = 0.0f;   // tracker azimuth, deg
    trkDir[1] = 45.0f;  // tracker elevation, deg

    trackerApiUpdate(trkLLA, satLLA, trkDir);
    cmdAz = trackerApiGetAzimuth();
    cmdEl = trackerApiGetElevation();

    return 0;
}
