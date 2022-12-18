#pragma once
#define GLM_FORCE_MESSAGES
#include <math.h>
#include <utility>
#include <glm/glm.hpp>
#include "types.h"
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_events/juce_events.h>
#include "PerlinNoise.hpp"
using namespace glm;
using namespace std;

class Tonnetz
{
private:
	const ivec2 i = ivec2(1, 0);
	const ivec2 j = ivec2(0, 1);
	const int octaves = 4;
	void cleanPosition(ivec2 &p);
	void cleanChordPosition();
	juce::Random random;
	siv::PerlinNoise perlin;
	void addMessageToBuffer(Note note, int channel);
	void addMessageToBuffer (Chord chord);
	int channel;
	juce::MidiBuffer eventsToAdd;

	double lastNoteTime;
	double lastChordTime;
	float noteDuration;
	float chordDuration;

	double startTime;
	
public:
	ivec2 note;
	struct {
		ivec2 one, three, five;
	} chord;
	Tonnetz(int channel);
	Tonnetz(uint32_t seed, int channel);

	void processNextMidiBuffer(juce::MidiBuffer& buffer, const int startSample, const int numSamples);

	void classicalNoteWalk();
	Note perlinNoteWalk();

	void  classicalChordWalk();
	Chord  perlinChordWalk();
	uint8_t getMidi(ivec2 &p);

};