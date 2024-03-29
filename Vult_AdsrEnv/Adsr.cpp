
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#include "Adsr.h"

void Adsr__ctx_type_2_init(Adsr__ctx_type_2 &_output_){
   Adsr__ctx_type_2 _ctx;
   _ctx.target = 0x0 /* 0.000000 */;
   _ctx.state = 0;
   _ctx.scale = 0x0 /* 0.000000 */;
   _ctx.rate = 0x0 /* 0.000000 */;
   _ctx.out = 0x0 /* 0.000000 */;
   Adsr__ctx_type_1_init(_ctx._inst981);
   Adsr__ctx_type_0_init(_ctx._inst2b7);
   _output_ = _ctx;
   return ;
}

fix16_t Adsr_process(Adsr__ctx_type_2 &_ctx, fix16_t gate, fix16_t a, fix16_t d, fix16_t s, fix16_t r){
   fix16_t a_rate;
   a_rate = fix_div(0x10000 /* 1.000000 */,(0x28f /* 0.010000 */ + fix_mul(0x640000 /* 100.000000 */,a)));
   fix16_t d_rate;
   d_rate = fix_div(0x10000 /* 1.000000 */,(0x28f /* 0.010000 */ + fix_mul(0x640000 /* 100.000000 */,d)));
   fix16_t r_rate;
   r_rate = fix_div(0x10000 /* 1.000000 */,(0x28f /* 0.010000 */ + fix_mul(0x640000 /* 100.000000 */,r)));
   _ctx.out = (_ctx.out + fix_mul(fix_mul(0x106 /* 0.004000 */,_ctx.rate),(_ctx.target + (- _ctx.out))));
   uint8_t bgate;
   bgate = (gate > 0x0 /* 0.000000 */);
   if(_ctx.state == 0){
      if(Adsr_edge(_ctx._inst2b7,bgate)){
         _ctx.state = 1;
         _ctx.scale = fix_mul(0x3333 /* 0.200000 */,gate);
      }
      _ctx.rate = r_rate;
      _ctx.target = 0x0 /* 0.000000 */;
   }
   if(_ctx.state == 1){
      if(bool_not(bgate)){
         _ctx.state = 0;
      }
      if(_ctx.out > 0x4000000 /* 1024.000000 */){
         _ctx.state = 2;
      }
      _ctx.rate = a_rate;
      _ctx.target = 0x4cccccc /* 1228.800000 */;
   }
   if(_ctx.state == 2){
      if(bool_not(bgate)){
         _ctx.state = 0;
      }
      _ctx.rate = d_rate;
      _ctx.target = (s << 10);
   }
   return fix_mul(Adsr_smooth(_ctx._inst981,_ctx.scale),fix_clip((_ctx.out >> 10),0x0 /* 0.000000 */,0x10000 /* 1.000000 */));
}


