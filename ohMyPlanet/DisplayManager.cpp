#include "DisplayManager.h"
#include <Arduino.h>


void DisplayManager::setup(const int latchPin, const int clockPin, const int dataPin) {
  _latchPin = latchPin;
  _clockPin = clockPin;
  _dataPin  = dataPin;

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
  clear();
}


void DisplayManager::clear() {
  digitalWrite(_latchPin, LOW);
  shiftOut(_dataPin, _clockPin, MSBFIRST , 0b00000000);
  shiftOut(_dataPin, _clockPin, MSBFIRST , 0b00000000);
  digitalWrite(_latchPin, HIGH);
}


void DisplayManager::showNumber(int num) {
  clear();
  
  if(num < 0) return;

  String numStr = String(num);
  
  if(num < 10) {
    showDecimal(num, 3);
  } else if(num < 100) {
    showDecimal(numStr.charAt(0)-'0', 2);
    showDecimal(numStr.charAt(1)-'0', 3);
  } else if(num < 1000) {
    showDecimal(numStr.charAt(0)-'0', 1);
    showDecimal(numStr.charAt(1)-'0', 2);
    showDecimal(numStr.charAt(2)-'0', 3);
  } else if(num < 10000) {
    showDecimal(numStr.charAt(0)-'0', 0);
    showDecimal(numStr.charAt(1)-'0', 1);
    showDecimal(numStr.charAt(2)-'0', 2);
    showDecimal(numStr.charAt(3)-'0', 3);
  }
}


void DisplayManager::showDecimal(int dec, int pos) {
  if(dec < 0 || dec > 9) return;
  if(pos < 0 || pos > 4) return;

  digitalWrite(_latchPin, LOW);
  shiftOut(_dataPin, _clockPin, MSBFIRST , ARR_POS[pos]);
  shiftOut(_dataPin, _clockPin, MSBFIRST , ARR_NUM[dec]);
  digitalWrite(_latchPin, HIGH);
}