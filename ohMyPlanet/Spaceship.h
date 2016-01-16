#pragma once

#include "EEPROMMemory.h"

struct Spaceship_t {
    int id;
    int resources;
};


class Spaceship {
	public:
		Spaceship();
		int id();
		int resources();

	private:
	void initSpaceship();

	Spaceship_t mSpaceship;
};





