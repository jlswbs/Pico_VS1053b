
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#ifndef RESCOMB_H
#define RESCOMB_H
#include <stdint.h>
#include <math.h>
#include "vultin.h"
#include "Rescomb.tables.h"

typedef struct Rescomb__ctx_type_0 {
   fix16_t pre_x;
} Rescomb__ctx_type_0;

typedef Rescomb__ctx_type_0 Rescomb_change_type;

static_inline void Rescomb__ctx_type_0_init(Rescomb__ctx_type_0 &_output_){
   Rescomb__ctx_type_0 _ctx;
   _ctx.pre_x = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

static_inline void Rescomb_change_init(Rescomb__ctx_type_0 &_output_){
   Rescomb__ctx_type_0_init(_output_);
   return ;
}

static_inline uint8_t Rescomb_change(Rescomb__ctx_type_0 &_ctx, fix16_t x){
   uint8_t v;
   v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}

typedef struct Rescomb__ctx_type_1 {
   fix16_t y1;
   fix16_t x1;
} Rescomb__ctx_type_1;

typedef Rescomb__ctx_type_1 Rescomb_dcblock_type;

void Rescomb__ctx_type_1_init(Rescomb__ctx_type_1 &_output_);

static_inline void Rescomb_dcblock_init(Rescomb__ctx_type_1 &_output_){
   Rescomb__ctx_type_1_init(_output_);
   return ;
}

fix16_t Rescomb_dcblock(Rescomb__ctx_type_1 &_ctx, fix16_t x0);

typedef struct Rescomb__ctx_type_2 {
   fix16_t x4;
   fix16_t x3;
   fix16_t x2;
} Rescomb__ctx_type_2;

typedef Rescomb__ctx_type_2 Rescomb_soft_type;

void Rescomb__ctx_type_2_init(Rescomb__ctx_type_2 &_output_);

static_inline void Rescomb_soft_init(Rescomb__ctx_type_2 &_output_){
   Rescomb__ctx_type_2_init(_output_);
   return ;
}

fix16_t Rescomb_soft(Rescomb__ctx_type_2 &_ctx, fix16_t x1);

static_inline fix16_t Rescomb_cvToPitch(fix16_t cv){
   return (0x180000 /* 24.000000 */ + fix_mul(0x780000 /* 120.000000 */,cv));
};

static_inline fix16_t Rescomb_cvToperiod_raw_c0(int index){
   return Rescomb_cvToperiod_c0[index];
};

static_inline fix16_t Rescomb_cvToperiod_raw_c1(int index){
   return Rescomb_cvToperiod_c1[index];
};

static_inline fix16_t Rescomb_cvToperiod_raw_c2(int index){
   return Rescomb_cvToperiod_c2[index];
};

static_inline fix16_t Rescomb_cvToperiod(fix16_t cv){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x1f0000 /* 31.000000 */,cv)),0,31);
   return (fix_wrap_array(Rescomb_cvToperiod_c0)[index] + fix_mul(cv,(fix_wrap_array(Rescomb_cvToperiod_c1)[index] + fix_mul(cv,fix_wrap_array(Rescomb_cvToperiod_c2)[index]))));
}

static_inline fix16_t Rescomb_tanh_table_raw_c0(int index){
   return Rescomb_tanh_table_c0[index];
};

static_inline fix16_t Rescomb_tanh_table_raw_c1(int index){
   return Rescomb_tanh_table_c1[index];
};

static_inline fix16_t Rescomb_tanh_table_raw_c2(int index){
   return Rescomb_tanh_table_c2[index];
};

static_inline fix16_t Rescomb_tanh_table(fix16_t x){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x50000 /* 5.000000 */,(0x180000 /* 24.000000 */ + x))),0,240);
   return (fix_wrap_array(Rescomb_tanh_table_c0)[index] + fix_mul(x,(fix_wrap_array(Rescomb_tanh_table_c1)[index] + fix_mul(x,fix_wrap_array(Rescomb_tanh_table_c2)[index]))));
}

static_inline fix16_t Rescomb_saturate_soft(fix16_t x){
   return Rescomb_tanh_table(x);
};

static_inline fix16_t Rescomb_tanh_table2_raw_c0(int index){
   return Rescomb_tanh_table2_c0[index];
};

static_inline fix16_t Rescomb_tanh_table2_raw_c1(int index){
   return Rescomb_tanh_table2_c1[index];
};

static_inline fix16_t Rescomb_tanh_table2_raw_c2(int index){
   return Rescomb_tanh_table2_c2[index];
};

static_inline fix16_t Rescomb_tanh_table2(fix16_t x){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x50000 /* 5.000000 */,(0x180000 /* 24.000000 */ + x))),0,240);
   return (fix_wrap_array(Rescomb_tanh_table2_c0)[index] + fix_mul(x,(fix_wrap_array(Rescomb_tanh_table2_c1)[index] + fix_mul(x,fix_wrap_array(Rescomb_tanh_table2_c2)[index]))));
}

static_inline fix16_t Rescomb_saturate(fix16_t x){
   return Rescomb_tanh_table2(x);
};

typedef struct Rescomb__ctx_type_18 {
   int write_pos;
   fix16_t buffer[1000];
} Rescomb__ctx_type_18;

typedef Rescomb__ctx_type_18 Rescomb_delayx_type;

void Rescomb__ctx_type_18_init(Rescomb__ctx_type_18 &_output_);

static_inline void Rescomb_delayx_init(Rescomb__ctx_type_18 &_output_){
   Rescomb__ctx_type_18_init(_output_);
   return ;
}

fix16_t Rescomb_delayx(Rescomb__ctx_type_18 &_ctx, fix16_t x, fix16_t cv);

static_inline fix16_t Rescomb_toneCurve_raw_c0(int index){
   return Rescomb_toneCurve_c0[index];
};

static_inline fix16_t Rescomb_toneCurve_raw_c1(int index){
   return Rescomb_toneCurve_c1[index];
};

static_inline fix16_t Rescomb_toneCurve_raw_c2(int index){
   return Rescomb_toneCurve_c2[index];
};

static_inline fix16_t Rescomb_toneCurve(fix16_t tone){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x1a4000 /* 26.250000 */,(0x13333 /* 1.200000 */ + tone))),0,63);
   return (fix_wrap_array(Rescomb_toneCurve_c0)[index] + fix_mul(tone,(fix_wrap_array(Rescomb_toneCurve_c1)[index] + fix_mul(tone,fix_wrap_array(Rescomb_toneCurve_c2)[index]))));
}

typedef struct Rescomb__ctx_type_23 {
   fix16_t stone;
   fix16_t output;
   Rescomb__ctx_type_18 _inst51d;
   Rescomb__ctx_type_2 _inst449;
   Rescomb__ctx_type_1 _inst3a1;
   Rescomb__ctx_type_0 _inst1dd;
} Rescomb__ctx_type_23;

typedef Rescomb__ctx_type_23 Rescomb_process_type;

void Rescomb__ctx_type_23_init(Rescomb__ctx_type_23 &_output_);

static_inline void Rescomb_process_init(Rescomb__ctx_type_23 &_output_){
   Rescomb__ctx_type_23_init(_output_);
   return ;
}

fix16_t Rescomb_process(Rescomb__ctx_type_23 &_ctx, fix16_t in, fix16_t cv, fix16_t tone, fix16_t res);



#endif // RESCOMB_H
