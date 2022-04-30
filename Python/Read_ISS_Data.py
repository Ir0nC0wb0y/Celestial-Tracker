# -*- coding: utf-8 -*-
"""
Created on Sat Apr 30 12:45:53 2022
@author: austi
"""

import json  
import urllib.request 
import time 

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

  
url = "https://api.wheretheiss.at/v1/satellites/25544" 
issApi = IssApi(url)

while(True):
    issApi.fetch()
    
    print(f"Timestamp: {issApi.timestamp}, ", end = '')
    print(f"Alt[mi]: {issApi.altitudeMi: .4f}, ", end = '')
    print(f"Lat[deg]: {issApi.latitudeDeg: .6f}, ", end = '')
    print(f"Lon[deg]: {issApi.longitudeDeg: .6f}")

    time.sleep(1)
