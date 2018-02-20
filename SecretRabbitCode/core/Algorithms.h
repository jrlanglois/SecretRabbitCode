#ifndef SRC_ALGORITHMS_H
#define SRC_ALGORITHMS_H

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

//=============================================================================
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

//=============================================================================
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

//=============================================================================
/** 4-point, 3rd-order B-spline interpolation. */
struct BSplineInterpolatorAlgorithm
{
    template<typename FloatType>
    static FloatType interpolate (FloatType x0, FloatType x1, FloatType x2, FloatType x3, FloatType t)
    {
        constexpr auto oneOverSix = static_cast<FloatType> (1.0 / 6.0);

        const auto ym1py1   = x0 + x2;
        const auto c0       = oneOverSix * ym1py1 + ValueConstants<FloatType>::twoThirds * x1;
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

//=============================================================================
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

#endif //SRC_ALGORITHMS_H
