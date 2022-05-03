#include "TrackerApi.h"
#include "CelestialTracker.h"

#ifdef TRACKER_SOURCE_CODE
CelestialTracker tracker{};

void trackerApiUpdate(float trackerLLADegKm[3], float satelliteLLADegKm[3], float trackerDirsAzElDeg[2]) {
    tracker.update(trackerLLADegKm, satelliteLLADegKm, trackerDirsAzElDeg);
    return;
}

float trackerApiGetAzimuth() {
    return tracker.getCmdAzDeg();
}

float trackerApiGetElevation() {
    return tracker.getCmdElDeg();
}
#endif