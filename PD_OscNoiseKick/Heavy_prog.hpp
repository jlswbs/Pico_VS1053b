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
#include "HvSignalVar.h"
#include "HvControlBinop.h"
#include "HvSignalDel1.h"
#include "HvSignalSamphold.h"
#include "HvControlVar.h"
#include "HvControlCast.h"
#include "HvControlUnop.h"
#include "HvControlSystem.h"
#include "HvSignalPhasor.h"
#include "HvSignalRPole.h"
#include "HvSignalLine.h"
#include "HvControlDelay.h"
#include "HvControlRandom.h"
#include "HvMath.h"

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
  static void cSwitchcase_cZPUgsYg_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_NYeWmCng_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_8g3Dm7NT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_zUGicvE3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_DhLoIhh6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_TW4NpsGz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_wchjtvK9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_guYgovDy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_tAwPoFln_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_QWNhS6fH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_G9kodeZJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_dk5Slxwf_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_wSfvld4k_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_dChBiwL1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_6O3uu6Px_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_8Z4VmdJX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Pz3U6jhR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_P0fvKoWj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_hKaLffcK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_f3Mx7zb6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_O80kkpZ2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_3PZmnvrL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_r579iMiK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_orM8JRqN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Um08afEW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_bIOIrE6Q_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_AczV2sbU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_8P3wCrD9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_5jynGwPh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_4rfQ4vZr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_9bieD4BD_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_6uJtQXvj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_8iqSS6dU_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_TiKRMUZB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_MU64lNnU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_hDfxr1DV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_YDAsnWCI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_loGKzX95_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_BXNrR52E_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_YKi2OsEd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_NR88xTYM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_sOqyfO0o_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_WQ8znNW4_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_Zu6LBoFP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_AXU7qkx9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_XXoRqAyl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_ybMA8YsU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_WAUqy0cQ_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_gsDmdS7D_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_OVDlBpvU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_lU48RUo4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_8X3WAAQv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_g9MK0rWC_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_taUF3GpM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_QGecDwFf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_iAODcsuQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_Mdsq61OW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_297vmgBp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_94CuR5Qs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_qRnpecR2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_mQBOJn8l_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_fQ2LdWRt_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_52YxK3dP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_U30s890X_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_r6WFYjnv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_gV9yGn5m_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_QwtqlZaJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_gbqzlpjR_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_fwTox30l_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ruOhQBcX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_mW3Yr8rQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_Oz84MfUw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_morIu36e_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_4sQLst0N_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_JQmo5X5m_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_wox0aYxS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_2H0ePoKc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_g3uBmuTL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Hc2Kedaw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_v8SdWfGD_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_WjuOAgTy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_cY6w0mBR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_pe1Cg85U_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_3D1Xkujt_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_MpkvuXyS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_QZrypkBE_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_BQnqxFqG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_xbWz8xt7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_0raPtbWp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_5iMo9HXG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_45GMHYam_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_vvzThO1K_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_VWIVEYsc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_YbMKMNi2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_WGR8Ricx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_7wW7IwxY_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalRPole sRPole_U3AyyFN9;
  SignalDel1 sDel1_atB57DLt;
  SignalLine sLine_jP6Yv9M7;
  SignalPhasor sPhasor_yq3VjBtN;
  SignalDel1 sDel1_ug2QJUr5;
  SignalSamphold sSamphold_TaeXY8D9;
  SignalPhasor sPhasor_oRTghQ0Z;
  SignalLine sLine_aKYcwOkC;
  SignalRPole sRPole_4WPzVv3k;
  SignalPhasor sPhasor_UtaeKqfB;
  ControlBinop cBinop_NYeWmCng;
  ControlRandom cRandom_zUGicvE3;
  ControlSlice cSlice_DhLoIhh6;
  SignalVari sVari_yJFvQ5Ii;
  ControlVar cVar_wchjtvK9;
  ControlBinop cBinop_guYgovDy;
  ControlVar cVar_tAwPoFln;
  ControlVar cVar_QWNhS6fH;
  ControlVar cVar_G9kodeZJ;
  ControlBinop cBinop_8P3wCrD9;
  SignalVarf sVarf_PclCXoOw;
  ControlDelay cDelay_TiKRMUZB;
  ControlVar cVar_loGKzX95;
  ControlBinop cBinop_YKi2OsEd;
  ControlBinop cBinop_NR88xTYM;
  ControlBinop cBinop_sOqyfO0o;
  ControlBinop cBinop_Zu6LBoFP;
  ControlRandom cRandom_XXoRqAyl;
  ControlSlice cSlice_ybMA8YsU;
  ControlBinop cBinop_gsDmdS7D;
  ControlRandom cRandom_lU48RUo4;
  ControlSlice cSlice_8X3WAAQv;
  ControlBinop cBinop_taUF3GpM;
  ControlRandom cRandom_iAODcsuQ;
  ControlSlice cSlice_Mdsq61OW;
  ControlVar cVar_297vmgBp;
  ControlBinop cBinop_mQBOJn8l;
  ControlBinop cBinop_fQ2LdWRt;
  SignalVarf sVarf_fhb6lLLw;
  ControlBinop cBinop_U30s890X;
  ControlBinop cBinop_r6WFYjnv;
  ControlBinop cBinop_gV9yGn5m;
  SignalVarf sVarf_9YFqBP2L;
  ControlVar cVar_QwtqlZaJ;
  ControlBinop cBinop_ruOhQBcX;
  ControlBinop cBinop_mW3Yr8rQ;
  SignalVarf sVarf_CrFxOtTg;
  ControlVar cVar_Oz84MfUw;
  ControlBinop cBinop_JQmo5X5m;
  ControlBinop cBinop_wox0aYxS;
  ControlBinop cBinop_g3uBmuTL;
  ControlBinop cBinop_Hc2Kedaw;
  ControlBinop cBinop_v8SdWfGD;
  ControlBinop cBinop_WjuOAgTy;
  ControlBinop cBinop_cY6w0mBR;
  SignalVarf sVarf_SWQmZoHx;
  ControlBinop cBinop_3D1Xkujt;
  ControlRandom cRandom_QZrypkBE;
  ControlSlice cSlice_BQnqxFqG;
  ControlBinop cBinop_xbWz8xt7;
  ControlBinop cBinop_0raPtbWp;
  ControlBinop cBinop_WGR8Ricx;
  SignalVarf sVarf_Z0piCSG1;
  SignalVarf sVarf_4nyFhAkT;
  SignalVarf sVarf_lDNZhXfx;
};

#endif // _HEAVY_CONTEXT_PROG_HPP_
