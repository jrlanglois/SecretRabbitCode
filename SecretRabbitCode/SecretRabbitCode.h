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

    dependencies:   juce_audio_utils juce_cryptography

    END_JUCE_MODULE_DECLARATION
*/
//=============================================================================
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_cryptography/juce_cryptography.h>

//=============================================================================
namespace SecretRabbitCode
{
    using namespace juce;

    #include "core/Maths.h"
    #include "core/Interpolation.h"
    #include "core/Algorithms.h"
    #include "resampling/Interpolator.h"
    #include "resampling/Resampler.h"
    #include "resampling/SincInterpolator.h"
    #include "resampling/ZeroOrderHoldResampler.h"
    #include "resampling/ResamplingProcessor.h"
}

#endif //SECRETRABBITCODE_MODULE_HEADER_H
