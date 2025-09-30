// Created by Benjahmin Singh-Reynolds on 9/29/25.

// Provides a stable alias for the Lato-Medium.ttf font resource that is
// compiled into the JUCE-generated Assets binary data target.
// This avoids having to include JUCE's generated BinaryData.h directly.

namespace BinaryData
{
    // Pointer to the in-memory font bytes
    extern const char* LatoMedium_ttf;
    extern const char* Logo_png;
    extern const char* Noise_png;

    // Size in bytes of the font data
    extern const int LatoMedium_ttfSize;
    extern const int Logo_pngSize;
    extern const int Noise_pngSize;


}
