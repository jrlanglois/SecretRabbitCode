#include "MainComponent.h"

using namespace SecretRabbitCode;

MainComponent::MainComponent()
{
    OwnedArray<Resampler> resamplers;

    resamplers.add (new CatmullRomResampler());
    resamplers.add (new LagrangeResampler());
    resamplers.add (new LinearResampler());
    resamplers.add (new CubicHermiteResampler());

    for (auto* r : resamplers)
        Logger::writeToLog (r->getDescription());

    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

void MainComponent::resized()
{
}
