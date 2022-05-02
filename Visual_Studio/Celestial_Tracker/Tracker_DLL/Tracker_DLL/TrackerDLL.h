#pragma once

#define TRACKER_DLL_EXPORTS

#ifdef TRACKER_DLL_EXPORTS
#define TRACKER_DLL_API __declspec(dllexport)
#else
#define TRACKER_DLL_API __declspec(dllimport)
#endif

extern "C" TRACKER_DLL_API void update();
