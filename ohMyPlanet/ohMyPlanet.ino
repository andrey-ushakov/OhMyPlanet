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

const byte btnPinGyro  = 4;
const byte ledPinGyro  = 3;

const byte btnPinMode  = 7;
const byte ledPinModeR = 6;
const byte ledPinModeG = 5;

const byte latchPin = A0;
const byte clockPin = A1;
const byte dataPin = A2;

const byte btnPinNfc  = 13;

Thread nfcThread = Thread();
Thread nfcReaderThread = Thread();

GestureRecognition *gesture = new GestureRecognition();
ModeManager *modeManager = new ModeManager();
Multiplex7Seg4Digit disp(latchPin, clockPin, dataPin);
Spaceship *spaceship;
ComboManager comboManager = ComboManager();

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);

bool isNfcBtnPressed = false;

void nfcThreadCallback() {
  nfcLoop();
}

void nfcReaderThreadCallback() {
  nfcReaderLoop();
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

  nfcReaderThread.onRun(nfcReaderThreadCallback);
  nfcReaderThread.setInterval(500);
}



void loop() {
  modeManager->run();
  gesture->run();
  disp.displayNum(spaceship->resources());

  if(digitalRead(btnPinNfc) == HIGH && !isNfcBtnPressed) { // btn NFC was pressed
    isNfcBtnPressed = true;
  }
  
  if( isNfcBtnPressed && (gesture->isComboAvailaible() || spaceship->isFriendlyMode()) ) {
    // :: Connect to the figurine
    disp.clear();
    if(nfcThread.shouldRun())
      nfcThread.run();
  } else if( isNfcBtnPressed && !gesture->isComboAvailaible() && !spaceship->isFriendlyMode() ) {
    // :: Connect to the reader
    disp.clear();
    if(nfcReaderThread.shouldRun())
      nfcReaderThread.run();
  }
  else {
    isNfcBtnPressed = false;
  }
}



bool isNfcMessageSent     = false;
bool isNfcMessageReceived = false;
byte myDamage = 0;
NdefMessage receivedNdef;

void nfcLoop() {
  int16_t result = nfc.poll();
  uint8_t ndefBuf[128];

  // ::: Client Peer :::
  if (result == 1 && !isNfcMessageSent) {
    //Serial.println(": Client peer");
    NdefMessage message = buildNdefMessage();
    int messageSize = message.getEncodedSize();
    message.encode(ndefBuf);
    nfc.put(ndefBuf, messageSize); // send character data

    isNfcMessageSent = true;
  }

  // ::: Server Peer :::
  else if (result == 2 && !isNfcMessageReceived) {
    //Serial.println(": Server peer");
    int msgSize = nfc.serve(ndefBuf, sizeof(ndefBuf));
    if (msgSize > 0) {
        receivedNdef  = NdefMessage(ndefBuf, msgSize);
        //receivedNdef.print();
        isNfcMessageReceived = true;
    } else {
    }
    delay(250);
  }

  // ::: Timeout :::
  else{
    //Serial.println(": Timeout");
  }

  // NFC communication done
  if( isNfcMessageSent && isNfcMessageReceived ) {
    boolean otherInFriendlyMode = (boolean)((char)receivedNdef.getRecord(1)._payload[3] - '0');
    NdefRecord recOtherId = receivedNdef.getRecord(0);
    unsigned char otherId = (unsigned char)(payloadToString(recOtherId._payload, recOtherId.getPayloadLength()).toInt());
    
    if( !spaceship->isFriendlyMode() && !otherInFriendlyMode ) { // 2 Unfriendly modes
      //myDamage
      NdefRecord recDmg = receivedNdef.getRecord(2);
      byte otherDamage = payloadToString( recDmg._payload, recDmg.getPayloadLength() ).toInt();

      if(myDamage > otherDamage) {
        spaceship->addResources(myDamage*100);
      } else if(myDamage < otherDamage) {
        spaceship->addResources(-otherDamage*100);
      }
      spaceship->reduceFriendshipPoints(otherId);
    } else if( spaceship->isFriendlyMode() && otherInFriendlyMode ) { // 2 Friendly modes
      spaceship->addFriendshipPoints(otherId);
    }
    isNfcMessageSent = isNfcMessageReceived = false;
    isNfcBtnPressed = false;
    gesture->resetCombo();
  }
  
}


int timeoutCnt = 0;

void nfcReaderLoop() {
  int16_t result = nfc.poll(2000);
  uint8_t ndefBuf[128];

  // ::: Client Peer :::
  if (result == 1 ) {
    //Serial.println(": Client peer");
    NdefMessage message = buildNdefReaderMessage();
    int messageSize = message.getEncodedSize();
    message.encode(ndefBuf);
    int res = nfc.put(ndefBuf, messageSize); // send character data
    if(res > 0) {
      timeoutCnt = 0;
    }
    
  }

  // ::: Server Peer :::
  else if (result == 2 ) {
    //Serial.println(": Server peer");
    int msgSize = nfc.serve(ndefBuf, sizeof(ndefBuf));
    if (msgSize > 0) {
        timeoutCnt = 0;
        
        receivedNdef  = NdefMessage(ndefBuf, msgSize);
        receivedNdef.print();
        // get new resource number
        int newResources = (payloadToString(receivedNdef.getRecord(0)._payload, receivedNdef.getRecord(0).getPayloadLength()).toInt());
        if(newResources >= 0) {
          spaceship->setResources(newResources);
          timeoutCnt = 0;
        }
    }
    
    delay(250);
  }

  // ::: Timeout :::
  else{
    //Serial.println("Timeout");
    // Count failures to disconnect from reader
    timeoutCnt++;

    if(timeoutCnt == 3) {    // max failures time before disconnect
      isNfcBtnPressed = false;
      timeoutCnt = 0;
    }
  }
  
}


NdefMessage buildNdefMessage() {
  NdefMessage message = NdefMessage();
  message.addTextRecord( String(spaceship->id()) );
  message.addTextRecord( String(spaceship->isFriendlyMode()) );
  if( !spaceship->isFriendlyMode() ) {
    Combo usedCombo = comboManager.getCombo(gesture->getCombo());
    myDamage = random(usedCombo.range.min, usedCombo.range.max + 1);
    message.addTextRecord( String(myDamage) );
  }
  return message;
}


NdefMessage buildNdefReaderMessage() {
  NdefMessage message = NdefMessage();
  message.addTextRecord( String(spaceship->id()) + ':' + String(spaceship->resources()) + '&' + spaceship->friendshipToString() );
  return message;
}



// removes first 3 characters ('.en')
String payloadToString(byte array[], byte len) {
  if(len <= 3) {
    return "";
  }
  char resStr[len-3+1];

  for (byte szPos=3; szPos < len; szPos++) {
    if (array[szPos] <= 0x1F)
      resStr[szPos-3] = '.';
    else
      resStr[szPos-3] = (char)array[szPos];
  }
  resStr[len-3] = '\0';
  return String(resStr);
}
