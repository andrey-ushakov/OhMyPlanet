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

unsigned char Spaceship::id() {
	return mSpaceship.id;
}

int Spaceship::resources() {
	return mSpaceship.resources;
}

bool Spaceship::isFriendlyMode() {
	return mSpaceship.isFriendlyMode;
}

void Spaceship::setFriendlyMode (bool isFriendlyMode) {
	mSpaceship.isFriendlyMode = isFriendlyMode;
	// write spaceship structure
	EEPROM_write(1, mSpaceship);
}

void Spaceship::initSpaceship() {
	// init spaceship structure
	mSpaceship.id 				    = random(1,256);
	mSpaceship.resources      = 0;
  mSpaceship.isFriendlyMode = true;

	// write spaceship structure
	EEPROM_write(1, mSpaceship);
	// write flag 'ok'
	EEPROM_write(0, true);
}

void Spaceship::addResources(int delta) {
  mSpaceship.resources += delta;
  // write spaceship structure
  EEPROM_write(1, mSpaceship);
}

