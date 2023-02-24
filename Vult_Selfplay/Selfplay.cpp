
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#include "Selfplay.h"

void Selfplay__ctx_type_16_init(Selfplay__ctx_type_16 &_output_){
   Selfplay__ctx_type_16 _ctx;
   _ctx.rate = 0x0 /* 0.000000 */;
   _ctx.phase = 0x0 /* 0.000000 */;
   Selfplay__ctx_type_0_init(_ctx._inst11a);
   Selfplay_phase_start(_ctx);
   _output_ = _ctx;
   return ;
}

fix16_t Selfplay_phase(Selfplay__ctx_type_16 &_ctx, fix16_t cv){
   if(Selfplay_change(_ctx._inst11a,cv)){
      _ctx.rate = Selfplay_cvToRate(cv);
   }
   _ctx.phase = (_ctx.phase + _ctx.rate);
   if(_ctx.phase > 0x10000 /* 1.000000 */){
      _ctx.phase = (-0x10000 /* -1.000000 */ + _ctx.phase);
   }
   return _ctx.phase;
}

void Selfplay__ctx_type_17_init(Selfplay__ctx_type_17 &_output_){
   Selfplay__ctx_type_17 _ctx;
   _ctx.rate = 0x0 /* 0.000000 */;
   _ctx.phase = 0x0 /* 0.000000 */;
   Selfplay__ctx_type_0_init(_ctx._inst11a);
   _output_ = _ctx;
   return ;
}

fix16_t Selfplay_lfo(Selfplay__ctx_type_17 &_ctx, fix16_t cv){
   if(Selfplay_change(_ctx._inst11a,cv)){
      _ctx.rate = fix_mul(0x10 /* 0.000250 */,cv);
   }
   _ctx.phase = (_ctx.phase + _ctx.rate);
   if(_ctx.phase > 0x10000 /* 1.000000 */){
      _ctx.phase = (-0x10000 /* -1.000000 */ + _ctx.phase);
   }
   return _ctx.phase;
}

void Selfplay__ctx_type_19_init(Selfplay__ctx_type_19 &_output_){
   Selfplay__ctx_type_19 _ctx;
   _ctx.out = 0x0 /* 0.000000 */;
   Selfplay__ctx_type_2_init(_ctx._inst364);
   Selfplay__ctx_type_1_init(_ctx._inst132);
   _output_ = _ctx;
   return ;
}

void Selfplay__ctx_type_20_init(Selfplay__ctx_type_20 &_output_){
   Selfplay__ctx_type_20 _ctx;
   _ctx.out = 0x0 /* 0.000000 */;
   Selfplay__ctx_type_2_init(_ctx._inst164);
   _output_ = _ctx;
   return ;
}

void Selfplay__ctx_type_21_init(Selfplay__ctx_type_21 &_output_){
   Selfplay__ctx_type_21 _ctx;
   Selfplay__ctx_type_19_init(_ctx._inst9cc);
   Selfplay__ctx_type_18_init(_ctx._inst8b2);
   Selfplay__ctx_type_19_init(_ctx._inst7cc);
   Selfplay__ctx_type_18_init(_ctx._inst6b2);
   Selfplay__ctx_type_18_init(_ctx._inst5b2);
   Selfplay__ctx_type_19_init(_ctx._inst4cc);
   Selfplay__ctx_type_17_init(_ctx._inst338);
   Selfplay__ctx_type_17_init(_ctx._inst238);
   Selfplay__ctx_type_20_init(_ctx._inst13e5);
   Selfplay__ctx_type_17_init(_ctx._inst138);
   Selfplay__ctx_type_18_init(_ctx._inst12b2);
   Selfplay__ctx_type_19_init(_ctx._inst11cc);
   Selfplay__ctx_type_18_init(_ctx._inst10b2);
   _output_ = _ctx;
   return ;
}

fix16_t Selfplay_process(Selfplay__ctx_type_21 &_ctx){
   fix16_t lfo1;
   lfo1 = Selfplay_lfo(_ctx._inst138,0x28f /* 0.010000 */);
   fix16_t lfo2;
   lfo2 = Selfplay_lfo(_ctx._inst238,0x3333 /* 0.200000 */);
   fix16_t lfo3;
   lfo3 = Selfplay_lfo(_ctx._inst338,0x1999 /* 0.100000 */);
   fix16_t mod1;
   mod1 = fix_mul(0x1999 /* 0.100000 */,lfo1);
   fix16_t mod2;
   mod2 = Selfplay_osc(_ctx._inst5b2,Selfplay_shnoise(_ctx._inst4cc,lfo3));
   fix16_t o1;
   o1 = fix_mul(Selfplay_osc(_ctx._inst6b2,(0x4ccc /* 0.300000 */ + mod1)),Selfplay_shnoise(_ctx._inst7cc,lfo2));
   fix16_t o2;
   o2 = fix_mul(Selfplay_osc(_ctx._inst8b2,(0x6666 /* 0.400000 */ + (- mod1))),Selfplay_shnoise(_ctx._inst9cc,lfo2));
   fix16_t o3;
   o3 = fix_mul(Selfplay_osc(_ctx._inst10b2,(0x6666 /* 0.400000 */ + fix_mul(0x4ccc /* 0.300000 */,mod2))),Selfplay_shnoise(_ctx._inst11cc,lfo3));
   fix16_t o4;
   o4 = fix_mul(fix_mul(0x18000 /* 1.500000 */,Selfplay_osc(_ctx._inst12b2,(0x1999 /* 0.100000 */ + fix_mul(0x1999 /* 0.100000 */,mod2)))),Selfplay_pulse(_ctx._inst13e5,lfo2));
   return (o1 + o2 + o3 + o4) >> 2;
}


