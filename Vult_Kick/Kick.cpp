
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#include "Kick.h"

fix16_t Kick_pinker(Kick__ctx_type_16 &_ctx, fix16_t x){
   fix16_t b0;
   b0 = 0x718 /* 0.027713 */;
   fix16_t b1;
   b1 = 0x718 /* 0.027713 */;
   fix16_t a1;
   a1 = -0xf1cf /* -0.944574 */;
   fix16_t w0;
   w0 = (x + (- fix_mul(_ctx.w1,a1)));
   fix16_t y0;
   y0 = (fix_mul(_ctx.w1,b1) + fix_mul(b0,w0));
   _ctx.w1 = w0;
   return y0;
}

void Kick__ctx_type_17_init(Kick__ctx_type_17 &_output_){
   Kick__ctx_type_17 _ctx;
   _ctx.x2 = 0;
   _ctx.x1 = 0;
   Kick__ctx_type_16_init(_ctx._inst1a5);
   _output_ = _ctx;
   return ;
}

fix16_t Kick_noise(Kick__ctx_type_17 &_ctx, fix16_t color){
   _ctx.x1 = ((7919 + (17389 * _ctx.x1)) % 32768);
   fix16_t y1;
   y1 = fix_mul(0x2 /* 0.000031 */,int_to_fix(_ctx.x1));
   _ctx.x2 = ((12553 + (27449 * _ctx.x2)) % 32768);
   fix16_t y2;
   y2 = fix_mul(0x2 /* 0.000031 */,int_to_fix(_ctx.x2));
   fix16_t out;
   if(color > 0x8000 /* 0.500000 */){
      out = Kick_pinker(_ctx._inst1a5,(y1 + (- y2)));
   }
   else
   {
      out = (y1 + (- y2));
   }
   return (out << 1);
}

void Kick__ctx_type_18_init(Kick__ctx_type_18 &_output_){
   Kick__ctx_type_18 _ctx;
   _ctx.reset_state = false;
   _ctx.reset_phase = 0x0 /* 0.000000 */;
   _ctx.rate = 0x0 /* 0.000000 */;
   _ctx.phase = 0x0 /* 0.000000 */;
   _ctx.direction = false;
   Kick__ctx_type_0_init(_ctx._inst480);
   Kick__ctx_type_0_init(_ctx._inst380);
   Kick__ctx_type_1_init(_ctx._inst1bf);
   _output_ = _ctx;
   return ;
}

fix16_t Kick_tricore(Kick__ctx_type_18 &_ctx, fix16_t cv, fix16_t reset, fix16_t disable){
   fix16_t out;
   out = 0x0 /* 0.000000 */;
   if(Kick_change(_ctx._inst1bf,cv)){
      _ctx.rate = (Kick_cvToRate_1024(cv) << 2);
   }
   fix16_t new_phase;
   fix16_t _if_13;
   if(_ctx.direction){
      _if_13 = _ctx.rate;
   }
   else
   {
      _if_13 = (- _ctx.rate);
   }
   new_phase = (_ctx.phase + _if_13);
   uint8_t bdisable;
   bdisable = (disable > 0x8000 /* 0.500000 */);
   if(Kick_edge(_ctx._inst380,(reset > 0x0 /* 0.000000 */)) || Kick_edge(_ctx._inst480,bdisable)){
      _ctx.reset_phase = _ctx.phase;
      _ctx.reset_state = true;
      new_phase = 0x0 /* 0.000000 */;
   }
   if(new_phase > 0x4000000 /* 1024.000000 */){
      new_phase = (0x4000000 /* 1024.000000 */ + (- (-0x4000000 /* -1024.000000 */ + new_phase)));
      _ctx.direction = false;
   }
   else
   {
      if(new_phase < -0x4000000 /* -1024.000000 */){
         _ctx.direction = true;
      }
   }
   if(bdisable){
      _ctx.phase = 0x0 /* 0.000000 */;
   }
   else
   {
      _ctx.phase = new_phase;
   }
   if(_ctx.reset_state){
      if((fix_abs(_ctx.reset_phase) > 0xa0000 /* 10.000000 */) || bdisable){
         fix16_t reset_speed;
         if(bdisable){
            reset_speed = 0x28f /* 0.010000 */;
         }
         else
         {
            reset_speed = 0x8000 /* 0.500000 */;
         }
         _ctx.reset_phase = (_ctx.reset_phase + (- fix_mul(_ctx.reset_phase,reset_speed)));
      }
      else
      {
         uint8_t _cond_17;
         _cond_17 = (_ctx.reset_phase > 0xa0000 /* 10.000000 */);
         if(bool_not(_cond_17)){
            _ctx.reset_phase = 0x0 /* 0.000000 */;
         }
         _ctx.reset_state = false;
         _ctx.direction = true;
      }
      out = _ctx.reset_phase;
   }
   out = (_ctx.phase + out);
   return (out >> 10);
}

void Kick__ctx_type_19_init(Kick__ctx_type_19 &_output_){
   Kick__ctx_type_19 _ctx;
   _ctx.out = 0x0 /* 0.000000 */;
   Kick__ctx_type_0_init(_ctx._inst180);
   _output_ = _ctx;
   return ;
}

fix16_t Kick_swept(Kick__ctx_type_19 &_ctx, fix16_t gate, fix16_t start, fix16_t end, fix16_t rate){
   fix16_t a_rate;
   a_rate = fix_div(0x10000 /* 1.000000 */,(0x28f /* 0.010000 */ + fix_mul(0x640000 /* 100.000000 */,rate)));
   uint8_t bgate;
   bgate = (gate > 0x8000 /* 0.500000 */);
   _ctx.out = (_ctx.out + fix_mul(fix_mul(0x106 /* 0.004000 */,rate),((- _ctx.out) + (end << 10))));
   if(Kick_edge(_ctx._inst180,bgate)){
      _ctx.out = (start << 10);
   }
   return (_ctx.out >> 10);
}

void Kick__ctx_type_20_init(Kick__ctx_type_20 &_output_){
   Kick__ctx_type_20 _ctx;
   _ctx.target = 0x0 /* 0.000000 */;
   _ctx.state = 0;
   _ctx.rate = 0x0 /* 0.000000 */;
   _ctx.out = 0x0 /* 0.000000 */;
   _ctx.hold_phase = 0x0 /* 0.000000 */;
   _ctx.ahr_ret_1 = 0x0 /* 0.000000 */;
   _ctx.ahr_ret_0 = 0x0 /* 0.000000 */;
   Kick__ctx_type_0_init(_ctx._inst380);
   Kick__ctx_type_0_init(_ctx._inst180);
   _output_ = _ctx;
   return ;
}

void Kick_ahr(Kick__ctx_type_20 &_ctx, fix16_t gate, fix16_t a, fix16_t h, fix16_t r){
   int release;
   release = 0;
   int attack;
   attack = 1;
   int hold;
   hold = 2;
   int reset;
   reset = 3;
   uint8_t bgate;
   bgate = (gate > 0x0 /* 0.000000 */);
   if(Kick_edge(_ctx._inst180,bgate)){
      _ctx.state = reset;
   }
   fix16_t resetting;
   if(_ctx.state == reset){
      resetting = 0x10000 /* 1.000000 */;
   }
   else
   {
      resetting = 0x0 /* 0.000000 */;
   }
   if(Kick_edge(_ctx._inst380,(_ctx.out > 0x4000000 /* 1024.000000 */))){
      _ctx.hold_phase = 0x0 /* 0.000000 */;
      _ctx.state = hold;
   }
   if((_ctx.out < 0xa0000 /* 10.000000 */) && (_ctx.state == reset)){
      _ctx.state = attack;
   }
   if((_ctx.hold_phase > 0x640000 /* 100.000000 */) && (_ctx.state == hold)){
      _ctx.state = release;
   }
   if(_ctx.state == reset){
      _ctx.rate = 0xfd77 /* 0.990099 */;
      _ctx.target = 0x0 /* 0.000000 */;
   }
   if(_ctx.state == attack){
      _ctx.rate = fix_div(0x10000 /* 1.000000 */,(0x28f /* 0.010000 */ + fix_mul(0x640000 /* 100.000000 */,a)));
      _ctx.target = 0x4cccccc /* 1228.800000 */;
   }
   if(_ctx.state == hold){
      fix16_t hrate;
      hrate = fix_div(0x10000 /* 1.000000 */,(0x28f /* 0.010000 */ + fix_mul(0x640000 /* 100.000000 */,h)));
      _ctx.hold_phase = (_ctx.hold_phase + hrate);
   }
   if(_ctx.state == release){
      _ctx.rate = fix_div(0x10000 /* 1.000000 */,(0x28f /* 0.010000 */ + fix_mul(0x640000 /* 100.000000 */,r)));
      _ctx.target = 0x0 /* 0.000000 */;
   }
   _ctx.out = (_ctx.out + fix_mul(fix_mul(0x147 /* 0.005000 */,_ctx.rate),(_ctx.target + (- _ctx.out))));
   _ctx.ahr_ret_0 = fix_clip((_ctx.out >> 10),0x0 /* 0.000000 */,0x10000 /* 1.000000 */);
   _ctx.ahr_ret_1 = resetting;
   return ;
}

void Kick__ctx_type_21_init(Kick__ctx_type_21 &_output_){
   Kick__ctx_type_21 _ctx;
   Kick__ctx_type_17_init(_ctx._inst5de);
   Kick__ctx_type_20_init(_ctx._inst458);
   Kick__ctx_type_18_init(_ctx._inst3ca);
   Kick__ctx_type_19_init(_ctx._inst24a);
   Kick__ctx_type_20_init(_ctx._inst158);
   _output_ = _ctx;
   return ;
}

fix16_t Kick_process(Kick__ctx_type_21 &_ctx, fix16_t gate, fix16_t odecay, fix16_t pitch, fix16_t swept, fix16_t noise){
   fix16_t osc_env;
   fix16_t env_reset;
   Kick_ahr(_ctx._inst158,gate,0x147 /* 0.005000 */,(0x1999 /* 0.100000 */ + odecay),(0x28f /* 0.010000 */ + odecay));
   osc_env = Kick_ahr_ret_0(_ctx._inst158);
   env_reset = Kick_ahr_ret_1(_ctx._inst158);
   fix16_t cv;
   cv = Kick_swept(_ctx._inst24a,(0x10000 /* 1.000000 */ + (- env_reset)),(pitch + fix_mul(0x6666 /* 0.400000 */,swept)),pitch,0x1999 /* 0.100000 */);
   fix16_t osc;
   osc = Kick_tricore(_ctx._inst3ca,cv,0x0 /* 0.000000 */,env_reset);
   fix16_t osc_sat;
   osc_sat = Kick_saturate(fix_mul(0x18000 /* 1.500000 */,osc));
   fix16_t noise_env;
   Kick_ahr(_ctx._inst458,gate,0x41 /* 0.001000 */,0x28f /* 0.010000 */,(0x41 /* 0.001000 */ + fix_mul(0x28f /* 0.010000 */,noise)));
   noise_env = Kick_ahr_ret_0(_ctx._inst458);
   Kick_ahr_ret_1(_ctx._inst458);
   fix16_t noise_osc;
   noise_osc = fix_mul(fix_mul(0x5555 /* 0.333333 */,noise),Kick_noise(_ctx._inst5de,0x10000 /* 1.000000 */));
   return (fix_mul(noise_env,noise_osc) + fix_mul(osc_env,osc_sat));
}


