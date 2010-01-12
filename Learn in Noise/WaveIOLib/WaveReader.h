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
#pragma once

#ifdef new
#undef new
#endif

#include "wavefile.h"
#include "waveiobase.h"
#include <fstream>

using namespace std;

class CWaveReader :
	public CWaveFile, public CWaveDataStore
{
public:
	CWaveReader(LPCSTR filename);
	~CWaveReader(void);

	//interfaces required by CWaveIOBase
	virtual void FetchData(void *, unsigned long *);
	virtual void GetWavePara(unsigned long *, unsigned short*, unsigned short*);
	virtual BOOL Seek(long, BOOL begin = TRUE);

	//Close wave file
	virtual void Close();

protected:
	void ReadRIFF();
	void ReadFMT();
	void ReadDATA();
	BOOL CWaveReader::CheckID(char *, char, char, char, char);

private:
	ifstream m_if;
	string m_filename;
	unsigned long m_dataoffset;
	unsigned long m_dataleft;
};
