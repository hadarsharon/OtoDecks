#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

class MainComponent : public juce::AudioAppComponent
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent() override;

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	//==============================================================================
	void paint(juce::Graphics& g) override;
	void resized() override;

private:
	juce::AudioFormatManager formatManager;
	juce::AudioThumbnailCache thumbCache{ 100 };

	// Audio players for Deck 1 and Deck 2 (controls Audio playback etc.)
	DJAudioPlayer player1{ formatManager };
	DJAudioPlayer player2{ formatManager };

	// Playlist/Library component
	PlaylistComponent playlistComponent{ &player1, &player2 };

	// Deck 1 and Deck 2 GUIs respectively
	DeckGUI deckGUI1{ &player1, formatManager, thumbCache, &playlistComponent };
	DeckGUI deckGUI2{ &player2, formatManager, thumbCache, &playlistComponent };

	juce::MixerAudioSource mixerSource;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
