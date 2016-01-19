#pragma once

#include "EEPROMMemory.h"

struct Spaceship_t {
    unsigned char id;
    int resources;
    bool isFriendlyMode;

    // for prototype we can have maximum 2 friends
    unsigned char slot1_id;
    unsigned char slot1_frPoints;     // 0..255 pts for prototype
    unsigned char slot2_id;
    unsigned char slot2_frPoints;     // 0..255 pts for prototype
};


class Spaceship {
	public:
		Spaceship();
		unsigned char id();
		int resources();
    bool isFriendlyMode();

    void setFriendlyMode(bool isFriendlyMode);
    void addResources(int delta);
    void setResources(int newResources);

	private:
	  void initSpaceship();

	Spaceship_t mSpaceship;
};





