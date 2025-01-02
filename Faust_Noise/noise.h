/* ------------------------------------------------------------
name: "noise"
Code generated with Faust 2.78.0 (https://faust.grame.fr)
Compilation options: -lang c -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 


#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

#include <math.h>
#include <stdint.h>
#include <stdlib.h>


#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

typedef struct {
	int iRec0[2];
	float fRec1[4];
	int fSampleRate;
} mydsp;

mydsp* newmydsp() { 
	mydsp* dsp = (mydsp*)calloc(1, sizeof(mydsp));
	return dsp;
}

void deletemydsp(mydsp* dsp) { 
	free(dsp);
}

int getSampleRatemydsp(mydsp* RESTRICT dsp) {
	return dsp->fSampleRate;
}

int getNumInputsmydsp(mydsp* RESTRICT dsp) {
	return 0;
}
int getNumOutputsmydsp(mydsp* RESTRICT dsp) {
	return 2;
}

void classInitmydsp(int sample_rate) {
}

void instanceResetUserInterfacemydsp(mydsp* dsp) {
}

void instanceClearmydsp(mydsp* dsp) {
	/* C99 loop */
	{
		int l0;
		for (l0 = 0; l0 < 2; l0 = l0 + 1) {
			dsp->iRec0[l0] = 0;
		}
	}
	/* C99 loop */
	{
		int l1;
		for (l1 = 0; l1 < 4; l1 = l1 + 1) {
			dsp->fRec1[l1] = 0.0f;
		}
	}
}

void instanceConstantsmydsp(mydsp* dsp, int sample_rate) {
	dsp->fSampleRate = sample_rate;
}
	
void instanceInitmydsp(mydsp* dsp, int sample_rate) {
	instanceConstantsmydsp(dsp, sample_rate);
	instanceResetUserInterfacemydsp(dsp);
	instanceClearmydsp(dsp);
}

void initmydsp(mydsp* dsp, int sample_rate) {
	classInitmydsp(sample_rate);
	instanceInitmydsp(dsp, sample_rate);
}

void computemydsp(mydsp* dsp, int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
	FAUSTFLOAT* output0 = outputs[0];
	FAUSTFLOAT* output1 = outputs[1];
	/* C99 loop */
	{
		int i0;
		for (i0 = 0; i0 < count; i0 = i0 + 1) {
			dsp->iRec0[0] = 1103515245 * dsp->iRec0[1] + 12345;
			float fTemp0 = 4.656613e-10f * (float)(dsp->iRec0[0]);
			output0[i0] = (FAUSTFLOAT)(fTemp0);
			dsp->fRec1[0] = 0.5221894f * dsp->fRec1[3] + fTemp0 + 2.494956f * dsp->fRec1[1] - 2.0172658f * dsp->fRec1[2];
			output1[i0] = (FAUSTFLOAT)(0.049922034f * dsp->fRec1[0] + 0.0506127f * dsp->fRec1[2] - (0.095993534f * dsp->fRec1[1] + 0.004408786f * dsp->fRec1[3]));
			dsp->iRec0[1] = dsp->iRec0[0];
			/* C99 loop */
			{
				int j0;
				for (j0 = 3; j0 > 0; j0 = j0 - 1) {
					dsp->fRec1[j0] = dsp->fRec1[j0 - 1];
				}
			}
		}
	}
}

#ifdef __cplusplus
}
#endif

#endif
