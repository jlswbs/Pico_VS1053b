// Neural phase oscillator music engine //

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
#define BPM         90
#define TABLE_SIZE  1024
#define DELAY_SIZE  2048

float waveTable1[TABLE_SIZE];
float waveTable2[TABLE_SIZE];
float waveTable3[TABLE_SIZE];

float delayLine[DELAY_SIZE];
int delayWritePtr = 0;

float phase1 = 0.0f, phaseInc1 = 0.0f, freq1 = 110.0f;
float phase2 = 0.0f, phaseInc2 = 0.0f, freq2 = 220.0f;
float phase3 = 0.0f, phaseInc3 = 0.0f, freq3 = 55.0f;

float w11 = 0.5f, w12 = 0.3f, w13 = 0.2f, wD1 = 0.2f, b1 = 0.0f;
float w21 = 0.2f, w22 = 0.5f, w23 = 0.3f, wD2 = -0.2f, b2 = 0.0f;

float fb1 = 0.0f;
float fb2 = 0.0f;

float v1   = 0.6f;
float v2   = 0.8f;
float bOut = 0.0f;
float outFeedback = 0.4f;

float env1 = 0.0f, env2 = 0.0f, env3 = 0.0f;
float decay1 = 0.0001f;
float decay2 = 0.0004f;
float decay3 = 0.0003f;

float h1_state = 0.0f;
float h2_state = 0.0f;
float out_state = 0.0f;

int scaleArray[4];
uint8_t rhythmPattern[16];
int currentStep = 0;

void triggerEnvelopes(int trig) {
  switch (trig) {
    case 0: env1 = 1.0f; break;
    case 1: env2 = 1.0f; break;
    case 2: env3 = 1.0f; break;
    default: break;
  }
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
  'R','I','F','F', 0x68,0xAC,0x00,0x00, 'W','A','V','E',
  'f','m','t',' ', 0x10,0x00,0x00,0x00, 0x01,0x00,0x02,0x00,
  0x22,0x56,0x00,0x00, 0x44,0xAC,0x00,0x00, 0x02,0x00,0x08,0x00,
  'd','a','t','a', 0x44,0xAC,0x00,0x00
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

  for (int i = 0; i < TABLE_SIZE; i++) {
    float t = (float)i / TABLE_SIZE * 2 * PI;
    float saw = 0.0f;
    for (int harm = 1; harm < 8; harm++) {
        saw += sinf(t * harm) / harm;
    }
    waveTable1[i] = tanhf(saw);
    float tri = (i < TABLE_SIZE / 2) ? (4.0f * i / TABLE_SIZE) - 1.0f : 3.0f - (4.0f * i / TABLE_SIZE);
    waveTable2[i] = sinf(tri * PI * 1.5f);
    waveTable3[i] = sinf(t + 0.3f * cosf(t * 2.0f));
  }

  v1 = (random(400, 800)) / 1000.0f;
  v2 = (random(400, 800)) / 1000.0f;

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
  WriteReg16(0x03, 0x9800);
  WriteReg16(0x0B, 0x4F4F);

  PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
  PicoSPI0.transfer(0xFF);

}

void loop() {

  while(!digitalRead(MP3_DREQ)) {}

  phase1 += phaseInc1; if (phase1 >= TABLE_SIZE) phase1 -= TABLE_SIZE;
  phase2 += phaseInc2; if (phase2 >= TABLE_SIZE) phase2 -= TABLE_SIZE;
  phase3 += phaseInc3; if (phase3 >= TABLE_SIZE) phase3 -= TABLE_SIZE;

  float in1 = waveTable1[(int)phase1 & (TABLE_SIZE - 1)];
  float in2 = waveTable2[(int)phase2 & (TABLE_SIZE - 1)];
  float in3 = waveTable3[(int)phase3 & (TABLE_SIZE - 1)];

  env1 -= decay1; if (env1 < 0.0f) env1 = 0.0f;
  env2 -= decay2; if (env2 < 0.0f) env2 = 0.0f;
  env3 -= decay3; if (env3 < 0.0f) env3 = 0.0f;

  in1 *= env1;
  in2 *= env2;
  in3 *= env3;

  float delayMod = (h1_state - h2_state) * 100.0f;
  float readPtr = (float)delayWritePtr - (DELAY_SIZE * 0.75f) + delayMod;
  if (readPtr < 0.0f) readPtr += DELAY_SIZE;
  if (readPtr >= DELAY_SIZE) readPtr -= DELAY_SIZE;
  
  float delaySample = delayLine[(int)readPtr % DELAY_SIZE];

  float h1 = tanhf(w11*in1 + w12*in2 + w13*in3 + wD1*delaySample + fb1*h1_state + b1);
  float h2 = tanhf(w21*in1 + w22*in2 + w23*in3 + wD2*delaySample + fb2*h2_state + b2);

  h1_state = h1;
  h2_state = h2;

  float outVal = tanhf(v1*h1 + v2*h2 + outFeedback*out_state + bOut);
  out_state = outVal;

  delayLine[delayWritePtr] = tanhf(outVal * 0.5f + h1 * 0.3f + h2 * 0.2f);
  delayWritePtr = (delayWritePtr + 1) % DELAY_SIZE;

  uint8_t sample = (uint8_t)(128.0f * (1.0f + outVal));

  digitalWrite(MP3_XDCS, LOW);
  PicoSPI0.transfer(sample);
  PicoSPI0.transfer(-sample);
  digitalWrite(MP3_XDCS, HIGH);

}

void setup1() {

  seed_random_safe();

  scaleArray[0] = 1;
  scaleArray[1] = 2;
  scaleArray[2] = 3;
  scaleArray[3] = 5;

  for (int i = 0; i < 16; i++) {
    if (random(0, 100) < 40) {
      rhythmPattern[i] = random(0, 3);
    } else {
      rhythmPattern[i] = 255;
    }
  }

}

void loop1() {

  uint8_t currentTrig = rhythmPattern[currentStep];
  if (currentTrig != 255) {
    triggerEnvelopes(currentTrig);
  }
  
  currentStep = (currentStep + 1) % 16;

  triggerEnvelopes(random(0, 3));

  w11 = (random(-1000, 1000)) / 1000.0f;
  w12 = (random(-1000, 1000)) / 1000.0f;
  w13 = (random(-1000, 1000)) / 1000.0f;

  w21 = (random(-1000, 1000)) / 1000.0f;
  w22 = (random(-1000, 1000)) / 1000.0f;
  w23 = (random(-1000, 1000)) / 1000.0f;

  fb1 = random(-400, 400) / 1000.0f;
  fb2 = random(-400, 400) / 1000.0f;

  float base_drone = 110.0f;
  freq1 = base_drone * scaleArray[random(0, 4)];
  freq2 = base_drone * scaleArray[random(0, 4)] * 4.0f;
  freq3 = base_drone * scaleArray[random(0, 4)] * 2.0f;

  phaseInc1 = (freq1 * TABLE_SIZE) / SAMPLE_RATE;
  phaseInc2 = (freq2 * TABLE_SIZE) / SAMPLE_RATE;
  phaseInc3 = (freq3 * TABLE_SIZE) / SAMPLE_RATE;

  int tempo = 60000 / BPM;
  delay(tempo / 4);

}