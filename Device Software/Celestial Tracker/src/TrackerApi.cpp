//////////////////////////////////////////////////////////////////////////////////
// @file: TrackerApi.cpp
//
// @brief: Tracker API library source
//
// @details: 
//
// @ingroup: Celestial Tracker
//
// @author: Austin M. Ottaway
//////////////////////////////////////////////////////////////////////////////////
//********************************************************************************
// Include Files
#include "TrackerApi.h"
#ifdef TRACKER_API_SOURCE_CODE
#include "CelestialTracker.h"

// Global Variables
CelestialTracker tracker{};
//********************************************************************************

//------------------------------------------------------------
// @function: trackerApiUpdate
//
// @brief: 
//
// @details: 
//------------------------------------------------------------
void trackerApiUpdate(float trackerLLADegKm[3], float satelliteLLADegKm[3], float trackerDirsAzElDeg[2]) {
    tracker.update(trackerLLADegKm, satelliteLLADegKm, trackerDirsAzElDeg);
    return;
}

//------------------------------------------------------------
// @function: trackerApiGetAzimuth
//
// @brief: 
//
// @details: 
//------------------------------------------------------------
float trackerApiGetAzimuth() {
    return tracker.getCmdAzDeg();
}

//------------------------------------------------------------
// @function: trackerApiGetElevation
//
// @brief: 
//
// @details: 
//------------------------------------------------------------
float trackerApiGetElevation() {
    return tracker.getCmdElDeg();
}
#endif