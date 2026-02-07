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

#ifndef _HEAVY_CONTEXT_PROG_HPP_
#define _HEAVY_CONTEXT_PROG_HPP_

// object includes
#include "HeavyContext.hpp"
#include "HvSignalDel1.h"
#include "HvControlUnop.h"
#include "HvControlVar.h"
#include "HvControlCast.h"
#include "HvSignalRPole.h"
#include "HvControlSystem.h"
#include "HvControlRandom.h"
#include "HvSignalVar.h"
#include "HvControlSlice.h"
#include "HvMath.h"
#include "HvControlBinop.h"
#include "HvSignalSamphold.h"
#include "HvSignalPhasor.h"
#include "HvSignalLine.h"

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
  struct Parameter {
    struct In {
      enum ParameterIn : hv_uint32_t {
        TEMPO = 0x26682824, // tempo
      };
    };
  };

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;

  // static sendMessage functions
  static void cVar_xw1mGq2l_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_LSJoNrY2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_2ATKqOLa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_TLAVfKbd_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_Crms06qr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Ru9D5eTR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_a7sSzp4M_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_S28wJP3s_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_760JfNgQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_lAEbZ5ez_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_MgSj2xnn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_QgccUOx0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_GaVAqUoP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_PvAM3HGn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Ps7uBUGa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_xpor2Hpo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_R4MospIj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_KVyU1YsZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_OGDiyBAQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_xe9aWXXl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_EM2m4BuH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_5EAXG1nA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_0ap8GQGG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_guseHSP8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_IIxr5hW1_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_aHi7jDCU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_Lp4tQ6Fe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_hi2zdDbr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_OLPPfgnd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_e7vqtAD6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_Fm6xgjAz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_fjkJqE1s_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_FbqwKN8C_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_zWHfnYoh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_BxWHJHBx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_1Lp8TTYH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_q3yTJPk0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_FkVf21JR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_dcB2rE3s_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_VqlizZyx_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_G9zXpNo9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_UVoHqOh4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_padfYcwh_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_NwUGNv0N_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_3VHp7H64_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_uSIJJ4Yd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_ilBkzoyV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_LFZMrhp8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_6phazKgA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_kvrptBmn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_UxParpD8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_rrg7hCWJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_EHCsZvVf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_VSVhLpgM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_7iTd2FU6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_dtUim4st_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_fUNcRz2O_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_wnYgtkBL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_b2RBHPAn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_t64UPgqL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_N45YWsX1_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_7RgykuMb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_6B2xnzHW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_9lZV8lIN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_gVdtbAVp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_pcYO1Zoj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_1Syeg9xA_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_cQkUE2hd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_PTUyVjmu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_Wz5qNoW8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_9BeB9bjR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_Llp6bDeo_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_vsYlevBD_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_kTpJxmnl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_0vxC7Z34_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_amEEHe9H_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_PYRsC6b4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_SWGs3knf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_LyuUczKA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_fLM3Z4kR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_pPtLJXxT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_9ComHiEW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_gabMzmGo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_YVDBtW4U_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_reXt6V8x_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_IEdVCssP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_1ec7vajz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_kJ2Ejwe6_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalRPole sRPole_CMFkc4Vk;
  SignalDel1 sDel1_0da2dfdo;
  SignalLine sLine_GPM84qEL;
  SignalPhasor sPhasor_sTQ2VfGS;
  SignalDel1 sDel1_QkoARA9l;
  SignalSamphold sSamphold_Ury8CONE;
  SignalPhasor sPhasor_DdNASZ6T;
  SignalPhasor sPhasor_cEi439Ko;
  SignalLine sLine_5h0nUEpP;
  SignalRPole sRPole_2Q5c8Ysv;
  ControlVar cVar_xw1mGq2l;
  ControlVar cVar_LSJoNrY2;
  ControlVar cVar_2ATKqOLa;
  ControlBinop cBinop_xe9aWXXl;
  SignalVarf sVarf_dSFA78l2;
  ControlBinop cBinop_aHi7jDCU;
  ControlRandom cRandom_hi2zdDbr;
  ControlSlice cSlice_OLPPfgnd;
  ControlVar cVar_e7vqtAD6;
  ControlBinop cBinop_FbqwKN8C;
  ControlBinop cBinop_zWHfnYoh;
  SignalVarf sVarf_b94bFdan;
  ControlBinop cBinop_1Lp8TTYH;
  ControlBinop cBinop_q3yTJPk0;
  ControlBinop cBinop_FkVf21JR;
  SignalVarf sVarf_nK4OwyfG;
  ControlVar cVar_dcB2rE3s;
  ControlVar cVar_UVoHqOh4;
  ControlBinop cBinop_NwUGNv0N;
  ControlRandom cRandom_uSIJJ4Yd;
  ControlSlice cSlice_ilBkzoyV;
  ControlBinop cBinop_LFZMrhp8;
  ControlBinop cBinop_6phazKgA;
  SignalVarf sVarf_v5Vyztih;
  ControlVar cVar_kvrptBmn;
  ControlBinop cBinop_EHCsZvVf;
  ControlBinop cBinop_VSVhLpgM;
  ControlBinop cBinop_dtUim4st;
  ControlBinop cBinop_fUNcRz2O;
  ControlBinop cBinop_wnYgtkBL;
  ControlBinop cBinop_b2RBHPAn;
  ControlBinop cBinop_t64UPgqL;
  SignalVarf sVarf_ezmUOCtN;
  ControlBinop cBinop_7RgykuMb;
  ControlRandom cRandom_9lZV8lIN;
  ControlSlice cSlice_gVdtbAVp;
  SignalVari sVari_DZ63AjSt;
  ControlBinop cBinop_cQkUE2hd;
  ControlRandom cRandom_Wz5qNoW8;
  ControlSlice cSlice_9BeB9bjR;
  ControlBinop cBinop_vsYlevBD;
  ControlRandom cRandom_0vxC7Z34;
  ControlSlice cSlice_amEEHe9H;
  ControlBinop cBinop_PYRsC6b4;
  ControlBinop cBinop_gabMzmGo;
  ControlBinop cBinop_YVDBtW4U;
  ControlBinop cBinop_reXt6V8x;
  ControlBinop cBinop_IEdVCssP;
  SignalVarf sVarf_95VmBnGZ;
  SignalVarf sVarf_nhejub5A;
  SignalVarf sVarf_gMMntJe4;
};

#endif // _HEAVY_CONTEXT_PROG_HPP_
