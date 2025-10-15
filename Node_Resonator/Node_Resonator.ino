// Resonant node network with reverb //

#include "hardware/structs/rosc.h"
#include "PicoSPI.h"

#define MP3_CLK 2
#define MP3_MOSI 3
#define MP3_MISO 4
#define MP3_CS 5
#define MP3_XDCS 17
#define MP3_DREQ 26
#define MP3_RST 13

#define SAMPLE_RATE 22050

#define NUM_MODES 6
#define WEIGHT_UPDATE_MS 150
#define BURST_US 100
#define Q15_ONE 32768
#define REVERB_BUF_SIZE 2205
#define REVERB_TAPS 4
#define REVERB_DECAY 0.45f

volatile float modeFreq[NUM_MODES];
float rMode = 0.998f;
volatile float sigCurr[NUM_MODES], sigPrev[NUM_MODES];
volatile float two_r_cosw[NUM_MODES], r2[NUM_MODES];
float gainMode[NUM_MODES];
volatile int16_t weights[NUM_MODES * NUM_MODES];
volatile bool burstActive = false;
volatile uint16_t burstSamplesRemaining = 0;
volatile bool triggerUpdate = false;
uint32_t rng_state = 1337;
float reverbBufL[REVERB_BUF_SIZE];
float reverbBufR[REVERB_BUF_SIZE];
uint16_t revIndex = 0;
const uint16_t tapDelaysL[REVERB_TAPS] = {311, 619, 887, 1301};
const uint16_t tapDelaysR[REVERB_TAPS] = {401, 733, 997, 1499};

float processReverb(float inMono, float *outL, float *outR) {
  float accL = 0.0f, accR = 0.0f;
  for (int i = 0; i < REVERB_TAPS; i++) {
    uint16_t idxL = (revIndex + REVERB_BUF_SIZE - tapDelaysL[i]) % REVERB_BUF_SIZE;
    uint16_t idxR = (revIndex + REVERB_BUF_SIZE - tapDelaysR[i]) % REVERB_BUF_SIZE;
    accL += reverbBufL[idxL];
    accR += reverbBufR[idxR];
  }
  accL /= REVERB_TAPS;
  accR /= REVERB_TAPS;
  reverbBufL[revIndex] = inMono + accL * REVERB_DECAY;
  reverbBufR[revIndex] = inMono + accR * REVERB_DECAY;
  revIndex++;
  if (revIndex >= REVERB_BUF_SIZE) revIndex = 0;
  float dry = 0.6f;
  float wet = 0.4f;
  float gain = 1.5f;
  *outL = gain * (dry * inMono + wet * accL);
  *outR = gain * (dry * inMono + wet * accR);
  return (accL + accR) * 0.5f;
}

uint16_t fastRand16() {
  rng_state = rng_state * 1664525u + 1013904223u;
  return (uint16_t)(rng_state >> 16);
}

inline float softClip(float x) {
  if (x > 1.0f) x = 1.0f;
  if (x < -1.0f) x = -1.0f;
  return x - (x * x * x) * 0.333f;
}

void randomizeWeights() {
  for (int i = 0; i < NUM_MODES * NUM_MODES; ++i) {
    int16_t v = ((int16_t)(fastRand16() % 13107)) - 6553;
    weights[i] = v;
  }
}

void randomizeFrequencies() {
  for (int i = 0; i < NUM_MODES; i++) {
    float rnd = (fastRand16() / 65535.0f);
    modeFreq[i] = 100.0f + rnd * (8000.0f - 100.0f);
  }
}

void updateModeCoeffs() {
  for (int i = 0; i < NUM_MODES; ++i) {
    float omega = 2.0f * (float)M_PI * modeFreq[i] / SAMPLE_RATE;
    two_r_cosw[i] = 2.0f * rMode * cosf(omega);
    r2[i] = rMode * rMode;
  }
}

void initModes() {
  for (int i = 0; i < NUM_MODES; ++i) {
    sigCurr[i] = sigPrev[i] = 0.0f;
    gainMode[i] = 0.7f;
  }
  randomizeFrequencies();
  updateModeCoeffs();
}

static inline uint32_t read_from_rosc() {
  uint32_t random = 0;
  volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
  for (int k = 0; k < 32; k++) {
    uint32_t bit = 0;
    while (1) {
      bit = (*rnd_reg) & 1;
      if (bit != ((*rnd_reg) & 1)) break;
    }
    random = (random << 1) | bit;
  }
  return random;
}

void seed_random_safe() {
  uint32_t seed = 0;
  for (int i = 0; i < 8; i++) {
    seed ^= read_from_rosc();
    delayMicroseconds(100);
  }
  rng_state = seed;
  srand(seed);
}

void WriteReg(unsigned char address, unsigned char highbyte, unsigned char lowbyte) {
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS, LOW);
  PicoSPI0.transfer(0x02);
  PicoSPI0.transfer(address);
  PicoSPI0.transfer(highbyte);
  PicoSPI0.transfer(lowbyte);
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS, HIGH);
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

const uint8_t wav_header[44] = {
  0x52,0x49,0x46,0x46,0xFF,0xFF,0xFF,0xFF,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
  0x10,0x00,0x00,0x00,0x01,0x00,0x02,0x00,0x22,0x56,0x00,0x00,0x44,0xac,0x00,0x00,
  0x02,0x00,0x08,0x00,0x64,0x61,0x74,0x61,0xFF,0xFF,0xFF,0xFF
};

void load_header() {
  for (int i = 0; i < sizeof(wav_header); i++) {
    uint8_t p = wav_header[i];
    while (!digitalRead(MP3_DREQ)) {}
    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(p);
    digitalWrite(MP3_XDCS, HIGH);
  }
}

void setup() {

  seed_random_safe();

  pinMode(MP3_DREQ, INPUT_PULLDOWN);
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

  initModes();
  randomizeWeights();

}

void loop() {

  float in = 0.0f;
  if (burstActive && burstSamplesRemaining > 0) {
    int16_t r = fastRand16();
    in = ((r / 32768.0f) * 0.8f);
    burstSamplesRemaining--;
  } else burstActive = false;
  float inputs[NUM_MODES] = {0};
  for (int i = 0; i < NUM_MODES; i++) {
    float acc = 0.0f;
    for (int j = 0; j < NUM_MODES; j++) acc += (weights[i * NUM_MODES + j] / (float)Q15_ONE) * sigCurr[j];
    inputs[i] = acc + in * gainMode[i];
  }
  float total = 0.0f;
  for (int i = 0; i < NUM_MODES; i++) {
    float sigNew = two_r_cosw[i] * sigCurr[i] - r2[i] * sigPrev[i] + inputs[i];
    sigNew = softClip(sigNew);
    sigPrev[i] = sigCurr[i];
    sigCurr[i] = sigNew;
    total += sigNew;
  }

  float outL, outR;
  processReverb(total * 0.35f, &outL, &outR);
  if (outL < -1) outL = -1; if (outL > 1) outL = 1;
  if (outR < -1) outR = -1; if (outR > 1) outR = 1;
  uint8_t left = (uint8_t)((outL * 0.5f + 0.5f) * 255);
  uint8_t right = (uint8_t)((outR * 0.5f + 0.5f) * 255);

  digitalWrite(MP3_XDCS, LOW);
  PicoSPI0.transfer(left);
  PicoSPI0.transfer(right);
  digitalWrite(MP3_XDCS, HIGH);
  while (!digitalRead(MP3_DREQ)) {}

}

void setup1() {

  seed_random_safe();
  analogReadResolution(12);
  rng_state = rand();

}

void loop1() {

  randomizeWeights();
  randomizeFrequencies();
  updateModeCoeffs();

  burstSamplesRemaining = (uint16_t)((SAMPLE_RATE * BURST_US) / 1000000.0f);
  if (burstSamplesRemaining < 1) burstSamplesRemaining = 1;
  burstActive = true;

  delay(WEIGHT_UPDATE_MS);

}