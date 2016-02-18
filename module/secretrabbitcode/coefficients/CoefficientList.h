#ifndef SRC_COEFFICIENT_LIST_H
#define SRC_COEFFICIENT_LIST_H

class CoefficientList
{
public:
    CoefficientList (int stepping) :
        step (stepping)
    {
    }

    CoefficientList (int stepping, const double* sourceCoefficients, int numberOfElements) :
        step (stepping)
    {
        setFrom (sourceCoefficients, numberOfElements);
    }

    void setFrom (const double* sourceCoefficients, int numberOfElements)
    {
        jassert (sourceCoefficients != nullptr);
        jassert (numberOfElements > 0);

        coefficients.insertArray (0, sourceCoefficients, numberOfElements);
    }

    int getStepping() const noexcept { return step; }

    double operator[] (int index) const { return coefficients[index]; }

    double getUnchecked (int index) const { return coefficients.getUnchecked (index); }

private:
    int step;
    Array<double> coefficients;

    JUCE_DECLARE_NON_COPYABLE (CoefficientList)
};

#endif //SRC_COEFFICIENT_LIST_H
