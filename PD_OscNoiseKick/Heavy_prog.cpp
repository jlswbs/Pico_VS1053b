/**
 * Copyright (c) 2026 Enzien Audio, Ltd.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the phrase "powered by heavy",
 *    the heavy logo, and a hyperlink to https://enzienaudio.com, all in a visible
 *    form.
 * 
 *   2.1 If the Application is distributed in a store system (for example,
 *       the Apple "App Store" or "Google Play"), the phrase "powered by heavy"
 *       shall be included in the app description or the copyright text as well as
 *       the in the app itself. The heavy logo will shall be visible in the app
 *       itself as well.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "Heavy_prog.hpp"

#include <new>

#define Context(_c) static_cast<Heavy_prog *>(_c)


/*
 * C Functions
 */

extern "C" {
  HV_EXPORT HeavyContextInterface *hv_prog_new(double sampleRate) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_prog));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_prog(sampleRate);
    return Context(ptr);
  }

  HV_EXPORT HeavyContextInterface *hv_prog_new_with_options(double sampleRate,
      int poolKb, int inQueueKb, int outQueueKb) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_prog));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_prog(sampleRate, poolKb, inQueueKb, outQueueKb);
    return Context(ptr);
  }

  HV_EXPORT void hv_prog_free(HeavyContextInterface *instance) {
    // call destructor
    Context(instance)->~Heavy_prog();
    // free memory
    hv_free(instance);
  }
} // extern "C"







/*
 * Class Functions
 */

Heavy_prog::Heavy_prog(double sampleRate, int poolKb, int inQueueKb, int outQueueKb)
    : HeavyContext(sampleRate, poolKb, inQueueKb, outQueueKb) {
  numBytes += sRPole_init(&sRPole_CMFkc4Vk);
  numBytes += sDel1_init(&sDel1_0da2dfdo);
  numBytes += sLine_init(&sLine_GPM84qEL);
  numBytes += sPhasor_k_init(&sPhasor_sTQ2VfGS, 0.0f, sampleRate);
  numBytes += sDel1_init(&sDel1_QkoARA9l);
  numBytes += sSamphold_init(&sSamphold_Ury8CONE);
  numBytes += sPhasor_init(&sPhasor_DdNASZ6T, sampleRate);
  numBytes += sPhasor_init(&sPhasor_cEi439Ko, sampleRate);
  numBytes += sLine_init(&sLine_5h0nUEpP);
  numBytes += sRPole_init(&sRPole_2Q5c8Ysv);
  numBytes += cVar_init_f(&cVar_xw1mGq2l, 0.0f);
  numBytes += cVar_init_f(&cVar_LSJoNrY2, 0.0f);
  numBytes += cVar_init_f(&cVar_2ATKqOLa, 0.0f);
  numBytes += cBinop_init(&cBinop_xe9aWXXl, 29.0f); // __div
  numBytes += sVarf_init(&sVarf_dSFA78l2, 0.0f, 0.0f, false);
  numBytes += cRandom_init(&cRandom_hi2zdDbr, -73434684);
  numBytes += cSlice_init(&cSlice_OLPPfgnd, 1, 1);
  numBytes += cVar_init_f(&cVar_e7vqtAD6, 22050.0f);
  numBytes += cBinop_init(&cBinop_FbqwKN8C, 0.0f); // __mul
  numBytes += sVarf_init(&sVarf_b94bFdan, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_nK4OwyfG, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_dcB2rE3s, 0.0f);
  numBytes += cVar_init_f(&cVar_UVoHqOh4, 0.0f);
  numBytes += cRandom_init(&cRandom_uSIJJ4Yd, 2003194336);
  numBytes += cSlice_init(&cSlice_ilBkzoyV, 1, 1);
  numBytes += sVarf_init(&sVarf_v5Vyztih, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_kvrptBmn, 8000.0f);
  numBytes += cBinop_init(&cBinop_t64UPgqL, 0.0f); // __div
  numBytes += sVarf_init(&sVarf_ezmUOCtN, 0.0f, 0.0f, false);
  numBytes += cRandom_init(&cRandom_9lZV8lIN, 1661261459);
  numBytes += cSlice_init(&cSlice_gVdtbAVp, 1, 1);
  numBytes += sVari_init(&sVari_DZ63AjSt, 0, 0, false);
  numBytes += cRandom_init(&cRandom_Wz5qNoW8, -397976201);
  numBytes += cSlice_init(&cSlice_9BeB9bjR, 1, 1);
  numBytes += cRandom_init(&cRandom_0vxC7Z34, -1983757035);
  numBytes += cSlice_init(&cSlice_amEEHe9H, 1, 1);
  numBytes += cBinop_init(&cBinop_gabMzmGo, 0.0f); // __mul
  numBytes += sVarf_init(&sVarf_95VmBnGZ, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_nhejub5A, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_gMMntJe4, 0.0f, 0.0f, false);
  
  // schedule a message to trigger all loadbangs via the __hv_init receiver
  scheduleMessageForReceiver(0xCE5CC65B, msg_initWithBang(HV_MESSAGE_ON_STACK(1), 0));
}

Heavy_prog::~Heavy_prog() {
  // nothing to free
}

HvTable *Heavy_prog::getTableForHash(hv_uint32_t tableHash) {
  return nullptr;
}

void Heavy_prog::scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) {
  switch (receiverHash) {
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_1ec7vajz_sendMessage);
      break;
    }
    case 0x26682824: { // tempo
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_kJ2Ejwe6_sendMessage);
      break;
    }
    default: return;
  }
}

int Heavy_prog::getParameterInfo(int index, HvParameterInfo *info) {
  if (info != nullptr) {
    switch (index) {
      case 0: {
        info->name = "tempo";
        info->hash = 0x26682824;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 1.0f;
        info->defaultVal = 0.5f;
        break;
      }
      default: {
        info->name = "invalid parameter index";
        info->hash = 0;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 0.0f;
        info->defaultVal = 0.0f;
        break;
      }
    }
  }
  return 1;
}



/*
 * Send Function Implementations
 */


void Heavy_prog::cVar_xw1mGq2l_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_TLAVfKbd_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cVar_LSJoNrY2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 30.0f, 0, m, &cBinop_PYRsC6b4_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_gabMzmGo, HV_BINOP_MULTIPLY, 1, m, &cBinop_gabMzmGo_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 16.0f, 0, m, &cBinop_reXt6V8x_sendMessage);
}

void Heavy_prog::cVar_2ATKqOLa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_sTQ2VfGS, 0, m);
  cBinop_onMessage(_c, &Context(_c)->cBinop_gabMzmGo, HV_BINOP_MULTIPLY, 0, m, &cBinop_gabMzmGo_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 16.0f, 0, m, &cBinop_IEdVCssP_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_nhejub5A, m);
}

void Heavy_prog::cSwitchcase_TLAVfKbd_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Crms06qr_sendMessage);
      break;
    }
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Ru9D5eTR_sendMessage);
      break;
    }
    case 0x40000000: { // "2.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_a7sSzp4M_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_Crms06qr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_R4MospIj_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_xpor2Hpo_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_KVyU1YsZ_sendMessage);
}

void Heavy_prog::cCast_Ru9D5eTR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_PvAM3HGn_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_GaVAqUoP_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Ps7uBUGa_sendMessage);
}

void Heavy_prog::cCast_a7sSzp4M_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_QgccUOx0_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_lAEbZ5ez_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_MgSj2xnn_sendMessage);
}

void Heavy_prog::cCast_S28wJP3s_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_xe9aWXXl, HV_BINOP_DIVIDE, 0, m, &cBinop_xe9aWXXl_sendMessage);
}

void Heavy_prog::cCast_760JfNgQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_OGDiyBAQ_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_lAEbZ5ez_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_guseHSP8_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_MgSj2xnn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_EM2m4BuH_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_QgccUOx0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_GaVAqUoP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_0ap8GQGG_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_PvAM3HGn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_Ps7uBUGa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_EM2m4BuH_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_xpor2Hpo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_5EAXG1nA_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_R4MospIj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_KVyU1YsZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_EM2m4BuH_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_OGDiyBAQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setFloat(m, 1, 0.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_5h0nUEpP, 0, m, NULL);
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setElementToFrom(m, 1, n, 0);
  sLine_onMessage(_c, &Context(_c)->sLine_5h0nUEpP, 0, m, NULL);
}

void Heavy_prog::cBinop_xe9aWXXl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_dSFA78l2, m);
}

void Heavy_prog::cMsg_EM2m4BuH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 110.0f);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_760JfNgQ_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_S28wJP3s_sendMessage);
}

void Heavy_prog::cMsg_5EAXG1nA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 5.5f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_xe9aWXXl, HV_BINOP_DIVIDE, 1, m, &cBinop_xe9aWXXl_sendMessage);
}

void Heavy_prog::cMsg_0ap8GQGG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 25.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_xe9aWXXl, HV_BINOP_DIVIDE, 1, m, &cBinop_xe9aWXXl_sendMessage);
}

void Heavy_prog::cMsg_guseHSP8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 12.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_xe9aWXXl, HV_BINOP_DIVIDE, 1, m, &cBinop_xe9aWXXl_sendMessage);
}

void Heavy_prog::cSwitchcase_IIxr5hW1_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_OLPPfgnd, 0, m, &cSlice_OLPPfgnd_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_hi2zdDbr, 0, m, &cRandom_hi2zdDbr_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_aHi7jDCU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_Lp4tQ6Fe_sendMessage);
}

void Heavy_prog::cUnop_Lp4tQ6Fe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_2ATKqOLa, 0, m, &cVar_2ATKqOLa_sendMessage);
}

void Heavy_prog::cRandom_hi2zdDbr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_aHi7jDCU_sendMessage);
}

void Heavy_prog::cSlice_OLPPfgnd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_hi2zdDbr, 1, m, &cRandom_hi2zdDbr_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_e7vqtAD6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_FbqwKN8C, HV_BINOP_MULTIPLY, 0, m, &cBinop_FbqwKN8C_sendMessage);
}

void Heavy_prog::cMsg_Fm6xgjAz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_fjkJqE1s_sendMessage);
}

void Heavy_prog::cSystem_fjkJqE1s_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_BxWHJHBx_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_FbqwKN8C_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 1.0f, 0, m, &cBinop_1Lp8TTYH_sendMessage);
}

void Heavy_prog::cBinop_zWHfnYoh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_FbqwKN8C, HV_BINOP_MULTIPLY, 1, m, &cBinop_FbqwKN8C_sendMessage);
}

void Heavy_prog::cMsg_BxWHJHBx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 6.28319f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_zWHfnYoh_sendMessage);
}

void Heavy_prog::cBinop_1Lp8TTYH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.0f, 0, m, &cBinop_q3yTJPk0_sendMessage);
}

void Heavy_prog::cBinop_q3yTJPk0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 1.0f, 0, m, &cBinop_FkVf21JR_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_nK4OwyfG, m);
}

void Heavy_prog::cBinop_FkVf21JR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_b94bFdan, m);
}

void Heavy_prog::cVar_dcB2rE3s_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_e7vqtAD6, 0, m, &cVar_e7vqtAD6_sendMessage);
}

void Heavy_prog::cSwitchcase_VqlizZyx_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_G9zXpNo9_sendMessage);
      break;
    }
    default: {
      cMsg_LyuUczKA_sendMessage(_c, 0, m);
      break;
    }
  }
}

void Heavy_prog::cCast_G9zXpNo9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_9ComHiEW_sendMessage(_c, 0, m);
}

void Heavy_prog::cVar_UVoHqOh4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_e7vqtAD6, 0, m, &cVar_e7vqtAD6_sendMessage);
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_sTQ2VfGS, 1, m);
  cVar_onMessage(_c, &Context(_c)->cVar_kvrptBmn, 0, m, &cVar_kvrptBmn_sendMessage);
}

void Heavy_prog::cSwitchcase_padfYcwh_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_ilBkzoyV, 0, m, &cSlice_ilBkzoyV_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_uSIJJ4Yd, 0, m, &cRandom_uSIJJ4Yd_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_NwUGNv0N_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_3VHp7H64_sendMessage);
}

void Heavy_prog::cUnop_3VHp7H64_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_xw1mGq2l, 0, m, &cVar_xw1mGq2l_sendMessage);
}

void Heavy_prog::cRandom_uSIJJ4Yd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 4.0f, 0, m, &cBinop_NwUGNv0N_sendMessage);
}

void Heavy_prog::cSlice_ilBkzoyV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_uSIJJ4Yd, 1, m, &cRandom_uSIJJ4Yd_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_LFZMrhp8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.0f, 0, m, &cBinop_6phazKgA_sendMessage);
}

void Heavy_prog::cBinop_6phazKgA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_EHCsZvVf_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -1.0f, 0, m, &cBinop_fUNcRz2O_sendMessage);
}

void Heavy_prog::cVar_kvrptBmn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_wnYgtkBL_sendMessage);
}

void Heavy_prog::cMsg_UxParpD8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_rrg7hCWJ_sendMessage);
}

void Heavy_prog::cSystem_rrg7hCWJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_t64UPgqL, HV_BINOP_DIVIDE, 1, m, &cBinop_t64UPgqL_sendMessage);
}

void Heavy_prog::cBinop_EHCsZvVf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_VSVhLpgM_sendMessage);
}

void Heavy_prog::cBinop_VSVhLpgM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_ezmUOCtN, m);
}

void Heavy_prog::cMsg_7iTd2FU6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_dtUim4st_sendMessage);
}

void Heavy_prog::cBinop_dtUim4st_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 1.0f, 0, m, &cBinop_LFZMrhp8_sendMessage);
}

void Heavy_prog::cBinop_fUNcRz2O_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_v5Vyztih, m);
}

void Heavy_prog::cBinop_wnYgtkBL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 6.28319f, 0, m, &cBinop_b2RBHPAn_sendMessage);
}

void Heavy_prog::cBinop_b2RBHPAn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_t64UPgqL, HV_BINOP_DIVIDE, 0, m, &cBinop_t64UPgqL_sendMessage);
}

void Heavy_prog::cBinop_t64UPgqL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_7iTd2FU6_sendMessage(_c, 0, m);
}

void Heavy_prog::cSwitchcase_N45YWsX1_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_gVdtbAVp, 0, m, &cSlice_gVdtbAVp_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_9lZV8lIN, 0, m, &cRandom_9lZV8lIN_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_7RgykuMb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_6B2xnzHW_sendMessage);
}

void Heavy_prog::cUnop_6B2xnzHW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_pcYO1Zoj_sendMessage(_c, 0, m);
}

void Heavy_prog::cRandom_9lZV8lIN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8388610.0f, 0, m, &cBinop_7RgykuMb_sendMessage);
}

void Heavy_prog::cSlice_gVdtbAVp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_9lZV8lIN, 1, m, &cRandom_9lZV8lIN_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cMsg_pcYO1Zoj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 1.0f);
  sVari_onMessage(_c, &Context(_c)->sVari_DZ63AjSt, m);
}

void Heavy_prog::cSwitchcase_1Syeg9xA_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_9BeB9bjR, 0, m, &cSlice_9BeB9bjR_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_Wz5qNoW8, 0, m, &cRandom_Wz5qNoW8_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_cQkUE2hd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_PTUyVjmu_sendMessage);
}

void Heavy_prog::cUnop_PTUyVjmu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_VqlizZyx_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cRandom_Wz5qNoW8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 12.0f, 0, m, &cBinop_cQkUE2hd_sendMessage);
}

void Heavy_prog::cSlice_9BeB9bjR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_Wz5qNoW8, 1, m, &cRandom_Wz5qNoW8_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSwitchcase_Llp6bDeo_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_amEEHe9H, 0, m, &cSlice_amEEHe9H_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_0vxC7Z34, 0, m, &cRandom_0vxC7Z34_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_vsYlevBD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_kTpJxmnl_sendMessage);
}

void Heavy_prog::cUnop_kTpJxmnl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_LSJoNrY2, 0, m, &cVar_LSJoNrY2_sendMessage);
}

void Heavy_prog::cRandom_0vxC7Z34_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 100.0f, 0, m, &cBinop_vsYlevBD_sendMessage);
}

void Heavy_prog::cSlice_amEEHe9H_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_0vxC7Z34, 1, m, &cRandom_0vxC7Z34_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_PYRsC6b4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_dcB2rE3s, 0, m, &cVar_dcB2rE3s_sendMessage);
}

void Heavy_prog::cMsg_SWGs3knf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_GPM84qEL, 0, m, NULL);
}

void Heavy_prog::cMsg_LyuUczKA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 20.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_GPM84qEL, 0, m, NULL);
}

void Heavy_prog::cCast_fLM3Z4kR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_1Syeg9xA_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cCast_pPtLJXxT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_SWGs3knf_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_9ComHiEW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 80.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_GPM84qEL, 0, m, NULL);
}

void Heavy_prog::cBinop_gabMzmGo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 110.0f, 0, m, &cBinop_YVDBtW4U_sendMessage);
}

void Heavy_prog::cBinop_YVDBtW4U_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_UVoHqOh4, 0, m, &cVar_UVoHqOh4_sendMessage);
}

void Heavy_prog::cBinop_reXt6V8x_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_95VmBnGZ, m);
  sVarf_onMessage(_c, &Context(_c)->sVarf_nhejub5A, m);
}

void Heavy_prog::cBinop_IEdVCssP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_gMMntJe4, m);
}

void Heavy_prog::cReceive_1ec7vajz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_Fm6xgjAz_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_e7vqtAD6, 0, m, &cVar_e7vqtAD6_sendMessage);
  cMsg_UxParpD8_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_kvrptBmn, 0, m, &cVar_kvrptBmn_sendMessage);
  cSwitchcase_N45YWsX1_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cReceive_kJ2Ejwe6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_padfYcwh_onMessage(_c, NULL, 0, m, NULL);
  cSwitchcase_IIxr5hW1_onMessage(_c, NULL, 0, m, NULL);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_pPtLJXxT_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_fLM3Z4kR_sendMessage);
  cSwitchcase_Llp6bDeo_onMessage(_c, NULL, 0, m, NULL);
}




/*
 * Context Process Implementation
 */

int Heavy_prog::process(float **inputBuffers, float **outputBuffers, int n) {
  while (hLp_hasData(&inQueue)) {
    hv_uint32_t numBytes = 0;
    ReceiverMessagePair *p = reinterpret_cast<ReceiverMessagePair *>(hLp_getReadBuffer(&inQueue, &numBytes));
    hv_assert(numBytes >= sizeof(ReceiverMessagePair));
    scheduleMessageForReceiver(p->receiverHash, &p->msg);
    hLp_consume(&inQueue);
  }

  sendBangToReceiver(0xDD21C0EB); // send to __hv_bang~ on next cycle
  const int n4 = n & ~HV_N_SIMD_MASK; // ensure that the block size is a multiple of HV_N_SIMD

  // temporary signal vars
  hv_bufferf_t Bf0, Bf1, Bf2, Bf3, Bf4, Bf5, Bf6, Bf7, Bf8, Bf9, Bf10, Bf11, Bf12, Bf13;
  hv_bufferi_t Bi0, Bi1;

  // input and output vars
  hv_bufferf_t O0, O1;

  // declare and init the zero buffer
  hv_bufferf_t ZERO; __hv_zero_f(VOf(ZERO));

  hv_uint32_t nextBlock = blockStartTimestamp;
  for (int n = 0; n < n4; n += HV_N_SIMD) {

    // process all of the messages for this block
    nextBlock += HV_N_SIMD;
    while (mq_hasMessageBefore(&mq, nextBlock)) {
      MessageNode *const node = mq_peek(&mq);
      node->sendMessage(this, node->let, node->m);
      mq_pop(&mq);
    }

    

    // zero output buffers
    __hv_zero_f(VOf(O0));
    __hv_zero_f(VOf(O1));

    // process all signal functions
    __hv_varread_i(&sVari_DZ63AjSt, VOi(Bi0));
    __hv_var_k_i(VOi(Bi1), 16807, 16807, 16807, 16807, 16807, 16807, 16807, 16807);
    __hv_mul_i(VIi(Bi0), VIi(Bi1), VOi(Bi1));
    __hv_cast_if(VIi(Bi1), VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f);
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_varwrite_i(&sVari_DZ63AjSt, VIi(Bi1));
    __hv_varread_f(&sVarf_v5Vyztih, VOf(Bf0));
    __hv_rpole_f(&sRPole_CMFkc4Vk, VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf2), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_del1_f(&sDel1_0da2dfdo, VIf(Bf0), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf2), VOf(Bf2));
    __hv_sub_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_varread_f(&sVarf_ezmUOCtN, VOf(Bf0));
    __hv_mul_f(VIf(Bf2), VIf(Bf0), VOf(Bf0));
    __hv_line_f(&sLine_GPM84qEL, VOf(Bf2));
    __hv_mul_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_var_k_f(VOf(Bf0), 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f);
    __hv_phasor_k_f(&sPhasor_sTQ2VfGS, VOf(Bf3));
    __hv_del1_f(&sDel1_QkoARA9l, VIf(Bf3), VOf(Bf4));
    __hv_lt_f(VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_samphold_f(&sSamphold_Ury8CONE, VIf(Bf1), VIf(Bf4), VOf(Bf4));
    __hv_varread_f(&sVarf_95VmBnGZ, VOf(Bf5));
    __hv_mul_f(VIf(Bf4), VIf(Bf5), VOf(Bf5));
    __hv_var_k_f(VOf(Bf6), 256.0f, 256.0f, 256.0f, 256.0f, 256.0f, 256.0f, 256.0f, 256.0f);
    __hv_mul_f(VIf(Bf3), VIf(Bf6), VOf(Bf6));
    __hv_mul_f(VIf(Bf5), VIf(Bf6), VOf(Bf6));
    __hv_phasor_f(&sPhasor_DdNASZ6T, VIf(Bf6), VOf(Bf6));
    __hv_var_k_f(VOf(Bf5), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf6), VIf(Bf5), VOf(Bf5));
    __hv_abs_f(VIf(Bf5), VOf(Bf5));
    __hv_var_k_f(VOf(Bf6), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf5), VIf(Bf6), VOf(Bf6));
    __hv_var_k_f(VOf(Bf5), 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f);
    __hv_mul_f(VIf(Bf6), VIf(Bf5), VOf(Bf5));
    __hv_mul_f(VIf(Bf5), VIf(Bf5), VOf(Bf6));
    __hv_mul_f(VIf(Bf5), VIf(Bf6), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf6), VOf(Bf6));
    __hv_var_k_f(VOf(Bf7), 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f);
    __hv_var_k_f(VOf(Bf8), -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f);
    __hv_fma_f(VIf(Bf3), VIf(Bf8), VIf(Bf5), VOf(Bf5));
    __hv_fma_f(VIf(Bf6), VIf(Bf7), VIf(Bf5), VOf(Bf5));
    __hv_varread_f(&sVarf_nhejub5A, VOf(Bf7));
    __hv_fma_f(VIf(Bf5), VIf(Bf7), VIf(Bf4), VOf(Bf4));
    __hv_phasor_f(&sPhasor_cEi439Ko, VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf7), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf4), VIf(Bf7), VOf(Bf7));
    __hv_abs_f(VIf(Bf7), VOf(Bf7));
    __hv_var_k_f(VOf(Bf4), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf7), VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf7), 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f);
    __hv_mul_f(VIf(Bf4), VIf(Bf7), VOf(Bf7));
    __hv_mul_f(VIf(Bf7), VIf(Bf7), VOf(Bf4));
    __hv_mul_f(VIf(Bf7), VIf(Bf4), VOf(Bf6));
    __hv_mul_f(VIf(Bf6), VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf8), 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f);
    __hv_var_k_f(VOf(Bf3), -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f);
    __hv_fma_f(VIf(Bf6), VIf(Bf3), VIf(Bf7), VOf(Bf7));
    __hv_fma_f(VIf(Bf4), VIf(Bf8), VIf(Bf7), VOf(Bf7));
    __hv_line_f(&sLine_5h0nUEpP, VOf(Bf8));
    __hv_varread_f(&sVarf_dSFA78l2, VOf(Bf4));
    __hv_mul_f(VIf(Bf8), VIf(Bf4), VOf(Bf4));
    __hv_mul_f(VIf(Bf4), VIf(Bf4), VOf(Bf4));
    __hv_mul_f(VIf(Bf4), VIf(Bf8), VOf(Bf4));
    __hv_floor_f(VIf(Bf4), VOf(Bf3));
    __hv_sub_f(VIf(Bf4), VIf(Bf3), VOf(Bf3));
    __hv_var_k_f(VOf(Bf4), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_abs_f(VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf3), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf4), VIf(Bf3), VOf(Bf3));
    __hv_var_k_f(VOf(Bf4), 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f);
    __hv_mul_f(VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_mul_f(VIf(Bf4), VIf(Bf4), VOf(Bf3));
    __hv_mul_f(VIf(Bf4), VIf(Bf3), VOf(Bf6));
    __hv_mul_f(VIf(Bf6), VIf(Bf3), VOf(Bf9));
    __hv_mul_f(VIf(Bf9), VIf(Bf3), VOf(Bf10));
    __hv_mul_f(VIf(Bf10), VIf(Bf3), VOf(Bf3));
    __hv_var_k_f(VOf(Bf11), 2.75573e-06f, 2.75573e-06f, 2.75573e-06f, 2.75573e-06f, 2.75573e-06f, 2.75573e-06f, 2.75573e-06f, 2.75573e-06f);
    __hv_var_k_f(VOf(Bf12), 0.00833333f, 0.00833333f, 0.00833333f, 0.00833333f, 0.00833333f, 0.00833333f, 0.00833333f, 0.00833333f);
    __hv_var_k_f(VOf(Bf13), 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f);
    __hv_mul_f(VIf(Bf6), VIf(Bf13), VOf(Bf13));
    __hv_sub_f(VIf(Bf4), VIf(Bf13), VOf(Bf13));
    __hv_fma_f(VIf(Bf9), VIf(Bf12), VIf(Bf13), VOf(Bf13));
    __hv_var_k_f(VOf(Bf12), 0.000198413f, 0.000198413f, 0.000198413f, 0.000198413f, 0.000198413f, 0.000198413f, 0.000198413f, 0.000198413f);
    __hv_mul_f(VIf(Bf10), VIf(Bf12), VOf(Bf12));
    __hv_sub_f(VIf(Bf13), VIf(Bf12), VOf(Bf12));
    __hv_fma_f(VIf(Bf3), VIf(Bf11), VIf(Bf12), VOf(Bf12));
    __hv_mul_f(VIf(Bf12), VIf(Bf8), VOf(Bf8));
    __hv_var_k_f(VOf(Bf12), 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f);
    __hv_mul_f(VIf(Bf8), VIf(Bf12), VOf(Bf12));
    __hv_varread_f(&sVarf_gMMntJe4, VOf(Bf8));
    __hv_mul_f(VIf(Bf1), VIf(Bf8), VOf(Bf8));
    __hv_varread_f(&sVarf_nK4OwyfG, VOf(Bf1));
    __hv_mul_f(VIf(Bf8), VIf(Bf1), VOf(Bf1));
    __hv_varread_f(&sVarf_b94bFdan, VOf(Bf8));
    __hv_rpole_f(&sRPole_2Q5c8Ysv, VIf(Bf1), VIf(Bf8), VOf(Bf8));
    __hv_add_f(VIf(Bf12), VIf(Bf8), VOf(Bf8));
    __hv_var_k_f(VOf(Bf12), 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
    __hv_mul_f(VIf(Bf8), VIf(Bf12), VOf(Bf12));
    __hv_add_f(VIf(Bf7), VIf(Bf12), VOf(Bf7));
    __hv_fma_f(VIf(Bf2), VIf(Bf0), VIf(Bf7), VOf(Bf7));
    __hv_var_k_f(VOf(Bf0), 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
    __hv_mul_f(VIf(Bf7), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O1), VOf(O1));
    __hv_var_k_f(VOf(Bf0), 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f);
    __hv_add_f(VIf(Bf5), VIf(Bf12), VOf(Bf12));
    __hv_fma_f(VIf(Bf2), VIf(Bf0), VIf(Bf12), VOf(Bf12));
    __hv_var_k_f(VOf(Bf0), 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
    __hv_mul_f(VIf(Bf12), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O0), VOf(O0));

    // save output vars to output buffer
    __hv_store_f(outputBuffers[0]+n, VIf(O0));
    __hv_store_f(outputBuffers[1]+n, VIf(O1));
  }

  blockStartTimestamp = nextBlock;

  return n4; // return the number of frames processed

}

int Heavy_prog::processInline(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(!(n4 & HV_N_SIMD_MASK)); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s)
  float **const bIn = NULL;

  // define the heavy output buffer for 2 channel(s)
  float **const bOut = reinterpret_cast<float **>(hv_alloca(2*sizeof(float *)));
  bOut[0] = outputBuffers+(0*n4);
  bOut[1] = outputBuffers+(1*n4);

  int n = process(bIn, bOut, n4);
  return n;
}

int Heavy_prog::processInlineInterleaved(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(n4 & ~HV_N_SIMD_MASK); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s), uninterleave
  float *const bIn = NULL;

  // define the heavy output buffer for 2 channel(s)
  float *const bOut = reinterpret_cast<float *>(hv_alloca(2*n4*sizeof(float)));

  int n = processInline(bIn, bOut, n4);

  // interleave the heavy output into the output buffer
  #if HV_SIMD_AVX
  for (int i = 0, j = 0; j < n4; j += 8, i += 16) {
    __m256 x = _mm256_load_ps(bOut+j);    // LLLLLLLL
    __m256 y = _mm256_load_ps(bOut+n4+j); // RRRRRRRR
    __m256 a = _mm256_unpacklo_ps(x, y);  // LRLRLRLR
    __m256 b = _mm256_unpackhi_ps(x, y);  // LRLRLRLR
    _mm256_store_ps(outputBuffers+i, a);
    _mm256_store_ps(outputBuffers+8+i, b);
  }
  #elif HV_SIMD_SSE
  for (int i = 0, j = 0; j < n4; j += 4, i += 8) {
    __m128 x = _mm_load_ps(bOut+j);    // LLLL
    __m128 y = _mm_load_ps(bOut+n4+j); // RRRR
    __m128 a = _mm_unpacklo_ps(x, y);  // LRLR
    __m128 b = _mm_unpackhi_ps(x, y);  // LRLR
    _mm_store_ps(outputBuffers+i, a);
    _mm_store_ps(outputBuffers+4+i, b);
  }
  #elif HV_SIMD_NEON
  // https://community.arm.com/groups/processors/blog/2012/03/13/coding-for-neon--part-5-rearranging-vectors
  for (int i = 0, j = 0; j < n4; j += 4, i += 8) {
    float32x4_t x = vld1q_f32(bOut+j);
    float32x4_t y = vld1q_f32(bOut+n4+j);
    float32x4x2_t z = {x, y};
    vst2q_f32(outputBuffers+i, z); // interleave and store
  }
  #else // HV_SIMD_NONE
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < n4; ++j) {
      outputBuffers[i+2*j] = bOut[i*n4+j];
    }
  }
  #endif

  return n;
}
