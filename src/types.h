#pragma once
#include <math.h>
using namespace std;

struct Triad {
	uint8_t oneMidi, threeMidi, fiveMidi;
};

struct Chord {
	float duration;
	Triad chord;
	float velocity;
};

struct Note {
	float duration;
	uint8_t note;
	float velocity;
};