// MIM resonator (Mass Interaction Model RK2 Heun) - 44100 16bit dual core //

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
#define BPM 120

queue_t sample_fifo;
unsigned long lastTriggerTime = 0;
float gain = 500.0f;

class MIMResonator {
private:
  float x = 0.0f;
  float v = 0.0f;
  float force = 0.0f;
  float dt;
  float m = 1.0f;
  float k_lin = 0.0f;
  float k_nonlin = 0.0f;
  float damping = 50.0f;
  float nonlin = 1e6;

  float getAcceleration(float currentX, float currentV, float currentForce) {
    float f_spring = -(k_lin * currentX + k_nonlin * (currentX * currentX * currentX));
    float f_damping = damping * currentV;
    return (currentForce + f_spring - f_damping) / m;
  }

public:
  MIMResonator(float sampleRate) {
    dt = 1.0f / sampleRate;
  }

  void trigger(float freq) {
    x = 0.0f;
    v = 0.0f;
    force = (float)rand() / RAND_MAX * 1000000.0f + 10000.0f;
    k_lin = m * powf(2.0f * M_PI * freq, 2.0f);
    k_nonlin = k_lin * nonlin;
  }

  float process() {
    float a1 = getAcceleration(x, v, force);
    float x_pred = x + v * dt;
    float v_pred = v + a1 * dt;
    float a2 = getAcceleration(x_pred, v_pred, 0.0f);

    x = x + (v + v_pred) * 0.5f * dt;
    v = v + (a1 + a2) * 0.5f * dt;
    force = 0.0f;

    return x;
  }
};

MIMResonator drum(SAMPLE_RATE);

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
  0x44,0xAC,0x00,0x00, 0x10,0xB1,0x02,0x00, 0x04,0x00,0x10,0x00,
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
  for (int i = 0; i < 44; i++) {
    while(!digitalRead(MP3_DREQ));
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

  queue_init(&sample_fifo, sizeof(int16_t), 128);

  load_header();
  
  WriteReg16(0x03, 0x9800);
  WriteReg16(0x0B, 0x3F3F);

  PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
  PicoSPI0.transfer(0xFF);

}

void loop() {

  if (queue_get_level(&sample_fifo) >= 8) {
    while(!digitalRead(MP3_DREQ));
    digitalWrite(MP3_XDCS, LOW);
    for (int i = 0; i < 8; i++) {
      int16_t out;
      queue_remove_blocking(&sample_fifo, &out);
      PicoSPI0.transfer(out & 0xFF);
      PicoSPI0.transfer(out >> 8);
      PicoSPI0.transfer(out & 0xFF);
      PicoSPI0.transfer(out >> 8);
    }
    digitalWrite(MP3_XDCS, HIGH);
  }

}

void setup1() {

  seed_random_safe();

}

void loop1() {

  float val = tanhf(gain * drum.process());
  int16_t out = (int16_t)(val * 32767.0f);
  queue_add_blocking(&sample_fifo, &out);

  unsigned long currentTime = millis();
  int tempoMs = 60000 / BPM / 3;

  if (currentTime - lastTriggerTime >= tempoMs) {
    lastTriggerTime = currentTime;
    drum.trigger(random(55, 880));
  }

}