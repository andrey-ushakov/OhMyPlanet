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

const int btnPinNfc  = 13;

Thread nfcThread = Thread();

GestureRecognition *gesture = new GestureRecognition();
ModeManager *modeManager = new ModeManager();
Multiplex7Seg4Digit disp(latchPin, clockPin, dataPin);
Spaceship *spaceship;
ComboManager comboManager = ComboManager();

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);

bool isNfcBtnPressed = false;

int nfcCounter = 0;   // debug variable
void nfcThreadCallback() {
  nfcLoop();
}


void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(3));
  
  pinMode(btnPinNfc, INPUT);

  spaceship = new Spaceship();    // init after randomSeed for random id generation
  
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

  if(digitalRead(btnPinNfc) == HIGH && !isNfcBtnPressed) { // btn NFC was pressed
    isNfcBtnPressed = true;
  }
  
  if( isNfcBtnPressed && (gesture->isComboAvailaible() || spaceship->isFriendlyMode()) ) {
    if(nfcThread.shouldRun())
      nfcThread.run();
  } else {
    isNfcBtnPressed = false;
  }
  
}



uint8_t ndefBuf[128];
int16_t result;

void nfcLoop() {
  Serial.print(++nfcCounter);
  
  result = nfc.poll();

  // ::: Client Peer :::
  if (result == 1) {
    Serial.println(": Client peer");
    NdefMessage message = buildNdefMessage();
    int messageSize = message.getEncodedSize();
    message.encode(ndefBuf);
    nfc.put(ndefBuf, messageSize); // send character data

    Serial.println("Client : Msg was sent");
  }

  // ::: Server Peer :::
  else if (result == 2) {
    Serial.println(": Server peer");
    int msgSize = nfc.serve(ndefBuf, sizeof(ndefBuf));
    if (msgSize > 0) {
        NdefMessage msg  = NdefMessage(ndefBuf, msgSize);
        msg.print();
        Serial.println("\nSuccess");
    } else {
        Serial.println("failed");
    }
    delay(250);
  }

  // ::: Timeout :::
  else{
    Serial.println(": Timeout");
  }
  
}


NdefMessage buildNdefMessage() {
  NdefMessage message = NdefMessage();
  message.addTextRecord( String(spaceship->id()) + ':' + spaceship->nickname() );
  message.addTextRecord( String(spaceship->isFriendlyMode()) );
  if( !spaceship->isFriendlyMode() ) {
    Combo usedCombo = comboManager.getCombo(gesture->getCombo());
    int dmg = random(usedCombo.range.min, usedCombo.range.max + 1);
    message.addTextRecord( String(dmg) + ':' + usedCombo.name );
  }

  return message;
}

