// 4 channel synthesizer engine //

/*
- (ch1) Karplus (plucked string)
- (ch2) Lead (phase-modulated)
- (ch3) Bass (saw + low-pass)
- (ch4) Drums (1=kick,2=snare,3=hat)
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

#define SAMPLE_RATE 22050
#define BPM         120
#define SONG_LENGTH_SEC 30
#define STEPS_PER_BEAT 3

const float SCALE[] = {261.63f, 293.66f, 329.63f, 349.23f, 392.00f, 440.00f, 493.88f};
const float OCTS[] = {0.5f, 1.0f, 2.0f};

uint32_t sampleIndex = 0;

const uint8_t wav_header[44] = {
  0x52,0x49,0x46,0x46,0xFF,0xFF,0xFF,0xFF,0x57,0x41,0x56,0x45,
  0x66,0x6d,0x74,0x20,0x10,0x00,0x00,0x00,0x01,0x00,0x02,0x00,
  0x22,0x56,0x00,0x00,0x44,0xac,0x00,0x00,0x02,0x00,0x08,0x00,
  0x64,0x61,0x74,0x61,0xFF,0xFF,0xFF,0xFF
};

static inline void seed_random_from_rosc(){
    uint32_t random = 0;
    uint32_t random_bit;
    volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
    for (int k = 0; k < 32; k++) {
        while (1) {
            random_bit = (*rnd_reg) & 1;
            if (random_bit != ((*rnd_reg) & 1)) break;
        }
        random = (random << 1) | random_bit;
    }
    srand(random);
    randomSeed(random);
}

static uint16_t LFSR = 0xACE1u;
static inline uint8_t lfsr_noise_byte() {
    uint16_t bit = ((LFSR>>0) ^ (LFSR>>2) ^ (LFSR>>3) ^ (LFSR>>5)) & 1;
    LFSR = (LFSR >> 1) | (bit << 15);
    return (uint8_t)(LFSR & 0xFF);
}

static inline uint8_t drum_kick(uint32_t t) {
    float env = expf(-0.0055f*(float)t);
    float pitch = 100.0f * expf(-0.0025f*(float)t);
    float s = sinf(2.0f * 3.14159265f * pitch * ((float)t / SAMPLE_RATE));
    int v = (int)((s * 127.0f + 128.0f) * env);
    if (v < 0) v = 0; if (v > 255) v = 255;
    return (uint8_t)v;
}
static inline uint8_t drum_snare(uint32_t t) {
    float env = expf(-0.004f*(float)t);
    uint8_t n = lfsr_noise_byte();
    float ton = 0.2f * (sinf(2.0f * 3.14159265f * 300.0f * ((float)t / SAMPLE_RATE)) * 0.5f + 0.5f);
    float outf = ((float)n * 0.85f + ton * 255.0f * 0.15f) * env;
    if (outf > 255.0f) outf = 255.0f;
    return (uint8_t)outf;
}
static inline uint8_t drum_hat(uint32_t t) {
    float env = expf(-0.009f*(float)t);
    uint8_t n = lfsr_noise_byte();
    float out = (float)n * env * 0.7f;
    if (out > 255.0f) out = 255.0f;
    return (uint8_t)out;
}

#define KS_MAX 2048
static float ks_buf[KS_MAX];
static uint32_t ks_len = 0;
static uint32_t ks_idx = 0;
static bool ks_active = false;
static uint32_t ks_phase = 0;

static float lead_phase = 0.0f;
static float lead_env = 0.0f;

static float bass_phase = 0.0f;
static float bass_env = 0.0f;
static float bp_lp = 0.0f;

struct Note {
    float freq1;
    float freq2;
    float freq3;
    uint8_t drum;
    float pwm;
};

uint32_t samplesPerStep;
Note currentNote;

float env1 = 0.0f, env2 = 0.0f, env3 = 0.0f;

bool drum_active = false;
uint32_t drum_phase = 0;
uint8_t drum_now = 0;

float target_freq1 = 0.0f;
float current_freq1 = 0.0f;

#define NUM_STEPS ((uint32_t)(SONG_LENGTH_SEC * BPM / 60.0f * STEPS_PER_BEAT + 0.5f))
Note songBuffer[NUM_STEPS];
uint32_t currentStepIndex = 0;

static inline float midi_to_phase_inc(float freq) {
    return freq / (float)SAMPLE_RATE;
}

static void ks_pluck(float freq, float velocity) {
    if (freq <= 0.0f) { ks_active = false; return; }
    uint32_t len = (uint32_t)((float)SAMPLE_RATE / freq + 0.5f);
    if (len < 2) len = 2;
    if (len > KS_MAX) len = KS_MAX;
    ks_len = len;
    ks_idx = 0;
    ks_phase = 0;
    for (uint32_t i = 0; i < ks_len; ++i) {
        ks_buf[i] = ((float)lfsr_noise_byte() / 255.0f - 0.5f) * 2.0f * velocity;
    }
    ks_active = true;
}

static inline float ks_tick() {
    if (!ks_active || ks_len < 2) return 0.0f;
    float out = ks_buf[ks_idx];
    float next = ks_buf[(ks_idx + 1) % ks_len];
    float avg = 0.5f * (out + next);
    avg *= 0.995f;
    ks_buf[ks_idx] = avg;
    ks_idx = (ks_idx + 1) % ks_len;
    ks_phase++;
    if (ks_phase > SAMPLE_RATE) {
        ks_active = false;
    }
    if (fabsf(avg) < 1e-4f) ks_active = false;
    return out;
}

static inline float lead_tick(float freq, float &phase, float &env) {
    if (freq <= 0.0f) return 0.0f;
    float modFreq = freq * 2.0f;
    float modIndex = 4.0f * (0.5f + 0.5f * env);
    float mod = sinf(2.0f * 3.14159265f * modFreq * ((phase) / (float)SAMPLE_RATE)) * modIndex;
    phase += freq;
    if (phase >= (float)SAMPLE_RATE) phase -= (float)SAMPLE_RATE;
    float c = sinf(2.0f * 3.14159265f * freq * ((phase + mod) / (float)SAMPLE_RATE));
    return c * env;
}

static inline float saw_tick(float &phase, float freq) {
    if (freq <= 0.0f) return 0.0f;
    phase += freq;
    if (phase >= (float)SAMPLE_RATE) phase -= (float)SAMPLE_RATE;
    float ph = phase / (float)SAMPLE_RATE;
    float s = 2.0f * (ph) - 1.0f;
    return s;
}

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

    seed_random_from_rosc();
    
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
    WriteReg16(0x03, 0x6000);
    WriteReg16(0x0B, 0x4F4F);
    PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
    PicoSPI0.transfer(0xFF);

    for(uint32_t i=0; i<NUM_STEPS; i++) {
        songBuffer[i].freq1 = SCALE[random(0,7)] * OCTS[random(0,3)];
        songBuffer[i].freq2 = SCALE[random(0,7)] * OCTS[random(0,3)];
        songBuffer[i].freq3 = SCALE[random(0,7)] * OCTS[random(0,3)/1];
        songBuffer[i].drum  = random(0,4);
        songBuffer[i].pwm   = 0.3f + 0.4f * ((float)random(0,100)/100.0f);
    }

    currentStepIndex = 0;
    currentNote = songBuffer[0];
    target_freq1 = currentNote.freq1;
    current_freq1 = target_freq1;
    samplesPerStep = (uint32_t)(SAMPLE_RATE * 60.0f / BPM / STEPS_PER_BEAT + 0.5f);

    env1 = env2 = env3 = 0.0f;
    ks_active = false;
    lead_phase = bass_phase = 0.0f;
    bp_lp = 0.0f;

}

void loop() {

    while(!digitalRead(MP3_DREQ)) {}

    static float prev_freq1 = 0.0f;
    if (currentNote.freq1 != prev_freq1) {
        ks_pluck(currentNote.freq1, 0.9f);
        env1 = 1.0f;
        prev_freq1 = currentNote.freq1;
    }
    float ks_s = ks_active ? ks_tick() : 0.0f;
    env1 *= 0.9995f;
    float ch1 = ks_s * env1 * 0.9f;

    static float lead_phase_acc = 0.0f;
    static float prev_freq2 = 0.0f;
    if (currentNote.freq2 != prev_freq2) {
        lead_env = 1.0f;
        prev_freq2 = currentNote.freq2;
    }
    float lead_freq = currentNote.freq2;
    lead_phase_acc += lead_freq;
    if (lead_phase_acc >= (float)SAMPLE_RATE) lead_phase_acc -= (float)SAMPLE_RATE;
    float mod = sinf(2.0f * 3.14159265f * (lead_freq*1.99f) * (lead_phase_acc / (float)SAMPLE_RATE)) * (0.5f + 0.5f*lead_env);
    float lead_out = sinf(2.0f * 3.14159265f * lead_freq * ((lead_phase_acc / (float)SAMPLE_RATE) + 0.002f*mod));
    lead_env *= 0.998f;
    float ch2 = lead_out * lead_env * 0.9f;

    static float bass_phase_acc = 0.0f;
    static float prev_freq3 = 0.0f;
    if (currentNote.freq3 != prev_freq3) {
        bass_env = 1.0f;
        prev_freq3 = currentNote.freq3;
    }
    float bass_freq = currentNote.freq3 * 0.5f;
    bass_phase_acc += bass_freq;
    if (bass_phase_acc >= (float)SAMPLE_RATE) bass_phase_acc -= (float)SAMPLE_RATE;
    float saw = 2.0f * (bass_phase_acc / (float)SAMPLE_RATE) - 1.0f;
    float cutoff = 200.0f + 800.0f * (bass_env);
    if (cutoff > (SAMPLE_RATE*0.45f)) cutoff = SAMPLE_RATE*0.45f;
    float rc = 1.0f / (2.0f * 3.14159265f * cutoff);
    float dt = 1.0f / (float)SAMPLE_RATE;
    float alpha = dt / (rc + dt);
    bp_lp += alpha * (saw - bp_lp);
    bass_env *= 0.999f;
    float ch3 = bp_lp * 0.8f * bass_env;

    uint8_t drum_out = 0;
    if (drum_active) {
        if (drum_now==1) drum_out = drum_kick(drum_phase);
        else if (drum_now==2) drum_out = drum_snare(drum_phase);
        else if (drum_now==3) drum_out = drum_hat(drum_phase);
        drum_phase++;
        if (drum_phase > SAMPLE_RATE/10) { drum_active=false; drum_phase=0; }
    }

    float left_f = 0.0f, right_f = 0.0f;
    left_f += ch1 * 0.2f;
    right_f += ch1 * 0.5f;
    left_f += ch2 * 0.8f;
    right_f += ch2 * 0.4f;
    left_f += ch3 * 0.4f;
    right_f += ch3 * 0.6f;
    float drum_norm = (float)drum_out / 255.0f;
    left_f += drum_norm * 4.0f;
    right_f += drum_norm * 4.0f;

    float mixL = 128.0f + left_f * 110.0f;
    float mixR = 128.0f + right_f * 110.0f;
    if (mixL < 0.0f) mixL = 0.0f; if (mixL > 255.0f) mixL = 255.0f;
    if (mixR < 0.0f) mixR = 0.0f; if (mixR > 255.0f) mixR = 255.0f;

    uint8_t outL = (uint8_t)mixL;
    uint8_t outR = (uint8_t)mixR;

    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(outL);
    PicoSPI0.transfer(outR);
    digitalWrite(MP3_XDCS, HIGH);

    sampleIndex++;
    if(sampleIndex >= samplesPerStep) {
        sampleIndex = 0;
        currentStepIndex++;
        if(currentStepIndex >= NUM_STEPS) currentStepIndex = 0;
        currentNote = songBuffer[currentStepIndex];
        if(currentNote.drum != 0) {
            drum_active = true;
            drum_now = currentNote.drum;
            drum_phase = 0;
        }
    }

}