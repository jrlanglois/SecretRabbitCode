#ifndef SRC_RESAMPLER_H
#define SRC_RESAMPLER_H

/** Derive from this class to design a resampling system of your choosing! */
class Resampler
{
public:
    /** Constructor. */
    Resampler() noexcept;

    /** Destructor. */
    virtual ~Resampler() noexcept;

    /** @returns some kind of Identifier representing this resampler. */
    virtual Identifier getId() const noexcept = 0;

    /** @returns the name of this type of resampler.

        If subclasses so choose to not override this method,
        it will just attempt translating the identifier provided by getId().

        @see getId
    */
    virtual String getName() const noexcept { return TRANS (getId().toString()); }

    /** @returns the description of this type of resampler.

        If subclasses choose to not override this method,
        it will just return an empty string.
    */
    virtual String getDescription() const noexcept { return { }; }

    /** */
    virtual void prepare (int numChannels, int numSamples, double sampleRate) = 0;

    /** */
    virtual void process (double ratio, AudioBuffer<float>& source, AudioBuffer<float>& dest) = 0;

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
template<typename AlgorithmToUse>
class AlgorithmicResampler : public Resampler
{
public:
    /** Constructor. */
    AlgorithmicResampler() noexcept { }

    /** Destructor. */
    ~AlgorithmicResampler() noexcept { }

    /** @internal */
    void prepare (int numChannels, int, double) override
    {
        resamplers.ensureStorageAllocated (numChannels);

        for (int i = 0; i < numChannels; ++i)
        {
            if (auto* r = resamplers[i])
                r->reset();
            else
                resamplers.add (new AlgorithmToUse());
        }
    }

    /** @internal */
    void process (double r, AudioBuffer<float>& source, AudioBuffer<float>& dest) override
    {
        const auto numOutSamples = dest.getNumSamples();

        for (int i = jmin (source.getNumChannels(), dest.getNumChannels(), resamplers.size()); --i >= 0;)
            resamplers.getUnchecked (i)->process (r, source.getReadPointer (i), dest.getWritePointer (i), numOutSamples);
    }

private:
    OwnedArray<AlgorithmToUse> resamplers;

    JUCE_DECLARE_NON_COPYABLE (AlgorithmicResampler)
};

//==============================================================================
/** */
class CatmullRomResampler : public AlgorithmicResampler<CatmullRomInterpolator>
{
public:
    /** Constructor. */
    CatmullRomResampler() noexcept { }

    /** Destructor. */
    ~CatmullRomResampler() noexcept { }

    /** @internal */
    Identifier getId() const noexcept override { return "Catmull-Rom"; }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CatmullRomResampler)
};

//==============================================================================
/** */
class LagrangeResampler : public AlgorithmicResampler<LagrangeInterpolator>
{
public:
    /** Constructor. */
    LagrangeResampler() noexcept { }

    /** Destructor. */
    ~LagrangeResampler() noexcept { }

    /** @internal */
    Identifier getId() const noexcept override { return "Lagrange"; }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LagrangeResampler)
};

//==============================================================================
/** */
template<typename AlgorithmFunctor>
class InterpolatorResampler : public AlgorithmicResampler<Interpolator<AlgorithmFunctor>>
{
public:
    /** Constructor. */
    InterpolatorResampler() noexcept { }

    /** Destructor. */
    ~InterpolatorResampler() noexcept { }

private:
    JUCE_DECLARE_NON_COPYABLE (InterpolatorResampler)
};

//==============================================================================
/** */
class LinearResampler : public InterpolatorResampler<LinearInterpolatorAlgorithm>
{
public:
    /** Constructor. */
    LinearResampler() noexcept { }

    /** Destructor. */
    ~LinearResampler() noexcept { }

    /** @internal */
    Identifier getId() const noexcept override { return NEEDS_TRANS ("Linear"); }
    /** @internal */
    String getDescription() const noexcept override { return NEEDS_TRANS ("Linear, 2-point, interpolation."); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearResampler)
};

//==============================================================================
/** */
class CubicHermiteResampler : public InterpolatorResampler<CubicHermiteInterpolatorAlgorithm>
{
public:
    /** Constructor. */
    CubicHermiteResampler() noexcept { }

    /** Destructor. */
    ~CubicHermiteResampler() noexcept { }

    /** @internal */
    Identifier getId() const noexcept override { return NEEDS_TRANS ("Cubic-Hermite"); }
    /** @internal */
    String getDescription() const noexcept override { return NEEDS_TRANS ("4-point Cubic Hermite interpolation."); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CubicHermiteResampler)
};

#endif //SRC_RESAMPLER_H
