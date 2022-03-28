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
PlaylistComponent::PlaylistComponent()
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	tableComponent.getHeader().addColumn("Title", 1, 250);
	tableComponent.getHeader().addColumn("Duration", 2, 250);
	tableComponent.getHeader().addColumn("Size", 3, 250);
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
	return trackTitles.size();
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
		g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
	if (columnId == 2) {
		g.drawText(trackDurations[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
	if (columnId == 3) {
		g.drawText(trackSizes[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}
}

//juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate)
//{
//	if (columnId == 1) {
//		if (existingComponentToUpdate == nullptr) {
//			juce::TextButton* btn = new juce::TextButton{ "play" };
//			juce::String id{std::to_string(rowNumber)};
//			btn->setComponentID(id);
//			btn->addListener(this);
//			existingComponentToUpdate = new juce::TextButton{ "Play" };
//		}
//	}
//	return existingComponentToUpdate;
//}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
	int id = std::stoi(button->getComponentID().toStdString());
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

void PlaylistComponent::addTrack(std::string trackName, double trackDuration, juce::int64 trackSize)
{
	trackTitles.push_back(trackName);
	trackDurations.push_back(secondsToPlaylistDuration(trackDuration));
	trackSizes.push_back(sizeBytesToMegabytesString(trackSize));
	tableComponent.updateContent();
}
