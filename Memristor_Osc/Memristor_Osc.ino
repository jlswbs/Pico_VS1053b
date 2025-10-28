// Multi memristor harmonic analog oscillator //

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

const int memristors = 5;

float R[memristors];
float states[memristors];
float states2[memristors];

const float Rmin = 0.1f;
const float Rmax = 1.0f;
const float k = 0.005f;

#define TANH(x) ((x) * (27.0f + (x)*(x)) / (27.0f + 9.0f*(x)*(x)))

float randomFloat(float minVal, float maxVal) {
  return minVal + (maxVal - minVal) * ((float)random() / (float)RAND_MAX);
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

}

void setup1() {

  seed_random_safe();

}

void loop() {

  while(!digitalRead(MP3_DREQ));

  float sample = 0.0f;

  for(int i=0; i<memristors; i++){

    float x_prev  = states[i];
    float x_prev2 = states2[i];

    float w0 = 1.0f / R[i];
    float w1 = -1.0f;

    float x = TANH(w0*x_prev + w1*x_prev2);

    states2[i] = x_prev;
    states[i]  = x;

    float dR = k * x;
    R[i] += dR;
    if(R[i] < Rmin) R[i] = Rmin;
    if(R[i] > Rmax) R[i] = Rmax;

    sample += x;
  
  }

  sample /= memristors;

  int16_t l = 65535.0f * sample;
  int16_t r = l;

  digitalWrite(MP3_XDCS, LOW);
  PicoSPI0.transfer(l & 0xFF);
  PicoSPI0.transfer(l >> 8);
  PicoSPI0.transfer(r & 0xFF);
  PicoSPI0.transfer(r >> 8);
  digitalWrite(MP3_XDCS, HIGH);

}

void loop1() {

  for(int i=0; i<memristors; i++){

    R[i] = randomFloat(0.53f, 0.85f);
    states[i]  = randomFloat(-0.25f, 0.25f);
    states2[i] = randomFloat(-0.25f, 0.25f);

  }

  int tempo = 60000 / BPM;
  delay(tempo / 3);

}