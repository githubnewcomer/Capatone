#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

#define USERNAME "rickcqy"
#define DEVICE_ID "esp8266_1"
#define DEVICE_CREDENTIAL "123456"

#define SSID "Hero"
#define SSID_PASSWORD "halfofacake"

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

enum errorEnum {
  SENSOR_VAL_FORM
};
int errorCode = 0;

void setup() {
  Serial.begin(9600);
  thing.add_wifi(SSID, SSID_PASSWORD);
  thing["sensor"] >> outputValue(read_from_sensor());
  thing["error"] >> outputValue(errorCode);
}

void loop() {
  thing.handle();
}

int read_from_sensor(){
  if(Serial.available() > 0) {
    int tempData = Serial.read();
    if(tempData != 1 && tempData != 0){
      errorCode = SENSOR_VAL_FORM;
    }else{
      return tempData;
    }
  }
}

