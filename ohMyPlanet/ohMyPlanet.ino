#include "GestureRecognition.h"
#include "ModeManager.h"
#include "DisplayManager.h"
#include "ComboManager.h"

const int btnPinGyro  = 3;
const int ledPinGyro  = 4;

const int btnPinMode  = 5;
const int ledPinModeR = 6;
const int ledPinModeG = 7;

const int latchPin = 8;
const int clockPin = 12;
const int dataPin = 11;

GestureRecognition gesture;
ModeManager modeManager;
DisplayManager displayManager;

void setup() {
  Serial.begin(115200);
  
  displayManager.setup(latchPin, clockPin, dataPin);
  modeManager.setup(btnPinMode, ledPinModeR, ledPinModeG);
  gesture.setup(btnPinGyro, ledPinGyro);
}


void loop() {
  modeManager.run();
  gesture.run();
  displayManager.showNumber(1234);
}



