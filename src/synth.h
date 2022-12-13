#pragma once
#include "sinewave.h"
#include "sinevoice.h"
#include "types.h"
#include <juce_audio_basics/juce_audio_basics.h>

class SynthAudioSource   : public juce::AudioSource, private juce::Timer, private juce::Logger
{
public:
    SynthAudioSource (juce::MidiKeyboardState& keyState)
        : keyboardState (keyState)
    {
        startTime = juce::Time::getMillisecondCounterHiRes();
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
    }
 
    void releaseResources() override {}
 
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
 
        // juce::MidiBuffer keyboardMidi;
        // keyboardState.processNextMidiBuffer(keyboardMidi, bufferToFill.startSample,
        //                                      bufferToFill.numSamples, true);
 
        synth.renderNextBlock (*bufferToFill.buffer, incomingMidiBuffer,
                               bufferToFill.startSample, bufferToFill.numSamples);
    }

    void addMessageToBuffer (const juce::MidiMessage& message)
    {
        auto timestamp = message.getTimeStamp();
        auto sampleNumber = (int) (timestamp * sampleRate);
        incomingMidiBuffer.addEvent (message, sampleNumber);
    }
    
    void playMidi (Note note, int channel)
    {
        auto message = juce::MidiMessage::noteOn(channel, note.note, note.velocity);
        message.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
        addMessageToBuffer(message);

        auto messageOff = juce::MidiMessage::noteOff (message.getChannel(), message.getNoteNumber());
        messageOff.setTimeStamp (message.getTimeStamp() + 2);
        addMessageToBuffer (messageOff);
    }

    void timerCallback() override
    {
        auto currentTime = juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime;
        auto currentSampleNumber = (int) (currentTime * sampleRate);
 
        for (const auto metadata : incomingMidiBuffer)
        {
            if (metadata.samplePosition > currentSampleNumber)
                break;
 
            auto message = metadata.getMessage();
            message.setTimeStamp (metadata.samplePosition / sampleRate);
            addMessageToList(message);
        }
 
        incomingMidiBuffer.clear (previousSampleNumber, currentSampleNumber - previousSampleNumber);
        previousSampleNumber = currentSampleNumber;   
    }
 
private:

    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;

    // sampling
    double startTime;
    double sampleRate = 44100.0;
    juce::MidiBuffer incomingMidiBuffer;
    int previousSampleNumber = 0;


    void logMessage(const String& message) override
    {
        outputDebugString(message);
    }
    
    void addMessageToList (const juce::MidiMessage& message)
    {
        auto time = message.getTimeStamp();
 
        auto hours   = ((int) (time / 3600.0)) % 24;
        auto minutes = ((int) (time / 60.0)) % 60;
        auto seconds = ((int) time) % 60;
        auto millis  = ((int) (time * 1000.0)) % 1000;
 
        auto timecode = juce::String::formatted ("%02d:%02d:%02d.%03d",
                                                 hours,
                                                 minutes,
                                                 seconds,
                                                 millis);
 
        logMessage (timecode + "  -  " + getMidiMessageDescription (message));
    }
    static juce::String getMidiMessageDescription (const juce::MidiMessage& m)
    {
        if (m.isNoteOn())           return "Note on "          + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff())          return "Note off "         + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())    return "Program change "   + juce::String (m.getProgramChangeNumber());
        if (m.isPitchWheel())       return "Pitch wheel "      + juce::String (m.getPitchWheelValue());
        if (m.isAftertouch())       return "After touch "      + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + juce::String (m.getAfterTouchValue());
        if (m.isChannelPressure())  return "Channel pressure " + juce::String (m.getChannelPressureValue());
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";
 
        if (m.isController())
        {
            juce::String name (juce::MidiMessage::getControllerName (m.getControllerNumber()));
 
            if (name.isEmpty())
                name = "[" + juce::String (m.getControllerNumber()) + "]";
 
            return "Controller " + name + ": " + juce::String (m.getControllerValue());
        }
 
        return juce::String::toHexString (m.getRawData(), m.getRawDataSize());
    }
};