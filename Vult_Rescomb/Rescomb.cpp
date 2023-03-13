
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#include "Rescomb.h"

void Rescomb__ctx_type_1_init(Rescomb__ctx_type_1 &_output_){
   Rescomb__ctx_type_1 _ctx;
   _ctx.y1 = 0x0 /* 0.000000 */;
   _ctx.x1 = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

fix16_t Rescomb_dcblock(Rescomb__ctx_type_1 &_ctx, fix16_t x0){
   fix16_t y0;
   y0 = (x0 + (- _ctx.x1) + fix_mul(0xfeb8 /* 0.995000 */,_ctx.y1));
   _ctx.x1 = x0;
   _ctx.y1 = y0;
   return y0;
}

void Rescomb__ctx_type_2_init(Rescomb__ctx_type_2 &_output_){
   Rescomb__ctx_type_2 _ctx;
   _ctx.x4 = 0x0 /* 0.000000 */;
   _ctx.x3 = 0x0 /* 0.000000 */;
   _ctx.x2 = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

fix16_t Rescomb_soft(Rescomb__ctx_type_2 &_ctx, fix16_t x1){
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

void Rescomb__ctx_type_18_init(Rescomb__ctx_type_18 &_output_){
   Rescomb__ctx_type_18 _ctx;
   _ctx.write_pos = 0;
   ;
   _output_ = _ctx;
   return ;
}

fix16_t Rescomb_delayx(Rescomb__ctx_type_18 &_ctx, fix16_t x, fix16_t cv){
   _ctx.write_pos = ((1 + _ctx.write_pos) % 1000);
   fix_set(_ctx.buffer,_ctx.write_pos,x);
   fix16_t r_size;
   r_size = 0x3e80000 /* 1000.000000 */;
   fix16_t r_index;
   r_index = ((int_to_fix(_ctx.write_pos) + (- Rescomb_cvToperiod(cv))) % r_size);
   if(r_index < 0x0 /* 0.000000 */){
      r_index = (r_index + r_size);
   }
   int t1;
   t1 = (fix_to_int(fix_floor(r_index)) % 1000);
   int t2;
   t2 = ((1 + t1) % 1000);
   fix16_t decimal;
   decimal = (r_index + (- int_to_fix(t1)));
   fix16_t x1;
   x1 = fix_get(_ctx.buffer,t1);
   fix16_t x2;
   x2 = fix_get(_ctx.buffer,t2);
   fix16_t ret;
   ret = (x1 + fix_mul(decimal,(x2 + (- x1))));
   return ret;
}

void Rescomb__ctx_type_23_init(Rescomb__ctx_type_23 &_output_){
   Rescomb__ctx_type_23 _ctx;
   _ctx.stone = 0x0 /* 0.000000 */;
   _ctx.output = 0x0 /* 0.000000 */;
   Rescomb__ctx_type_18_init(_ctx._inst51d);
   Rescomb__ctx_type_2_init(_ctx._inst449);
   Rescomb__ctx_type_1_init(_ctx._inst3a1);
   Rescomb__ctx_type_0_init(_ctx._inst1dd);
   _output_ = _ctx;
   return ;
}

fix16_t Rescomb_process(Rescomb__ctx_type_23 &_ctx, fix16_t in, fix16_t cv, fix16_t tone, fix16_t res){
   if(Rescomb_change(_ctx._inst1dd,tone)){
      _ctx.stone = Rescomb_toneCurve(tone);
   }
   fix16_t feedback;
   feedback = Rescomb_dcblock(_ctx._inst3a1,fix_mul(_ctx.output,res));
   fix16_t saturated_input;
   saturated_input = Rescomb_saturate_soft((feedback + in));
   _ctx.output = (in + fix_mul(_ctx.stone,Rescomb_delayx(_ctx._inst51d,saturated_input,Rescomb_soft(_ctx._inst449,cv))));
   return Rescomb_saturate(_ctx.output);
}

