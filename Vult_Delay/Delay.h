
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#ifndef DELAY_H
#define DELAY_H
#include <stdint.h>
#include <math.h>
#include "vultin.h"
#include "Delay.tables.h"

static_inline fix16_t Delay_tanh_table_raw_c0(int index){
   return Delay_tanh_table_c0[index];
};

static_inline fix16_t Delay_tanh_table_raw_c1(int index){
   return Delay_tanh_table_c1[index];
};

static_inline fix16_t Delay_tanh_table_raw_c2(int index){
   return Delay_tanh_table_c2[index];
};

static_inline fix16_t Delay_tanh_table(fix16_t x){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x50000 /* 5.000000 */,(0x180000 /* 24.000000 */ + x))),0,240);
   return (fix_wrap_array(Delay_tanh_table_c0)[index] + fix_mul(x,(fix_wrap_array(Delay_tanh_table_c1)[index] + fix_mul(x,fix_wrap_array(Delay_tanh_table_c2)[index]))));
}

static_inline fix16_t Delay_saturate(fix16_t x){
   return Delay_tanh_table(x);
};

typedef struct Delay__ctx_type_5 {
   fix16_t x4;
   fix16_t x3;
   fix16_t x2;
} Delay__ctx_type_5;

typedef Delay__ctx_type_5 Delay_soft_type;

void Delay__ctx_type_5_init(Delay__ctx_type_5 &_output_);

static_inline void Delay_soft_init(Delay__ctx_type_5 &_output_){
   Delay__ctx_type_5_init(_output_);
   return ;
}

fix16_t Delay_soft(Delay__ctx_type_5 &_ctx, fix16_t x1);

typedef struct Delay__ctx_type_6 {
   int write_pos;
   fix16_t buffer[32767];
} Delay__ctx_type_6;

typedef Delay__ctx_type_6 Delay_do_type;

void Delay__ctx_type_6_init(Delay__ctx_type_6 &_output_);

static_inline void Delay_do_init(Delay__ctx_type_6 &_output_){
   Delay__ctx_type_6_init(_output_);
   return ;
}

fix16_t Delay_do(Delay__ctx_type_6 &_ctx, fix16_t x, fix16_t time, fix16_t feedback);

typedef struct Delay__ctx_type_7 {
   Delay__ctx_type_6 _inst2b3;
   Delay__ctx_type_5 _inst14a;
} Delay__ctx_type_7;

typedef Delay__ctx_type_7 Delay_process_type;

void Delay__ctx_type_7_init(Delay__ctx_type_7 &_output_);

static_inline void Delay_process_init(Delay__ctx_type_7 &_output_){
   Delay__ctx_type_7_init(_output_);
   return ;
}

static_inline fix16_t Delay_process(Delay__ctx_type_7 &_ctx, fix16_t x, fix16_t time, fix16_t feedback){
   return Delay_do(_ctx._inst2b3,x,Delay_soft(_ctx._inst14a,time),feedback);
};



#endif // DELAY_H
