#include <ArduinoJson.h>

JsonObject& buildJson(JsonBuffer& jsonBuffer) {
  JsonObject& root = jsonBuffer.createObject();

  JsonArray& dataArray = root.createNestedArray("data");

//  for(int i=0; i<5;i++) {
    JsonObject& measuresObj = dataArray.createNestedObject();
  
    measuresObj["temperature"] = 25.0;
    measuresObj["luminosity"] = 25;
//  }

  return root;
}

void setup() {
  Serial.begin(9600);
  while(!Serial) ;
  // put your setup code here, to run once:

}

void loop() {
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& json = buildJson(jsonBuffer);

  json.printTo(Serial);
  Serial.println();
  Serial.println();
  json.prettyPrintTo(Serial);
  Serial.println();
  Serial.println();
  
  delay(5000);
}
