///////////////////////////////////////////////////////////////////////////////
// Implementation of CWaveIOBase, the base class of CWavePlayer and 
// CWaveRecorder.
// CWaveIOBase maintains queues holding wave headers. Wave headers
// are processed by two threads: one thread sends prepared wave headers 
// to sound driver, the other prepares free wave headers. Details on how 
// to perform these jobs are implemented in CWavePlayer and CWaveRecorder
//
// version 02.24.2005, Dublin, VA 24084
// author: Qiang Yu (qiangyu#gmail.com)
////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\waveiobase.h"

// ctor creats two initially suspended threads
CWaveIOBase::CWaveIOBase(void):m_hwave(0),
m_quit(FALSE), m_suspend(TRUE), m_datastore(0),
m_workingthread(0), m_wsuspended(TRUE),
m_supportthread(0), m_ssuspended(TRUE)
{
	unsigned id;
	m_workingthread = 
	(HANDLE)_beginthreadex(NULL, 0, _workingthreadperoc, this, CREATE_SUSPENDED, &id);
	m_supportthread = 
	(HANDLE)_beginthreadex(NULL, 0, _supportthreadperoc, this, CREATE_SUSPENDED, &id);
}

CWaveIOBase::~CWaveIOBase(void){
}

// open a wave data storage and the sound device for
// recording or playing
// pre-condition: the new store is ready for reading or writing.
// i.e. if new storage is a file, it should be already opened
BOOL CWaveIOBase::Open(CWaveDataStore * newstore){
	if(m_datastore){
		Close();
	}

	m_datastore = newstore;
	if(m_datastore){	
		m_datastore->GetWavePara(&m_wfx.nSamplesPerSec, &m_wfx.wBitsPerSample, 
		&m_wfx.nChannels);	
		m_wfx.cbSize = 0;
		m_wfx.wFormatTag = 1;
		m_wfx.nBlockAlign = (m_wfx.wBitsPerSample>>3)*m_wfx.nChannels;
		m_wfx.nAvgBytesPerSec = m_wfx.nBlockAlign * m_wfx.nSamplesPerSec;
		InitQueue();
		return OpenDevice();
	}
	return TRUE;
}

// start recording or playing
BOOL CWaveIOBase::Start(){
	if(!m_datastore) return FALSE;
	
	if(m_suspend){		
		m_suspend = FALSE;
		m_ssuspended = FALSE;
		ResumeThread(m_supportthread);
		Sleep(100);
		m_wsuspended = FALSE;
		ResumeThread(m_workingthread);
		StartDevice();
	}
	return TRUE;
}

BOOL CWaveIOBase::Pause(){	
	return PauseDevice();
}

BOOL CWaveIOBase::Resume(){
	return ResumeDevice();
}

// close the sound device:
// Should call Open again to make the sound device work
BOOL CWaveIOBase::Close(){
	ResetQueues();
	CloseDevice();
	m_datastore = 0;
	UninitQueue();
	return TRUE;
}

// reset the data storage and the sound device call start after reset 
// will replay data from or re-record data to the data storage 
// (previously recorded data lost)
BOOL CWaveIOBase::Reset(){
	return Seek(0); 
}

BOOL CWaveIOBase::Seek(long pos, BOOL begin){
	if(m_datastore){
		ResetQueues();
		return m_datastore->Seek(pos, begin);
	}else
		return FALSE;
}

// Asking two working thread to quit and close sound device
void CWaveIOBase::Quit(){
	if(m_quit == TRUE) return;
	Close();
	m_quit = TRUE;
	m_suspend = m_wsuspended = m_ssuspended = FALSE;
	ResumeThread(m_supportthread);	
	ResumeThread(m_workingthread);
	WaitForSingleObject(m_supportthread, -1);
	WaitForSingleObject(m_workingthread, -1);
	m_datastore = 0;
}

// Suspend the two threads
// Reset queues to their initial states
inline void CWaveIOBase::ResetQueues(){
	m_suspend = TRUE;
	while(!(m_wsuspended&&m_ssuspended)){}
	ResetDevice();
	
	while(!m_readyq.empty()){
		CWaveHdr * phdr = m_readyq.front();
		m_readyq.pop();
		m_freeq.push(phdr);
	}

	while(m_inuseq.size()!=m_returnctr.Value()){
	}

	m_returnctr.Reset();

	while(!m_inuseq.empty()){
			CWaveHdr *phdr = m_inuseq.front();
			m_inuseq.pop();
			FreeReturnedHdr(phdr);
			m_freeq.push(phdr);
	}
}

// The thread sending wave headers to wave device
int CWaveIOBase::WorkingThreadProc(){
	CWaveHdr * phdr = 0;
	while(!m_quit){
		m_readyq.Lock();
		if(!m_readyq.empty()){
			phdr = m_readyq.front();
			m_readyq.pop();		
			SendToSoundDevice(phdr);
			m_inuseq.Lock();
			m_inuseq.push(phdr);
			m_inuseq.Unlock();
		}
		m_readyq.Unlock();
		
		if(m_suspend){
			m_wsuspended = TRUE;
			SuspendThread(m_workingthread);
		}

		Sleep(20);
	}
	return 0;
}

// The thread prepare wave headers to be used by the other thread 
int CWaveIOBase::SupportThreadProc(){
	CWaveHdr * phdr = 0;
	while(!m_quit){
		m_freeq.Lock();
		if(!m_freeq.empty()){
			phdr = m_freeq.front();			
			PrepareFreeHdr(phdr);
			if(phdr->dwBufferLength != 0){
				m_freeq.pop();
				m_readyq.Lock();
				m_readyq.push(phdr);
				m_readyq.Unlock();
			}
		}
		m_freeq.Unlock();
		
		if(m_returnctr.Value()!=0){
			m_inuseq.Lock();
			if(!m_inuseq.empty()){
				phdr = m_inuseq.front();
				m_inuseq.pop();
				m_returnctr.Dec();
				FreeReturnedHdr(phdr);
				m_freeq.Lock();
				m_freeq.push(phdr);
				m_freeq.Unlock();
			}
			m_inuseq.Unlock();
		}

		if(m_suspend){
			m_ssuspended = TRUE;
			SuspendThread(m_supportthread);
		}

		Sleep(20);
	}
	return 0;
}

// Allocate memory spaces for wave headers
// Initlize queues
void CWaveIOBase::InitQueue(){
	m_headerbufferlen = m_wfx.nAvgBytesPerSec/20 * 2;
	for(int t=0; t<HDRCUNT; t++){
		CWaveHdr * phdr = new CWaveHdr();
		ZeroMemory(phdr, sizeof(CWaveHdr));	
		phdr->lpData = new char[m_headerbufferlen];
		m_freeq.push(phdr);
	}
}

// Release memory spaces for wave headers
// Empty queues
void CWaveIOBase::UninitQueue(){
	while(!m_freeq.empty()){
		CWaveHdr * phdr = m_freeq.front();
		m_freeq.pop();
		delete[] phdr->lpData;
		delete phdr;
	}
}