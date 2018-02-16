#ifndef SRC_LINEAR_INTERPOLATOR_H
#define SRC_LINEAR_INTERPOLATOR_H

/** Interpolator for resampling a stream of floats using linear interpolation.

    Note that the resampler is stateful, so when there's a break in the continuity
    of the input stream you're feeding it, you should call reset() before feeding
    it any new data. And like with any other stateful filter, if you're resampling
    multiple channels, make sure each one uses its own LinearInterpolator object.

    @see LagrangeInterpolator, CatmullRomInterpolator
*/
class LinearInterpolator
{
public:
    LinearInterpolator() noexcept;
    ~LinearInterpolator() noexcept;

    /** Resets the state of the interpolator.
        Call this when there's a break in the continuity of the input data stream.
    */
    void reset() noexcept;

    /** Resamples a stream of samples.

        @param speedRatio       the number of input samples to use for each output sample
        @param inputSamples     the source data to read from. This must contain at
                                least (speedRatio * numOutputSamplesToProduce) samples.
        @param outputSamples    the buffer to write the results into
        @param numOutputSamplesToProduce    the number of output samples that should be created

        @returns the actual number of input samples that were used
    */
    int process (double speedRatio,
                 const float* inputSamples,
                 float* outputSamples,
                 int numOutputSamplesToProduce) noexcept;

    /** Resamples a stream of samples.

        @param speedRatio       the number of input samples to use for each output sample
        @param inputSamples     the source data to read from. This must contain at
                                least (speedRatio * numOutputSamplesToProduce) samples.
        @param outputSamples    the buffer to write the results into
        @param numOutputSamplesToProduce    the number of output samples that should be created
        @param available        the number of available input samples. If it needs more samples
                                than available, it either wraps back for wrapAround samples, or
                                it feeds zeroes
        @param wrapAround       if the stream exceeds available samples, it wraps back for
                                wrapAround samples. If wrapAround is set to 0, it will feed zeroes.

        @returns the actual number of input samples that were used
    */
    int process (double speedRatio,
                 const float* inputSamples,
                 float* outputSamples,
                 int numOutputSamplesToProduce,
                 int available,
                 int wrapAround) noexcept;

    /** Resamples a stream of samples, adding the results to the output data
        with a gain.

        @param speedRatio       the number of input samples to use for each output sample
        @param inputSamples     the source data to read from. This must contain at
                                least (speedRatio * numOutputSamplesToProduce) samples.
        @param outputSamples    the buffer to write the results to - the result values will be added
                                to any pre-existing data in this buffer after being multiplied by
                                the gain factor
        @param numOutputSamplesToProduce    the number of output samples that should be created
        @param gain             a gain factor to multiply the resulting samples by before
                                adding them to the destination buffer

        @returns the actual number of input samples that were used
    */
    int processAdding (double speedRatio,
                       const float* inputSamples,
                       float* outputSamples,
                       int numOutputSamplesToProduce,
                       float gain) noexcept;

    /** Resamples a stream of samples, adding the results to the output data with a gain.

        @param speedRatio       the number of input samples to use for each output sample
        @param inputSamples     the source data to read from. This must contain at
                                least (speedRatio * numOutputSamplesToProduce) samples.
        @param outputSamples    the buffer to write the results to - the result values will be added
                                to any pre-existing data in this buffer after being multiplied by
                                the gain factor
        @param numOutputSamplesToProduce    the number of output samples that should be created
        @param available        the number of available input samples. If it needs more samples
                                than available, it either wraps back for wrapAround samples, or
                                it feeds zeroes
        @param wrapAround       if the stream exceeds available samples, it wraps back for
                                wrapAround samples. If wrapAround is set to 0, it will feed zeroes.
        @param gain             a gain factor to multiply the resulting samples by before
                                adding them to the destination buffer

        @returns the actual number of input samples that were used
    */
    int processAdding (double speedRatio,
                       const float* inputSamples,
                       float* outputSamples,
                       int numOutputSamplesToProduce,
                       int available,
                       int wrapAround,
                       float gain) noexcept;

private:
    float lastInputSamples[5];
    double subSamplePos;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearInterpolator)
};

#endif //SRC_LINEAR_INTERPOLATOR_H
