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
	// Render all objects first
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

	// Add listeners to buttons
	playButton.addListener(this);
	stopButton.addListener(this);
	loadButton.addListener(this);
	rewindButton.addListener(this);
	forwardButton.addListener(this);

	// Add texts to labels where needed
	rewindLabel.setText("REWIND", juce::dontSendNotification); // add REWIND as text to arrow button
	forwardLabel.setText("FORWARD", juce::dontSendNotification); // ditto but FORWARD

	rewindLabel.attachToComponent(&rewindButton, false);
	forwardLabel.attachToComponent(&forwardButton, false);

	dbBox.setText("File not loaded...", juce::dontSendNotification);
	dbBox.setJustificationType(juce::Justification::centred);

	// Add listeners to sliders
	granularitySlider.addListener(this);
	posSlider.addListener(this);
	speedSlider.addListener(this);
	volSlider.addListener(this);

	// Set initial sliders ranges (this may change after file is loaded)
	// For example position is set to 0.0, 0.0 since we will determine how many seconds it can go only once we know the audio file length\
	// Interval (3rd argument) can be set as 1.0 for all, to allow smooth movement without overkill precision
	granularitySlider.setRange(1.0, 10.0, 1.0);
	posSlider.setRange(0.0, 0.0, 1.0);
	speedSlider.setRange(0.0, 200.0, 1.0);
	volSlider.setRange(0.0, 150.0, 1.0);

	// Set initial values for the sliders (defaults)
	granularitySlider.setValue(1.0);
	posSlider.setValue(0.0);
	speedSlider.setValue(100.0);
	volSlider.setValue(75.0);
	granularitySlider.setValue(0);

	// Truncate decimal places from values as they can easily be displayed as whole integers
	posSlider.setNumDecimalPlacesToDisplay(0);
	speedSlider.setNumDecimalPlacesToDisplay(0);
	volSlider.setNumDecimalPlacesToDisplay(0);

	// Set text suffixes for slider labels accordingly i.e. speed is in percentage, position is in seconds...
	granularitySlider.setTextValueSuffix(" s");
	posSlider.setTextValueSuffix(" s");
	speedSlider.setTextValueSuffix(" %");
	volSlider.setTextValueSuffix(" %");

	// Disable sliders initially, re-enabled only after file has been loaded
	posSlider.setEnabled(false);
	speedSlider.setEnabled(false);
	volSlider.setEnabled(false);

	// Set slider labels text to signify which slider is which
	granularityLabel.setText("Granularity", juce::dontSendNotification);
	posLabel.setText("Position", juce::dontSendNotification);
	speedLabel.setText("Speed", juce::dontSendNotification);
	volLabel.setText("Volume", juce::dontSendNotification);

	granularityLabel.attachToComponent(&granularitySlider, true);
	posLabel.attachToComponent(&posSlider, true);
	speedLabel.attachToComponent(&speedSlider, true);
	volLabel.attachToComponent(&volSlider, true);

	// Timer for updating the Waveform audio head every 500 ms
	startTimer(500);
}

DeckGUI::~DeckGUI()
{
	stopTimer(); // release timer when component is destroyed to prevent memory leak
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

	// set buttons bounds and placement
	playButton.setBounds(0, 0, getWidth() / 3.5, rowH * 2);
	stopButton.setBounds(getWidth() / 3.5 + buttonsMargin, 0, getWidth() / 3.5, rowH * 2);
	rewindButton.setBounds(getWidth() / 1.6, rowH, getWidth() / 7, rowH);
	forwardButton.setBounds(getWidth() / 1.25, rowH, getWidth() / 7, rowH);

	// set sliders bounds and placement
	granularitySlider.setBounds(sliderLeft, rowH * 3, getWidth() - sliderLeft - 6, rowH);
	posSlider.setBounds(sliderLeft, rowH * 4, getWidth() - sliderLeft - 6, rowH);
	speedSlider.setBounds(sliderLeft, rowH * 5, getWidth() - sliderLeft - 6, rowH);
	volSlider.setBounds(sliderLeft, rowH * 6, getWidth() - sliderLeft - 6, rowH);

	// Ditto for remaining components
	dbBox.setBounds(0, rowH * 7, getWidth(), rowH);
	waveformDisplay.setBounds(0, rowH * 8, getWidth(), rowH * 2);
	loadButton.setBounds(0, rowH * 10, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button) {
	if (button == &playButton) { // PLAY
		player->start();
	}
	if (button == &stopButton) { // STOP
		player->stop();
	}
	if (button == &rewindButton) { // REWIND
		player->setPosition(player->getPositionAbsolute() - granularitySlider.getValue());
	}
	if (button == &forwardButton) { // FAST-FORWARD
		player->setPosition(player->getPositionAbsolute() + granularitySlider.getValue());
	}
	if (button == &loadButton) { // LOAD
		auto dlgFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
		chooser.launchAsync(dlgFlags, [this](const juce::FileChooser& chooser) {
			juce::URL fileUri = chooser.getURLResult();
			if (!fileUri.isEmpty()) { // verify file path exists before re-enabling everything and loading...
				player->loadURL(fileUri); // audio player loads the file
				waveformDisplay.loadURL(fileUri); // waveform loads the file (for visualization)

				speedSlider.setValue(100.0); // speedslider defaults to 100%

				double AudioFileLengthInSeconds = player->getLengthInSeconds();
				posSlider.setRange(0, AudioFileLengthInSeconds, 1.0); // position slider range is defined based on audio length
				posSlider.setValue(0.0); // reset position slider to 0 every time new audio file is loaded
				// update position slider suffix to account for total file length as well
				// e.g. (5s / 320s) - we are 5 seconds into a 320 seconds file...
				std::string newPosSuffix = " s / " + std::to_string((int)AudioFileLengthInSeconds) + " s";
				posSlider.setTextValueSuffix(newPosSuffix);

				// enable sliders
				volSlider.setEnabled(true);
				speedSlider.setEnabled(true);
				posSlider.setEnabled(true);

				// render text box of noise in decibels based on the audio file gain
				std::string gainInDecibels = std::to_string(player->getGainInDecibels()) + " dB";
				dbBox.setText(gainInDecibels, juce::dontSendNotification);

				// finally, add loaded audio file and its meta data to library using the playlist method
				playlist->addTrack(fileUri, AudioFileLengthInSeconds);
			}
			});
	}
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
	if (slider == &posSlider) { // Position slider changed
		player->setPosition(slider->getValue());
	}
	if (slider == &speedSlider) { // Speed slider changed
		player->setSpeed(slider->getValue() / 100);
	}
	if (slider == &volSlider) { // Volume slider changed
		player->setGain(slider->getValue() / 100);
		if (slider->isEnabled()) {
			// also update noise in decibels accordingly
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
