#ifndef SRC_RESAMPLER_H
#define SRC_RESAMPLER_H

/**
*/
class Resampler
{
public:
    /** */
    Resampler();

    /** */
    virtual ~Resampler();

    //==============================================================================
    /** If the ratio has changed, you must update your subclass */
    virtual void updateRatio() = 0;

    /** Override this in your subclass to do your particular resample using
        32-bit floating point audio samples.
    */
    virtual void performResample (double ratio, AudioBuffer<float>& buffer) = 0;

    /** Override this in your subclass to do your particular resample using
        64-bit floating point audio samples.
    */
    virtual void performResample (double ratio, AudioBuffer<double>& buffer) = 0;

    //==============================================================================
    /** Set the ratio directly */
    void setRatio (double newRatio);

    /** Set the ratio via sampling rates; the source rate, and the destination rate to correct to */
    void setRatio (double sourceRate, double destinationRate);

    /** Get the resampling ratio */
    double getRatio() const noexcept { return ratio.get(); }

    /** Get the inverse resampling ratio */
    double getInverseRatio() const noexcept { return 1.0 / getRatio(); }

private:
    //==============================================================================
    Atomic<double> ratio;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Resampler)
};

#endif //SRC_RESAMPLER_H
