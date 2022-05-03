# -*- coding: utf-8 -*-
"""
Created on Sun May  1 13:04:03 2022

@author: austi
"""

import ctypes

# Load Tracker API
trackerApi = ctypes.cdll.LoadLibrary('Tracker_DLL.dll')

# Satellite States
satLatDeg = -1.0
satLonDeg = 0.1
satAltKm = 400.0

satLLA = (ctypes.c_float * 3)(*[satLatDeg, satLonDeg, satAltKm])

# Tracker States
trkLatDeg = 0.0
trkLonDeg = 0.0
trkAltKm = 0.0

trkAzDeg = 0.0
trkElDeg = 45.0

trkLLA = (ctypes.c_float * 3)(*[trkLatDeg, trkLonDeg, trkAltKm])
trkDir = (ctypes.c_float * 2)(*[trkAzDeg, trkElDeg])

# Tracker API Update
trackerApi.trackerApiGetAzimuth.restype = ctypes.c_float
trackerApi.trackerApiGetElevation.restype = ctypes.c_float

trackerApi.trackerApiUpdate(trkLLA, satLLA, trkDir)

cmdAzDeg = trackerApi.trackerApiGetAzimuth()
cmdElDeg = trackerApi.trackerApiGetElevation()

print(cmdAzDeg)
print(cmdElDeg)