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
#include "HvControlVar.h"
#include "HvControlCast.h"
#include "HvControlBinop.h"
#include "HvSignalPhasor.h"
#include "HvSignalVar.h"
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
  struct Parameter {
    struct In {
      enum ParameterIn : hv_uint32_t {
        MIX = 0x764F1C36, // mix
        PITCH = 0x8B2148DD, // pitch
        SHAPE = 0xB063C1EC, // shape
      };
    };
  };

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;

  // static sendMessage functions
  static void cVar_v58xBIdO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_40FnMpXO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_G9py5QBZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_RtWvV0V9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_b1skY4wH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_qL6mkyZt_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ipq7lzuS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_vvWQwX0x_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_xWClsPwd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_vZgFZPWq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_jNhlaaIo_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalPhasor sPhasor_VpENSjuV;
  SignalPhasor sPhasor_jb9QxPiL;
  ControlVar cVar_v58xBIdO;
  ControlVar cVar_40FnMpXO;
  ControlVar cVar_G9py5QBZ;
  ControlBinop cBinop_RtWvV0V9;
  ControlBinop cBinop_b1skY4wH;
  ControlBinop cBinop_qL6mkyZt;
  SignalVarf sVarf_SvCqCP8p;
};

#endif // _HEAVY_CONTEXT_PROG_HPP_
