#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


typedef struct {
    float latitude = 0.0;
    float longitude = 0.0;
    float altitude = 0.0;
} PositionLLA;

PositionLLA WhereIsTheISS();