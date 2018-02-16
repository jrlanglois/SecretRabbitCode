#ifndef SRC_ZERO_ORDER_HOLD_RESAMPLER_H
#define SRC_ZERO_ORDER_HOLD_RESAMPLER_H

/**
*/
class ZeroOrderHoldResampler : public Resampler
{
public:
    /** */
    ZeroOrderHoldResampler();

    //==============================================================================
    /** @internal */
    void prepare (int numChannels, int numSamples, double sampleRate) override;
    /** @internal */
    void process (AudioBuffer<float>& source, AudioBuffer<float>& dest, double ratio) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZeroOrderHoldResampler)
};

#endif //SRC_ZERO_ORDER_HOLD_RESAMPLER_H
