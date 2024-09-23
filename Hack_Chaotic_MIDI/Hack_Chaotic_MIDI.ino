// VS1053 DSP MIDI stream hack - Chaotic music generator //

/*

  Based at simplest chaotic equation

  Pot1 = chaos constant
  Pot2 = samplerate
  Pot3 = tempo
 
  Created by JLS 2024

*/

#include "PicoSPI.h"

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

#define MAXADC    4095  // max ADC value
#define MINADC    0     // min ADC value

  float a = 0.0f;
  float x = 0.1f;

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

uint8_t data[] = {
// Header chunk
0x4D,0x54,0x68,0x64,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x01,0x01,0xE0,
// Track chunk
0x4D,0x54,0x72,0x6B,0x00,0x00,0x02,0xC3,
// Midi events
0x00,0xFF,0x51,0x03,0x07,0xA1,0x20,0x00,0xFF,
0x58,0x04,0x04,0x02,0x08,0x08,0x00,0xC0,0x0D,0x00,0x90
};

void setup(){

  analogReadResolution(12);
  
  pinMode(MP3_DREQ, INPUT);
  pinMode(MP3_CS, OUTPUT);
  pinMode(MP3_XDCS, OUTPUT);
  pinMode(MP3_RST, OUTPUT);

  PicoSPI0.configure (MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 4000000, 3, true);
  PicoSPI0.transfer(0xFF);

  digitalWrite(MP3_RST, HIGH);
  digitalWrite(MP3_CS, HIGH);
  digitalWrite(MP3_XDCS, HIGH);

  WriteReg16(SCI_CLOCKF, 0x6000); // set multiplier to 3.0x
  WriteReg16(SCI_VOL, 0x3F3F);    // set volume

  uint8_t *p;
  p = &data[0];
  
  while(p <= &data[sizeof(data) - 1]){
    while(!digitalRead(MP3_DREQ)){}
    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(*p++);
    digitalWrite(MP3_XDCS, HIGH);
  }

}

void loop(){

  uint16_t srate = map(analogRead(A2), MINADC, MAXADC, 44100, 1000);
  WriteReg16(SCI_AUDATA, srate);  // set samplerate   

  a = map(analogRead(A1), MINADC, MAXADC, 12999, 19999);
  a /= 10000.0f;
  
  float nx = x;
  x = a - powf(nx, 2.0f);
  uint8_t p = 64.0f + (32.0f * x);
  
  while(!digitalRead(MP3_DREQ)){}
  digitalWrite(MP3_XDCS, LOW);
  PicoSPI0.transfer(p);    
  digitalWrite(MP3_XDCS, HIGH);
  
  uint16_t del = map(analogRead(A3), MINADC, MAXADC, 120, 1);
  delay(del);

}