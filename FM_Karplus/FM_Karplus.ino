// FM-Karplus modulated engine //

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
#define MAX_DELAY   1024
#define BPM         120

float freq1 = 220.0f;
float freq2 = 1000.0f;
float mod_index = 0.95f;
float cutoff = 0.999f;
float feedback = 0.95f;

int delay_len = 0;
float prev_sample = 0.0;
unsigned long n_samples = 0;
float phase = 0.0;
float ks_buf[MAX_DELAY];
int ks_index = 0;
float ks_index_f = 0.0f;

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
  srand(seed);
  randomSeed(seed);
}

const uint8_t wav_header[44] = {
  0x52,0x49,0x46,0x46,0xFF,0xFF,0xFF,0xFF,0x57,0x41,0x56,0x45,
  0x66,0x6d,0x74,0x20,0x10,0x00,0x00,0x00,0x01,0x00,0x02,0x00,
  0x22,0x56,0x00,0x00,0x44,0xac,0x00,0x00,0x02,0x00,0x08,0x00,
  0x64,0x61,0x74,0x61,0xFF,0xFF,0xFF,0xFF
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

void init_fm_karplus(float f1, float f2, float mod_idx) {

    freq1 = f1;
    freq2 = f2;
    mod_index = mod_idx;

    delay_len = int(SAMPLE_RATE / freq2);
    if (delay_len > MAX_DELAY) delay_len = MAX_DELAY;

    for (int i = 0; i < delay_len; i++) {
        ks_buf[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    }

    ks_index = 0;
    ks_index_f = 0.0f;
    prev_sample = 0.0f;
    phase = 0.0f;
    n_samples = 0;

}

float karplus_modulator() {

    int i0 = int(ks_index_f);
    int i1 = (i0 + 1) % delay_len;
    float frac = ks_index_f - i0;
    float cur = ks_buf[i0] * (1 - frac) + ks_buf[i1] * frac;

    float new_sample = cutoff * (cur + prev_sample) * 0.5f;
    ks_buf[i0] = new_sample * feedback;
    prev_sample = new_sample;

    ks_index_f += (freq2 / SAMPLE_RATE) * delay_len;
    if (ks_index_f >= delay_len) ks_index_f -= delay_len;

    return cur;
}

float fm_karplus_sample() {

    float mod = karplus_modulator();
    float phase_inc = 2.0f * 3.14159265f * freq1 / SAMPLE_RATE;
    phase += phase_inc * (1.0f + mod_index * mod);

    if (phase > 2.0f * 3.14159265f) phase -= 2.0f * 3.14159265f;

    return sinf(phase);

}


void setup() {

    seed_random_safe();

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

}

void loop() {

    while(!digitalRead(MP3_DREQ)) {}

    uint8_t sample = 128 + 127.0f * fm_karplus_sample();

    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(sample);
    PicoSPI0.transfer(sample);
    digitalWrite(MP3_XDCS, HIGH);

}

void setup1() {

  seed_random_safe();

}

void loop1() {

    freq1 = random(220, 880);
    freq2 = random(50, 440);

    init_fm_karplus(freq1, freq2, mod_index);

    int tempo = 60000 / BPM;
    delay(tempo / 3);

}