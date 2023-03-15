// 2413 FM sound chip emulator - tetris + reverb //

#include "hardware/structs/rosc.h"
#include "PicoSPI.h"
#include "emu2413.h"

#define MSX_CLK 3579545
#define SAMPLERATE 44100

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

#define SAMPLE_RATE       22050

const int FIXED_BITS = 14;
const int FIXED_SCALE = (1 << FIXED_BITS);
const int REVERB_SIZE = 0x2000;
const int REVERB_LENGTH = (int)(REVERB_SIZE * 0.9f);
const int REVERB_DECAY = (int)(FIXED_SCALE * 0.7f);

int16_t reverbAddr;
int reverbBuffer[REVERB_SIZE];

static inline void seed_random_from_rosc(){
  
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

#define PLUGIN_SIZE  840

const static word plugin[PLUGIN_SIZE] PROGMEM = { /* Compressed plugin */
  0x0007,0x0001, /*copy 1*/
	0x8d00,
	0x0006,0x033c, /*copy 828*/
	0x2803,0x87c0,0x0000,0x0024,0x3e12,0xb817,0x3e12,0x7808,
	0x3e11,0xb811,0x3e15,0x7810,0x3e18,0xb823,0x3e18,0x3821,
	0x3e10,0x3801,0x48b2,0x0024,0x3e10,0x3801,0x3e11,0x3802,
	0x3009,0x3814,0x2903,0x7200,0x0000,0x0024,0x3009,0x1bd4,
	0x36f1,0x1802,0x36f0,0x1801,0x2210,0x0000,0x36f0,0x1801,
	0x36f8,0x1821,0x36f8,0x9823,0x36f5,0x5810,0x36f1,0x9811,
	0x36f2,0x5808,0x3602,0x8024,0x0030,0x0717,0x2100,0x0000,
	0x3f05,0xdbd7,0x3613,0x0024,0x3e12,0xb817,0x3e12,0x3815,
	0x3e05,0xb814,0x3615,0x0024,0x0000,0x800a,0x3e10,0xb813,
	0xb884,0x4493,0x3a00,0x8c4c,0x4484,0x2c50,0x3b14,0x0024,
	0x3b10,0x7841,0x3b10,0x3800,0x3b11,0x0024,0x2914,0xb200,
	0x3b04,0x4024,0x4084,0x4493,0x3373,0x1bc0,0x3b00,0x9bc1,
	0x36f0,0x9813,0x3405,0x9014,0x36f3,0x0024,0x36f2,0x1815,
	0x2000,0x0000,0x36f2,0x9817,0x3613,0x0024,0x3e22,0xb815,
	0x3e05,0xb814,0x3615,0x0024,0x0000,0x800a,0x3e10,0x3801,
	0x3e11,0x3810,0x0006,0x1850,0x3e14,0x7812,0x0006,0x1891,
	0x3e04,0xc024,0x3001,0x0024,0x3100,0x0024,0x6408,0x0024,
	0x4488,0x0024,0x0000,0x3000,0x2803,0x52d9,0x0000,0x0024,
	0x0000,0x4000,0x4408,0x0024,0x0000,0x3000,0x6400,0x0024,
	0x0006,0x1812,0x2803,0x55d8,0x0000,0x8001,0x0006,0x1912,
	0x0000,0x0000,0x0030,0x0693,0x3a00,0x0024,0x0006,0x1812,
	0x3300,0x0024,0xc012,0x0024,0x3b00,0x4024,0x3200,0x0024,
	0x4080,0x0024,0x0000,0x0800,0x2803,0x5985,0x6400,0x0024,
	0x0030,0x0653,0x3200,0x0024,0x2803,0x58c9,0x3300,0x4024,
	0xc100,0x0024,0x2803,0x5980,0x3b00,0x0024,0xd090,0x0024,
	0xb100,0x0024,0x3b00,0x0024,0x0030,0x0313,0x4490,0x0024,
	0x3b00,0x0024,0x36f4,0xc024,0x36f4,0x5812,0x36f1,0x1810,
	0x36f0,0x1801,0x3405,0x9014,0x36e3,0x0024,0x2000,0x0000,
	0x36f2,0x9815,0x3613,0x0024,0x3e12,0xb817,0x3e12,0x3815,
	0x3e05,0xb814,0x3615,0x0024,0x0000,0x800a,0x3e10,0xb803,
	0x0000,0x00c2,0x3e11,0x3805,0x3e14,0x7812,0x6124,0x3813,
	0x0000,0x0182,0x2803,0x6415,0x003b,0x0003,0x0030,0x0351,
	0x0006,0x1893,0x3100,0x8024,0x3304,0x8024,0x3300,0xa802,
	0x6294,0x0024,0x6238,0x2c02,0x0000,0x0024,0x2903,0x4d91,
	0x0003,0x6388,0x0000,0x4003,0x3300,0x8024,0x6236,0x0024,
	0x2903,0x4d80,0x3b00,0xc024,0x2803,0x6fc0,0x0000,0x0024,
	0x6124,0x0024,0x0000,0x0102,0x2803,0x6595,0x0000,0x0024,
	0x2903,0x4d80,0x0003,0x6fc8,0x6124,0x0024,0x0000,0x3fc2,
	0x2803,0x6fd5,0x0030,0x02d1,0x0000,0x0103,0x3131,0x0024,
	0x3101,0x4024,0xb52a,0x0024,0xb424,0x0024,0x6236,0x0024,
	0x0001,0x0003,0x2803,0x6a51,0x003f,0xc002,0xb424,0x184c,
	0x6236,0x0024,0x0000,0x3fc3,0x2803,0x6b41,0xb434,0x0024,
	0x36f3,0x0024,0x0000,0x3fc3,0x0001,0x0104,0x31d3,0x184c,
	0xb434,0x2404,0xb536,0x0024,0x4234,0x0024,0x0000,0x0303,
	0x6236,0x0024,0x0000,0x0243,0x2803,0x6e81,0x4534,0x0024,
	0x0000,0x0305,0x0000,0x3fc2,0x0030,0x0391,0xb424,0x0024,
	0x652a,0x0024,0x4534,0x2405,0x0006,0x18d1,0x2912,0x1400,
	0x3009,0x3840,0x4084,0x9bc0,0x3900,0x8024,0x36f4,0xc024,
	0x36f4,0x5812,0x36f1,0x1805,0x36f0,0x9803,0x3405,0x9014,
	0x36f3,0x0024,0x36f2,0x1815,0x2000,0x0000,0x36f2,0x9817,
	0x3613,0x0024,0x3e12,0xb817,0x3e12,0x3815,0x3e05,0xb814,
	0x3615,0x0024,0x0000,0x800a,0x3e10,0x3801,0x3e10,0xb803,
	0x3e11,0x3810,0x3e14,0x7812,0x0006,0x1851,0x3e04,0xc024,
	0x3104,0x0024,0x2912,0x1300,0x3009,0x0004,0x0006,0x1610,
	0x3000,0x0024,0x4080,0x0024,0x003b,0x0001,0x2803,0x8115,
	0x0000,0x0024,0x3104,0x804c,0x3004,0xc844,0xf400,0x4480,
	0x0006,0x1890,0x6014,0x2c04,0xf400,0x4480,0x2803,0x79d1,
	0x003f,0xc013,0x3283,0x0024,0xf400,0x4480,0x3000,0x4024,
	0x6014,0x0024,0x0006,0x1913,0x2803,0x8055,0xb880,0x0024,
	0x0000,0xfa01,0x3300,0x0024,0x6090,0x0024,0x6012,0x2c00,
	0x001f,0xae02,0x2803,0x7f09,0x003f,0x7fc1,0x0030,0x0690,
	0x3000,0x0024,0xb012,0x0024,0xb880,0x2001,0x0030,0x1290,
	0x3810,0x0024,0x3800,0x0024,0x2912,0x1180,0x0003,0x8548,
	0xfe28,0x0410,0x48b6,0x0024,0x4dd6,0x0024,0x2803,0x8100,
	0x3009,0x2003,0x3b00,0x0024,0x2903,0x4d80,0x3904,0x8024,
	0x0006,0x1710,0x3000,0x0024,0x4090,0x0024,0x0030,0x1293,
	0x2803,0x83c9,0x0006,0x18d2,0x2903,0x9e40,0x0006,0x1612,
	0xf400,0x4004,0x0006,0x18d2,0x0030,0x1293,0x3200,0x0024,
	0xfe08,0x0024,0x48b2,0x0024,0x4180,0x2c41,0x2912,0x1180,
	0x3b00,0x4024,0x36f4,0xc024,0x36f4,0x5812,0x36f1,0x1810,
	0x36f0,0x9803,0x36f0,0x1801,0x3405,0x9014,0x36f3,0x0024,
	0x36f2,0x1815,0x2000,0x0000,0x36f2,0x9817,0x3613,0x0024,
	0x3e12,0xb817,0x3e12,0x3815,0x3e05,0xb814,0x3615,0x0024,
	0x0000,0x800a,0x3e10,0x3801,0x3e10,0xb803,0x3e11,0x3805,
	0x3e14,0x3811,0x0006,0x1811,0x0030,0x0350,0x3e04,0xb813,
	0x003a,0xc012,0x0006,0x1893,0xb882,0x0000,0x0006,0x1850,
	0x3900,0x0024,0x0000,0x0880,0x3b04,0x8024,0x3804,0x8024,
	0x2914,0xbec0,0x000d,0x6010,0x0030,0x0310,0x0015,0x8881,
	0x3000,0x0024,0x6012,0x0024,0x000a,0xc441,0x2803,0x90d1,
	0x0006,0x1612,0x0000,0x0881,0x0000,0x0004,0x000d,0x6010,
	0x6890,0x4111,0x2903,0x4740,0x0003,0x94c8,0x3000,0x0024,
	0x6012,0x0024,0x0000,0x00c0,0x2803,0x93d1,0x0014,0xe651,
	0x0014,0xffd1,0x0000,0x0881,0x000d,0x6010,0x0000,0x0080,
	0x0000,0x1004,0x2903,0x4740,0x0003,0x94c8,0x0000,0x0881,
	0x000d,0x6010,0x2903,0x4740,0x0000,0x1984,0x0006,0xc610,
	0x0001,0xf402,0x0030,0x0311,0x0006,0x1712,0x3100,0x4003,
	0xff26,0x0800,0x48b6,0x0024,0xff02,0x0024,0x2915,0x8600,
	0x48b2,0x0024,0x0004,0x0001,0x4090,0x0024,0x0030,0x1250,
	0x0006,0x1811,0x0030,0x05d2,0x0003,0x5c53,0xc010,0x0024,
	0x3800,0x0024,0x0007,0x9250,0x3100,0x0024,0x3200,0x4024,
	0xc100,0x0024,0x0000,0x0101,0x2903,0x4d80,0x3a00,0x0024,
	0xb880,0x2013,0x2903,0x5c40,0xf400,0x4010,0x36f4,0x9813,
	0x36f4,0x1811,0x36f1,0x1805,0x36f0,0x9803,0x36f0,0x1801,
	0x3405,0x9014,0x36f3,0x0024,0x36f2,0x1815,0x2000,0x0000,
	0x36f2,0x9817,0x3613,0x0024,0x3e12,0xb804,0x3e10,0xf802,
	0x3e15,0x380d,0x3e13,0xf80e,0x3009,0x3813,0xf400,0x4497,
	0x3710,0xc024,0x3755,0x4024,0x37e0,0x0024,0x4030,0x1fd3,
	0xbd86,0x4012,0x3740,0x0024,0x001f,0xffc4,0x4408,0x1e40,
	0x4090,0x4114,0x0001,0x000a,0x2403,0xa300,0xfe25,0x5c28,
	0x5807,0x5c28,0x4db6,0x1d00,0x6090,0x1f04,0xf7d6,0x3c40,
	0x6044,0x1fd5,0x0008,0x0002,0x2803,0xa608,0xf400,0x4500,
	0x4200,0x0024,0xb080,0x4014,0x3f10,0x160c,0x3f05,0x4024,
	0x4380,0x9bd3,0x36f3,0xd80e,0x36f5,0x180d,0x36f0,0xd802,
	0x2000,0x0000,0x36f2,0x9804,
	0x0007,0x0001, /*copy 1*/
	0x8029,
	0x0006,0x0002, /*copy 2*/
	0x2a03,0x408e
};

void load_code(void){
  
  int i = 0;
  while (i< PLUGIN_SIZE) {
    word addr, n, val;
    addr = pgm_read_word (&(plugin[i++]));
    n = pgm_read_word (&(plugin[i++]));
    if (n & 0x8000U) {
      n &= 0x7FFF;
      val = pgm_read_word (&(plugin[i++]));
      while (n--) {
        WriteReg16(addr, val);       
      }
    } 
    else {
      while (n--) {
        val = pgm_read_word (&(plugin[i++]));
        WriteReg16(addr, val);        
      }
    }
  }
  return;
}

OPLL *opll;

int fnum[12] = {172,181,192,204,216,229,242,257,272,288,305,323};

void noteon(int ch, int num, int inst, int vol) {
  // num = 12 - 107 (MIDI Note Numbers)
  // oct = 0 - 7
  int oct;
  if (num >= 12) {num = num - 12;};
  oct = num / 12;
  if (oct >= 8) {oct = 7;};
  num = fnum[num % 12];
  
  // Note(9ch) 0x10 - 15 (0 - 5)
  OPLL_writeReg(opll, (0x10 + ch), num);

  // Inst & Vol(9ch)
  OPLL_writeReg(opll, (0x30 + ch), (inst << 4) | vol);

  // NoteON & Oct & Note
  OPLL_writeReg(opll, (0x20 + ch), (16 | (oct << 1) | (num >> 8)));
}

void noteoff(int ch) {
  // 0x20 - 25 (0 - 5)
  OPLL_writeReg(opll, (0x20 + ch), 0);
}

void drumvol(int bd, int sd, int tom, int tcy, int hh) {
  OPLL_writeReg(opll, 0x36, bd);
  OPLL_writeReg(opll, 0x37, hh << 4 | sd);
  OPLL_writeReg(opll, 0x38, tom << 4 | tcy);
}

void drum(int val) {
  OPLL_writeReg(opll, 0x0E, 0);
  OPLL_writeReg(opll, 0x0E, (val + 32));
}

void setup(){

  seed_random_from_rosc();
  analogReadResolution(12);
  
  pinMode(MP3_DREQ, INPUT);
  pinMode(MP3_CS, OUTPUT);
  pinMode(MP3_XDCS, OUTPUT);
  pinMode(MP3_RST, OUTPUT);

  PicoSPI0.configure (MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 8000000, 0, true);
  PicoSPI0.transfer(0xFF);

  digitalWrite(MP3_RST, HIGH);
  digitalWrite(MP3_CS, HIGH);
  digitalWrite(MP3_XDCS, HIGH);

  load_code();

  WriteReg16(SCI_AICTRL0, SAMPLE_RATE); // sample rate
  //WriteReg16(SCI_AICTRL1, 0x10);      // GPIO4 is high if space for 32 samples
  WriteReg16(SCI_VOL, 0x0000);          // lower volume
  WriteReg16(SCI_AICTRL2, 40);          // set pcm volume
  WriteReg16(SCI_AIADDR, 0x0d00);       // start pcm mixer

  opll = OPLL_new(MSX_CLK, SAMPLERATE);
  OPLL_reset(opll);

  // Drum Volume (BD, SD, TOM, TCY, HH) [0 - 15]
  drumvol(0,0,0,0,0);  

}

void wait() {
  delay(110);
}

void loop(){

 if (ReadReg(SCI_AICTRL0) > 32){

    for (int i = 0; i < 32; i++) {

      int16_t sample = OPLL_calc(opll);

      int reverb = ((int)reverbBuffer[reverbAddr] * REVERB_DECAY) >> FIXED_BITS;
      reverb += sample;
      reverbBuffer[reverbAddr] = reverb;
      reverbAddr++;
      if (reverbAddr > REVERB_LENGTH) reverbAddr = 0;

      int16_t output = sample + (reverbBuffer[reverbAddr]>>3);
      WriteReg16(SCI_AICTRL1, output);
    
    }

  }

}

void loop1(){
  
  //ch 0 : Synth1
  //ch 1 : Synth2
  //ch 2 : Synth3
  //ch 3 : Bass1
  //ch 4 : Bass2
  
  // [1- 1/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,76,10,0); //E6
  noteon(1,64,10,0); //E5
  noteon(2,71,10,0); //B5
  noteon(3,40,13,0); //E3
  noteon(4,40,14,0); //E3
  drum(0B11011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,52,13,0); //E4
  noteon(4,52,14,0); //E4
  drum(0B00001);
  wait();wait();

  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,71,10,0); //B5
  noteon(1,59,10,0); //B4
  noteon(2,68,10,0); //G#5
  noteon(3,40,13,0); //E3
  noteon(4,40,14,0); //E3
  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,69,10,0); //A5
  noteon(3,52,13,0); //E4
  noteon(4,52,14,0); //E4
  drum(0B10001);
  wait();wait();
  
  // [1- 2/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,74,10,0); //D6
  noteon(1,62,10,0); //D5
  noteon(2,71,10,0); //B5
  noteon(3,40,13,0); //E3
  noteon(4,40,14,0); //E3
  drum(0B01011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,52,13,0); //E4
  noteon(4,52,14,0); //E4
  drum(0B10001);
  wait();wait();

  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,69,10,0); //A5
  noteon(3,40,13,0); //E3
  noteon(4,40,14,0); //E3
  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,71,10,0); //B5
  noteon(1,59,10,0); //B4
  noteon(2,68,10,0); //G#5
  noteon(3,52,13,0); //E4
  noteon(4,52,14,0); //E4
  drum(0B00001);
  wait();wait();
  
  // [2- 1/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,69,10,0); //A5
  noteon(1,57,10,0); //A4
  noteon(2,64,10,0); //E5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B11011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,57,13,0); //A4
  noteon(4,57,14,0); //A4
  drum(0B00001);
  wait();wait();

  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,69,10,0); //A5
  noteon(1,57,10,0); //A4
  noteon(2,64,10,0); //E5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,69,10,0); //A5
  noteon(3,57,13,0); //A4
  noteon(4,57,14,0); //A4
  drum(0B10001);
  wait();wait();
  
  // [2- 2/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,76,10,0); //E6
  noteon(1,64,10,0); //E5
  noteon(2,72,10,0); //C6
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B11011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,57,13,0); //A4
  noteon(4,57,14,0); //A4
  drum(0B00001);
  wait();wait();

  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,74,10,0); //D6
  noteon(1,62,10,0); //D5
  noteon(2,71,10,0); //B5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,69,10,0); //A5
  noteon(3,57,13,0); //A4
  noteon(4,57,14,0); //A4
  drum(0B10001);
  wait();wait();
  
  // [3- 1/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,71,10,0); //B5
  noteon(1,59,10,0); //B4
  noteon(2,68,10,0); //G#5
  noteon(3,44,13,0); //G#3
  noteon(4,44,14,0); //G#3
  drum(0B11011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,56,13,0); //G#4
  noteon(4,56,14,0); //G#4
  drum(0B00001);
  wait();wait();

  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,71,10,0); //B5
  noteon(1,59,10,0); //B4
  noteon(2,68,10,0); //G#5
  noteon(3,44,13,0); //G#3
  noteon(4,44,14,0); //G#3
  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,69,10,0); //A5
  noteon(3,56,13,0); //G#4
  noteon(4,56,14,0); //G#4
  drum(0B10001);
  wait();wait();
  
  // [3- 2/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,74,10,0); //D6
  noteon(1,62,10,0); //D5
  noteon(2,71,10,0); //B5
  noteon(3,40,13,0); //E3
  noteon(4,40,14,0); //E3
  drum(0B11011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,52,13,0); //E4
  noteon(4,52,14,0); //E4
  drum(0B00001);
  wait();wait();

  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,76,10,0); //E6
  noteon(1,64,10,0); //E5
  noteon(2,72,10,0); //C6
  noteon(3,40,13,0); //E3
  noteon(4,40,14,0); //E3
  drum(0B01011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,52,13,0); //E4
  noteon(4,52,14,0); //E4
  drum(0B10001);
  wait();wait();
  
  // [4- 1/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,69,10,0); //A5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B11011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,57,13,0); //A4
  noteon(4,57,14,0); //A4
  drum(0B00001);
  wait();wait();

  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,69,10,0); //A5
  noteon(1,57,10,0); //A4
  noteon(2,64,10,0); //E5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B01011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,57,13,0); //A4
  noteon(4,57,14,0); //A4
  drum(0B10001);
  wait();wait();
  
  // [4- 2/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,69,10,0); //A5
  noteon(1,57,10,0); //A4
  noteon(2,64,10,0); //E5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B11011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,57,13,0); //A4
  noteon(4,57,14,0); //A4
  drum(0B00001);
  wait();wait();

  noteoff(3);
  noteoff(4);
  noteon(3,47,13,0); //B3
  noteon(4,47,14,0); //B3
  drum(0B01011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,48,13,0); //C4
  noteon(4,48,14,0); //C4
  drum(0B10011);
  wait();wait();
  
  // [5- 1/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(3,50,13,0); //D4
  noteon(4,50,14,0); //D4
  drum(0B11011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(0,74,10,0); //D6
  noteon(1,62,10,0); //D5
  noteon(2,65,10,0); //F5
  noteon(3,38,13,0); //D3
  noteon(4,38,14,0); //D3
  drum(0B00001);
  wait();wait();

  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,77,10,0); //F6
  noteon(1,65,10,0); //F5
  noteon(2,69,10,0); //A5
  noteon(3,38,13,0); //D3
  noteon(4,38,14,0); //D3
  drum(0B10001);
  wait();
  drum(0B01000);
  wait();
  
  // [5- 2/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteon(0,81,10,0); //A6
  noteon(1,69,10,0); //A5
  noteon(2,72,10,0); //C6
  drum(0B11011);
  wait();
  drum(0B01000);
  wait();
  
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(2,72,10,0); //C6
  noteon(3,38,13,0); //D3
  noteon(4,38,14,0); //D3
  drum(0B00001);
  wait();
  
  noteoff(2);
  noteon(2,72,10,0); //C6
  wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,79,10,0); //G6
  noteon(1,67,10,0); //G5
  noteon(2,71,10,0); //B5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,77,10,0); //F6
  noteon(1,65,10,0); //F5
  noteon(2,69,10,0); //A5
  noteon(3,41,13,0); //F3
  noteon(4,41,14,0); //F3
  drum(0B10001);
  wait();
  drum(0B01000);
  wait();
  
  // [6- 1/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,76,10,0); //E6
  noteon(1,64,10,0); //E5
  noteon(2,67,10,0); //G5
  noteon(3,36,13,0); //C3
  noteon(4,36,14,0); //C3
  drum(0B11011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  
  noteon(3,48,13,0); //C4
  noteon(4,48,14,0); //C4
  drum(0B00001);
  wait();wait();

  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,64,10,0); //E5
  noteon(3,48,13,0); //C4
  noteon(4,48,14,0); //C4
  drum(0B10001);
  wait();
  drum(0B01000);
  wait();
  
  // [6- 2/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,76,10,0); //E6
  noteon(1,64,10,0); //E5
  noteon(2,67,10,0); //G5
  noteon(3,36,13,0); //C3
  noteon(4,36,14,0); //C3
  drum(0B11011);
  wait();
  drum(0B01000);
  wait();
  
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(2,69,10,0); //A5
  noteon(3,43,13,0); //G3
  noteon(4,43,14,0); //G3
  drum(0B00001);
  wait();
  
  noteoff(2);
  noteon(2,67,10,0); //G5
  wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,74,10,0); //D6
  noteon(1,62,10,0); //D5
  noteon(2,65,10,0); //F5
  noteon(3,43,13,0); //G3
  noteon(4,43,14,0); //G3
  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,64,10,0); //E5
  drum(0B10001);
  wait();
  drum(0B01000);
  wait();
  
  // [7- 1/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,71,10,0); //B5
  noteon(1,59,10,0); //B4
  noteon(2,68,10,0); //G#5
  noteon(3,47,13,0); //B3
  noteon(4,47,14,0); //B3
  drum(0B11011);
  wait();wait();
  
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(2,64,10,0); //E5
  noteon(3,59,13,0); //B4
  noteon(4,59,14,0); //B4
  drum(0B00001);
  wait();wait();

  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteon(0,71,10,0); //B5
  noteon(1,59,10,0); //B4
  noteon(2,68,10,0); //G#5
  drum(0B01011);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,69,10,0); //A5
  noteon(3,59,13,0); //B4
  noteon(4,59,14,0); //B4
  drum(0B10001);
  wait();
  drum(0B01000);
  wait();
  
  // [7- 2/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteon(0,74,10,0); //D6
  noteon(1,62,10,0); //D5
  noteon(2,71,10,0); //B5
  drum(0B11011);
  wait();
  drum(0B01000);
  wait();
  
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(2,68,10,0); //G#5
  noteon(3,52,13,0); //E4
  noteon(4,52,14,0); //E4
  drum(0B00001);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteon(0,76,10,0); //E6
  noteon(1,64,10,0); //E5
  noteon(2,72,10,0); //C6
  drum(0B01011);
  wait();wait();
  
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(2,68,10,0); //G#5
  noteon(3,56,13,0); //G#4
  noteon(4,56,14,0); //G#4
  drum(0B10001);
  wait();
  drum(0B01000);
  wait();
  
  // [8- 1/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,72,10,0); //C6
  noteon(1,60,10,0); //C5
  noteon(2,69,10,0); //A5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B11011);
  wait();wait();
  
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(2,64,10,0); //E5
  noteon(3,52,13,0); //E4
  noteon(4,52,14,0); //E4
  drum(0B00001);
  wait();wait();

  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,69,10,0); //A5
  noteon(1,57,10,0); //A4
  noteon(2,64,10,0); //E5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B01011);
  wait();wait();
  
  noteoff(3);
  noteoff(4);
  noteon(3,52,13,0); //E4
  noteon(4,52,14,0); //E4
  drum(0B10001);
  wait();
  drum(0B01000);
  wait();
  
  // [8- 2/2] ------------------------------
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  noteon(0,69,10,0); //A5
  noteon(1,57,10,0); //A4
  noteon(2,64,10,0); //E5
  noteon(3,45,13,0); //A3
  noteon(4,45,14,0); //A3
  drum(0B11011);
  wait();
  drum(0B01000);
  wait();
  
  drum(0B00001);
  wait();wait();
  
  noteoff(0);
  noteoff(1);
  noteoff(2);
  noteoff(3);
  noteoff(4);
  drum(0B01011);
  wait();wait();
  
  drum(0B10001);
  wait();wait();
  
  // end  -----------------------------

}