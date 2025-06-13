#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    // set default font
    auto& lnf = getLookAndFeel();
    lnf.setDefaultSansSerifTypeface(juce::Typeface::createSystemTypefaceFor(BinaryData::PressStart2PRegular_ttf, BinaryData::PressStart2PRegular_ttfSize));
    lnf.setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xFFD9FEFA));

    // load background svg
    background = juce::Drawable::createFromImageData(BinaryData::Background_svg, BinaryData::Background_svgSize);

    auto string = juce::String::createStringFromData(BinaryData::data_json, BinaryData::data_jsonSize);
    auto json = juce::JSON::parse(string);

    if(!json.isObject())
    {
        DBG("Error loading JSON.");
    }

    auto grimlock = json.getProperty("grimlock", "");
    if(grimlock.toString().isEmpty())
    {
        DBG("Grimlock not found.");
    }

    if(!grimlock.isArray())
    {
        DBG("Grimlock not array.");
    }

    QuoteGenerator quoteGen;
    auto quoteInfo = quoteGen.generateRandomQuote(grimlock);

    quoteInfo.printDebugInfo();

    display.setQuoteInfo(quoteInfo);
    addAndMakeVisible(display);

    addAndMakeVisible(reroll);
    reroll.onClick = [&]()
    {
        auto string = juce::String::createStringFromData(BinaryData::data_json, BinaryData::data_jsonSize);
        auto json = juce::JSON::parse(string);

        if(!json.isObject())
        {
            DBG("Error loading JSON.");
        }

        auto grimlock = json.getProperty("grimlock", "");
        if(grimlock.toString().isEmpty())
        {
            DBG("Grimlock not found.");
        }

        if(!grimlock.isArray())
        {
            DBG("Grimlock not array.");
        }

        QuoteGenerator quoteGen;
        auto quoteInfo = quoteGen.generateRandomQuote(grimlock);

        quoteInfo.printDebugInfo();
        display.setQuoteInfo(quoteInfo);
    };

    setResizable(true, true);
    auto windowConstrainer = getConstrainer();
    windowConstrainer->setFixedAspectRatio(4.0 / 3.0);
    setSize (750, 562);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
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
