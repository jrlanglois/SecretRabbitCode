#ifndef SRC_RESAMPLER_H
#define SRC_RESAMPLER_H

/** Derive from this class to design a resampling system of your choosing! */
class Resampler
{
public:
    /** Constructor */
    Resampler() noexcept;

    /** Destructor */
    virtual ~Resampler() noexcept;

    /**  */
    virtual void prepare (int numChannels, int numSamples, double sampleRate) = 0;

    /**  */
    virtual void process (AudioBuffer<float>& source, AudioBuffer<float>& dest, double ratio) = 0;

    /** Set the ratio directly */
    void setRatio (double newRatio);

    /** Set the ratio via sampling rates; the source rate, and the destination rate to correct to */
    void setRatio (double sourceRate, double destinationRate);

    /** If the ratio has changed, you might need to update your subclass. */
    virtual void updateRatio (double newRatio) { ignoreUnused (newRatio); }

    /** Get the resampling ratio */
    double getRatio() const noexcept { return ratio; }

    /** Get the inverse resampling ratio */
    double getInverseRatio() const noexcept { return 1.0 / getRatio(); }

private:
    std::atomic<double> ratio { 1.0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Resampler)
};

//==============================================================================
class CatmullRomResampler : public Resampler
{
public:
    CatmullRomResampler() noexcept;
    ~CatmullRomResampler() noexcept;

    /** @internal */
    void prepare (int numChannels, int numSamples, double sampleRate) override;
    /** @internal */
    void process (AudioBuffer<float>& source, AudioBuffer<float>& dest, double ratio) override;

private:
    OwnedArray<CatmullRomInterpolator> resamplers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CatmullRomResampler)
};

//==============================================================================
class LagrangeResampler : public Resampler
{
public:
    LagrangeResampler() noexcept;
    ~LagrangeResampler() noexcept;

    /** @internal */
    void prepare (int numChannels, int numSamples, double sampleRate) override;
    /** @internal */
    void process (AudioBuffer<float>& source, AudioBuffer<float>& dest, double ratio) override;

private:
    OwnedArray<LagrangeInterpolator> resamplers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LagrangeResampler)
};

//==============================================================================
class LinearResampler : public Resampler
{
public:
    LinearResampler() noexcept;
    ~LinearResampler() noexcept;

    /** @internal */
    void prepare (int numChannels, int numSamples, double sampleRate) override;
    /** @internal */
    void process (AudioBuffer<float>& source, AudioBuffer<float>& dest, double ratio) override;

private:
    OwnedArray<LinearInterpolator> resamplers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearResampler)
};

#endif //SRC_RESAMPLER_H
