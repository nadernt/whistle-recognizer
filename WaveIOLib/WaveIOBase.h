////////////////////////////////////////////////////////
// Declaration of CWaveIOBase, the base class of 
// CWavePlayer and CWaveRecorder
//
// version: 02/24/2005, Dublin, VA 24084
// author: Qiang Yu ( qiangyu#gmail.com)
////////////////////////////////////////////////////////
#pragma once

#ifdef new
#undef new
#endif


#include <windows.h>
#include <mmsystem.h>
#include <algorithm>
#include <queue>
#include <process.h>

using namespace std;

class CWaveHdr:
public WAVEHDR{

};

class CQueue:
public queue<CWaveHdr *>{
public:
	CQueue(){
		InitializeCriticalSection(&m_cs);
	}
	~CQueue(){
		DeleteCriticalSection(&m_cs);
	}

	void Lock(){
		EnterCriticalSection(&m_cs);
	}

	void Unlock(){
		LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;
};

class CCounter{
public:
	CCounter(){
		InitializeCriticalSection(&m_cs);
		count = 0;
	}

	~CCounter(){
		DeleteCriticalSection(&m_cs);
	}

	void Inc(){
		EnterCriticalSection(&m_cs);
		++count;
		LeaveCriticalSection(&m_cs);
	}

	void Dec(){
		EnterCriticalSection(&m_cs);
		--count;
		LeaveCriticalSection(&m_cs);
	}

	void Reset(){
		EnterCriticalSection(&m_cs);
		count = 0;
		LeaveCriticalSection(&m_cs);
	}

	unsigned Value(){
		return count;
	}

private:
	unsigned count;
	CRITICAL_SECTION m_cs;
};

// Interface definition, through which wave player gets necessary data to play
// and wave recorder saves recorded data
//
class CWaveDataStore{
public:
	//save data
	virtual void StoreData(const void *, const unsigned long *){};
	//get data. If no more data, should set size to 0
	virtual void FetchData(void *, unsigned long *size){*size = 0;};
	//get wave parameters: samples per second, bits per sample and number of channels
	virtual void GetWavePara(unsigned long *, unsigned short*, unsigned short*) = 0;
	//set the current playing/recording position.
	//BOOLean parameter determines whether it is an absolute seeking (related to 0ms, 
	//the beginning)
	//or a relative one (related to curren pos)
	virtual BOOL Seek(long, BOOL begin = TRUE)=0;
};

#define HDRCUNT 8

class CWaveIOBase
{
public:
	CWaveIOBase(void);
	virtual ~CWaveIOBase(void);

	// Operations allowed on player or recorder 
	// General operation sequence:
	// Open --> Start --> [ Seek | Reset, Start ] --> [ Pause, Resume ] -->Close 
	// [Op1, Op2] means Op2 must be invoked after Op1 to make player or recorder
	// continue working. Other ops (except Close) would be ignored.
	// 
	// It is allowed to call Open without calling Close first. e.g.
	// Open(d1) --> Start --> Open(d2) --> Start --> Open(d1) --> Start --> Close
	// In this example, after the second Start, d2 will be played (or 
	// recorded); after the third Start, d1 will be CONTINUED from its previous
	// position. That means if you do not call Close on CWaveDataStorage object, 
	// its current playing, recording pos will be retained. 
	BOOL Open(CWaveDataStore *);
	BOOL Close();
	BOOL Start();
	BOOL Pause();
	BOOL Resume();
	BOOL Reset();
	BOOL Seek(long, BOOL begin = TRUE);

	// Kill threads
	void Quit();

protected:
	// CWaveIOBase distinguishes three different wave header types
	// 1. free, held in queue m_freeq: free wave headers
	// 2. in use, held in queue m_inuseq: wave headers held by sound device
	// 3. ready: wave headers waiting to be sent to sound device
	
	// Send ready headers to sound device
	virtual void SendToSoundDevice(WAVEHDR *) = 0;
	// Fill information into a free wave header to make it ready
	virtual void PrepareFreeHdr(WAVEHDR *) = 0;
	// Process a returned (released by sound driver) wave header to make it free
	virtual void FreeReturnedHdr(WAVEHDR *) = 0;

	virtual BOOL OpenDevice() = 0;
	virtual BOOL CloseDevice() = 0;
	virtual BOOL StartDevice() = 0;
	virtual BOOL PauseDevice() = 0;
	virtual BOOL ResumeDevice() = 0;	
	virtual BOOL ResetDevice() = 0;
	
	static void CALLBACK WaveIOProc(HWAVE, UINT umsg, 
		DWORD objptr, DWORD, DWORD){
		if(umsg == WOM_DONE || umsg == WIM_DATA)
			((CWaveIOBase *)objptr)->DoCallBack();
	};

	virtual void DoCallBack(){
		m_returnctr.Inc();
	};

	int WorkingThreadProc();
	int SupportThreadProc();
	void ResetQueues(); 
	void InitQueue();
	void UninitQueue();

private:
	static unsigned __stdcall _workingthreadperoc(void * p){
		return ((CWaveIOBase*)p)->WorkingThreadProc();
	}

	static unsigned __stdcall _supportthreadperoc(void * p){
		return ((CWaveIOBase*)p)->SupportThreadProc();
	}

protected:
	HWAVE  m_hwave;
	WAVEFORMATEX   m_wfx;
	long   m_headerbufferlen; //length of header buffer, 100ms wave data by default

	CWaveDataStore * m_datastore;

private:
	CQueue m_freeq; //free header queue
	CQueue m_inuseq; //inuse header queue
	CQueue m_readyq; //ready header queue

	CCounter m_returnctr; // count of currently released headers
	
	HANDLE m_workingthread;
	BOOL   m_wsuspended;
	HANDLE m_supportthread;	
	BOOL   m_ssuspended;

	BOOL m_quit;
	BOOL m_suspend;
};
