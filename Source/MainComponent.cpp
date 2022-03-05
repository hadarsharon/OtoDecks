#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
	// Make sure you set the size of the component after
	// you add any child components.
	setSize(800, 600);

	// Some platforms require permissions to open input channels so request that here
	if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
		&& !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
	{
		juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
			[&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
	}
	else
	{
		// Specify the number of input and output channels that we want to open
		setAudioChannels(0, 2);
	}

	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);
	addAndMakeVisible(loadButton);
	addAndMakeVisible(volSlider);
	addAndMakeVisible(speedSlider);

	playButton.addListener(this);
	stopButton.addListener(this);
	loadButton.addListener(this);
	volSlider.addListener(this);
	speedSlider.addListener(this);
	
	volSlider.setRange(0.0, 1.0);
}

MainComponent::~MainComponent()
{
	// This shuts down the audio device and clears the audio source.
	shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.

	// You can use this function to initialise any resources you might need,
	// but be careful - it will be called on the audio thread, not the GUI thread.

	// For more details, see the help for AudioProcessor::prepareToPlay()

	player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
	
	//formatManager.registerBasicFormats();
	//juce::URL audioURL{ "file:///C:\\workspace\\OtoDecks\\Source\\tracks/aon_inspired.mp3" };

	//transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	//resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


/* SYNTHESIS CODE */
//void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
//{
//	// Your audio-processing code goes here!
//
//	// For more details, see the help for AudioProcessor::getNextAudioBlock()
//
//	// Right now we are not producing any data, in which case we need to clear the buffer
//	// (to prevent the output of random noise)
//
//	auto* leftChan = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
//	auto* rightChan = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
//
//	for (auto i = 0; i < bufferToFill.numSamples; ++i) {
//		// double sample = rand.nextDouble() * 0.25;
//		// double sample = fmod(phase, 0.2);
//		double sample = sin(phase) * 0.1;
//
//		leftChan[i] = sample;
//		rightChan[i] = sample;
//
//		phase += dphase;
//	}
//
//	// bufferToFill.clearActiveBufferRegion();
//}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	player1.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
	// For more details, see the help for AudioProcessor::releaseResources()
	player1.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	// You can add your drawing code here!
}

void MainComponent::resized()
{
	// This is called when the MainContentComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
	double rowH = getHeight() / 5.0;
	playButton.setBounds(0, 0, getWidth(), rowH);
	stopButton.setBounds(0, rowH, getWidth(), rowH);

	volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
	speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);

	loadButton.setBounds(0, rowH * 4, getWidth(), rowH);
}

void MainComponent::buttonClicked(juce::Button* button) {
	if (button == &playButton) {
		DBG("Play button was clicked ");
		player1.start();
	}
	if (button == &stopButton) {
		DBG("Stop button was clicked ");
		player1.stop();
	}
	if (button == &loadButton) {
		auto dlgFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
		chooser.launchAsync(dlgFlags, [this](const juce::FileChooser& chooser) {
			auto fileUri = chooser.getURLResult();
			player1.loadURL(fileUri);
			});
	}
}

void MainComponent::sliderValueChanged(juce::Slider* slider) {
	if (slider == &volSlider) {
		player1.setGain(slider->getValue());
	}
	if (slider == &speedSlider) {
		player1.setSpeed(slider->getValue());
	}
}