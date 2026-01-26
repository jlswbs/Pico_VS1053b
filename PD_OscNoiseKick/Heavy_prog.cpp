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
  numBytes += sRPole_init(&sRPole_fVALYJ5g);
  numBytes += sDel1_init(&sDel1_MDBTTKDU);
  numBytes += sLine_init(&sLine_wMLytt6S);
  numBytes += sPhasor_k_init(&sPhasor_ndfG8Sy2, 0.0f, sampleRate);
  numBytes += sDel1_init(&sDel1_5RE5oKdi);
  numBytes += sSamphold_init(&sSamphold_v7TLWLo9);
  numBytes += sPhasor_init(&sPhasor_MVSIYAQ2, sampleRate);
  numBytes += sPhasor_init(&sPhasor_ILH3Kzax, sampleRate);
  numBytes += sLine_init(&sLine_NJ9KOKSm);
  numBytes += sRPole_init(&sRPole_V92cpwNw);
  numBytes += cVar_init_f(&cVar_Fyf3KaMx, 0.0f);
  numBytes += cVar_init_f(&cVar_WP9CysuC, 0.0f);
  numBytes += cVar_init_f(&cVar_8ekAWnCB, 0.0f);
  numBytes += cBinop_init(&cBinop_uQUvdkK7, 29.0f); // __div
  numBytes += sVarf_init(&sVarf_HRuactSx, 0.0f, 0.0f, false);
  numBytes += cRandom_init(&cRandom_kcrAUz4k, 1470437499);
  numBytes += cSlice_init(&cSlice_zDnPJmOD, 1, 1);
  numBytes += cVar_init_f(&cVar_OEzkZyz7, 22050.0f);
  numBytes += cBinop_init(&cBinop_Z6DZ4m6N, 0.0f); // __mul
  numBytes += sVarf_init(&sVarf_kxIdumy8, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_vZ3freh8, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_L4gHzjFo, 0.0f);
  numBytes += cVar_init_f(&cVar_9WVqBWGL, 0.0f);
  numBytes += cRandom_init(&cRandom_2Zpi6uD8, 46926707);
  numBytes += cSlice_init(&cSlice_aKh6QNu7, 1, 1);
  numBytes += sVarf_init(&sVarf_LfbkmlPF, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_nVhFXPsO, 8000.0f);
  numBytes += cBinop_init(&cBinop_QDww6r23, 0.0f); // __div
  numBytes += sVarf_init(&sVarf_zWT1IZPq, 0.0f, 0.0f, false);
  numBytes += cRandom_init(&cRandom_kEUpe3Ez, -1339467175);
  numBytes += cSlice_init(&cSlice_tiBg7swR, 1, 1);
  numBytes += sVari_init(&sVari_O1SIByJc, 0, 0, false);
  numBytes += cRandom_init(&cRandom_0rW6epQm, -534989812);
  numBytes += cSlice_init(&cSlice_HppREbMU, 1, 1);
  numBytes += cRandom_init(&cRandom_PWdriHN8, 1238109043);
  numBytes += cSlice_init(&cSlice_FqmV7XIQ, 1, 1);
  numBytes += cBinop_init(&cBinop_I5qhDPya, 0.0f); // __mul
  numBytes += sVarf_init(&sVarf_yubGt1V0, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_c7luVdha, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_x3JNJEoy, 0.0f, 0.0f, false);
  
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
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_dY6z9ZgY_sendMessage);
      break;
    }
    case 0x26682824: { // tempo
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_ZOfw8fDb_sendMessage);
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


void Heavy_prog::cVar_Fyf3KaMx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_87W5gmI1_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cVar_WP9CysuC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 30.0f, 0, m, &cBinop_jY4kWi8M_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_I5qhDPya, HV_BINOP_MULTIPLY, 1, m, &cBinop_I5qhDPya_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 16.0f, 0, m, &cBinop_5vMJSCzr_sendMessage);
}

void Heavy_prog::cVar_8ekAWnCB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_ndfG8Sy2, 0, m);
  cBinop_onMessage(_c, &Context(_c)->cBinop_I5qhDPya, HV_BINOP_MULTIPLY, 0, m, &cBinop_I5qhDPya_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 16.0f, 0, m, &cBinop_eTp9IeV0_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_c7luVdha, m);
}

void Heavy_prog::cSwitchcase_87W5gmI1_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_cSQXXcQI_sendMessage);
      break;
    }
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_4RixOwED_sendMessage);
      break;
    }
    case 0x40000000: { // "2.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_83awyfce_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_cSQXXcQI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_0xTbxmHk_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_wtQFAqeo_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_WhzKULQ4_sendMessage);
}

void Heavy_prog::cCast_4RixOwED_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_pKr3EurY_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_30qA1eFC_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_9N0h6jUR_sendMessage);
}

void Heavy_prog::cCast_83awyfce_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_ZdrLqIt1_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_cuJ6PLt2_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_g7sgOXVl_sendMessage);
}

void Heavy_prog::cCast_hc5YNCkc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_uQUvdkK7, HV_BINOP_DIVIDE, 0, m, &cBinop_uQUvdkK7_sendMessage);
}

void Heavy_prog::cCast_S2LMpnIi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_tGLvh64M_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_cuJ6PLt2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_EotkF4JC_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_ZdrLqIt1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_g7sgOXVl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_hIwbLsfr_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_9N0h6jUR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_hIwbLsfr_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_pKr3EurY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_30qA1eFC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_eOM6YRZC_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_0xTbxmHk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_WhzKULQ4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_hIwbLsfr_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_wtQFAqeo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_EwXplAcX_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_tGLvh64M_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setFloat(m, 1, 0.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_NJ9KOKSm, 0, m, NULL);
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setElementToFrom(m, 1, n, 0);
  sLine_onMessage(_c, &Context(_c)->sLine_NJ9KOKSm, 0, m, NULL);
}

void Heavy_prog::cBinop_uQUvdkK7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_HRuactSx, m);
}

void Heavy_prog::cMsg_hIwbLsfr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 110.0f);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_S2LMpnIi_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_hc5YNCkc_sendMessage);
}

void Heavy_prog::cMsg_EwXplAcX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 5.5f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_uQUvdkK7, HV_BINOP_DIVIDE, 1, m, &cBinop_uQUvdkK7_sendMessage);
}

void Heavy_prog::cMsg_eOM6YRZC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 25.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_uQUvdkK7, HV_BINOP_DIVIDE, 1, m, &cBinop_uQUvdkK7_sendMessage);
}

void Heavy_prog::cMsg_EotkF4JC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 12.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_uQUvdkK7, HV_BINOP_DIVIDE, 1, m, &cBinop_uQUvdkK7_sendMessage);
}

void Heavy_prog::cSwitchcase_FH1SIaHg_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_zDnPJmOD, 0, m, &cSlice_zDnPJmOD_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_kcrAUz4k, 0, m, &cRandom_kcrAUz4k_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_phSsGhPg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_DHVZY6LU_sendMessage);
}

void Heavy_prog::cUnop_DHVZY6LU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_8ekAWnCB, 0, m, &cVar_8ekAWnCB_sendMessage);
}

void Heavy_prog::cRandom_kcrAUz4k_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_phSsGhPg_sendMessage);
}

void Heavy_prog::cSlice_zDnPJmOD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_kcrAUz4k, 1, m, &cRandom_kcrAUz4k_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_OEzkZyz7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Z6DZ4m6N, HV_BINOP_MULTIPLY, 0, m, &cBinop_Z6DZ4m6N_sendMessage);
}

void Heavy_prog::cMsg_oYttykXC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_voXfLKuS_sendMessage);
}

void Heavy_prog::cSystem_voXfLKuS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_PwVnI789_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_Z6DZ4m6N_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 1.0f, 0, m, &cBinop_2S13Q4Ra_sendMessage);
}

void Heavy_prog::cBinop_wNiKfITA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Z6DZ4m6N, HV_BINOP_MULTIPLY, 1, m, &cBinop_Z6DZ4m6N_sendMessage);
}

void Heavy_prog::cMsg_PwVnI789_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 6.28319f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_wNiKfITA_sendMessage);
}

void Heavy_prog::cBinop_2S13Q4Ra_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.0f, 0, m, &cBinop_4J9gsaN9_sendMessage);
}

void Heavy_prog::cBinop_4J9gsaN9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 1.0f, 0, m, &cBinop_VSORPMYj_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_vZ3freh8, m);
}

void Heavy_prog::cBinop_VSORPMYj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_kxIdumy8, m);
}

void Heavy_prog::cVar_L4gHzjFo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_OEzkZyz7, 0, m, &cVar_OEzkZyz7_sendMessage);
}

void Heavy_prog::cSwitchcase_6aTj8MRG_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_qM7TcyVa_sendMessage);
      break;
    }
    default: {
      cMsg_MKXyn3Lf_sendMessage(_c, 0, m);
      break;
    }
  }
}

void Heavy_prog::cCast_qM7TcyVa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_uhBSJ6jO_sendMessage(_c, 0, m);
}

void Heavy_prog::cVar_9WVqBWGL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_OEzkZyz7, 0, m, &cVar_OEzkZyz7_sendMessage);
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_ndfG8Sy2, 1, m);
  cVar_onMessage(_c, &Context(_c)->cVar_nVhFXPsO, 0, m, &cVar_nVhFXPsO_sendMessage);
}

void Heavy_prog::cSwitchcase_08JU16lW_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_aKh6QNu7, 0, m, &cSlice_aKh6QNu7_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_2Zpi6uD8, 0, m, &cRandom_2Zpi6uD8_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_DB2L4Kpb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_Ton1iR5i_sendMessage);
}

void Heavy_prog::cUnop_Ton1iR5i_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_Fyf3KaMx, 0, m, &cVar_Fyf3KaMx_sendMessage);
}

void Heavy_prog::cRandom_2Zpi6uD8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 4.0f, 0, m, &cBinop_DB2L4Kpb_sendMessage);
}

void Heavy_prog::cSlice_aKh6QNu7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_2Zpi6uD8, 1, m, &cRandom_2Zpi6uD8_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_hOIVa3qd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.0f, 0, m, &cBinop_4NnMiXWZ_sendMessage);
}

void Heavy_prog::cBinop_4NnMiXWZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_wncP78dr_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -1.0f, 0, m, &cBinop_1ZvktRSX_sendMessage);
}

void Heavy_prog::cVar_nVhFXPsO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_MVtdXCS4_sendMessage);
}

void Heavy_prog::cMsg_SCrIvVnv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_vjqHYM8w_sendMessage);
}

void Heavy_prog::cSystem_vjqHYM8w_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_QDww6r23, HV_BINOP_DIVIDE, 1, m, &cBinop_QDww6r23_sendMessage);
}

void Heavy_prog::cBinop_wncP78dr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_OwjoBzFd_sendMessage);
}

void Heavy_prog::cBinop_OwjoBzFd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_zWT1IZPq, m);
}

void Heavy_prog::cMsg_xqOh6oa8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_6zvg6rqB_sendMessage);
}

void Heavy_prog::cBinop_6zvg6rqB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 1.0f, 0, m, &cBinop_hOIVa3qd_sendMessage);
}

void Heavy_prog::cBinop_1ZvktRSX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_LfbkmlPF, m);
}

void Heavy_prog::cBinop_MVtdXCS4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 6.28319f, 0, m, &cBinop_Now2skPI_sendMessage);
}

void Heavy_prog::cBinop_Now2skPI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_QDww6r23, HV_BINOP_DIVIDE, 0, m, &cBinop_QDww6r23_sendMessage);
}

void Heavy_prog::cBinop_QDww6r23_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_xqOh6oa8_sendMessage(_c, 0, m);
}

void Heavy_prog::cSwitchcase_HbTKcl9j_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_tiBg7swR, 0, m, &cSlice_tiBg7swR_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_kEUpe3Ez, 0, m, &cRandom_kEUpe3Ez_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_TfgUgUOs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_gTktGCvm_sendMessage);
}

void Heavy_prog::cUnop_gTktGCvm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_xyEyMaNk_sendMessage(_c, 0, m);
}

void Heavy_prog::cRandom_kEUpe3Ez_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8388610.0f, 0, m, &cBinop_TfgUgUOs_sendMessage);
}

void Heavy_prog::cSlice_tiBg7swR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_kEUpe3Ez, 1, m, &cRandom_kEUpe3Ez_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cMsg_xyEyMaNk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 1.0f);
  sVari_onMessage(_c, &Context(_c)->sVari_O1SIByJc, m);
}

void Heavy_prog::cSwitchcase_5tEhAC34_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_HppREbMU, 0, m, &cSlice_HppREbMU_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_0rW6epQm, 0, m, &cRandom_0rW6epQm_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_p81Vs5ZL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_6vjbkvMc_sendMessage);
}

void Heavy_prog::cUnop_6vjbkvMc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_6aTj8MRG_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cRandom_0rW6epQm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 12.0f, 0, m, &cBinop_p81Vs5ZL_sendMessage);
}

void Heavy_prog::cSlice_HppREbMU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_0rW6epQm, 1, m, &cRandom_0rW6epQm_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSwitchcase_t2jfKJeH_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_FqmV7XIQ, 0, m, &cSlice_FqmV7XIQ_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_PWdriHN8, 0, m, &cRandom_PWdriHN8_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_zRSLw1sW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_ZJzEbObe_sendMessage);
}

void Heavy_prog::cUnop_ZJzEbObe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_WP9CysuC, 0, m, &cVar_WP9CysuC_sendMessage);
}

void Heavy_prog::cRandom_PWdriHN8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 100.0f, 0, m, &cBinop_zRSLw1sW_sendMessage);
}

void Heavy_prog::cSlice_FqmV7XIQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_PWdriHN8, 1, m, &cRandom_PWdriHN8_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_jY4kWi8M_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_L4gHzjFo, 0, m, &cVar_L4gHzjFo_sendMessage);
}

void Heavy_prog::cMsg_qH2scIH0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_wMLytt6S, 0, m, NULL);
}

void Heavy_prog::cMsg_MKXyn3Lf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 20.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_wMLytt6S, 0, m, NULL);
}

void Heavy_prog::cCast_MIg4GjYP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_qH2scIH0_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_4VaoAHoj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_5tEhAC34_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cMsg_uhBSJ6jO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 80.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_wMLytt6S, 0, m, NULL);
}

void Heavy_prog::cBinop_I5qhDPya_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 110.0f, 0, m, &cBinop_mEvytSox_sendMessage);
  cMsg_QApHSFQS_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_mEvytSox_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_9WVqBWGL, 0, m, &cVar_9WVqBWGL_sendMessage);
}

void Heavy_prog::cMsg_QApHSFQS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "seed");
  msg_setFloat(m, 1, 123.0f);
  cMsg_xyEyMaNk_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_5vMJSCzr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_yubGt1V0, m);
  sVarf_onMessage(_c, &Context(_c)->sVarf_c7luVdha, m);
}

void Heavy_prog::cBinop_eTp9IeV0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_x3JNJEoy, m);
}

void Heavy_prog::cReceive_dY6z9ZgY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_oYttykXC_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_OEzkZyz7, 0, m, &cVar_OEzkZyz7_sendMessage);
  cMsg_SCrIvVnv_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_nVhFXPsO, 0, m, &cVar_nVhFXPsO_sendMessage);
  cSwitchcase_HbTKcl9j_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cReceive_ZOfw8fDb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_08JU16lW_onMessage(_c, NULL, 0, m, NULL);
  cSwitchcase_FH1SIaHg_onMessage(_c, NULL, 0, m, NULL);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_MIg4GjYP_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_4VaoAHoj_sendMessage);
  cSwitchcase_t2jfKJeH_onMessage(_c, NULL, 0, m, NULL);
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
    __hv_varread_i(&sVari_O1SIByJc, VOi(Bi0));
    __hv_var_k_i(VOi(Bi1), 16807, 16807, 16807, 16807, 16807, 16807, 16807, 16807);
    __hv_mul_i(VIi(Bi0), VIi(Bi1), VOi(Bi1));
    __hv_cast_if(VIi(Bi1), VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f);
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_varwrite_i(&sVari_O1SIByJc, VIi(Bi1));
    __hv_varread_f(&sVarf_LfbkmlPF, VOf(Bf0));
    __hv_rpole_f(&sRPole_fVALYJ5g, VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf2), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_del1_f(&sDel1_MDBTTKDU, VIf(Bf0), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf2), VOf(Bf2));
    __hv_sub_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_varread_f(&sVarf_zWT1IZPq, VOf(Bf0));
    __hv_mul_f(VIf(Bf2), VIf(Bf0), VOf(Bf0));
    __hv_line_f(&sLine_wMLytt6S, VOf(Bf2));
    __hv_mul_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_var_k_f(VOf(Bf0), 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f);
    __hv_phasor_k_f(&sPhasor_ndfG8Sy2, VOf(Bf3));
    __hv_del1_f(&sDel1_5RE5oKdi, VIf(Bf3), VOf(Bf4));
    __hv_lt_f(VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_samphold_f(&sSamphold_v7TLWLo9, VIf(Bf1), VIf(Bf4), VOf(Bf4));
    __hv_varread_f(&sVarf_yubGt1V0, VOf(Bf5));
    __hv_mul_f(VIf(Bf4), VIf(Bf5), VOf(Bf5));
    __hv_var_k_f(VOf(Bf6), 256.0f, 256.0f, 256.0f, 256.0f, 256.0f, 256.0f, 256.0f, 256.0f);
    __hv_mul_f(VIf(Bf3), VIf(Bf6), VOf(Bf6));
    __hv_mul_f(VIf(Bf5), VIf(Bf6), VOf(Bf6));
    __hv_phasor_f(&sPhasor_MVSIYAQ2, VIf(Bf6), VOf(Bf6));
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
    __hv_varread_f(&sVarf_c7luVdha, VOf(Bf7));
    __hv_fma_f(VIf(Bf5), VIf(Bf7), VIf(Bf4), VOf(Bf4));
    __hv_phasor_f(&sPhasor_ILH3Kzax, VIf(Bf4), VOf(Bf4));
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
    __hv_line_f(&sLine_NJ9KOKSm, VOf(Bf8));
    __hv_varread_f(&sVarf_HRuactSx, VOf(Bf4));
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
    __hv_varread_f(&sVarf_x3JNJEoy, VOf(Bf8));
    __hv_mul_f(VIf(Bf1), VIf(Bf8), VOf(Bf8));
    __hv_varread_f(&sVarf_vZ3freh8, VOf(Bf1));
    __hv_mul_f(VIf(Bf8), VIf(Bf1), VOf(Bf1));
    __hv_varread_f(&sVarf_kxIdumy8, VOf(Bf8));
    __hv_rpole_f(&sRPole_V92cpwNw, VIf(Bf1), VIf(Bf8), VOf(Bf8));
    __hv_add_f(VIf(Bf12), VIf(Bf8), VOf(Bf8));
    __hv_var_k_f(VOf(Bf12), 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
    __hv_mul_f(VIf(Bf8), VIf(Bf12), VOf(Bf12));
    __hv_add_f(VIf(Bf7), VIf(Bf12), VOf(Bf7));
    __hv_fma_f(VIf(Bf2), VIf(Bf0), VIf(Bf7), VOf(Bf7));
    __hv_var_k_f(VOf(Bf0), 0.15f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f);
    __hv_mul_f(VIf(Bf7), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O1), VOf(O1));
    __hv_var_k_f(VOf(Bf0), 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f);
    __hv_add_f(VIf(Bf5), VIf(Bf12), VOf(Bf12));
    __hv_fma_f(VIf(Bf2), VIf(Bf0), VIf(Bf12), VOf(Bf12));
    __hv_var_k_f(VOf(Bf0), 0.15f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f);
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
