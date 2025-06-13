#pragma once

#include "QuoteGenerator.hpp"
#include "PluginProcessor.h"
#include <BinaryData.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "ScreenDisplay.hpp"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    AudioPluginAudioProcessor& processorRef;

    std::unique_ptr<juce::Drawable> background;
    ScreenDisplay display;

    juce::TextButton reroll {"Reroll", "Reroll"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
