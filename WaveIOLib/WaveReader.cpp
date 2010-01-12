///////////////////////////////////////////////////////////////////
// The wave reader, derived from CWavefile and CWaveDataStorage 
// CWaveReader provides data to sound device to play
//
// Some functions are borrowed from Alexander Beletsky
// http://www.codeguru.com/Cpp/G-M/multimedia/audio/article.php/c4739/
// 
// version 02.24.2005, Dublin, VA 24084
// author: Qiang Yu (qiangyu#gmail.com)
///////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\wavereader.h"

CWaveReader::CWaveReader(LPCSTR filename):CWaveFile(),m_filename(filename), 
m_if(filename, ios::in|ios::binary){
	ReadRIFF();
	ReadFMT();
	ReadDATA();
	m_dataoffset = m_if.tellg();
	m_dataleft = pDATA->dataSIZE;
}

CWaveReader::~CWaveReader(void){
	Close();
}

void CWaveReader::FetchData(void * buffer, unsigned long * size){
	if(!m_if.is_open()||!m_dataleft){
		*size = 0;
	}else{
		m_if.read((char *)buffer, min(*size, m_dataleft));
		*size = m_if.gcount(); m_dataleft -= *size;
	}
}

void CWaveReader::GetWavePara(unsigned long * sps, unsigned short* bps, 
							  unsigned short * nch){
	*sps = pFMT->fmtFORMAT.nSamplesPerSec;
	*bps = pFMT->fmtFORMAT.wBitsPerSample;
	*nch = pFMT->fmtFORMAT.nChannels;
}

BOOL CWaveReader::Seek(long pos, BOOL begin){
	if(!m_if.is_open())
		return FALSE;
	if(begin){
		m_dataleft = pDATA->dataSIZE;
		if(pos<0) pos = m_dataleft;
		m_if.seekg(pos = min(pos, m_dataleft) + m_dataoffset);
	}else{
		if(pos<0){
			pos = max(pos, m_dataleft - pDATA->dataSIZE);
		}else{
			pos = min(m_dataleft, pos);
		}
		m_if.seekg(pos, ios::cur);
	}
	m_dataleft -= pos;
	return TRUE;
}

void CWaveReader::Close(){	
	if(m_if.is_open())
		m_if.close();
}

inline void CWaveReader::ReadRIFF(){
	m_if.read((char*)pRIFF, sizeof(RIFF));
    if( !CheckID( pRIFF->riffID, 'R', 'I', 'F', 'F') ) {
		throw WaveErrors::RiffDoesntMatch();
        }
	if( !CheckID( pRIFF->riffFORMAT, 'W', 'A', 'V', 'E') ) {
		throw WaveErrors::WaveDoesntMatch();
	}
}

inline void CWaveReader::ReadFMT(){
	m_if.read((char*)pFMT, sizeof(FMT));
	if( !CheckID( pFMT->fmtID, 'f', 'm', 't', ' ') ) {
		throw WaveErrors::FmtDoesntMatch();
	}
}

inline void CWaveReader::ReadDATA(){
	try {
		m_if.read((char*)pDATA, sizeof(DATA));
		if( !CheckID( pDATA->dataID, 'd', 'a', 't', 'a') ) {
			throw WaveErrors::DataDoesntMatch();
		}
	}catch( WaveErrors::DataDoesntMatch & ) {
		//something stange! In Microsoft wave files DATA identifier 
		//can be offseted (maybe because of address aligment)
		//Start to looking DATA_ID "manualy" ;)
		m_if.seekg(sizeof(RIFF)+sizeof(FMT));
		BOOL foundData = FALSE;
		char ch;
		while(m_if) {
			m_if.get(ch);
			if(ch == 'd' ) {
				//It can be DATA_ID, check it!
				m_if.unget();
				m_if.read((char*)pDATA, sizeof(DATA));
				if( CheckID( pDATA->dataID, 'd','a','t','a' ) ) {
					foundData = TRUE;
					break;
				}
			}
		}
		if( !foundData ) {
			throw WaveErrors::DataDoesntMatch();
		}
	}
}

inline BOOL CWaveReader::CheckID(char *idPar, char A, char B, char C, char D){
	return(
		(idPar[0] == A) &&
		(idPar[1] == B) &&
		(idPar[2] == C) &&
		(idPar[3] == D)
		);
}
