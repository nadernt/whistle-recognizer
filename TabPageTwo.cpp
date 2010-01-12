// TabPageTwo.cpp : implementation file
//

#include "stdafx.h"
#include "waveinfft.h"
#include "TabPageTwo.h"
#include ".\tabpagetwo.h"
#include "mmsystem.h"
#include "Listener.h"
#include <iostream>
#include <fstream>
using namespace std;

#pragma comment ( lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabPageTwo dialog


CTabPageTwo::CTabPageTwo(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPageTwo::IDD, pParent)
	
{
	//{{AFX_DATA_INIT(CTabPageTwo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabPageTwo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabPageTwo)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK1, m_ChkPLSync);
	DDX_Control(pDX, IDC_CHECK2, m_chkFinalSampPL);
	DDX_Control(pDX, IDC_OSCCheck, m_CheckPreview);
	DDX_Control(pDX, IDC_txtCutOff, m_txtCutOff);
	DDX_Control(pDX, IDC_SPINCutOff, m_SpinCutOff);
	DDX_Control(pDX, IDC_SynaShow, m_SynShow);
	DDX_Control(pDX, IDC_EDIT5, m_txtLogs);
	DDX_Control(pDX, IDC_OSCCheck3, m_btnShow);
}


BEGIN_MESSAGE_MAP(CTabPageTwo, CDialog)
	//{{AFX_MSG_MAP(CTabPageTwo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_WavCat, OnBnClickedWavcat)
	ON_BN_CLICKED(IDC_PlaySamplBtn, OnBnClickedPlaysamplbtn)
	ON_BN_CLICKED(IDC_OSCCheck, OnBnClickedOsccheck)
	ON_BN_CLICKED(IDC_btnLearnListen, OnBnClickedbtnlearnlisten)
//	ON_EN_CHANGE(IDC_txtCutOff, OnEnChangetxtcutoff)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINCutOff, OnDeltaposSpincutoff)
	ON_BN_CLICKED(IDC_SynaShow, OnBnClickedSynashow)
	ON_WM_TIMER()
//	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_BN_CLICKED(IDC_OSCCheck3, OnBnClickedOsccheck3)
	ON_BN_CLICKED(IDC_OSCCheck4, OnBnClickedOsccheck4)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabPageTwo message handlers

BOOL CTabPageTwo::PreTranslateMessage(MSG* pMsg)
{
          if(pMsg->message==WM_KEYDOWN)
          {
              if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
                  pMsg->wParam=NULL ;
          }
	return CDialog::PreTranslateMessage(pMsg);
}

void CTabPageTwo::OnBnClickedWavcat()
{
	CString nPath,strResult,strTmp[3];
	nPath=Aptool.ChooseFolder(this->m_hWnd,FALSE); 
	if (Aptool.IsDirExist(nPath)==FALSE || nPath==""){
		if(nPath!="")
		AfxMessageBox("Directory is not Exist",MB_ICONEXCLAMATION);
		return;
	}

	nPath+="\\"; 
	strTmp[0]=nPath+"samp1.wav";
	
	if (!Aptool.IsFileExist(strTmp[0]))
	{
		AfxMessageBox("Sample files are not exist in this path.\n"\
			"Your may didn't capture sample files.",MB_ICONEXCLAMATION);
		return;
	}
	int bobo=0;
	__int64 Ibig=Aptool.GetFileSizeInByte(strTmp[0]);
	if(Ibig < 97044)
		{
			bobo=AfxMessageBox("Fatal Error: Sample files are less than 1 second or \nthey processed before.\n"\
			"do you want only mix them",MB_YESNO|MB_ICONEXCLAMATION);
			if (bobo==IDNO)
				return;
		}
			strTmp[1]=nPath+"samp2.wav";
			strTmp[2]=nPath+"samp3.wav";
	
	if(bobo==0)
	for(int i=0 ; i<3 ; i++){
		if (m_ChkPLSync.GetCheck()) 
		sndPlaySound(strTmp[i],SND_SYNC);
		
		DieWavSpaces(strTmp[i]);
		
		if (m_ChkPLSync.GetCheck()) 
		sndPlaySound(strTmp[i],SND_SYNC);
		}   	
		
	
	
		nPath=App_Path + "MainSample\\";

		if (!Aptool.IsDirExist(nPath))
			CreateDirectory(nPath,NULL);
		else{Aptool.EmptyDir(nPath);}
		Sleep(30);
		nPath+="FinalSample.wav";
		LearneFinalFileMixer(strTmp[0],strTmp[1],strTmp[2],nPath); 
		
		if (m_ChkPLSync.GetCheck())
		{
			AfxMessageBox("All Done now listen it!");
			sndPlaySound(nPath,SND_SYNC);
		}
		else
			AfxMessageBox("All Done!");
}

void CTabPageTwo::DieWavSpaces(const char *FileName)
{
	char *databuf;;
	short iptr;
	double MaxPositivePeak=0,AvgCofPeakPower=0;
	ifstream file;
	long datasize,j=0,ArrIndex=0;
	BOOL Find_Reliable_Peaks=FALSE;
	int Counter_Peak=0;
	char *Out_Stack= new char[SAMPLE_RATE],CopyHeader[44];
	const char *FILEIN = "Header.dat";
	const char *FILEOUT = "TempClip";
	file.open (FileName, ios::binary | ios::in);
    file.clear();
	// Move to Data point of PCM
	file.seekg(40, ios::beg);
	file.read ((char*)&datasize, 4);
    databuf = new char [datasize];
	file.read (databuf, datasize);
	file.close();
	
	//Calculating RMS.	
	for (long i = 0 ; i< datasize;i+=2)
	{
		iptr=(short&)databuf[i];
			 if (iptr > MaxPositivePeak)
				{
					MaxPositivePeak=iptr;
				}
	}
	AvgCofPeakPower=MaxPositivePeak*0.7;

	for(i=0;i<datasize;i+=2)
	{
				iptr=(short&)databuf[i];
		
			if (iptr >= AvgCofPeakPower)
				Counter_Peak++;
			if( Counter_Peak >= 4)
			{
				Find_Reliable_Peaks=TRUE;
				if((i%2)!=0)
					i--;
				// Insure for receiving to high peak levels of sound.
				for (j=i;j < SAMPLE_RATE + i ;j++){
							Out_Stack[ArrIndex++]=databuf[j];
				}
					ifstream fin(FILEIN, ios::binary | ios::in);
					ofstream fout(FILEOUT ,ios::binary | ios::out);
					fin.read(CopyHeader,44); 
					fout.write(CopyHeader,44);
					fout.write(Out_Stack,SAMPLE_RATE);
					fin.close();
					fout.close();
					DeleteFile(FileName);
					Sleep(10);
					CopyFile(FILEOUT,FileName,FALSE);
					DeleteFile(FILEOUT);
					
			}	
					
			
			if (Find_Reliable_Peaks==TRUE)
					break;
	}			
	delete []Out_Stack;
	delete []databuf;	

}

void CTabPageTwo::LearneFinalFileMixer(const char *InputFileName1,const char *InputFileName2,const char *InputFileName3,const char *OutputResultFile)
{
	char *databuf1,*databuf2,*databuf3,*Out_Stack= new char[SAMPLE_RATE],ch;
	ifstream file1,file2,file3;
	long datasize1,datasize2,datasize3;
	char CopyHeader[44];
	const char *FILEIN = "Header.dat";
	file1.open (InputFileName1, ios::binary | ios::in);
    file1.clear();
	// Move to Data point of PCM
	file1.seekg(40, ios::beg);
	file1.read ((char*)&datasize1, 4);
    databuf1 = new char [datasize1];
	file1.read (databuf1, datasize1);
	file1.close();

	file2.open (InputFileName2, ios::binary | ios::in);
    file2.clear();
	file2.seekg(40, ios::beg);
	file2.read ((char*)&datasize2, 4);
    databuf2 = new char [datasize2];
	file2.read (databuf2, datasize2);
	file2.close();

	file3.open (InputFileName3, ios::binary | ios::in);
    file3.clear();
	file3.seekg(40, ios::beg);
	file3.read ((char*)&datasize3, 4);
    databuf3 = new char [datasize3];
	file3.read (databuf3, datasize3);
	file3.close();

					ifstream fin(FILEIN, ios::binary | ios::in);
					ofstream fout(OutputResultFile ,ios::binary | ios::out);
					fin.read(CopyHeader,44); 
					fout.write(CopyHeader,44);
					//Mix Wave files for just 1 sec.
					for(long i=0 ; i< SAMPLE_RATE ; i++)
					{
							ch=databuf1[i]+databuf2[i]+databuf3[i];
							
							if ((databuf1[i]+databuf2[i]+databuf3[i]) >= 128)
								ch=127;
							else if ((databuf1[i]+databuf2[i]+databuf3[i]) <= -128)
								ch=-128;

							Out_Stack[i]=ch;
					}
					fout.write(Out_Stack,SAMPLE_RATE);
					fin.close();
					fout.close();
										
				
	delete []Out_Stack;
	delete []databuf1;	
	delete []databuf2;
	delete []databuf3;
}

void CTabPageTwo::OnBnClickedPlaysamplbtn()
{
CString T = App_Path + "MainSample\\"  ;
T+=	"FinalSample.wav";
if (!Aptool.IsFileExist(T))
	{
		AfxMessageBox("files is not exist in this path.\n"\
			"Your may didn't creat it.",MB_ICONEXCLAMATION);
		return;
	}
	sndPlaySound(T,SND_SYNC);
}

void CTabPageTwo::OnBnClickedOsccheck()
{
	if (PreviewToggle==TRUE)
	{
		PreviewToggle=FALSE;
		m_CheckPreview.SetWindowText("Preview OFF"); 
	}
	else 
	{
		PreviewToggle=TRUE;
		m_CheckPreview.SetWindowText("Preview ON"); 
	}
}

void CTabPageTwo::OnBnClickedbtnlearnlisten()
{
	Listener T;
	T.LearningSampleSound(CUT_OFF_Freq);
}

BOOL CTabPageTwo::OnInitDialog()
{
	CDialog::OnInitDialog();
	CUT_OFF_Freq=4000;
    m_txtCutOff.SetWindowText("4000"); 
	m_SpinCutOff.SetRange(0,10000);
	m_SpinCutOff.SetPos(4000); 
	Mytimer = SetTimer(1, 300, 0);
	m_ChkPLSync.SetCheck(1); 
	m_chkFinalSampPL.SetCheck(1); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTabPageTwo::OnDeltaposSpincutoff(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CUT_OFF_Freq=m_SpinCutOff.GetPos();
	*pResult = 0;
}

void CTabPageTwo::OnBnClickedSynashow()
{
		if(SynapseWeightShow)
		{
			SynapseWeightShow=FALSE;
			m_SynShow.SetWindowText("Synapse Weghit Show ON"); 
		}

		else
		{
			SynapseWeightShow=TRUE;
			m_SynShow.SetWindowText("Synapse Weghit Show OFF"); 
		}
}

void CTabPageTwo::OnTimer(UINT nIDEvent)
{
	firstTher=false;
	nI=0;
	CDialog::OnTimer(nIDEvent);
}

void CTabPageTwo::OnNcDestroy()
{
	CDialog::OnNcDestroy();
}

void CTabPageTwo::OnBnClickedOsccheck3()
{
sndPlaySound("Aligator.wav",SND_ASYNC);
}

void CTabPageTwo::OnBnClickedOsccheck4()
{
sndPlaySound(NULL,SND_ASYNC);
}
