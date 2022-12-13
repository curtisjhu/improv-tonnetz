#pragma once
#include <math.h>
#include <utility>
#include <glm/glm.hpp>
#include "types.h"
#include <juce_core/juce_core.h>
#include "PerlinNoise.hpp"
using namespace glm;
using namespace std;

class Tonnetz
{
private:
	const ivec2 i = ivec2(1, 0);
	const ivec2 j = ivec2(0, 1);
	float step = 1.5f;
	const int octaves = 4;
	void cleanPosition(ivec2 &p);
	juce::Random random;
	siv::PerlinNoise perlin;
	
public:
	ivec2 note;
	struct {
		ivec2 one, three, five;
	} chord;
	Tonnetz();
	Tonnetz(uint32_t seed);

	Note classicalNoteWalk();
	Note perlinNoteWalk();

	Chord  classicalChordWalk();
	Chord  perlinChordWalk();
	uint8_t getMidi(ivec2 &p);
};