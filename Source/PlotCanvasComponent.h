/*
  ==============================================================================

    PlotCanvasComponent.h
    Created: 4 Jan 2023 11:36:10pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Utilities.h"

//==============================================================================
/*
*/
template <typename T, int NUMPOINTS>
class PlotCanvasComponent  : public juce::Component
{
public:
    
    // line vs scatter plot, not disimilar to matplotlib
    enum StrokeType
    {
        LINE = 0,
        SQUARE,
        CIRCLE
    };
    
    PlotCanvasComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        xLimit = std::make_pair(1.0, 0.0);
        yLimit = std::make_pair(1.0, 0.0);
        
        std::fill_n(xData.begin(), NUMPOINTS, 0.0f);
        std::fill_n(yData.begin(), NUMPOINTS, 0.0f);
    }

    ~PlotCanvasComponent() override
    {
    }
    
    // set x and y limits
    void setLimits(T xMin, T xMax, T yMin, T yMax)
    {
        // NOTE: these are in reverse order of what we usually expect.
        // This is to flip the orientation so that values increase bottom to
        // top and not the other way around (as is typical in computer graphics)
        xLimit = std::make_pair(xMax, xMin);
        yLimit = std::make_pair(yMax, yMin);
    }
    
    // manipulate data on canvas
    void setDataX(int index, T x)
    {
        xData[index] = x;
    }
    void setDataY(int index, T y)
    {
        yData[index] = y;
    }
    void setDataXY(int index, T x, T y)
    {
        setDataX(index, x);
        setDataY(index, y);
    }
    
    //
    void setStroke(StrokeType newStrokeType)
    {
        strokeType = newStrokeType;
        repaint();
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

        g.setColour (strokeColour);
        
        if (strokeType == StrokeType::LINE)
        {
            updatePath();
            g.strokePath(path, juce::PathStrokeType(1.0f));
        }
        else
        {
            // x bounds as a (max, MIN) pair
            std::pair<T, T> widthBounds (getWidth(), 0);
            // y bounds as a (MIN, max) pair
            std::pair<T, T> heightBounds (0, getHeight());
            
            for (int i = 0; i < NUMPOINTS; i++)
            {
                T xScaled = scale(xData[i], xLimit, widthBounds);
                T yScaled = scale(yData[i], yLimit, heightBounds);
                
                if (strokeType == StrokeType::SQUARE)
                {
                    g.drawRect(xScaled - pointRadius/2.0f, yScaled - pointRadius/2.0f, pointRadius, pointRadius);
                }
                else if (strokeType)
                {
                    g.drawEllipse(xScaled - pointRadius/2.0f, yScaled - pointRadius/2.0f, pointRadius, pointRadius, 1.0f);
                }
            }
        }
        
    }
    
    void updatePath()
    {
        path.clear();
        
        // x bounds as a (max, MIN) pair
        std::pair<T, T> widthBounds (getWidth(), 0);
        // y bounds as a (MIN, max) pair
        std::pair<T, T> heightBounds (0, getHeight());
        
        for (int i = 0; i < NUMPOINTS; i++)
        {
            T xScaled = scale(xData[i], xLimit, widthBounds);
            T yScaled = scale(yData[i], yLimit, heightBounds);
            
            // first element, start new path
            if (i == 0)
            {
                path.startNewSubPath(xScaled, yScaled);
            }
            else // otherwise add to path
            {
                path.lineTo(xScaled, yScaled);
            }
        }
    }

    void resized() override
    {
        
    }

private:

    // Limits for scaling data to fit on canvas
    std::pair<T, T> xLimit;
    std::pair<T, T> yLimit;

    // data to be visualized
    std::array<T, NUMPOINTS> xData, yData;
    
    // plot stroke
    StrokeType strokeType {StrokeType::CIRCLE};
    
    // path to show line plot of data
    juce::Path path;
    // if its a scatter plot
    float pointRadius = 5.0f;
    
    // colours
    juce::Colour strokeColour {juce::Colours::chartreuse};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlotCanvasComponent)
};
