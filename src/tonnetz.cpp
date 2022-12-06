#include "tonnetz.h"

Tonnetz::Tonnetz(uint32_t seed) {
	rand.seed(seed);
	perlin.setSeed(seed);
	perlin.setOctaves(4);
	time.start(1);
}

float Tonnetz::classicalNoteWalk() {
	switch (rand.randUint(4)) {
		case 0:
			pos += i;
			break;
		case 1:
			pos -= i;
			break;
		case 2:
			pos += j;
			break;
		case 3:
			pos -= j;
			break;
	}
	cleanPosition();
	return 1;
}

float Tonnetz::classicalChordWalk() {

}

float Tonnetz::perlinNoteWalk() {
	double t = time.getSeconds();
	int x = static_cast<int>(round(perlin.noise(steps) * 10));
	int y =  static_cast<int>(round(perlin.noise(steps + 1000) * 10));
	pos += i * x + j * y;
	cleanPosition();
	steps++;
	return perlin.noise(steps) * 20;
}

float Tonnetz::perlinChordWalk() {

}


float Tonnetz::getFreq() {
	cleanPosition();
	int midi = (pos.x * 7) % 12 + (pos.y * 3);
	midi = 60 + (midi % 12);

	return audio::midiToFreq((float) midi);
}

void Tonnetz::cleanPosition() {
	pos.y = (pos.y < 0) ? 3 + (pos.y % 3) : pos.y % 3;
	pos.x = (pos.x < 0) ? 12 + (pos.x % 12) : pos.x % 12;
}