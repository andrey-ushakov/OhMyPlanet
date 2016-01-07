#ifndef SpeakerManager_h
#define SpeakerManager_h
#include "pitches.h"

#define MELODY_WIN 0

struct Melody {
  int melodyLength = 0;
  int notes[10] = {};
  int noteDurations[10] = {};
};


class SpeakerManager {
	public:
		void setup(const int speakerPin);
    void playMelody();
    void setMelody(int melodyInd);

	private:
		int _speakerPin;

		unsigned long _timer;
    int _curMelody;
		int _curNote;
		bool _isPause;

    Melody melodyLibrary[1];
};



#endif
