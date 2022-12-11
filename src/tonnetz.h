#pragma once
#include <JuceHeader.h>
#include <math.h>
#include <utility>
#include <random>
// using namespace glm;
using namespace std;

// struct Triad {
// 	ivec2 one, three, five;
// 	float oneFreq, threeFreq, fiveFreq;
// };

// class Tonnetz {
// 	private:
// 		const ivec2 i = ivec2(1, 0);
// 		const ivec2 j = ivec2(0, 1);
// 		float step = 1.5f;
// 		void cleanPosition(ivec2 &p);
// 		juce::Random random;
// 		siv::PerlinNoise perlin;
		
// 	public:
// 		ivec2 note;
// 		Triad chord;
// 		Tonnetz(uint32_t seed = 1u);

// 		tuple<float, float, float> classicalNoteWalk();
// 		tuple<float, float, float> perlinNoteWalk();

// 		tuple<float, Triad, float>  classicalChordWalk();
// 		tuple<float, Triad, float>  perlinChordWalk();
// 		uint8_t getMidi(ivec2 &p);
// };