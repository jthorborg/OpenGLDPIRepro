/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <memory>

class InnerWindow;

//==============================================================================
/**
*/
class OpenGLDPIReproAudioProcessorEditor 
    : public juce::AudioProcessorEditor
    , juce::KeyListener
{
public:
    OpenGLDPIReproAudioProcessorEditor (OpenGLDPIReproAudioProcessor&);
    ~OpenGLDPIReproAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void swapFullscreen();
    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OpenGLDPIReproAudioProcessor& audioProcessor;
    juce::OpenGLContext context;
    juce::TextButton kiosk;
    std::unique_ptr<InnerWindow> glWindow;
    bool isKiosk{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLDPIReproAudioProcessorEditor)
};
