#pragma once

#include "EEPROMMemory.h"

struct Spaceship_t {
    int id;
    int resources;
    bool isFriendlyMode;
};


class Spaceship {
	public:
		Spaceship();
		int id();
		int resources();
		bool isFriendlyMode();

    	void setFriendlyMode(bool isFriendlyMode);

	private:
	void initSpaceship();

	Spaceship_t mSpaceship;
};





