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
    void updateRatio() override;
    /** @internal */
    void performResample (double ratio, AudioBuffer<float>& buffer) override;
    /** @internal */
    void performResample (double ratio, AudioBuffer<double>& buffer) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZeroOrderHoldResampler)
};

#endif //SRC_ZERO_ORDER_HOLD_RESAMPLER_H
