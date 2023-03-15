
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#include "Swept.h"

void Swept__ctx_type_1_init(Swept__ctx_type_1 &_output_){
   Swept__ctx_type_1 _ctx;
   _ctx.out = 0x0 /* 0.000000 */;
   Swept__ctx_type_0_init(_ctx._inst10);
   _output_ = _ctx;
   return ;
}

fix16_t Swept_process(Swept__ctx_type_1 &_ctx, fix16_t gate, fix16_t start, fix16_t end, fix16_t rate){
   fix16_t a_rate;
   a_rate = fix_div(0x10000 /* 1.000000 */,(0x28f /* 0.010000 */ + fix_mul(0x640000 /* 100.000000 */,rate)));
   uint8_t bgate;
   bgate = (gate > 0x8000 /* 0.500000 */);
   _ctx.out = (_ctx.out + fix_mul(fix_mul(0x106 /* 0.004000 */,rate),((- _ctx.out) + (end << 10))));
   if(Swept_edge(_ctx._inst10,bgate)){
      _ctx.out = (start << 10);
   }
   return (_ctx.out >> 10);
}

