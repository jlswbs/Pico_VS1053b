// 4 channel chip tune engine //

/*
- (ch1) Square PWM glide
- (ch2) Triangle
- (ch3) Saw/Noise
- (ch4) Drum (BD/SD/HAT)
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

static inline uint8_t square_wave_pwm(float freq, float &phase, float pwm) {
    if (freq <= 0.0f) return 0;
    phase += freq / SAMPLE_RATE;
    if (phase >= 1.0f) phase -= 1.0f;
    return (phase < pwm) ? 200 : 20;
}

static inline uint8_t triangle_wave_phase(float freq, float &phase) {
    if (freq <= 0.0f) return 0;
    phase += freq / SAMPLE_RATE;
    if (phase >= 1.0f) phase -= 1.0f;
    float v = phase < 0.5f ? phase*2.0f : (1.0f - (phase-0.5f)*2.0f);
    return (uint8_t)(v*255.0f);
}

static inline uint8_t saw_wave_phase(float freq, float &phase) {
    if (freq <= 0.0f) return 0;
    phase += freq / SAMPLE_RATE;
    if (phase >= 1.0f) phase -= 1.0f;
    return (uint8_t)(phase*255.0f);
}

struct Note {
    float freq1;
    float freq2;
    float freq3;
    uint8_t drum;
    float pwm;
};

uint32_t samplesPerStep;
Note currentNote;

float phase1 = 0.0f;
float phase2 = 0.0f;
float phase3 = 0.0f;
float env1 = 0.0f, env2 = 0.0f, env3 = 0.0f;

bool drum_active = false;
uint32_t drum_phase = 0;
uint8_t drum_now = 0;

float target_freq1 = 0.0f;
float current_freq1 = 0.0f;

#define NUM_STEPS ((uint32_t)(SONG_LENGTH_SEC * BPM / 60.0f * STEPS_PER_BEAT + 0.5f))
Note songBuffer[NUM_STEPS];
uint32_t currentStepIndex = 0;

static inline uint8_t drum_kick(uint32_t t) {
    float pitch = 100.0f * expf(-0.0015f*(float)t);
    if (pitch < 30.0f) pitch = 30.0f;
    float env = expf(-0.005f*(float)t);
    uint8_t s = (t % 2 < 1) ? 200 : 20;
    return (uint8_t)((float)s * env);
}

static inline uint8_t drum_snare(uint32_t t) {
    float env = expf(-0.0035f*(float)t);
    uint8_t n = lfsr_noise_byte();
    uint8_t tone = ((t % 4 < 2) ? 200 : 50) >> 2;
    float outf = ((float)n*0.8f + (float)tone*0.2f) * env;
    return (uint8_t)((outf>255.0f)?255.0f:outf);
}

static inline uint8_t drum_hat(uint32_t t) {
    float env = expf(-0.01f*(float)t);
    uint8_t n = lfsr_noise_byte();
    float outf = (float)n * env * 0.6f;
    return (uint8_t)((outf>255.0f)?255.0f:outf);
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
        songBuffer[i].freq3 = SCALE[random(0,7)] * OCTS[random(0,3)];
        songBuffer[i].drum  = random(0,4);
        songBuffer[i].pwm   = 0.3f + 0.4f * ((float)random(0,100)/100.0f);
    }

    currentStepIndex = 0;
    currentNote = songBuffer[0];
    target_freq1 = currentNote.freq1;
    current_freq1 = target_freq1;
    samplesPerStep = (uint32_t)(SAMPLE_RATE * 60.0f / BPM / STEPS_PER_BEAT + 0.5f);

}

void loop() {

    while(!digitalRead(MP3_DREQ)) {}

    current_freq1 += (target_freq1 - current_freq1) * 0.02f;
    uint8_t c1 = (uint8_t)((float)square_wave_pwm(current_freq1, phase1, currentNote.pwm) * env1);
    uint8_t c2 = (uint8_t)((float)triangle_wave_phase(currentNote.freq2, phase2) * env2);
    uint8_t c3 = (uint8_t)((float)saw_wave_phase(currentNote.freq3, phase3) * env3);

    uint8_t drum_out = 0;
    if(drum_active) {
        if(drum_now==1) drum_out = drum_kick(drum_phase);
        else if(drum_now==2) drum_out = drum_snare(drum_phase);
        else if(drum_now==3) drum_out = drum_hat(drum_phase);
        drum_phase++;
        if(drum_phase > SAMPLE_RATE/10) { drum_active=false; drum_phase=0; }
    }

    uint8_t drum_left  = drum_out;
    uint8_t drum_right = drum_out;
    uint8_t square_left  = (uint8_t)(c1 * 0.6f);
    uint8_t square_right = (uint8_t)(c1 * 0.4f);
    uint8_t tri_left  = (uint8_t)(c2 * 0.35f);
    uint8_t tri_right = (uint8_t)(c2 * 0.65f);
    uint8_t saw_left  = (uint8_t)(c3 * 0.55f);
    uint8_t saw_right = (uint8_t)(c3 * 0.45f);

    int left  = square_left + tri_left + saw_left + drum_left;
    int right = square_right + tri_right + saw_right + drum_right;
    if(left > 255) left = 255;
    if(right > 255) right = 255;

    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(left);
    PicoSPI0.transfer(right);
    digitalWrite(MP3_XDCS, HIGH);

    env1 *= 0.9997f;
    env2 *= 0.995f;
    env3 *= 0.99f;

    sampleIndex++;
    if(sampleIndex >= samplesPerStep) {
        sampleIndex = 0;
        currentStepIndex++;
        if(currentStepIndex >= NUM_STEPS) currentStepIndex = 0;
        currentNote = songBuffer[currentStepIndex];
        phase1 = phase2 = phase3 = 0.0f;
        env1 = 1.0f; env2 = 1.0f; env3 = 0.9f;
        target_freq1 = currentNote.freq1;
        if(currentNote.drum != 0) {
            drum_active = true;
            drum_now = currentNote.drum;
            drum_phase = 0;
        }
    }

}