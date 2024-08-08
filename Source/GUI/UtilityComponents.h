/*
  ==============================================================================

    UtilityComponents.h
    Created: 8 Aug 2024 12:50:17am
    Author:  blake $

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct Placeholder : juce::Component
{
    Placeholder();

    void paint(juce::Graphics& g) override;

    juce::Colour customColor;
};

struct RotarySlider : juce::Slider
{
    RotarySlider();
};