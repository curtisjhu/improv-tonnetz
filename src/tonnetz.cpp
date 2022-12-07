#include "tonnetz.h"

Tonnetz::Tonnetz(uint32_t seed) {
	rand.seed(seed);
	perlin.setSeed(seed);
	perlin.setOctaves(4);
	chord.one = ivec2(0, 0);
	chord.five = ivec2(1, 0);
	chord.three = ivec2(0, 1);
}

tuple<float, float, float> Tonnetz::classicalNoteWalk() {
	switch (rand.randUint(4)) {
		case 0: pos += i; break;
		case 1: pos -= i; break;
		case 2: pos += j; break;
		case 3: pos -= j; break;
	}
	cleanPosition(pos);

	float interval = 1;
	return make_tuple(interval, getFreq(pos), 0.3);
}

tuple<float, Triad, float> Tonnetz::classicalChordWalk() {
	auto pivot = [this](ivec2 &pivot, ivec2 &third, ivec2 &fifth){
		switch(rand.randUint(6)) {
			case 0: third = pivot + i; fifth = third - j; break;
			case 1: third = pivot - i; fifth = third + j; break;
			case 2: third = pivot + j; fifth = pivot + i; break;
			case 3: third = pivot - j; fifth = pivot - i; break;
			case 4: third = pivot - i + j; fifth = pivot + j; break;
			case 5: third = pivot + i - j; fifth = pivot - j; break;
		}
	};

	switch (rand.randUint(3)) {
		case 0:
			pivot(chord.one, chord.three, chord.five);
			break;
		case 1:
			pivot(chord.three, chord.one, chord.five);
			break;
		case 2:
			pivot(chord.five, chord.one, chord.three);
			break;
	}
	cleanPosition(chord.one);
	cleanPosition(chord.three);
	cleanPosition(chord.five);

	float interval = 2;
	float gain = 0.3;
	chord.oneFreq = getFreq(chord.one);
	chord.threeFreq = getFreq(chord.three);
	chord.fiveFreq = getFreq(chord.five);

	return make_tuple(interval, chord, gain);
}

tuple<float, float, float> Tonnetz::perlinNoteWalk() {
	int x = static_cast<int>(round(perlin.noise(step) * 10));
	int y =  static_cast<int>(round(perlin.noise(step + 100) * 10));
	pos += i * x;
	pos += j * y;
	cleanPosition(pos);

	float gain = abs(perlin.noise(step + 1000) * (0.8 - 0.2) + 0.2);

	step += 0.1;
	float duration = abs(perlin.noise(step)) * 4;
	duration = 0.5 + round(duration * 2) / 2;

	return make_tuple(duration, getFreq(pos), gain);
}

uint8_t Tonnetz::getMidi(ivec2 &p) {
	cleanPosition(p);
	int midi = (p.x * 7) % 12 + (p.y * 3);
	midi = 60 + (midi % 12);
	return midi;
}

void Tonnetz::cleanPosition(ivec2 &p) {
	p.y = (p.y < 0) ? 3 + (p.y % 3) : p.y % 3;
	p.x = (p.x < 0) ? 12 + (p.x % 12) : p.x % 12;
}