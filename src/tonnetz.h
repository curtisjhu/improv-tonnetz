#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include "cinder/audio/Utilities.h"
#include "cinder/Timer.h"
#include <math.h>
using namespace glm;
using namespace ci;

struct Triad {
	uint8_t midi1, midi2, midi3;
};

class Tonnetz {
	private:
		const uvec2 i = uvec2(1, 0);
		const uvec2 j = uvec2(0, 1);
		Perlin perlin;
		Rand rand;
		Timer time;
		
	public:
		uvec2 pos = uvec2(0, 0);
		Tonnetz(uint32_t seed = 1);
		void classicalNoteWalk();
		void perlinNoteWalk();

		void gaussianNoteWalk(Triad* triad);
		void perlinGaussianNoteWalk(Triad* triad);

		void classicalChordWalk();
		void perlinChordWalk();
		float getFreq();
};