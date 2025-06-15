#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    processorRef.quoteValue.addListener(this);
    auto quoteIndex = processorRef.getQuoteIndex();

    // set default font
    auto& lnf = getLookAndFeel();
    lnf.setDefaultSansSerifTypeface(juce::Typeface::createSystemTypefaceFor(BinaryData::PressStart2PRegular_ttf, BinaryData::PressStart2PRegular_ttfSize));
    lnf.setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xFFD9FEFA));

    // load background svg
    background = juce::Drawable::createFromImageData(BinaryData::Background_svg, BinaryData::Background_svgSize);

    auto grimlock = getGrimlockData();
    QuoteGenerator quoteGen;
    GrimlockQuote quoteInfo;
    if(quoteIndex == -1) {
        quoteInfo = quoteGen.generateRandomQuote(grimlock);
        processorRef.setQuoteIndex(quoteGen.getQuoteIndex());
    }
    else {
        quoteInfo = quoteGen.getQuoteFromIndex(grimlock, quoteIndex);
    }

    quoteInfo.printDebugInfo();

    display.setQuoteInfo(quoteInfo);
    addAndMakeVisible(display);

    // this button is for debugging mostly
    /*
    addAndMakeVisible(reroll);
    reroll.onClick = [&]()
    {
        auto grimlock = getGrimlockData();
        QuoteGenerator quoteGen;
        auto quoteInfo = quoteGen.generateRandomQuote(grimlock);

        quoteInfo.printDebugInfo();
        display.setQuoteInfo(quoteInfo);
        processorRef.setQuoteIndex(quoteGen.getQuoteIndex());
    };
    */

    setResizable(true, true);
    auto windowConstrainer = getConstrainer();
    windowConstrainer->setFixedAspectRatio(4.0 / 3.0);
    setSize (750, 562);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    processorRef.quoteValue.removeListener(this);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    background->drawWithin(g, getLocalBounds().toFloat(), juce::RectanglePlacement::fillDestination, 1.0f);
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto sizeRatio = bounds.toFloat().getWidth() / 750.0f;

    display.setSizeRatio(sizeRatio);

    display.setBounds(
        juce::roundToInt(94.0f * sizeRatio),
        juce::roundToInt(76.0f * sizeRatio),
        juce::roundToInt(560.0f * sizeRatio),
        juce::roundToInt(398.0f * sizeRatio)
    );

    reroll.setBounds(0,0,150,25);
}

juce::var AudioPluginAudioProcessorEditor::getGrimlockData()
{
    auto string = juce::String::createStringFromData(BinaryData::data_json, BinaryData::data_jsonSize);
    auto json = juce::JSON::parse(string);
    return json.getProperty("grimlock", "");
}

void AudioPluginAudioProcessorEditor::valueChanged(juce::Value& value)
{
    juce::ignoreUnused(value);
    auto quoteIndex = int(processorRef.quoteValue.getValue());
    auto grimlock = getGrimlockData();
    QuoteGenerator quoteGen;
    GrimlockQuote quoteInfo;
    if(quoteIndex == -1) {
        quoteInfo = quoteGen.generateRandomQuote(grimlock);
        processorRef.setQuoteIndex(quoteGen.getQuoteIndex());
    }
    else {
        quoteInfo = quoteGen.getQuoteFromIndex(grimlock, quoteIndex);
    }
    quoteInfo.printDebugInfo();
    display.setQuoteInfo(quoteInfo);
}