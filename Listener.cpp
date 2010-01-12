#include "stdafx.h"
#include ".\listener.h"
BOOL StartStop=FALSE;
//#define CUT_OFF_OFFSET 4000
Listener::Listener(void)
{
NumOfPos=1,NumOfNeg=1;
}

Listener::~Listener(void)
{

}

BOOL Listener::WhistleListener(int StartPointOfListen,int CUT_OFF_OFFSET,int endofview)
{
	int fft_len=FFT_LEN/2,i=200;
	
	double FinalWeight[FFT_LEN/2];
	long FinalWeightP1,FinalWeightP2,FinalWeightP3,
		 LearnedSynapseP1,LearnedSynapseP2,LearnedSynapseP3;
	
	memcpy(FinalWeight,NurlStk,(FFT_LEN/2)*sizeof(double));
	for ( ; i< 512 - endofview ; i+=2)
	{
		FinalWeightP1=(long)(FinalWeight[i]-OldWeight[i]);
		FinalWeightP2=(long)(FinalWeight[i+1]-OldWeight[i+1]);
		FinalWeightP3=(long)(FinalWeight[i+2]-OldWeight[i+2]);
		if(CUT_OFF_OFFSET>FinalWeightP1)
				FinalWeightP1=0;
		if(CUT_OFF_OFFSET>FinalWeightP2)
				FinalWeightP2=0;
		if(CUT_OFF_OFFSET>FinalWeightP3)
				FinalWeightP3=0;
		if (FinalWeightP1<FinalWeightP2 && FinalWeightP2>FinalWeightP3)		  /*  /\  A*/
		{
			NurlStkWeightState=1;	
		}
		else if (FinalWeightP1>FinalWeightP2 && FinalWeightP2<FinalWeightP3)  /*  \/  C*/
		{
			NurlStkWeightState=2;
		}
		else if (FinalWeightP1==FinalWeightP2 && FinalWeightP2==FinalWeightP3)/*  --  B*/
		{
			NurlStkWeightState=3;
		}
		else if (FinalWeightP1<FinalWeightP2 && FinalWeightP2==FinalWeightP3) /*  /--  1*/
		{
			NurlStkWeightState=4;	
		}
		else if (FinalWeightP1==FinalWeightP2 && FinalWeightP2>FinalWeightP3) /*  --\  1*/
		{
			NurlStkWeightState=5;	
		}
		else if (FinalWeightP1>FinalWeightP2 && FinalWeightP2==FinalWeightP3) /*  \__  2*/
		{
			NurlStkWeightState=6;
		}
		else if (FinalWeightP1==FinalWeightP2 && FinalWeightP2<FinalWeightP3) /*  __/  2*/
		{
			NurlStkWeightState=7;
		}
		else if (FinalWeightP1<FinalWeightP2 && FinalWeightP2<FinalWeightP3)  /*  ,."  3*/
		{
			NurlStkWeightState=8;
		}
		else if (FinalWeightP1>FinalWeightP2 && FinalWeightP2>FinalWeightP3)  /*  ".,  3*/
		{
			NurlStkWeightState=9;
		}

		/*******************************************************************************************/

		LearnedSynapseP1=(long)LearnedSynapse[i];LearnedSynapseP2=(long)LearnedSynapse[i+1];LearnedSynapseP3=(long)LearnedSynapse[i+2];
		if (LearnedSynapseP1<LearnedSynapseP2 && LearnedSynapseP2>LearnedSynapseP3)		  /*  /\  A*/
		{
			LrnSynapseWeightState=1;	
		}
		else if (LearnedSynapseP1>LearnedSynapseP2 && LearnedSynapseP2<LearnedSynapseP3)  /*  \/  C*/
		{
			LrnSynapseWeightState=2;
		}
		else if (LearnedSynapseP1==LearnedSynapseP2 && LearnedSynapseP2==LearnedSynapseP3)/*  --  B*/
		{
			LrnSynapseWeightState=3;
		}
		else if (LearnedSynapseP1<LearnedSynapseP2 && LearnedSynapseP2==LearnedSynapseP3) /*  /--  1*/
		{
			LrnSynapseWeightState=4;	
		}
		else if (LearnedSynapseP1==LearnedSynapseP2 && LearnedSynapseP2>LearnedSynapseP3) /*  --\  1*/
		{
			LrnSynapseWeightState=5;	
		}
		else if (LearnedSynapseP1>LearnedSynapseP2 && LearnedSynapseP2==LearnedSynapseP3) /*  \__  2*/
		{
			LrnSynapseWeightState=6;
		}
		else if (LearnedSynapseP1==LearnedSynapseP2 && LearnedSynapseP2<LearnedSynapseP3) /*  __/  2*/
		{
			LrnSynapseWeightState=7;
		}
		else if (LearnedSynapseP1<LearnedSynapseP2 && LearnedSynapseP2<LearnedSynapseP3)  /*  ,."  3*/
		{
			LrnSynapseWeightState=8;
		}
		else if (LearnedSynapseP1>LearnedSynapseP2 && LearnedSynapseP2>LearnedSynapseP3)  /*  ".,  3*/
		{
			LrnSynapseWeightState=9;
		}		/*******************************************************************************************/
	
		if (NurlStkWeightState == LrnSynapseWeightState)
		{
	
			NumOfPos++;
		}
		else
		{
			NumOfNeg++;
		}
	

	}
if(AvrgWeightPos<NumOfPos)
	AvrgWeightPos=NumOfPos;
if(AvrgWeightNeg<NumOfNeg)	
	AvrgWeightNeg=NumOfNeg;	

memcpy(OldWeight,FinalWeight,(FFT_LEN/2)*sizeof(double));

	if((NumOfPos/NumOfNeg)<=(AvrgWeightTotal-0.8))
		return FALSE;
	else
	{
		return TRUE;
	}
}
CString Listener::GetResult()
{
CString T;
T.Format("%f %f",NumOfNeg,NumOfPos); 
//T.Format("%% %f",(NumOfPos/187)*100); 
return T;
}

void FAR PASCAL OnTimerShot(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
		StartStop=FALSE;
		sndPlaySound(NULL,SND_ASYNC);
}

UINT KillTimerCallback(UINT timerID)
{
 return(timeKillEvent(timerID) == TIMERR_NOERROR);
}

UINT SetTimerCallback(DWORD npSeq, UINT msInterval, UINT tEvent)
{ 
 UINT wTimerID = timeSetEvent(msInterval, msInterval, OnTimerShot, (DWORD)npSeq, tEvent);
 return(wTimerID);
}

void Listener::LearningSampleSound(int CUT_OFF_OFFSET)
{
	SndCard Snd;
	BOOL OUT_SPEAKERS=FALSE,OUT_WAVE=FALSE,OUT_MICROPHONE=FALSE,OUT_LINE=FALSE;
	int fft_len=FFT_LEN/2;
	int StrtPnt=0,EndPnt=fft_len;
	float tmp1=0,tmp2=0;
			Snd.nSourceSelector("Wave Out Mix");
			scrT = App_Path + "MainSample\\";
			scrT+=	"FinalSample.wav";
		if (!Aptool.IsFileExist(scrT))
			{
			AfxMessageBox("Main sample files is not exist.\n"\
			"Your may didn't creat it.",MB_ICONEXCLAMATION);
			return;
			}
		Snd.nMuteUnMute("OUT_SPEAKERS",true);
		for(int j=0; j<fft_len;j++)
            LearnedSynapse[j]=0;
		char buffer[1] = "";
		UINT timerID = SetTimerCallback((DWORD)(char [1])buffer, 3000, TIME_ONESHOT);
		sndPlaySound(scrT,SND_ASYNC | SND_LOOP);
		long ito=0;
		StartStop=TRUE;
		while(StartStop==TRUE)
		{
			ito++;
			for (int i=0; i< fft_len ; i++){
				if(NurlStk[i]>LearnedSynapse[i])
					{
						if(CUT_OFF_OFFSET<NurlStk[i])
							LearnedSynapse[i]=NurlStk[i];
						else
							LearnedSynapse[i]=0;
					}
			}
			DoEvents();
		}
		Snd.nMuteUnMute("OUT_SPEAKERS",false);
		AvrgWeightTotal=AvrgWeightPos/AvrgWeightNeg;
		CString Tito;
		Tito.Format("%f",AvrgWeightTotal);
		Tito+=" Learn is ended.";
		AfxMessageBox(Tito);
}

void Listener::DoEvents()
{
    MSG msg;
    PeekMessage(&msg,NULL,0,0,PM_REMOVE);
    {
	   TranslateMessage(&msg);
	   DispatchMessage(&msg);
    }
}

