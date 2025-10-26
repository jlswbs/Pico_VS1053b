// VS1053 DSP MIDI stream hack - polyphony and tempo //

#include "hardware/structs/rosc.h"
#include "PicoSPI.h"

#define MP3_CLK   2
#define MP3_MOSI  3
#define MP3_MISO  4
#define MP3_CS    5
#define MP3_XDCS  17
#define MP3_DREQ  26
#define MP3_RST   13

#define TICKS_PER_QUARTER 96
#define BPM 480

#define NUM_CHANNELS 4
#define POLY_PER_CHANNEL 3

#define MIN_NOTE_TICKS 0
#define MAX_NOTE_TICKS 24
#define DELTA_BETWEEN_BLOCKS 24

uint8_t channelPrograms[NUM_CHANNELS] = {0, 10, 16, 20};
uint8_t channelPan[NUM_CHANNELS]      = {32, 64, 96, 64};

void midiSendByte(uint8_t b){
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_XDCS, LOW);
  PicoSPI0.transfer(b);
  digitalWrite(MP3_XDCS, HIGH);
}

void midiSendVLQ(uint32_t value){
  uint8_t buffer[4];
  int i=0;
  buffer[i++] = value & 0x7F;
  while(value >>= 7){
    buffer[i++] = 0x80 | (value & 0x7F);
  }
  for(int j=i-1;j>=0;j--) midiSendByte(buffer[j]);
}

void sendMIDIHeader(){

  const uint8_t header[] = {
    'M','T','h','d',0x00,0x00,0x00,0x06,
    0x00,0x00,0x00,0x01,
    0x00,0x60,
    'M','T','r','k',
    0x00,0xFF,0xFF,0xFF
  };

  for(size_t i=0;i<sizeof(header);i++) midiSendByte(header[i]);

  uint32_t tempo_us = 60000000 / BPM;
  midiSendByte(0x00);
  midiSendByte(0xFF);
  midiSendByte(0x51);
  midiSendByte(0x03);
  midiSendByte((tempo_us >> 16) & 0xFF);
  midiSendByte((tempo_us >> 8) & 0xFF);
  midiSendByte(tempo_us & 0xFF);

  for(uint8_t ch=0;ch<NUM_CHANNELS;ch++){

    midiSendVLQ(0);
    midiSendByte(0xC0 | ch);
    midiSendByte(channelPrograms[ch]);

    midiSendVLQ(0);
    midiSendByte(0xB0 | ch);
    midiSendByte(10);
    midiSendByte(channelPan[ch]);

    midiSendVLQ(0);
    midiSendByte(0xB0 | ch);
    midiSendByte(91);
    midiSendByte(96);

  }

}

void sendNoteOn(uint8_t ch,uint8_t note,uint8_t vel,uint16_t delta_ticks){
  midiSendVLQ(delta_ticks);
  midiSendByte(0x90 | ch);
  midiSendByte(note);
  midiSendByte(vel);
}

void sendNoteOff(uint8_t ch,uint8_t note,uint8_t vel,uint16_t delta_ticks){
  midiSendVLQ(delta_ticks);
  midiSendByte(0x80 | ch);
  midiSendByte(note);
  midiSendByte(vel);
}

static inline void seed_random_from_rosc(){
  uint32_t random = 0;
  uint32_t random_bit;
  volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
  for(int k=0;k<32;k++){
    while(1){
      random_bit = (*rnd_reg) & 1;
      if(random_bit != ((*rnd_reg) & 1)) break;
    }
    random = (random << 1) | random_bit;
  }
  srand(random);
  randomSeed(random);
}

void WriteReg16(unsigned char address, unsigned int databyte){
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS, LOW);
  PicoSPI0.transfer(0x02);
  PicoSPI0.transfer(address);
  PicoSPI0.transfer((databyte>>8)&0xFF);
  PicoSPI0.transfer(databyte&0xFF);
  while(!digitalRead(MP3_DREQ));
  digitalWrite(MP3_CS,HIGH);
}


void setup(){

  seed_random_from_rosc();
  analogReadResolution(12);

  pinMode(MP3_DREQ, INPUT);
  pinMode(MP3_CS, OUTPUT);
  pinMode(MP3_XDCS, OUTPUT);
  pinMode(MP3_RST, OUTPUT);

  PicoSPI0.configure(MP3_CLK, MP3_MOSI, MP3_MISO, MP3_CS, 4000000, 0, true);
  PicoSPI0.transfer(0xFF);

  digitalWrite(MP3_RST,HIGH);
  digitalWrite(MP3_CS,HIGH);
  digitalWrite(MP3_XDCS,HIGH);

  WriteReg16(0x03,0x6000); // SCI_CLOCKF 3x
  WriteReg16(0x0B,0x3F3F); // SCI_VOL

  sendMIDIHeader();

}

void loop() {

  uint8_t notes[NUM_CHANNELS][POLY_PER_CHANNEL];
  uint16_t lengths[NUM_CHANNELS][POLY_PER_CHANNEL];

  for(uint8_t ch=0; ch<NUM_CHANNELS; ch++){
    for(uint8_t i=0;i<POLY_PER_CHANNEL;i++){
      notes[ch][i] = random(36,96) + i*2;
      lengths[ch][i] = random(MIN_NOTE_TICKS, MAX_NOTE_TICKS);
    }
  }

  for(uint8_t ch=0; ch<NUM_CHANNELS; ch++){
    for(uint8_t i=0;i<POLY_PER_CHANNEL;i++){
      sendNoteOn(ch, notes[ch][i], random(48,108), (ch==0 && i==0) ? DELTA_BETWEEN_BLOCKS : 0);
    }
  }

  for(uint8_t ch=0; ch<NUM_CHANNELS; ch++){
    for(uint8_t i=0;i<POLY_PER_CHANNEL;i++){
      sendNoteOff(ch, notes[ch][i], 0, lengths[ch][i]);
    }
  }

}