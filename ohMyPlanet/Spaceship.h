#pragma once

#include "EEPROMMemory.h"

struct Spaceship_t {
    int id;
    int resources;
    bool isFriendlyMode;
    char nickname[16];
    //int nicknameLength;
};


class Spaceship {
	public:
		Spaceship();
		int id();
		int resources();
    bool isFriendlyMode();
    String nickname();

    void setFriendlyMode(bool isFriendlyMode);

	private:
	  void initSpaceship();

	Spaceship_t mSpaceship;
};





