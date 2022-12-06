#include "tonnetz.h"

Tonnetz::Tonnetz(uint32_t seed) {
	rand.seed(seed);
	perlin.setSeed(seed);
	perlin.setOctaves(4);
	time.start(1);
}

float Tonnetz::classicalNoteWalk() {
	switch (rand.randUint(4)) {
		case 0: pos += i;
		case 1: pos -= i;
		case 2: pos += j;
		case 3: pos -= j;
	}
	cleanPosition();
	return 1;
}

float Tonnetz::perlinNoteWalk() {
	double t = time.getSeconds();
	int x = static_cast<int>(round(perlin.noise(t) * 10));
	int y =  static_cast<int>(round(perlin.noise(t) * 10));
	pos += i * x + j * y;
	cleanPosition();

	steps++;
	return perlin.noise(steps) * 10;
}

void Tonnetz::classicalChordWalk() {
	
}

float Tonnetz::getFreq() {
	cleanPosition();
	int midi = (pos.x * 7) % 12 + (pos.y * 3);
	midi = 48 + (midi % 12);

	return audio::midiToFreq((float) midi);
}

void Tonnetz::cleanPosition() {
	pos.y %= 3;
	pos.x %= 12;
}