// Particle grain generator with reverb //

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

#define GRAINS 16
#define GRAIN_LEN 256
#define REVERB_LEN 6400

int16_t grainBuffers[GRAINS][GRAIN_LEN];
int grainPos[GRAINS];
float grainGain[GRAINS];
float grainPan[GRAINS];
bool grainActive[GRAINS];

float combBuf[1200];
int combPos = 0;

float reverbL[REVERB_LEN];
float reverbR[REVERB_LEN];
int revPosL = 0, revPosR = 0;

float tLFO1 = 0, tLFO2 = 0, tLFO3 = 0, tMetal = 0;

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
  for(int i=0;i<sizeof(wav_header);i++){
    uint8_t p=wav_header[i];
    while(!digitalRead(MP3_DREQ)){}
    digitalWrite(MP3_XDCS,LOW);
    PicoSPI0.transfer(p);
    digitalWrite(MP3_XDCS,HIGH);
  }
  for(int i=0;i<512;i++){
    while(!digitalRead(MP3_DREQ)){}
    digitalWrite(MP3_XDCS,LOW);
    PicoSPI0.transfer(0x00);
    digitalWrite(MP3_XDCS,HIGH);
  }
}

void setup() {

  seed_random_safe();

  pinMode(MP3_DREQ,INPUT_PULLDOWN);
  pinMode(MP3_CS,OUTPUT);
  pinMode(MP3_XDCS,OUTPUT);
  pinMode(MP3_RST,OUTPUT);
  PicoSPI0.configure(MP3_CLK,MP3_MOSI,MP3_MISO,MP3_CS,4000000,0,true);
  PicoSPI0.transfer(0xFF);
  digitalWrite(MP3_RST,HIGH);
  digitalWrite(MP3_CS,HIGH);
  digitalWrite(MP3_XDCS,HIGH);
  load_header();
  WriteReg16(0x03,0x9800);
  WriteReg16(0x0B,0x3F3F);
  PicoSPI0.configure(MP3_CLK,MP3_MOSI,MP3_MISO,MP3_CS,16000000,0,true);
  PicoSPI0.transfer(0xFF);

}

void loop() {

  while(!digitalRead(MP3_DREQ));

  float lfo1 = 0.5f + 0.5f * sinf(tLFO1);
  float lfo2 = 0.5f + 0.5f * sinf(tLFO2);
  float lfo3 = 0.5f + 0.5f * sinf(tLFO3);
  tLFO1 += 2.0f * M_PI * 0.07f / SAMPLE_RATE;
  tLFO2 += 2.0f * M_PI * 0.05f / SAMPLE_RATE;
  tLFO3 += 2.0f * M_PI * 0.025f / SAMPLE_RATE;

  float mixL=0,mixR=0;
  for(int g=0;g<GRAINS;g++){
    if(grainActive[g]){
      mixL+=grainBuffers[g][grainPos[g]]*grainGain[g]*(1.0f-grainPan[g]);
      mixR+=grainBuffers[g][grainPos[g]]*grainGain[g]*grainPan[g];
      grainPos[g]++;
      if(grainPos[g]>=GRAIN_LEN)grainActive[g]=false;
    }
  }

  int trig = 400 + (int)(300*lfo2);
  if(random(0,trig)==0){
    int g=random(GRAINS);
    grainActive[g]=true;
    grainGain[g]=random(25,90)/90.0f;
    grainPan[g]=fminf(1.0f,fmaxf(0.0f,lfo3+(random(-30,30)/100.0f)));
    grainPos[g]=0;
    for(int i=0;i<GRAIN_LEN;i++)
      grainBuffers[g][i]=(random(-32768,32767))>>3;
  }

  float metalFM = sinf(tMetal*2.0f)*0.3f + sinf(tMetal*3.0f)*0.15f;
  tMetal += 2.0f * M_PI * 150.0f / SAMPLE_RATE;
  int combShift = 900 + (int)(300*lfo1 + 80*metalFM);
  if(++combPos>=combShift)combPos=0;
  float combOut=combBuf[combPos];
  combBuf[combPos]=(mixL+mixR)*0.5f+combOut*(0.88f+0.05f*lfo1+0.05f*metalFM);

  float metalNoise = ((random(-32768,32767))/32768.0f)*0.08f*metalFM;
  mixL=mixL*0.6f+combOut*0.6f+metalNoise;
  mixR=mixR*0.6f+combOut*0.6f-metalNoise;

  float revInL=mixL,revInR=mixR;
  float revOutL=reverbL[revPosL];
  float revOutR=reverbR[revPosR];
  reverbL[revPosL]=revInL+revOutL*(0.78f+0.15f*lfo2);
  reverbR[revPosR]=revInR+revOutR*(0.78f+0.15f*(1.0f-lfo2));
  if(++revPosL>=REVERB_LEN)revPosL=0;
  if(++revPosR>=REVERB_LEN-421)revPosR=0;

  float outL=(mixL+revOutL*0.95f)*1.8f;
  float outR=(mixR+revOutR*0.95f)*1.8f;
  int16_t l=(int16_t)outL;
  int16_t r=(int16_t)outR;

  digitalWrite(MP3_XDCS,LOW);
  PicoSPI0.transfer(l&0xFF);
  PicoSPI0.transfer(l>>8);
  PicoSPI0.transfer(r&0xFF);
  PicoSPI0.transfer(r>>8);
  digitalWrite(MP3_XDCS,HIGH);

}