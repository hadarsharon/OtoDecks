/*
  ==============================================================================

	PlaylistComponent.h
	Created: 7 Mar 2022 10:23:33pm
	Author:  HadarS

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener
{
public:
	PlaylistComponent(DJAudioPlayer* player1, DJAudioPlayer* player2);
	~PlaylistComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	int getNumRows() override;
	void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;
	void buttonClicked(juce::Button* button) override;

	/* Add a new track to the playlist after loading it using the Load button */
	void addTrack(juce::URL track, double trackDuration);


private:
	/* Playlist table component to be rendered with data as tracks load */
	juce::TableListBox tableComponent;

	/* Stores the files so as to be able to access all of their attributes */
	std::vector<juce::URL> tracks;

	/* Stores the file durations since these are calculated from the player with the loaded audio and not the file itself */
	std::vector<std::string> trackDurations;

	/* References to the 2 decks' players so as to use their methods and the like */
	DJAudioPlayer* player1;
	DJAudioPlayer* player2;

	/* Helper functions for prettifying the playlist table meta data - for example file size in MB instead of bytes */
	std::string secondsToPlaylistDuration(double seconds);
	std::string sizeBytesToMegabytesString(juce::int64 fileSizeInBytes);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
