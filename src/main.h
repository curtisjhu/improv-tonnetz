#pragma once
#include <JuceHeader.h>

using namespace std;

class ImprovTonnetz : public juce::AudioAppComponent 
{
public:
	ImprovTonnetz();
	~ImprovTonnetz() override;
	void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	void paint (juce::Graphics& g) override;
	void resized() override;
};
