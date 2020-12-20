/**
 * @file DaisyDrone.cpp
 * @details DaisyDrone implementation file
 * @author Thomas COIN
 */
//-------------------------------------------------
// INCLUDES
//-------------------------------------------------
#include "DaisyDrone.h"

using namespace daisy;

//-------------------------------------------------
// DEFINITIONS
//-------------------------------------------------

//-------------------------------------------------
// STATIC VARIABLES
//-------------------------------------------------
static DaisyDrone * s_instance = nullptr;

//-------------------------------------------------
// CONSTANTS
//-------------------------------------------------
static const float FreqIncFactor                    = 1.5;          ///< Frequency increment factor for each oscillator of each channel
static const uint8_t FreqKnobsPins[CHANNELS_COUNT]  = { 21, 20 };   ///< Channels frequencies knobs pins
static const float FreqMin                          = 15.0;         ///< Min frequency in Hz
static const float FreqMax                          = 1000.0;       ///< Max frequency in Hz
static const float LfoFreqMin                       = 0.05;         ///< Min frequency in Hz for LFO
static const float LfoFreqMax                       = 500;          ///< Max frequency in Hz for LFO

//-------------------------------------------------
// CONSTRUCTOR
//-------------------------------------------------

DaisyDrone::DaisyDrone()
{

}

//-------------------------------------------------
// METHODS
//-------------------------------------------------

DaisyDrone * DaisyDrone::GetInstance()
{
    static DaisyDrone __instance;
    if (s_instance == nullptr)
        s_instance = &__instance;
    return s_instance;
}

void DaisyDrone::Init()
{
    // Seed init
    m_seed.Configure();
    m_seed.Init();

    // Oscillators init
    float sampleRate = m_seed.AudioSampleRate();

    for (int chan = 0; chan < CHANNELS_COUNT; chan++)
    {
        for (int osc = 0; osc < OSCILLATORS_BY_CHANNEL; osc++)
        {
            m_oscs[chan][osc].Init(sampleRate);
            m_oscs[chan][osc].SetWaveform(Oscillator::WAVE_SQUARE);
            m_oscs[chan][osc].SetFreq(0.0);
            m_oscs[chan][osc].SetAmp(0.5);
        }
    }

    // ADC init
    AdcChannelConfig adcFreqKnobsCfg[CHANNELS_COUNT /* channels knobs */ + 1 /* freq lfo*/];
    for (int chan = 0; chan < CHANNELS_COUNT; chan++)
    {
        adcFreqKnobsCfg[chan].InitSingle(m_seed.GetPin(FreqKnobsPins[chan]));
    }
    adcFreqKnobsCfg[2].InitSingle(m_seed.GetPin(19));
    m_seed.adc.Init(adcFreqKnobsCfg, sizeof(adcFreqKnobsCfg) / sizeof(AdcChannelConfig));
    m_seed.adc.Start();

    // Parameters init
    for (int chan = 0; chan < CHANNELS_COUNT; chan++)
    {
        AnalogControl ctl;
        ctl.Init(m_seed.adc.GetPtr(chan), m_seed.AudioCallbackRate());
        m_channelFreqParam[chan].Init(ctl, FreqMin, FreqMax, Parameter::LOGARITHMIC);
    }

    // Global freq LFO init
    AnalogControl freqLogCtl;
    freqLogCtl.Init(m_seed.adc.GetPtr(2), m_seed.AudioCallbackRate());
    m_freqLfoParam.Init(freqLogCtl, LfoFreqMin, LfoFreqMax, Parameter::LINEAR);
    m_freqLfo.Init(sampleRate);
    m_freqLfo.SetFreq(0.0);
    m_freqLfo.SetAmp(50);
    m_freqLfo.SetWaveform(Oscillator::WAVE_SIN);

    // start callback
    m_seed.StartAudio(AudioCallback);
}

void DaisyDrone::Update()
{

}

//-------------------------------------------------
// CALLBACKS
//-------------------------------------------------

void DaisyDrone::AudioCallback(float *in, float *out, size_t size)
{
    DaisyDrone *d = GetInstance();

    float sig;

    float lfoFreq = d->m_freqLfoParam.Process();
    d->m_freqLfo.SetFreq(lfoFreq);
    float lfoRate = abs(d->m_freqLfo.Process());

    for (int chan = 0; chan < CHANNELS_COUNT; chan++)
    {
        float newFreq = d->m_channelFreqParam[chan].Process() + lfoRate;
        for (size_t osc = 0; osc < OSCILLATORS_BY_CHANNEL; osc++)
        {
            d->m_oscs[chan][osc].SetFreq((newFreq + osc*FreqIncFactor));
        }
    }

    for(size_t i = 0; i < size; i += 2)
    {
        sig = 0;
        for (int chan = 0; chan < CHANNELS_COUNT; chan++)
        {
            for (size_t osc = 0; osc < OSCILLATORS_BY_CHANNEL; osc++)
            {
                sig += d->m_oscs[chan][osc].Process();
            }
        }
        out[i] = sig;// left out
        //out[i + 1] = sig; // right out
    }
}
