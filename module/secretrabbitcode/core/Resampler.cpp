Resampler::Resampler() :
    ratio (1.0)
{
}

Resampler::~Resampler()
{
}

//==============================================================================
void Resampler::setRatio (const double newRatio)
{
    if (ratio.get() != newRatio)
    {
        ratio.exchange (newRatio);
        updateRatio();
    }
}

void Resampler::setRatio (double sourceRate, double destinationRate)
{
    if (sourceRate > 0.0 && destinationRate > 0.0)
        setRatio (sourceRate / destinationRate);
}
