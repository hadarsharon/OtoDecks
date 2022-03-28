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

	void addTrack(juce::URL track, double trackDuration);


private:
	juce::TableListBox tableComponent;
	std::vector<juce::URL> tracks;
	std::vector<std::string> trackDurations;
	
	DJAudioPlayer* player1;
	DJAudioPlayer* player2;


	std::string secondsToPlaylistDuration(double seconds);
	std::string sizeBytesToMegabytesString(juce::int64 fileSizeInBytes);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
