// Spectral drone with reverb //

#include "PicoSPI.h"

#define MP3_CLK   2
#define MP3_MOSI  3
#define MP3_MISO  4
#define MP3_CS    5
#define MP3_XDCS  17
#define MP3_DREQ  26
#define MP3_RST   13

#define SAMPLE_RATE 22050
#define NUM_OSC     32
#define DELAY_LEN   8000
#define FEEDBACK_Q15  24576
#define WET_Q15       16384

int16_t sinLUT[256];

inline int16_t fast_sin(uint16_t phase) {
  return sinLUT[(phase >> 8) & 0xFF];
}

void init_sinLUT() {
  for (int i = 0; i < 256; i++)
    sinLUT[i] = (int16_t)(sinf(2.0f * M_PI * i / 256.0f) * 32767.0f);
}

struct Osc {
  uint32_t phase;
  uint32_t step;
  int16_t  amp;
};

Osc osc[NUM_OSC];

int16_t delayL[DELAY_LEN];
int16_t delayR[DELAY_LEN];
uint32_t dIdx = 0;

uint32_t lfo_phase_amp = 0;
uint32_t lfo_phase_pan = 0;
uint32_t lfo_phase_freq = 0;
uint32_t lfo_phase_drift = 0;

void next_sample(int16_t &l, int16_t &r) {

  lfo_phase_amp  += 25;
  lfo_phase_pan  += 40;
  lfo_phase_freq += 10;
  lfo_phase_drift += 2;

  int16_t lfo_amp  = fast_sin(lfo_phase_amp) >> 2;
  int16_t lfo_pan  = fast_sin(lfo_phase_pan) >> 2;
  int16_t lfo_freq = fast_sin(lfo_phase_freq) >> 6;
  int16_t lfo_drift = fast_sin(lfo_phase_drift) >> 7;

  int32_t mix = 0;
  for (int i = 0; i < NUM_OSC; i++) {
    uint32_t drift_mod = ((lfo_drift * (i + 1)) >> 3);
    uint32_t step_mod = osc[i].step + ((lfo_freq * (i + 1)) >> 3) + drift_mod;
    osc[i].phase += step_mod;
    int16_t s = fast_sin(osc[i].phase);
    mix += (s * (osc[i].amp + ((lfo_amp * (NUM_OSC - i)) >> 4))) >> 15;
  }

  int16_t dry = (int16_t)(mix >> 1);
  int16_t delayedL = delayL[dIdx];
  int16_t delayedR = delayR[(dIdx + DELAY_LEN / 2) % DELAY_LEN];

  int32_t newL = dry + ((delayedR * FEEDBACK_Q15) >> 15);
  int32_t newR = dry + ((delayedL * FEEDBACK_Q15) >> 15);

  delayL[dIdx] = (int16_t)newL;
  delayR[dIdx] = (int16_t)newR;

  dIdx++;
  if (dIdx >= DELAY_LEN) dIdx = 0;

  int32_t panL = 32767 - (lfo_pan + 32768) / 2;
  int32_t panR = (lfo_pan + 32768) / 2;

  l = (int16_t)(((dry * panL + delayedL * WET_Q15) >> 16));
  r = (int16_t)(((dry * panR + delayedR * WET_Q15) >> 16));

}

const uint8_t wav_header[44] = {
  'R','I','F','F', 0x24,0x80,0x00,0x00, 'W','A','V','E',
  'f','m','t',' ', 0x10,0x00,0x00,0x00, 0x01,0x00,0x02,0x00,
  0x22,0x56,0x00,0x00, 0x88,0x58,0x01,0x00, 0x04,0x00,0x10,0x00,
  'd','a','t','a', 0x00,0x80,0x00,0x00
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
  for (int i = 0; i < sizeof(wav_header); i++) {
    uint8_t p = wav_header[i];
    while (!digitalRead(MP3_DREQ)) {}
    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(p);
    digitalWrite(MP3_XDCS, HIGH);
  }
  for (int i = 0; i < 512; i++) {
    while (!digitalRead(MP3_DREQ)) {}
    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(0x00);
    digitalWrite(MP3_XDCS, HIGH);
  }
}

void setup() {

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
  WriteReg16(0x03, 0x9800);
  WriteReg16(0x0B, 0x3F3F);
  PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);

  init_sinLUT();

  for (int i = 0; i < NUM_OSC; i++) {
    osc[i].phase = 0;
    float f = 110.0f * (i + 1);
    osc[i].step = (uint32_t)((f / SAMPLE_RATE) * 65536.0f);
    osc[i].amp = 8192 / (i + 1);
  }

  for (int i = 0; i < DELAY_LEN; i++) {
    delayL[i] = delayR[i] = 0;
  }

}

void loop() {

  while(!digitalRead(MP3_DREQ));

  int16_t l, r;
  next_sample(l, r);

  digitalWrite(MP3_XDCS, LOW);
  PicoSPI0.transfer(l & 0xFF);
  PicoSPI0.transfer(l >> 8);
  PicoSPI0.transfer(r & 0xFF);
  PicoSPI0.transfer(r >> 8);
  digitalWrite(MP3_XDCS, HIGH);

}