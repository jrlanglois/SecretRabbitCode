struct LinearInterpolatorAlgorithm
{
    template<typename FloatType>
    static FloatType valueAtOffset (const FloatType* const inputs, const FloatType offset) noexcept
    {
        constexpr auto one = static_cast<FloatType> (1);
        const auto y0 = inputs[0];
        const auto y1 = inputs[1];

        return ((one - offset) * y0) + (offset * y1);
    }
};

LinearInterpolator::LinearInterpolator() noexcept { reset(); }
LinearInterpolator::~LinearInterpolator() noexcept {}

void LinearInterpolator::reset() noexcept
{
    subSamplePos = 1.0;

    for (auto& s : lastInputSamples)
        s = 0;
}

int LinearInterpolator::process (double actualRatio, const float* in, float* out, int numOut, int available, int wrap) noexcept
{
    return interpolate<LinearInterpolatorAlgorithm> (lastInputSamples, subSamplePos, actualRatio, in, out, numOut, available, wrap);
}

int LinearInterpolator::process (double actualRatio, const float* in, float* out, int numOut) noexcept
{
    return interpolate<LinearInterpolatorAlgorithm> (lastInputSamples, subSamplePos, actualRatio, in, out, numOut);
}

int LinearInterpolator::processAdding (double actualRatio, const float* in, float* out, int numOut, int available, int wrap, float gain) noexcept
{
    return interpolateAdding<LinearInterpolatorAlgorithm> (lastInputSamples, subSamplePos, actualRatio, in, out, numOut, available, wrap, gain);
}

int LinearInterpolator::processAdding (double actualRatio, const float* in, float* out, int numOut, float gain) noexcept
{
    return interpolateAdding<LinearInterpolatorAlgorithm> (lastInputSamples, subSamplePos, actualRatio, in, out, numOut, gain);
}
