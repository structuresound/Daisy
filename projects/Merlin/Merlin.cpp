#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "perlin.hpp"
/** TODO: ADD CALIBRATION */

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

constexpr auto nPoly = 2U;
constexpr auto nUnisons = 3U;
constexpr auto nVoices = nPoly * nUnisons;
constexpr auto normalize = 1.0f / nVoices;
constexpr auto fast_inv_7b = 1.0f / 128.0f;


constexpr auto detuneRange = 0.025f;
constexpr auto spread = 1.0f;

float drift_phase = 0.0f;
float drift_accum = 0.001f;

// const float equalTempered[13] = {1.000000f,
//                                  1.059463f,
//                                  1.122462f,
//                                  1.189207f,
//                                  1.259921f,
//                                  1.334839f,
//                                  1.414213f,
//                                  1.498307f,
//                                  1.587401f,
//                                  1.681792f,
//                                  1.781797f,
//                                  1.887748f,
//                                  2.000000f};

DaisyPatchSM patch;
Oscillator voices[nVoices];

float detune_perlin(float angle){
    // float ap = angle + phase;
    float xoff = cosf(angle) + 1 * 0.5;
    float yoff = sinf(angle) + 1 * 0.5;
    return perlin_two(xoff, yoff);
}

void AudioCallback(AudioHandle::InputBuffer in,
                   AudioHandle::OutputBuffer out,
                   size_t bufferLength)
{
    patch.ProcessAllControls();

    float knob_coarse = patch.GetAdcValue(CV_1);
    float knob_fine = patch.GetAdcValue(CV_2);
    float knob_detune = patch.GetAdcValue(CV_3);
    float knob_drift = patch.GetAdcValue(CV_4);

    /** Get Coarse, Fine, and V/OCT inputs from hardware
     *  MIDI Note number are easy to use for defining ranges */

    float coarse_tune = fmap(knob_coarse, 0, 48);
    float fine_tune = fmap(knob_fine, 0, 4);


    float cv_p1 = patch.GetAdcValue(CV_5);
    // float voct = powf(2.0f, (cv_p1 * 3.3f)) * 55;
    float voct = fmap(cv_p1, 0, 60);
    // float voct = ftom(f);
    float cv_p2 = patch.GetAdcValue(CV_6);
    // float voct2 = powf(2.0f, (cv_p2 * 3.3f)) * 55;
    float voct2 = fmap(cv_p2, 0, 60);
    //  float voct2 = ftom(f2);

    // float cv_semi = patch.GetAdcValue(CV_4);
    // size_t semi = floor(fmap(cv_semi, 0, 13));

    float pitches[nPoly] = {0,0};
    float loudness[nPoly] = {0, 0};

    /** Calculate a detune amount */
    // float drift = detune_perlin(drift_phase+=drift_accum) * knob_drift;

    /** Convert from MIDI note number to frequency */
    float midi_nn = fclamp(coarse_tune + fine_tune  + voct, 0.f, 127.f);
    pitches[0] = mtof(midi_nn);
    // loudness[0] = powf((127 - midi_nn) * fast_inv_7b, 2.0f) * normalize;

    float midi_nn2 = fclamp(coarse_tune + fine_tune  + voct2, 0.f, 127.f);
    pitches[1] = mtof(midi_nn2);
    // loudness[1] = powf((127 - midi_nn2) * fast_inv_7b, 2.0f) * normalize;

    /** BRate **/
    for (size_t p = 0; p < nPoly; p++)
    {
        float detune = pitches[p] * knob_detune * detuneRange;
        for (size_t u = 0; u < nUnisons; u++)
        {
            auto v = p * nUnisons + u;
            float f = pitches[p] + (u == 1
                                        ? detune 
                                        : u == 2
                                          ? -detune 
                                          : 0);
            voices[v].SetFreq(f);
        }
    }

    /** SRate **/
    for (size_t i = 0; i < bufferLength; i++)
    {
        // VOICE 1
        float sig_l = voices[0].Process();
        float sig_r = voices[nUnisons].Process();
        // for (size_t u = 1; u < nUnisons; u++)
        // {
        //     sig_l = sig_l + (voices[u].Process() * knob_detune);
        //     sig_r = sig_r + (voices[nUnisons + u].Process() * knob_detune);
        // }
        OUT_L[i] = sig_l * normalize;
        OUT_R[i] = sig_r * normalize;
    }
}

int main(void)
{
    patch.Init();
    for (size_t i = 0; i < nVoices; i++)
    {
        // voices[i] = new Oscillator();
        voices[i].Init(patch.AudioSampleRate());
        voices[i].SetWaveform(Oscillator::WAVE_TRI);
    }
    patch.StartAudio(AudioCallback);
    while (1)
    {
    }
}
