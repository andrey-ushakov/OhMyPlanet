#pragma once
#include "GestureRecognition.h"

#define COMBOS_NUM 4

struct DamageRange {
	byte min = 0;
	byte max = 0;
};

struct Combo {
	int combo[MAX_COMBO_LENGTH];
	DamageRange range;
};




class ComboManager {
  public:
  ComboManager();
  Combo getCombo(byte gestures[MAX_COMBO_LENGTH]);   // return combo by given gesture sequence


  private:
  Combo combos[COMBOS_NUM];
  Combo getBasicCombo(byte gestures[MAX_COMBO_LENGTH]);
};

