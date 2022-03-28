/*
  ==============================================================================

	PlaylistComponent.cpp
	Created: 7 Mar 2022 10:23:33pm
	Author:  HadarS

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2) : player1(_player1), player2(_player2)
{
	// Initialize playlist table with required headers
	tableComponent.getHeader().addColumn("Title", 1, 160);
	tableComponent.getHeader().addColumn("Duration", 2, 120);
	tableComponent.getHeader().addColumn("Size", 3, 120);
	tableComponent.getHeader().addColumn("Type", 4, 100);
	tableComponent.getHeader().addColumn("Deck 1", 5, 100);
	tableComponent.getHeader().addColumn("Deck 2", 6, 100);
	tableComponent.getHeader().addColumn("", 7, 100);
	tableComponent.setModel(this);

	addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	g.setColour(juce::Colours::white);
	g.setFont(14.0f);
	g.drawText("PlaylistComponent", getLocalBounds(),
		juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
	tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
	return tracks.size(); // This vector is enough to determine how many tracks there are in the library
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
// Paint row in orange if it's selected
{
	if (rowIsSelected) {
		g.fillAll(juce::Colours::orange);
	}
	else {
		g.fillAll(juce::Colours::darkgrey);
	}
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
/* This function populates the playlist table with the respective data based on the column being accessed */
{
	if (columnId == 1) { // Title
		g.drawText(tracks[rowNumber].getLocalFile().getFileNameWithoutExtension().toStdString(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
	if (columnId == 2) { // Duration
		g.drawText(trackDurations[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
	if (columnId == 3) { // Size
		g.drawText(sizeBytesToMegabytesString(tracks[rowNumber].getLocalFile().getSize()), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
	if (columnId == 4) { // Type
		g.drawText(tracks[rowNumber].getLocalFile().getFileExtension().toStdString(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate)
/* This function adds the play/remove buttons to the playlist table */
{
	if (columnId == 5 || columnId == 6) { // Play button (Deck 1 is column 5, Deck 2 is column 6)
		if (existingComponentToUpdate == nullptr) {
			juce::TextButton* btn = new juce::TextButton{ "play" };
			// Identifier uses column as first digit and row number as the rest
			// e.g. 523 - column 5 (i.e. Deck 1), row 23 -> "Play track #23 on Deck 1"
			std::string buttonId = std::to_string(columnId) + std::to_string(rowNumber);
			juce::String id{ buttonId };
			btn->setComponentID(id);
			btn->addListener(this);
			existingComponentToUpdate = new juce::TextButton{ "Play" };
		}
	}
	if (columnId == 7) { // Remove button
		if (existingComponentToUpdate == nullptr) {
			juce::TextButton* btn = new juce::TextButton{ "remove" };
			// Identifier uses column as first digit and row number as the rest
			// e.g. 715 - column 7 (i.e. Remove), row 15 -> "Remove track #15 from the library"
			std::string buttonId = std::to_string(columnId) + std::to_string(rowNumber);
			juce::String id{ buttonId };
			btn->setComponentID(id);
			btn->addListener(this);
			existingComponentToUpdate = new juce::TextButton{ "Remove" };
		}
	}
	return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
	DJAudioPlayer* player;

	std::string buttonId = button->getComponentID().toStdString();
	int columnId = std::stoi(buttonId.substr(0, 1)); // 5,6,7 i.e. Deck 1, Deck 2, or Remove
	int rowNumber = std::stoi(buttonId.substr(1)); // Row number i.e button ID without the column ID

	if (columnId == 7) { // Remove track
		tableComponent.removeChildComponent(tableComponent.getChildComponent(rowNumber));
		tableComponent.updateContent();
	}
	else if (columnId == 5) { // Load track to Deck 1
		player = player1;
		player->loadURL(tracks[rowNumber]);
	}
	else { // Load track to Deck 2
		player = player2;
		player->loadURL(tracks[rowNumber]);
	}

}

std::string PlaylistComponent::secondsToPlaylistDuration(double seconds)
/*
Helper function to prettify duration in seconds to more readable format, e.g. 73s -> 01:13
This goes in the `Duration` column
*/
{
	int minutes;
	minutes = (int)seconds / 60;
	std::string duration = std::to_string(minutes) + ":" + std::to_string((int)seconds % 60);
	return duration;
}

std::string PlaylistComponent::sizeBytesToMegabytesString(juce::int64 fileSizeInBytes)
/*
Helper function to prettify file size in bytes to more readable format (in megabytes, e.g. 1460000 -> 1.46 MB)
This goes in the 'Size' column
*/
{
	long double kilobytes = fileSizeInBytes / 1024;
	long double megabytes = kilobytes / 1024;
	std::string fileSize = std::to_string(megabytes) + " MB";
	return fileSize;
}

void PlaylistComponent::addTrack(juce::URL track, double trackDuration)
/* Add track to playlist and refresh the table after having added it */
{
	tracks.push_back(track);
	trackDurations.push_back(secondsToPlaylistDuration(trackDuration));
	tableComponent.updateContent();
}
