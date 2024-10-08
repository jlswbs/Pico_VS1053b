#pragma once
#ifndef DSY_VARISAWOSCILLATOR_H
#define DSY_VARISAWOSCILLATOR_H

#include <stdint.h>

class VariableSawOscillator
{
  public:
    VariableSawOscillator() {}
    ~VariableSawOscillator() {}

    void Init(float sample_rate);

    /** Get the next sample */
    float Process();

    /** Set master freq.
        \param frequency Freq in Hz.
    */
    void SetFreq(float frequency);

    /** Adjust the wave depending on the shape
        \param pw Notch or slope. Works best -1 to 1.
    */
    void SetPW(float pw);

    /** Slope or notch
        \param waveshape 0 = notch, 1 = slope
    */
    void SetWaveshape(float waveshape);


  private:
    float ComputeNaiveSample(float phase,
                             float pw,
                             float slope_up,
                             float slope_down,
                             float triangle_amount,
                             float notch_amount);

    float sample_rate_;

    float phase_;
    float next_sample_;
    float previous_pw_;
    bool  high_;

    const float kVariableSawNotchDepth = 0.2f;

    float frequency_;
    float pw_;
    float waveshape_;
};

#endif