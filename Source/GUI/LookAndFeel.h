/*
  ==============================================================================

    LookAndFeel.h
    Created: 8 Aug 2024 12:34:10am
    Author:  blake $

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "LookAndFeel.h"

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics&,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider&) override;

    void drawToggleButton(juce::Graphics& g,
        juce::ToggleButton& toggleButton,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;
};