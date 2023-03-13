
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#include "Lfo.h"

void Lfo__ctx_type_2_init(Lfo__ctx_type_2 &_output_){
   Lfo__ctx_type_2 _ctx;
   _ctx.x4 = 0x0 /* 0.000000 */;
   _ctx.x3 = 0x0 /* 0.000000 */;
   _ctx.x2 = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

fix16_t Lfo_soft(Lfo__ctx_type_2 &_ctx, fix16_t x1){
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

void Lfo__ctx_type_12_init(Lfo__ctx_type_12 &_output_){
   Lfo__ctx_type_12 _ctx;
   _ctx.rate = 0x0 /* 0.000000 */;
   _ctx.phase = 0x0 /* 0.000000 */;
   Lfo__ctx_type_2_init(_ctx._inst59b);
   Lfo__ctx_type_1_init(_ctx._inst39d);
   Lfo__ctx_type_0_init(_ctx._inst1d7);
   _output_ = _ctx;
   return ;
}

fix16_t Lfo_process(Lfo__ctx_type_12 &_ctx, fix16_t cv, fix16_t shape, fix16_t reset){
   if(Lfo_change(_ctx._inst1d7,cv)){
      _ctx.rate = Lfo_cvToRate((-0x4ccc /* -0.300000 */ + cv));
   }
   _ctx.phase = (_ctx.phase + _ctx.rate);
   if(_ctx.phase > 0x20000 /* 2.000000 */){
      _ctx.phase = (-0x20000 /* -2.000000 */ + _ctx.phase);
   }
   uint8_t breset;
   breset = (reset > 0x0 /* 0.000000 */);
   if(Lfo_edge(_ctx._inst39d,breset)){
      _ctx.phase = 0x0 /* 0.000000 */;
   }
   fix16_t tmp;
   tmp = (-0x10000 /* -1.000000 */ + _ctx.phase);
   fix16_t o;
   if(shape < 0x10000 /* 1.000000 */){
      o = tmp;
   }
   else
   {
      if(shape < 0x20000 /* 2.000000 */){
         o = (-0x10000 /* -1.000000 */ + (fix_abs(tmp) << 1));
      }
      else
      {
         if(tmp > 0x0 /* 0.000000 */){
            o = 0x10000 /* 1.000000 */;
         }
         else
         {
            o = 0x0 /* 0.000000 */;
         }
      }
   }
   return Lfo_soft(_ctx._inst59b,o);
}

