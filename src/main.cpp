#include "main.h"

ImprovTonnetz::ImprovTonnetz()
{
	setSize(800, 600);
	setAudioChannels(0, 2);
}

ImprovTonnetz::~ImprovTonnetz()
{ shutdownAudio(); }


void ImprovTonnetz::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{}

void ImprovTonnetz::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{}

void ImprovTonnetz::releaseResources()
{}

void ImprovTonnetz::paint(juce::Graphics& g)
{}

void ImprovTonnetz::resized()
{}
