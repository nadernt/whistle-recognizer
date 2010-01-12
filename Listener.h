#pragma once
#include "SndCardServices.h"
#include "apptools.h"
#define FFT_LEN 1024
class Listener
{
private:
		AppTools Aptool;
		CString scrT;
		int NurlStkWeightState,LrnSynapseWeightState;
		float  NumOfNeg,NumOfPos;
public:
		Listener(void);
		~Listener(void);
		BOOL WhistleListener(int StartPointOfListen,int CUT_OFF_OFFSET,int endofview);
		void LearningSampleSound(int CUT_OFF_OFFSET);
		double *WeightLayer;
		double *InputLayer;
		void DoEvents();
		CString GetResult();
};
