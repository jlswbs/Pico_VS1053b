
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#include "Delay.h"

void Delay__ctx_type_5_init(Delay__ctx_type_5 &_output_){
   Delay__ctx_type_5 _ctx;
   _ctx.x4 = 0x0 /* 0.000000 */;
   _ctx.x3 = 0x0 /* 0.000000 */;
   _ctx.x2 = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

fix16_t Delay_soft(Delay__ctx_type_5 &_ctx, fix16_t x1){
   fix16_t o;
   o = ((_ctx.x2 + _ctx.x3 + _ctx.x4 + x1) >> 2);
   fix16_t _tmp_1_0;
   _tmp_1_0 = x1;
   fix16_t _tmp_1_1;
   _tmp_1_1 = _ctx.x2;
   fix16_t _tmp_1_2;
   _tmp_1_2 = _ctx.x3;
   _ctx.x2 = _tmp_1_0;
   _ctx.x3 = _tmp_1_1;
   _ctx.x4 = _tmp_1_2;
   return o;
}

void Delay__ctx_type_6_init(Delay__ctx_type_6 &_output_){
   Delay__ctx_type_6 _ctx;
   _ctx.write_pos = 0;
   ;
   _output_ = _ctx;
   return ;
}

fix16_t Delay_do(Delay__ctx_type_6 &_ctx, fix16_t x, fix16_t time, fix16_t feedback){
   time = fix_clip(time,0x0 /* 0.000000 */,0x10000 /* 1.000000 */);
   feedback = fix_clip(feedback,0x0 /* 0.000000 */,0x10000 /* 1.000000 */);
   fix16_t index_r;
   index_r = fix_mul(0x7fff0000 /* 32767.000000 */,time);
   int index_i;
   index_i = fix_to_int(fix_floor(index_r));
   int delta;
   delta = (_ctx.write_pos + (- index_i));
   int read_pos;
   if(delta < 0){
      read_pos = (32767 + delta);
   }
   else
   {
      read_pos = delta;
   }
   fix16_t decimal;
   decimal = (index_r + (- int_to_fix(index_i)));
   fix16_t x1;
   x1 = fix_get(_ctx.buffer,read_pos);
   fix16_t x2;
   x2 = fix_get(_ctx.buffer,((1 + read_pos) % 32767));
   fix16_t ret;
   ret = (x1 + fix_mul(decimal,(x2 + (- x1))));
   _ctx.write_pos = ((1 + _ctx.write_pos) % 32767);
   fix_set(_ctx.buffer,_ctx.write_pos,Delay_saturate((x + fix_mul(feedback,ret))));
   return ret;
}

void Delay__ctx_type_7_init(Delay__ctx_type_7 &_output_){
   Delay__ctx_type_7 _ctx;
   Delay__ctx_type_6_init(_ctx._inst2b3);
   Delay__ctx_type_5_init(_ctx._inst14a);
   _output_ = _ctx;
   return ;
}


