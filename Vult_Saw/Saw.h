
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#ifndef SAW_H
#define SAW_H
#include <stdint.h>
#include <math.h>
#include "vultin.h"
#include "Saw.tables.h"

typedef struct Util__ctx_type_0 {
   uint8_t pre;
} Util__ctx_type_0;

typedef Util__ctx_type_0 Util_edge_type;

static_inline void Util__ctx_type_0_init(Util__ctx_type_0 &_output_){
   Util__ctx_type_0 _ctx;
   _ctx.pre = false;
   _output_ = _ctx;
   return ;
}

static_inline void Util_edge_init(Util__ctx_type_0 &_output_){
   Util__ctx_type_0_init(_output_);
   return ;
}

static_inline uint8_t Util_edge(Util__ctx_type_0 &_ctx, uint8_t x){
   uint8_t ret;
   ret = (x && bool_not(_ctx.pre));
   _ctx.pre = x;
   return ret;
}

typedef struct Util__ctx_type_1 {
   fix16_t pre_x;
} Util__ctx_type_1;

typedef Util__ctx_type_1 Util_change_type;

static_inline void Util__ctx_type_1_init(Util__ctx_type_1 &_output_){
   Util__ctx_type_1 _ctx;
   _ctx.pre_x = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

static_inline void Util_change_init(Util__ctx_type_1 &_output_){
   Util__ctx_type_1_init(_output_);
   return ;
}

static_inline uint8_t Util_change(Util__ctx_type_1 &_ctx, fix16_t x){
   uint8_t v;
   v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}

static_inline fix16_t Util_map(fix16_t x, fix16_t x0, fix16_t x1, fix16_t y0, fix16_t y1){
   return (y0 + fix_div(fix_mul((x + (- x0)),(y1 + (- y0))),(x1 + (- x0))));
};

typedef struct Util__ctx_type_3 {
   fix16_t y1;
   fix16_t x1;
} Util__ctx_type_3;

typedef Util__ctx_type_3 Util_dcblock_type;

void Util__ctx_type_3_init(Util__ctx_type_3 &_output_);

static_inline void Util_dcblock_init(Util__ctx_type_3 &_output_){
   Util__ctx_type_3_init(_output_);
   return ;
}

fix16_t Util_dcblock(Util__ctx_type_3 &_ctx, fix16_t x0);

typedef struct Util__ctx_type_4 {
   fix16_t x;
} Util__ctx_type_4;

typedef Util__ctx_type_4 Util_smooth_type;

static_inline void Util__ctx_type_4_init(Util__ctx_type_4 &_output_){
   Util__ctx_type_4 _ctx;
   _ctx.x = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

static_inline void Util_smooth_init(Util__ctx_type_4 &_output_){
   Util__ctx_type_4_init(_output_);
   return ;
}

static_inline fix16_t Util_smooth(Util__ctx_type_4 &_ctx, fix16_t input){
   _ctx.x = (_ctx.x + fix_mul(0x147 /* 0.005000 */,(input + (- _ctx.x))));
   return _ctx.x;
}

typedef struct Util__ctx_type_5 {
   fix16_t x0;
} Util__ctx_type_5;

typedef Util__ctx_type_5 Util_average2_type;

static_inline void Util__ctx_type_5_init(Util__ctx_type_5 &_output_){
   Util__ctx_type_5 _ctx;
   _ctx.x0 = 0x0 /* 0.000000 */;
   _output_ = _ctx;
   return ;
}

static_inline void Util_average2_init(Util__ctx_type_5 &_output_){
   Util__ctx_type_5_init(_output_);
   return ;
}

static_inline fix16_t Util_average2(Util__ctx_type_5 &_ctx, fix16_t x1){
   fix16_t result;
   result = ((_ctx.x0 + x1) >> 1);
   _ctx.x0 = x1;
   return result;
}

static_inline fix16_t Util_cubic_clipper(fix16_t x){
   if(x <= -0xaaaa /* -0.666667 */){
      return -0xaaaa /* -0.666667 */;
   }
   else
   {
      if(x >= 0xaaaa /* 0.666667 */){
         return 0xaaaa /* 0.666667 */;
      }
      else
      {
         return (x + fix_mul(fix_mul(fix_mul(-0x5555 /* -0.333333 */,x),x),x));
      }
   }
};

static_inline fix16_t Util_pitchToRate_1024_raw_c0(int index){
   return Util_pitchToRate_1024_c0[index];
};

static_inline fix16_t Util_pitchToRate_1024_raw_c1(int index){
   return Util_pitchToRate_1024_c1[index];
};

static_inline fix16_t Util_pitchToRate_1024_raw_c2(int index){
   return Util_pitchToRate_1024_c2[index];
};

static_inline fix16_t Util_pitchToRate_1024(fix16_t pitch){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x3e7c /* 0.244094 */,pitch)),0,31);
   return (fix_wrap_array(Util_pitchToRate_1024_c0)[index] + fix_mul(pitch,(fix_wrap_array(Util_pitchToRate_1024_c1)[index] + fix_mul(pitch,fix_wrap_array(Util_pitchToRate_1024_c2)[index]))));
}

static_inline fix16_t Util_pitchToRate_raw_c0(int index){
   return Util_pitchToRate_c0[index];
};

static_inline fix16_t Util_pitchToRate_raw_c1(int index){
   return Util_pitchToRate_c1[index];
};

static_inline fix16_t Util_pitchToRate_raw_c2(int index){
   return Util_pitchToRate_c2[index];
};

static_inline fix16_t Util_pitchToRate(fix16_t pitch){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x3e7c /* 0.244094 */,pitch)),0,31);
   return (fix_wrap_array(Util_pitchToRate_c0)[index] + fix_mul(pitch,(fix_wrap_array(Util_pitchToRate_c1)[index] + fix_mul(pitch,fix_wrap_array(Util_pitchToRate_c2)[index]))));
}

static_inline fix16_t Util_cvToPitch(fix16_t cv){
   return (0x180000 /* 24.000000 */ + fix_mul(0x780000 /* 120.000000 */,cv));
};

static_inline fix16_t Util_cvToRate_1024_raw_c0(int index){
   return Util_cvToRate_1024_c0[index];
};

static_inline fix16_t Util_cvToRate_1024_raw_c1(int index){
   return Util_cvToRate_1024_c1[index];
};

static_inline fix16_t Util_cvToRate_1024_raw_c2(int index){
   return Util_cvToRate_1024_c2[index];
};

static_inline fix16_t Util_cvToRate_1024(fix16_t cv){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x2271c7 /* 34.444444 */,cv)),0,31);
   return (fix_wrap_array(Util_cvToRate_1024_c0)[index] + fix_mul(cv,(fix_wrap_array(Util_cvToRate_1024_c1)[index] + fix_mul(cv,fix_wrap_array(Util_cvToRate_1024_c2)[index]))));
}

static_inline fix16_t Util_cvToRate_raw_c0(int index){
   return Util_cvToRate_c0[index];
};

static_inline fix16_t Util_cvToRate_raw_c1(int index){
   return Util_cvToRate_c1[index];
};

static_inline fix16_t Util_cvToRate_raw_c2(int index){
   return Util_cvToRate_c2[index];
};

static_inline fix16_t Util_cvToRate(fix16_t cv){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x8d1c71 /* 141.111111 */,cv)),0,127);
   return (fix_wrap_array(Util_cvToRate_c0)[index] + fix_mul(cv,(fix_wrap_array(Util_cvToRate_c1)[index] + fix_mul(cv,fix_wrap_array(Util_cvToRate_c2)[index]))));
}

static_inline fix16_t Util_pitchToCv(fix16_t pitch){
   return fix_mul(0x222 /* 0.008333 */,(-0x180000 /* -24.000000 */ + pitch));
};

static_inline fix16_t Util_cvToperiod_raw_c0(int index){
   return Util_cvToperiod_c0[index];
};

static_inline fix16_t Util_cvToperiod_raw_c1(int index){
   return Util_cvToperiod_c1[index];
};

static_inline fix16_t Util_cvToperiod_raw_c2(int index){
   return Util_cvToperiod_c2[index];
};

static_inline fix16_t Util_cvToperiod(fix16_t cv){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x1f0000 /* 31.000000 */,cv)),0,31);
   return (fix_wrap_array(Util_cvToperiod_c0)[index] + fix_mul(cv,(fix_wrap_array(Util_cvToperiod_c1)[index] + fix_mul(cv,fix_wrap_array(Util_cvToperiod_c2)[index]))));
}

static_inline fix16_t Util_cvTokHz_raw_c0(int index){
   return Util_cvTokHz_c0[index];
};

static_inline fix16_t Util_cvTokHz_raw_c1(int index){
   return Util_cvTokHz_c1[index];
};

static_inline fix16_t Util_cvTokHz_raw_c2(int index){
   return Util_cvTokHz_c2[index];
};

static_inline fix16_t Util_cvTokHz(fix16_t cv){
   int index;
   index = int_clip(fix_to_int(fix_mul(0x1f0000 /* 31.000000 */,cv)),0,31);
   return (fix_wrap_array(Util_cvTokHz_c0)[index] + fix_mul(cv,(fix_wrap_array(Util_cvTokHz_c1)[index] + fix_mul(cv,fix_wrap_array(Util_cvTokHz_c2)[index]))));
}

typedef struct Saw__ctx_type_0 {
   fix16_t phase;
   fix16_t inc;
   Util__ctx_type_1 _inst13b;
} Saw__ctx_type_0;

typedef Saw__ctx_type_0 Saw_process_type;

void Saw__ctx_type_0_init(Saw__ctx_type_0 &_output_);

static_inline void Saw_process_init(Saw__ctx_type_0 &_output_){
   Saw__ctx_type_0_init(_output_);
   return ;
}

fix16_t Saw_process(Saw__ctx_type_0 &_ctx, fix16_t cv);

class Saw
{
public:
  Saw(void)
  {
    Saw_process_init(data);
  }

  int16_t output(int cv) { return Saw_process(data, cv) >> 2; }

private:
  Saw_process_type data;

};

#endif // SAW_H
