#include "main.h"

void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{ synthAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate); }

void MainContentComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{ synthAudioSource.getNextAudioBlock(bufferToFill); }

void MainContentComponent::releaseResources()
{ synthAudioSource.releaseResources(); }

void MainContentComponent::timerCallback()
{
	keyboardComponent.grabKeyboardFocus();
	stopTimer();
}

void MainContentComponent::paint(juce::Graphics& g)
{ }

void MainContentComponent::resized()
{
	keyboardComponent.setBounds (10,  40, getWidth() - 20, getHeight() - 50);	
}
