//////////////////////////////////////////////////////////////////////////////////
// @file: TrackerApi.h
//
// @brief: Tracker API library header
//
// @details: 
//
// @ingroup: Celestial Tracker
//
// @author: Austin M. Ottaway
//////////////////////////////////////////////////////////////////////////////////
#ifndef TRACKER_API_H
#define TRACKER_API_H
//********************************************************************************
// Macro Definitions
#define TRACKER_API_UTILIZE_SRC  // define when using TrackerApi source code (e.g. Arduino upload event)
//#define TRACKER_API_DLL_EXPORTS  // define when building a new TrackerApi dll
//#define TRACKER_API_DLL_IMPORTS  // define when using TrackerApi dll

#ifndef TRACKER_API_UTILIZE_SRC
#ifdef TRACKER_API_DLL_EXPORTS
#define TRACKER_API_SOURCE_CODE  // gives permission to compile TrackerApi src
#define TRACKER_API __declspec(dllexport)  // dll exports declaration
#endif
#ifdef TRACKER_API_DLL_IMPORTS
#define TRACKER_API __declspec(dllimport)  // dll imports declaration
#endif
#else
#define TRACKER_API_SOURCE_CODE  // gives permission to compile TrackerApi src
#define TRACKER_API  // empty declaration (no dll being built or used)
#endif
//********************************************************************************

// API Function Declarations
extern "C" {
    TRACKER_API void trackerApiUpdate(float trackerLLADegKm[3], 
        float satelliteLLADegKm[3], float trackerDirsAzElDeg[2]);
    TRACKER_API float trackerApiGetAzimuth();
    TRACKER_API float trackerApiGetElevation();
}
#endif