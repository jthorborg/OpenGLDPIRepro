/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


class InnerWindow : public juce::Component, public juce::OpenGLRenderer
{
public:

    InnerWindow()
    {
        setOpaque(true);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::red);
    }

    // Inherited via OpenGLRenderer
    void newOpenGLContextCreated() override { }
    void openGLContextClosing() override {}
    void renderOpenGL() override
    {
        using namespace juce::gl;
        juce::OpenGLHelpers::clear(juce::Colours::black);

        glBegin(GL_LINES);

        glColor3f(0, 1, 0);

        glVertex3f(-1, -1, 0);
        glVertex3f(1, 1, 0);

        glVertex3f(-1, 1, 0);
        glVertex3f(1, -1, 0);

        glEnd();
    }
};

//==============================================================================
OpenGLDPIReproAudioProcessorEditor::OpenGLDPIReproAudioProcessorEditor (OpenGLDPIReproAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), kiosk("Swap fullscreen")
{
    setOpaque(true);
    glWindow = std::make_unique<InnerWindow>();

    addAndMakeVisible(kiosk);
    addAndMakeVisible(glWindow.get());
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    context.setComponentPaintingEnabled(false);
    context.setContinuousRepainting(true);
    context.attachTo(*glWindow);
    context.setRenderer(glWindow.get());
    kiosk.onClick = [this] { swapFullscreen(); };
}

OpenGLDPIReproAudioProcessorEditor::~OpenGLDPIReproAudioProcessorEditor()
{
    context.detach();
}

void OpenGLDPIReproAudioProcessorEditor::swapFullscreen()
{
    if (!isKiosk)
    {
        glWindow->addKeyListener(this);
        removeChildComponent(glWindow.get());
#if JUCE_WINDOWS
        const juce::ScopedThreadDPIAwarenessSetter scope{ getWindowHandle() };
#endif
        glWindow->addToDesktop(juce::ComponentPeer::StyleFlags::windowAppearsOnTaskbar);
        juce::Desktop::getInstance().setKioskModeComponent(glWindow.get());
        glWindow->setWantsKeyboardFocus(true);
        glWindow->toFront(true);
    }
    else
    {
        juce::Desktop::getInstance().setKioskModeComponent(nullptr);
        glWindow->removeFromDesktop();
        addChildComponent(glWindow.get());
        glWindow->removeKeyListener(this);
    }

    isKiosk = !isKiosk;
}

bool OpenGLDPIReproAudioProcessorEditor::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    if (key == juce::KeyPress::escapeKey)
    {
        swapFullscreen();
        return true;
    }

    return false;
}

//==============================================================================
void OpenGLDPIReproAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    g.setColour(juce::Colours::black);


    g.drawLine(0, 0, getWidth(), getHeight());
    g.drawLine(0, getHeight(), getWidth(), 0);
}

void OpenGLDPIReproAudioProcessorEditor::resized()
{
    kiosk.setBounds(10, 10, 100, 40);

    if (!isKiosk)
        glWindow->setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
