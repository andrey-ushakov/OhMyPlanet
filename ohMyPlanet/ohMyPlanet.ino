#include <Thread.h>

#include "SPI.h"
#include "PN532_SPI.h"
#include "snep.h"
#include "NdefMessage.h"


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

const int latchPin = A0;
const int clockPin = A1;
const int dataPin = A2;

Thread nfcThread = Thread();

GestureRecognition *gesture = new GestureRecognition();
ModeManager *modeManager = new ModeManager();
Multiplex7Seg4Digit disp(latchPin, clockPin, dataPin);
Spaceship *spaceship = new Spaceship();

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
uint8_t ndefBuf[128];


int nfcCounter = 0;   // debug variable
void nfcThreadCallback() {
  nfcLoop();
}


void setup() {
  Serial.begin(115200);
  
  gesture->setup(btnPinGyro, ledPinGyro, spaceship);
  modeManager->setup(btnPinMode, ledPinModeR, ledPinModeG, spaceship, gesture);
  modeManager->setMode(spaceship->isFriendlyMode());
  
  nfcThread.onRun(nfcThreadCallback);
  nfcThread.setInterval(500);
}

void loop() {
    
  modeManager->run();
  gesture->run();
  disp.displayNum(spaceship->resources());

  if( gesture->isComboAvailaible() || spaceship->isFriendlyMode() ) {
    if(nfcThread.shouldRun())
      nfcThread.run();
  }
  
}


void nfcLoop() {
  Serial.print(++nfcCounter);
  
  int16_t result;
  result = nfc.poll(50);
  
  if (result == 1) { // client
    Serial.println(": Client peer");
  }
  else if (result == 2) { // server
    Serial.println(": Server peer");
  }
  else{
    Serial.println(": Timeout");
  }
    
}

