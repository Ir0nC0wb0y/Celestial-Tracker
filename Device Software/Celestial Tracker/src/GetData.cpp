#include "GetData.h"


lla WhereIsTheISS() {
  
  String ISS_JSON;
  lla ISS_lla;
  
  //if (WiFi.status() = WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://api.wheretheiss.at/v1/satellites/25544");
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
      //Serial.print("HTTP Response code: ");
      //Serial.println(httpResponseCode);
      ISS_JSON = http.getString();
      //Serial.println(ISS_JSON);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end(); // Free resources
    
    //Serial.println("Starting JSON Deserialization");
    // Do something with the data
    StaticJsonDocument<500> ISS_doc;
    DeserializationError error = deserializeJson(ISS_doc, ISS_JSON);
    //JSONVar ISS_obj = JSON.parse(ISS_JSON);
    if (error) {
      Serial.println("Parsing input failed!");
    } else {
      ISS_lla.latitude = ISS_doc["latitude"];
      ISS_lla.longitude = ISS_doc["longitude"];
      ISS_lla.altitude = ISS_doc["altitude"]; // comes in km
      //Serial.println("Parsed LLA");
    }
    
  /*
  } else {
    // If WiFi not connected, do something.
    Serial.println("WiFi not Connected, maintaining LLA 0's");
  }
  */
  

  return ISS_lla;
}
