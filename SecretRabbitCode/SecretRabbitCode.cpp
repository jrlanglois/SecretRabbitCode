#ifdef SECRETRABBITCODE_MODULE_HEADER_H
    /** When you add this cpp file to your project, you mustn't include it in a file where you've
        already included any other headers - just put it inside a file on its own, possibly with your config
        flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
        header files that the compiler may be using.
    */
    #error "Incorrect use of SecretRabbitCode.cpp file!"
#endif

#include "SecretRabbitCode.h"

namespace SecretRabbitCode
{
    template<typename FloatType>
    static forcedinline void pushInterpolationSample (FloatType* lastInputSamples, FloatType newValue = ValueConstants<FloatType>::zero) noexcept
    {
       #if 0
        lastInputSamples[4] = lastInputSamples[3];
        lastInputSamples[3] = lastInputSamples[2];
        lastInputSamples[2] = lastInputSamples[1];
        lastInputSamples[1] = lastInputSamples[0];
       #else
        constexpr auto rotationLength = numStokeSamples - 1;
        auto arrayEnd = lastInputSamples + rotationLength;

        std::rotate (lastInputSamples, arrayEnd, arrayEnd);
       #endif

        lastInputSamples[0] = newValue;
    }

    template<typename FloatType>
    static forcedinline void pushInput (FloatType* lastInputSamples, const FloatType* input, int numSamplesToPush) noexcept
    {
        for (int i = 0; i < numSamplesToPush; ++i)
            pushInterpolationSample (lastInputSamples, input[i]);
    }

    template<typename FloatType>
    static forcedinline void pushInterpolationSamples (FloatType* lastInputSamples, const FloatType* input, int numOut) noexcept
    {
        if (numOut >= numStokeSamples)
        {
            for (int i = 0; i < numStokeSamples; ++i)
                lastInputSamples[i] = input[--numOut];
        }
        else
        {
            pushInput (lastInputSamples, input, numOut);
        }
    }

    template<typename FloatType>
    static forcedinline void pushInterpolationSamples (FloatType* lastInputSamples, const FloatType* input,
                                                       int numOut, int available, int wrapAround) noexcept
    {
        if (numOut >= numStokeSamples)
        {
            if (available >= numStokeSamples)
            {
                for (int i = 0; i < numStokeSamples; ++i)
                    lastInputSamples[i] = input[--numOut];
            }
            else
            {
                for (int i = 0; i < available; ++i)
                    lastInputSamples[i] = input[--numOut];

                if (wrapAround > 0)
                {
                    numOut -= wrapAround;

                    for (int i = available; i < numStokeSamples; ++i)
                        lastInputSamples[i] = input[--numOut];
                }
                else
                {
                    for (int i = available; i < numStokeSamples; ++i)
                        lastInputSamples[i] = ValueConstants<FloatType>::zero;
                }
            }
        }
        else
        {
            if (numOut > available)
            {
                pushInput (lastInputSamples, input, available);

                if (wrapAround > 0)
                {
                    for (int i = 0; i < numOut - available; ++i)
                        pushInterpolationSample (lastInputSamples, input[i + available - wrapAround]);
                }
                else
                {
                    for (int i = 0; i < numOut - available; ++i)
                        pushInterpolationSample (lastInputSamples);
                }
            }
            else
            {
                pushInput (lastInputSamples, input, numOut);
            }
        }
    }

    template<typename InterpolatorType, typename FloatType>
    static int interpolate (FloatType* lastInputSamples, double& subSamplePos, double actualRatio,
                            const FloatType* in, FloatType* out, int numOut) noexcept
    {
        auto pos = subSamplePos;

        if (actualRatio == ValueConstants<FloatType>::one && pos == ValueConstants<FloatType>::one)
        {
            FloatVectorOperations::copy (out, in, numOut);
            pushInterpolationSamples (lastInputSamples, in, numOut);
            return numOut;
        }

        int numUsed = 0;

        while (numOut > 0)
        {
            while (pos >= ValueConstants<FloatType>::one)
            {
                pushInterpolationSample (lastInputSamples, in[numUsed++]);
                pos -= ValueConstants<FloatType>::one;
            }

            *out++ = InterpolatorType::valueAtOffset (lastInputSamples, (FloatType) pos);
            pos += actualRatio;
            --numOut;
        }

        subSamplePos = pos;
        return numUsed;
    }

    template<typename InterpolatorType, typename FloatType>
    static int interpolate (FloatType* lastInputSamples, double& subSamplePos, double actualRatio,
                            const FloatType* in, FloatType* out, int numOut, int available, int wrap) noexcept
    {
        if (actualRatio == ValueConstants<FloatType>::one)
        {
            if (available >= numOut)
            {
                FloatVectorOperations::copy (out, in, numOut);
                pushInterpolationSamples (lastInputSamples, in, numOut, available, wrap);
            }
            else
            {
                FloatVectorOperations::copy (out, in, available);
                pushInterpolationSamples (lastInputSamples, in, numOut, available, wrap);

                if (wrap > 0)
                {
                    FloatVectorOperations::copy (out + available, in + available - wrap, numOut - available);
                    pushInterpolationSamples (lastInputSamples, in, numOut, available, wrap);
                }
                else
                {
                    for (int i = 0; i < numOut - available; ++i)
                        pushInterpolationSample (lastInputSamples);
                }
            }

            return numOut;
        }

        auto originalIn = in;
        auto pos = subSamplePos;
        bool exceeded = false;

        if (actualRatio < ValueConstants<FloatType>::one)
        {
            for (int i = numOut; --i >= 0;)
            {
                if (pos >= ValueConstants<FloatType>::one)
                {
                    if (exceeded)
                    {
                        pushInterpolationSample (lastInputSamples);
                    }
                    else
                    {
                        pushInterpolationSample (lastInputSamples, *in++);

                        if (--available <= 0)
                        {
                            if (wrap > 0)
                            {
                                in -= wrap;
                                available += wrap;
                            }
                            else
                            {
                                exceeded = true;
                            }
                        }
                    }

                    pos -= ValueConstants<FloatType>::one;
                }

                *out++ = InterpolatorType::valueAtOffset (lastInputSamples, (FloatType) pos);
                pos += actualRatio;
            }
        }
        else
        {
            for (int i = numOut; --i >= 0;)
            {
                while (pos < actualRatio)
                {
                    if (exceeded)
                    {
                        pushInterpolationSample (lastInputSamples);
                    }
                    else
                    {
                        pushInterpolationSample (lastInputSamples, *in++);

                        if (--available <= 0)
                        {
                            if (wrap > 0)
                            {
                                in -= wrap;
                                available += wrap;
                            }
                            else
                            {
                                exceeded = true;
                            }
                        }
                    }

                    pos += ValueConstants<FloatType>::one;
                }

                pos -= actualRatio;
                *out++ = InterpolatorType::valueAtOffset (lastInputSamples, jmax (ValueConstants<FloatType>::zero, ValueConstants<FloatType>::one - (FloatType) pos));
            }
        }

        subSamplePos = pos;
        return ((int) (in - originalIn) + wrap) % wrap;
    }

    template<typename InterpolatorType, typename FloatType>
    static int interpolateAdding (FloatType* lastInputSamples, double& subSamplePos, double actualRatio,
                                  const FloatType* in, FloatType* out, int numOut,
                                  int available, int wrap, FloatType gain) noexcept
    {
        if (actualRatio == ValueConstants<FloatType>::one)
        {
            if (available >= numOut)
            {
                FloatVectorOperations::addWithMultiply (out, in, gain, numOut);
                pushInterpolationSamples (lastInputSamples, in, numOut, available, wrap);
            }
            else
            {
                FloatVectorOperations::addWithMultiply (out, in, gain, available);
                pushInterpolationSamples (lastInputSamples, in, available, available, wrap);

                if (wrap > 0)
                {
                    FloatVectorOperations::addWithMultiply (out, in - wrap, gain, numOut - available);
                    pushInterpolationSamples (lastInputSamples, in - wrap, numOut - available, available, wrap);
                }
                else
                {
                    for (int i = 0; i < numOut-available; ++i)
                        pushInterpolationSample (lastInputSamples);
                }
            }

            return numOut;
        }

        auto originalIn = in;
        auto pos = subSamplePos;
        bool exceeded = false;

        if (actualRatio < ValueConstants<FloatType>::one)
        {
            for (int i = numOut; --i >= 0;)
            {
                if (pos >= ValueConstants<FloatType>::one)
                {
                    if (exceeded)
                    {
                        pushInterpolationSample (lastInputSamples);
                    }
                    else
                    {
                        pushInterpolationSample (lastInputSamples, *in++);

                        if (--available <= 0)
                        {
                            if (wrap > 0)
                            {
                                in -= wrap;
                                available += wrap;
                            }
                            else
                            {
                                exceeded = true;
                            }
                        }
                    }

                    pos -= 1.0;
                }

                *out++ += gain * InterpolatorType::valueAtOffset (lastInputSamples, (FloatType) pos);
                pos += actualRatio;
            }
        }
        else
        {
            for (int i = numOut; --i >= 0;)
            {
                while (pos < actualRatio)
                {
                    if (exceeded)
                    {
                        pushInterpolationSample (lastInputSamples);
                    }
                    else
                    {
                        pushInterpolationSample (lastInputSamples, *in++);

                        if (--available <= 0)
                        {
                            if (wrap > 0)
                            {
                                in -= wrap;
                                available += wrap;
                            }
                            else
                            {
                                exceeded = true;
                            }
                        }
                    }

                    pos += ValueConstants<FloatType>::one;
                }

                pos -= actualRatio;
                *out++ += gain * InterpolatorType::valueAtOffset (lastInputSamples, jmax (ValueConstants<FloatType>::zero, ValueConstants<FloatType>::one - (FloatType) pos));
            }
        }

        subSamplePos = pos;
        return ((int) (in - originalIn) + wrap) % wrap;
    }

    template<typename InterpolatorType, typename FloatType>
    static int interpolateAdding (FloatType* lastInputSamples, double& subSamplePos, double actualRatio,
                                  const FloatType* in, FloatType* out, int numOut, FloatType gain) noexcept
    {
        auto pos = subSamplePos;

        if (actualRatio == ValueConstants<FloatType>::one && pos == 1.0)
        {
            FloatVectorOperations::addWithMultiply (out, in, gain, numOut);
            pushInterpolationSamples (lastInputSamples, in, numOut);
            return numOut;
        }

        int numUsed = 0;

        while (numOut > 0)
        {
            while (pos >= ValueConstants<FloatType>::one)
            {
                pushInterpolationSample (lastInputSamples, in[numUsed++]);
                pos -= 1.0;
            }

            *out++ += gain * InterpolatorType::valueAtOffset (lastInputSamples, (FloatType) pos);
            pos += actualRatio;
            --numOut;
        }

        subSamplePos = pos;
        return numUsed;
    }

    #include "coefficients/CoefficientList.h"
    #include "coefficients/FastestCoefficients.h"
    #include "coefficients/HighQualityCoefficients.h"
    #include "coefficients/MidQualityCoefficients.h"
    #include "coefficients/DefaultCoefficientInitialiser.h"

    #include "core/LinearInterpolator.cpp"
    #include "core/Resampler.cpp"
    #include "core/SincInterpolator.cpp"
    #include "core/ZeroOrderHoldResampler.cpp"
}
