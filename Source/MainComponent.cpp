#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
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

	addAndMakeVisible(deckGUI1);
	addAndMakeVisible(deckGUI2);

	addAndMakeVisible(playlistComponent);

	formatManager.registerBasicFormats();
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
	player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

	mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

	mixerSource.addInputSource(&player1, false);
	mixerSource.addInputSource(&player2, false);
}


void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
	// Once we no longer need the audio players, release their resources to prevent memory leaks and the like
	player1.releaseResources();
	player2.releaseResources();
	mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	// This is called when the MainContentComponent is resized.
	deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() / 2);
	deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);
	playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}