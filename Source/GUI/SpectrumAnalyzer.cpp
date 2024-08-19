/*
  ==============================================================================

    SpectrumAnalyzer.cpp
    Created: 14 Aug 2024 8:34:59pm
    Author:  blake $

  ==============================================================================
*/

#include "SpectrumAnalyzer.h"
#include "Utilities.h"

SpectrumAnalyzer::SpectrumAnalyzer(F7MBCompAudioProcessor& p) :
    audioProcessor(p),
    leftPathProducer(audioProcessor.leftChannelFifo),
    rightPathProducer(audioProcessor.rightChannelFifo)
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params)
    {
        param->addListener(this);
    }

    startTimerHz(60);
}

SpectrumAnalyzer::~SpectrumAnalyzer()
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params)
    {
        param->removeListener(this);
    }
}

void SpectrumAnalyzer::drawFFTAnalysis(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    using namespace juce;

    auto responseArea = getAnalysisArea(bounds);

    Graphics::ScopedSaveState sss(g);
    g.reduceClipRegion(responseArea);

    auto leftChannelFFTPath = leftPathProducer.getPath();
    leftChannelFFTPath.applyTransform(AffineTransform().translation(responseArea.getX(), 0 /*responseArea.getY()*/));

    g.setColour(Colours::darkgrey); //left channel color
    g.strokePath(leftChannelFFTPath, PathStrokeType(1.f));

    auto rightChannelFFTPath = rightPathProducer.getPath();
    rightChannelFFTPath.applyTransform(AffineTransform().translation(responseArea.getX(), 0 /*responseArea.getY()*/));

    g.setColour(Colours::black); //right channel color
    g.strokePath(rightChannelFFTPath, PathStrokeType(1.f));
}

void SpectrumAnalyzer::paint(juce::Graphics& g)
{
    using namespace juce;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colour::Colour());

    auto bounds = drawModuleBackground(g, getLocalBounds());

    drawBackgroundGrid(g, bounds);


    if (shouldShowFFTAnalysis)
    {
        drawFFTAnalysis(g, bounds);
    }

    Path border;

    border.setUsingNonZeroWinding(false);

    border.addRoundedRectangle(getRenderArea(bounds), 4);
    border.addRectangle(getLocalBounds());

    g.setColour(Colour::Colour());

    g.fillPath(border);

    drawTextLabels(g, bounds);

    g.setColour(Colours::black);
    g.drawRoundedRectangle(getRenderArea(bounds).toFloat(), 4.f, 1.f);
}

std::vector<float> SpectrumAnalyzer::getFrequencies()
{
    return std::vector<float>
    {
            20, /*30, 40,*/ 50, 100,
            200, /*300, 400,*/ 500, 1000,
            2000, /*3000, 4000,*/ 5000, 10000,
            20000
    };
}

std::vector<float> SpectrumAnalyzer::getGains()
{
    return std::vector<float>
    {
        -24, -12, 0, 12, 24
    };
}

std::vector<float> SpectrumAnalyzer::getXs(const std::vector<float>& freqs, float left, float width)
{
    std::vector<float> xs;
    for (auto f : freqs)
    {
        auto normX = juce::mapFromLog10(f, 20.f, 20000.f);
        xs.push_back(left + width * normX);
    }

    return xs;
}

void SpectrumAnalyzer::drawBackgroundGrid(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    using namespace juce;
    auto freqs = getFrequencies();

    auto renderArea = getAnalysisArea(bounds);
    auto left = renderArea.getX();
    auto right = renderArea.getRight();
    auto top = renderArea.getY();
    auto bottom = renderArea.getBottom();
    auto width = renderArea.getWidth();

    auto xs = getXs(freqs, left, width);

    g.setColour(Colours::lightgrey);
    for (auto x : xs)
    {
        g.drawVerticalLine(x, top, bottom);
    }

    auto gain = getGains();

    for (auto gDb : gain)
    {
        auto y = jmap(gDb, -24.f, 24.f, float(bottom), float(top));

        g.setColour(gDb == 0.f ? Colours::red : Colours::lightgrey);
        g.drawHorizontalLine(y, left, right);
    }
}

void SpectrumAnalyzer::drawTextLabels(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    using namespace juce;
    g.setColour(Colours::black);
    const int fontHeight = 10;
    g.setFont(fontHeight);

    auto renderArea = getAnalysisArea(bounds);
    auto left = renderArea.getX();

    auto top = renderArea.getY();
    auto bottom = renderArea.getBottom();
    auto width = renderArea.getWidth();

    auto freqs = getFrequencies();
    auto xs = getXs(freqs, left, width);

    for (int i = 0; i < freqs.size(); ++i)
    {
        auto f = freqs[i];
        auto x = xs[i];

        bool addK = false;
        String str;
        if (f > 999.f)
        {
            addK = true;
            f /= 1000.f;
        }

        str << f;
        if (addK)
            str << "k";
        str << "Hz";

        auto textWidth = g.getCurrentFont().getStringWidth(str);

        Rectangle<int> r;

        r.setSize(textWidth, fontHeight);
        r.setCentre(x, 0);
        //r.setY(1);
        r.setY(bounds.getY());

        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }

    auto gain = getGains();

    for (auto gDb : gain)
    {
        auto y = jmap(gDb, -24.f, 24.f, float(bottom), float(top));

        String str;
        if (gDb > 0)
            str << "+";
        str << gDb;

        auto textWidth = g.getCurrentFont().getStringWidth(str);

        Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setX(bounds.getRight()-textWidth);
        r.setCentre(r.getCentreX(), y);

        g.setColour(gDb == 0.f ? Colours::red : Colours::black);

        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);

        str.clear();
        str << (gDb - 24.f);

        r.setX(bounds.getX() + 1);
        textWidth = g.getCurrentFont().getStringWidth(str);
        r.setSize(textWidth, fontHeight);
        g.setColour(Colours::black);
        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);
    }
}

void SpectrumAnalyzer::resized()
{
    using namespace juce;

    auto bounds = getLocalBounds();
    auto fftBounds = getAnalysisArea(bounds).toFloat();
    auto negInf = jmap(bounds.toFloat().getBottom(),
        fftBounds.getBottom(), fftBounds.getY(),
        -48.f, 0.f);
    DBG("Negative Infinity: " << negInf);
    leftPathProducer.updateNegativeInfinity(negInf);
    rightPathProducer.updateNegativeInfinity(negInf);
}

void SpectrumAnalyzer::parameterValueChanged(int parameterIndex, float newValue)
{
    parametersChanged.set(true);
}

void SpectrumAnalyzer::timerCallback()
{
    if (shouldShowFFTAnalysis)
    {
        auto bounds = getLocalBounds();
        auto fftBounds = getAnalysisArea(bounds).toFloat();
        fftBounds.setBottom(bounds.getBottom());
        auto sampleRate = audioProcessor.getSampleRate();

        leftPathProducer.process(fftBounds, sampleRate);
        rightPathProducer.process(fftBounds, sampleRate);
    }

    if (parametersChanged.compareAndSetBool(false, true))
    {

    }

    repaint();
}

juce::Rectangle<int> SpectrumAnalyzer::getRenderArea(juce::Rectangle<int> bounds)
{
    //auto bounds = getLocalBounds();

    bounds.removeFromTop(12);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);

    return bounds;
}

juce::Rectangle<int> SpectrumAnalyzer::getAnalysisArea(juce::Rectangle<int> bounds)
{
    bounds = getRenderArea(bounds);
    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);
    return bounds;
}