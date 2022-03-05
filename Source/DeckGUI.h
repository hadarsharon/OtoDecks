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

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener
{
public:
	DeckGUI(DJAudioPlayer* player);
	~DeckGUI() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	/** implement Button::Listener */
	void buttonClicked(juce::Button*) override;

	/** implement Slider::Listener */
	void sliderValueChanged(juce::Slider* slider) override;

private:
	juce::TextButton playButton{ "PLAY" };
	juce::TextButton stopButton{ "STOP" };
	juce::TextButton loadButton{ "LOAD" };

	juce::Slider volSlider;
	juce::Slider speedSlider;
	juce::Slider posSlider;

	juce::FileChooser chooser{ "Select an audio file..." };

	DJAudioPlayer* player;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
