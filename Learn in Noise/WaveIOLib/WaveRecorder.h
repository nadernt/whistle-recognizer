/////////////////////////////////////////////////////////////
// The wave recorder, derived from CWaveIOBase 
//
// version 02.24.2005, Dublin, VA 24084
// author: Qiang Yu (qiangyu#gmail.com)
/////////////////////////////////////////////////////////////
#pragma once

#ifdef new
#undef new
#endif

#include "waveiobase.h"

class CWaveRecorder :
	public CWaveIOBase
{
public:
	CWaveRecorder(void);
	virtual ~CWaveRecorder(void);

protected:
	virtual void SendToSoundDevice(WAVEHDR *);
	virtual void PrepareFreeHdr(WAVEHDR *);
	virtual void FreeReturnedHdr(WAVEHDR *);

	virtual BOOL OpenDevice(){
		if(waveInOpen((HWAVEIN *)&m_hwave, WAVE_MAPPER, &m_wfx, 
			(DWORD_PTR)CWaveRecorder::WaveIOProc, 
			(DWORD)this, CALLBACK_FUNCTION)!=MMSYSERR_NOERROR){
			m_hwave = 0;
			return FALSE;
		}
		else
			return TRUE;
	};
	
	virtual BOOL CloseDevice(){
		waveInClose((HWAVEIN)m_hwave);
		return TRUE;
	};

	virtual BOOL StartDevice(){
		waveInStart((HWAVEIN)m_hwave);
		return TRUE;
	};

	virtual BOOL PauseDevice(){
		waveInStop((HWAVEIN)m_hwave);
		return TRUE;
	};

	virtual BOOL ResumeDevice(){
		waveInStart((HWAVEIN)m_hwave);
		return TRUE;
	};

	virtual BOOL ResetDevice(){
		waveInReset((HWAVEIN)m_hwave);
		return TRUE;
	};
};
