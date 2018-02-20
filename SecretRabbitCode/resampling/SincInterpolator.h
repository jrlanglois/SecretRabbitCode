#ifndef SRC_SINC_RESAMPLERS_H
#define SRC_SINC_RESAMPLERS_H

/** Interpolator for resampling a stream of floats using linear interpolation.

    Note that the resampler is stateful, so when there's a break in the continuity
    of the input stream you're feeding it, you should call reset() before feeding
    it any new data. And like with any other stateful filter, if you're resampling
    multiple channels, make sure each one uses its own LinearInterpolator object.

    @see LagrangeInterpolator, CatmullRomInterpolator
*/
class SincInterpolator
{
public:
    /** Constructor. */
    SincInterpolator() noexcept;

    /** Destructor. */
    ~SincInterpolator() noexcept;

    /** Resets the state of the interpolator.
        Call this when there's a break in the continuity of the input data stream.
    */
    void reset() noexcept;

    /** Resamples a stream of samples.

        @param speedRatio                   The number of input samples to use for each output sample.
        @param inputSamples                 The source data to read from. This must contain at
                                            least (speedRatio * numOutputSamplesToProduce) samples.
        @param outputSamples                The buffer to write the results into.
        @param numOutputSamplesToProduce    The number of output samples that should be created.

        @returns the actual number of input samples that were used
    */
    int process (double speedRatio,
                 const float* inputSamples,
                 float* outputSamples,
                 int numOutputSamplesToProduce) noexcept;

    /** Resamples a stream of samples.

        @param speedRatio                   The number of input samples to use for each output sample.
        @param inputSamples                 The source data to read from. This must contain at
                                            least (speedRatio * numOutputSamplesToProduce) samples.
        @param outputSamples                The buffer to write the results into
        @param numOutputSamplesToProduce    The number of output samples that should be created
        @param available                    The number of available input samples. If it needs more samples
                                            than available, it either wraps back for wrapAround samples, or it feeds zeroes.
        @param wrapAround                   If the stream exceeds available samples, it wraps back for wrapAround samples.
                                            If wrapAround is set to 0, it will feed zeroes.

        @returns the actual number of input samples that were used.
    */
    int process (double speedRatio,
                 const float* inputSamples,
                 float* outputSamples,
                 int numOutputSamplesToProduce,
                 int available,
                 int wrapAround) noexcept;

    /** Resamples a stream of samples, adding the results to the output data with a gain.

        @param speedRatio                   The number of input samples to use for each output sample.
        @param inputSamples                 The source data to read from. This must contain at
                                            least (speedRatio * numOutputSamplesToProduce) samples.
        @param outputSamples                The buffer to write the results to - the result values will be added
                                            to any pre-existing data in this buffer after being multiplied by
                                            the gain factor.
        @param numOutputSamplesToProduce    The number of output samples that should be created.
        @param gain                         A gain factor to multiply the resulting samples by before
                                            adding them to the destination buffer.

        @returns the actual number of input samples that were used.
    */
    int processAdding (double speedRatio,
                       const float* inputSamples,
                       float* outputSamples,
                       int numOutputSamplesToProduce,
                       float gain) noexcept;

    /** Resamples a stream of samples, adding the results to the output data with a gain.

        @param speedRatio       The number of input samples to use for each output sample
        @param inputSamples     The source data to read from. This must contain at
                                least (speedRatio * numOutputSamplesToProduce) samples.
        @param outputSamples    The buffer to write the results to - the result values will be added
                                to any pre-existing data in this buffer after being multiplied by
                                the gain factor
        @param numOutputSamplesToProduce    The number of output samples that should be created
        @param available        The number of available input samples. If it needs more samples
                                than available, it either wraps back for wrapAround samples, or it feeds zeroes
        @param wrapAround       If the stream exceeds available samples, it wraps back for wrapAround samples.
                                If wrapAround is set to 0, it will feed zeroes.
        @param gain             A gain factor to multiply the resulting samples by before
                                adding them to the destination buffer.

        @returns the actual number of input samples that were used.
    */
    int processAdding (double speedRatio,
                       const float* inputSamples,
                       float* outputSamples,
                       int numOutputSamplesToProduce,
                       int available,
                       int wrapAround,
                       float gain) noexcept;

private:
    float lastInputSamples[numStokeSamples];
    double subSamplePos;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SincInterpolator)
};

#endif //SRC_SINC_RESAMPLERS_H
