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


int timeoutCnt = 0;

void nfcReaderLoop() {
  int16_t result = nfc.poll(2000);
  uint8_t ndefBuf[128];

  // ::: Client Peer :::
  if (result == 1 ) {
    NdefMessage message = NdefMessage();
    message.addTextRecord( String(resources) );
    
    int messageSize = message.getEncodedSize();
    message.encode(ndefBuf);
    int res = nfc.put(ndefBuf, messageSize); // send character data
    if(res > 0) {
      timeoutCnt = 0;
    }
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
    Serial.println("disconnected");
    timeoutCnt++;

    if(timeoutCnt == 3) {    // max failures time before disconnect
      resources = -1;
      timeoutCnt = 0;
    }
  }
}



// Receive new resource number from Unreal (from Serial port)
void serialEvent() {
  int newResources = 0;
  if (Serial.available()) {
    char serialdata[10];
    int numBytes = Serial.readBytesUntil('\n', serialdata, sizeof(serialdata));
    serialdata[numBytes] = '\0';
    resources = String(serialdata).toInt();
  }
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
