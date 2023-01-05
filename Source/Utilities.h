/*
  ==============================================================================

    Utilities.h
    Created: 4 Jan 2023 11:44:05pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#pragma once

// Linearly scale a value from one range to another range.
template <typename T>
T scale(T x, T oldMin, T oldMax, T newMin, T newMax)
{
    return (((x - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;
}

template <typename T>
T scale(T x, std::pair<T, T> oldRange, std::pair<T, T> newRange)
{
    return scale(x, oldRange.first, oldRange.second, newRange.first, newRange.second);
}

