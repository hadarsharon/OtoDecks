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
	trackTitles.push_back("Track 1");
	trackTitles.push_back("Track 2");

	tableComponent.getHeader().addColumn("Track title", 1, 400);
	tableComponent.getHeader().addColumn("Artist", 2, 400);
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
	g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}
