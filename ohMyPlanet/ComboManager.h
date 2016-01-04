#pragma once
#include "GestureRecognition.h"

#define COMBOS_NUM 4

struct DamageRange {
	int min = 0;
	int max = 0;
};

struct Combo {
  String name = "";
	int combo[MAX_COMBO_LENGTH];
	DamageRange range;
};




class ComboManager {
  public:
  ComboManager();
  Combo getCombo(int gestures[MAX_COMBO_LENGTH]);   // return combo by given gesture sequence


  private:
  Combo combos[COMBOS_NUM];
  Combo getBasicCombo(int gestures[MAX_COMBO_LENGTH]);
};

