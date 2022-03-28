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
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

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
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/

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
	// This method is where you should set the bounds of any child
	// components that your component contains..
	tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
	return tracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected) {
		g.fillAll(juce::Colours::orange);
	}
	else {
		g.fillAll(juce::Colours::darkgrey);
	}
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
	if (columnId == 1) {
		g.drawText(tracks[rowNumber].getLocalFile().getFileNameWithoutExtension().toStdString(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
	if (columnId == 2) {
		g.drawText(trackDurations[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
	if (columnId == 3) {
		g.drawText(sizeBytesToMegabytesString(tracks[rowNumber].getLocalFile().getSize()), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
	if (columnId == 4) {
		g.drawText(tracks[rowNumber].getLocalFile().getFileExtension().toStdString(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate)
{
	if (columnId == 5 || columnId == 6) {
		if (existingComponentToUpdate == nullptr) {
			juce::TextButton* btn = new juce::TextButton{ "play" };
			std::string buttonId = std::to_string(columnId) + std::to_string(rowNumber);
			juce::String id{buttonId};
			btn->setComponentID(id);
			btn->addListener(this);
			existingComponentToUpdate = new juce::TextButton{ "Play" };
		}
	}
	if (columnId == 7) {
		if (existingComponentToUpdate == nullptr) {
			juce::TextButton* btn = new juce::TextButton{ "remove" };
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
	int columnId = std::stoi(buttonId.substr(0, 1));
	int rowNumber = std::stoi(buttonId.substr(1));

	if (columnId == 7) {
		tableComponent.removeChildComponent(tableComponent.getChildComponent(rowNumber));
		tableComponent.updateContent();
	}
	else if (columnId == 5) {
		player = player1;
		player->loadURL(tracks[rowNumber]);
	}
	else {
		player = player2;
		player->loadURL(tracks[rowNumber]);
	}

}

std::string PlaylistComponent::secondsToPlaylistDuration(double seconds)
{
	int minutes;
	minutes = (int)seconds / 60;
	std::string duration = std::to_string(minutes) + ":" + std::to_string((int)seconds % 60);
	return duration;
}

std::string PlaylistComponent::sizeBytesToMegabytesString(juce::int64 fileSizeInBytes)
{
	long double kilobytes = fileSizeInBytes / 1024;
	long double megabytes = kilobytes / 1024;
	std::string fileSize = std::to_string(megabytes) + " MB";
	return fileSize;
}

void PlaylistComponent::addTrack(juce::URL track, double trackDuration)
{
	tracks.push_back(track);
	trackDurations.push_back(secondsToPlaylistDuration(trackDuration));
	tableComponent.updateContent();
}
