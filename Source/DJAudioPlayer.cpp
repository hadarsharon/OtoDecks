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
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.
	formatManager.registerBasicFormats();
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	// audio-processing code goes here
	transportSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.
	transportSource.releaseResources();
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
	transportSource.start();
}

void DJAudioPlayer::stop()
{
	transportSource.stop();
}
