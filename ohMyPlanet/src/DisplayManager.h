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

class DisplayManager {
  public:
  void setup(const int latchPin, const int clockPin, const int dataPin);
  void clear();
  void showNumber(int num);


  private:
  int _latchPin;
  int _clockPin;
  int _dataPin;

  void showDecimal(int dec, int pos);
};

