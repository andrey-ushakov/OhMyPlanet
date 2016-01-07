#include <Multiplex7Seg4Digit.h>
#include "GestureRecognition.h"
#include "ModeManager.h"
#include "ComboManager.h"
#include "SpeakerManager.h"

const int btnPinGyro  = 3;
const int ledPinGyro  = 4;

const int btnPinMode  = 5;
const int ledPinModeR = 6;
const int ledPinModeG = 7;

const int speakerPin = 9;

const int latchPin = 8;
const int clockPin = 12;
const int dataPin = 11;


GestureRecognition gesture;
ModeManager modeManager;
SpeakerManager speaker;
Multiplex7Seg4Digit disp(latchPin, clockPin, dataPin);


void setup() {
  Serial.begin(115200);
  
  modeManager.setup(btnPinMode, ledPinModeR, ledPinModeG);
  gesture.setup(btnPinGyro, ledPinGyro, speaker);
  speaker.setup(speakerPin);
}
bool flag = false;

void loop() {
  modeManager.run();
  gesture.run();
  speaker.playMelody();
  disp.displayNum(9876);

  if(!flag) {
    speaker.setMelody(MELODY_WIN);
    flag = true;
  }
}



