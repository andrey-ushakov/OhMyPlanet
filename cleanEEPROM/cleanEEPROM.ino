#include <EEPROM.h>

// Clean Arduino Static Memory

void setup() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void loop() {
}
