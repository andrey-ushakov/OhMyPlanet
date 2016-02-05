#include <Thread.h>

#include "SPI.h"
#include "PN532_SPI.h"
#include "snep.h"
#include "NdefMessage.h"

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
int resources = -1;
NdefMessage receivedNdef;

Thread nfcReaderThread = Thread();



void nfcReaderThreadCallback() {
  nfcReaderLoop();
}

void setup() {
  Serial.begin(115200);

  nfcReaderThread.onRun(nfcReaderThreadCallback);
  nfcReaderThread.setInterval(500);
}



void loop() {
  if(nfcReaderThread.shouldRun())
      nfcReaderThread.run();
}



void nfcReaderLoop() {
  int16_t result = nfc.poll(2000);
  uint8_t ndefBuf[128];

  // ::: Client Peer :::
  if (result == 1 ) {
    NdefMessage message = NdefMessage();
    message.addTextRecord( String(resources) );
    
    int messageSize = message.getEncodedSize();
    message.encode(ndefBuf);
    nfc.put(ndefBuf, messageSize); // send character data
  }

  // ::: Server Peer :::
  else if (result == 2 ) {
    int msgSize = nfc.serve(ndefBuf, sizeof(ndefBuf));
    if (msgSize > 0) {
        receivedNdef  = NdefMessage(ndefBuf, msgSize);
        //receivedNdef.print();
        
        String data = payloadToString( receivedNdef.getRecord(0)._payload, receivedNdef.getRecord(0).getPayloadLength() );
        Serial.println(data);
    }
    delay(250);
  }

  // ::: Timeout :::
  else{
  }
}



// Receive new resource number from Unreal (from Serial port)
void serialEvent() {
  int newResources = 0;
  while (Serial.available()) {
    // get the new byte:
    int incomingByte = (int)Serial.read() - 48;
    newResources = newResources * 10 + incomingByte;
  }
  resources = newResources;
}
/*void serialEvent() {
  int newResources = 0;
  if (Serial.available()) {
    char serialdata[80];
    Serial.readBytesUntil('#', serialdata, 80);
    Serial.println(serialdata);
    // get the new byte:
    //int incomingByte = (int)Serial.read() - 48;
    //newResources = newResources * 10 + incomingByte;
  }
  resources = 11;//newResources;
}*/



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
