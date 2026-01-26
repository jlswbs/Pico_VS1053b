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
#include "HvControlCast.h"
#include "HvControlBinop.h"
#include "HvControlUnop.h"
#include "HvControlSlice.h"
#include "HvSignalRPole.h"
#include "HvSignalDel1.h"
#include "HvSignalSamphold.h"
#include "HvSignalLine.h"
#include "HvSignalPhasor.h"
#include "HvControlSystem.h"
#include "HvMath.h"
#include "HvSignalVar.h"
#include "HvControlVar.h"
#include "HvControlRandom.h"

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
  static void cVar_Fyf3KaMx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_WP9CysuC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_8ekAWnCB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_87W5gmI1_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_cSQXXcQI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_4RixOwED_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_83awyfce_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_hc5YNCkc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_S2LMpnIi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_cuJ6PLt2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ZdrLqIt1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_g7sgOXVl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_9N0h6jUR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_pKr3EurY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_30qA1eFC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_0xTbxmHk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_WhzKULQ4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_wtQFAqeo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_tGLvh64M_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_uQUvdkK7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_hIwbLsfr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_EwXplAcX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_eOM6YRZC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_EotkF4JC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_FH1SIaHg_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_phSsGhPg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_DHVZY6LU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_kcrAUz4k_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_zDnPJmOD_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_OEzkZyz7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_oYttykXC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_voXfLKuS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Z6DZ4m6N_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_wNiKfITA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_PwVnI789_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_2S13Q4Ra_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_4J9gsaN9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_VSORPMYj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_L4gHzjFo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_6aTj8MRG_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_qM7TcyVa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_9WVqBWGL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_08JU16lW_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_DB2L4Kpb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_Ton1iR5i_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_2Zpi6uD8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_aKh6QNu7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_hOIVa3qd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_4NnMiXWZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_nVhFXPsO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_SCrIvVnv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_vjqHYM8w_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_wncP78dr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_OwjoBzFd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_xqOh6oa8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_6zvg6rqB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_1ZvktRSX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_MVtdXCS4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Now2skPI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_QDww6r23_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_HbTKcl9j_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_TfgUgUOs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_gTktGCvm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_kEUpe3Ez_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_tiBg7swR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_xyEyMaNk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_5tEhAC34_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_p81Vs5ZL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_6vjbkvMc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_0rW6epQm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_HppREbMU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_t2jfKJeH_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_zRSLw1sW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_ZJzEbObe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_PWdriHN8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_FqmV7XIQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_jY4kWi8M_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_qH2scIH0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_MKXyn3Lf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_MIg4GjYP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_4VaoAHoj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_uhBSJ6jO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_I5qhDPya_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_mEvytSox_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_QApHSFQS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_5vMJSCzr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_eTp9IeV0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_dY6z9ZgY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_ZOfw8fDb_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalRPole sRPole_fVALYJ5g;
  SignalDel1 sDel1_MDBTTKDU;
  SignalLine sLine_wMLytt6S;
  SignalPhasor sPhasor_ndfG8Sy2;
  SignalDel1 sDel1_5RE5oKdi;
  SignalSamphold sSamphold_v7TLWLo9;
  SignalPhasor sPhasor_MVSIYAQ2;
  SignalPhasor sPhasor_ILH3Kzax;
  SignalLine sLine_NJ9KOKSm;
  SignalRPole sRPole_V92cpwNw;
  ControlVar cVar_Fyf3KaMx;
  ControlVar cVar_WP9CysuC;
  ControlVar cVar_8ekAWnCB;
  ControlBinop cBinop_uQUvdkK7;
  SignalVarf sVarf_HRuactSx;
  ControlBinop cBinop_phSsGhPg;
  ControlRandom cRandom_kcrAUz4k;
  ControlSlice cSlice_zDnPJmOD;
  ControlVar cVar_OEzkZyz7;
  ControlBinop cBinop_Z6DZ4m6N;
  ControlBinop cBinop_wNiKfITA;
  SignalVarf sVarf_kxIdumy8;
  ControlBinop cBinop_2S13Q4Ra;
  ControlBinop cBinop_4J9gsaN9;
  ControlBinop cBinop_VSORPMYj;
  SignalVarf sVarf_vZ3freh8;
  ControlVar cVar_L4gHzjFo;
  ControlVar cVar_9WVqBWGL;
  ControlBinop cBinop_DB2L4Kpb;
  ControlRandom cRandom_2Zpi6uD8;
  ControlSlice cSlice_aKh6QNu7;
  ControlBinop cBinop_hOIVa3qd;
  ControlBinop cBinop_4NnMiXWZ;
  SignalVarf sVarf_LfbkmlPF;
  ControlVar cVar_nVhFXPsO;
  ControlBinop cBinop_wncP78dr;
  ControlBinop cBinop_OwjoBzFd;
  ControlBinop cBinop_6zvg6rqB;
  ControlBinop cBinop_1ZvktRSX;
  ControlBinop cBinop_MVtdXCS4;
  ControlBinop cBinop_Now2skPI;
  ControlBinop cBinop_QDww6r23;
  SignalVarf sVarf_zWT1IZPq;
  ControlBinop cBinop_TfgUgUOs;
  ControlRandom cRandom_kEUpe3Ez;
  ControlSlice cSlice_tiBg7swR;
  SignalVari sVari_O1SIByJc;
  ControlBinop cBinop_p81Vs5ZL;
  ControlRandom cRandom_0rW6epQm;
  ControlSlice cSlice_HppREbMU;
  ControlBinop cBinop_zRSLw1sW;
  ControlRandom cRandom_PWdriHN8;
  ControlSlice cSlice_FqmV7XIQ;
  ControlBinop cBinop_jY4kWi8M;
  ControlBinop cBinop_I5qhDPya;
  ControlBinop cBinop_mEvytSox;
  ControlBinop cBinop_5vMJSCzr;
  ControlBinop cBinop_eTp9IeV0;
  SignalVarf sVarf_yubGt1V0;
  SignalVarf sVarf_c7luVdha;
  SignalVarf sVarf_x3JNJEoy;
};

#endif // _HEAVY_CONTEXT_PROG_HPP_
