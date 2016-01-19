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

void Spaceship::setResources(int newResources) {
  mSpaceship.resources = newResources;
}

void Spaceship::addFriendshipPoints(unsigned char friendId) {
  if(mSpaceship.slot1_id == 0) {
    mSpaceship.slot1_id       = friendId;
    mSpaceship.slot1_frPoints = 10;
  } else if (mSpaceship.slot1_id == friendId) {
    mSpaceship.slot1_frPoints++;
  } else if(mSpaceship.slot2_id == 0) {
    mSpaceship.slot2_id       = friendId;
    mSpaceship.slot2_frPoints = 10;
  } else if (mSpaceship.slot2_id == friendId) {
    mSpaceship.slot2_frPoints++;
  }
  
  // write spaceship structure
  EEPROM_write(1, mSpaceship);
}

void Spaceship::reduceFriendshipPoints(unsigned char friendId) {
  if(mSpaceship.slot1_id == 0) {
    mSpaceship.slot1_id       = friendId;
    mSpaceship.slot1_frPoints = 0;
  } else if (mSpaceship.slot1_id == friendId) {
    if(mSpaceship.slot1_frPoints - 1 >= 0)  mSpaceship.slot1_frPoints--;
  } else if(mSpaceship.slot2_id == 0) {
    mSpaceship.slot2_id       = friendId;
    mSpaceship.slot2_frPoints = 0;
  } else if (mSpaceship.slot2_id == friendId) {
    if(mSpaceship.slot2_frPoints - 1 >= 0)  mSpaceship.slot2_frPoints--;
  }
  
  // write spaceship structure
  EEPROM_write(1, mSpaceship);
}

String Spaceship::friendshipToString() {
  return String( String(mSpaceship.slot1_id) + ':' + String(mSpaceship.slot1_frPoints) + '&' + String(mSpaceship.slot2_id) + ':' + String(mSpaceship.slot2_frPoints) );
}

void Spaceship::initSpaceship() {
	// init spaceship structure
	mSpaceship.id 				    = random(1,256);
	mSpaceship.resources      = 0;
  mSpaceship.isFriendlyMode = true;

  mSpaceship.slot1_id          = 0;
  mSpaceship.slot1_frPoints    = 0;
  mSpaceship.slot2_id          = 0;
  mSpaceship.slot2_frPoints    = 0;

	// write spaceship structure
	EEPROM_write(1, mSpaceship);
	// write flag 'ok'
	EEPROM_write(0, true);
}

void Spaceship::addResources(int delta) {
  if(mSpaceship.resources + delta >= 9999)
    mSpaceship.resources = 9999;
  else if(mSpaceship.resources + delta <= 0)
    mSpaceship.resources = 0;
  else
    mSpaceship.resources += delta;
    
  // write spaceship structure
  EEPROM_write(1, mSpaceship);
}

