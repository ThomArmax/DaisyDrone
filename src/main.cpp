//-------------------------------------------------
// INCLUDES
//-------------------------------------------------
#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

//-------------------------------------------------
// DEFINITIONS
//-------------------------------------------------

#define OSC_COUNT           4           ///< Number of oscillators
#define BASE_FREQ           440.0       ///< Base frequency for oscillators
#define FREQ_INC_FACTOR     1.5         ///< Base frequency for oscillators
#define FREQ_MODIFIER_PIN   21

//-------------------------------------------------
// VARIABLES
//-------------------------------------------------
static DaisySeed    seed;               ///< The DaisySeed instance
static Oscillator   oscs[OSC_COUNT];    ///< Oscillators array
static float        freqs[OSC_COUNT];   ///< Oscillators frequencies
static float        freqModifier = 1.0;

//-------------------------------------------------
// CALLBACKS
//-------------------------------------------------

static void AudioCallback(float *in, float *out, size_t size)
{
    float sig;
    freqModifier = seed.adc.GetFloat(0);

    for (size_t j = 0; j < OSC_COUNT; j++)
    {
        oscs[j].SetFreq(freqs[j] * freqModifier);
    }

    for(size_t i = 0; i < size; i += 2)
    {
        sig = 0;
        for (size_t j = 0; j < OSC_COUNT; j++)
        {
            sig += oscs[j].Process();
        }
        out[i] = sig;// left out
        out[i + 1] = sig; // right out
    }
}

//-------------------------------------------------
// FUNCTIONS
//-------------------------------------------------

int main(void)
{
    // initialize seed hardware and oscillator daisysp module
    float sampleRate;
    seed.Configure();
    seed.Init();
    sampleRate = seed.AudioSampleRate();

    // Oscillators init
    for (int i = 0; i < OSC_COUNT; i++)
    {
        freqs[i] = BASE_FREQ + i*FREQ_INC_FACTOR;
        oscs[i].Init(sampleRate);
        oscs[i].SetWaveform(Oscillator::WAVE_RAMP);
        oscs[i].SetFreq(freqs[i]);
        oscs[i].SetAmp(0.5);
    }

    // ADC init
    AdcChannelConfig adcConfig;
    adcConfig.InitSingle(seed.GetPin(FREQ_MODIFIER_PIN));
    seed.adc.Init(&adcConfig, 1);
    seed.adc.Start();

    // start callback
    seed.StartAudio(AudioCallback);

    volatile bool ledState = false;

    while(1)
    {
        seed.SetLed(ledState);
        ledState = !ledState;
        dsy_system_delay(100);
    }
}
