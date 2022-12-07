#pragma once
#include "PerlinNoise.hpp"
#include "glm/glm.hpp"
#include <math.h>
#include <utility>
#include <random>
using namespace glm;
using namespace std;

struct Triad {
	ivec2 one, three, five;
	float oneFreq, threeFreq, fiveFreq;
};

class Tonnetz {
	private:
		const ivec2 i = ivec2(1, 0);
		const ivec2 j = ivec2(0, 1);
		float step = 1.5f;
		void cleanPosition(ivec2 &p);
		
	public:
		ivec2 pos = ivec2(0, 0);
		Triad chord;
		Tonnetz(uint32_t seed = 1);

		tuple<float, float, float> classicalNoteWalk();
		tuple<float, float, float> perlinNoteWalk();

		tuple<float, Triad, float>  classicalChordWalk();
		tuple<float, Triad, float>  perlinChordWalk();
		uint8_t getMidi(ivec2 &p);
};