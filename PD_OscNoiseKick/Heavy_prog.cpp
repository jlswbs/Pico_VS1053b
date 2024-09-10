/**
 * Copyright (c) 2024 Enzien Audio, Ltd.
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
  numBytes += sRPole_init(&sRPole_U3AyyFN9);
  numBytes += sDel1_init(&sDel1_atB57DLt);
  numBytes += sLine_init(&sLine_jP6Yv9M7);
  numBytes += sPhasor_k_init(&sPhasor_yq3VjBtN, 0.0f, sampleRate);
  numBytes += sDel1_init(&sDel1_ug2QJUr5);
  numBytes += sSamphold_init(&sSamphold_TaeXY8D9);
  numBytes += sPhasor_init(&sPhasor_oRTghQ0Z, sampleRate);
  numBytes += sLine_init(&sLine_aKYcwOkC);
  numBytes += sRPole_init(&sRPole_4WPzVv3k);
  numBytes += sPhasor_init(&sPhasor_UtaeKqfB, sampleRate);
  numBytes += cRandom_init(&cRandom_zUGicvE3, -964803548);
  numBytes += cSlice_init(&cSlice_DhLoIhh6, 1, 1);
  numBytes += sVari_init(&sVari_yJFvQ5Ii, 0, 0, false);
  numBytes += cVar_init_f(&cVar_wchjtvK9, 1.0f);
  numBytes += cVar_init_f(&cVar_tAwPoFln, 0.0f);
  numBytes += cVar_init_f(&cVar_QWNhS6fH, 0.0f);
  numBytes += cVar_init_f(&cVar_G9kodeZJ, 0.0f);
  numBytes += cBinop_init(&cBinop_8P3wCrD9, 29.0f); // __div
  numBytes += sVarf_init(&sVarf_PclCXoOw, 0.0f, 0.0f, false);
  numBytes += cDelay_init(this, &cDelay_TiKRMUZB, 0.0f);
  numBytes += cVar_init_f(&cVar_loGKzX95, 240.0f);
  numBytes += cBinop_init(&cBinop_sOqyfO0o, 0.0f); // __mul
  numBytes += cRandom_init(&cRandom_XXoRqAyl, 186637103);
  numBytes += cSlice_init(&cSlice_ybMA8YsU, 1, 1);
  numBytes += cRandom_init(&cRandom_lU48RUo4, -1957089922);
  numBytes += cSlice_init(&cSlice_8X3WAAQv, 1, 1);
  numBytes += cRandom_init(&cRandom_iAODcsuQ, 392487833);
  numBytes += cSlice_init(&cSlice_Mdsq61OW, 1, 1);
  numBytes += cVar_init_f(&cVar_297vmgBp, 22050.0f);
  numBytes += cBinop_init(&cBinop_mQBOJn8l, 0.0f); // __mul
  numBytes += sVarf_init(&sVarf_fhb6lLLw, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_9YFqBP2L, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_QwtqlZaJ, 0.0f);
  numBytes += sVarf_init(&sVarf_CrFxOtTg, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_Oz84MfUw, 8000.0f);
  numBytes += cBinop_init(&cBinop_cY6w0mBR, 0.0f); // __div
  numBytes += sVarf_init(&sVarf_SWQmZoHx, 0.0f, 0.0f, false);
  numBytes += cRandom_init(&cRandom_QZrypkBE, -1123377493);
  numBytes += cSlice_init(&cSlice_BQnqxFqG, 1, 1);
  numBytes += sVarf_init(&sVarf_Z0piCSG1, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_4nyFhAkT, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_lDNZhXfx, 0.0f, 0.0f, false);
  
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
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_7wW7IwxY_sendMessage);
      break;
    }
    default: return;
  }
}

int Heavy_prog::getParameterInfo(int index, HvParameterInfo *info) {
  if (info != nullptr) {
    switch (index) {
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
  return 0;
}



/*
 * Send Function Implementations
 */


void Heavy_prog::cSwitchcase_cZPUgsYg_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_DhLoIhh6, 0, m, &cSlice_DhLoIhh6_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_zUGicvE3, 0, m, &cRandom_zUGicvE3_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_NYeWmCng_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_8g3Dm7NT_sendMessage);
}

void Heavy_prog::cUnop_8g3Dm7NT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_TW4NpsGz_sendMessage(_c, 0, m);
}

void Heavy_prog::cRandom_zUGicvE3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8388610.0f, 0, m, &cBinop_NYeWmCng_sendMessage);
}

void Heavy_prog::cSlice_DhLoIhh6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_zUGicvE3, 1, m, &cRandom_zUGicvE3_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cMsg_TW4NpsGz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 1.0f);
  sVari_onMessage(_c, &Context(_c)->sVari_yJFvQ5Ii, m);
}

void Heavy_prog::cVar_wchjtvK9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_guYgovDy_sendMessage);
  cSwitchcase_8iqSS6dU_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cBinop_guYgovDy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_wchjtvK9, 1, m, &cVar_wchjtvK9_sendMessage);
}

void Heavy_prog::cVar_tAwPoFln_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_dk5Slxwf_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cVar_QWNhS6fH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 16.0f, 0, m, &cBinop_xbWz8xt7_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 30.0f, 0, m, &cBinop_0raPtbWp_sendMessage);
}

void Heavy_prog::cVar_G9kodeZJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_yq3VjBtN, 0, m);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 16.0f, 0, m, &cBinop_WGR8Ricx_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_4nyFhAkT, m);
}

void Heavy_prog::cSwitchcase_dk5Slxwf_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_wSfvld4k_sendMessage);
      break;
    }
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_dChBiwL1_sendMessage);
      break;
    }
    case 0x40000000: { // "2.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_6O3uu6Px_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_wSfvld4k_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_orM8JRqN_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_bIOIrE6Q_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Um08afEW_sendMessage);
}

void Heavy_prog::cCast_dChBiwL1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_O80kkpZ2_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_3PZmnvrL_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_r579iMiK_sendMessage);
}

void Heavy_prog::cCast_6O3uu6Px_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_f3Mx7zb6_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_P0fvKoWj_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_hKaLffcK_sendMessage);
}

void Heavy_prog::cCast_8Z4VmdJX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_AczV2sbU_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_Pz3U6jhR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_8P3wCrD9, HV_BINOP_DIVIDE, 0, m, &cBinop_8P3wCrD9_sendMessage);
}

void Heavy_prog::cCast_P0fvKoWj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_4rfQ4vZr_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_hKaLffcK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_5jynGwPh_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_f3Mx7zb6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_O80kkpZ2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_3PZmnvrL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_9bieD4BD_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_r579iMiK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_5jynGwPh_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_orM8JRqN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_Um08afEW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_5jynGwPh_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_bIOIrE6Q_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_6uJtQXvj_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_AczV2sbU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setFloat(m, 1, 0.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_aKYcwOkC, 0, m, NULL);
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setElementToFrom(m, 1, n, 0);
  sLine_onMessage(_c, &Context(_c)->sLine_aKYcwOkC, 0, m, NULL);
}

void Heavy_prog::cBinop_8P3wCrD9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_PclCXoOw, m);
}

void Heavy_prog::cMsg_5jynGwPh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 110.0f);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_8Z4VmdJX_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Pz3U6jhR_sendMessage);
}

void Heavy_prog::cMsg_4rfQ4vZr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 10.1f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_8P3wCrD9, HV_BINOP_DIVIDE, 1, m, &cBinop_8P3wCrD9_sendMessage);
}

void Heavy_prog::cMsg_9bieD4BD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 20.9f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_8P3wCrD9, HV_BINOP_DIVIDE, 1, m, &cBinop_8P3wCrD9_sendMessage);
}

void Heavy_prog::cMsg_6uJtQXvj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 5.5f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_8P3wCrD9, HV_BINOP_DIVIDE, 1, m, &cBinop_8P3wCrD9_sendMessage);
}

void Heavy_prog::cSwitchcase_8iqSS6dU_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cMsg_BXNrR52E_sendMessage(_c, 0, m);
      break;
    }
    case 0x7A5B032D: { // "stop"
      cMsg_BXNrR52E_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_MU64lNnU_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cDelay_TiKRMUZB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_TiKRMUZB, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_TiKRMUZB, 0, m, &cDelay_TiKRMUZB_sendMessage);
  cSwitchcase_WQ8znNW4_onMessage(_c, NULL, 0, m, NULL);
  cSwitchcase_WAUqy0cQ_onMessage(_c, NULL, 0, m, NULL);
  cSwitchcase_g9MK0rWC_onMessage(_c, NULL, 0, m, NULL);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_vvzThO1K_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_VWIVEYsc_sendMessage);
}

void Heavy_prog::cCast_MU64lNnU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_BXNrR52E_sendMessage(_c, 0, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_TiKRMUZB, 0, m, &cDelay_TiKRMUZB_sendMessage);
  cSwitchcase_WQ8znNW4_onMessage(_c, NULL, 0, m, NULL);
  cSwitchcase_WAUqy0cQ_onMessage(_c, NULL, 0, m, NULL);
  cSwitchcase_g9MK0rWC_onMessage(_c, NULL, 0, m, NULL);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_vvzThO1K_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_VWIVEYsc_sendMessage);
}

void Heavy_prog::cMsg_hDfxr1DV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_YDAsnWCI_sendMessage);
}

void Heavy_prog::cSystem_YDAsnWCI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_NR88xTYM_sendMessage);
}

void Heavy_prog::cVar_loGKzX95_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_sOqyfO0o, HV_BINOP_MULTIPLY, 0, m, &cBinop_sOqyfO0o_sendMessage);
}

void Heavy_prog::cMsg_BXNrR52E_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_TiKRMUZB, 0, m, &cDelay_TiKRMUZB_sendMessage);
}

void Heavy_prog::cBinop_YKi2OsEd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_TiKRMUZB, 2, m, &cDelay_TiKRMUZB_sendMessage);
}

void Heavy_prog::cBinop_NR88xTYM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_sOqyfO0o, HV_BINOP_MULTIPLY, 1, m, &cBinop_sOqyfO0o_sendMessage);
}

void Heavy_prog::cBinop_sOqyfO0o_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_YKi2OsEd_sendMessage);
}

void Heavy_prog::cSwitchcase_WQ8znNW4_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_ybMA8YsU, 0, m, &cSlice_ybMA8YsU_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_XXoRqAyl, 0, m, &cRandom_XXoRqAyl_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_Zu6LBoFP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_AXU7qkx9_sendMessage);
}

void Heavy_prog::cUnop_AXU7qkx9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_tAwPoFln, 0, m, &cVar_tAwPoFln_sendMessage);
}

void Heavy_prog::cRandom_XXoRqAyl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 3.0f, 0, m, &cBinop_Zu6LBoFP_sendMessage);
}

void Heavy_prog::cSlice_ybMA8YsU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_XXoRqAyl, 1, m, &cRandom_XXoRqAyl_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSwitchcase_WAUqy0cQ_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_8X3WAAQv, 0, m, &cSlice_8X3WAAQv_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_lU48RUo4, 0, m, &cRandom_lU48RUo4_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_gsDmdS7D_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_OVDlBpvU_sendMessage);
}

void Heavy_prog::cUnop_OVDlBpvU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_G9kodeZJ, 0, m, &cVar_G9kodeZJ_sendMessage);
}

void Heavy_prog::cRandom_lU48RUo4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_gsDmdS7D_sendMessage);
}

void Heavy_prog::cSlice_8X3WAAQv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_lU48RUo4, 1, m, &cRandom_lU48RUo4_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSwitchcase_g9MK0rWC_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_Mdsq61OW, 0, m, &cSlice_Mdsq61OW_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_iAODcsuQ, 0, m, &cRandom_iAODcsuQ_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_taUF3GpM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_QGecDwFf_sendMessage);
}

void Heavy_prog::cUnop_QGecDwFf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_QWNhS6fH, 0, m, &cVar_QWNhS6fH_sendMessage);
}

void Heavy_prog::cRandom_iAODcsuQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 100.0f, 0, m, &cBinop_taUF3GpM_sendMessage);
}

void Heavy_prog::cSlice_Mdsq61OW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_iAODcsuQ, 1, m, &cRandom_iAODcsuQ_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_297vmgBp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_mQBOJn8l, HV_BINOP_MULTIPLY, 0, m, &cBinop_mQBOJn8l_sendMessage);
}

void Heavy_prog::cMsg_94CuR5Qs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_qRnpecR2_sendMessage);
}

void Heavy_prog::cSystem_qRnpecR2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_52YxK3dP_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_mQBOJn8l_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 1.0f, 0, m, &cBinop_U30s890X_sendMessage);
}

void Heavy_prog::cBinop_fQ2LdWRt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_mQBOJn8l, HV_BINOP_MULTIPLY, 1, m, &cBinop_mQBOJn8l_sendMessage);
}

void Heavy_prog::cMsg_52YxK3dP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 6.28319f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_fQ2LdWRt_sendMessage);
}

void Heavy_prog::cBinop_U30s890X_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.0f, 0, m, &cBinop_r6WFYjnv_sendMessage);
}

void Heavy_prog::cBinop_r6WFYjnv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 1.0f, 0, m, &cBinop_gV9yGn5m_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_9YFqBP2L, m);
}

void Heavy_prog::cBinop_gV9yGn5m_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_fhb6lLLw, m);
}

void Heavy_prog::cVar_QwtqlZaJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_297vmgBp, 0, m, &cVar_297vmgBp_sendMessage);
}

void Heavy_prog::cSwitchcase_gbqzlpjR_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_fwTox30l_sendMessage);
      break;
    }
    default: {
      cMsg_45GMHYam_sendMessage(_c, 0, m);
      break;
    }
  }
}

void Heavy_prog::cCast_fwTox30l_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_YbMKMNi2_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_ruOhQBcX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.0f, 0, m, &cBinop_mW3Yr8rQ_sendMessage);
}

void Heavy_prog::cBinop_mW3Yr8rQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_JQmo5X5m_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -1.0f, 0, m, &cBinop_Hc2Kedaw_sendMessage);
}

void Heavy_prog::cVar_Oz84MfUw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_v8SdWfGD_sendMessage);
}

void Heavy_prog::cMsg_morIu36e_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_4sQLst0N_sendMessage);
}

void Heavy_prog::cSystem_4sQLst0N_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_cY6w0mBR, HV_BINOP_DIVIDE, 1, m, &cBinop_cY6w0mBR_sendMessage);
}

void Heavy_prog::cBinop_JQmo5X5m_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_wox0aYxS_sendMessage);
}

void Heavy_prog::cBinop_wox0aYxS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_SWQmZoHx, m);
}

void Heavy_prog::cMsg_2H0ePoKc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_g3uBmuTL_sendMessage);
}

void Heavy_prog::cBinop_g3uBmuTL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 1.0f, 0, m, &cBinop_ruOhQBcX_sendMessage);
}

void Heavy_prog::cBinop_Hc2Kedaw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_CrFxOtTg, m);
}

void Heavy_prog::cBinop_v8SdWfGD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 6.28319f, 0, m, &cBinop_WjuOAgTy_sendMessage);
}

void Heavy_prog::cBinop_WjuOAgTy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_cY6w0mBR, HV_BINOP_DIVIDE, 0, m, &cBinop_cY6w0mBR_sendMessage);
}

void Heavy_prog::cBinop_cY6w0mBR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_2H0ePoKc_sendMessage(_c, 0, m);
}

void Heavy_prog::cSwitchcase_pe1Cg85U_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_BQnqxFqG, 0, m, &cSlice_BQnqxFqG_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_QZrypkBE, 0, m, &cRandom_QZrypkBE_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_3D1Xkujt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_MpkvuXyS_sendMessage);
}

void Heavy_prog::cUnop_MpkvuXyS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_gbqzlpjR_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cRandom_QZrypkBE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_3D1Xkujt_sendMessage);
}

void Heavy_prog::cSlice_BQnqxFqG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_QZrypkBE, 1, m, &cRandom_QZrypkBE_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_xbWz8xt7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_Z0piCSG1, m);
  sVarf_onMessage(_c, &Context(_c)->sVarf_4nyFhAkT, m);
}

void Heavy_prog::cBinop_0raPtbWp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_QwtqlZaJ, 0, m, &cVar_QwtqlZaJ_sendMessage);
}

void Heavy_prog::cMsg_5iMo9HXG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_jP6Yv9M7, 0, m, NULL);
}

void Heavy_prog::cMsg_45GMHYam_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 20.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_jP6Yv9M7, 0, m, NULL);
}

void Heavy_prog::cCast_vvzThO1K_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_5iMo9HXG_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_VWIVEYsc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_pe1Cg85U_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cMsg_YbMKMNi2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 80.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_jP6Yv9M7, 0, m, NULL);
}

void Heavy_prog::cBinop_WGR8Ricx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_lDNZhXfx, m);
}

void Heavy_prog::cReceive_7wW7IwxY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_cZPUgsYg_onMessage(_c, NULL, 0, m, NULL);
  cMsg_hDfxr1DV_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_loGKzX95, 0, m, &cVar_loGKzX95_sendMessage);
  cMsg_94CuR5Qs_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_297vmgBp, 0, m, &cVar_297vmgBp_sendMessage);
  cMsg_morIu36e_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_Oz84MfUw, 0, m, &cVar_Oz84MfUw_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_wchjtvK9, 0, m, &cVar_wchjtvK9_sendMessage);
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
  hv_bufferf_t Bf0, Bf1, Bf2, Bf3, Bf4, Bf5, Bf6, Bf7, Bf8, Bf9, Bf10, Bf11;
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
    __hv_varread_i(&sVari_yJFvQ5Ii, VOi(Bi0));
    __hv_var_k_i(VOi(Bi1), 16807, 16807, 16807, 16807, 16807, 16807, 16807, 16807);
    __hv_mul_i(VIi(Bi0), VIi(Bi1), VOi(Bi1));
    __hv_cast_if(VIi(Bi1), VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f);
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_varwrite_i(&sVari_yJFvQ5Ii, VIi(Bi1));
    __hv_varread_f(&sVarf_CrFxOtTg, VOf(Bf0));
    __hv_rpole_f(&sRPole_U3AyyFN9, VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf2), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_del1_f(&sDel1_atB57DLt, VIf(Bf0), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf2), VOf(Bf2));
    __hv_sub_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_varread_f(&sVarf_SWQmZoHx, VOf(Bf0));
    __hv_mul_f(VIf(Bf2), VIf(Bf0), VOf(Bf0));
    __hv_line_f(&sLine_jP6Yv9M7, VOf(Bf2));
    __hv_mul_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_var_k_f(VOf(Bf0), 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f);
    __hv_phasor_k_f(&sPhasor_yq3VjBtN, VOf(Bf3));
    __hv_del1_f(&sDel1_ug2QJUr5, VIf(Bf3), VOf(Bf4));
    __hv_lt_f(VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_samphold_f(&sSamphold_TaeXY8D9, VIf(Bf1), VIf(Bf4), VOf(Bf4));
    __hv_varread_f(&sVarf_Z0piCSG1, VOf(Bf5));
    __hv_mul_f(VIf(Bf4), VIf(Bf5), VOf(Bf5));
    __hv_var_k_f(VOf(Bf6), 128.0f, 128.0f, 128.0f, 128.0f, 128.0f, 128.0f, 128.0f, 128.0f);
    __hv_mul_f(VIf(Bf3), VIf(Bf6), VOf(Bf6));
    __hv_mul_f(VIf(Bf5), VIf(Bf6), VOf(Bf6));
    __hv_phasor_f(&sPhasor_oRTghQ0Z, VIf(Bf6), VOf(Bf6));
    __hv_var_k_f(VOf(Bf3), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf6), VIf(Bf3), VOf(Bf3));
    __hv_abs_f(VIf(Bf3), VOf(Bf3));
    __hv_var_k_f(VOf(Bf6), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf3), VIf(Bf6), VOf(Bf6));
    __hv_var_k_f(VOf(Bf3), 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f);
    __hv_mul_f(VIf(Bf6), VIf(Bf3), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf3), VOf(Bf6));
    __hv_mul_f(VIf(Bf3), VIf(Bf6), VOf(Bf7));
    __hv_mul_f(VIf(Bf7), VIf(Bf6), VOf(Bf6));
    __hv_var_k_f(VOf(Bf8), 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f);
    __hv_var_k_f(VOf(Bf9), -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f);
    __hv_fma_f(VIf(Bf7), VIf(Bf9), VIf(Bf3), VOf(Bf3));
    __hv_fma_f(VIf(Bf6), VIf(Bf8), VIf(Bf3), VOf(Bf3));
    __hv_line_f(&sLine_aKYcwOkC, VOf(Bf8));
    __hv_varread_f(&sVarf_PclCXoOw, VOf(Bf6));
    __hv_mul_f(VIf(Bf8), VIf(Bf6), VOf(Bf6));
    __hv_mul_f(VIf(Bf6), VIf(Bf6), VOf(Bf6));
    __hv_mul_f(VIf(Bf6), VIf(Bf8), VOf(Bf6));
    __hv_floor_f(VIf(Bf6), VOf(Bf9));
    __hv_sub_f(VIf(Bf6), VIf(Bf9), VOf(Bf9));
    __hv_var_k_f(VOf(Bf6), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf9), VIf(Bf6), VOf(Bf6));
    __hv_abs_f(VIf(Bf6), VOf(Bf6));
    __hv_var_k_f(VOf(Bf9), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf6), VIf(Bf9), VOf(Bf9));
    __hv_var_k_f(VOf(Bf6), 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f);
    __hv_mul_f(VIf(Bf9), VIf(Bf6), VOf(Bf6));
    __hv_mul_f(VIf(Bf6), VIf(Bf6), VOf(Bf9));
    __hv_mul_f(VIf(Bf6), VIf(Bf9), VOf(Bf7));
    __hv_mul_f(VIf(Bf7), VIf(Bf9), VOf(Bf9));
    __hv_var_k_f(VOf(Bf10), 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f);
    __hv_var_k_f(VOf(Bf11), 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f);
    __hv_mul_f(VIf(Bf7), VIf(Bf11), VOf(Bf11));
    __hv_sub_f(VIf(Bf6), VIf(Bf11), VOf(Bf11));
    __hv_fma_f(VIf(Bf9), VIf(Bf10), VIf(Bf11), VOf(Bf11));
    __hv_mul_f(VIf(Bf11), VIf(Bf8), VOf(Bf8));
    __hv_var_k_f(VOf(Bf11), 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f);
    __hv_mul_f(VIf(Bf8), VIf(Bf11), VOf(Bf11));
    __hv_varread_f(&sVarf_lDNZhXfx, VOf(Bf8));
    __hv_mul_f(VIf(Bf1), VIf(Bf8), VOf(Bf8));
    __hv_varread_f(&sVarf_9YFqBP2L, VOf(Bf1));
    __hv_mul_f(VIf(Bf8), VIf(Bf1), VOf(Bf1));
    __hv_varread_f(&sVarf_fhb6lLLw, VOf(Bf8));
    __hv_rpole_f(&sRPole_4WPzVv3k, VIf(Bf1), VIf(Bf8), VOf(Bf8));
    __hv_add_f(VIf(Bf11), VIf(Bf8), VOf(Bf8));
    __hv_var_k_f(VOf(Bf11), 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
    __hv_mul_f(VIf(Bf8), VIf(Bf11), VOf(Bf11));
    __hv_add_f(VIf(Bf3), VIf(Bf11), VOf(Bf8));
    __hv_fma_f(VIf(Bf2), VIf(Bf0), VIf(Bf8), VOf(Bf8));
    __hv_var_k_f(VOf(Bf0), 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
    __hv_mul_f(VIf(Bf8), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O0), VOf(O0));
    __hv_var_k_f(VOf(Bf0), 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
    __hv_add_f(VIf(Bf3), VIf(Bf5), VOf(Bf5));
    __hv_varread_f(&sVarf_4nyFhAkT, VOf(Bf3));
    __hv_fma_f(VIf(Bf5), VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_phasor_f(&sPhasor_UtaeKqfB, VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf3), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf4), VIf(Bf3), VOf(Bf3));
    __hv_abs_f(VIf(Bf3), VOf(Bf3));
    __hv_var_k_f(VOf(Bf4), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf3), 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f);
    __hv_mul_f(VIf(Bf4), VIf(Bf3), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf3), VOf(Bf4));
    __hv_mul_f(VIf(Bf3), VIf(Bf4), VOf(Bf5));
    __hv_mul_f(VIf(Bf5), VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf8), 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f);
    __hv_var_k_f(VOf(Bf1), -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f);
    __hv_fma_f(VIf(Bf5), VIf(Bf1), VIf(Bf3), VOf(Bf3));
    __hv_fma_f(VIf(Bf4), VIf(Bf8), VIf(Bf3), VOf(Bf3));
    __hv_add_f(VIf(Bf3), VIf(Bf11), VOf(Bf11));
    __hv_fma_f(VIf(Bf2), VIf(Bf0), VIf(Bf11), VOf(Bf11));
    __hv_var_k_f(VOf(Bf0), 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
    __hv_mul_f(VIf(Bf11), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O1), VOf(O1));

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
