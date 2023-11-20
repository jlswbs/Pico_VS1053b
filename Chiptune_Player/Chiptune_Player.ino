// VLSI 1053 codec PCM mixer - Chiptune player //
// http://www.linusakesson.net/hardware/chiptune.php //

#include "hardware/structs/rosc.h"
#include "PicoSPI.h"
#include "track.h"

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

#define SAMPLE_RATE 8000
#define FREQ        50
#define TRACKLEN    32

uint8_t trackwait;
uint8_t trackpos;
uint8_t playsong;
uint8_t songpos;

uint32_t noiseseed = 1;

uint8_t readsongbyte(uint16_t offset)
{
    return songdata[offset];
}

const uint16_t freqtable[] = {
	0x0085, 0x008d, 0x0096, 0x009f, 0x00a8, 0x00b2, 0x00bd, 0x00c8, 0x00d4,
	0x00e1, 0x00ee, 0x00fc, 0x010b, 0x011b, 0x012c, 0x013e, 0x0151, 0x0165,
	0x017a, 0x0191, 0x01a9, 0x01c2, 0x01dd, 0x01f9, 0x0217, 0x0237, 0x0259,
	0x027d, 0x02a3, 0x02cb, 0x02f5, 0x0322, 0x0352, 0x0385, 0x03ba, 0x03f3,
	0x042f, 0x046f, 0x04b2, 0x04fa, 0x0546, 0x0596, 0x05eb, 0x0645, 0x06a5,
	0x070a, 0x0775, 0x07e6, 0x085f, 0x08de, 0x0965, 0x09f4, 0x0a8c, 0x0b2c,
	0x0bd6, 0x0c8b, 0x0d4a, 0x0e14, 0x0eea, 0x0fcd, 0x10be, 0x11bd, 0x12cb,
	0x13e9, 0x1518, 0x1659, 0x17ad, 0x1916, 0x1a94, 0x1c28, 0x1dd5, 0x1f9b,
	0x217c, 0x237a, 0x2596, 0x27d3, 0x2a31, 0x2cb3, 0x2f5b, 0x322c, 0x3528,
	0x3851, 0x3bab, 0x3f37
};

const int8_t sinetable[] =
{
    0, 12, 25, 37, 49, 60, 71, 81, 90, 98, 106, 112, 117, 122, 125, 126,
    127, 126, 125, 122, 117, 112, 106, 98, 90, 81, 71, 60, 49, 37, 25, 12,
    0, -12, -25, -37, -49, -60, -71, -81, -90, -98, -106, -112, -117, -122,
    -125, -126, -127, -126, -125, -122, -117, -112, -106, -98, -90, -81,
    -71, -60, -49, -37, -25, -12
};

const uint8_t validcmds[] = "0dfijlmtvw~+=";

enum
{
    WF_TRI,
    WF_SAW,
    WF_PUL,
    WF_NOI
};

volatile struct oscillator
{
    uint16_t     freq;
    uint16_t     phase;
    uint16_t     duty;
    uint8_t      waveform;
    uint8_t      volume; // 0-255
} osc[4];

struct trackline
{
    uint8_t      note;
    uint8_t      instr;
    uint8_t      cmd[2];
    uint8_t      param[2];
};

struct track
{
    struct trackline        line[TRACKLEN];
};

struct unpacker
{
    uint16_t     nextbyte;
    uint8_t      buffer;
    uint8_t      bits;
};

struct channel
{
    struct unpacker         trackup;
    uint8_t                      tnum;
    int8_t                      transp;
    uint8_t                      tnote;
    uint8_t                      lastinstr;
    uint8_t                      inum;
    uint16_t                     iptr;
    uint8_t                      iwait;
    uint8_t                      inote;
    int8_t                      bendd;
    int16_t                     bend;
    int8_t                      volumed;
    int16_t                     dutyd;
    uint8_t                      vdepth;
    uint8_t                      vrate;
    uint8_t                      vpos;
    int16_t                     inertia;
    uint16_t                     slur;
} channel[4];

uint16_t resources[16 + MAXTRACK];

struct unpacker songup;


void initup(struct unpacker *up, uint16_t offset)
{
    up->nextbyte = offset;
    up->bits = 0;
}

uint8_t readbit(struct unpacker *up)
{
    uint8_t val;

    if(!up->bits)
    {
        up->buffer = readsongbyte(up->nextbyte++);
        up->bits = 8;
    }

    up->bits--;
    val = up->buffer & 1;
    up->buffer >>= 1;

    return val;
}

uint16_t readchunk(struct unpacker *up, uint8_t n)
{
    uint16_t val = 0;
    uint8_t i;

    for(i = 0; i < n; i++)
    {
        if(readbit(up))
        {
            val |= (1 << i);
        }
    }

    return val;
}

void readinstr(uint8_t num, uint8_t pos, uint8_t *dest)
{
    dest[0] = readsongbyte(resources[num] + 2 * pos + 0);
    dest[1] = readsongbyte(resources[num] + 2 * pos + 1);
}

void runcmd(uint8_t ch, uint8_t cmd, uint8_t param)
{
    switch(validcmds[cmd])
    {
    case '0':
        channel[ch].inum = 0;
        break;
    case 'd':
        osc[ch].duty = param << 8;
        break;
    case 'f':
        channel[ch].volumed = param;
        break;
    case 'i':
        channel[ch].inertia = param << 1;
        break;
    case 'j':
        channel[ch].iptr = param;
        break;
    case 'l':
        channel[ch].bendd = param;
        break;
    case 'm':
        channel[ch].dutyd = param << 6;
        break;
    case 't':
        channel[ch].iwait = param;
        break;
    case 'v':
        osc[ch].volume = param;
        break;
    case 'w':
        osc[ch].waveform = param;
        break;
    case '+':
        channel[ch].inote = param + channel[ch].tnote - 12 * 4;
        break;
    case '=':
        channel[ch].inote = param;
        break;
    case '~':
        if(channel[ch].vdepth != (param >> 4))
        {
            channel[ch].vpos = 0;
        }
        channel[ch].vdepth = param >> 4;
        channel[ch].vrate = param & 15;
        break;
    }
}

void playroutine()                      // called at 50 Hz
{
    uint8_t ch;

    if(playsong)
    {
        if(trackwait)
        {
            trackwait--;
        }
        else
        {
            trackwait = 4;

            if(!trackpos)
            {
                if(playsong)
                {
                    if(songpos >= SONGLEN)
                    {
                        playsong = 0;
                    }
                    else
                    {
                        for(ch = 0; ch < 4; ch++)
                        {
                            uint8_t gottransp;
                            uint8_t transp;

                            gottransp = readchunk(&songup, 1);
                            channel[ch].tnum = readchunk(&songup, 6);
                            if(gottransp)
                            {
                                transp = readchunk(&songup, 4);
                                if(transp & 0x8) transp |= 0xf0;
                            }
                            else
                            {
                                transp = 0;
                            }
                            channel[ch].transp = (int8_t) transp;
                            if(channel[ch].tnum)
                            {
                                initup(&channel[ch].trackup, resources[16 + channel[ch].tnum - 1]);
                            }
                        }
                        songpos++;
                    }
                }
            }

            if(playsong)
            {
                for(ch = 0; ch < 4; ch++)
                {
                    if(channel[ch].tnum)
                    {
                        uint8_t note, instr, cmd, param;
                        uint8_t fields;

                        fields = readchunk(&channel[ch].trackup, 3);
                        note = 0;
                        instr = 0;
                        cmd = 0;
                        param = 0;
                        if(fields & 1) note = readchunk(&channel[ch].trackup, 7);
                        if(fields & 2) instr = readchunk(&channel[ch].trackup, 4);
                        if(fields & 4)
                        {
                            cmd = readchunk(&channel[ch].trackup, 4);
                            param = readchunk(&channel[ch].trackup, 8);
                        }
                        if(note)
                        {
                            channel[ch].tnote = note + channel[ch].transp;
                            if(!instr) instr = channel[ch].lastinstr;
                        }
                        if(instr)
                        {
                          channel[ch].lastinstr = instr;
                          channel[ch].inum = instr;
                          channel[ch].iptr = 0;
                          channel[ch].iwait = 0;
                          channel[ch].bend = 0;
                          channel[ch].bendd = 0;
                          channel[ch].volumed = 0;
                          channel[ch].dutyd = 0;
                          channel[ch].vdepth = 0;
                        }
                        if(cmd) runcmd(ch, cmd, param);
                    }
                }

                trackpos++;
                trackpos &= 31;
            }
        }
    }

    for(ch = 0; ch < 4; ch++)
    {
        int16_t vol;
        uint16_t duty;
        uint16_t slur;

        while(channel[ch].inum && !channel[ch].iwait)
        {
            uint8_t il[2];

            readinstr(channel[ch].inum, channel[ch].iptr, il);
            channel[ch].iptr++;

            runcmd(ch, il[0], il[1]);
        }
        if(channel[ch].iwait) channel[ch].iwait--;

        if(channel[ch].inertia)
        {
            int16_t diff;

            slur = channel[ch].slur;
            diff = freqtable[channel[ch].inote] - slur;
            //diff >>= channel[ch].inertia;
            if(diff > 0)
            {
                if(diff > channel[ch].inertia) diff = channel[ch].inertia;
            }
            else if(diff < 0)
            {
                if(diff < -channel[ch].inertia) diff = -channel[ch].inertia;
            }
            slur += diff;
            channel[ch].slur = slur;
        }
        else
        {
            slur = freqtable[channel[ch].inote];
        }
        osc[ch].freq =
            slur +
            channel[ch].bend +
            ((channel[ch].vdepth * sinetable[channel[ch].vpos & 63]) >> 2);
        channel[ch].bend += channel[ch].bendd;
        vol = osc[ch].volume + channel[ch].volumed;
        if(vol < 0) vol = 0;
        if(vol > 255) vol = 255;
        osc[ch].volume = vol;

        duty = osc[ch].duty + channel[ch].dutyd;
        if(duty > 0xe000) duty = 0x2000;
        if(duty < 0x2000) duty = 0xe000;
        osc[ch].duty = duty;

        channel[ch].vpos += channel[ch].vrate;
    }

}

void initresources()
{
    uint8_t i;
    struct unpacker up;

    initup(&up, 0);
    for(i = 0; i < 16 + MAXTRACK; i++)
    {
        resources[i] = readchunk(&up, 13);
    }

    initup(&songup, resources[0]);
}

int16_t chiptune_callback()        // called at 8 KHz
{

    uint8_t newbit = 0;
    if(noiseseed & 0x80000000L) newbit ^= 1;
    if(noiseseed & 0x01000000L) newbit ^= 1;
    if(noiseseed & 0x00000040L) newbit ^= 1;
    if(noiseseed & 0x00000200L) newbit ^= 1;
    noiseseed = (noiseseed << 1) | newbit;

    int16_t acc = 0;
    for(int i = 0; i < 4; i++)
    {
        int8_t value; // [-32,31]

        switch(osc[i].waveform)
        {
        case WF_TRI:
            if(osc[i].phase < 0x8000)
            {
                value = -32 + (osc[i].phase >> 9);
            }
            else
            {
                value = 31 - ((osc[i].phase - 0x8000) >> 9);
            }
            break;
        case WF_SAW:
            value = -32 + (osc[i].phase >> 10);
            break;
        case WF_PUL:
            value = (osc[i].phase > osc[i].duty)? -32 : 31;
            break;
        case WF_NOI:
            value = (noiseseed & 63) - 32;
            break;
        default:
            value = 0;
            break;
        }
        osc[i].phase += osc[i].freq;

        acc += value * osc[i].volume; // rhs = [-8160,7905]
    }

    return acc >> 1;

}

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

  WriteReg16(SCI_AICTRL0, SAMPLE_RATE);   // sample rate
  //WriteReg16(SCI_AICTRL1, 0x10);  // GPIO4 is high if space for 32 samples
  WriteReg16(SCI_VOL, 0x0000);      // lower volume
  WriteReg16(SCI_AICTRL2, 60);      // set pcm volume
  WriteReg16(SCI_AIADDR, 0x0d00);   // start pcm mixer

  trackwait = 0;
  trackpos = 0;
  playsong = 1;
  songpos = 0;

  osc[0].volume = 0;
  channel[0].inum = 0;
  osc[1].volume = 0;
  channel[1].inum = 0;
  osc[2].volume = 0;
  channel[2].inum = 0;
  osc[3].volume = 0;
  channel[3].inum = 0;

}

void loop(){

  int16_t sample = chiptune_callback();
  if (ReadReg(SCI_AICTRL0) > 32) WriteReg16(SCI_AICTRL1, sample);

}

void loop1(){

  playroutine();

  int time = 1000000 / FREQ;
  delayMicroseconds(time);

}