//  VS1053b PCM mode stereo 8bit 22050 Hz - GameBoy APU emulator //

#include "hardware/structs/rosc.h"
#include "PicoSPI.h"
#include "minigb_apu.c"

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

#define AUDIO_BUFFER_SIZE (AUDIO_SAMPLES * sizeof(u_int32_t))
  
  uint8_t *audio_stream;
  uint16_t freq1, freq2;

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

  audio_stream = (uint8_t *)malloc(AUDIO_BUFFER_SIZE);
  audio_init();
  
  audio_write(0xFF26, 0x80);
  audio_write(0xFF24, 0x77);
  audio_write(0xFF25, 0xFF);
  
}

void loop(){

  while (!digitalRead(MP3_DREQ)) {}

  audio_callback((uint8_t *)audio_stream, AUDIO_BUFFER_SIZE);

  play_ch1(freq1, 128);
  play_ch2(freq2, 196);

  for (int i=0; i<AUDIO_BUFFER_SIZE; i=i+8) {

    uint8_t l = audio_stream[i];
    uint8_t r = audio_stream[i+1];

    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(l);
    PicoSPI0.transfer(r);
    digitalWrite(MP3_XDCS, HIGH);
 
  }

}

void play_ch1(uint16_t freq, uint8_t pw) {

	uint8_t flo, fhi;
	audio_write(0xFF12, 0xA3);	// F=maximum volume, 3=sound fade out
	audio_write(0xFF11, pw);	// 50% square wave
	flo = (uint8_t)freq & 0xFF;
	fhi = (uint8_t)((freq & 0x0700)>>8);
	audio_write(0xFF13, flo);
	audio_write(0xFF14, 0x80 | fhi);

}

void play_ch2(uint16_t freq, uint8_t pw) {

	uint8_t flo, fhi;
	audio_write(0xFF17, 0xF3);  // F=maximum volume, 3=sound fade out
	audio_write(0xFF16, pw);	// 50% square wave
	flo = (uint8_t)freq & 0xFF;
	fhi = (uint8_t)((freq & 0x0700)>>8);
	audio_write(0xFF18, flo);
	audio_write(0xFF19, 0x80 | fhi);

}

void loop1(){

  freq1 = random(44, 2015);
  freq2 = random(44, 2015);

  delay(240);

}