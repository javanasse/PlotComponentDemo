/*
  ==============================================================================

    PlotComponent.h
    Created: 4 Jan 2023 11:15:30pm
    Author:  Julian Vanasse
 
    Consists of a canvas with title, ticks and labels. Useful for plotting
    ordered pairs, filter curves, fft data, etc.

  ==============================================================================
*/

#pragma once

#include <cmath>

#include <JuceHeader.h>

#include "PlotCanvasComponent.h"

//==============================================================================
/*
*/
template <typename T, int NUMPOINTS = 128>
class PlotComponent  : public juce::Component
{
public:
    
    PlotComponent()
    {
        initializePlotComponent();
    }

    ~PlotComponent() override
    {
    }
    
    void initializePlotComponent()
    {
        addAndMakeVisible(canvas);
        addAndMakeVisible(titleLabel);
        titleLabel.setJustificationType(juce::Justification::centred);
        titleLabel.setFont(juce::Font(titleFontSize, juce::Font::FontStyleFlags::bold));
        
        initializeLabels(xTickLabels);
        initializeLabels(yTickLabels);
    }
    
    void setTitle(juce::String titleString)
    {
        titleLabel.setText(titleString, juce::dontSendNotification);
    }
    
    void setLimits(T xMin, T xMax, T yMin, T yMax)
    {
        xLimit = std::make_pair(xMin, xMax);
        yLimit = std::make_pair(yMin, yMax);
        
        setLabelsTextFromMinMax(xTickLabels, xMin, xMax);
        setLabelsTextFromMinMax(yTickLabels, yMax, yMin);
        
        canvas.setLimits(xMin, xMax, yMin, yMax);
    }

    void setDataXY(int index, T x, T y)
    {
        canvas.setDataXY(index, x, y);
    }
    void setDataX(int index, T x)
    {
        canvas.setDataX(index, x);
    }
    void setDataY(int index, T y)
    {
        canvas.setDataY(index, y);
    }
    
    void paint (juce::Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (juce::Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (juce::Colours::white);
        g.setFont (labelFontSize);
        
//
//        for (juce::Line<T> &line : xTicks)
//        {
////            g.drawLine(line);
//            g.drawDashedLine(line, dashLengths, 3);
//        }
    }
    
    void paintOverChildren(juce::Graphics& g) override
    {
        g.setColour (juce::Colours::white);
        
        const float dashLengths[3] = {1.0, 2.0, 3.0};
        for (juce::Line<T> &line : xTicks)
        {
            g.drawDashedLine(line, dashLengths, 3);
        }
        for (juce::Line<T> &line : yTicks)
        {
            g.drawDashedLine(line, dashLengths, 3);
        }
    }

    void resized() override
    {
        titleLabel.setBounds(0, 0, getWidth(), border);
        canvas.setBounds(border, border, getWidth() - 2*border, getHeight() - 2*border);
        for (int i = 0; i < NUMXTICKLABELS; i++)
        {
            T x = static_cast<T>(i) * static_cast<T>(canvas.getWidth()) / static_cast<T>(NUMXTICKLABELS - 1);
            T y = canvas.getY() + canvas.getHeight() + labelFontSize;
            xTickLabels[i]->setBounds(x + (tickLabelWidth / 2.0f), y, tickLabelWidth, labelFontSize);
        }
        for (int i = 0; i < NUMYTICKLABELS; i++)
        {
            T x = canvas.getX() - tickLabelWidth + labelFontSize;
            T y = canvas.getY() - (labelFontSize / 2) + static_cast<T>(i) * static_cast<T>(canvas.getHeight()) / static_cast<T>(NUMYTICKLABELS - 1);
            yTickLabels[i]->setBounds(x, y, tickLabelWidth, labelFontSize);
        }
        
        for (int i = 0; i < xTicks.size(); i++)
        {
            if (i + 1 < NUMXTICKLABELS)
            {
                juce::Rectangle<int> labelBounds = xTickLabels[i+1]->getBounds();
                xTicks[i].setStart(labelBounds.getCentreX(), canvas.getY());
                xTicks[i].setEnd(labelBounds.getCentreX(), canvas.getY() + canvas.getHeight());
            }
        }
        for (int i = 0; i < yTicks.size(); i++)
        {
            if (i + 1 < NUMYTICKLABELS)
            {
                juce::Rectangle<int> labelBounds = yTickLabels[i+1]->getBounds();
                yTicks[i].setStart(canvas.getX(), labelBounds.getCentreY());
                yTicks[i].setEnd(canvas.getX() + canvas.getWidth(), labelBounds.getCentreY());
            }
        }
        
        
    }

private:
    
    template <size_t SIZE>
    void initializeLabels(std::array<std::unique_ptr<juce::Label>, SIZE> &labels)
    {
        for (auto &label : labels)
        {
            label = std::make_unique<juce::Label>("", "");
            label->setFont(labelFontSize);
            label->setJustificationType(juce::Justification::centred);
            addAndMakeVisible(*label);
        }
    }
    
    template <size_t SIZE>
    void setLabelsTextFromMinMax(std::array<std::unique_ptr<juce::Label>, SIZE> &labels, T min, T max)
    {
        for (int i = 0; i < SIZE; i++)
        {
            T value = min + static_cast<T>(i)/(static_cast<T>(SIZE - 1))*(max - min);
            juce::String str = Num2String(value, numDecimalPlaces);
            labels[i]->setText(str, juce::dontSendNotification);
        }
    }
    
    static juce::String Num2String(T value, int numDecimalPlaces, bool showLeadingZero = true)
    {
        int intValue = (int) value;
        T decimalValue = value - intValue;
        juce::String intPartString (intValue);
        juce::String decimalPartString (decimalValue, numDecimalPlaces);
        decimalPartString = decimalPartString.fromFirstOccurrenceOf(".", false, true);
        juce::String result = "";
        if (value < 0 && std::abs(value) < 1.f)
        {
            result += "-";
        }
        result += intPartString + "." + decimalPartString;
        return result;
    }
    
    int titleFontSize {36};
    int labelFontSize {14};
    T border {labelFontSize * 5.0f};
    std::pair<T, T> xLimit;
    std::pair<T, T> yLimit;
    
    // tick labels equally spaced
    // TODO: allow for non-equally spaced tick labels
    int numDecimalPlaces {2};
    int tickLabelWidth {labelFontSize * 5};
    bool gridOn {true};
    static const int NUMXTICKLABELS {7};
    static const int NUMYTICKLABELS {8};
    std::array<std::unique_ptr<juce::Label>, NUMXTICKLABELS> xTickLabels;
    std::array<std::unique_ptr<juce::Label>, NUMYTICKLABELS> yTickLabels;
    // ticks
    std::array<juce::Line<T>, NUMXTICKLABELS-2> xTicks;
    std::array<juce::Line<T>, NUMYTICKLABELS-2> yTicks;
    
    
    juce::Label titleLabel;
    
    // the actual plot display
    PlotCanvasComponent<T, NUMPOINTS> canvas;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlotComponent)
};
