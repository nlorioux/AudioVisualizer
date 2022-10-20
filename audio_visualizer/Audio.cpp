#include "Audio.h"

Audio::Audio(const string fileName) {
	if (!buffer.loadFromFile(fileName))
	{
		cerr << "File couldn't open." << endl;
	}
	else {
		music.setBuffer(buffer);
		samples = buffer.getSamples();
		samplesCount = buffer.getSampleCount();
		duration = buffer.getDuration();
	}
}

size_t Audio::getSamplesCount() {
	return samplesCount;
}

const sf::Int16* Audio::getSamples() {
	return samples;
}

sf::Time Audio::getDuration() {
	return duration;
}