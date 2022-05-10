#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJSON.h>


class lla {
  public:
    float latitude = 0.0;
    float longitude = 0.0;
    float altitude = 0.0;
};

lla WhereIsTheISS();