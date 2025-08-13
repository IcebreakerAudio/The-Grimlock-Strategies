#include "ScreenDisplay.hpp"

ScreenDisplay::ScreenDisplay()
{
    title.setText("The Grimlock Strategies", juce::dontSendNotification);
    linkButton.setButtonText("Watch >>");

    title.setJustificationType(juce::Justification::centred);
    quote.setJustificationType(juce::Justification::centred);
    epCode.setJustificationType(juce::Justification::centred);
    epName.setJustificationType(juce::Justification::centred);

    quote.setMinimumHorizontalScale(0.95f);
    quote.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour(0xC014191D));

    addAndMakeVisible(screenshot);

    addAndMakeVisible(linkButton);
    addAndMakeVisible(title);
    addAndMakeVisible(quote);
    addAndMakeVisible(epCode);
    addAndMakeVisible(epName);

    addAndMakeVisible(scanLines);
}

void ScreenDisplay::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void ScreenDisplay::resized()
{
    auto bounds = getLocalBounds();
    const auto kerning = 0.1f;

    scanLines.setBounds(bounds);

    title.setBounds(bounds.removeFromTop(juce::roundToInt(37.0f * sizeRatio)));
    title.setFont(juce::Font(juce::FontOptions().withHeight(37.0f * sizeRatio).withKerningFactor(kerning)));

    auto width = bounds.getWidth();

    auto infoBounds = bounds.removeFromTop(juce::roundToInt(21.0f * sizeRatio));
    infoBounds.reduce(width / 25, 0);
    epCode.setBounds(infoBounds.removeFromLeft(width / 5));
    linkButton.setBounds(infoBounds.removeFromRight(width / 5));
    epName.setBounds(infoBounds.reduced(juce::roundToInt(4.0f * sizeRatio),0));

    auto font = juce::Font(juce::FontOptions().withHeight(15.0f * sizeRatio).withKerningFactor(kerning));
    epCode.setFont(font);
    linkButton.setFont(font, false);
    epName.setFont(font);

    auto screenshotBounds = bounds.withSizeKeepingCentre(juce::roundToInt(391.0f * sizeRatio), juce::roundToInt(299.0f * sizeRatio));
    screenshotBounds.setY(screenshotBounds.getY() - juce::roundToInt(12.0f * sizeRatio));
    screenshot.setBounds(screenshotBounds);

    auto quoteFont = juce::Font{juce::FontOptions().withHeight(25.0f * sizeRatio).withKerningFactor(kerning)};
    auto quoteText = juce::AttributedString(quote.getText());
    quoteText.setFont(quoteFont);
    juce::TextLayout layout;
    layout.createLayout(quoteText, 401.0f * sizeRatio);

    auto quoteBounds = juce::Rectangle<int>{juce::roundToInt(layout.getWidth() + (sizeRatio * 12.0f)), juce::roundToInt(layout.getHeight() + (sizeRatio * 10.0f))};
    quoteBounds.setY(bounds.getBottom() - quoteBounds.getHeight());
    quoteBounds.setX((bounds.getWidth() - quoteBounds.getWidth()) / 2);

    quote.setFont(quoteFont);
    quote.setBounds(quoteBounds);
}

void ScreenDisplay::setQuoteInfo(GrimlockQuote& quoteInfo)
{
    quote.setText(quoteInfo.quote, juce::dontSendNotification);
    epCode.setText(juce::String("Episode:") + quoteInfo.episodeCode, juce::dontSendNotification);
    epName.setText(juce::String("\'") + quoteInfo.episodeName + juce::String("\'"), juce::dontSendNotification);
    linkButton.setURL(juce::URL(quoteInfo.link));

    int dataSize = 0;
    auto* data = BinaryData::getNamedResource(quoteInfo.imageCode.toRawUTF8(), dataSize);
    screenshot.setImage(juce::ImageFileFormat::loadFrom(data, dataSize));

    #if JUCE_DEBUG
        resized();
    #endif
}

void ScreenDisplay::setSizeRatio(float newSizeRatio)
{
    sizeRatio = newSizeRatio;
    scanLines.setLineThickness(sizeRatio * 2.0f);
}

//========================================================

ScreenDisplay::ScanLines::ScanLines()
{
    setInterceptsMouseClicks(false, false);
}

void ScreenDisplay::ScanLines::paint(juce::Graphics& g)
{
    g.setColour(lineColour);

    for(float i = 0.0f; i < numLines; i += 1.0f)
    {
        auto posY = drawingBounds.getHeight() * i / numLines;

        g.fillRect(0.0f, posY, drawingBounds.getWidth(), thickness);
    }
}

void ScreenDisplay::ScanLines::resized()
{
    drawingBounds = getLocalBounds().toFloat();
    numLines = std::floor(drawingBounds.getHeight() / (thickness * 2.0f));
}

void ScreenDisplay::ScanLines::setLineThickness(float newThickness)
{
    thickness = newThickness;
}
