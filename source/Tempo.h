// Created by Benjahmin Singh-Reynolds on 9/30/25.

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class Tempo
{
public:

    void reset() noexcept;
    void update(const juce::AudioPlayHead* playHead) noexcept;
    double getMillisecondsFromNoteLength(int index) const noexcept;

    double getTempo() const noexcept
    {
        return bpm;
    }

private:

        double bpm { 120.0 };
};