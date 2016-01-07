#pragma once

#include "I2Cdev.h"
#include "SpeakerManager.h"

#define MAX_COMBO_LENGTH 4

#define GESTURE_DOWN  111
#define GESTURE_UP    112
#define GESTURE_LEFT  113
#define GESTURE_RIGHT 114

class GestureRecognition {
  public:
    void setup(const int btnPinGyro, const int ledPinGyro, SpeakerManager speaker);
    void run();
    // true if recognized 4 gestures
    bool isComboAvailaible();
    // return combo array
    int* getCombo();

  private:
    int _btnPinGyro;
    int _ledPinGyro;
    SpeakerManager _speaker;

    int _comboArr[MAX_COMBO_LENGTH];       // gestures sequence
    int _comboLength = 0;   // number of recognized gestures

    bool _isNoGesture = false;

    void analyseData();
    void recognizeGesture();
};
