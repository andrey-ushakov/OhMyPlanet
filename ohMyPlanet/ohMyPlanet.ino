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

  spaceship = new Spaceship();
  
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
  //Serial.println(spaceship->id());

  /*if( gesture->isComboAvailaible() || spaceship->isFriendlyMode() ) {
    if(nfcThread.shouldRun())
      nfcThread.run();
  }*/

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



uint8_t header[] = {0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x3a, 0x00};

uint8_t ndefBuf[128];
uint8_t bufferMsg[128];
int msgLength = 0;
int16_t result;
uint8_t length;
int16_t length16;

void nfcLoop() {
  Serial.print(++nfcCounter);
  
  result = nfc.poll();

  // ::: Client Peer :::
  if (result == 1) {
    Serial.println(": Client peer");
    //NdefMessage message = NdefMessage();
    //message.addTextRecord("hello world!");
    NdefMessage message = buildNdefMessage();
    int messageSize = message.getEncodedSize();
    message.encode(ndefBuf);
    nfc.put(ndefBuf, messageSize); // send character data
    
    /*bufferMsg[0] = spaceship->id();
    msgLength = 1;
    length = setCharNdef(ndefBuf, bufferMsg, msgLength*sizeof(uint8_t));
    nfc.put(ndefBuf, length); // send character data
    */
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
    /*length16 = nfc.serve(ndefBuf, sizeof(ndefBuf)); // get peer character data
    if(length16 > 0) {
      //print_ndef(ndefBuf, length16);
      print_ndef_payload(ndefBuf, length16);
    }*/
    delay(250);
  }

  // ::: Timeout :::
  else{
    Serial.println(": Timeout");
    //isNfcBtnPressed = false;      // break NFC communication
  }
  
  //delay(250);
  
}


uint8_t setCharNdef(uint8_t *buffer, const uint8_t *character, uint8_t length) {
  buffer[0] = 0xD4;
  buffer[1] = sizeof(header);
  buffer[2] = length;
  
  header[buffer[1] - 1] = 0x43;
  
  memcpy(buffer + 3, header, buffer[1]);
  memcpy(buffer + 3 + buffer[1], character, buffer[2]);
  
  return 3 + buffer[1] + buffer[2];
}

void print_ndef_payload(const uint8_t *buffer, uint8_t length) {
  NdefMessage message = NdefMessage(buffer, length);
  NdefRecord record = message.getRecord(0);
  int len = record.getPayloadLength();
  for (int szPos=0; szPos < len; szPos++) {
    if (record._payload[szPos] <= 0x1F)
      Serial.print(".");
    else
      Serial.print((char)record._payload[szPos]);
  }
  Serial.println("");
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

