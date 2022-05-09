#include "GetData.h"



lla WhereIsTheISS() {
  
  String ISS_JSON;
  lla ISS_lla;
  
  if (WiFi.status() = WL_CONNECTED) {
    //ISS_location = GET_Request("https://api.wheretheiss.at/v1/satellites/25544");
    HTTPClient http;
    http.begin("https://api.wheretheiss.at/v1/satellites/25544");
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      ISS_JSON = http.getString();
      Serial.println(ISS_location);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end(); // Free resources
    
    // Do something with the data
    JSONVar ISS_obj = JSON.parse(ISS_JSON);
    if (JSON.typeof(ISS_obj) == "undefined") {
      Serial.println("Parsing input failed!");
    } else {
      ISS_lla.latitude = atof(ISS_obj["latitude"]); 
      ISS_lla.latitude = atof(ISS_obj["longitude"]);
      ISS_lla.latitude = atof(ISS_obj["altitude"]); // comes in km
      Serial.println("Parsed LLA");
    }
    
  } else {
    // If WiFi not connected, do something.
    Serial.println("WiFi not Connected, maintaining LLA 0's");
  }
  

  return ISS_lla;
}
