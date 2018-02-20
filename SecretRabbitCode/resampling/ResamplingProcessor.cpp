ResamplingProcessor::ResamplingProcessor() :
    realtime (new LagrangeResampler()),
    offline (new LagrangeResampler())
{
    if (auto* ins = getBus (false, 0))
        ins->setCurrentLayout (AudioChannelSet::canonicalChannelSet (2));

    if (auto* outs = getBus (true, 0))
        outs->setCurrentLayout (AudioChannelSet::canonicalChannelSet (2));

    enableAllBuses();
    setRateAndBufferSizeDetails (44100.0, 256);
}

ResamplingProcessor::~ResamplingProcessor()
{
}

void ResamplingProcessor::setResamplers (Resampler* realtimeResampler, Resampler* offlineResampler)
{
    const ScopedLock sl (getCallbackLock());
    if (realtime != realtimeResampler || offline != offlineResampler)
    {
        realtime = realtimeResampler;
        offline = offlineResampler;
    }

    jassert (realtime != nullptr);
}

void ResamplingProcessor::setRatio (double newRatio)
{
    ratio = jlimit (0.00001, 5.0, newRatio);
}

void ResamplingProcessor::setRatio (double sourceRate, double destinationRate)
{
    if (sourceRate > 0.0 && destinationRate > 0.0)
        setRatio (sourceRate / destinationRate);
}

void ResamplingProcessor::prepareToPlay (double newSampleRate, int estimatedSamplesPerBlock)
{
    setRateAndBufferSizeDetails (newSampleRate, estimatedSamplesPerBlock);

    const ScopedLock sl (getCallbackLock());

    const int numChans = jmax (getTotalNumInputChannels(), getTotalNumOutputChannels());

    jassert (realtime != nullptr);
    realtime->prepare (numChans, estimatedSamplesPerBlock, newSampleRate);

    if (offline != nullptr)
        offline->prepare (numChans, estimatedSamplesPerBlock, newSampleRate);

    result.setSize (numChans, estimatedSamplesPerBlock, false, true, true);
}

void ResamplingProcessor::processBlock (AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    const auto r = getRatio();

    if (isSuspended() || numSamples <= 0 || r == 1.0000000000)
        return;

    const ScopedLock sl (getCallbackLock());

    auto* resamplerToUse = realtime.get();

    if (isNonRealtime() && offline != nullptr)
        resamplerToUse = offline;

    jassert (resamplerToUse != nullptr);
    resamplerToUse->setRatio (r);

    const int newSize = (int) jmax (1.0, (double) numSamples / r);
    result.setSize (buffer.getNumChannels(), newSize, false, false, true);
    result.clear();

    resamplerToUse->process (r, buffer, result);
}

void ResamplingProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
    processBlock (buffer);
}
