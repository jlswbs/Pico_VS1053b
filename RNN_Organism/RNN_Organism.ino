// Stereo RNN - Recurrent Network Sound Generator //

/*

  Pot1 = Energy
  Pot2 = Slew
  Pot3 = RestLen

*/

#include "hardware/structs/rosc.h"
#include "PicoSPI.h"

#define MP3_CLK   2
#define MP3_MOSI  3
#define MP3_MISO  4
#define MP3_CS    5
#define MP3_XDCS  17
#define MP3_DREQ  26
#define MP3_RST   13

#define SAMPLE_RATE 44100
#define N_NEURONS   12
#define DT          (1.0f / SAMPLE_RATE)

float soft_sign(float x) { return x / (1.0f + fabsf(x)); }

struct Neuron {
    float x;
    int   rest;
    bool  prevFired;
    float adapt;
    float slewMult;
};

static volatile float p_couple;
static volatile float p_restLen;
static volatile float p_baseStep;
static float p_gain;
static float p_bias;
static float p_adaptRate;
static float p_adaptAmt;

static Neuron neurons[N_NEURONS];
static float  W[N_NEURONS * N_NEURONS];
static float  inputBuf[N_NEURONS];

static uint32_t lcg_state = 1;

static inline uint32_t murmur_mix(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6bu;
    h ^= h >> 13;
    h *= 0xc2b2ae35u;
    h ^= h >> 16;
    return h;
}

static inline uint32_t lcg_next() {
    lcg_state = lcg_state * 1664525u + 1013904223u;
    return murmur_mix(lcg_state);
}

static inline float lcg_bipolar() {
    return ((lcg_next() >> 8) * (1.0f / 16777216.0f)) * 2.0f - 1.0f;
}

static inline float rnd_range(float lo, float hi) {
    return lo + (hi - lo) * ((lcg_next() >> 8) * (1.0f / 16777216.0f));
}

static void seed_random_safe() {

    uint32_t seed = 0x12345678u;
    volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

    for (int i = 0; i < 512; i++) {
        seed ^= (*rnd_reg) & 1;
    }

    for (int i = 0; i < 64; i++) {
        uint32_t bit = (*rnd_reg) & 1;
        seed = murmur_mix(seed ^ (bit << (i & 31)) ^ micros());
        delayMicroseconds(13 + (seed & 0x1F)); 
    }
    
    lcg_state = seed ? seed : 0x9E3779B9u;

}

static void randomize_params() {

    float slewNorm = rnd_range(0.0f, 1.0f);
    float initSlew = 0.8f + (slewNorm * slewNorm) * 7.5f;
    p_baseStep = initSlew * DT * 1000.0f;

    float loopEnergy = rnd_range(2.6f, 4.4f);
    float gainNorm = rnd_range(0.0f, 1.0f);
    p_gain = 0.85f + (gainNorm * gainNorm) * 2.35f;
    p_couple = loopEnergy / p_gain; 

    float restNorm = rnd_range(0.0f, 1.0f);
    p_restLen = 0.0005f + (restNorm * restNorm) * 0.0045f;

    float rateNorm = rnd_range(0.0f, 1.0f);
    p_adaptRate = 0.00005f + (rateNorm * rateNorm) * 0.00445f;
    p_adaptAmt = rnd_range(0.15f, 0.85f);

    float biasNorm = rnd_range(-1.0f, 1.0f);
    p_bias = biasNorm * fabsf(biasNorm) * 0.045f;

}

static void build_network() {

    float density = rnd_range(0.20f, 0.45f);
    float weightScale = rnd_range(1.20f, 1.45f) / sqrtf(N_NEURONS * density);

    for (int i = 0; i < N_NEURONS; ++i) {
        for (int j = 0; j < N_NEURONS; ++j) {
            float w = 0.0f;
            if (i != j) {
                int dist = abs(i - j);
                if (dist > N_NEURONS / 2) dist = N_NEURONS - dist;
                float prob = (dist == 1) ? 0.80f : (dist == 2 ? 0.40f : density * 0.3f);
                float r = (lcg_next() >> 8) * (1.0f / 16777216.0f);
                if (r < prob) {
                    float raw = lcg_bipolar();
                    w = raw * fabsf(raw) * 1.8f * weightScale;
                    if (w > 0.0f) w *= 0.85f;
                }
            }
            W[i * N_NEURONS + j] = w;
        }
    }

}

static void init_neurons() {

    for (int i = 0; i < N_NEURONS; ++i) {
        neurons[i].x = lcg_bipolar() * 0.1f;
        neurons[i].rest = 0;
        neurons[i].prevFired = false;
        neurons[i].adapt = 0.0f;
        float r = (lcg_next() >> 8) * (1.0f / 16777216.0f);
        neurons[i].slewMult = 0.3f + (r * r) * 1.9f;
    }

}

static void squark_step() {

    for (int i = 0; i < N_NEURONS; ++i) {
        if (neurons[i].rest > 0) { inputBuf[i] = 0.0f; continue; }
        float s = p_bias;
        const float* row = &W[i * N_NEURONS];
        for (int j = 0; j < N_NEURONS; ++j) {
            s += row[j] * neurons[j].x;
        }
        inputBuf[i] = s * p_couple;
    }

    for (int i = 0; i < N_NEURONS; ++i) {
        Neuron& n = neurons[i];

        if (n.rest > 0) { --n.rest; continue; }

        float signedAdapt = n.x; 
        n.adapt += p_adaptRate * (signedAdapt - n.adapt);
        float adaptTerm = n.adapt * p_adaptAmt;

        float drive  = inputBuf[i] - adaptTerm;
        float target = soft_sign(p_gain * drive);

        float dx = target - n.x;
        float maxStep = p_baseStep * n.slewMult;
        if (dx >  maxStep) dx =  maxStep;
        if (dx < -maxStep) dx = -maxStep;
        n.x += dx;

        bool nowFired = (n.x > 0.0f);
        bool fired    = nowFired && !n.prevFired;
        n.prevFired   = nowFired;

        if (fired) {
            n.rest = (int)(p_restLen / DT);
        }
    }

}

const uint8_t wav_header[44] = {

    'R','I','F','F', 0xFF,0xFF,0xFF,0x7F, 'W','A','V','E',
    'f','m','t',' ', 0x10,0x00,0x00,0x00, 0x01,0x00,0x02,0x00,
    0x44,0xAC,0x00,0x00, 0x88,0x58,0x01,0x00, 0x02,0x00,0x08,0x00,
    'd','a','t','a', 0xDB,0xFF,0xFF,0x7F

};

void WriteReg16(unsigned char address, unsigned int databyte) {

    while(!digitalRead(MP3_DREQ));
    digitalWrite(MP3_CS, LOW);
    PicoSPI0.transfer(0x02);
    PicoSPI0.transfer(address);
    PicoSPI0.transfer((databyte >> 8) & 0xFF);
    PicoSPI0.transfer(databyte & 0xFF);
    while(!digitalRead(MP3_DREQ));
    digitalWrite(MP3_CS, HIGH);

}

void load_header() {

    for (int i=0; i<44; i++) {
        while(!digitalRead(MP3_DREQ)){ }
        digitalWrite(MP3_XDCS, LOW);
        PicoSPI0.transfer(wav_header[i]);
        digitalWrite(MP3_XDCS, HIGH);
    }

}


void setup() {

    analogReadResolution(12);
    seed_random_safe();

    randomize_params();
    build_network();
    init_neurons();

    pinMode(MP3_DREQ, INPUT);
    pinMode(MP3_CS, OUTPUT);
    pinMode(MP3_XDCS, OUTPUT);
    pinMode(MP3_RST, OUTPUT);

    PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 4000000, 0, true);
    PicoSPI0.transfer(0xFF);
    digitalWrite(MP3_RST, HIGH);
    digitalWrite(MP3_CS, HIGH);
    digitalWrite(MP3_XDCS, HIGH);

    load_header();
    WriteReg16(0x03, 0x9800);
    WriteReg16(0x0B, 0x3F3F);

    PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
    PicoSPI0.transfer(0xFF);

}

void loop() {

    while(!digitalRead(MP3_DREQ)) {}

    squark_step();
   
    static float dcL = 0.0f, dcR = 0.0f;
    dcL += 0.001f * (neurons[0].x - dcL);
    dcR += 0.001f * (neurons[N_NEURONS/2].x - dcR);

    float outL = neurons[0].x - dcL;
    float outR = neurons[N_NEURONS/2].x - dcR;

    if (outL >  0.99f) outL =  0.99f;
    if (outL < -0.99f) outL = -0.99f;
    if (outR >  0.99f) outR =  0.99f;
    if (outR < -0.99f) outR = -0.99f;

    uint8_t sampleL = (uint8_t)(128.0f + 127.0f * outL);
    uint8_t sampleR = (uint8_t)(128.0f + 127.0f * outR);

    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(sampleL);
    PicoSPI0.transfer(sampleR);
    digitalWrite(MP3_XDCS, HIGH);

}

void setup1() {

}

void loop1() {

    static float smoothEnergy = 1.5f;
    static float smoothSlew   = 5.0f;
    static float smoothRest   = 0.002f;

    float rawPot1 = analogRead(A1) / 4095.0f;
    float targetEnergy = 2.0f + (rawPot1 * rawPot1) * 2.8f;
    smoothEnergy += 0.05f * (targetEnergy - smoothEnergy);

    float rawPot2 = analogRead(A2) / 4095.0f;
    float targetSlew = 0.8f + (rawPot2 * rawPot2) * 24.2f;
    smoothSlew += 0.05f * (targetSlew - smoothSlew);

    float rawPot3 = analogRead(A3) / 4095.0f;
    float targetRest = 0.0001f + (rawPot3 * rawPot3 * rawPot3) * 0.025f;
    smoothRest += 0.05f * (targetRest - smoothRest);

    p_couple   = smoothEnergy / p_gain;
    p_restLen  = smoothRest;
    p_baseStep = smoothSlew * DT * 1000.0f;

    delay(1);

}