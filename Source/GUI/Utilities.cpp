/*
  ==============================================================================

    Utilities.cpp
    Created: 8 Aug 2024 12:57:34am
    Author:  blake $

  ==============================================================================
*/

#include "Utilities.h"

juce::String getValString(const juce::RangedAudioParameter& param, bool getLow, juce::String suffix)
{
    juce::String str;

    auto val = getLow ? param.getNormalisableRange().start : param.getNormalisableRange().end;

    bool useK = truncateKiloValue(val);
    str << val;

    if (useK)
        str << "k";

    str << suffix;

    return str;
}

juce::Rectangle<int> drawModuleBackground(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    using namespace juce;

    g.setColour(Colours::lightgrey);
    g.fillAll();

    auto localBounds = bounds;

    bounds.reduce(3, 3);
    g.setColour(Colours::white); //WHITE RECTANGLE ON TOP OF BLUEVIOLET RECTANGLE ( CHANGE LATER )
    g.fillRoundedRectangle(bounds.toFloat(), 3);

    g.drawRect(localBounds);

    return bounds;
}