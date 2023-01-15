// VLSI 1053 codec PCM mixer - FM synthesizer random melody //

#include "hardware/structs/rosc.h"
#include "PicoSPI.h"
#define SAMPLERATE  22050

const int STATE_ATTACK = 0;
const int STATE_DECAY = 1;
const int STATE_SUSTAIN = 2;
const int STATE_RELEASE = 3;
const int STATE_SLEEP = 4;
const int WAVE_BUFFER_BITS = 8;
const int INT_BITS = 32;
const int FIXED_BITS = 14;
const int FIXED_BITS_ENV = 8;
const int WAVE_ADDR_SHIFT = (INT_BITS - WAVE_BUFFER_BITS);
const int WAVE_ADDR_SHIFT_M = (WAVE_ADDR_SHIFT - FIXED_BITS);
const int FIXED_SCALE = (1 << FIXED_BITS);
const int FIXED_SCALE_M1 = (FIXED_SCALE - 1);
const int WAVE_BUFFER_SIZE = (1 << WAVE_BUFFER_BITS);
const int WAVE_BUFFER_SIZE_M1 = (WAVE_BUFFER_SIZE - 1);
const int OPS = 4;
const int CHANNELS = 4;
const int OSCS = (OPS * CHANNELS);
const int TEMPO = 4096;
const int SEQ_LENGTH = 16;
const int ADD_RATE = 6;
const int DEL_RATE = 12;
const int OCT_MIN = 6;
const int OCT_WIDTH = 4;

const int CONST03 = (1 << FIXED_BITS << FIXED_BITS_ENV);
const int CONST04 = (FIXED_SCALE >> 1);
const int VOLUME = (CONST04 / CHANNELS);
const int REVERB_VOLUME = (int)(VOLUME * 1.4);
const int REVERB_BUFFER_SIZE = 0x2000;
const int REVERB_LENGTH = (int)(REVERB_BUFFER_SIZE * 0.95);
const int REVERB_DECAY = (int)(FIXED_SCALE * 0.7);

typedef struct{
  
  int envelopeLevelA;
  int envelopeLevelS;
  int envelopeDiffA;
  int envelopeDiffD;
  int envelopeDiffR;
  int modPatch0;
  int modPatch1;
  int modLevel0;
  int modLevel1;
  int level;
  int levelRev;
  int state;
  int count;
  int currentLevel;
  int pitch;
  int velocity;
  int mod0;
  int mod1;
  int outData;
  int outWave;
  boolean mixOut;
  boolean noteOn;
  boolean noteOnSave;
  boolean goToggle;
  boolean outDoneToggle;
  
} params_t;

typedef struct{
  unsigned char note;
  unsigned char oct;
} seqData_t;

short waveData[] = {
  0,402,803,1205,1605,2005,2403,2800,
  3196,3589,3980,4369,4755,5139,5519,5896,
  6269,6639,7004,7365,7722,8075,8422,8764,
  9101,9433,9759,10079,10393,10700,11002,11296,
  11584,11865,12139,12405,12664,12915,13158,13394,
  13621,13841,14052,14254,14448,14633,14810,14977,
  15135,15285,15425,15556,15677,15789,15892,15984,
  16068,16141,16205,16259,16304,16338,16363,16378,
  16383,16378,16363,16338,16304,16259,16205,16141,
  16068,15984,15892,15789,15677,15556,15425,15285,
  15135,14977,14810,14633,14448,14254,14052,13841,
  13621,13394,13158,12915,12664,12405,12139,11865,
  11584,11296,11002,10700,10393,10079,9759,9433,
  9101,8764,8422,8075,7722,7365,7004,6639,
  6269,5896,5519,5139,4755,4369,3980,3589,
  3196,2800,2403,2005,1605,1205,803,402,
  0,-403,-804,-1206,-1606,-2006,-2404,-2801,
  -3197,-3590,-3981,-4370,-4756,-5140,-5520,-5897,
  -6270,-6640,-7005,-7366,-7723,-8076,-8423,-8765,
  -9102,-9434,-9760,-10080,-10394,-10701,-11003,-11297,
  -11585,-11866,-12140,-12406,-12665,-12916,-13159,-13395,
  -13622,-13842,-14053,-14255,-14449,-14634,-14811,-14978,
  -15136,-15286,-15426,-15557,-15678,-15790,-15893,-15985,
  -16069,-16142,-16206,-16260,-16305,-16339,-16364,-16379,
  -16383,-16379,-16364,-16339,-16305,-16260,-16206,-16142,
  -16069,-15985,-15893,-15790,-15678,-15557,-15426,-15286,
  -15136,-14978,-14811,-14634,-14449,-14255,-14053,-13842,
  -13622,-13395,-13159,-12916,-12665,-12406,-12140,-11866,
  -11585,-11297,-11003,-10701,-10394,-10080,-9760,-9434,
  -9102,-8765,-8423,-8076,-7723,-7366,-7005,-6640,
  -6270,-5897,-5520,-5140,-4756,-4370,-3981,-3590,
  -3197,-2801,-2404,-2006,-1606,-1206,-804,-403,
};

int scaleTable[] = {
  153791,162936,172624,182889,193764,205286,217493,230426,
  244128,258644,274024,290319,307582,325872,345249,365779,
};

const int CHORD_LENGTH = 5;
const int CHORDS = 3;
unsigned char chordData[CHORDS][8] = {
  {0,2,4,7,9,0,0,0},
  {0,2,4,7,9,0,0,0},
  {0,2,4,7,9,0,0,0}
};
unsigned char progressionData[CHORDS][2] = {
  {0, 3},
  {0, 3},
  {0, 3}
};
unsigned char bassData[CHORDS] = {4, 2, 1};
unsigned char bassPattern[SEQ_LENGTH] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char toneData[CHANNELS][OPS];

params_t params[OSCS];
seqData_t seqData[OSCS][SEQ_LENGTH];
bool activeCh[OSCS];

short reverbBuffer[REVERB_BUFFER_SIZE];

int32_t timer;
int32_t timerNext;
int counter;
int seqCounter;
int barCounter;
int deleteCounter;
int chord;
int note;
int sc;
int reverbAddr;
int out;

void render(){

  int mix = 0;

  for (int i = 0; i < OSCS; i++)
  {
    if (activeCh[i] == true)
    {
      // envelope generator
      switch (params[i].state)
      {
        case STATE_SLEEP:
        {
          break;
        }

        case STATE_ATTACK:
        {
          params[i].currentLevel += params[i].envelopeDiffA;
          if (params[i].currentLevel > params[i].envelopeLevelA)
          {
            params[i].currentLevel = params[i].envelopeLevelA;
            params[i].state = STATE_DECAY;
          }
          break;
        }

        case STATE_DECAY:
        {
          params[i].currentLevel += params[i].envelopeDiffD;
          if (params[i].currentLevel < params[i].envelopeLevelS)
          {
            params[i].currentLevel = params[i].envelopeLevelS;
            params[i].state = STATE_SUSTAIN;
          }
          break;
        }

        case STATE_SUSTAIN:
        {
          break;
        }

        case STATE_RELEASE:
        {
          params[i].currentLevel += params[i].envelopeDiffR;
          if (params[i].currentLevel < 0)
          {
            params[i].currentLevel = 0;
            activeCh[i] = false;
            params[i].state = STATE_SLEEP;
          }
          break;
        }
      }

      params[i].mod0 = params[params[i].modPatch0].outData;
      params[i].mod1 = params[params[i].modPatch1].outData;

      int waveAddr = (unsigned int)(params[i].count +
                                    (params[i].mod0 * params[i].modLevel0) +
                                    (params[i].mod1 * params[i].modLevel1)) >> WAVE_ADDR_SHIFT_M;

      // fetch wave data
      int waveAddrF = waveAddr >> FIXED_BITS;
      int waveAddrR = (waveAddrF + 1) & WAVE_BUFFER_SIZE_M1;    
      int oscOutF = waveData[waveAddrF];
      int oscOutR = waveData[waveAddrR];
      int waveAddrM = waveAddr & FIXED_SCALE_M1;
      int oscOut = ((oscOutF * (FIXED_SCALE - waveAddrM)) >> FIXED_BITS) +
        ((oscOutR * waveAddrM) >> FIXED_BITS);
      params[i].outData = (oscOut * (params[i].currentLevel >> FIXED_BITS_ENV)) >> FIXED_BITS;
      params[i].count += params[i].pitch;

      // mix
      if (params[i].mixOut == 0)
      {
        params[i].outWave = 0;
      }
      else
      {
        params[i].outWave = (params[i].outData * params[i].level) >> FIXED_BITS;
      }

      mix += params[i].outWave;

    }
  }

  // reverb
  int reverb = ((int)reverbBuffer[reverbAddr] * REVERB_DECAY) >> FIXED_BITS;
  reverb += mix;
  reverbBuffer[reverbAddr] = reverb;
  reverbAddr++;
  if (reverbAddr > REVERB_LENGTH) reverbAddr = 0;
  out = mix + reverbBuffer[reverbAddr];

}

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

#define PLUGIN_SIZE 840

const static word plugin[PLUGIN_SIZE] PROGMEM = {
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
	0x2a03,0x408e};


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

void setup(){

  seed_random_from_rosc();
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

  load_code();

  WriteReg16(SCI_AICTRL0, SAMPLERATE); // sample rate
  //WriteReg16(SCI_AICTRL1, 0x10); // GPIO4 is high if space for 32 samples
  WriteReg16(SCI_VOL, 0x0000); // lower volume
  WriteReg16(SCI_AICTRL2, 50); // set pcm volume
  WriteReg16(SCI_AIADDR, 0x0d00); // start pcm mixer

  counter = 0;
  seqCounter = 0;
  barCounter = 0;
  deleteCounter = 0;
  chord = 0;
  note = 0;
  sc = 0;
  reverbAddr = 0;

  for (int i = 0; i < OSCS; i++){
    
    activeCh[i] = false;
    params[i].state = STATE_SLEEP;
    params[i].envelopeLevelA = CONST03;
    params[i].envelopeLevelS = 0;
    params[i].envelopeDiffA = CONST03 >> (4 + rand()%9);
    params[i].envelopeDiffD = (0 - CONST03) >> (13 + rand()%9);
    params[i].envelopeDiffR = (0 - CONST03) >> 13;
    params[i].level = VOLUME;
    params[i].levelRev = REVERB_VOLUME;
    params[i].mixOut = true;
    params[i].modLevel0 = FIXED_SCALE * rand()%5;
    params[i].modPatch0 = i;
    params[i].modPatch1 = i;
    params[i].modLevel1 = 0;
    
  }

  for (int i = 0; i < CHANNELS; i++){
    
    int osc = i * OPS;
    params[osc].modPatch0 = osc + 1;
    params[osc + 1].modPatch0 = osc + 1;
    params[osc + 1].modLevel0 = 0;
    params[osc + 1].mixOut = false;
    
  }
  params[4].level = VOLUME >> 1;
  params[6].level = VOLUME;

  for (int i = 0; i < CHANNELS; i++){
    for (int j = 0; j < OPS; j++) toneData[i][j] = 0;
  }

  for (int i = 0; i < 3; i++) bassPattern[rand()%12] = 1;
  
}

void process_audio(){
  // sequencer
  counter++;
  if (counter > TEMPO)
  {
    counter = 0;
    if (seqCounter >= SEQ_LENGTH)
    {
      seqCounter = 0;
      if ((barCounter & 3) == 0)
      {
        // tone change
        for (int i = 0; i < CHANNELS; i++)
        {
          int osc = i * OPS;
          params[osc].modLevel0 = FIXED_SCALE * rand()%5;
          params[osc + 1].modLevel0 = FIXED_SCALE * rand()%4;
          for (int j = 0; j < OPS; j++)
          {
            osc = i * OPS + j;
            params[osc].envelopeDiffA = CONST03 >> (3 + rand()%10);
            params[osc].envelopeDiffD = -CONST03 >> (13 + rand()%9);
            toneData[i][j] = (rand()%3) - 1;
          }
        }
        // chord change
        chord = (rand()%progressionData[chord][1]) + progressionData[chord][0];
        // delete rate change
        deleteCounter = rand()%DEL_RATE;
      }
      // delete note
      for (int i = 0; i < deleteCounter; i++)
      {
        seqData[rand()%CHANNELS][rand()%SEQ_LENGTH].oct = 0;
      }
      // add note
      for (int i = 0; i < ADD_RATE; i++)
      {
        int ch = rand()%CHANNELS;
        int beat = rand()%SEQ_LENGTH;
        int beat_prev = beat - 1;
        if (beat_prev < 0)
        {
          beat_prev += SEQ_LENGTH;
        }
        seqData[ch][beat].note = rand()%CHORD_LENGTH;
        seqData[ch][beat].oct = (rand()%OCT_WIDTH) + OCT_MIN;
        seqData[ch][beat_prev].oct = 0;
      }
      // add bass
      for (int i = 0; i < SEQ_LENGTH; i++)
      {
        if (bassPattern[i] == 1)
        {
          seqData[0][i].note = bassData[chord];
          seqData[0][i].oct = OCT_MIN;
        }
      }
      barCounter++;
    }
    // note on/off
    for (int i = 0; i < CHANNELS; i++)
    {
      if (seqData[i][seqCounter].oct != 0)
      {
        int n = chordData[chord][seqData[i][seqCounter].note];
        for (int j = 0; j < OPS; j++)
        {
          int osc = i * OPS + j;
          params[osc].pitch = scaleTable[n] << (seqData[i][seqCounter].oct + toneData[i][j]);
          params[osc].state = STATE_ATTACK;
          params[osc].noteOn = true;
          activeCh[osc] = true;
        }
      }
      else
      {
        for (int j = 0; j < OPS; j++)
        {
          int osc = i * OPS + j;
          if (params[osc].noteOn == true)
          {
            params[osc].state = STATE_RELEASE;
            params[osc].noteOn = false;
          }
        }
      }
    }
    seqCounter++;
  }

  render();
  
}

void loop(){

  if (ReadReg(SCI_AICTRL0) > 32) {
    
    for (int k = 0; k < 32; k++) { 

      process_audio();
      WriteReg16(SCI_AICTRL1, out);   

    }
  }

}