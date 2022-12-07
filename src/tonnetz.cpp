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
	int x = static_cast<int>(round(perlin.noise(step) * 10));
	int y =  static_cast<int>(round(perlin.noise(step + 100) * 10));
	pos += i * x;
	pos += j * y;
	cleanPosition();

	step += 0.1;
	float duration = abs(perlin.noise(step)) * 4;
	CI_LOG_D(duration);
	duration = 0.5 + round(duration * 2) / 2;
	return duration;
}

float Tonnetz::perlinChordWalk() {
	perlin.noise(chordPos);
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