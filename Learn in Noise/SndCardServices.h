#pragma once
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib")

class SndCard
{
public:
	SndCard(void);
	~SndCard(void);
	void nSetVolume(char *SrcName,int ControlTypeID,DWORD VolSize); // ControlTypeID-> 0:Speaker 1:Input 2:OutPut
	DWORD nGetVolume(char *SrcName,int ControlTypeID); // ControlTypeID-> 0:Speaker 1:Input 2:OutPut
	void nSourceSelector(char *SrcName);
	void nMuteUnMute(char *SrcName,BOOL MuteState);
	int nMuteGetState(char *SrcName);
private:	
	DWORD nGetDevType(char *SrcName);
	
};
