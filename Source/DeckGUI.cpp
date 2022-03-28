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
	juce::AudioThumbnailCache& cacheToUse,
	PlaylistComponent* _playlist) : player(_player), playlist(_playlist),
	waveformDisplay(formatManagerToUse, cacheToUse)
{
	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);
	addAndMakeVisible(loadButton);
	addAndMakeVisible(rewindButton);
	addAndMakeVisible(forwardButton);

	addAndMakeVisible(rewindLabel);
	addAndMakeVisible(forwardLabel);

	addAndMakeVisible(dbBox);

	addAndMakeVisible(granularitySlider);
	addAndMakeVisible(posSlider);
	addAndMakeVisible(speedSlider);
	addAndMakeVisible(volSlider);

	addAndMakeVisible(granularityLabel);
	addAndMakeVisible(posLabel);
	addAndMakeVisible(speedLabel);
	addAndMakeVisible(volLabel);

	addAndMakeVisible(waveformDisplay);

	playButton.addListener(this);
	stopButton.addListener(this);
	loadButton.addListener(this);
	rewindButton.addListener(this);
	forwardButton.addListener(this);

	rewindLabel.setText("REWIND", juce::dontSendNotification);
	forwardLabel.setText("FORWARD", juce::dontSendNotification);

	rewindLabel.attachToComponent(&rewindButton, false);
	forwardLabel.attachToComponent(&forwardButton, false);

	dbBox.setText("File not loaded...", juce::dontSendNotification);
	dbBox.setJustificationType(juce::Justification::centred);

	granularitySlider.addListener(this);
	posSlider.addListener(this);
	speedSlider.addListener(this);
	volSlider.addListener(this);

	// TODO: move to function
	granularitySlider.setRange(1.0, 10.0, 1.0);
	posSlider.setRange(0.0, 0.0, 1.0);
	speedSlider.setRange(0.0, 200.0, 1.0);
	volSlider.setRange(0.0, 150.0, 1.0);

	granularitySlider.setValue(1.0);
	posSlider.setValue(0.0);
	speedSlider.setValue(100.0);
	volSlider.setValue(75.0);

	granularitySlider.setValue(0);
	posSlider.setNumDecimalPlacesToDisplay(0);
	speedSlider.setNumDecimalPlacesToDisplay(0);
	volSlider.setNumDecimalPlacesToDisplay(0);

	granularitySlider.setTextValueSuffix(" s");
	posSlider.setTextValueSuffix(" s");
	speedSlider.setTextValueSuffix(" %");
	volSlider.setTextValueSuffix(" %");

	posSlider.setEnabled(false);
	speedSlider.setEnabled(false);
	volSlider.setEnabled(false);

	granularityLabel.setText("Granularity", juce::dontSendNotification);
	posLabel.setText("Position", juce::dontSendNotification);
	speedLabel.setText("Speed", juce::dontSendNotification);
	volLabel.setText("Volume", juce::dontSendNotification);
	
	granularityLabel.attachToComponent(&granularitySlider, true);
	posLabel.attachToComponent(&posSlider, true);
	speedLabel.attachToComponent(&speedSlider, true);
	volLabel.attachToComponent(&volSlider, true);

	startTimer(500);
}

DeckGUI::~DeckGUI()
{
	stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	g.setColour(juce::Colours::white);
	g.setFont(14.0f);
}

void DeckGUI::resized()
{
	int numRows = 11;
	int rowH = getHeight() / numRows;
	auto sliderLeft = 60; // margin for sliders to fit labels
	int buttonsMargin = 5; // margin for play/stop/rewind/forward buttons
	playButton.setBounds(0, 0, getWidth() / 3.5, rowH * 2);
	stopButton.setBounds(getWidth() / 3.5 + buttonsMargin, 0, getWidth() / 3.5, rowH * 2);
	rewindButton.setBounds(getWidth() / 1.6, rowH, getWidth() / 7, rowH);
	forwardButton.setBounds(getWidth() / 1.25, rowH, getWidth() / 7, rowH);
	granularitySlider.setBounds(sliderLeft, rowH * 3, getWidth() - sliderLeft - 6, rowH);
	posSlider.setBounds(sliderLeft, rowH * 4, getWidth() - sliderLeft - 6, rowH);
	speedSlider.setBounds(sliderLeft, rowH * 5, getWidth() - sliderLeft - 6, rowH);
	volSlider.setBounds(sliderLeft, rowH * 6, getWidth() - sliderLeft - 6, rowH);
	dbBox.setBounds(0, rowH * 7, getWidth(), rowH);
	waveformDisplay.setBounds(0, rowH * 8, getWidth(), rowH * 2);
	loadButton.setBounds(0, rowH * 10, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button) {
	if (button == &playButton) {
		player->start();
	}
	if (button == &stopButton) {
		player->stop();
	}
	if (button == &rewindButton) {
		player->setPosition(player->getPositionAbsolute() - granularitySlider.getValue());
	}
	if (button == &forwardButton) {
		player->setPosition(player->getPositionAbsolute() + granularitySlider.getValue());
	}
	if (button == &loadButton) {
		auto dlgFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
		chooser.launchAsync(dlgFlags, [this](const juce::FileChooser& chooser) {
			juce::URL fileUri = chooser.getURLResult();
			if (!fileUri.isEmpty()) {
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
				std::string gainInDecibels = std::to_string(player->getGainInDecibels()) + " dB";
				dbBox.setText(gainInDecibels, juce::dontSendNotification);
				playlist->addTrack(fileUri,	AudioFileLengthInSeconds);
			}
			});
	}
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
	if (slider == &posSlider) {
		player->setPosition(slider->getValue());
	}
	if (slider == &speedSlider) {
		player->setSpeed(slider->getValue() / 100);
	}
	if (slider == &volSlider) {
		player->setGain(slider->getValue() / 100);
		if (slider->isEnabled()) {
			std::string gainInDecibels = std::to_string(player->getGainInDecibels()) + " dB";
			dbBox.setText(gainInDecibels, juce::dontSendNotification);
		}
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
