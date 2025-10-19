// Pulse train oscillator with feedback comb filter //

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

volatile float pulse_freq_hz = 110.0f;
volatile float comb_freq_hz = 440.0f;
volatile int16_t comb_feedback_q15 = 25000;

const int MAX_COMB_DELAY = 8192;
static int32_t comb_buf[MAX_COMB_DELAY];
static int comb_buf_len = 512;
static int comb_write_idx = 0;

static uint32_t pulse_counter = 0;
static uint32_t pulse_period = 200;

static inline int32_t mul_q15(int32_t sample, int32_t q15_gain) {
  int64_t tmp = (int64_t)sample * (int64_t)q15_gain;
  return (int32_t)(tmp >> 15);
}

static inline int16_t clip_s16(int32_t x) {
  if (x > 32767) return 32767;
  if (x < -32768) return -32768;
  return (int16_t)x;
}

void update_dsp_params() {

  int desired_delay = (int)((double)SAMPLE_RATE / (comb_freq_hz <= 1 ? 1 : comb_freq_hz) + 0.5);
  if (desired_delay < 1) desired_delay = 1;
  if (desired_delay > MAX_COMB_DELAY) desired_delay = MAX_COMB_DELAY;
  comb_buf_len = desired_delay;
  for (int i = 0; i < comb_buf_len; i++) comb_buf[i] = 0;
  comb_write_idx = 0;

  pulse_period = (uint32_t)((double)SAMPLE_RATE / (pulse_freq_hz <= 1 ? 1 : pulse_freq_hz));
  if (pulse_period < 1) pulse_period = 1;
  pulse_counter = 0;

}

int16_t dsp_process_frame() {

  int32_t x = 0;
  if (pulse_counter == 0) {
    x = 32767;
  }
  pulse_counter++;
  if (pulse_counter >= pulse_period) pulse_counter = 0;

  int read_idx = comb_write_idx - comb_buf_len;
  if (read_idx < 0) read_idx += comb_buf_len;
  int32_t delayed = comb_buf[read_idx];

  int32_t fb = mul_q15(delayed, comb_feedback_q15);
  int32_t y = x + fb;

  comb_buf[comb_write_idx] = y;
  comb_write_idx++;
  if (comb_write_idx >= comb_buf_len) comb_write_idx = 0;

  return clip_s16(y);

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

  WriteReg16(0x03, 0x9800); // 4.0x multiplier
  WriteReg16(0x0B, 0x3F3F); // volume output internal DAC
  PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
  PicoSPI0.transfer(0xFF);

  update_dsp_params();
  
}

void loop() {

  while(!digitalRead(MP3_DREQ));

  int16_t sample = dsp_process_frame();
  int16_t l = sample;
  int16_t r = sample;

  digitalWrite(MP3_XDCS, LOW);
  PicoSPI0.transfer(l & 0xFF);
  PicoSPI0.transfer(l >> 8);
  PicoSPI0.transfer(r & 0xFF);
  PicoSPI0.transfer(r >> 8);
  digitalWrite(MP3_XDCS, HIGH);

}

void setup1() {

  seed_random_safe();

}

void loop1() {

  pulse_freq_hz = random(60, 400);
  comb_freq_hz  = random(150, 1200);
  
  update_dsp_params();

  int tempo = 60000 / BPM;
  delay(tempo / 3);

}