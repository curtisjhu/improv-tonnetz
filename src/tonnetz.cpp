#include "tonnetz.h"

Tonnetz::Tonnetz(uint32_t seed) {
	rand.seed(seed);
	perlin.setSeed(seed);
	perlin.setOctaves(4);
	time.start();
}

void Tonnetz::classicalNoteWalk() {
	switch (rand.randUint(4)) {
		case 0: pos += i;
		case 1: pos -= i;
		case 2: pos += j;
		case 3: pos -= j;
	}
	pos.y %= 3;
	pos.x %= 12;
}

void Tonnetz::perlinNoteWalk() {
	float t = (float) time.getSeconds();
	int8_t x = (int8_t) round(perlin.noise(t));
	int8_t y = (int8_t) round(perlin.noise(t));
	pos += i * uvec2(x, x) + j * uvec2(y, y);
}

void Tonnetz::classicalChordWalk() {

}

float Tonnetz::getFreq() {
	int midi = ((pos.x % 12) * 7) % 12 + (pos.y % 3) * 3;
	midi = 48 + (midi % 12);

	return audio::midiToFreq((float) midi);
}

