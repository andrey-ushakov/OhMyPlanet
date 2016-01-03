#include "GestureRecognition.h"
#include "src/ModeManager.cpp"

const int btnPinGyro  = 3;
const int ledPinGyro  = 4;

const int btnPinMode  = 5;
const int ledPinModeR = 6;
const int ledPinModeG = 7;

GestureRecognition gesture;
ModeManager modeManager;

void setup() {
  Serial.begin(115200);
  pinMode(btnPinGyro, INPUT);
  pinMode(ledPinGyro, OUTPUT);
  pinMode(btnPinMode, INPUT);
  pinMode(ledPinModeR, OUTPUT);
  pinMode(ledPinModeG, OUTPUT);

  // off led rgb
  digitalWrite(ledPinModeR, HIGH);
  digitalWrite(ledPinModeG, HIGH);
  
  modeManager.setup(btnPinMode, ledPinModeR, ledPinModeG);
  gesture.setup(btnPinGyro, ledPinGyro);
}


void loop() {
  modeManager.run();
  gesture.run();
}



