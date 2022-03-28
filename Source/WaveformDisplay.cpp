/*
  ==============================================================================

	WaveformDisplay.cpp
	Created: 5 Mar 2022 7:38:57pm
	Author:  HadarS

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(
	juce::AudioFormatManager& formatManagerToUse,
	juce::AudioThumbnailCache& cacheToUse) : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
{
	audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	g.setColour(juce::Colours::orange);

	if (fileLoaded) {
		audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
		g.setColour(juce::Colours::lightgreen);
		g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
	}
	else {
		g.setFont(20.0f);
		g.drawText("File not loaded...", getLocalBounds(),
			juce::Justification::centred, true);   // draw some placeholder text when no waveform can be drawn
	}
}

void WaveformDisplay::resized()
{
	// noop; no child components
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	repaint();
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
	audioThumb.clear(); // clear existing waveform before loading a new one based on new file
	fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
	if (fileLoaded) {
		repaint();
	}
	else {
		DBG("WaveformDisplay::loadURL: not loaded!");
	}
}

void WaveformDisplay::setPositionRelative(double pos)
/* Position the audio head according to the current audio file */
{
	if (pos != position) {
		position = pos;
		repaint();
	}
}
