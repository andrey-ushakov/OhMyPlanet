#pragma once

#include <Arduino.h>
#include "Spaceship.h"
#include "GestureRecognition.h"

class ModeManager {
  public:
  void setup(const byte btnPinMode, const byte ledPinModeR, const byte ledPinModeG, Spaceship *spaceship, GestureRecognition *gesture);
  void run();
  void setMode(bool isFriendlyMode);

  private:
  byte _btnPinMode;
  byte _ledPinModeR;
  byte _ledPinModeG;
  bool _isFriendlyMode = true;    // TODO init from static memory

  byte _reading;           // the current reading from the input pin
  byte _previous = LOW;    // the previous reading from the input pin
  long _time = 0;         // the last time the output pin was toggled
  long _debounce = 200;   // the debounce time, increase if the output flickers

  Spaceship *_spaceship;
  GestureRecognition *_gesture;

  void turnFriendlyMode();
  void turnUnfriendlyMode();
};

