#ifndef SRC_MATHS_H
#define SRC_MATHS_H

//=============================================================================
#if ! DOXYGEN
#if JUCE_HAS_CONSTEXPR
    #define SRC_DECLARE_VALUE_CONST(name, value) \
        static constexpr FloatType name = static_cast<FloatType> (value);

    #define SRC_DEFINE_VALUE_CONST(name, value)
#else
    #define SRC_DECLARE_VALUE_CONST(name, value) \
        static const ValueType name;

    #define SRC_DEFINE_VALUE_CONST(name, value) \
        template<typename FloatType> \
        const FloatType ValueConstants<FloatType>::name = static_cast<FloatType> (value);
#endif
#endif

/** Commonly used single value constants. */
template<typename FloatType>
class ValueConstants
{
public:
    /** This allows templated code to access the underlying type. */
    typedef typename FloatType Type;

    /** A predefined value for zero. */
    SRC_DECLARE_VALUE_CONST (zero, 0)

    /** A predefined value for 1/3. */
    SRC_DECLARE_VALUE_CONST (oneThird, 1.0 / 3.0)

    /** A predefined value for 1/2. */
    SRC_DECLARE_VALUE_CONST (half, 0.5)

    /** A predefined value for 2/3. */
    SRC_DECLARE_VALUE_CONST (twoThirds, 2.0 / 3.0)

    /** A predefined value for one. */
    SRC_DECLARE_VALUE_CONST (one, 1)

    /** A predefined value for two. */
    SRC_DECLARE_VALUE_CONST (two, 2)

    /** A predefined value for three. */
    SRC_DECLARE_VALUE_CONST (three, 3)

private:
    ValueConstants() noexcept
    {
        static_assert (std::is_floating_point<FloatType>::value, "Underlying type must be floating point!");
    }

    JUCE_DECLARE_NON_COPYABLE (ValueConstants)
};

SRC_DEFINE_VALUE_CONST (zero, 0)
SRC_DEFINE_VALUE_CONST (oneThird, 1.0 / 3.0)
SRC_DEFINE_VALUE_CONST (half, 0.5)
SRC_DEFINE_VALUE_CONST (twoThirds, 2.0 / 3.0)
SRC_DEFINE_VALUE_CONST (one, 1)
SRC_DEFINE_VALUE_CONST (two, 2)
SRC_DEFINE_VALUE_CONST (three, 3)

#undef SRC_DECLARE_VALUE_CONST
#undef SRC_DEFINE_VALUE_CONST

//=============================================================================
/** */
constexpr int numStokeSamples = 5;

//=============================================================================
template<typename ValueType>
inline JUCE_CONSTEXPR ValueType absolute (const ValueType value) noexcept
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
    const auto v = absolute (value);

    return v > ValueConstants<FloatType>::half
            ? ValueConstants<FloatType>::zero
            : (v < ValueConstants<FloatType>::half
                ? ValueConstants<FloatType>::one
                : ValueConstants<FloatType>::half);
}

#endif //SRC_MATHS_H