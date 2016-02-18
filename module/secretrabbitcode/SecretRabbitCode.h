#ifndef SECRETRABBITCODE_MODULE_HEADER_H
#define SECRETRABBITCODE_MODULE_HEADER_H

//=============================================================================
#include <modules/juce_audio_utils/juce_audio_utils.h>
#include <modules/juce_gui_basics/juce_gui_basics.h>
#include <modules/juce_cryptography/juce_cryptography.h>

//=============================================================================
namespace SecretRabbitCode
{
    using namespace juce;
    using juce::MemoryBlock;

    #include "core/LinearResampler.h"
    #include "core/Resampler.h"
    #include "core/SincResamplers.h"
    #include "core/ZeroOrderHoldResampler.h"
}

#endif //SECRETRABBITCODE_MODULE_HEADER_H
