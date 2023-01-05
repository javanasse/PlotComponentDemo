#pragma once

#include <JuceHeader.h>

#include "PlotComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component, public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    //==============================================================================
    // Your private member variables go here...
    static const int NUMPOINTS {128};
    PlotComponent<float, NUMPOINTS> plot;
    float phase {0.0f};
    float dphase {0.02f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
