#ifndef SECRETRABBITCODE_MODULE_HEADER_H
#define SECRETRABBITCODE_MODULE_HEADER_H

/** BEGIN_JUCE_MODULE_DECLARATION

    ID:             SecretRabbitCode
    vendor:         jrlanglois
    version:        1.0.0
    name:           SecretRabbitCode
    description:    A library for performing sample rate conversion of audio data.
    website:        https://github.com/jrlanglois/SecretRabbitCode
    license:        BSD

    dependencies:   juce_audio_utils juce_gui_basics juce_cryptography

    END_JUCE_MODULE_DECLARATION
*/
//=============================================================================
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_cryptography/juce_cryptography.h>

//=============================================================================
namespace SecretRabbitCode
{
    using namespace juce;

    //=============================================================================
    /** Commonly used single value constants. */
    template<typename ValueType>
    struct ValueConstants
    {
        /** A predefined value for zero. */
        static constexpr ValueType zero = static_cast<ValueType> (0);

        /** A predefined value for 1/2. This will probably be something
            weird for integrals, best to avoid using such a type!
        */
        static constexpr ValueType half = static_cast<ValueType> (0.5);

        /** A predefined value for one. */
        static constexpr ValueType one = static_cast<ValueType> (1);

        /** A predefined value for two. */
        static constexpr ValueType two = static_cast<ValueType> (2);

        /** A predefined value for three. */
        static constexpr ValueType three = static_cast<ValueType> (3);
    };

    //=============================================================================
    /** */
    constexpr int numStokeSamples = 5;

    //=============================================================================
    template<typename ValueType>
    inline constexpr ValueType absolute (const ValueType value) noexcept
    {
        return value > ValueType{} ? value : -value;
    }

    /** */
    inline double fmodOne (double value)
    {
        const auto r = value - std::rint (value);
        if (r < 0.0)
            return r + 1.0;

        return r;
    }

    /** */
    inline float fmodOne (float value)
    {
        const auto r = value - std::rintf (value);
        if (r < 0.0f)
            return r + 1.0f;

        return r;
    }

    //=============================================================================
    /** */
    template<typename FloatType>
    inline FloatType sinc (FloatType value)
    {
        if (value == ValueConstants<FloatType>::zero)
            return ValueConstants<FloatType>::one;

        return std::sin (value) / value;
    }

    /** */
    template<typename FloatType>
    inline FloatType normalisedSinc (FloatType value)
    {
        return sinc (value * MathConstants<FloatType>::pi);
    }

    //=============================================================================
    /** */
    template<typename FloatType>
    inline FloatType rect (const FloatType value) noexcept
    {
        constexpr auto half = static_cast<FloatType> (0.5);
        const auto v = absolute (value);

        return v > half
                ? ValueConstants<FloatType>::zero
                : (v < half ? ValueConstants<FloatType>::one : half);
    }

    //=============================================================================
    /** 2-point linear interpolation. */
    struct LinearInterpolatorAlgorithm
    {
        template<typename FloatType>
        static FloatType valueAtOffset (const FloatType* const inputs, const FloatType offset) noexcept
        {
            const auto y0 = inputs[0];
            const auto y1 = inputs[1];

            return ((ValueConstants<FloatType>::one - offset) * y0) + (offset * y1);
        }
    };

    /** 4-point Cubic Hermite interpolation. */
    struct CubicHermiteInterpolatorAlgorithm
    {
        template<typename FloatType>
        static FloatType interpolate (FloatType x0, FloatType x1, FloatType x2, FloatType x3, FloatType t)
        {
            constexpr auto onePointFive = static_cast<FloatType> (1.5);
            constexpr auto twoPointFive = static_cast<FloatType> (2.5);

            const auto c0 = x1;
            const auto c1 = ValueConstants<FloatType>::half * (x2 - x0);
            const auto c2 = x0 - (twoPointFive * x1) + (ValueConstants<FloatType>::two * x2) - (ValueConstants<FloatType>::half * x3);
            const auto c3 = (ValueConstants<FloatType>::half * (x3 - x0)) + (onePointFive * (x1 - x2));

            return (((((c3 * t) + c2) * t) + c1) * t) + c0;
        }

        template<typename FloatType>
        static FloatType valueAtOffset (const FloatType* const inputs, const FloatType offset) noexcept
        {
            return interpolate (inputs[0], inputs[1], inputs[2], inputs[3], offset);
        }
    };

    /** 4-point cubic interpolation. */
    struct CubicInterpolatorAlgorithm
    {
        template<typename FloatType>
        static FloatType interpolate (FloatType x0, FloatType x1, FloatType x2, FloatType x3, FloatType t)
        {
            const auto a0 = x3 - x2 - x0 + x1;
            const auto a1 = x0 - x1 - a0;
            const auto a2 = x2 - x0;

            return (a0 * std::pow (t, ValueConstants<FloatType>::three))
                 + (a1 * std::pow (t, ValueConstants<FloatType>::two))
                 + (a2 * t)
                 + x1;
        }

        template<typename FloatType>
        static FloatType valueAtOffset (const FloatType* const inputs, const FloatType offset) noexcept
        {
            return interpolate (inputs[0], inputs[1], inputs[2], inputs[3], offset);
        }
    };

    /** 4-point, 3rd-order B-spline interpolation. */
    struct BSplineInterpolatorAlgorithm
    {
        template<typename FloatType>
        static FloatType interpolate (FloatType x0, FloatType x1, FloatType x2, FloatType x3, FloatType t)
        {
            constexpr auto oneOverSix = static_cast<FloatType> (1.0 / 6.0);
            constexpr auto twoThirds = static_cast<FloatType> (2.0 / 3.0);

            const auto ym1py1   = x0 + x2;
            const auto c0       = oneOverSix * ym1py1 + twoThirds * x1;
            const auto c1       = ValueConstants<FloatType>::half * (x2 - x0);
            const auto c2       = ValueConstants<FloatType>::half * ym1py1 - x1;
            const auto c3       = ValueConstants<FloatType>::half * (x1 - x2) + oneOverSix * (x3 - x0);

            return (((c3 * t + c2) * t + c1) * t + c0);
        }

        template<typename FloatType>
        static FloatType valueAtOffset (const FloatType* const inputs, const FloatType offset) noexcept
        {
            return interpolate (inputs[0], inputs[1], inputs[2], inputs[3], offset);
        }
    };

    /** */
    struct SincInterpolatorAlgorithm
    {
        template<typename FloatType>
        static FloatType valueAtOffset (const FloatType* const inputs, const FloatType offset) noexcept
        {
            const auto y0 = inputs[0];
            const auto y1 = inputs[1];

            return ((ValueConstants<FloatType>::one - offset) * y0) + (offset * y1);
        }
    };

    //=============================================================================
    #include "core/LinearInterpolator.h"
    #include "core/SincInterpolator.h"
    #include "core/Resampler.h"
    #include "core/ZeroOrderHoldResampler.h"
}

#endif //SECRETRABBITCODE_MODULE_HEADER_H
