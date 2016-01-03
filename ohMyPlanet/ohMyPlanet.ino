#include "GestureRecognition.h"

const int btnPinGyro = 3;
const int ledPinGyro = 4;

GestureRecognition gesture;


void setup() {
  Serial.begin(115200);
  pinMode(btnPinGyro, INPUT);
  pinMode(ledPinGyro, OUTPUT);
  
  gesture.setup(btnPinGyro, ledPinGyro);
}


void loop() {
  gesture.run();
}
