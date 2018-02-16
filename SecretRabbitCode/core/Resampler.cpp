Resampler::Resampler() noexcept { }
Resampler::~Resampler() noexcept { }

void Resampler::setRatio (double newRatio)
{
    jassert (newRatio > 0.0);

    if (ratio != newRatio && newRatio > 0.0)
    {
        ratio = newRatio;
        updateRatio (ratio);
    }
}

void Resampler::setRatio (double sourceRate, double destinationRate)
{
    if (sourceRate > 0.0 && destinationRate > 0.0)
        setRatio (sourceRate / destinationRate);
}

//==============================================================================
namespace
{
    template<typename Type>
    static inline void prepareResamplers (OwnedArray<Type>& resamplers, int numChannels)
    {
        resamplers.ensureStorageAllocated (numChannels);

        for (int i = 0; i < numChannels; ++i)
        {
            if (auto* r = resamplers[i])
                r->reset();
            else
                resamplers.add (new Type());
        }
    }

    template<typename Type>
    static inline void processResamplers (OwnedArray<Type>& resamplers, AudioBuffer<float>& source, AudioBuffer<float>& dest, double r)
    {
        const int numOutSamples = dest.getNumSamples();

        for (int i = jmin (source.getNumChannels(), dest.getNumChannels(), resamplers.size()); --i >= 0;)
            resamplers.getUnchecked (i)->process (r, source.getReadPointer (i), dest.getWritePointer (i), numOutSamples);
    }
}

CatmullRomResampler::CatmullRomResampler() noexcept                                                 { }
CatmullRomResampler::~CatmullRomResampler() noexcept                                                { }
void CatmullRomResampler::prepare (int numChannels, int, double)                                    { prepareResamplers (resamplers, numChannels); }
void CatmullRomResampler::process (AudioBuffer<float>& source, AudioBuffer<float>& dest, double r)  { processResamplers (resamplers, source, dest, r); }

LagrangeResampler::LagrangeResampler() noexcept                                                     { }
LagrangeResampler::~LagrangeResampler() noexcept                                                    { }
void LagrangeResampler::prepare (int numChannels, int, double)                                      { prepareResamplers (resamplers, numChannels); }
void LagrangeResampler::process (AudioBuffer<float>& source, AudioBuffer<float>& dest, double r)    { processResamplers (resamplers, source, dest, r); }

LinearResampler::LinearResampler() noexcept                                                         { }
LinearResampler::~LinearResampler() noexcept                                                        { }
void LinearResampler::prepare (int numChannels, int, double)                                        { prepareResamplers (resamplers, numChannels); }
void LinearResampler::process (AudioBuffer<float>& source, AudioBuffer<float>& dest, double r)      { processResamplers (resamplers, source, dest, r); }

