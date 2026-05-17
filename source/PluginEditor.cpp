#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    processorRef.quoteValue.addListener(this);

    // set default font
    auto& lnf = getLookAndFeel();
    lnf.setDefaultSansSerifTypeface(juce::Typeface::createSystemTypefaceFor(BinaryData::Jersey10Regular_ttf, BinaryData::Jersey10Regular_ttfSize));
    lnf.setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xFFD9FEFA));

    // load background svg
    background = juce::Drawable::createFromImageData(BinaryData::Background_svg, BinaryData::Background_svgSize);
    addAndMakeVisible(background.get());

    loadAndDisplayQuote();
    addAndMakeVisible(display);

    // this button is for testing and debugging
    #if JUCE_DEBUG
        addAndMakeVisible(reroll);
        reroll.onClick = [this]()
        {
            auto& grimlockData = processorRef.getGrimlockData();
            QuoteGenerator quoteGen;
            auto quoteInfo = quoteGen.generateRandomQuote(grimlockData);
            if(!quoteInfo) return;

            quoteInfo->printDebugInfo();
            display.setQuoteInfo(*quoteInfo);
            processorRef.setQuoteIndex(quoteGen.getQuoteIndex());
        };
    #endif
    

    setResizable(true, true);
    auto windowConstrainer = getConstrainer();
    windowConstrainer->setFixedAspectRatio(4.0 / 3.0);
    setSize (928, 696);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    processorRef.quoteValue.removeListener(this);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& /* g */) {}

void AudioPluginAudioProcessorEditor::resized()
{
    const auto bounds = getLocalBounds();
    const float sizeRatio = bounds.toFloat().getWidth() / Layout::REFERENCE_WIDTH;

    background->setTransformToFit(bounds.toFloat(), juce::RectanglePlacement::fillDestination);
    display.setSizeRatio(sizeRatio);

    const auto displayBounds = juce::Rectangle<int>{
        juce::roundToInt(Layout::DISPLAY_X * sizeRatio),
        juce::roundToInt(Layout::DISPLAY_Y * sizeRatio),
        juce::roundToInt(Layout::DISPLAY_WIDTH * sizeRatio),
        juce::roundToInt(Layout::DISPLAY_HEIGHT * sizeRatio)
    };
    
    display.setBounds(displayBounds);

    #if JUCE_DEBUG
        reroll.setBounds(0, 0, 150, 25);
    #endif
}

void AudioPluginAudioProcessorEditor::loadAndDisplayQuote()
{
    auto quoteIndex = int(processorRef.quoteValue.getValue());

    if(loadedQuoteIndex == quoteIndex) {
        return;
    }

    loadedQuoteIndex = quoteIndex;
    auto& grimlockData = processorRef.getGrimlockData();
    QuoteGenerator quoteGen;
    std::optional<GrimlockQuote> quoteInfo;
    if(quoteIndex < 0) {
        quoteInfo = quoteGen.generateRandomQuote(grimlockData);
        processorRef.setQuoteIndex(quoteGen.getQuoteIndex());
    }
    else {
        quoteInfo = quoteGen.getQuoteFromIndex(grimlockData, quoteIndex);
    }
    if(!quoteInfo) return;
    #if JUCE_DEBUG
        quoteInfo->printDebugInfo();
    #endif
    display.setQuoteInfo(*quoteInfo);
}

void AudioPluginAudioProcessorEditor::valueChanged(juce::Value& value)
{
    juce::ignoreUnused(value);
    loadAndDisplayQuote();
}