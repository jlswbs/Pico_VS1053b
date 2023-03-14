/* Code automatically generated by Vult https://github.com/modlfo/vult */
#ifndef SWEPT_H
#define SWEPT_H
#include <stdint.h>
#include <math.h>
#include "vultin.h"

typedef struct Swept__ctx_type_0 {
   uint8_t pre;
} Swept__ctx_type_0;

typedef Swept__ctx_type_0 Swept_edge_type;

static_inline void Swept__ctx_type_0_init(Swept__ctx_type_0 &_output_){
   Swept__ctx_type_0 _ctx;
   _ctx.pre = false;
   _output_ = _ctx;
   return ;
}

static_inline void Swept_edge_init(Swept__ctx_type_0 &_output_){
   Swept__ctx_type_0_init(_output_);
   return ;
}

static_inline uint8_t Swept_edge(Swept__ctx_type_0 &_ctx, uint8_t x){
   uint8_t ret;
   ret = (x && bool_not(_ctx.pre));
   _ctx.pre = x;
   return ret;
}

typedef struct Swept__ctx_type_1 {
   fix16_t out;
   Swept__ctx_type_0 _inst10;
} Swept__ctx_type_1;

typedef Swept__ctx_type_1 Swept_process_type;

void Swept__ctx_type_1_init(Swept__ctx_type_1 &_output_);

static_inline void Swept_process_init(Swept__ctx_type_1 &_output_){
   Swept__ctx_type_1_init(_output_);
   return ;
}

fix16_t Swept_process(Swept__ctx_type_1 &_ctx, fix16_t gate, fix16_t start, fix16_t end, fix16_t rate);



#endif // SWEPT_H
