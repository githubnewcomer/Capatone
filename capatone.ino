#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#define USERNAME "xiejing941122"
#define DEVICE_ID "esp8266_1"
#define DEVICE_CREDENTIAL "123456"

#define SSID_USERNAME "Avalon"
#define SSID_PASSWORD "ExcaliburAK"

int microwave = D0; // D0
int pir = D5; //D5
int outputPin = D1;
int n = 1;

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

enum errorEnum {
  SENSOR_VAL_FORM
};


int op_status; // 0 - unoccupied; 1 - occupied
int sum;
int count;
int trigger;


void setup() {
  pinMode(microwave, INPUT);
  pinMode(pir, INPUT);
  pinMode(outputPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(9600);
  thing.add_wifi(SSID_USERNAME, SSID_PASSWORD);
  thing["sensor"] >> outputValue(read_from_sensor());
  thing["op_status"] >> outputValue(op_status);
  op_status=0;
  count = 0;
  sum = 0;
  trigger = 0;

  digitalWrite(outputPin, LOW);
}


int read_from_sensor(){
    if(digitalRead(pir) == HIGH){
      return 1;
    }else{
      return 0;
    }
}

int queryNtimes(){
  int i = 0;
  int on_count = 0;
  int off_count = 1;
  for(i=0;i<n;i++){
    if(digitalRead(microwave) == HIGH){
      on_count++;
    }else{
      off_count++;
    }
    delay(6000);
  }
  if(on_count > off_count){
    return 1;
  }else{
    return 0;
  }
}

// the loop function runs over and over again forever
void loop() {
  //digitalWrite(outputPin, HIGH);

  thing.handle();

  if(op_status == 1) {
    digitalWrite(LED_BUILTIN, LOW);

    if(digitalRead(microwave) == HIGH && digitalRead(pir) == LOW) {
      count++;
    }
    else {
      count = 0;
    }

    if(count == 1000000) {
      op_status = 0;
      count = 0;
      digitalWrite(outputPin, LOW);
    }
  }
  else {
    digitalWrite(LED_BUILTIN, HIGH);
    if(trigger && count < 10000000) {
      //digitalWrite(outputPin, HIGH);
      count++;
    }
    if(count == 10000000) {
      trigger = 0;
      count = 0;
      digitalWrite(outputPin, LOW);
    }
    
    // microwave movement
    if(digitalRead(microwave) == LOW) {
      // Turn on PIR
      if(trigger == 0) {
        trigger = 1;
        digitalWrite(outputPin, HIGH);
        //delay(3000); // PIR warmup time
      }
      if(count < 10000) {
        ; // PIR warmup
      }
      else if(digitalRead(pir) == HIGH) {
        trigger = 0;
        count = 0;
        op_status = 1;
      }
      
    }
  }

  /*if(op_status == 1) {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if(digitalRead(microwave) == HIGH){ //no movement
    sum++;
  }
  count ++;
  if(count == 10000){
    if(sum == 10000){
      op_status = 0;
    }else{
      op_status = 1;
    }
    count = 0;
    sum = 0;
  }*/
}
