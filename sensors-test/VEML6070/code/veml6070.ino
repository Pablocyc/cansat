#include <Wire.h>
#include "Adafruit_VEML6070.h"

Adafruit_VEML6070 uv = Adafruit_VEML6070();

void setup() {
  Serial.begin(9600);
  Serial.println("VEML6070 Test");
  uv.begin(VEML6070_1_T);  // pass in the integration time constant
}


void loop() {
  long senUV = ( uv.readUV() / 65535 ) * 100;
  Serial.print("UV light level: "); 
  Serial.print(senUV); Serial.println("%");
  
  delay(1000);
}