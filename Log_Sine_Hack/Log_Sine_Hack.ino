// VS1053 DSP - logistic sine generator //

/*

  Based at Logistic equation

  Pot1 = chaos constant
  Pot2 = samplerate
  Pot3 = tempo
 
  Created by JLS 2023

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

  float r = 0.0f;
  float x = 0.1f;
  float coef = 1.4865; // Coefficient for frequency  

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

void SetVolume(unsigned char left, unsigned char right){ WriteReg(SCI_VOL, left, right); }

void setup(){

  analogReadResolution(12);
  
  pinMode(MP3_DREQ, INPUT);
  pinMode(MP3_CS, OUTPUT);
  pinMode(MP3_XDCS, OUTPUT);
  pinMode(MP3_RST, OUTPUT);

  PicoSPI0.configure (MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 1000000, 0, true);
  PicoSPI0.transfer(0xFF);

  digitalWrite(MP3_RST, HIGH);
  digitalWrite(MP3_CS, HIGH);
  digitalWrite(MP3_XDCS, HIGH);

  SetVolume(100, 100); //Set initial volume (20 = -10dB)

  WriteReg16(0x00,(1<<SM_TESTS)|(1<<SM_SDISHARE)|(1<<SM_SDINEW)|(0<<SM_DIFF)|(0<<SM_CLK_RANGE)|(0<<SM_EARSPEAKER_HI));

  WriteReg16(0x05, 0xAC44); // set sample rate 44100 Hz 
  WriteReg16(0x0A, 0x4020); // run new sine test

}

void loop(){

  uint16_t srate = map(analogRead(A2), MINADC, MAXADC, 1000, 44100);

  WriteReg16(SCI_AUDATA, srate);  // set samplerate  

  r = map(analogRead(A1), MINADC, MAXADC, 34999, 39999);
  r /= 10000.0f;  

  float nx = x;
  x = r * nx * (1.0f - nx);
  uint16_t p = 8000.0f * x;

  WriteReg16(0x0C, p * coef); // left channel frequency
  WriteReg16(0x0D, 25 + (p * coef)); // right channel frequency

  uint16_t del = map(analogRead(A3), MINADC, MAXADC, 120, 1);
  delay(del);  
  
}
