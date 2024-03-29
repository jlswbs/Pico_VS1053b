
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#ifndef AD_H
#define AD_H
#include <stdint.h>
#include <math.h>
#include "vultin.h"

typedef struct Ad__ctx_type_0 {
   uint8_t pre;
} Ad__ctx_type_0;

typedef Ad__ctx_type_0 Ad_edge_type;

static_inline void Ad__ctx_type_0_init(Ad__ctx_type_0 &_output_){
   Ad__ctx_type_0 _ctx;
   _ctx.pre = false;
   _output_ = _ctx;
   return ;
}

static_inline void Ad_edge_init(Ad__ctx_type_0 &_output_){
   Ad__ctx_type_0_init(_output_);
   return ;
}

static_inline uint8_t Ad_edge(Ad__ctx_type_0 &_ctx, uint8_t x){
   uint8_t ret;
   ret = (x && bool_not(_ctx.pre));
   _ctx.pre = x;
   return ret;
}

typedef struct Ad__ctx_type_1 {
   fix16_t target;
   int state;
   fix16_t scale;
   fix16_t rate;
   fix16_t out;
   Ad__ctx_type_0 _inst2b3;
} Ad__ctx_type_1;

typedef Ad__ctx_type_1 Ad_process_type;

void Ad__ctx_type_1_init(Ad__ctx_type_1 &_output_);

static_inline void Ad_process_init(Ad__ctx_type_1 &_output_){
   Ad__ctx_type_1_init(_output_);
   return ;
}

fix16_t Ad_process(Ad__ctx_type_1 &_ctx, fix16_t gate, fix16_t a, fix16_t d);



#endif // AD_H
