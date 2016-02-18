#ifndef MAIN_COMPONENT_H
#define MAIN_COMPONENT_H

#include "JuceHeader.h"

class MainComponent : public Component
{
public:
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void resized() override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

#endif //MAIN_COMPONENT_H
