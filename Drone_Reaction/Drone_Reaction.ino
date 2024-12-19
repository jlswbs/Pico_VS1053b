//  VS1053b PCM mode stereo 8bit 22050 Hz - 1D Diffusion-Reaction noise oscillation drone //

#include "hardware/structs/rosc.h"
#include "PicoSPI.h"

/*

  Pot1 = rule morph A
  Pot2 = rule morph B
  Pot3 = rule morph C
 
  Created by JLS 2024

*/

#define MP3_CLK   2
#define MP3_MOSI  3
#define MP3_MISO  4
#define MP3_CS    5
#define MP3_XDCS  17
#define MP3_DREQ  26
#define MP3_RST   13

#define SCI_MODE          0x00
#define SCI_STATUS        0x01
#define SCI_BASS          0x02
#define SCI_CLOCKF        0x03
#define SCI_DECODE_TIME   0x04
#define SCI_AUDATA        0x05
#define SCI_WRAM          0x06
#define SCI_WRAMADDR      0x07
#define SCI_HDAT0         0x08
#define SCI_HDAT1         0x09
#define SCI_AIADDR        0x0A
#define SCI_VOL           0x0B
#define SCI_AICTRL0       0x0C
#define SCI_AICTRL1       0x0D
#define SCI_AICTRL2       0x0E
#define SCI_AICTRL3       0x0F

#define SM_DIFF           0x00
#define SM_LAYER12        0x01
#define SM_RESET          0x02
#define SM_CANCEL         0x03
#define SM_EARSPEAKER_LO  0x04
#define SM_TESTS          0x05
#define SM_STREAM         0x06
#define SM_EARSPEAKER_HI  0x07
#define SM_DACT           0x08
#define SM_SDIORD         0x09
#define SM_SDISHARE       0x0A
#define SM_SDINEW         0x0B
#define SM_ADPCM          0x0C
#define SM_ADPCM_HP       0x0D
#define SM_LINE1          0x0E
#define SM_CLK_RANGE      0x0F

#define SAMPLE_RATE 22050

#define MAXADC    4095  // max ADC value
#define MINADC    0     // min ADC value
#define WIDTH     256   // reaction buffer

  float A[WIDTH]; 
  float I[WIDTH];
  float D2A[WIDTH]; 
  float D2I[WIDTH];
  float p[6];
  float dt = 0.05f;

class Trand {

  public: Trand();

};

Trand::Trand() {
  
  uint32_t random = 0;
  uint32_t random_bit;
  volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

  for (int k = 0; k < 32; k++) {
    while (1) {
      random_bit = (*rnd_reg) & 1;
      if (random_bit != ((*rnd_reg) & 1)) break;
    }

    random = (random << 1) | random_bit;
  }
  
  srand(random);

}

Trand Trand;

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}

void rndrule(){

  for(int k=0;k<WIDTH; k++) {
    
    A[k] = randomf(0.0f, 1.0f);
    I[k] = randomf(0.0f, 1.0f);
    
  }

}

void WriteReg(unsigned char address, unsigned char highbyte, unsigned char lowbyte){
  
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS, LOW);
  PicoSPI0.transfer(0x02);
  PicoSPI0.transfer(address);
  PicoSPI0.transfer(highbyte);
  PicoSPI0.transfer(lowbyte);
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS, HIGH);
  
}

void WriteReg16(unsigned char address, unsigned int databyte){
  
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS, LOW);
  PicoSPI0.transfer(0x02);
  PicoSPI0.transfer(address);
  PicoSPI0.transfer((databyte >> 8) & 0xFF);
  PicoSPI0.transfer(databyte & 0xFF);
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS, HIGH);
  
}

unsigned int ReadReg(unsigned char address){
  
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS, LOW);
  PicoSPI0.transfer(0x03);
  PicoSPI0.transfer(address);
  char response1 = PicoSPI0.transfer(0xFF);
  while(!digitalRead(MP3_DREQ));
  char response2 = PicoSPI0.transfer(0xFF);
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS, HIGH);
  int resultvalue = response1 << 8;
  resultvalue |= response2;
  return resultvalue;
  
}

const uint8_t wav_header[44] = {

  0x52, 0x49, 0x46, 0x46,  // RIFF
  0xFF, 0xFF, 0xFF, 0xFF,  // size
  0x57, 0x41, 0x56, 0x45,  // WAVE
  0x66, 0x6d, 0x74, 0x20,  // fmt
  0x10, 0x00, 0x00, 0x00,  // subchunk1size
  0x01, 0x00,              // audio format - pcm
  0x02, 0x00,              // numof channels - stereo
  0x22, 0x56, 0x00, 0x00,  // samplerate - 22050
  0x44, 0xac, 0x00, 0x00,  // byterate = samplerate * channels * blocks
  0x02, 0x00,              // blockalign = channels * blocks
  0x08, 0x00,              // bits per sample - 8
  0x64, 0x61, 0x74, 0x61,  // subchunk3id -"data"
  0xFF, 0xFF, 0xFF, 0xFF   // subchunk3size (endless)

};

void load_header() {

  for (int i = 0; i < sizeof(wav_header); i++) {

    uint8_t p = wav_header[i];
    while (!digitalRead(MP3_DREQ)) {}
    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(p);
    digitalWrite(MP3_XDCS, HIGH);

  }

}

void setup(){

  analogReadResolution(12);
    
  pinMode(MP3_DREQ, INPUT);
  pinMode(MP3_CS, OUTPUT);
  pinMode(MP3_XDCS, OUTPUT);
  pinMode(MP3_RST, OUTPUT);

  PicoSPI0.configure (MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 4000000, 0, true);
  PicoSPI0.transfer(0xFF);

  digitalWrite(MP3_RST, HIGH);
  digitalWrite(MP3_CS, HIGH);
  digitalWrite(MP3_XDCS, HIGH);

  load_header();

  WriteReg16(SCI_CLOCKF, 0x6000);    // set multiplier to 3.0x
  WriteReg16(SCI_VOL, 0x4F4F);       // set volume

  PicoSPI0.configure (MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
  PicoSPI0.transfer(0xFF);

  WriteReg16(SCI_AUDATA, 22050 / 2);  // set samplerate to like lo-fi distortion

  rndrule();

}

void loop(){

  while (!digitalRead(MP3_DREQ)) {}

  for(int k=1; k<WIDTH-1; k++) {
      
    D2A[k] = A[k-1] + A[k+1] - 2.0f * A[k];
    D2I[k] = I[k-1] + I[k+1] - 2.0f * I[k];
 
  }

  for(int i=0; i<WIDTH; i++) {
    
    A[i] = A[i] + dt * (5.0f * A[i] * A[i] * A[i] / (I[i] * I[i]) + p[0] - p[1] * A[i] + p[2] * D2A[i]);
    I[i] = I[i] + dt * (A[i] * A[i] * A[i] - p[3] * I[i] + p[4] * D2I[i] + p[5]);

    uint8_t l = 255.0f - (50.0f * A[i]);
    uint8_t r = 255.0f - (50.0f * I[i]);

    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(l);
    PicoSPI0.transfer(r);
    digitalWrite(MP3_XDCS, HIGH);
 
  }

}

void loop1(){

  float pa = map(analogRead(A1), MINADC, MAXADC, 0, 9999);
  pa /= 10000.0f;
  p[0] = pa;

  float pb = map(analogRead(A2), MINADC, MAXADC, 0, 149999);
  pb /= 10000.0f;
  p[1] = pb;

  float pc = map(analogRead(A3), MINADC, MAXADC, 39999, 0);
  pc /= 10000.0f;
  p[2] = pc;

  float pd = map(analogRead(A3), MINADC, MAXADC, 0, 149999);
  pd /= 10000.0f;
  p[3] = pd;

  float pe = map(analogRead(A2), MINADC, MAXADC, 0, 39999);
  pe /= 10000.0f;
  p[4] = pe;

  float pf = map(analogRead(A1), MINADC, MAXADC, 0, 19999);
  pf /= 10000.0f;
  p[5] = pf;

}