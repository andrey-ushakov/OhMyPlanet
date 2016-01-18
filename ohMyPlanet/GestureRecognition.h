#pragma once

#include "I2Cdev.h"
#include "Spaceship.h"

#define MAX_COMBO_LENGTH 4

#define GESTURE_DOWN  111
#define GESTURE_UP    112
#define GESTURE_LEFT  113
#define GESTURE_RIGHT 114

class GestureRecognition {
  public:
    void setup(const byte btnPinGyro, const byte ledPinGyro, Spaceship *spaceship);
    void run();
    // true if recognized 4 gestures
    bool isComboAvailaible();
    // return combo array
    byte* getCombo();
    void resetCombo();

  private:
    byte _btnPinGyro;
    byte _ledPinGyro;
    Spaceship *_spaceship;

    byte _comboArr[MAX_COMBO_LENGTH];       // gestures sequence
    byte _comboLength = 0;   // number of recognized gestures

    bool _isNoGesture = false;

    void analyseData();
    void recognizeGesture();
};
