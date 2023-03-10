#include "fmtg.h"
//
// Operator mult code to actual multiplier conversion
//
const PROGMEM uint8_t mult_tab[16] = {
// code:       0   1  2  3  4   5   6   7   8   9  10  11  12  13  14  15
// multiplier: 0.5 1  2  3  4   5   6   7   8   9  10  10  12  12  15  15
               1,  2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 20, 24, 24, 30, 30
}; // const PROGMEM prog_uint8_t mult_tab[]
//
// MIDInote to oct-keycode table
//
const PROGMEM uint8_t octkey[32] = {
    0x00, 0x04, 0x08, 0x10, 0x14, 0x18, 0x20, 0x24, 
    0x28, 0x30, 0x34, 0x38, 0x40, 0x44, 0x48, 0x50, 
    0x54, 0x58, 0x60, 0x64, 0x68, 0x70, 0x74, 0x78, 
    0x80, 0x84, 0x88, 0x90, 0x94, 0x98, 0xa0, 0xa4
}; // const PROGMEM prog_uint8_t octkey[]
//
// 512 point sine table
// only first 129 points (0..pi/2) for symmetry
//
const PROGMEM uint8_t slbtab[129] = {
    0xff, 0x65, 0x55, 0x4c, 0x45, 0x40, 0x3c, 0x38, 
    0x35, 0x32, 0x30, 0x2e, 0x2c, 0x2a, 0x28, 0x27, 
    0x25, 0x24, 0x23, 0x21, 0x20, 0x1f, 0x1e, 0x1d, 
    0x1c, 0x1b, 0x1a, 0x19, 0x19, 0x18, 0x17, 0x16, 
    0x16, 0x15, 0x14, 0x14, 0x13, 0x13, 0x12, 0x11, 
    0x11, 0x10, 0x10, 0x0f, 0x0f, 0x0e, 0x0e, 0x0d, 
    0x0d, 0x0d, 0x0c, 0x0c, 0x0b, 0x0b, 0x0b, 0x0a, 
    0x0a, 0x0a, 0x09, 0x09, 0x09, 0x08, 0x08, 0x08, 
    0x08, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 
    0x05, 0x05, 0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 
    0x04, 0x04, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00
}; // const PROGMEM prog_uint8_t slbtab[]
//
// convert log2 value to linear value (Q15)
// LIN = lb2lin[LB] = 2**(-LB/16)
//
const PROGMEM int16_t lb2lin[256] = {
  0x7f00, 0x799e, 0x7476, 0x6f86, 0x6acb, 0x6644, 0x61ee, 0x5dc7, 
  0x59cd, 0x55ff, 0x5259, 0x4edc, 0x4b84, 0x4850, 0x453f, 0x4250, 
  0x3f80, 0x3ccf, 0x3a3b, 0x37c3, 0x3566, 0x3322, 0x30f7, 0x2ee4, 
  0x2ce7, 0x2aff, 0x292d, 0x276e, 0x25c2, 0x2428, 0x22a0, 0x2128, 
  0x1fc0, 0x1e67, 0x1d1d, 0x1be1, 0x1ab3, 0x1991, 0x187c, 0x1772, 
  0x1673, 0x1580, 0x1496, 0x13b7, 0x12e1, 0x1214, 0x1150, 0x1094, 
  0x0fe0, 0x0f34, 0x0e8f, 0x0df1, 0x0d59, 0x0cc9, 0x0c3e, 0x0bb9, 
  0x0b3a, 0x0ac0, 0x0a4b, 0x09db, 0x0970, 0x090a, 0x08a8, 0x084a, 
  0x07f0, 0x079a, 0x0747, 0x06f8, 0x06ad, 0x0664, 0x061f, 0x05dc, 
  0x059d, 0x0560, 0x0526, 0x04ee, 0x04b8, 0x0485, 0x0454, 0x0425, 
  0x03f8, 0x03cd, 0x03a4, 0x037c, 0x0356, 0x0332, 0x030f, 0x02ee, 
  0x02ce, 0x02b0, 0x0293, 0x0277, 0x025c, 0x0243, 0x022a, 0x0212, 
  0x01fc, 0x01e6, 0x01d2, 0x01be, 0x01ab, 0x0199, 0x0188, 0x0177, 
  0x0167, 0x0158, 0x0149, 0x013b, 0x012e, 0x0121, 0x0115, 0x0109, 
  0x00fe, 0x00f3, 0x00e9, 0x00df, 0x00d6, 0x00cd, 0x00c4, 0x00bc, 
  0x00b4, 0x00ac, 0x00a5, 0x009e, 0x0097, 0x0091, 0x008a, 0x0085, 
  0x007f, 0x007a, 0x0074, 0x0070, 0x006b, 0x0066, 0x0062, 0x005e, 
  0x005a, 0x0056, 0x0052, 0x004f, 0x004c, 0x0048, 0x0045, 0x0042, 
  0x0040, 0x003d, 0x003a, 0x0038, 0x0035, 0x0033, 0x0031, 0x002f, 
  0x002d, 0x002b, 0x0029, 0x0027, 0x0026, 0x0024, 0x0023, 0x0021, 
  0x0020, 0x001e, 0x001d, 0x001c, 0x001b, 0x001a, 0x0018, 0x0017, 
  0x0016, 0x0015, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0011, 
  0x0010, 0x000f, 0x000f, 0x000e, 0x000d, 0x000d, 0x000c, 0x000c, 
  0x000b, 0x000b, 0x000a, 0x000a, 0x0009, 0x0009, 0x0009, 0x0008, 
  0x0008, 0x0008, 0x0007, 0x0007, 0x0007, 0x0006, 0x0006, 0x0006, 
  0x0006, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0004, 0x0004, 
  0x0004, 0x0004, 0x0004, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 
  0x0003, 0x0003, 0x0003, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
  0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0001, 
  0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
  0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
  0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000
}; // const PROGMEM prog_int16_t lb2lin[]
