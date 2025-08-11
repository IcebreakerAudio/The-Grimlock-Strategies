#include "ScreenDisplay.hpp"

ScreenDisplay::ScreenDisplay()
{
    title.setText("The Grimlock Strategies", juce::dontSendNotification);
    linkButton.setButtonText("Watch >>");

    addAndMakeVisible(linkButton);
    addAndMakeVisible(title);
    addAndMakeVisible(quote);
    addAndMakeVisible(epCode);
    addAndMakeVisible(epName);

    title.setJustificationType(juce::Justification::centred);
    quote.setJustificationType(juce::Justification::centred);
    epCode.setJustificationType(juce::Justification::centred);
    epName.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(screenshot);
}

void ScreenDisplay::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void ScreenDisplay::resized()
{
    auto bounds = getLocalBounds();
    const auto kerning = 0.1f;

    title.setBounds(bounds.removeFromTop(juce::roundToInt(60.0f * sizeRatio)));
    title.setFont(juce::Font(juce::FontOptions().withHeight(37.0f * sizeRatio).withKerningFactor(kerning)));

    bounds.removeFromTop(juce::roundToInt(13.0f * sizeRatio));

    auto middleBounds = bounds.removeFromTop(juce::roundToInt(243.0f * sizeRatio));
    screenshot.setBounds(middleBounds.removeFromLeft(juce::roundToInt(324.0f * sizeRatio)));

    quote.setBounds(middleBounds.removeFromRight(juce::roundToInt(230.0f * sizeRatio)));
    quote.setFont(juce::Font(juce::FontOptions().withHeight(25.0f * sizeRatio).withKerningFactor(kerning)));

    bounds = bounds.removeFromBottom(juce::roundToInt(41.0f * sizeRatio));
    auto width = bounds.getWidth();

    epCode.setBounds(bounds.removeFromLeft(width / 5));
    linkButton.setBounds(bounds.removeFromRight(width / 5));
    epName.setBounds(bounds.reduced(juce::roundToInt(4.0f * sizeRatio),0));

    auto font = juce::Font(juce::FontOptions().withHeight(19.0f * sizeRatio).withKerningFactor(kerning));
    epCode.setFont(font);
    linkButton.setFont(font, false);
    epName.setFont(font);
}

void ScreenDisplay::setQuoteInfo(GrimlockQuote& quoteInfo)
{
    quote.setText(quoteInfo.quote, juce::dontSendNotification);
    epCode.setText(juce::String("Episode:") + juce::newLine + quoteInfo.episodeCode, juce::dontSendNotification);
    epName.setText(juce::String("\'") + quoteInfo.episodeName + juce::String("\'"), juce::dontSendNotification);
    linkButton.setURL(juce::URL(quoteInfo.link));

    int dataSize = 0;
    auto* data = BinaryData::getNamedResource(quoteInfo.imageCode.toRawUTF8(), dataSize);
    screenshot.setImage(juce::ImageFileFormat::loadFrom(data, dataSize));
}

void ScreenDisplay::setSizeRatio(float newSizeRatio)
{
    sizeRatio = newSizeRatio;
}
