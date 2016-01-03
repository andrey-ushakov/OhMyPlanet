
class ModeManager {
  public:
  void setup(const int btnPinMode, const int ledPinModeR, const int ledPinModeG);
  void run();

  private:
  int _btnPinMode;
  int _ledPinModeR;
  int _ledPinModeG;
  bool isFriendlyMode = true;    // TODO init from static memory

  int reading;           // the current reading from the input pin
  int previous = LOW;    // the previous reading from the input pin
  long time = 0;         // the last time the output pin was toggled
  long debounce = 200;   // the debounce time, increase if the output flickers

  void turnFriendlyMode();
  void turnUnfriendlyMode();
};

