#include "Spaceship.h"
#include <Arduino.h>

Spaceship::Spaceship() {
	// First power on
	bool flag = EEPROM.read(0);
	if(!flag) {
    	initSpaceship();
	}
	EEPROM_read(1, mSpaceship);
}

int Spaceship::id() {
	return mSpaceship.id;
}

int Spaceship::resources() {
	return mSpaceship.resources;
}

void Spaceship::initSpaceship() {
	// init spaceship structure
	randomSeed(analogRead(0));
	mSpaceship.id = random(1,256);
	mSpaceship.resources = 0;

	// write spaceship structure
	EEPROM_write(1, mSpaceship);
	// write flag 'ok'
	EEPROM_write(0, true);
}