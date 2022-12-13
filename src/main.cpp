#include "main.h"

void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{ synthAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate); }

void MainContentComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{ synthAudioSource.getNextAudioBlock(bufferToFill); }

void MainContentComponent::releaseResources()
{ synthAudioSource.releaseResources(); }

void MainContentComponent::timerCallback()
{
	// keyboardComponent.grabKeyboardFocus();
	Note n = tonnetz.classicalNoteWalk();
	synthAudioSource.playMidi(n, 1);
	// stopTimer();
}

void MainContentComponent::paint(juce::Graphics& g)
{ }

void MainContentComponent::resized()
{
	keyboardComponent.setBounds (8, 96, getWidth() - 16, 64);
}
