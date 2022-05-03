#pragma once

#define TRACKER_SOURCE_CODE // define this if using source instead of dll
//#define TRACKER_DLL_EXPORTS // define this when building a new dll (overrides above macro)

#ifdef TRACKER_DLL_EXPORTS
// Protection against potential user error;
// TRACKER_STATIC_LIB and TRACKER_DLL_EXPORTS
// must not be both defined at the same time
#ifdef TRACKER_SOURCE_CODE
#undef TRACKER_SOURCE_CODE
#endif
#endif

#ifndef TRACKER_SOURCE_CODE
// Distinguish between DLL export or import declaration
#ifdef TRACKER_DLL_EXPORTS
#define TRACKER_API __declspec(dllexport)
#else
#define TRACKER_API __declspec(dllimport)
#endif
#else
// Define API with no DLL specification
#define TRACKER_API
#endif

extern "C" TRACKER_API void trackerApiUpdate(float trackerLLADegKm[3], float satelliteLLADegKm[3], float trackerDirsAzElDeg[2]);

extern "C" TRACKER_API float trackerApiGetAzimuth();

extern "C" TRACKER_API float trackerApiGetElevation();
