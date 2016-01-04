
const int latchPin = 8;
const int clockPin = 12;
const int dataPin = 11;

const int DIG_1 = 0b00000010;
const int DIG_2 = 0b00000100;
const int DIG_3 = 0b00001000;
const int DIG_4 = 0b00010000;

const int ZERO  = 0b11000000;
const int ONE   = 0b11111001;
const int TWO   = 0b10100100;
const int THREE = 0b10110000;
const int FOUR  = 0b10011001;
const int FIVE  = 0b10010010;
const int SIX   = 0b10000010;
const int SEVEN = 0b11111000;
const int EIGHT = 0b10000000;
const int NINE  = 0b10010000;

const int ARR_POS[4]  = {DIG_1, DIG_2, DIG_3, DIG_4};
const int ARR_NUM[10] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};



void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

}

void loop() {
  showNumber(1009, true);
}

void showNumber(int num, bool isClear) {
  if(isClear) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST , 0b00000000);
    shiftOut(dataPin, clockPin, MSBFIRST , 0b00000000);
    digitalWrite(latchPin, HIGH);
  }
  
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

void showDecimal(int dec, int pos) {
  if(dec < 0 || dec > 9) return;
  if(pos < 0 || pos > 4) return;

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST , ARR_POS[pos]);
  shiftOut(dataPin, clockPin, MSBFIRST , ARR_NUM[dec]);
  digitalWrite(latchPin, HIGH);
}

