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

    #include "core/LinearSampleRateConverter.h"
    #include "core/SampleRateConverter.h"
    #include "core/SincSampleRateConverters.h"
    #include "core/ZeroOrderHoldSampleRateConverter.h"
}

#endif //SECRETRABBITCODE_MODULE_HEADER_H
