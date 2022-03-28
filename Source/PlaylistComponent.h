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

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener
{
public:
	PlaylistComponent();
	~PlaylistComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	int getNumRows() override;
	void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	//juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;
	void buttonClicked(juce::Button* button) override;

	void addTrack(std::string trackName, double trackDuration, juce::int64 trackSize);


private:
	juce::TableListBox tableComponent;
	std::vector<std::string> trackTitles;
	std::vector<std::string> trackDurations;
	std::vector<std::string> trackSizes;

	std::string secondsToPlaylistDuration(double seconds);
	std::string sizeBytesToMegabytesString(juce::int64 fileSizeInBytes);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
