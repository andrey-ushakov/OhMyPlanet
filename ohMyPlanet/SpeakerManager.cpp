#include "SpeakerManager.h"
#include <Arduino.h>

void SpeakerManager::setup(const int speakerPin) {
	_speakerPin = speakerPin;
  _curMelody = -1;
	_curNote = 0;
	_isPause = false;


	Melody mel_win;
	mel_win.melodyLength = 8;
  
  mel_win.notes[0] = NOTE_C4;
  mel_win.notes[1] = NOTE_G3;
  mel_win.notes[2] = NOTE_G3;
  mel_win.notes[3] = NOTE_A3;
  mel_win.notes[4] = NOTE_G3;
  mel_win.notes[5] = 0;
  mel_win.notes[6] = NOTE_B3;
  mel_win.notes[7] = NOTE_C4;

  mel_win.noteDurations[0] = 4;
  mel_win.noteDurations[1] = 8;
  mel_win.noteDurations[2] = 8;
  mel_win.noteDurations[3] = 4;
  mel_win.noteDurations[4] = 4;
  mel_win.noteDurations[5] = 4;
  mel_win.noteDurations[6] = 4;
  mel_win.noteDurations[7] = 4;

  // add melodies to library
  melodyLibrary[MELODY_WIN] = mel_win;

}

void SpeakerManager::playMelody() {
  //Serial.println(_curMelody);
  if(_curMelody >= 0) {
    Melody melody = melodyLibrary[_curMelody];

    int noteDuration = 1000 / melody.noteDurations[_curNote];
    //Serial.print("note dur : ");Serial.println(melody.noteDurations[0]);
    
    if(!_isPause) {
      tone(_speakerPin, melody.notes[_curNote], noteDuration);
      _timer = millis();
      _isPause = true;
      //Serial.print("pause start. Note : ");Serial.println(_curNote);
    } else {
      int pauseBetweenNotes = noteDuration * 1.30;
      //Serial.print("Pause: "); Serial.println(pauseBetweenNotes);
      if(millis() - _timer >= pauseBetweenNotes) {
        _isPause = false;
        _curNote++;
        noTone(_speakerPin);
      }
    }
    
    
    if(_curNote >= melody.melodyLength) {
      _curMelody = -1;
      noTone(_speakerPin);
    }
  }
}


void SpeakerManager::setMelody(int melodyInd) {
  _curMelody = melodyInd;
  _curNote = 0;
  _isPause = false;
}

