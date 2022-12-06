#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/audio/Context.h"
#include "cinder/audio/GenNode.h"
#include "cinder/audio/GainNode.h"

#include "cinder/Text.h"
#include "cinder/Log.h"
#include "cinder/Utilities.h"

#include "tonnetz.h"
#include <string>

using namespace ci;
using namespace ci::app;
using namespace std;

class Improv : public App {
  public:
	void setup() override;
	void draw() override;
	void update() override;
	void render(string text);

	void updateNote();
	void updateChord();

	audio::GenNodeRef	mGen;
	audio::GainNodeRef	mGain;

	Timer time;
	float lastNoteTime = 0;
	float noteDuration = 1;
	float lastChordTime = 0;
	float chordDuration = 1;
	TextBox mText;
	gl::TextureRef mTextTexture;
	Tonnetz tonnetz{123};
};
