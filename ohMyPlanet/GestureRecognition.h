#pragma once

#include "I2Cdev.h"

#define MAX_COMBO_LENGTH 4

#define GESTURE_DOWN  111
#define GESTURE_UP    112
#define GESTURE_LEFT  113
#define GESTURE_RIGHT 114

class GestureRecognition {
  public:
    void setup(const int btnPinGyro, const int ledPinGyro);
    void run();
    // true if recognized 4 gestures
    bool isComboAvailaible();
    // return combo array
    int* getCombo();

  private:
    //void dmpDataReady();
    int _btnPinGyro;
    int _ledPinGyro;

    int _comboArr[MAX_COMBO_LENGTH];       // gestures sequence
    int _comboLength = 0;   // number of recognized gestures

    bool _isNoGesture = false;

    void analyseData();
    void recognizeGesture();
};
