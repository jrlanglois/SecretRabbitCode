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
    static forcedinline void pushInterpolationSample (FloatType* lastInputSamples, FloatType newValue = (FloatType) 0) noexcept
    {
        lastInputSamples[4] = lastInputSamples[3];
        lastInputSamples[3] = lastInputSamples[2];
        lastInputSamples[2] = lastInputSamples[1];
        lastInputSamples[1] = lastInputSamples[0];
        lastInputSamples[0] = newValue;
    }

    template<typename FloatType>
    static forcedinline void pushInterpolationSamples (FloatType* lastInputSamples, const FloatType* input, int numOut) noexcept
    {
        if (numOut >= 5)
        {
            for (int i = 0; i < 5; ++i)
                lastInputSamples[i] = input[--numOut];
        }
        else
        {
            for (int i = 0; i < numOut; ++i)
                pushInterpolationSample (lastInputSamples, input[i]);
        }
    }

    template<typename FloatType>
    static forcedinline void pushInterpolationSamples (FloatType* lastInputSamples, const FloatType* input,
                                                       int numOut, int available, int wrapAround) noexcept
    {
        if (numOut >= 5)
        {
            if (available >= 5)
            {
                for (int i = 0; i < 5; ++i)
                    lastInputSamples[i] = input[--numOut];
            }
            else
            {
                for (int i = 0; i < available; ++i)
                    lastInputSamples[i] = input[--numOut];

                if (wrapAround > 0)
                {
                    numOut -= wrapAround;

                    for (int i = available; i < 5; ++i)
                        lastInputSamples[i] = input[--numOut];
                }
                else
                {
                    for (int i = available; i < 5; ++i)
                        lastInputSamples[i] = 0.0f;
                }
            }
        }
        else
        {
            if (numOut > available)
            {
                for (int i = 0; i < available; ++i)
                    pushInterpolationSample (lastInputSamples, input[i]);

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
                for (int i = 0; i < numOut; ++i)
                    pushInterpolationSample (lastInputSamples, input[i]);
            }
        }
    }

    template<typename InterpolatorType, typename FloatType>
    static int interpolate (FloatType* lastInputSamples, double& subSamplePos, double actualRatio,
                            const FloatType* in, FloatType* out, int numOut) noexcept
    {
        auto pos = subSamplePos;

        if (actualRatio == 1.0 && pos == 1.0)
        {
            memcpy (out, in, (size_t) numOut * sizeof (FloatType));
            pushInterpolationSamples (lastInputSamples, in, numOut);
            return numOut;
        }

        int numUsed = 0;

        while (numOut > 0)
        {
            while (pos >= 1.0)
            {
                pushInterpolationSample (lastInputSamples, in[numUsed++]);
                pos -= 1.0;
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
        if (actualRatio == 1.0)
        {
            if (available >= numOut)
            {
                memcpy (out, in, (size_t) numOut * sizeof (FloatType));
                pushInterpolationSamples (lastInputSamples, in, numOut, available, wrap);
            }
            else
            {
                memcpy (out, in, (size_t) available * sizeof (FloatType));
                pushInterpolationSamples (lastInputSamples, in, numOut, available, wrap);

                if (wrap > 0)
                {
                    memcpy (out + available, in + available - wrap, (size_t) (numOut - available) * sizeof (FloatType));
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

        if (actualRatio < 1.0)
        {
            for (int i = numOut; --i >= 0;)
            {
                if (pos >= 1.0)
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

                    pos += 1.0;
                }

                pos -= actualRatio;
                *out++ = InterpolatorType::valueAtOffset (lastInputSamples, jmax (0.0f, 1.0f - (FloatType) pos));
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
        if (actualRatio == 1.0)
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

        if (actualRatio < 1.0)
        {
            for (int i = numOut; --i >= 0;)
            {
                if (pos >= 1.0)
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

                    pos += 1.0;
                }

                pos -= actualRatio;
                *out++ += gain * InterpolatorType::valueAtOffset (lastInputSamples, jmax (0.0f, 1.0f - (FloatType) pos));
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

        if (actualRatio == 1.0 && pos == 1.0)
        {
            FloatVectorOperations::addWithMultiply (out, in, gain, numOut);
            pushInterpolationSamples (lastInputSamples, in, numOut);
            return numOut;
        }

        int numUsed = 0;

        while (numOut > 0)
        {
            while (pos >= 1.0)
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

    #include "core/Resampler.cpp"
    #include "core/LinearInterpolator.cpp"
    #include "core/ZeroOrderHoldResampler.cpp"
}
