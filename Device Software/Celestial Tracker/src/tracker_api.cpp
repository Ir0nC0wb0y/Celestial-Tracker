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
#include "tracker_api.h"
#ifdef TRACKER_API_SOURCE_CODE
#include "celestial_tracker.h"

// Global Variables
static CelestialTracker tracker{};
//********************************************************************************

//------------------------------------------------------------
// @function: trackerApiUpdate
//
// @brief: 
//
// @details: 
//------------------------------------------------------------
void TrackerApiUpdate(float trackerLLADegKm[3], float satelliteLLADegKm[3], float trackerDirsAzElDeg[2]) {
    tracker.Update(trackerLLADegKm, satelliteLLADegKm, trackerDirsAzElDeg);
    return;
}

//------------------------------------------------------------
// @function: trackerApiGetAzimuth
//
// @brief: 
//
// @details: 
//------------------------------------------------------------
float TrackerApiGetAzimuth() {
    return tracker.GetCmdAzDeg();
}

//------------------------------------------------------------
// @function: trackerApiGetElevation
//
// @brief: 
//
// @details: 
//------------------------------------------------------------
float TrackerApiGetElevation() {
    return tracker.GetCmdElDeg();
}
#endif