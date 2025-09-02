#include "ScreenDisplay.hpp"

ScreenDisplay::ScreenDisplay() : bloomLayer(displayLayer)
{
    addAndMakeVisible(displayLayer);
    addAndMakeVisible(bloomLayer);
    addAndMakeVisible(scanLines);
}

void ScreenDisplay::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void ScreenDisplay::resized()
{
    const auto bounds = getLocalBounds();

    displayLayer.setBounds(bounds);
    bloomLayer.setBounds(bounds);
    scanLines.setBounds(bounds);
}

void ScreenDisplay::setQuoteInfo(GrimlockQuote& quoteInfo)
{
    displayLayer.setQuoteInfo(quoteInfo);
    bloomLayer.updateBloom();
}

void ScreenDisplay::setSizeRatio(float newSizeRatio)
{
    sizeRatio = newSizeRatio;
    displayLayer.setSizeRatio(sizeRatio);
    scanLines.setLineThickness(sizeRatio * 1.5f);
}

//========================================================

ScreenDisplay::DisplayLayer::DisplayLayer()
{
    title.setText("The Grimlock Strategies", juce::dontSendNotification);
    linkButton.setButtonText("Watch >>");

    title.setJustificationType(juce::Justification::centred);
    epCode.setJustificationType(juce::Justification::centred);
    epName.setJustificationType(juce::Justification::centred);
    quote.setJustificationType(juce::Justification::centred);

    quote.setMinimumHorizontalScale(0.95f);
    quote.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour(0xC014191D));

    addAndMakeVisible(title);
    addAndMakeVisible(epCode);
    addAndMakeVisible(epName);
    addAndMakeVisible(linkButton);
    addAndMakeVisible(screenshot);
    addAndMakeVisible(quote);
}

void ScreenDisplay::DisplayLayer::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF14191D));
}

void ScreenDisplay::DisplayLayer::resized()
{
    auto bounds = getLocalBounds();
    const auto kerning = 0.1f;

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

void ScreenDisplay::DisplayLayer::setQuoteInfo(GrimlockQuote& quoteInfo)
{
    quote.setText(quoteInfo.quote, juce::dontSendNotification);
    epCode.setText(juce::String("Episode:") + quoteInfo.episodeCode, juce::dontSendNotification);
    epName.setText(juce::String("\'") + quoteInfo.episodeName + juce::String("\'"), juce::dontSendNotification);
    linkButton.setURL(juce::URL(quoteInfo.link));

    int dataSize = 0;
    auto* data = BinaryData::getNamedResource(quoteInfo.imageCode.toRawUTF8(), dataSize);
    screenshot.setImage(juce::ImageFileFormat::loadFrom(data, static_cast<size_t>(dataSize)));

    #if JUCE_DEBUG
        resized();
    #endif
}

void ScreenDisplay::DisplayLayer::setSizeRatio(float newSizeRatio)
{
    sizeRatio = newSizeRatio;
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

//========================================================

ScreenDisplay::BloomLayer::BloomLayer(DisplayLayer& d) : display(d)
{
    setInterceptsMouseClicks(false, false);
}

void ScreenDisplay::BloomLayer::paint(juce::Graphics& g)
{
    if (!bloomEnabled || bloomIntensity <= 0.0f) {
        return;
    }
    
    if (needsBloomUpdate)
    {
        auto originalImage = display.createComponentSnapshot(display.getLocalBounds());
        if (!originalImage.isNull())
        {
            cachedBloomImage = originalImage.createCopy();
            gin::applyStackBlur(cachedBloomImage, blurRadius);
            if (chromaticAberrationEnabled) {
                applyChromaticAberration(cachedBloomImage, aberrationOffset);
            }
        }
        needsBloomUpdate = false;
    }

    if (!cachedBloomImage.isNull())
    {
        g.setOpacity(bloomIntensity);
        g.drawImageWithin(cachedBloomImage, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::fillDestination);
        g.setOpacity(1.0f);
    }
}

void ScreenDisplay::BloomLayer::updateBloom()
{
    needsBloomUpdate = true;
    repaint();
}

void ScreenDisplay::BloomLayer::setBloomSettings(bool enabled, float intensity, int radius)
{
    if (bloomEnabled != enabled || !juce::approximatelyEqual(bloomIntensity, intensity) || blurRadius != radius)
    {
        bloomEnabled = enabled;
        bloomIntensity = intensity;
        blurRadius = radius;
        updateBloom();
    }
}

void ScreenDisplay::BloomLayer::setChromaticAberration(bool enabled, int offset)
{
    if (chromaticAberrationEnabled != enabled || aberrationOffset != offset)
    {
        chromaticAberrationEnabled = enabled;
        aberrationOffset = offset;
        updateBloom();
    }
}

void ScreenDisplay::BloomLayer::applyChromaticAberration(juce::Image& source, int offset)
{
    if (source.isNull() || offset == 0) {
        return;
    }

    jassert(source.getFormat() == juce::Image::PixelFormat::RGB
            || source.getFormat() == juce::Image::PixelFormat::ARGB);
    
    const int width = source.getWidth();
    const int height = source.getHeight();
    const int xLimit = width - 1;
    
    juce::Image::BitmapData bitmapData(source, 0, 0, width, height, juce::Image::BitmapData::readWrite);
    
    for (int y = 0; y < height; ++y)
    {
        juce::uint8* linePtr = bitmapData.getLinePointer(y);
        
        for (int x = 0; x < width; ++x)
        {
            int blueX = juce::jlimit(0, xLimit, x + offset);
            juce::uint8* currentPixel = linePtr + (x * bitmapData.pixelStride);
            juce::uint8* bluePixel = linePtr + (blueX * bitmapData.pixelStride);
            
            currentPixel[2] = bluePixel[2];
        }
        
        for (int x = width - 1; x >= 0; --x)
        {
            int redX = juce::jlimit(0, xLimit, x - offset);
            juce::uint8* currentPixel = linePtr + (x * bitmapData.pixelStride);
            juce::uint8* redPixel = linePtr + (redX * bitmapData.pixelStride);
            
            currentPixel[0] = redPixel[0];
        }
    }
}
