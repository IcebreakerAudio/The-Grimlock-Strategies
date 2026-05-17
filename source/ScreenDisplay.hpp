#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <BinaryData.h>
#include "QuoteGenerator.hpp"
#include "utils/GinStackBlur.hpp"

class ScreenDisplay : public juce::Component
{
public:

    ScreenDisplay();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setQuoteInfo(GrimlockQuote& quoteInfo);
    void setSizeRatio(float newSizeRatio);

private:

    float sizeRatio = 1.0f;

    //========================================================

    class DisplayLayer : public juce::Component
    {
    public:

        DisplayLayer();

        void paint(juce::Graphics& g) override;
        void resized() override;

        void setQuoteInfo(const GrimlockQuote& quoteInfo);
        void setSizeRatio(float newSizeRatio);

    private:

        float sizeRatio = 1.0f;

        juce::HyperlinkButton linkButton;
        juce::Label title, quote, epCode, epName;

        juce::ImageComponent screenshot;
    };

    DisplayLayer displayLayer;

    //========================================================
    
    class BloomLayer : public juce::Component
    {
    public:

        BloomLayer(DisplayLayer& d);
        
        void paint(juce::Graphics& g) override;
        void updateBloom();
        void setBloomSettings(bool enabled, float intensity, int radius);
        void setChromaticAberration(bool enabled, int offset);
        
    private:

        DisplayLayer& display;
        juce::Image cachedBloomImage;
        bool needsBloomUpdate = true;
        bool bloomEnabled = true;
        float bloomIntensity = 0.33f;
        int blurRadius = 4;
        
        // Chromatic aberration settings
        bool chromaticAberrationEnabled = true;
        int aberrationOffset = 3;
        
        void applyChromaticAberration(juce::Image& source, int offset);
    };
    
    BloomLayer bloomLayer;

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
