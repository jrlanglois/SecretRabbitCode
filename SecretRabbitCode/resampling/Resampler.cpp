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
