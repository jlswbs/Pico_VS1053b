// 4 channel drum chiptune engine //

/*
- (ch1) Kick
- (ch2) Snare
- (ch3) Hat
- (ch4) Tom
*/

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
#define BPM         120
#define SONG_LENGTH_SEC 30
#define STEPS_PER_BEAT 3

uint32_t sampleIndex = 0;

const uint8_t wav_header[44] = {
  0x52,0x49,0x46,0x46,0xFF,0xFF,0xFF,0xFF,0x57,0x41,0x56,0x45,
  0x66,0x6d,0x74,0x20,0x10,0x00,0x00,0x00,0x01,0x00,0x02,0x00,
  0x22,0x56,0x00,0x00,0x44,0xac,0x00,0x00,0x02,0x00,0x08,0x00,
  0x64,0x61,0x74,0x61,0xFF,0xFF,0xFF,0xFF
};

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
    randomSeed(random);
}

static uint16_t LFSR = 0xACE1u;
static inline uint8_t lfsr_noise_byte() {
    uint16_t bit = ((LFSR>>0) ^ (LFSR>>2) ^ (LFSR>>3) ^ (LFSR>>5)) & 1;
    LFSR = (LFSR >> 1) | (bit << 15);
    return (uint8_t)(LFSR & 0xFF);
}

static inline uint8_t drum_kick(uint32_t t) {
    float pitch = 100.0f * expf(-0.0015f*(float)t);
    if (pitch < 30.0f) pitch = 30.0f;
    float env = expf(-0.005f*(float)t);
    uint8_t s = (t % 2 < 1) ? 200 : 20;
    return (uint8_t)((float)s * env);
}

static inline uint8_t drum_snare(uint32_t t) {
    float env = expf(-0.0035f*(float)t);
    uint8_t n = lfsr_noise_byte();
    uint8_t tone = ((t % 4 < 2) ? 200 : 50) >> 2;
    float outf = ((float)n*0.8f + (float)tone*0.2f) * env;
    return (uint8_t)((outf>255.0f)?255.0f:outf);
}

static inline uint8_t drum_hat(uint32_t t) {
    float env = expf(-0.01f*(float)t);
    uint8_t n = lfsr_noise_byte();
    float outf = (float)n * env * 0.6f;
    return (uint8_t)((outf>255.0f)?255.0f:outf);
}

static inline uint8_t drum_tom(uint32_t t) {
    float env = expf(-0.004f*(float)t);
    float pitch = 200.0f * expf(-0.0025f*(float)t);
    float v = sinf(2.0f * 3.1415926f * pitch * ((float)t / SAMPLE_RATE));
    int s = (int)((v * 127.0f + 128.0f) * env);
    if (s < 0) s = 0; if (s > 255) s = 255;
    return (uint8_t)s;
}

struct DrumNote {
    bool trig1;
    bool trig2;
    bool trig3;
    bool trig4;
};

#define NUM_STEPS ((uint32_t)(SONG_LENGTH_SEC * BPM / 60.0f * STEPS_PER_BEAT + 0.5f))
DrumNote songBuffer[NUM_STEPS];
uint32_t samplesPerStep;
uint32_t currentStepIndex = 0;

struct DrumChannel {
    bool active;
    uint32_t phase;
    uint8_t type;
};
DrumChannel ch[4];

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

    seed_random_from_rosc();
    
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
    WriteReg16(0x03, 0x6000);
    WriteReg16(0x0B, 0x4F4F);
    PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 16000000, 0, true);
    PicoSPI0.transfer(0xFF);

    for(uint32_t i=0; i<NUM_STEPS; i++) {
        songBuffer[i].trig1 = (random(0,100) < 35);
        songBuffer[i].trig2 = (random(0,100) < 25);
        songBuffer[i].trig3 = (random(0,100) < 45);
        songBuffer[i].trig4 = (random(0,100) < 20);
    }

    for(int i=0; i<4; i++){ ch[i].active=false; ch[i].phase=0; ch[i].type=i+1; }
    currentStepIndex = 0;
    samplesPerStep = (uint32_t)(SAMPLE_RATE * 60.0f / BPM / STEPS_PER_BEAT + 0.5f);

}

void loop() {

    while(!digitalRead(MP3_DREQ)) {}

    uint8_t mixL=0, mixR=0;

    for(int i=0;i<4;i++){
        if(ch[i].active){
            uint8_t outv=0;
            if(ch[i].type==1) outv=drum_kick(ch[i].phase);
            else if(ch[i].type==2) outv=drum_snare(ch[i].phase);
            else if(ch[i].type==3) outv=drum_hat(ch[i].phase);
            else if(ch[i].type==4) outv=drum_tom(ch[i].phase);
            ch[i].phase++;
            if(ch[i].phase > SAMPLE_RATE/8) ch[i].active=false;
            uint8_t panL=(i<2)?200:100;
            uint8_t panR=(i<2)?100:200;
            mixL += (outv*panL)>>8;
            mixR += (outv*panR)>>8;
        }
    }

    if(mixL>255) mixL=255;
    if(mixR>255) mixR=255;

    digitalWrite(MP3_XDCS, LOW);
    PicoSPI0.transfer(mixL);
    PicoSPI0.transfer(mixR);
    digitalWrite(MP3_XDCS, HIGH);

    sampleIndex++;
    if(sampleIndex >= samplesPerStep){
        sampleIndex = 0;
        currentStepIndex++;
        if(currentStepIndex >= NUM_STEPS) currentStepIndex = 0;

        DrumNote &n = songBuffer[currentStepIndex];
        if(n.trig1){ ch[0].active=true; ch[0].phase=0; }
        if(n.trig2){ ch[1].active=true; ch[1].phase=0; }
        if(n.trig3){ ch[2].active=true; ch[2].phase=0; }
        if(n.trig4){ ch[3].active=true; ch[3].phase=0; }
    }

}