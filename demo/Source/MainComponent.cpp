#include "MainComponent.h"

using namespace SecretRabbitCode;

MainComponent::MainComponent()
{
    LinearInterpolator li;
    li.reset();

    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

void MainComponent::resized()
{
}
