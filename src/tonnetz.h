#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include "cinder/audio/Utilities.h"
#include "cinder/Timer.h"
#include <math.h>
#include "cinder/Log.h"
using namespace glm;
using namespace ci;

struct Triad {
	uint8_t midi1, midi2, midi3;
};

class Tonnetz {
	private:
		const ivec2 i = ivec2(1, 0);
		const ivec2 j = ivec2(0, 1);
		Perlin perlin;
		Rand rand;
		Timer time;
		int steps = 0;

		void cleanPosition();
		
	public:
		ivec2 pos = ivec2(0, 0);
		Tonnetz(uint32_t seed = 1);
		float duration();

		float classicalNoteWalk();
		float perlinNoteWalk();

		void gaussianNoteWalk(Triad* triad);
		void perlinGaussianNoteWalk(Triad* triad);

		void classicalChordWalk();
		void perlinChordWalk();
		float getFreq();
};