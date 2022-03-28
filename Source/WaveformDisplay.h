/*
  ==============================================================================

	WaveformDisplay.h
	Created: 5 Mar 2022 7:38:57pm
	Author:  HadarS

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay : public juce::Component, public juce::ChangeListener
{
public:
	WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
	~WaveformDisplay() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void changeListenerCallback(juce::ChangeBroadcaster* source) override;

	/** Used to render the Waveform based on some input file URL */
	void loadURL(juce::URL audioURL);

	/** set the relative position of the playhead */
	void setPositionRelative(double pos);

private:
	juce::AudioThumbnail audioThumb;
	bool fileLoaded; // Flag to determine whether a waveform can be rendered or should we display a "File not loaded" message
	double position; // Position of the audio head within the waveform for the sake of moving along with the audio file as it plays

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
