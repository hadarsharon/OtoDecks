/*
  ==============================================================================

	DJAudioPlayer.cpp
	Created: 5 Mar 2022 1:19:48pm
	Author:  HadarS

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	// audio-processing code goes here
	resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.
	transportSource.releaseResources();
	resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL) {
	/* This function actually loads the file using its URL and attaches it to the audio source */
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
/* Pertains to the `Volume` slider in the GUI */
{
	if (gain < 0 || gain > 1.5) {
		DBG("DJAudioPlayer::setGain gain should be between 0 and 1.5 ");
	}
	else {
		transportSource.setGain(gain);
	}
}

void DJAudioPlayer::setSpeed(double ratio)
/* Pertains to the `Speed` slider in the GUI */
{
	if (ratio < 0 || ratio > 100) {
		DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 1 ");
	}
	else {
		resampleSource.setResamplingRatio(ratio);
	}
}

void DJAudioPlayer::setPosition(double posInSecs)
/* Pertains to the `Position` slider in the GUI */
{
	transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
/* Used by the `Position` slider in the GUI so as to travel back and forth in the audio file */
{
	if (pos < 0 || pos > 1) {
		DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1 ");
	}
	else {
		double posInSecs = transportSource.getLengthInSeconds() * pos;
		setPosition(posInSecs);
	}
}

void DJAudioPlayer::start()
{
	transportSource.start();
}

void DJAudioPlayer::stop()
{
	transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
/* Getter function to get the relative position in the audio file (between 0.0 i.e. start and 1.0 i.e end) */
{
	if (transportSource.getLengthInSeconds()) {
		return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
	}
	else {
		return 0.0;
	}
}

double DJAudioPlayer::getPositionAbsolute()
/* Getter function to get the absolute position in seconds within the audio file */
{
	if (transportSource.getCurrentPosition()) {
		return transportSource.getCurrentPosition();
	}
	else {
		return 0.0;
	}
}

double DJAudioPlayer::getLengthInSeconds()
/* Getter function to get the total length in seconds of the audio file */
{
	if (transportSource.getLengthInSeconds()) {
		return transportSource.getLengthInSeconds();
	}
	else {
		return 0.0;
	}
}

float DJAudioPlayer::getGainInDecibels()
/* Getter function to convert the audio file gain to decibels, to show in the UI above waveform */
{
	if (transportSource.getGain()) {
		return juce::Decibels::gainToDecibels(transportSource.getGain());
	}
	else {
		return -std::numeric_limits<float>::infinity(); // no volume equals -INF dB for the sake of displaying a valid value
	}
}
