/*
  ==============================================================================

	PlaylistComponent.h
	Created: 7 Mar 2022 10:23:33pm
	Author:  HadarS

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component
{
public:
	PlaylistComponent();
	~PlaylistComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	juce::TableListBox tableComponent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
