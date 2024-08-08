/*
  ==============================================================================

    CustomButtons.h
    Created: 8 Aug 2024 12:48:33am
    Author:  blake $

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct PowerButton : juce::ToggleButton {};

struct AnalyzerButton : juce::ToggleButton
{
    void resized() override;

    juce::Path randomPath;
};