#ifndef SRC_LINEAR_RESAMPLER_H
#define SRC_LINEAR_RESAMPLER_H

#include "Resampler.h"

/**
*/
class LinearResampler : public Resampler
{
public:
    /** */
    LinearResampler();

    //==============================================================================
    /** @internal */
    void updateRatio() override;
    /** @internal */
    void performResample (double ratio, AudioBuffer<float>& buffer) override;
    /** @internal */
    void performResample (double ratio, AudioBuffer<double>& buffer) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearResampler)
};

#endif //SRC_LINEAR_RESAMPLER_H
