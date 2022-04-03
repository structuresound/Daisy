#include "daisy_patch_sm.h"
#include "daisysp.h"

/** TODO: ADD CALIBRATION */

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

constexpr auto nVoices = 4U;
constexpr auto nPitches = 2U;
constexpr auto nUnisons = nVoices / nPitches;
constexpr auto normalize = 1.0f / nVoices;
constexpr auto detuneRange = 0.025f;
constexpr auto spread = 1.0f;

const float equalTempered[13] = {1.000000f,
                                 1.059463f,
                                 1.122462f,
                                 1.189207f,
                                 1.259921f,
                                 1.334839f,
                                 1.414213f,
                                 1.498307f,
                                 1.587401f,
                                 1.681792f,
                                 1.781797f,
                                 1.887748f,
                                 2.000000f};

DaisyPatchSM patch;
Oscillator voices[nVoices];

void AudioCallback(AudioHandle::InputBuffer in,
                   AudioHandle::OutputBuffer out,
                   size_t bufferLength)
{
    patch.ProcessAllControls();

    /** Get Coarse, Fine, and V/OCT inputs from hardware
     *  MIDI Note number are easy to use for defining ranges */
    float knob_coarse = patch.GetAdcValue(CV_1);
    float coarse_tune = fmap(knob_coarse, 12, 84);

    float knob_fine = patch.GetAdcValue(CV_2);
    float fine_tune = fmap(knob_fine, 0, 10);

    float cv_voct = patch.GetAdcValue(CV_5);
    float voct = fmap(cv_voct, 0, 60);

    float cv_semi = patch.GetAdcValue(CV_4);
    size_t semi = floor(fmap(cv_semi, 0, 13));

    /** Convert from MIDI note number to frequency */
    float midi_nn = fclamp(coarse_tune + fine_tune + voct, 0.f, 127.f);
    float root = mtof(midi_nn);

    /** Calculate a detune amount */
    float detuneAmt = patch.GetAdcValue(CV_3);

    float pitches[2] = {root, root * equalTempered[semi]};

    /** BRate **/
    for (size_t p = 0; p < nPitches; p++)
    {
        float detune = pitches[p] * detuneAmt * (detuneRange * (p + 1.0f));
        for (size_t u = 0; u < nUnisons; u++)
        {
            auto v = p * nUnisons + u;
            float f = pitches[p] + (u == 0
                                        ? detune
                                        : -detune);
            voices[v].SetFreq(f);
        }
    }

    /** SRate **/
    for (size_t i = 0; i < bufferLength; i++)
    {
        float sig = 0;
        for (auto &v : voices)
        {
            sig = sig + v.Process();
        }
        OUT_L[i] = sig * normalize;
        OUT_R[i] = OUT_L[i];
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
