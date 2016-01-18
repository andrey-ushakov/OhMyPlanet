#include "ComboManager.h"

ComboManager::ComboManager() {
  Combo c1;
  c1.combo[0] = GESTURE_DOWN;
  c1.combo[1] = GESTURE_DOWN;
  c1.combo[2] = GESTURE_UP;
  c1.combo[3] = GESTURE_UP;
  c1.range.min = 1;
  c1.range.max = 3;

  Combo c2;
  c2.combo[0] = GESTURE_UP;
  c2.combo[1] = GESTURE_DOWN;
  c2.combo[2] = GESTURE_LEFT;
  c2.combo[3] = GESTURE_RIGHT;
  c2.range.min = 2;
  c2.range.max = 5;

  Combo c3;
  c3.combo[0] = GESTURE_LEFT;
  c3.combo[1] = GESTURE_LEFT;
  c3.combo[2] = GESTURE_UP;
  c3.combo[3] = GESTURE_UP;
  c3.range.min = 3;
  c3.range.max = 7;

  Combo c4;
  c4.combo[0] = GESTURE_DOWN;
  c4.combo[1] = GESTURE_RIGHT;
  c4.combo[2] = GESTURE_UP;
  c4.combo[3] = GESTURE_DOWN;
  c4.range.min = 4;
  c4.range.max = 6;


  //
  combos[0] = c1;
  combos[1] = c2;
  combos[2] = c3;
  combos[3] = c4;
}


Combo ComboManager::getCombo(byte gestures[MAX_COMBO_LENGTH]) {
  for(byte i=0; i<COMBOS_NUM; ++i) {
    bool isFound = true;
    for(byte j=0; j<MAX_COMBO_LENGTH; ++j) {
      if(combos[i].combo[j] != gestures[j]) {
        isFound = false;
        break;
      }
    }
    if(isFound) {
      return combos[i];
    }
  }
  
  // combo didn't find in a combo list --> return basic combo
  return getBasicCombo(gestures);
}

Combo ComboManager::getBasicCombo(byte gestures[MAX_COMBO_LENGTH]) {
  Combo c;
  c.range.min = 0;
  c.range.max = 3;
  for(byte i=0; i<MAX_COMBO_LENGTH; ++i) {
    c.combo[i] = gestures[i];
  }
  
  return c;
}

