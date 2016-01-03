
class ModeManager {
  public:
  void setup(const int btnPinMode, const int ledPinModeR, const int ledPinModeG);
  void run();
  bool isFriendlyMode();

  private:
  int _btnPinMode;
  int _ledPinModeR;
  int _ledPinModeG;
  bool _isFriendlyMode = true;    // TODO init from static memory

  int _reading;           // the current reading from the input pin
  int _previous = LOW;    // the previous reading from the input pin
  long _time = 0;         // the last time the output pin was toggled
  long _debounce = 200;   // the debounce time, increase if the output flickers

  void turnFriendlyMode();
  void turnUnfriendlyMode();
};

