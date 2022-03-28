/*
  ==============================================================================

	DeckGUI.h
	Created: 5 Mar 2022 3:53:33pm
	Author:  HadarS

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "PlaylistComponent.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
	public juce::FileDragAndDropTarget,
	public juce::Timer
{
public:
	DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, PlaylistComponent* playlist);
	~DeckGUI() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	/** implement Button::Listener */
	void buttonClicked(juce::Button*) override;

	/** implement Slider::Listener */
	void sliderValueChanged(juce::Slider* slider) override;

	bool isInterestedInFileDrag(const juce::StringArray& files) override;
	void filesDropped(const juce::StringArray& files, int x, int y) override;

	void timerCallback() override;

private:
	/* Playback buttons (Top of component) */
	juce::TextButton playButton{ "PLAY" };
	juce::TextButton stopButton{ "STOP" };
	juce::TextButton loadButton{ "LOAD" };
	juce::ArrowButton rewindButton{ "REWIND" , 0.5, juce::Colours::lightgreen };
	juce::ArrowButton forwardButton{ "FORWARD" , 0.0, juce::Colours::lightgreen };

	/* Add REWIND/FAST-FORWARD labels to arrow buttons since they have no built-in text */
	juce::Label rewindLabel;
	juce::Label forwardLabel;

	/* Text box that shows noise in Decibels (just above the waveform) */
	juce::Label dbBox;

	/* Sliders */
	juce::Slider granularitySlider;
	juce::Slider posSlider;
	juce::Slider volSlider;
	juce::Slider speedSlider;

	/* Labels for Sliders */
	juce::Label granularityLabel;
	juce::Label posLabel;
	juce::Label volLabel;
	juce::Label speedLabel;

	/* File chooser to couple with Load button */
	juce::FileChooser chooser{ "Select an audio file..." };

	DJAudioPlayer* player;

	PlaylistComponent* playlist;

	WaveformDisplay waveformDisplay;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
