#pragma once
#include "sinewave.h"
#include "sinevoice.h"
#include "types.h"
#include "tonnetz.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_events/juce_events.h>

class SynthAudioSource   : public juce::AudioSource
{
public:
    SynthAudioSource (juce::MidiKeyboardState& keyState)
        : keyboardState (keyState), tonnetz(1234, 1)
    {
        for (auto i = 0; i < 4; ++i)
            synth.addVoice (new SineWaveVoice());
 
        synth.addSound (new SineWaveSound());
    }
 
    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }
 
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        synthSampleRate = sampleRate;
    }
 
    void releaseResources() override {}
 
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();

        juce::MidiBuffer incomingBuffer;
        keyboardState.processNextMidiBuffer(incomingBuffer, bufferToFill.startSample,
                                                 bufferToFill.numSamples, true);

        tonnetz.classicalNoteWalk();
        tonnetz.processNextMidiBuffer(incomingBuffer, bufferToFill.startSample, bufferToFill.numSamples);
 
        synth.renderNextBlock (*bufferToFill.buffer, incomingBuffer,
                               bufferToFill.startSample, bufferToFill.numSamples);
    }

 
private:

    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    Tonnetz tonnetz;

    double synthSampleRate;
};