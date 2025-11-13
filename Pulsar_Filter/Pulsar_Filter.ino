// Curtis Roads - Pulsar synthesis + LP filter //

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
#define BPM 120

  bool pulse_mod = false;
  float pulse_density = 40.0f;
  float pulse_len = 0.01f;
  float freq = 440.0f;
  float amp = 0.6f;
  float sigma = 0.5f;
  float lp_freq = 0.0f;

  float pulse_phase = 0.0f;
  float current_time = 0.0f;
  float next_pulse_time = 0.0f;

float lowpass_filter(float x, float cutoff_hz, float fs) {

    static float y_prev = 0.0f;
    float rc = 1.0f / (2.0f * 3.14159265f * cutoff_hz);
    float alpha = 1.0f / (1.0f + rc * fs);

    float y = y_prev + alpha * (x - y_prev);
    y_prev = y;
    return y;

}

float pulsar_sample(float dt) {

  float sample = 0.0f;
  current_time += dt;

  if (current_time >= next_pulse_time) {

    pulse_phase = 0.0f;

    if (pulse_mod) {
      freq = freq * (1.0f + 0.5f * ((float)rand()/RAND_MAX - 0.5f));
      amp = 0.6f + 0.4f * ((float)rand()/RAND_MAX);
    }

    next_pulse_time = current_time + 1.0f / pulse_density;

  }

  if (pulse_phase < pulse_len) {

    float x = (pulse_phase / pulse_len) - 0.5f;
    float env = expf(-0.5f * (x / sigma) * (x / sigma));

    sample = amp * env * sinf(2.0f * 3.14159265f * freq * pulse_phase);
    pulse_phase += dt;

  }

  return sample;

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
    WriteReg16(0x0B, 0x3F3F);

    PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
    PicoSPI0.transfer(0xFF);

}

void loop() {

    while(!digitalRead(MP3_DREQ)) {}

    float dt = 1.0f / (float)SAMPLE_RATE;
    float sample_f = lowpass_filter(pulsar_sample(dt), lp_freq, SAMPLE_RATE);

    uint8_t sample = (uint8_t)(128.0f + 127.0f * sample_f);

    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(sample);
    PicoSPI0.transfer(sample);
    digitalWrite(MP3_XDCS, HIGH);

}

void setup1() {

  seed_random_safe();

}

void loop1() {

  freq = random(880, 3600);
  lp_freq = random(200, 2000);

  int tempo = 60000 / BPM;
  delay(tempo / 3);

}