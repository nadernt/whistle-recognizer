// TabPageOne.cpp : implementation file
//
#include "stdafx.h"
#include "waveinfft.h"
#include "TabPageOne.h"
#include ".\tabpageone.h"
#include "StepsParameter.h"
#include "AppTools.h"
#include <iostream>
#include <fstream>
#define MAXVOL 32767
#define TICFEREQ 3276
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CTabPageOne dialog


CTabPageOne::CTabPageOne(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPageOne::IDD, pParent), writer(0)
	, blPeakLighten(FALSE)
	,ListenPeakRangMin(250)
	, m_FirsttxtEntry1(FALSE)
	, m_FirsttxtEntry2(FALSE)
{
	
	//{{AFX_DATA_INIT(CTabPageOne)
	//}}AFX_DATA_INIT
}
CTabPageOne::~CTabPageOne()
{
	recorder.Quit(); /////!!!!!REMEMBER TO QUIT THE RECORDER
	delete writer;   /////
}

void CTabPageOne::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabPageOne)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PROGRESS1, m_Peak);
	DDX_Control(pDX, IDC_PeakMon, m_PeakmonBtn);
	DDX_Control(pDX, IDC_txtPeak, m_TxtPeak);
	DDX_Control(pDX, IDC_PeakHot, m_HotPoint);
	DDX_Control(pDX, IDC_txtLog, m_TxtLogs);
	DDX_Control(pDX, IDC_New, m_NewBtn);
	DDX_Control(pDX, IDC_SourceSel, m_SelSourcBtn);
	DDX_Control(pDX, IDC_PeakMon2, m_PeakmonLightBtn);
	DDX_Control(pDX, IDC_PROGRESS2, m_EnvPeak);
	DDX_Control(pDX, IDC_SLIDER1, m_LineInRec);
	DDX_Control(pDX, IDC_SLIDER2, m_OutMixRec);
	DDX_Control(pDX, IDC_SLIDER3, m_MicRec);
	DDX_Control(pDX, IDC_SLIDER5, m_SpeakerVol);
	DDX_Control(pDX, IDC_SLIDER6, m_WaveVol);
	DDX_Control(pDX, IDC_SLIDER7, m_MicVol);
	DDX_Control(pDX, IDC_SLIDER4, m_LineInVol);
	DDX_Control(pDX, IDC_ChkSp, m_ChkSP);
	DDX_Control(pDX, IDC_ChkWav, m_ChkWav);
	DDX_Control(pDX, IDC_ChkMic, m_ChkMic);
	DDX_Control(pDX, IDC_ChkLine, m_ChkLine);
	DDX_Control(pDX, IDC_PeakMon3, m_chkAreaMon);
	DDX_Control(pDX, IDC_EDIT2, m_txtThreshold);
	DDX_Control(pDX, IDC_SPIN3, m_SpinThreshold);
	DDX_Control(pDX, IDC_EDIT3, m_txtTrigThreshold);
	DDX_Control(pDX, IDC_SPIN4, m_SpinTrigThreshold);
	//DDX_Control(pDX, IDC_EDIT5, m_t1);
}


BEGIN_MESSAGE_MAP(CTabPageOne, CDialog)
	//{{AFX_MSG_MAP(CTabPageOne)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_New, OnBnClickedNew)
	ON_BN_CLICKED(IDC_PeakMon, OnBnClickedPeakmon)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SourceSel, OnBnClickedSourcesel)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PeakMon2, OnBnClickedPeakmon2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, OnNMCustomdrawSlider5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, OnNMCustomdrawSlider6)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER7, OnNMCustomdrawSlider7)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, OnNMCustomdrawSlider4)
	ON_BN_CLICKED(IDC_RADIOLINE, OnBnClickedRadioline)
	ON_BN_CLICKED(IDC_RADIOOUTMIX, OnBnClickedRadiooutmix)
	ON_BN_CLICKED(IDC_RADIOMIC, OnBnClickedRadiomic)
	ON_BN_CLICKED(IDC_ChkSp, OnBnClickedChksp)
	ON_BN_CLICKED(IDC_ChkWav, OnBnClickedChkwav)
	ON_BN_CLICKED(IDC_ChkMic, OnBnClickedChkmic)
	ON_BN_CLICKED(IDC_ChkLine, OnBnClickedChkline)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpin3)
	ON_BN_CLICKED(IDC_PeakMon3, OnBnClickedPeakmon3)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT2, OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnEnChangeEdit3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, OnDeltaposSpin4)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabPageOne message handlers
BOOL CTabPageOne::OnInitDialog()
{
	CDialog::OnInitDialog();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTabPageOne::OnPaint()
{
	CPaintDC dc(this);
	LoadBitmapfromFile(mCourtWnd,PicFileName);
	ChangeCtrlColore(m_HotPoint.m_hWnd,Ctrlrgb);
}

void CTabPageOne::InitApp()
{
	AppTools apptool;
	m_Peak.SetRange(0,MAXVOL);
	m_Peak.SetStep(10); 
	m_EnvPeak.SetRange(0,MAXVOL);
	m_EnvPeak.SetStep(10); 
	
	m_PeakmonBtn.SetCheck(TRUE); 
	nIDEventTimer1 = SetTimer(1, 10, 0);
	PicFileName = App_Path + "Pics\\" + "court.bmp";
	LoadBitmapfromFile(mCourtWnd,PicFileName);
	
	CString Tmpstr=App_Path +"Settings\\" +"Setting.ini";
	Number_Mic_Sources=apptool.GetSettingInt(Tmpstr,"SourceNumber","Sampler");
	ListenPeakRangMin=apptool.GetSettingInt(Tmpstr,"Threshold","Sampler");
	m_SpinThreshold.SetRange(0,500);
	m_SpinThreshold.SetPos(ListenPeakRangMin); 
	CString Tmp1;
	Tmp1.Format("%d",ListenPeakRangMin);
	m_txtThreshold.SetWindowText(Tmp1);

	TrigThreshold=apptool.GetSettingInt(Tmpstr,"TrigThreshold","Sampler");
	m_SpinTrigThreshold.SetRange(0,100);
	m_SpinTrigThreshold.SetPos(TrigThreshold); 
	Tmp1.Format("%d",TrigThreshold);
	m_txtTrigThreshold.SetWindowText(Tmp1);


	Ctrlrgb=RGB(0,255,0);
	IsMaxPeak=FALSE;
	RecordCommand=FALSE;
	breakWizardLoop=FALSE;
	IsAtRecord=FALSE;
	blPeakAreaSound=FALSE;
	breakWizardLoop=0;
	mToggle=TRUE;
	IsNormalEndWizard=TRUE;
	m_TxtLogs.SetWindowText("Record Idle");
	blPeakLighten=TRUE;


	m_LineInRec.SetRange(0,MAXVOL,TRUE); 
	m_LineInRec.SetTicFreq(TICFEREQ);
	m_LineInRec.SetPos((int) (MAXVOL-(Snd.nGetVolume("OUT_LINE",1)/2)));
	
	m_OutMixRec.SetRange(0,MAXVOL,TRUE); 
    m_OutMixRec.SetTicFreq(TICFEREQ);
	m_OutMixRec.SetPos((int) (MAXVOL-(Snd.nGetVolume("OUT_WAVE",1)/2)));
		
	m_MicRec.SetRange(0,MAXVOL,TRUE); 
    m_MicRec.SetTicFreq(TICFEREQ);
	m_MicRec.SetPos((int) (MAXVOL-(Snd.nGetVolume("OUT_MICROPHONE",1)/2)));
	
	m_SpeakerVol.SetRange(0,MAXVOL,FALSE); 
    m_SpeakerVol.SetTicFreq(TICFEREQ);
	m_SpeakerVol.SetPos((int) (MAXVOL-(Snd.nGetVolume("Baboo",0)/2)));
	
	m_WaveVol.SetRange(0,MAXVOL,TRUE); 
    m_WaveVol.SetTicFreq(TICFEREQ);
	m_WaveVol.SetPos((int) (MAXVOL-(Snd.nGetVolume("OUT_WAVE",2)/2)));

	m_MicVol.SetRange(0,MAXVOL,TRUE); 
    m_MicVol.SetTicFreq(TICFEREQ);
	m_MicVol.SetPos((int) (MAXVOL-(Snd.nGetVolume("OUT_MICROPHONE",2)/2)));
	
	m_LineInVol.SetRange(0,MAXVOL,TRUE); 
    m_LineInVol.SetTicFreq(TICFEREQ);
	m_LineInVol.SetPos((int) (MAXVOL-(Snd.nGetVolume("OUT_LINE",2)/2)));

	//Snd.nSourceSelector("Microphone");
	Snd.nSourceSelector("Wave Out Mix"); 
	CheckDlgButton(IDC_RADIOOUTMIX, BST_CHECKED);   
	CheckDlgButton(IDC_RADIOMIC, BST_UNCHECKED);
	CheckDlgButton(IDC_RADIOLINE, BST_UNCHECKED);
	
	BOOL bl=FALSE;
	
	if (Snd.nMuteGetState("OUT_SPEAKERS")==1)
		bl=TRUE;
	else
		bl=FALSE;
		m_ChkSP.SetCheck(bl); 
			
	if (Snd.nMuteGetState("OUT_WAVE")==1)
		bl=TRUE;
	else
		bl=FALSE;
		m_ChkWav.SetCheck(bl); 
			
	if (Snd.nMuteGetState("OUT_MICROPHONE")==1)
		bl=TRUE;
	else
		bl=FALSE;
		m_ChkMic.SetCheck(bl); 
			
	if (Snd.nMuteGetState("OUT_LINE")==1)
		bl=TRUE;
	else
		bl=FALSE;
		m_ChkLine.SetCheck(bl); 
		
	/*
"IN_DIGITAL"
"IN_HEADPHONES"
"IN_LINE"
"IN_MONITOR"
"IN_SPEAKERS"
"IN_TELEPHONE"
"IN_VOICE"
"IN_WAVE"
"OUT_ANALOG"
"OUT_AUXILIARY"
"OUT_COMPACTDISC"
"OUT_DIGITAL"
"OUT_LINE"
"OUT_MICROPHONE"
"OUT_SPEAKERS"
"OUT_SYNTHESIZER"
"OUT_TELEPHONE"
"OUT_WAVE"
*/
	
/* In My sound card was :
"Wave Out Mix"
"Mono Out"
"Phone"
"Line In"
"Aux"
"Microphone"
"CD Player"
*/

}

BOOL CTabPageOne::PreTranslateMessage(MSG* pMsg)
{
          if(pMsg->message==WM_KEYDOWN)
          {
              if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
                  pMsg->wParam=NULL ;
          }
	return CDialog::PreTranslateMessage(pMsg);
}

void  CTabPageOne::DiskWaveFileHandler(CString WaveNameFile,CString OldWavfile,int iState) 
//State 1: rename file. 2: delete 3: Delete all files in Directory 4: Create Directory 5: Remove directory 6: Rename Directory
{
	if (iState==1){
		rename(OldWavfile,WaveNameFile);
	}
	else if (iState==2){
			DeleteFile(WaveNameFile);
	}
	else if (iState==3)
	{

		CFileFind ff;
		CString path = WaveNameFile;
	
		if(path.Right(1) != "\\")
		path += "\\";

		path += "*.*";

		BOOL res = ff.FindFile(path);

		while(res)
		{	
			res = ff.FindNextFile();
			if (!ff.IsDots() && !ff.IsDirectory())
				DeleteFile(ff.GetFilePath());
		}
	}
	else if (iState==4)
		CreateDirectory(WaveNameFile,NULL);
	else if (iState==5)
		RemoveDirectory(WaveNameFile);
	else if (iState==6)
	{	CFile Bo;
		Bo.Rename(OldWavfile,WaveNameFile);
	}
}

void  CTabPageOne::NewWaveForRecord()
{

	CStringA WaveNameFile = App_Path + "Temp.wav";
	CWaveWriter * ptmp = 0;
	try{
		ptmp = new CWaveWriter(WaveNameFile.GetString(), 48000, 16, 1);
		recorder.Close();
		if(writer){
			writer->Close();
			delete writer;
		}
		writer = ptmp;
		recorder.Open(writer);
	}catch(...){
		AfxMessageBox(_T("File Create Error"));
	}

}

void CTabPageOne::OnBnClickedNew()
{
	if (AfxMessageBox("Do you want to capture sample?\nBefore this, you should declared number of sources.\nNOTE: All of last sampled files may delete.",
					MB_YESNO|MB_ICONINFORMATION)==IDYES)
	{	
		m_NewBtn.EnableWindow(FALSE); 
		m_SelSourcBtn.EnableWindow(FALSE);
		m_PeakmonBtn.EnableWindow(FALSE);
		IsNormalEndWizard=FALSE;
		
		CString strTmp;
		strTmp=App_Path + "Temp.wav";
		DiskWaveFileHandler(strTmp,"",2);//Delete temporary wave file if exist
		
		strTmp=App_Path + "WTemp\\";
		DiskWaveFileHandler(strTmp,"",3); //Delete all file in temporary directory if exist
		DiskWaveFileHandler(strTmp,"",4); //Create temporary directory.
		
		strTmp=App_Path + "Wave Samples\\";
		DiskWaveFileHandler(strTmp,"",3); //Delete all file in Wave Samples directory if exist
		DiskWaveFileHandler(strTmp,"",5); //Remove Wave Samples directory.
		
		mToggle=TRUE;
		nIDEventTimer1 = SetTimer(1, 10, 0);
		m_PeakmonBtn.SetWindowText("Monitor ON"); 
		m_PeakmonBtn.SetCheck(TRUE);
		BOOL IsMyfirstInstant=FALSE;
		CString CounterF,scrMessgae;
		int FileCount=1,i;
			
		while(breakWizardLoop==FALSE)
		{
		
			if(RecordCommand==FALSE){

						if (IsMyfirstInstant!=TRUE){
									IsMyfirstInstant=TRUE;
									PicFileName = App_Path + "Pics\\" + "court1.bmp";
									LoadBitmapfromFile(mCourtWnd,PicFileName);
									NewWaveForRecord();
									AfxMessageBox("Please go to located place by court plane\nandclick on smart finder,then blow to whistle for more than 1 Second.\n\nNow,if you ready press Ok for recording first sample.");
									RecordCommand=TRUE;
						}
						else
						{
									//Stop Command for recoder class.
									recorder.Close(); //1. do not call Reset here since it will erase previously recorded sound	
									//2. call close to save previously recorded data
									writer->Close();  //3, you'd better close the writer/reader after closing the recorder/player
									CounterF.Format("%d",FileCount); 
									Wavfilename = App_Path + "WTemp\\Samp" + CounterF + ".wav";
									DiskWaveFileHandler(Wavfilename,App_Path+"Temp.wav",1); //Rename Temp file to specify file queue.
									if (FileCount == Number_Mic_Sources )
									{
										scrMessgae = "All sample file was recorded.\ndid your recent recoded sample was correct?\n\nNOTE:\nCancel->for Ending.\nRetry->for sample recent file Again";	
										IsNormalEndWizard=TRUE;
										i=AfxMessageBox(scrMessgae,MB_RETRYCANCEL|MB_ICONINFORMATION); //when in Last record time
									}
									else
									{
										scrMessgae = "Samp" + CounterF + ".wav wave sample file was record.\ndid your recoded sample was correct?\nrecorde another sample?";
										i=AfxMessageBox(scrMessgae,MB_YESNOCANCEL|MB_ICONINFORMATION); //In normal sequence record
									}
									
									if (i==IDYES){
											FileCount++;	
											CounterF.Format("%d",FileCount);
											PicFileName = App_Path + "Pics\\" + "court" + CounterF + ".bmp";
											LoadBitmapfromFile(mCourtWnd,PicFileName);
											CounterF="";
											//Command for allocating new wave file.
											NewWaveForRecord();
											AfxMessageBox("Please go to located place by court plane.\nthen blow to whistle for more than 1 Second.\n\nNow if you ready press Ok for recording first sample.\nNOTE:\nSytem is auto record.");
											RecordCommand=TRUE;
										}
									else if (i==IDNO || i==IDRETRY ){
											DiskWaveFileHandler(Wavfilename,"" ,2);
											Sleep(10);
											CounterF="";
											NewWaveForRecord();
											RecordCommand=TRUE;
										}
									else{//CANCEL was selected
										RecordCommand=FALSE;
										PicFileName = App_Path + "Pics\\" + "court.bmp";
										LoadBitmapfromFile(mCourtWnd,PicFileName);
										DiskWaveFileHandler(App_Path + "Temp.wav","",2);
										
										if (IsNormalEndWizard==TRUE){
										DiskWaveFileHandler(App_Path + "Wave Samples\\",App_Path + "WTemp\\",6);
										AfxMessageBox("All of your sample saved in <Wave Samples> directory.",MB_ICONINFORMATION); 
										}
										m_NewBtn.EnableWindow(TRUE);
										m_SelSourcBtn.EnableWindow(TRUE);
										m_PeakmonBtn.EnableWindow(TRUE);
										break;

										}
			
						}
			}
			
		DoEvents(); //For system messages.

		}// End of while loop.
	} //End of top if.
}


//Wavfilename
void CTabPageOne::WaveRecorder()
{
	if(writer == 0){
		AfxMessageBox(_T("Create A File First"));
		return;
	}

	if(m_paused)
		recorder.Resume();

	recorder.Start();
	m_paused = FALSE;
}

void CTabPageOne::OnTimer(UINT nIDEvent)
{
	int i;
	i=nGetPeak();
	if(blPeakLighten==TRUE)
		{
			Ctrlrgb=RGB(i+125,0,0);
			ChangeCtrlColore(m_HotPoint.m_hWnd,Ctrlrgb);
		}
	else
	{
		if(i>TrigThreshold)
			Ctrlrgb=RGB(255,0,0);
		else	
			Ctrlrgb=RGB(235,233,237);

			ChangeCtrlColore(m_HotPoint.m_hWnd,Ctrlrgb);
					
	}
	
	if(nIDEvent==1)
	{
										
			if (RecordCommand==TRUE)
			{
				if (IsAtRecord==FALSE)
				{	
				//TrigThreshold is Triger Point and start recording
					if (i>=TrigThreshold)
					{
						IsAtRecord=TRUE; // For eliminating reenter in record thread.
						m_TxtLogs.SetWindowText("Good Peak... Now recording.");
						WaveRecorder();
						SetTimer(2, 2500, 0);
					}
					else
					{
						m_TxtLogs.SetWindowText("Looking for suit Peak. Weak"); 
					}
				}
			}
	}
	else if(nIDEvent==2)
	{
		KillTimer(2);
		m_TxtLogs.SetWindowText("Record Idle.");
		IsAtRecord=FALSE;
		RecordCommand=FALSE;
	}

/*CStringA Toti;
	m_Text.GetWindowText(Toti);
	char *tch=NULL;
	tch=new char[Toti.GetAllocLength()];
	Toti.CopyChars(tch,Toti.GetBuffer(),Toti.GetAllocLength()+1);*/
	CDialog::OnTimer(nIDEvent);
}

int CTabPageOne::nGetPeak()
{
	double WhistleArea=0,AllArea=0,NewVal=1,tmp1=0,tmp2=0;
	int ListenPeakRangMax=512,i=0,j; 
	
	//Hear is only for optimiziation and faster speed.
	if(blPeakAreaSound==FALSE)
		i=ListenPeakRangMin;
	
	/*	ListenPeakRangMin -> Minimum Frequency ListenPeak			*
	 *	ListenPeakRangMax -> Maximum Frequency ListenPeakRangMax	*
	 *	Note: for (int ..,i<Fourier Samples ...;i++)				*/
	for( ; i< ListenPeakRangMax ; i++)
	{
		 if (f_Buffer[i]>WhistleArea && i >= ListenPeakRangMin)
		 {			
			 WhistleArea=f_Buffer[i];
			if(blPeakAreaSound==FALSE)
				AllArea=f_Buffer[i];
		 }
		 if(f_Buffer[i]>AllArea  && blPeakAreaSound==TRUE)
			 AllArea=f_Buffer[i];
	}
	
	m_Peak.SetPos(WhistleArea);
	m_EnvPeak.SetPos(AllArea); 
	CString T;
	i=(int)((WhistleArea/65536)*100);
	j=(int)((AllArea/65536)*100);
	T.Format("%%%d : %%%d",j, i);
	m_TxtPeak.SetWindowText( T ); 
	
		return i;
	
}

void CTabPageOne::OnBnClickedPeakmon()
{

	if (mToggle==FALSE)
	{
		mToggle=TRUE;
		nIDEventTimer1 = SetTimer(1, 10, 0);
		m_PeakmonBtn.SetWindowText("Monitor ON"); 
	}
	else
	{
		mToggle=FALSE;
		KillTimer(nIDEventTimer1);  
		m_PeakmonBtn.SetWindowText("Monitor OFF");
		m_Peak.SetPos(0); 
		
	}
	
}

void CTabPageOne::LoadBitmapfromFile(HWND hWnd, CString sFilename)
{

	HANDLE BitHandle;
	BITMAP bm;
	HDC hdc = ::GetDC(hWnd);
    BitHandle = LoadImage(NULL, sFilename, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
    if(BitHandle == NULL)
	{
        AfxMessageBox("Error loading the specified bitmap",MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
    }
    HDC dc = CreateCompatibleDC(hdc);
    SelectObject(dc, BitHandle);
    GetObject(BitHandle, sizeof(BITMAP), &bm);
    BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, dc, 0,0, SRCCOPY); 
	::ReleaseDC(hWnd, hdc);
}

void CTabPageOne::OnBnClickedSourcesel()
{
	StepsParameter ParDlg;
	ParDlg.DoModal(); 
	Number_Mic_Sources=ParDlg.NumberOfSources;
}

void  CTabPageOne::ChangeCtrlColore(HWND hWnd,COLORREF Crgb)
{
	COLORREF brColor= Crgb;
	CRect Recto;
	CClientDC dc(this);
	::GetWindowRect(hWnd,Recto);
	ScreenToClient(&Recto);
	CBrush BrushColor(brColor);
	CBrush *pBrush = dc.SelectObject(&BrushColor);
	dc.Rectangle(&Recto);
	dc.SelectObject(pBrush);
}

void CTabPageOne::DoEvents()
{
    MSG msg;
    PeekMessage(&msg,NULL,0,0,PM_REMOVE);
    {
		if (msg.message==WM_NULL)
			WhenIamDestroying();

		TranslateMessage(&msg);
	   DispatchMessage(&msg);
    }
}

void CTabPageOne::WhenIamDestroying()
{
//*****************Here there is some Exception handeling in record time.***********************************

		breakWizardLoop=TRUE;
		KillTimer(2);
		recorder.Close(); 
		writer->Close();
		KillTimer(nIDEventTimer1);  
}

void CTabPageOne::OnBnClickedPeakmon2()
{
	if (blPeakLighten==FALSE)
	{
		blPeakLighten=TRUE;
		m_PeakmonLightBtn.SetWindowText("Hot Peak ON"); 
	}
	else
	{
		blPeakLighten=FALSE;
		m_PeakmonLightBtn.SetWindowText("Hot Peak OFF");
	}
}

void CTabPageOne::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Snd.nSetVolume("OUT_LINE",1,(MAXVOL-m_LineInRec.GetPos())*2);
	*pResult = 0;
}
	
void CTabPageOne::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Snd.nSetVolume("OUT_WAVE",1,(MAXVOL-m_OutMixRec.GetPos())*2);
	*pResult = 0;
}

void CTabPageOne::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Snd.nSetVolume("OUT_MICROPHONE",1,(MAXVOL-m_MicRec.GetPos())*2);
	*pResult = 0;
}

void CTabPageOne::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Snd.nSetVolume("Baboo",0,(MAXVOL-m_SpeakerVol.GetPos())*2);
	*pResult = 0;
}

void CTabPageOne::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Snd.nSetVolume("OUT_WAVE",2,(MAXVOL-m_WaveVol.GetPos())*2);
	*pResult = 0;
}

void CTabPageOne::OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Snd.nSetVolume("OUT_MICROPHONE",2,(MAXVOL-m_MicVol.GetPos())*2);
	*pResult = 0;
}

void CTabPageOne::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Snd.nSetVolume("OUT_LINE",2,(MAXVOL-m_LineInVol.GetPos())*2);
	*pResult = 0;
}

void CTabPageOne::OnBnClickedRadioline()
{
Snd.nSourceSelector("Line In"); 
}

void CTabPageOne::OnBnClickedRadiooutmix()
{
Snd.nSourceSelector("Wave Out Mix"); 
}

void CTabPageOne::OnBnClickedRadiomic()
{
Snd.nSourceSelector("Microphone"); 
}

void CTabPageOne::OnBnClickedChksp()
{
	BOOL bl;
	if(m_ChkSP.GetCheck()==BST_UNCHECKED)
	{
		bl=FALSE;
	}
	else
	{
		bl=TRUE;
	}
	Snd.nMuteUnMute("OUT_SPEAKERS",bl); 
}

void CTabPageOne::OnBnClickedChkwav()
{
	BOOL bl;
	if(m_ChkWav.GetCheck()==BST_UNCHECKED)
	{
		bl=FALSE;
	}
	else
	{
		bl=TRUE;
	}
	Snd.nMuteUnMute("OUT_WAVE",bl); 
}

void CTabPageOne::OnBnClickedChkmic()
{
	BOOL bl;
	if(m_ChkMic.GetCheck()==BST_UNCHECKED)
	{
		bl=FALSE;
	}
	else
	{
		bl=TRUE;
	}
	Snd.nMuteUnMute("OUT_MICROPHONE",bl); 
}

void CTabPageOne::OnBnClickedChkline()
{
	BOOL bl;
	if(m_ChkLine.GetCheck()==BST_UNCHECKED)
	{
		bl=FALSE;
	}
	else
	{
		bl=TRUE;
	}
	Snd.nMuteUnMute("OUT_LINE",bl); 
}

void CTabPageOne::OnBnClickedPeakmon3()
{
if (blPeakAreaSound==FALSE)
	{
		blPeakAreaSound=TRUE;
		m_chkAreaMon.SetWindowText("Area Monitor ON"); 
	}
	else
	{
		blPeakAreaSound=FALSE;
		m_chkAreaMon.SetWindowText("Area Monitor OFF");
	}
}

void CTabPageOne::OnDestroy()
{
	CDialog::OnDestroy();
	AppTools apptool;
	CString Tmpstr=App_Path +"Settings\\" +"Setting.ini";
	CString tmp1;
	tmp1.Format("%d",Number_Mic_Sources);
	apptool.SetSetting(Tmpstr,tmp1,"SourceNumber","Sampler");
	tmp1.Format("%d",m_SpinThreshold.GetPos32());
	apptool.SetSetting(Tmpstr,tmp1,"Threshold","Sampler");
	tmp1.Format("%d",m_SpinTrigThreshold.GetPos32());
	apptool.SetSetting(Tmpstr,tmp1,"TrigThreshold","Sampler");
}

void CTabPageOne::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	ListenPeakRangMin=m_SpinThreshold.GetPos();
}

void CTabPageOne::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	TrigThreshold=m_SpinTrigThreshold.GetPos();
}

void CTabPageOne::OnEnChangeEdit2()
{
	if(m_FirsttxtEntry1)
	{
		CString Bo;
		m_txtThreshold.GetWindowText(Bo);
		m_SpinThreshold.SetPos(atoi(Bo)); 
		ListenPeakRangMin=atoi(Bo);
	}
	m_FirsttxtEntry1=TRUE;
}

void CTabPageOne::OnEnChangeEdit3()
{
	if(m_FirsttxtEntry2)
	{
		CString Bo;
		m_txtTrigThreshold.GetWindowText(Bo);
		m_SpinTrigThreshold.SetPos(atoi(Bo)); 
		TrigThreshold=atoi(Bo);
	}
	m_FirsttxtEntry2=TRUE;
}