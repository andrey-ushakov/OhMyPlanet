#include "ModeManager.h"
#include <Arduino.h>

void ModeManager::setup(const int btnPinMode, const int ledPinModeR, const int ledPinModeG) {
  _btnPinMode   = btnPinMode;
  _ledPinModeR  = ledPinModeR;
  _ledPinModeG  = ledPinModeG;
}

void ModeManager::run() {
  reading = digitalRead(_btnPinMode);
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    isFriendlyMode = !isFriendlyMode;
    time = millis();
  }

  if(isFriendlyMode) {
    turnFriendlyMode();
  } else {
    turnUnfriendlyMode();
  }

  previous = reading;

}

void ModeManager::turnFriendlyMode() {
  digitalWrite(_ledPinModeR, HIGH);
  digitalWrite(_ledPinModeG, LOW);
}

void ModeManager::turnUnfriendlyMode() {
  digitalWrite(_ledPinModeR, LOW);
  digitalWrite(_ledPinModeG, HIGH);
}

