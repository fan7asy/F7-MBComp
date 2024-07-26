/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
struct Placeholder : juce::Component 
{
    Placeholder();

    void paint(juce::Graphics& g) override
    {
        g.fillAll(customColor);
    }

    juce::Colour customColor;
};

struct GlobalControls : juce::Component
{
    void paint(juce::Graphics& g) override;
};
/**
*/
class F7MBCompAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    F7MBCompAudioProcessorEditor (F7MBCompAudioProcessor&);
    ~F7MBCompAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    F7MBCompAudioProcessor& audioProcessor;

    Placeholder controlBar, analyzer, /*globalControls,*/ bandControls;
    GlobalControls globalControls;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (F7MBCompAudioProcessorEditor)
};
