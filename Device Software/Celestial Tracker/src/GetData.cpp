#include "GetData.h"

String ISS_location;

lla WhereIsTheISS() {
  //ISS_location = GET_Request("https://api.wheretheiss.at/v1/satellites/25544");

  HTTPClient http;
  http.begin("https://api.wheretheiss.at/v1/satellites/25544");
  int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    ISS_location = http.getString();
    Serial.println(ISS_location);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  JSONVar ISS_obj = JSON.parse(ISS_location);
  if (JSON.typeof(ISS_obj) == "undefined") {
    Serial.println("Parsing input failed!");
  }

  lla ISS_lla;
  ISS_lla.latitude = atof(ISS_obj["latitude"]); 
  ISS_lla.latitude = atof(ISS_obj["longitude"]);
  ISS_lla.latitude = atof(ISS_obj["altitude"]); // comes in km

  return ISS_lla;
}