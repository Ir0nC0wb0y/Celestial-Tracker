#pragma once

//#define TRACKER_STATIC_LIB // define this if used in embedded code

#ifndef TRACKER_STATIC_LIB
// Distinguish between DLL export or import declaration
#ifdef TRACKER_EXPORTS
#define TRACKER_API __declspec(dllexport)
#else
#define TRACKER_API __declspec(dllimport)
#endif
// Define API with no DLL specification
#else
#define TRACKER_API
#endif

extern "C" TRACKER_API void trackerApiUpdate(float trackerLLADegKm[3], float satelliteLLADegKm[3], float trackerDirsAzElDeg[2]);

extern "C" TRACKER_API float trackerApiGetAzimuth();

extern "C" TRACKER_API float trackerApiGetElevation();
