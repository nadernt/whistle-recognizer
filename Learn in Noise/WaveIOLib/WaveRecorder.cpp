/////////////////////////////////////////////////////////////
// The wave recorder, derived from CWaveIOBase 
//
// version 02.24.2005, Dublin, VA 24084
// author: Qiang Yu (qiangyu#gmail.com)
/////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\waverecorder.h"

CWaveRecorder::CWaveRecorder(void)
{
}

CWaveRecorder::~CWaveRecorder(void)
{
	Quit();
}

inline void CWaveRecorder::SendToSoundDevice(WAVEHDR * phdr){
	if(phdr){
		waveInPrepareHeader((HWAVEIN)m_hwave, phdr, sizeof(WAVEHDR));
		waveInAddBuffer((HWAVEIN)m_hwave, phdr, sizeof(WAVEHDR));
	}
}

inline void CWaveRecorder::FreeReturnedHdr(WAVEHDR * phdr){
	if(phdr){
		if(m_datastore)
			m_datastore->StoreData(phdr->lpData, &phdr->dwBytesRecorded);
		if(phdr->dwFlags&WHDR_PREPARED)
			waveInUnprepareHeader((HWAVEIN)m_hwave, phdr, sizeof(WAVEHDR));
	}
}

inline void CWaveRecorder::PrepareFreeHdr(WAVEHDR * phdr){
	if(phdr){
		void * pdata = phdr->lpData;
		ZeroMemory(phdr, sizeof(WAVEHDR));
		phdr->lpData = (LPSTR)pdata;
		phdr->dwBufferLength = m_headerbufferlen;
	}
}
