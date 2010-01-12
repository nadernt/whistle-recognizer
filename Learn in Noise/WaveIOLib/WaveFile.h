///////////////////////////////////////////////////////////////////
// CWaveFile defines the structure of a wave file
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

#include <string>
using namespace std;

struct WAVEFORM
{
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
};

struct RIFF 
{
	char riffID[4];
	DWORD riffSIZE;
	char riffFORMAT[4];
};

struct FMT
{
	char fmtID[4];
	DWORD fmtSIZE;
	WAVEFORM fmtFORMAT;
};

struct DATA 
{
	char dataID[4];
	DWORD dataSIZE;
};

namespace WaveErrors {
	class FileOperation {};
	class RiffDoesntMatch {};
	class WaveDoesntMatch {};
	class FmtDoesntMatch {};
	class DataDoesntMatch {};
}

class CWaveFile{
public:
	CWaveFile(){
		pRIFF = new RIFF();
		pFMT = new FMT();
		pDATA = new DATA();
	};
	virtual ~CWaveFile(){
		delete pRIFF;
		delete pFMT;
		delete pDATA;
	};

protected:
	RIFF* pRIFF;
	FMT* pFMT;
	DATA* pDATA;
};