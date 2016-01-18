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

bool Spaceship::isFriendlyMode() {
	return mSpaceship.isFriendlyMode;
}

String Spaceship::nickname() {
  return mSpaceship.nickname;
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
  strcpy (mSpaceship.nickname,"noname");
  //mSpaceship.nicknameLength  = strlen(mSpaceship.nickname);

	// write spaceship structure
	EEPROM_write(1, mSpaceship);
	// write flag 'ok'
	EEPROM_write(0, true);
}

