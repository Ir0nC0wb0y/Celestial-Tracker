# -*- coding: utf-8 -*-
"""
Created on Sun May  1 13:04:03 2022

@author: austi
"""

import json  
import urllib.request 
import time 
import ctypes

# ISS Location API Class
class IssApi:
    def __init__(self, url):
        self.url = url
        
        self.mi2ft = 5280
        self.mi2km = 1.609344
        self.km2ft = 3280.8399
        self.km2mi = 1 / 1.609344
        self.ft2mi = 1 / 5280
        self.ft2km = 1 / 3280.8399
        
        response = urllib.request.urlopen(self.url) 
        result = json.loads(response.read())
        
        self.timestamp = result.get('timestamp')
        self.latitudeDeg = result.get('latitude')
        self.longitudeDeg = result.get('longitude')
        self.altitudeKm = result.get('altitude') 
        self.altitudeMi = self.altitudeKm * self.km2mi 
        self.altitudeFt = self.altitudeKm * self.km2ft
        
    def fetch(self):
        response = urllib.request.urlopen(self.url) 
        result = json.loads(response.read())
        
        self.timestamp = result.get('timestamp')
        self.latitudeDeg = result.get('latitude')
        self.longitudeDeg = result.get('longitude')
        self.altitudeKm = result.get('altitude') 
        self.altitudeMi = self.altitudeKm * self.km2mi 
        self.altitudeFt = self.altitudeKm * self.km2ft

# Load Tracker API
trackerApi = ctypes.cdll.LoadLibrary('Tracker_DLL.dll')

# ISS API
url = "https://api.wheretheiss.at/v1/satellites/25544" 
issApi = IssApi(url)

# Main Loop
while(True):
    # Get ISS Data
    issApi.fetch()
    
    # Satellite States
    satLatDeg = issApi.latitudeDeg
    satLonDeg = issApi.longitudeDeg
    satAltKm = issApi.AltitudeKm
    
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
    
    print(f"Alt[mi]: {issApi.altitudeMi: .4f}, ", end = '')
    print(f"Lat[deg]: {issApi.latitudeDeg: .6f}, ", end = '')
    print(f"Lon[deg]: {issApi.longitudeDeg: .6f}, ", end = '')
    print(f"CmdAz[deg]: {cmdAzDeg}, ", end = '')
    print(f"CmdEl[deg]: {cmdElDeg: .6f}")
    
    time.sleep(1)
    
    
    
    
    
    