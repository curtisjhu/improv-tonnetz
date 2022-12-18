#include "tonnetz.h"

Tonnetz::Tonnetz(int channel)
{ Tonnetz(1964, channel); }

Tonnetz::Tonnetz(uint32_t seed, int channel)
		: startTime(juce::Time::getMillisecondCounterHiRes() * 0.001)
{
	channel = channel;
	lastChordTime = startTime, lastNoteTime = startTime;
	noteDuration = 2, chordDuration = 2;

	random.setSeed(seed);
	perlin.reseed(seed);

	note = ivec2(0, 0);
	chord = { ivec2(0, 0), ivec2(1, 0), ivec2(0, 1) };
}

void Tonnetz::addMessageToBuffer (Note note)
{
	auto message = juce::MidiMessage::noteOn(channel, note.note, note.velocity);
	auto timestamp = juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime;
	message.setTimeStamp(timestamp);
	eventsToAdd.addEvent(message, timestamp);

	auto messageOff = juce::MidiMessage::noteOff (message.getChannel(), message.getNoteNumber());
	auto timestampOff = message.getTimeStamp() + note.duration;
	messageOff.setTimeStamp (timestampOff);
	eventsToAdd.addEvent(messageOff, timestampOff);
}

void Tonnetz::processNextMidiBuffer(juce::MidiBuffer& buffer, const int startSample, const int numSamples)
{
	juce::MidiBuffer overflow;
	for (const auto event : eventsToAdd) {
		const auto message = event.getMessage();
		const auto deltaTime = juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime - message.getTimeStamp();
		if (deltaTime > 0) {
			// sketchy: I'm assuming eventsToAdd.getLastEventTime() is overall size of buffer
			const auto percent = event.samplePosition / eventsToAdd.getLastEventTime();
			buffer.addEvent(message, startSample + numSamples * percent);
		} else {
			overflow.addEvent(message, message.getTimeStamp());
		}
	}
	eventsToAdd.clear();
	eventsToAdd.swapWith(overflow);
}

Note Tonnetz::classicalNoteWalk()
{
	double currentTime = juce::Time::getMillisecondCounter() * 0.001;
	if (currentTime - lastNoteTime > noteDuration) {
		lastNoteTime = currentTime;

		switch (random.nextInt(4)) {
			case 0: note += i; break;
			case 1: note -= i; break;
			case 2: note += j; break;
			case 3: note -= j; break;
		}
		cleanPosition(note);

		Note n = { duration: 2, note: getMidi(note), velocity: 0.3 };
		addMessageToBuffer(n);

		noteDuration = n.duration;
	}
}

Chord Tonnetz::classicalChordWalk()
{
	auto pivot = [this](ivec2 &pivot, ivec2 &third, ivec2 &fifth){
		switch(random.nextInt(6)) {
			case 0: third = pivot + i; fifth = third - j; break;
			case 1: third = pivot - i; fifth = third + j; break;
			case 2: third = pivot + j; fifth = pivot + i; break;
			case 3: third = pivot - j; fifth = pivot - i; break;
			case 4: third = pivot - i + j; fifth = pivot + j; break;
			case 5: third = pivot + i - j; fifth = pivot - j; break;
		}
	};

	switch (random.nextInt(3)) {
		case 0:
			pivot(chord.one, chord.three, chord.five);
			break;
		case 1:
			pivot(chord.three, chord.one, chord.five);
			break;
		case 2:
			pivot(chord.five, chord.one, chord.three);
			break;
	}
	cleanPosition(chord.one);
	cleanPosition(chord.three);
	cleanPosition(chord.five);

	Triad c;
	c.oneMidi = getMidi(chord.one);
	c.fiveMidi = getMidi(chord.five);
	c.threeMidi = getMidi(chord.three);

	Chord ch;
	ch.chord = c;
	ch.duration = 2;
	ch.velocity = 0.3;
	return ch;
}

Note Tonnetz::perlinNoteWalk()
{
	int x = static_cast<int>(round(perlin.octave1D_11(step, octaves) * 10));
	int y =  static_cast<int>(round(perlin.octave1D_11(step + 100, octaves) * 10));
	note += i * x;
	note += j * y;
	cleanPosition(note);

	float gain = abs(perlin.octave1D_01(step + 1000, octaves) * (0.8 - 0.2) + 0.2);

	step += 0.1;
	float duration = abs(perlin.octave1D_01(step, octaves)) * 4;
	duration = 0.5 + round(duration * 2) / 2;

	Note n;
	n.duration = duration;
	n.note = getMidi(note);
	n.velocity = gain;
	return n;
}

uint8_t Tonnetz::getMidi(ivec2 &p)
{
	int baseNote = 48;
	cleanPosition(p);
	int midi = (p.x * 7) % 12 + (p.y * 3);
	midi = baseNote + (midi % 12);
	return midi;
}

void Tonnetz::cleanPosition(ivec2 &p)
{
	p.y = (p.y < 0) ? 3 + (p.y % 3) : p.y % 3;
	p.x = (p.x < 0) ? 12 + (p.x % 12) : p.x % 12;
}