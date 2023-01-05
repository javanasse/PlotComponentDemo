#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (800, 500);
    addAndMakeVisible(plot);
    plot.setLimits(0.0f, 2.0f * M_PI, -1.0, 1.0);
    plot.setTitle("Animated Plot Demo");
    
    // start timer for animation
    startTimer(30);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    plot.setBounds(getBounds());
}

void MainComponent::timerCallback()
{
    for (int i = 0; i < NUMPOINTS; i++)
        {
            float x = (static_cast<float>(i) / static_cast<float>(128)) * M_PI * 2;
            float y = std::cos(2.0f*x + phase);
            plot.setDataXY(i, x, y);
        }
        phase += dphase;
        phase = std::fmod(phase, M_PI * 2.0f);
        repaint();
}
