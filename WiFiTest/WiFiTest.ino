#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

#define USERNAME "rickcqy"
#define DEVICE_ID "esp8266_1"
#define DEVICE_CREDENTIAL "123456"

#define SSID "Hero"
#define SSID_PASSWORD "halfofacake"

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  pinMode(D4, OUTPUT);

  thing.add_wifi(SSID, SSID_PASSWORD);

  // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  thing["led"] << digitalPin(D4);

  // resource output example (i.e. reading a sensor value)
  thing["millis"] >> outputValue(millis());

  // more details at http://docs.thinger.io/arduino/
}

void loop() {
  thing.handle();
}
