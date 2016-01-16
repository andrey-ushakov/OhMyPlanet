#include <Multiplex7Seg4Digit.h>
#include "GestureRecognition.h"
#include "ModeManager.h"
#include "ComboManager.h"
#include "Spaceship.h"

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
Multiplex7Seg4Digit disp(latchPin, clockPin, dataPin);
Spaceship *spaceship = new Spaceship();

void setup() {
  Serial.begin(115200);
  
  modeManager.setup(btnPinMode, ledPinModeR, ledPinModeG, spaceship);
  gesture.setup(btnPinGyro, ledPinGyro);
  modeManager.setMode(spaceship->isFriendlyMode());
}


void loop() {
  modeManager.run();
  gesture.run();
  disp.displayNum(spaceship->resources());
}



