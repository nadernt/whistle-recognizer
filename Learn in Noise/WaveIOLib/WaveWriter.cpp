///////////////////////////////////////////////////////////////////
// The wave writer, derived from CWavefile and CWaveDataStorage 
// CWaveWriter writes data recorded by sound device
//
// Some functions are borrowed from Alexander Beletsky
// http://www.codeguru.com/Cpp/G-M/multimedia/audio/article.php/c4739/
// 
// version 02.24.2005, Dublin, VA 24084
// author: Qiang Yu (qiangyu#gmail.com)
///////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\wavewriter.h"

CWaveWriter::CWaveWriter(LPCSTR filename, unsigned long sps,
						 unsigned short bps, unsigned short nch):
CWaveFile(), m_filename(filename), 
m_of(filename, ios::out|ios::binary|ios::trunc){
	if(!m_of.is_open()){	
		throw WaveErrors::FileOperation();
	}

	m_dataoffset = sizeof(RIFF)+sizeof(FMT)+sizeof(DATA);
	m_datalength = 0;
	//RIFF
	strncpy(pRIFF->riffID, "RIFF", 4);
	pRIFF->riffSIZE = m_dataoffset - 8;
	strncpy(pRIFF->riffFORMAT, "WAVE", 4);
	
	//FMT
	strncpy(pFMT->fmtID, "fmt ", 4);
	pFMT->fmtSIZE = sizeof(WAVEFORM);
	pFMT->fmtFORMAT.wFormatTag = 1;
	pFMT->fmtFORMAT.nChannels = nch;
	pFMT->fmtFORMAT.nSamplesPerSec = sps;
	pFMT->fmtFORMAT.nBlockAlign = (bps>>3)*nch;
	pFMT->fmtFORMAT.nAvgBytesPerSec = pFMT->fmtFORMAT.nBlockAlign*sps;
	pFMT->fmtFORMAT.wBitsPerSample = bps;

	//DATA
	strncpy(pDATA->dataID, "data", 4);
	pDATA->dataSIZE = 0;	

	m_of.write((char*)pRIFF, sizeof(RIFF));
	m_of.write((char*)pFMT, sizeof(FMT));
	m_of.write((char*)pDATA, sizeof(DATA));
}

CWaveWriter::~CWaveWriter(void){
	Close();
}

void CWaveWriter::StoreData(const void * ptr, const unsigned long * length){
	if(!m_of.is_open()) return;
	m_datalength += *length;
	m_of.write((char*)ptr, *length);
}

void CWaveWriter::GetWavePara(unsigned long * sps, unsigned short* bps, unsigned short* nch){
	*sps = pFMT->fmtFORMAT.nSamplesPerSec;
	*bps = pFMT->fmtFORMAT.wBitsPerSample;
	*nch = pFMT->fmtFORMAT.nChannels;
}

// for file writer, we only allow pos = 0 & begin = TRUE
BOOL CWaveWriter::Seek(long pos, BOOL begin){
	if(!begin||pos!=0||!m_of.is_open()) return FALSE;
	m_datalength = 0;
	m_of.close();
	m_of.open(m_filename.c_str(), ios::out|ios::binary|ios::trunc);
	pRIFF->riffSIZE = m_dataoffset - 8;
	pDATA->dataSIZE = 0;
	m_of.write((char*)pRIFF, sizeof(RIFF));
	m_of.write((char*)pFMT, sizeof(FMT));
	m_of.write((char*)pDATA, sizeof(DATA));	
	return FALSE;
}

void CWaveWriter::Close(){
	if(m_of.is_open()){
		m_of.seekp(0);
		pRIFF->riffSIZE += m_datalength;
		pDATA->dataSIZE += m_datalength;
		m_of.write((char*)pRIFF, sizeof(RIFF));
		m_of.write((char*)pFMT, sizeof(FMT));
		m_of.write((char*)pDATA, sizeof(DATA));
		m_of.close();
	}
}


