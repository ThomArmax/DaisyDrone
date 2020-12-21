/**
 * @file DaisyDrone.h
 * @details DaisyDrone header's file
 * @author Thomas COIN
 */
#ifndef DAISY_DRONE_H
#define DAISY_DRONE_H

//-------------------------------------------------
// INCLUDES
//-------------------------------------------------
#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisysp;
using namespace daisy;

//-------------------------------------------------
// CLASS DEFINITION
//-------------------------------------------------

/**
 * @brief HW abstraction for Daisy Drone
 */
class DaisyDrone
{
#define OSCILLATORS_BY_CHANNEL  2   ///< Number of oscillators by channels
#define CHANNELS_COUNT          2   ///< Number of channels

public:
    /**
     * @brief Returns the singleton instance of the DaisyDrone object
     */
    static DaisyDrone * GetInstance();

    /**
     * @brief Performs the initialization
     */
    void Init();

    /**
     * @brief Performs updates (Knobs & cie)
     */
    void Update();

private:
    DaisyDrone();
    static void AudioCallback(float *in, float *out, size_t size);

private:
    DaisySeed m_seed;
    Oscillator m_oscs[CHANNELS_COUNT][OSCILLATORS_BY_CHANNEL];    ///< Oscillators array
    Parameter m_channelFreqParam[CHANNELS_COUNT];
    Oscillator m_freqLfo;
    Parameter m_freqLfoParam;
    Switch m_oscsFormSwitch;
};

#endif // DAISY_DRONE_H
