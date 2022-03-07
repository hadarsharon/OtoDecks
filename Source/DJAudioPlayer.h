/*
  ==============================================================================

	DJAudioPlayer.h
	Created: 5 Mar 2022 1:19:48pm
	Author:  HadarS

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource {
public:
	DJAudioPlayer(juce::AudioFormatManager& _formatManager);
	~DJAudioPlayer();

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	void loadURL(juce::URL audioURL);
	void setGain(double gain);
	void setSpeed(double ratio);
	void setPosition(double posInSecs);
	void setPositionRelative(double pos);

	void start();
	void stop();

private:
	juce::AudioFormatManager& formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};