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
    #include "coefficients/CoefficientList.h"
    #include "coefficients/FastestCoefficients.h"
    #include "coefficients/HighQualityCoefficients.h"
    #include "coefficients/MidQualityCoefficients.h"
    #include "coefficients/DefaultCoefficientInitialiser.h"
    #include "resampling/Interpolator.cpp"
    #include "resampling/Resampler.cpp"
    #include "resampling/SincInterpolator.cpp"
    #include "resampling/ZeroOrderHoldResampler.cpp"
    #include "resampling/ResamplingProcessor.cpp"
}
