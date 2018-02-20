#ifndef SRC_RESAMPLING_PROCESSOR_H
#define SRC_RESAMPLING_PROCESSOR_H

/**
*/
class ResamplingProcessor : public AudioProcessor
{
public:
    /** Constructor. */
    ResamplingProcessor();

    /** Destructor. */
    ~ResamplingProcessor();

    //==============================================================================
    /** Change the resampling ratio.

        A value of 1.0 means no change.
    */
    void setRatio (double newRatio);

    /** Set the ratio via sampling rates.

        @param[in] sourceRate       The source rate of the audio content.
        @param[in] destinationRate  The target rate to correct to.
    */
    void setRatio (double sourceRate, double destinationRate);

    /** @returns The currently set ratio. */
    double getRatio() const noexcept { return ratio.load(); }

    //==============================================================================
    /** Changes the resamplers to use. */
    void setResamplers (Resampler* realtimeResampler, Resampler* offlineResampler);

    /** @see AudioProcessor::processBlock */
    void processBlock (AudioBuffer<float>& buffer);

    //==============================================================================
    /** @internal */
    const String getName() const override { return TRANS ("Resampler"); }
    /** @internal */
    void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock) override;
    /** @internal */
    void releaseResources() override { }
    /** @internal */
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
    /** @internal */
    double getTailLengthSeconds() const override { return 0.0; }
    /** @internal */
    bool acceptsMidi() const override { return false; }
    /** @internal */
    bool producesMidi() const override { return false; }
    /** @internal */
    AudioProcessorEditor* createEditor() override { return nullptr; }
    /** @internal */
    bool hasEditor() const override { return false; }
    /** @internal */
    int getNumPrograms() override { return 1; }
    /** @internal */
    int getCurrentProgram() override { return 1; }
    /** @internal */
    void setCurrentProgram (int) override { }
    /** @internal */
    const String getProgramName (int) override { return { }; }
    /** @internal */
    void changeProgramName (int, const String&) override { }
    /** @internal */
    void getStateInformation (MemoryBlock&) override { }
    /** @internal */
    void setStateInformation (const void*, int) override { }

private:
    //==============================================================================
    std::atomic<double> ratio { 1.0 };
    ScopedPointer<Resampler> realtime, offline;
    AudioBuffer<float> result;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResamplingProcessor)
};

#endif //SRC_RESAMPLING_PROCESSOR_H
