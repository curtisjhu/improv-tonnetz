#pragma once
#include "juce_core/juce_core.h"
#include "tonnetz.h"
#include <string>

using namespace std;

struct Chord {
	audio::GenNodeRef one, three, five;
	audio::GainNodeRef gain;
};

struct Note {
	audio::GenNodeRef wave;
	audio::GainNodeRef gain;
};

class Improv : public App {
  public:
	void setup() override;
	void draw() override;
	void update() override;
	void render(string text);

	void updateNote();
	void updateChord();

	Note note;
	Chord chord;

	Timer time;
	float lastNoteTime = 0;
	float noteDuration = 0.5;
	float lastChordTime = 0;
	float chordDuration = 1;
	
	TextBox mText;
	gl::TextureRef mTextTexture;
	Tonnetz tonnetz{};
};
