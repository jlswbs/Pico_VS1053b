// Band pass delayline node percussor //

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

#define NUM_FILTERS 4
#define BURST_SAMPLES (SAMPLE_RATE * 0.003f)
#define DELAY_SAMPLES_MAX ((uint32_t)(SAMPLE_RATE * 0.2))

struct BandpassFilter {
  float a0, a1, a2, b1, b2;
  float x1, x2, y1, y2;
  float process(float x) {
    float y = a0 * x + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;
    x2 = x1; x1 = x; y2 = y1; y1 = y;
    return y;
  }
};

struct DelayLine {
  float buffer[DELAY_SAMPLES_MAX];
  uint32_t write_idx;
  uint32_t delay_samples;
  DelayLine(uint32_t delay) : write_idx(0), delay_samples(delay) {
    for (int i = 0; i < DELAY_SAMPLES_MAX; i++) buffer[i] = 0.0f;
  }
  float process(float input) {
    uint32_t read_idx = (write_idx - delay_samples + DELAY_SAMPLES_MAX) % DELAY_SAMPLES_MAX;
    float output = buffer[read_idx];
    buffer[write_idx] = input;
    write_idx = (write_idx + 1) % DELAY_SAMPLES_MAX;
    return output;
  }
};

BandpassFilter filters[NUM_FILTERS];
float filter_outputs[NUM_FILTERS];
float delayed_outputs[NUM_FILTERS];
DelayLine* delay_lines[NUM_FILTERS];
float mix_matrix[NUM_FILTERS][NUM_FILTERS];
float input_signal = 0.0f;
volatile bool burst_active = false;
volatile uint32_t burst_count = 0;

void init_bandpass_filter(BandpassFilter &f, float freq, float Q) {
  float w0 = 2 * M_PI * freq / SAMPLE_RATE;
  float alpha = sin(w0) / (2 * Q);
  float cos_w0 = cos(w0);
  float norm = 1 / (1 + alpha);
  f.a0 = alpha * norm;
  f.a1 = 0.0f;
  f.a2 = -alpha * norm;
  f.b1 = -2 * cos_w0 * norm;
  f.b2 = (1 - alpha) * norm;
  f.x1 = f.x2 = f.y1 = f.y2 = 0.0f;
}

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

  WriteReg16(0x03, 0x9800); // 4.0x multiplier
  WriteReg16(0x0B, 0x3F3F); // volume output internal DAC

  PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
  PicoSPI0.transfer(0xFF);

  for (int i = 0; i < NUM_FILTERS; i++) {
    uint32_t delay_ms = random(1, 500);
    uint32_t delay_samples = (uint32_t)(SAMPLE_RATE * delay_ms / 1000.0f);
    delay_lines[i] = new DelayLine(delay_samples);
  }

}

void loop() {

  while(!digitalRead(MP3_DREQ));

  if (burst_active) {
    input_signal = random_float();
    burst_count++;
    if (burst_count >= BURST_SAMPLES) {
      burst_active = false;
      input_signal = 0.0f;
    }
  } else {
    input_signal = 0.0f;
  }

  float filter_inputs[NUM_FILTERS] = {0.0f};
  for (int i = 0; i < NUM_FILTERS; i++) {
    float sum = 0.0f;
    for (int j = 0; j < NUM_FILTERS; j++) {
      sum += mix_matrix[i][j] * delayed_outputs[j];
    }
    filter_inputs[i] = sum + input_signal;
    filter_outputs[i] = filters[i].process(filter_inputs[i]);
    delayed_outputs[i] = delay_lines[i]->process(filter_outputs[i]);
  }

  float output = 0.0f;
  for (int i = 0; i < NUM_FILTERS; i++) {
    output += filter_outputs[i];
  }

  int16_t l = 65535.0f * output;
  int16_t r = -l;

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

  float freqs[NUM_FILTERS] = {240.0f, 400.0f, 480.0f, 560.0f}; 
  float Q = 10.0f;

  for (int i = 0; i < NUM_FILTERS; i++) init_bandpass_filter(filters[i], freqs[i] * random(1, 7), Q);

  for (int i = 0; i < NUM_FILTERS; i++) {
    for (int j = 0; j < NUM_FILTERS; j++) {
      mix_matrix[i][j] = random_float();
    }
  }

  burst_active = true;
  burst_count = 0;

  int tempo = 60000 / BPM;
  delay(tempo / 3);

}