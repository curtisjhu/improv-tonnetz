#pragma once
#include <random>
#include "glm/glm.hpp"
using namespace glm;

class Tonnetz {
private:
	const vec2 i = vec2(1, 0);
	const vec2 j = vec2(0, 1);
	vec2 pos = vec2(0, 0);
	
public:
	Tonnetz() {
		std::srand(1);
	}

	void walk() {
		int rnum = std::rand();
		switch (rnum % 4) {
			case 0:
				pos += i;
			case 1:
				pos -= i;
			case 2:
				pos += j;
			case 3:
				pos -= j;
			default:
		}
	}

	/** Retrieves the chord based on the current melody */
	void chord() {

	}

	

};