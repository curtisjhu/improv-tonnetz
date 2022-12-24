#include "tonnetz.h"

juce::String getLogPath()
{
	// TODO: make this not specific to machine
	juce::File myFile = juce::File("/Users/curtisjhu/cpp/improv-tonnetz/log.txt");
	if (myFile.exists())
		return myFile.getFullPathName();
	return juce::String("~");
}

Tonnetz::Tonnetz(uint32_t seed, int channel)
	: startTime(juce::Time::getMillisecondCounterHiRes() * 0.001),
	  FileLogger(juce::File(getLogPath()), juce::String("Logging Notes:"), (long long)128 * 1024)
{
	channel = channel;
	lastChordTime = startTime, lastNoteTime = startTime;
	noteDuration = 2, chordDuration = 2;

	random.setSeed(seed);
	perlin.reseed(seed);
	sigma = 1;
	step = 1;

	note = ivec2(0, 0);
	chord = {ivec2(0, 0), ivec2(1, 0), ivec2(0, 1)};
}

Tonnetz::~Tonnetz() {}

void Tonnetz::addMessageToBuffer(Note note, int channel)
{
	auto message = juce::MidiMessage::noteOn(channel, note.note, note.velocity);
	auto timestamp = juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime;
	message.setTimeStamp(timestamp);
	eventsToAdd.addEvent(message, timestamp);

	addMessageToList(message);

	auto messageOff = juce::MidiMessage::noteOff(message.getChannel(), message.getNoteNumber());
	auto timestampOff = message.getTimeStamp() + note.duration;
	messageOff.setTimeStamp(timestampOff);
	eventsToAdd.addEvent(messageOff, timestampOff);
}

void Tonnetz::addMessageToBuffer(Chord c)
{
	addMessageToBuffer(c.one, 3);
	addMessageToBuffer(c.three, 4);
	addMessageToBuffer(c.five, 5);
}

void Tonnetz::processNextMidiBuffer(juce::MidiBuffer &buffer, const int startSample, const int numSamples)
{
	juce::MidiBuffer overflow;
	for (const auto event : eventsToAdd)
	{
		const auto message = event.getMessage();
		const auto deltaTime = juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime - message.getTimeStamp();
		if (deltaTime > 0)
		{
			// sketchy: I'm assuming eventsToAdd.getLastEventTime() is overall size of buffer
			const auto percent = event.samplePosition / eventsToAdd.getLastEventTime();
			buffer.addEvent(message, startSample + numSamples * percent);
		}
		else
		{
			overflow.addEvent(message, message.getTimeStamp());
		}
	}
	eventsToAdd.clear();
	eventsToAdd.swapWith(overflow);
}

void Tonnetz::addMessageToList(const juce::MidiMessage &message)
{
	auto time = message.getTimeStamp();

	auto hours = ((int)(time / 3600.0)) % 24;
	auto minutes = ((int)(time / 60.0)) % 60;
	auto seconds = ((int)time) % 60;
	auto millis = ((int)(time * 1000.0)) % 1000;

	auto timecode = juce::String::formatted("%02d:%02d:%02d.%03d",
											hours,
											minutes,
											seconds,
											millis);

	logMessage(timecode + "  -  " + getMidiMessageDescription(message));
}

juce::String Tonnetz::getMidiMessageDescription(const juce::MidiMessage &m)
{
	if (m.isNoteOn())
		return "Note on " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
	if (m.isNoteOff())
		return "Note off " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
	if (m.isProgramChange())
		return "Program change " + juce::String(m.getProgramChangeNumber());
	if (m.isPitchWheel())
		return "Pitch wheel " + juce::String(m.getPitchWheelValue());
	if (m.isAftertouch())
		return "After touch " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + juce::String(m.getAfterTouchValue());
	if (m.isChannelPressure())
		return "Channel pressure " + juce::String(m.getChannelPressureValue());
	if (m.isAllNotesOff())
		return "All notes off";
	if (m.isAllSoundOff())
		return "All sound off";
	if (m.isMetaEvent())
		return "Meta event";

	if (m.isController())
	{
		juce::String name(juce::MidiMessage::getControllerName(m.getControllerNumber()));

		if (name.isEmpty())
			name = "[" + juce::String(m.getControllerNumber()) + "]";

		return "Controller " + name + ": " + juce::String(m.getControllerValue());
	}

	return juce::String::toHexString(m.getRawData(), m.getRawDataSize());
}

void Tonnetz::classicalNoteWalk()
{
	double currentTime = juce::Time::getMillisecondCounter() * 0.001;
	if (currentTime - lastNoteTime > noteDuration)
	{
		lastNoteTime = currentTime;

		switch (random.nextInt(4))
		{
		case 0:
			note += i;
			break;
		case 1:
			note -= i;
			break;
		case 2:
			note += j;
			break;
		case 3:
			note -= j;
			break;
		}

		Note n = {.duration = 2, .note = getNoteMidi(), .velocity = 0.3f};
		addMessageToBuffer(n, 1);

		noteDuration = n.duration;
	}
}

void Tonnetz::classicalChordWalk()
{

	double currentTime = juce::Time::getMillisecondCounter() * 0.001;
	if (currentTime - lastChordTime > chordDuration)
	{
		lastChordTime = currentTime;
		needsNewScale = true;
		switch (random.nextInt(3))
		{
		case 0:
			chord.one = (chord.three - chord.one) + (chord.five - chord.one) + chord.one;
			break;
		case 1:
			chord.three = (chord.one - chord.three) + (chord.five - chord.three) + chord.three;
			break;
		case 2:
			chord.five = (chord.one - chord.five) + (chord.three - chord.five) + chord.five;
			break;
		}

		triadMidi chordMidis = getChordMidi();
		Note one = {.duration = 2, .note = chordMidis.one, .velocity = 0.3f};
		Note three = {.duration = 2, .note = chordMidis.three, .velocity = 0.3f};
		Note five = {.duration = 2, .note = chordMidis.five, .velocity = 0.3f};
		Chord c = {.one = one, .three = three, .five = five};

		addMessageToBuffer(c);

		chordDuration = std::max(std::max(one.duration, three.duration), five.duration);
	}
}

void Tonnetz::perlinNoteWalk()
{
	double currentTime = juce::Time::getMillisecondCounterHiRes() * 0.001;
	if (currentTime - lastNoteTime > noteDuration)
	{
		lastNoteTime = currentTime;
		int x = static_cast<int>(round(perlin.octave1D_11(step, octaves) * 3));
		int y = static_cast<int>(round(perlin.octave1D_11(step + 100, octaves) * 3));
		note += i * x;
		note += j * y;

		float velocity = abs(perlin.octave1D_01(step + 1000, octaves) * (0.8 - 0.2) + 0.2);

		float duration = abs(perlin.octave1D_01(step, octaves)) * 4;
		duration = 0.5 + round(duration * 2) / 2;

		step += 0.3f;

		Note n = {.duration = duration, .note = getNoteMidi(), .velocity = velocity};
		addMessageToBuffer(n, 1);
		noteDuration = n.duration;
	}
}

void Tonnetz::gaussianNoteWalk()
{
	double currentTime = juce::Time::getMillisecondCounterHiRes() * 0.001;
	if (currentTime - lastNoteTime > noteDuration)
	{
		lastNoteTime = currentTime;

		randGaussianPosition(note);

		float velocity = abs(perlin.octave1D_01(step + 1000, octaves) * (0.8 - 0.2) + 0.2);

		float duration = abs(perlin.octave1D_01(step, octaves)) * 1;

		duration = 0.75 + round(duration * 4) / 4;

		step += .3f;

		Note n = {.duration = duration, .note = getNoteMidi(), .velocity = velocity};
		addMessageToBuffer(n, 1);
		noteDuration = n.duration;
	}
}

void Tonnetz::randGaussianPosition(ivec2 &n)
{
	vec2 mean = (chord.one + chord.three + chord.five) / 3;
	std::random_device rd{};
	std::mt19937 gen{rd()};

	std::normal_distribution<float> dx{mean.x, sigma};
	int x = static_cast<int>(round(dx(gen)));

	std::normal_distribution<float> dy{mean.y, sigma};
	int y = static_cast<int>(round(dy(gen)));

	n = ivec2(x, y);
}

ivec2 getRoot(ivec2 &hor1, ivec2 &hor2)
{
	// important that both arguments must be on the same horizontal line!!
	return (hor1.x < hor2.x) ? hor1 : hor2;
}

int findIndexWithMinDistance(int arr[7], int currentNote)
{
	int finalPosition;
	for (int i = 0, diff = 1000; i < 7; i++)
	{
		int currdiff = std::abs(arr[i] - currentNote);
		if (currdiff < diff)
		{
			diff = currdiff;
			finalPosition = i;
		}
	}
}

void shiftScale(int currentscale[7], int scale[7], int targetIndex, int currentIndex)
{
	int temp[7];
	// from currentIndex to the end
	for (int i = currentIndex; i < 7; i++, targetIndex = (targetIndex + 1) % 7)
		temp[i] = scale[targetIndex];
	// from begin to currentIndex
	for (int i = 0; i < currentIndex; i++, targetIndex = (targetIndex + 1) % 7)
		temp[i] = scale[targetIndex];
	for (int i = 0; i < 7; i++, currentscale[i] = temp[i]);
}

/** We are not using the tonnetz here*/
void Tonnetz::tonalCenterNoteWalk()
{

	double currentTime = juce::Time::getMillisecondCounterHiRes() * 0.001;
	if (currentTime - lastNoteTime > noteDuration)
	{
		lastNoteTime = currentTime;

		if (needsNewScale)
		{
			needsNewScale = false;
			ivec2 tonalCenter;
			bool major = false;
			if (chord.one.y == chord.three.y)
			{
				major = chord.five.y < chord.one.y;
				tonalCenter = getRoot(chord.one, chord.three);
			}
			else if (chord.one.y == chord.five.y)
			{
				major = chord.three.y < chord.one.y;
				tonalCenter = getRoot(chord.one, chord.five);
			}
			else
			{
				major = chord.one.y < chord.three.y;
				tonalCenter = getRoot(chord.three, chord.five);
			}

			int root = getMidi(tonalCenter, 60);
			if (major)
			{
				int scale[7];
				for (int i = 0; i < 7; i++)
					scale[i] = majorscale[i] + root;
				int targetIndex = findIndexWithMinDistance(scale, currentscale[currentIndex]);

				shiftScale(currentscale, scale, targetIndex, currentIndex);
			}
			else
			{
				int scale[7];
				for (int i = 0; i < 7; i++)
					scale[i] = minorscale[i] + root;
				int targetIndex = findIndexWithMinDistance(scale, currentscale[currentIndex]);

				shiftScale(currentscale, scale, targetIndex, currentIndex);
			}
		}

		currentIndex = static_cast<int>(round(perlin.octave1D_01(step, octaves) * 10));
		step += 0.3f;

		float duration = abs(perlin.octave1D_01(step, octaves)) * 1;
		duration = 0.75 + round(duration * 4) / 4;

		Note n = {.duration = duration, .note = currentscale[currentIndex], .velocity = 0.3f};
		addMessageToBuffer(n, 1);
		noteDuration = n.duration;
	}
}

void Tonnetz::noChordWalk()
{
	double currentTime = juce::Time::getMillisecondCounter() * 0.001;
	if (currentTime - lastChordTime > chordDuration)
	{
		lastChordTime = currentTime;

		triadMidi chordMidis = getChordMidi();
		Note one = {.duration = 200, .note = chordMidis.one, .velocity = 0.3f};
		Note three = {.duration = 200, .note = chordMidis.three, .velocity = 0.3f};
		Note five = {.duration = 200, .note = chordMidis.five, .velocity = 0.3f};
		Chord c = {.one = one, .three = three, .five = five};

		addMessageToBuffer(c);

		chordDuration = std::max(std::max(one.duration, three.duration), five.duration);
	}
}

uint8_t Tonnetz::getNoteMidi()
{
	return getMidi(note, 60);
}

Tonnetz::triadMidi Tonnetz::getChordMidi()
{
	triadMidi m = {.one = getMidi(chord.one, 48), .three = getMidi(chord.three, 48), .five = getMidi(chord.five, 48)};
	return m;
}

uint8_t Tonnetz::getMidi(ivec2 &point, int startNote)
{
	ivec2 p = cleanPosition(point);
	int midi = (p.x * 7) % 12 + (p.y * 3);
	midi = startNote + (midi % 12);
	return midi;
}

ivec2 Tonnetz::cleanPosition(ivec2 p)
{
	// the tonnetz repeats itself so, I'll map the point to something in the first quadrant.
	ivec2 newPoint;
	newPoint.y = (4 + (p.y % 4)) % 4;
	newPoint.x = (12 + (p.x % 12)) % 12;
	return newPoint;
}
