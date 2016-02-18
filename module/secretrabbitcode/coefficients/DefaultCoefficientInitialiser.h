#ifndef SRC_DEFAULT_COEFFICIENT_INITIALISER_H
#define SRC_DEFAULT_COEFFICIENT_INITIALISER_H

struct DefaultCoefficientInitialiser
{
    DefaultCoefficientInitialiser() :
        fastestCoefficients (128, fastCoefficientsData, numElementsInArray (fastCoefficientsData)),
        midQualityCoefficients (491, midQualityCoefficientsData, numElementsInArray (midQualityCoefficientsData)),
        highQualityCoefficients (2381, highQualityCoefficientsData, numElementsInArray (highQualityCoefficientsData))
    {
    }

    CoefficientList fastestCoefficients, midQualityCoefficients, highQualityCoefficients;

private:
    JUCE_DECLARE_NON_COPYABLE (DefaultCoefficientInitialiser)
};

static const DefaultCoefficientInitialiser defaultCoefficients;

#endif //SRC_DEFAULT_COEFFICIENT_INITIALISER_H
