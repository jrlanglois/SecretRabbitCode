#ifndef SECRETRABBITCODE_MODULE_HEADER_H
#define SECRETRABBITCODE_MODULE_HEADER_H

/** BEGIN_JUCE_MODULE_DECLARATION

    ID:             SecretRabbitCode
    vendor:         jrlanglois
    version:        1.0.0
    name:           SecretRabbitCode
    description:    A library for performing sample rate conversion of audio data.
    website:        https://github.com/jrlanglois/SecretRabbitCode
    license:        BSD

    dependencies:   juce_audio_utils juce_gui_basics juce_cryptography

    END_JUCE_MODULE_DECLARATION
*/
//=============================================================================
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_cryptography/juce_cryptography.h>

//=============================================================================
namespace SecretRabbitCode
{
    using namespace juce;

    constexpr double minRatioDiff = 1e-20;

    inline double fmodOne (double x)
    {
        const auto r = x - std::rint (x);
        if (r < 0.0)
            return r + 1.0;

        return r;
    }

    #include "core/LinearInterpolator.h"
    #include "core/Resampler.h"
    #include "core/SincResamplers.h"
    #include "core/ZeroOrderHoldResampler.h"
}

#endif //SECRETRABBITCODE_MODULE_HEADER_H
