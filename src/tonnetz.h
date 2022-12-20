#pragma once
#include <math.h>
#include <cmath>
#include <utility>
#include <glm/glm.hpp>
#include "types.h"
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_events/juce_events.h>
#include "PerlinNoise.hpp"
#include <random>
using namespace glm;
using namespace std;

class Tonnetz : private juce::FileLogger
{
private:
	const ivec2 i = ivec2(1, 0);
	const ivec2 j = ivec2(0, 1);
	const int octaves = 4;
	int channel;
	juce::Random random;
	siv::PerlinNoise perlin;
	juce::MidiBuffer eventsToAdd;

	ivec2 cleanPosition(ivec2 p);
	void addMessageToBuffer(Note note, int channel);
	void addMessageToBuffer (Chord chord);
	juce::String getMidiMessageDescription (const juce::MidiMessage& m);
	void addMessageToList (const juce::MidiMessage& message);

	float sigma;
	void moveAccordingToCDF(ivec2& point);
	void randGaussianPosition(ivec2& point);

	uint8_t getNoteMidi();
	uint8_t getMidi(ivec2 &p, int startNote);
	struct triadMidi {
		uint8_t one, three, five;
	};
	triadMidi getChordMidi();

	double lastNoteTime;
	double lastChordTime;
	float noteDuration;
	float chordDuration;
	float step;

	int majorscale[7] = {0, 2, 4, 5, 7, 9, 11};
	int minorscale[7] = {0, 2, 3, 5, 7, 8, 10};
	int currentscale[7];
	int currentIndex{0};
	bool needsNewScale{true};

	double startTime;
	
public:
	ivec2 note;
	struct {
		ivec2 one, three, five;
	} chord;
	Tonnetz(uint32_t seed, int channel);
	~Tonnetz();

	void processNextMidiBuffer(juce::MidiBuffer& buffer, const int startSample, const int numSamples);

	void classicalNoteWalk();
	void perlinNoteWalk();
	void gaussianNoteWalk();
	void tonalCenterNoteWalk();

	void classicalChordWalk();
	void noChordWalk();

};