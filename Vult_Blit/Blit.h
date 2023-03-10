
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#ifndef BLIT_H
#define BLIT_H
#include <stdint.h>
#include <math.h>
#include "vultin.h"
#include "Blit.tables.h"

static_inline fix16_t Blit_nsine_raw_c0(int index){
   return Blit_nsine_c0[index];
};

static_inline fix16_t Blit_nsine_raw_c1(int index){
   return Blit_nsine_c1[index];
};

static_inline fix16_t Blit_nsine_raw_c2(int index){
   return Blit_nsine_c2[index];
};

static_inline fix16_t Blit_nsine(fix16_t x){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x7f0000 /* 127.000000 */,x)),0,127);
   return (fix_wrap_array(Blit_nsine_c0)[index] + fix_mul(x,(fix_wrap_array(Blit_nsine_c1)[index] + fix_mul(x,fix_wrap_array(Blit_nsine_c2)[index]))));
}

typedef struct Blit__ctx_type_4 {
   uint8_t pre;
} Blit__ctx_type_4;

typedef Blit__ctx_type_4 Blit_edge_type;

static_inline void Blit__ctx_type_4_init(Blit__ctx_type_4 &_output_){
   Blit__ctx_type_4 _ctx;
   _ctx.pre = false;
   _output_ = _ctx;
   return ;
}

static_inline void Blit_edge_init(Blit__ctx_type_4 &_output_){
   Blit__ctx_type_4_init(_output_);
   return ;
}

static_inline uint8_t Blit_edge(Blit__ctx_type_4 &_ctx, uint8_t x){
   uint8_t ret;
   ret = (x && bool_not(_ctx.pre));
   _ctx.pre = x;
   return ret;
}

typedef struct Blit__ctx_type_5 {
   fix16_t pre_x;
} Blit__ctx_type_5;

typedef Blit__ctx_type_5 Blit_change_type;

static_inline void Blit__ctx_type_5_init(Blit__ctx_type_5 &_output_){
   Blit__ctx_type_5 _ctx;
   _ctx.pre_x = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

static_inline void Blit_change_init(Blit__ctx_type_5 &_output_){
   Blit__ctx_type_5_init(_output_);
   return ;
}

static_inline uint8_t Blit_change(Blit__ctx_type_5 &_ctx, fix16_t x){
   uint8_t v;
   v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}

typedef struct Blit__ctx_type_6 {
   fix16_t y1;
   fix16_t x1;
} Blit__ctx_type_6;

typedef Blit__ctx_type_6 Blit_dcblock_type;

void Blit__ctx_type_6_init(Blit__ctx_type_6 &_output_);

static_inline void Blit_dcblock_init(Blit__ctx_type_6 &_output_){
   Blit__ctx_type_6_init(_output_);
   return ;
}

fix16_t Blit_dcblock(Blit__ctx_type_6 &_ctx, fix16_t x0);

static_inline fix16_t Blit_pitchToRate_1024_raw_c0(int index){
   return Blit_pitchToRate_1024_c0[index];
};

static_inline fix16_t Blit_pitchToRate_1024_raw_c1(int index){
   return Blit_pitchToRate_1024_c1[index];
};

static_inline fix16_t Blit_pitchToRate_1024_raw_c2(int index){
   return Blit_pitchToRate_1024_c2[index];
};

static_inline fix16_t Blit_pitchToRate_1024(fix16_t pitch){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x3e7c /* 0.244094 */,pitch)),0,31);
   return (fix_wrap_array(Blit_pitchToRate_1024_c0)[index] + fix_mul(pitch,(fix_wrap_array(Blit_pitchToRate_1024_c1)[index] + fix_mul(pitch,fix_wrap_array(Blit_pitchToRate_1024_c2)[index]))));
}

static_inline fix16_t Blit_pitchToRate_raw_c0(int index){
   return Blit_pitchToRate_c0[index];
};

static_inline fix16_t Blit_pitchToRate_raw_c1(int index){
   return Blit_pitchToRate_c1[index];
};

static_inline fix16_t Blit_pitchToRate_raw_c2(int index){
   return Blit_pitchToRate_c2[index];
};

static_inline fix16_t Blit_pitchToRate(fix16_t pitch){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x3e7c /* 0.244094 */,pitch)),0,31);
   return (fix_wrap_array(Blit_pitchToRate_c0)[index] + fix_mul(pitch,(fix_wrap_array(Blit_pitchToRate_c1)[index] + fix_mul(pitch,fix_wrap_array(Blit_pitchToRate_c2)[index]))));
}

static_inline fix16_t Blit_cvToPitch(fix16_t cv){
   return (0x180000 /* 24.000000 */ + fix_mul(0x780000 /* 120.000000 */,cv));
};

static_inline fix16_t Blit_cvToRate_raw_c0(int index){
   return Blit_cvToRate_c0[index];
};

static_inline fix16_t Blit_cvToRate_raw_c1(int index){
   return Blit_cvToRate_c1[index];
};

static_inline fix16_t Blit_cvToRate_raw_c2(int index){
   return Blit_cvToRate_c2[index];
};

static_inline fix16_t Blit_cvToRate(fix16_t cv){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x8d1c71 /* 141.111111 */,cv)),0,127);
   return (fix_wrap_array(Blit_cvToRate_c0)[index] + fix_mul(cv,(fix_wrap_array(Blit_cvToRate_c1)[index] + fix_mul(cv,fix_wrap_array(Blit_cvToRate_c2)[index]))));
}

static_inline fix16_t Blit_cvToRate_1024_raw_c0(int index){
   return Blit_cvToRate_1024_c0[index];
};

static_inline fix16_t Blit_cvToRate_1024_raw_c1(int index){
   return Blit_cvToRate_1024_c1[index];
};

static_inline fix16_t Blit_cvToRate_1024_raw_c2(int index){
   return Blit_cvToRate_1024_c2[index];
};

static_inline fix16_t Blit_cvToRate_1024(fix16_t cv){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x2271c7 /* 34.444444 */,cv)),0,31);
   return (fix_wrap_array(Blit_cvToRate_1024_c0)[index] + fix_mul(cv,(fix_wrap_array(Blit_cvToRate_1024_c1)[index] + fix_mul(cv,fix_wrap_array(Blit_cvToRate_1024_c2)[index]))));
}

static_inline fix16_t Blit_cvToperiod_raw_c0(int index){
   return Blit_cvToperiod_c0[index];
};

static_inline fix16_t Blit_cvToperiod_raw_c1(int index){
   return Blit_cvToperiod_c1[index];
};

static_inline fix16_t Blit_cvToperiod_raw_c2(int index){
   return Blit_cvToperiod_c2[index];
};

static_inline fix16_t Blit_cvToperiod(fix16_t cv){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x1f0000 /* 31.000000 */,cv)),0,31);
   return (fix_wrap_array(Blit_cvToperiod_c0)[index] + fix_mul(cv,(fix_wrap_array(Blit_cvToperiod_c1)[index] + fix_mul(cv,fix_wrap_array(Blit_cvToperiod_c2)[index]))));
}

typedef struct Blit__ctx_type_28 {
   fix16_t rate;
   fix16_t phase;
   Blit__ctx_type_4 _inst3a2;
   Blit__ctx_type_5 _inst16e;
} Blit__ctx_type_28;

typedef Blit__ctx_type_28 Blit_phase_type;

void Blit__ctx_type_28_init(Blit__ctx_type_28 &_output_);

static_inline void Blit_phase_init(Blit__ctx_type_28 &_output_){
   Blit__ctx_type_28_init(_output_);
   return ;
}

fix16_t Blit_phase(Blit__ctx_type_28 &_ctx, fix16_t cv, fix16_t reset);

typedef struct Blit__ctx_type_29 {
   fix16_t scale;
   int m;
   Blit__ctx_type_5 _inst16e;
} Blit__ctx_type_29;

typedef Blit__ctx_type_29 Blit_pulse_train_type;

void Blit__ctx_type_29_init(Blit__ctx_type_29 &_output_);

static_inline void Blit_pulse_train_init(Blit__ctx_type_29 &_output_){
   Blit__ctx_type_29_init(_output_);
   return ;
}

fix16_t Blit_pulse_train(Blit__ctx_type_29 &_ctx, fix16_t cv, fix16_t phase);

typedef Blit__ctx_type_29 Blit_pulse_start_type;

static_inline void Blit_pulse_start_init(Blit__ctx_type_29 &_output_){
   Blit__ctx_type_29_init(_output_);
   return ;
}

static_inline void Blit_pulse_start(Blit__ctx_type_29 &_ctx){
   _ctx.m = 675;
   _ctx.scale = 0x2a33efc /* 675.246035 */;
}

typedef struct Blit__ctx_type_30 {
   fix16_t state_saw;
   fix16_t state_pulse;
   fix16_t rate;
   fix16_t output;
   Blit__ctx_type_29 _inst846;
   Blit__ctx_type_29 _inst746;
   Blit__ctx_type_28 _inst515;
   Blit__ctx_type_5 _inst36e;
   Blit__ctx_type_6 _inst1192;
} Blit__ctx_type_30;

typedef Blit__ctx_type_30 Blit_osc_blit_type;

void Blit__ctx_type_30_init(Blit__ctx_type_30 &_output_);

static_inline void Blit_osc_blit_init(Blit__ctx_type_30 &_output_){
   Blit__ctx_type_30_init(_output_);
   return ;
}

fix16_t Blit_osc_blit(Blit__ctx_type_30 &_ctx, fix16_t cv, fix16_t pw, fix16_t wave);

typedef Blit__ctx_type_30 Blit_init_type;

static_inline void Blit_init_init(Blit__ctx_type_30 &_output_){
   Blit__ctx_type_30_init(_output_);
   return ;
}

static_inline void Blit_init(Blit__ctx_type_30 &_ctx){
   _ctx.rate = 0x30 /* 0.000742 */;
};

typedef struct Blit__ctx_type_31 {
   Blit__ctx_type_30 _inst14c;
} Blit__ctx_type_31;

typedef Blit__ctx_type_31 Blit_process_type;

static_inline void Blit__ctx_type_31_init(Blit__ctx_type_31 &_output_){
   Blit__ctx_type_31 _ctx;
   Blit__ctx_type_30_init(_ctx._inst14c);
   _output_ = _ctx;
   return ;
}

static_inline void Blit_process_init(Blit__ctx_type_31 &_output_){
   Blit__ctx_type_31_init(_output_);
   return ;
}

static_inline fix16_t Blit_process(Blit__ctx_type_31 &_ctx, fix16_t cv, fix16_t pw, fix16_t wave){
   return Blit_osc_blit(_ctx._inst14c,cv,pw,wave);
};



#endif // BLIT_H
