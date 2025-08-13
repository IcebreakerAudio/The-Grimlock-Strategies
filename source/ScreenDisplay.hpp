#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "QuoteGenerator.hpp"
#include <BinaryData.h>

class ScreenDisplay : public juce::Component
{
public:

    ScreenDisplay();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setQuoteInfo(GrimlockQuote& quoteInfo);
    void setSizeRatio(float newSizeRatio);

private:

    juce::HyperlinkButton linkButton;
    juce::Label title, quote, epCode, epName;

    juce::ImageComponent screenshot;

    float sizeRatio = 1.0f;

    //========================================================

    class ScanLines : public juce::Component
    {
    public:

        ScanLines();

        void paint(juce::Graphics& g) override;
        void resized() override;

        void setLineThickness(float newThickness);

    private:

        float thickness = 1.5f;
        float numLines = 0.0f;
        juce::Rectangle<float> drawingBounds;

        const juce::Colour lineColour { 0x2F14191D };

    };

    ScanLines scanLines;

};
