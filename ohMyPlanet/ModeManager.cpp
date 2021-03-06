#include "ModeManager.h"

void ModeManager::setup(const byte btnPinMode, const byte ledPinModeR, const byte ledPinModeG, Spaceship *spaceship, GestureRecognition *gesture) {
  _btnPinMode   = btnPinMode;
  _ledPinModeR  = ledPinModeR;
  _ledPinModeG  = ledPinModeG;
  _spaceship    = spaceship;
  _gesture      = gesture;

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
    if( !_gesture->isComboAvailaible() ) {
      _isFriendlyMode = !_isFriendlyMode;
      _time = millis();
      // save current mode
      _spaceship->setFriendlyMode(_isFriendlyMode);
    }
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
  analogWrite(_ledPinModeG, 250);
}


void ModeManager::turnUnfriendlyMode() {
  analogWrite(_ledPinModeR, 250);
  digitalWrite(_ledPinModeG, HIGH);
}


void ModeManager::setMode(bool isFriendlyMode) {
  _isFriendlyMode = isFriendlyMode;
}
