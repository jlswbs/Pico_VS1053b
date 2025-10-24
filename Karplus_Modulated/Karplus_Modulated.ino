// Karplus-Strong with modulation //

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

#define BURST_SAMPLES (SAMPLE_RATE * 0.01f)
#define KS_BUFFER_MAX 200

struct KarplusStrong {
  float buffer[KS_BUFFER_MAX];
  uint32_t write_idx;
  float buffer_size;
  float base_size;
  float mod_depth;
  float mod_freq;
  float mod_phase;
  float damp_factor;
  KarplusStrong(float freq, float mod_depth = 5.0f, float mod_freq = 7.0f, float damp_factor = 0.995f) 
    : write_idx(0), base_size(SAMPLE_RATE / freq), mod_depth(mod_depth), mod_freq(mod_freq), mod_phase(0.0f), damp_factor(damp_factor) {
    for (int i = 0; i < KS_BUFFER_MAX; i++) buffer[i] = random_float() * 0.5f;
  }
  float process(bool trigger) {
    if (trigger) {
      for (int i = 0; i < KS_BUFFER_MAX; i++) buffer[i] = random_float() * 0.5f;
    }
    mod_phase += 2 * M_PI * mod_freq / SAMPLE_RATE;
    if (mod_phase > 2 * M_PI) mod_phase -= 2 * M_PI;
    buffer_size = base_size + mod_depth * sin(mod_phase);
    uint32_t read_idx = (write_idx + KS_BUFFER_MAX - (uint32_t)buffer_size) % KS_BUFFER_MAX;
    uint32_t read_idx_next = (read_idx + 1) % KS_BUFFER_MAX;
    float output = 0.98f * (buffer[read_idx] + buffer[read_idx_next]) / 2.0f;
    buffer[write_idx] = output * damp_factor;
    write_idx = (write_idx + 1) % KS_BUFFER_MAX;
    return output;
  }
  float random_float() {
    return (float)(rand() % 2000 - 1000) / 1000.0f;
  }
};

KarplusStrong* ks_synth;
volatile bool burst_active = false;
volatile uint32_t burst_count = 0;

float random_float() {
  return (float)(rand() % 2000 - 1000) / 1000.0f;
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
  srand(seed);
  randomSeed(seed);
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

  WriteReg16(0x03, 0x9800);
  WriteReg16(0x0B, 0x3F3F);
  PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
  PicoSPI0.transfer(0xFF);

  ks_synth = new KarplusStrong(0.0f, 0.0f, 0.0f, 0.995f);

}

void setup1() {

  seed_random_safe();

}

void loop() {

  while(!digitalRead(MP3_DREQ));

  float output = ks_synth->process(burst_active);
  if (burst_active) {
    burst_count++;
    if (burst_count >= BURST_SAMPLES) {
      burst_active = false;
    }
  }

  int16_t l = 65535.0f * output;
  int16_t r = l;

  digitalWrite(MP3_XDCS, LOW);
  PicoSPI0.transfer(l & 0xFF);
  PicoSPI0.transfer(l >> 8);
  PicoSPI0.transfer(r & 0xFF);
  PicoSPI0.transfer(r >> 8);
  digitalWrite(MP3_XDCS, HIGH);

}

void loop1() {

  burst_active = true;
  burst_count = 0;

  float ks_freqs = random(40, 880);
  ks_synth->base_size = SAMPLE_RATE / ks_freqs;
  ks_synth->mod_depth = 10;
  ks_synth->mod_freq = random (1, 800);

  int tempo = 60000 / BPM;
  delay(tempo / 3);

}