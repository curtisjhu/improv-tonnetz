#pragma once
#include <juce_core/juce_core.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "synth.h"
#include "tonnetz.h"
#include "types.h"

using namespace std;

class MainContentComponent
	:	public juce::AudioAppComponent, juce::Timer
{
public:
	MainContentComponent ()
		:	keyboardComponent (keyboardState, juce::KeyboardComponentBase::horizontalKeyboard),
			synthAudioSource (keyboardState)
	{
		addAndMakeVisible (keyboardComponent);
        setAudioChannels (0, 2);
		
        setSize (600, 160);
		startTimer(400);
	};

	~MainContentComponent() override
	{ shutdownAudio(); }

	void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	void paint (juce::Graphics& g) override;
	void resized() override;

	void timerCallback() override;


private:
	juce::MidiKeyboardState keyboardState;
	juce::MidiKeyboardComponent keyboardComponent;
	SynthAudioSource synthAudioSource;
	Tonnetz tonnetz;
	juce::TextButton playbackButton;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
