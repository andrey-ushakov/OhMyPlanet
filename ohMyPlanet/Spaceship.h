#pragma once

#include "EEPROMMemory.h"

struct Spaceship_t {
    unsigned char id;
    int resources;
    bool isFriendlyMode;
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





