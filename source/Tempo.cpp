// Created by Benjahmin Singh-Reynolds on 9/30/25

#include "Tempo.h"

static std::array<double, 16> noteLengthMultipliers =
{
    0.125,                  //  0 = 1/32
    0.5 / 3.0,              //  1 = 1/16T
    0.1875,                 //  2 = 1/32.
    0.25,                   //  3 = 1/16
    1.0 / 3.0,              //  4 = 1/8T
    0.375,                  //  5 = 1/16.
    0.5,                    //  6 = 1/8
    2.0 / 3.0,              //  7 = 1/4T
    0.75,                   //  8 = 1/8.
    1.0,                    //  9 = 1/4
    4.0 / 3.0,              //  10 = 1/2T
    1.5,                    //  11 = 1/4.
    2.0,                    //  12 = 1/2
    8.0 / 3.0,              //  13 = 1/1T
    3.0,                    //  14 = 1/2.
    4.0,                    //  15 = 1/1
};

void Tempo::reset() noexcept
{
    bpm = 120.00;
}

void Tempo::update(const juce::AudioPlayHead* playHead) noexcept
{
    reset();

    if (playHead == nullptr) { return; }

    const auto opt = playHead->getPosition();

    if (!opt.hasValue()) { return; }

    const auto& pos = *opt;

    if (pos.getBpm().hasValue())
    {
        bpm = *pos.getBpm();
    }
}

double Tempo::getMillisecondsFromNoteLength(int index) const noexcept
{
    return 60000.0 * noteLengthMultipliers[(size_t)index] / bpm;
}