SincInterpolator::SincInterpolator() noexcept { reset(); }
SincInterpolator::~SincInterpolator() noexcept { }

void SincInterpolator::reset() noexcept
{
    subSamplePos = 1.0;

    for (auto& s : lastInputSamples)
        s = 0.0f;
}

int SincInterpolator::process (double actualRatio, const float* in, float* out, int numOut, int available, int wrap) noexcept
{
    return interpolate<SincInterpolatorAlgorithm> (lastInputSamples, subSamplePos, actualRatio, in, out, numOut, available, wrap);
}

int SincInterpolator::process (double actualRatio, const float* in, float* out, int numOut) noexcept
{
    return interpolate<SincInterpolatorAlgorithm> (lastInputSamples, subSamplePos, actualRatio, in, out, numOut);
}

int SincInterpolator::processAdding (double actualRatio, const float* in, float* out, int numOut, int available, int wrap, float gain) noexcept
{
    return interpolateAdding<SincInterpolatorAlgorithm> (lastInputSamples, subSamplePos, actualRatio, in, out, numOut, available, wrap, gain);
}

int SincInterpolator::processAdding (double actualRatio, const float* in, float* out, int numOut, float gain) noexcept
{
    return interpolateAdding<SincInterpolatorAlgorithm> (lastInputSamples, subSamplePos, actualRatio, in, out, numOut, gain);
}
