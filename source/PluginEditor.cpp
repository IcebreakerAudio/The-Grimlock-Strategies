#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <BinaryData.h>

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    auto string = juce::String::createStringFromData(BinaryData::data_json, BinaryData::data_jsonSize);
    auto json = juce::JSON::parse(string);

    if(!json.isObject())
    {
        DBG("Error loading JSON.");
    }

    auto level1 = json.getProperty("grimlock", "");
    if(level1.toString().isEmpty())
    {
        DBG("Grimlock not found.");
    }

    if(level1.isArray())
    {
        DBG("Grimlock is array.");
        auto level2 = level1.getArray();
        DBG("He this big:");
        DBG(level2->size());
        int total = 0;
        for(auto& e : *level2)
        {
            DBG("Episode:");
            DBG(e.getProperty("episode", "episode not found").toString());
            DBG(e.getProperty("title", "title not found").toString());
            if(e.hasProperty("quotes"))
            {
                total += e.getProperty("quotes",{}).getArray()->size();
            }
        }
        DBG("Total Quotes");
        DBG(total);
    }

    setSize (400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
