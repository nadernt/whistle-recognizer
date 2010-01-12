/////////////////////////////////////////////////////////////
// The wave player, derived from CWaveIOBase 
//
// version 02.24.2005, Dublin, VA 24084
// author: Qiang Yu (qiangyu#gmail.com)
/////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\waveplayer.h"

CWavePlayer::CWavePlayer(void)
{
}

CWavePlayer::~CWavePlayer(void)
{
	Quit();
}

inline void CWavePlayer::SendToSoundDevice(WAVEHDR * phdr){
	if(phdr){		
		waveOutPrepareHeader((HWAVEOUT)m_hwave, phdr, sizeof(WAVEHDR));
		waveOutWrite((HWAVEOUT)m_hwave, phdr, sizeof(WAVEHDR));
	}
}

inline void CWavePlayer::FreeReturnedHdr(WAVEHDR * phdr){
	if(phdr){
		if(phdr->dwFlags&WHDR_PREPARED){
			waveOutUnprepareHeader((HWAVEOUT)m_hwave, phdr, sizeof(WAVEHDR));
		}		
	}
}

inline void CWavePlayer::PrepareFreeHdr(WAVEHDR * phdr){
	if(phdr){
		void * pdata = phdr->lpData;
		ZeroMemory(phdr, sizeof(WAVEHDR));
		phdr->lpData = (LPSTR)pdata;
		phdr->dwBufferLength = m_headerbufferlen;
		if(m_datastore)
			m_datastore->FetchData(phdr->lpData, &phdr->dwBufferLength);
	}
}
