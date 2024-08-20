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

#ifndef _HEAVY_CONTEXT_PROG_HPP_
#define _HEAVY_CONTEXT_PROG_HPP_

// object includes
#include "HeavyContext.hpp"
#include "HvControlSlice.h"
#include "HvControlUnop.h"
#include "HvSignalLine.h"
#include "HvControlRandom.h"
#include "HvSignalDel1.h"
#include "HvControlDelay.h"
#include "HvControlBinop.h"
#include "HvControlVar.h"
#include "HvSignalVar.h"
#include "HvMath.h"
#include "HvControlSystem.h"
#include "HvSignalSamphold.h"
#include "HvSignalPhasor.h"
#include "HvControlCast.h"

class Heavy_prog : public HeavyContext {

 public:
  Heavy_prog(double sampleRate, int poolKb=10, int inQueueKb=2, int outQueueKb=0);
  ~Heavy_prog();

  const char *getName() override { return "prog"; }
  int getNumInputChannels() override { return 0; }
  int getNumOutputChannels() override { return 2; }

  int process(float **inputBuffers, float **outputBuffer, int n) override;
  int processInline(float *inputBuffers, float *outputBuffer, int n) override;
  int processInlineInterleaved(float *inputBuffers, float *outputBuffer, int n) override;

  int getParameterInfo(int index, HvParameterInfo *info) override;

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;

  // static sendMessage functions
  static void cSwitchcase_8BInVhJh_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_6q51H9tS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_5LAkJnbB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_pNABoDFg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_T2TPMad7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_XBJjL4Eu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_EBvb6Y4B_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_GqnN2bZA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_8BSD4jHL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_qWmdPrJS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_1onI3uRF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_HJfDNN5x_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_anK5auYk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_9TboWNQy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_LZi1nGIz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_UmcWB7lZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_KjFusc6F_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_0lWG87FP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_jBrZbZ5c_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_QAYsjBHS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_4p4ohfG7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_RoXJds5a_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_NP7D3fP7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_r5TvYyqG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_py1VrhhZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_FAXovFzV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_pmL4Mxe9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_zMcWS86f_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_MT2WByod_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_XfLCaRbi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_gA01LkLv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_0JKARpfL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_IlxYxhFe_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_zdAyLWE3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_VMCTJwJc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_sZB5SvmE_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_9DKG7nJU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_lJ4Lozih_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_fmSTjIHh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_R72csJak_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_GFJfS0iH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Hm3tWTQP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_S699he7M_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_VAJlg8dj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_cBkWi8Rf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_OaFGRudt_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_8QrveA7L_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_fhiZCOLk_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_K0TGGdeN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_zh02zAsL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_cG3Wr6lh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_2uRm7uF0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_yl3w1YBU_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_P8glYnIu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_bpjX3FlR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_ORDgu9yq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_9qw6zHrC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_tH2TCfxr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_8XJMHF6h_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_QS6t9Kc8_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalPhasor sPhasor_4DFzpsoM;
  SignalDel1 sDel1_RVNlPfGe;
  SignalSamphold sSamphold_aGpSOPLF;
  SignalPhasor sPhasor_4pzAjIob;
  SignalLine sLine_HPZzD5zI;
  SignalPhasor sPhasor_VLoUdYiD;
  ControlBinop cBinop_6q51H9tS;
  ControlRandom cRandom_pNABoDFg;
  ControlSlice cSlice_T2TPMad7;
  SignalVari sVari_H8vA3h9K;
  ControlVar cVar_EBvb6Y4B;
  ControlBinop cBinop_GqnN2bZA;
  ControlVar cVar_8BSD4jHL;
  ControlVar cVar_qWmdPrJS;
  ControlVar cVar_1onI3uRF;
  ControlBinop cBinop_zMcWS86f;
  SignalVarf sVarf_xdS58E1U;
  ControlDelay cDelay_zdAyLWE3;
  ControlVar cVar_lJ4Lozih;
  ControlBinop cBinop_R72csJak;
  ControlBinop cBinop_GFJfS0iH;
  ControlBinop cBinop_Hm3tWTQP;
  ControlBinop cBinop_VAJlg8dj;
  ControlRandom cRandom_OaFGRudt;
  ControlSlice cSlice_8QrveA7L;
  ControlBinop cBinop_K0TGGdeN;
  ControlRandom cRandom_cG3Wr6lh;
  ControlSlice cSlice_2uRm7uF0;
  ControlBinop cBinop_P8glYnIu;
  ControlRandom cRandom_ORDgu9yq;
  ControlSlice cSlice_9qw6zHrC;
  ControlBinop cBinop_tH2TCfxr;
  SignalVarf sVarf_qvxDZhdp;
  ControlBinop cBinop_8XJMHF6h;
  SignalVarf sVarf_JiNjdA6m;
  SignalVarf sVarf_Ngcj4Vcs;
};

#endif // _HEAVY_CONTEXT_PROG_HPP_
