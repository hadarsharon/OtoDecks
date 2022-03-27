/*
  ==============================================================================

	DeckGUI.cpp
	Created: 5 Mar 2022 3:53:33pm
	Author:  HadarS

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(
	DJAudioPlayer* _player,
	juce::AudioFormatManager& formatManagerToUse,
	juce::AudioThumbnailCache& cacheToUse) : player(_player),
	waveformDisplay(formatManagerToUse, cacheToUse)
{
	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);
	addAndMakeVisible(loadButton);

	addAndMakeVisible(volSlider);
	addAndMakeVisible(speedSlider);
	addAndMakeVisible(posSlider);

	addAndMakeVisible(volLabel);
	addAndMakeVisible(speedLabel);
	addAndMakeVisible(posLabel);

	addAndMakeVisible(waveformDisplay);

	playButton.addListener(this);
	stopButton.addListener(this);
	loadButton.addListener(this);

	volSlider.addListener(this);
	speedSlider.addListener(this);
	posSlider.addListener(this);

	// TODO: move to function
	volSlider.setRange(0.0, 100.0, 1.0);
	speedSlider.setRange(0.0, 200.0, 1.0);
	posSlider.setRange(0.0, 0.0, 1.0);

	volSlider.setValue(50.0);
	speedSlider.setValue(100.0);
	posSlider.setValue(0.0);

	volSlider.setNumDecimalPlacesToDisplay(0);
	speedSlider.setNumDecimalPlacesToDisplay(0);
	posSlider.setNumDecimalPlacesToDisplay(0);

	volSlider.setTextValueSuffix(" %");
	speedSlider.setTextValueSuffix(" %");
	posSlider.setTextValueSuffix(" s");

	volSlider.setEnabled(false);
	speedSlider.setEnabled(false);
	posSlider.setEnabled(false);

	volLabel.setText("Volume", juce::dontSendNotification);
	speedLabel.setText("Speed", juce::dontSendNotification);
	posLabel.setText("Position", juce::dontSendNotification);
	
	volLabel.attachToComponent(&volSlider, true);
	speedLabel.attachToComponent(&speedSlider, true);
	posLabel.attachToComponent(&posSlider, true);

	startTimer(500);
}

DeckGUI::~DeckGUI()
{
	stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
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
	//g.drawText("DeckGUI", getLocalBounds(),
	//	juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	int rowH = getHeight() / 8;
	auto sliderLeft = 60;
	playButton.setBounds(0, 0, getWidth(), rowH);
	stopButton.setBounds(0, rowH, getWidth(), rowH);
	volSlider.setBounds(sliderLeft, rowH * 2, getWidth() - sliderLeft - 6, rowH);
	speedSlider.setBounds(sliderLeft, rowH * 3, getWidth() - sliderLeft - 6, rowH);
	posSlider.setBounds(sliderLeft, rowH * 4, getWidth() - sliderLeft - 6, rowH);
	waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
	loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button) {
	if (button == &playButton) {
		DBG("Play button was clicked ");
		player->start();
	}
	if (button == &stopButton) {
		DBG("Stop button was clicked ");
		player->stop();
	}
	if (button == &loadButton) {
		auto dlgFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
		chooser.launchAsync(dlgFlags, [this](const juce::FileChooser& chooser) {
			auto fileUri = chooser.getURLResult();
			player->loadURL(fileUri);
			waveformDisplay.loadURL(fileUri);
			speedSlider.setValue(100.0);
			double AudioFileLengthInSeconds = player->getLengthInSeconds();
			posSlider.setRange(0, AudioFileLengthInSeconds, 1.0);
			posSlider.setValue(0.0);
			std::string newPosSuffix = " s / " + std::to_string((int)AudioFileLengthInSeconds) + " s";
			posSlider.setTextValueSuffix(newPosSuffix);
			volSlider.setEnabled(true);
			speedSlider.setEnabled(true);
			posSlider.setEnabled(true);
			});
	}
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
	if (slider == &volSlider) {
		player->setGain(slider->getValue() / 100);
	}
	if (slider == &speedSlider) {
		player->setSpeed(slider->getValue() / 100);
	}
	if (slider == &posSlider) {
		player->setPosition(slider->getValue());
	}
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
	return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
	if (files.size() == 1) {
		player->loadURL(juce::URL(juce::File{ files[0] }));
	}
}

void DeckGUI::timerCallback()
{
	waveformDisplay.setPositionRelative(player->getPositionRelative());
}
