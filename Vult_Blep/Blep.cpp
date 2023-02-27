
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#include "Blep.h"

void Blep__ctx_type_13_init(Blep__ctx_type_13 &_output_){
   Blep__ctx_type_13 _ctx;
   _ctx.rate = 0x0 /* 0.000000 */;
   _ctx.phase = 0x0 /* 0.000000 */;
   Blep__ctx_type_2_init(_ctx._inst31a);
   Blep__ctx_type_3_init(_ctx._inst1bb);
   _output_ = _ctx;
   return ;
}

fix16_t Blep_phase(Blep__ctx_type_13 &_ctx, fix16_t cv, fix16_t reset){
   if(Blep_change(_ctx._inst1bb,cv)){
      _ctx.rate = Blep_cvToRate_1024(cv);
   }
   if(Blep_edge(_ctx._inst31a,(reset > 0x8000 /* 0.500000 */))){
      _ctx.phase = 0x0 /* 0.000000 */;
   }
   else
   {
      _ctx.phase = (_ctx.phase + _ctx.rate);
   }
   if(_ctx.phase > 0x4000000 /* 1024.000000 */){
      _ctx.phase = (-0x4000000 /* -1024.000000 */ + _ctx.phase);
   }
   return (_ctx.phase >> 10);
}

void Blep__ctx_type_14_init(Blep__ctx_type_14 &_output_){
   Blep__ctx_type_14 _ctx;
   _ctx.playing = false;
   _ctx.index = 0;
   Blep__ctx_type_2_init(_ctx._inst11a);
   _output_ = _ctx;
   return ;
}

fix16_t Blep_blep_play(Blep__ctx_type_14 &_ctx, uint8_t trigger){
   if(Blep_edge(_ctx._inst11a,trigger)){
      _ctx.index = 0;
      _ctx.playing = true;
   }
   fix16_t out;
   out = 0x0 /* 0.000000 */;
   if(_ctx.playing){
      if(_ctx.index >= Blep_blep_samples()){
         _ctx.playing = false;
         _ctx.index = 0;
      }
      else
      {
         _ctx.index = (16 + _ctx.index);
      }
      if(_ctx.playing){
         out = Blep_blep(0,_ctx.index);
      }
      else
      {
         out = 0x0 /* 0.000000 */;
      }
   }
   return out;
}

void Blep__ctx_type_15_init(Blep__ctx_type_15 &_output_){
   Blep__ctx_type_15 _ctx;
   _ctx.pre_phase = 0x0 /* 0.000000 */;
   _ctx.active_blep = 0;
   _ctx.acc = 0x0 /* 0.000000 */;
   Blep__ctx_type_14_init(_ctx._inst715);
   Blep__ctx_type_14_init(_ctx._inst615);
   Blep__ctx_type_14_init(_ctx._inst515);
   Blep__ctx_type_14_init(_ctx._inst415);
   Blep__ctx_type_2_init(_ctx._inst21a);
   Blep__ctx_type_13_init(_ctx._inst19e);
   _output_ = _ctx;
   return ;
}

fix16_t Blep_blep_step(Blep__ctx_type_15 &_ctx, fix16_t cv, uint8_t reset){
   fix16_t phase;
   phase = Blep_phase(_ctx._inst19e,cv,0x0 /* 0.000000 */);
   uint8_t wrap;
   wrap = ((_ctx.pre_phase + (- phase)) > 0x8000 /* 0.500000 */);
   _ctx.pre_phase = phase;
   if(wrap || Blep_edge(_ctx._inst21a,reset)){
      _ctx.active_blep = ((1 + _ctx.active_blep) % 4);
   }
   fix16_t o0;
   o0 = Blep_blep_play(_ctx._inst415,(_ctx.active_blep == 0));
   fix16_t o1;
   o1 = Blep_blep_play(_ctx._inst515,(_ctx.active_blep == 1));
   fix16_t o2;
   o2 = Blep_blep_play(_ctx._inst615,(_ctx.active_blep == 2));
   fix16_t o3;
   o3 = Blep_blep_play(_ctx._inst715,(_ctx.active_blep == 3));
   fix16_t pulses;
   pulses = (o0 + o2 + (- o1) + (- o3));
   _ctx.acc = (fix_mul(0xfef9 /* 0.996000 */,_ctx.acc) + fix_mul(0x140000 /* 20.000000 */,pulses));
   return _ctx.acc;
}

fix16_t Blep_process(Blep__ctx_type_16 &_ctx, fix16_t cv, fix16_t reset){
   uint8_t breset;
   breset = (reset > 0x8000 /* 0.500000 */);
   fix16_t out1;
   out1 = Blep_blep_step(_ctx.o,cv,breset);
   fix16_t out2;
   out2 = Blep_blep_step(_ctx.o,cv,breset);
   fix16_t out3;
   out3 = Blep_blep_step(_ctx.o,cv,breset);
   fix16_t out4;
   out4 = Blep_blep_step(_ctx.o,cv,breset);
   return ((out1 + out2 + out3 + out4) >> 2);
}


