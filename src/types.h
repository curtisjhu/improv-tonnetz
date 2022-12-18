#pragma once
#include <math.h>
using namespace std;

struct Note {
	float duration;
	int note;
	float velocity;
};

struct Chord {
	Note one, three, five;
};