#pragma once
#include <iostream>
#include <SFML/Audio.hpp>
#include <string>
using namespace std;

class Audio {
private:
	sf::SoundBuffer buffer;
	sf::Sound music;
	const sf::Int16* samples;
	size_t samplesCount;
	sf::Time duration;
public:
	Audio(const string fileName);
	const sf::Int16* getSamples();
	size_t getSamplesCount();
	sf::Time getDuration();
};