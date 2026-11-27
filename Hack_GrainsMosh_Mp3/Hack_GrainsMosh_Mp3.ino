// VS1053 DSP mp3 stream hack - Grains datamosh sampler //

/*
  Pot1 = sample position
  Pot2 = samplerate
  Pot3 = sample grains
*/

#include "PicoSPI.h"
#include "sample.h"

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

int findFirstSync() {

  for (int i = 0; i < sizeof(samples) - 2; i++) {
    if (samples[i] == 0xFF && (samples[i+1] & 0xE0) == 0xE0) {
      return i;
    }
  }
  return 0;

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

  WriteReg16(SCI_CLOCKF, 0x6000);   // set multiplier to 3.0x
  WriteReg16(SCI_VOL, 0x3F3F);      // set volume

}

void loop(){

  static uint32_t t;

  uint16_t srate = map(analogRead(A2), MINADC, MAXADC, 44100, 4410);
  WriteReg16(SCI_AUDATA, (srate & 0xFFFE) | 0x0001);

  int firstSyncPos = findFirstSync();

  int numFrames = map(analogRead(A3), MINADC, MAXADC, 2, (sizeof(samples) / 417 / 4) - 1);
  int grains = numFrames * 417;

  int maxTargetPos = sizeof(samples) - grains - 417;
  int targetPos = map(analogRead(A1), MINADC, MAXADC, firstSyncPos, maxTargetPos);

  while (targetPos < sizeof(samples) - 2) {
    if (samples[targetPos] == 0xFF && (samples[targetPos+1] & 0xE0) == 0xE0) {
      break; 
    }
    targetPos++;
  }

  digitalWrite(MP3_XDCS, LOW);
  
  int framePos = 0;
  int frameCount = 0;
  int index = targetPos;

  for (int i = 0; i < grains; i++) {

    if (index >= sizeof(samples)) index = firstSyncPos;
    uint8_t data = samples[index];
    bool moshFrame = ((frameCount % 3) == 0);

    if (moshFrame && framePos > 36 && ((framePos & 0x03) == 0)) {
      data = t + ((data << 2) | (data >> 6));
    }

    while (!digitalRead(MP3_DREQ));
    PicoSPI0.transfer(data);

    index++;
    framePos++;
    if (framePos >= 417) { framePos = 0; frameCount++; }

  }

  digitalWrite(MP3_XDCS, HIGH);
  t++;

}