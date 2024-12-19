#include <ESP32Servo.h>
const int servoPin = 15;

Servo servo1;

void setup() {

  Serial.begin(115200);
  servo1.attach(servoPin);
}

void loop() {
  if(Serial.available()){
    int angle = Serial.parseInt();
    servo1.write(angle);
  }
  delay(100);
  
}