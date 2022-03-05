/*
  ==============================================================================

	DJAudioPlayer.cpp
	Created: 5 Mar 2022 1:19:48pm
	Author:  HadarS

  ==============================================================================
*/

#include "DJAudioPlayer.h"

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
}

void DJAudioPlayer::releaseResources()
{
}

void DJAudioPlayer::loadURL(juce::URL audioURL) {
	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
	if (reader != nullptr) { // good file!
		std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
	}
	else {
		DBG("Something went wrong when loading the file");
	}
}

void DJAudioPlayer::setGain(double gain)
{
}

void DJAudioPlayer::setSpeed(double ratio)
{
}

void DJAudioPlayer::setPosition(double posInSecs)
{
}

void DJAudioPlayer::start()
{
}

void DJAudioPlayer::stop()
{
}
