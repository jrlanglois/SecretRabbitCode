#include "MainComponent.h"

using namespace SecretRabbitCode;

MainComponent::MainComponent()
{
    LinearResampler resampler;
    DBG (resampler.getDescription());

    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

void MainComponent::resized()
{
}
