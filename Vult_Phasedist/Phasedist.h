
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#ifndef PHASEDIST_H
#define PHASEDIST_H
#include <stdint.h>
#include <math.h>
#include "vultin.h"
#include "Phasedist.tables.h"

static_inline fix16_t Phasedist_nsine_raw_c0(int index){
   return Phasedist_nsine_c0[index];
};

static_inline fix16_t Phasedist_nsine_raw_c1(int index){
   return Phasedist_nsine_c1[index];
};

static_inline fix16_t Phasedist_nsine_raw_c2(int index){
   return Phasedist_nsine_c2[index];
};

static_inline fix16_t Phasedist_nsine(fix16_t x){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x7f0000 /* 127.000000 */,x)),0,127);
   return (fix_wrap_array(Phasedist_nsine_c0)[index] + fix_mul(x,(fix_wrap_array(Phasedist_nsine_c1)[index] + fix_mul(x,fix_wrap_array(Phasedist_nsine_c2)[index]))));
}

typedef struct Phasedist__ctx_type_4 {
   uint8_t pre;
} Phasedist__ctx_type_4;

typedef Phasedist__ctx_type_4 Phasedist_edge_type;

static_inline void Phasedist__ctx_type_4_init(Phasedist__ctx_type_4 &_output_){
   Phasedist__ctx_type_4 _ctx;
   _ctx.pre = false;
   _output_ = _ctx;
   return ;
}

static_inline void Phasedist_edge_init(Phasedist__ctx_type_4 &_output_){
   Phasedist__ctx_type_4_init(_output_);
   return ;
}

static_inline uint8_t Phasedist_edge(Phasedist__ctx_type_4 &_ctx, uint8_t x){
   uint8_t ret;
   ret = (x && bool_not(_ctx.pre));
   _ctx.pre = x;
   return ret;
}

typedef struct Phasedist__ctx_type_5 {
   fix16_t pre_x;
} Phasedist__ctx_type_5;

typedef Phasedist__ctx_type_5 Phasedist_change_type;

static_inline void Phasedist__ctx_type_5_init(Phasedist__ctx_type_5 &_output_){
   Phasedist__ctx_type_5 _ctx;
   _ctx.pre_x = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

static_inline void Phasedist_change_init(Phasedist__ctx_type_5 &_output_){
   Phasedist__ctx_type_5_init(_output_);
   return ;
}

static_inline uint8_t Phasedist_change(Phasedist__ctx_type_5 &_ctx, fix16_t x){
   uint8_t v;
   v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}

typedef struct Phasedist__ctx_type_6 {
   fix16_t x;
} Phasedist__ctx_type_6;

typedef Phasedist__ctx_type_6 Phasedist_smooth_type;

static_inline void Phasedist__ctx_type_6_init(Phasedist__ctx_type_6 &_output_){
   Phasedist__ctx_type_6 _ctx;
   _ctx.x = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

static_inline void Phasedist_smooth_init(Phasedist__ctx_type_6 &_output_){
   Phasedist__ctx_type_6_init(_output_);
   return ;
}

static_inline fix16_t Phasedist_smooth(Phasedist__ctx_type_6 &_ctx, fix16_t input){
   _ctx.x = (_ctx.x + fix_mul(0x147 /* 0.005000 */,(input + (- _ctx.x))));
   return _ctx.x;
}

static_inline fix16_t Phasedist_pitchToRate_1024_raw_c0(int index){
   return Phasedist_pitchToRate_1024_c0[index];
};

static_inline fix16_t Phasedist_pitchToRate_1024_raw_c1(int index){
   return Phasedist_pitchToRate_1024_c1[index];
};

static_inline fix16_t Phasedist_pitchToRate_1024_raw_c2(int index){
   return Phasedist_pitchToRate_1024_c2[index];
};

static_inline fix16_t Phasedist_pitchToRate_1024(fix16_t pitch){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x3e7c /* 0.244094 */,pitch)),0,31);
   return (fix_wrap_array(Phasedist_pitchToRate_1024_c0)[index] + fix_mul(pitch,(fix_wrap_array(Phasedist_pitchToRate_1024_c1)[index] + fix_mul(pitch,fix_wrap_array(Phasedist_pitchToRate_1024_c2)[index]))));
}

static_inline fix16_t Phasedist_cvToPitch(fix16_t cv){
   return (0x180000 /* 24.000000 */ + fix_mul(0x780000 /* 120.000000 */,cv));
};

static_inline fix16_t Phasedist_cvToRate_1024_raw_c0(int index){
   return Phasedist_cvToRate_1024_c0[index];
};

static_inline fix16_t Phasedist_cvToRate_1024_raw_c1(int index){
   return Phasedist_cvToRate_1024_c1[index];
};

static_inline fix16_t Phasedist_cvToRate_1024_raw_c2(int index){
   return Phasedist_cvToRate_1024_c2[index];
};

static_inline fix16_t Phasedist_cvToRate_1024(fix16_t cv){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x2271c7 /* 34.444444 */,cv)),0,31);
   return (fix_wrap_array(Phasedist_cvToRate_1024_c0)[index] + fix_mul(cv,(fix_wrap_array(Phasedist_cvToRate_1024_c1)[index] + fix_mul(cv,fix_wrap_array(Phasedist_cvToRate_1024_c2)[index]))));
}

typedef struct Phasedist__ctx_type_16 {
   fix16_t rate;
   fix16_t phase;
   Phasedist__ctx_type_4 _inst3cb;
   Phasedist__ctx_type_5 _inst11a;
} Phasedist__ctx_type_16;

typedef Phasedist__ctx_type_16 Phasedist_phase_type;

void Phasedist__ctx_type_16_init(Phasedist__ctx_type_16 &_output_);

static_inline void Phasedist_phase_init(Phasedist__ctx_type_16 &_output_){
   Phasedist__ctx_type_16_init(_output_);
   return ;
}

fix16_t Phasedist_phase(Phasedist__ctx_type_16 &_ctx, fix16_t cv, fix16_t reset);

typedef struct Phasedist__ctx_type_17 {
   fix16_t pre_phase1;
   Phasedist__ctx_type_16 _inst364;
   Phasedist__ctx_type_6 _inst2b6;
   Phasedist__ctx_type_16 _inst164;
} Phasedist__ctx_type_17;

typedef Phasedist__ctx_type_17 Phasedist_process_type;

void Phasedist__ctx_type_17_init(Phasedist__ctx_type_17 &_output_);

static_inline void Phasedist_process_init(Phasedist__ctx_type_17 &_output_){
   Phasedist__ctx_type_17_init(_output_);
   return ;
}

fix16_t Phasedist_process(Phasedist__ctx_type_17 &_ctx, fix16_t cv, fix16_t detune);



#endif // PHASEDIST_H
