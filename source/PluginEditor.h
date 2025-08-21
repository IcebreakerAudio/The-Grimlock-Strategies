#pragma once

#include "QuoteGenerator.hpp"
#include "PluginProcessor.h"
#include <BinaryData.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "ScreenDisplay.hpp"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor, public juce::Value::Listener
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void valueChanged(juce::Value& value) override;

private:

    AudioPluginAudioProcessor& processorRef;

    std::unique_ptr<juce::Drawable> background;
    ScreenDisplay display;

    struct Layout
    {
        static constexpr float REFERENCE_WIDTH = 750.0f;
        
        static constexpr float DISPLAY_X = 94.0f;
        static constexpr float DISPLAY_Y = 76.0f;
        static constexpr float DISPLAY_WIDTH = 560.0f;
        static constexpr float DISPLAY_HEIGHT = 398.0f;
    };

    void loadAndDisplayQuote();

    #if JUCE_DEBUG
        juce::TextButton reroll {"Reroll", "Reroll"};
    #endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
