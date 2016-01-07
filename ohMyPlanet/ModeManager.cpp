#include "ModeManager.h"

void ModeManager::setup(const int btnPinMode, const int ledPinModeR, const int ledPinModeG) {
  _btnPinMode   = btnPinMode;
  _ledPinModeR  = ledPinModeR;
  _ledPinModeG  = ledPinModeG;

  pinMode(btnPinMode, INPUT);
  pinMode(ledPinModeR, OUTPUT);
  pinMode(ledPinModeG, OUTPUT);
    // off led rgb
  digitalWrite(ledPinModeR, HIGH);
  digitalWrite(ledPinModeG, HIGH);
}


void ModeManager::run() {
  _reading = digitalRead(_btnPinMode);
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (_reading == HIGH && _previous == LOW && millis() - _time > _debounce) {
    _isFriendlyMode = !_isFriendlyMode;
    _time = millis();
  }

  if(_isFriendlyMode) {
    turnFriendlyMode();
  } else {
    turnUnfriendlyMode();
  }

  _previous = _reading;

}


void ModeManager::turnFriendlyMode() {
  digitalWrite(_ledPinModeR, HIGH);
  digitalWrite(_ledPinModeG, LOW);
}


void ModeManager::turnUnfriendlyMode() {
  digitalWrite(_ledPinModeR, LOW);
  digitalWrite(_ledPinModeG, HIGH);
}


bool ModeManager::isFriendlyMode() {
  return _isFriendlyMode;
}