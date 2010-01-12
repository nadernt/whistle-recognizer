// waveInFFTDlg.cpp : implementation file
//
#include "stdafx.h"
#include "waveInFFT.h"
#include "waveInFFTDlg.h"
#include "fourier.h"
#include "Listener.h"
#include <math.h>
#include ".\waveinfftdlg.h"
#include "AppTools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL PreviewToggle;
CString App_Path;
HWND mCourtWnd;
double f_Buffer[1024/2];
double LearnedSynapse[1024/2];
double NurlStk[1024/2];
double RealTimeWeight[1024/2];
double OldWeight[1024/2];
int CUT_OFF_Freq=0;
float  AvrgWeightPos=0,AvrgWeightNeg=0,AvrgWeightTotal=0;
BOOL MessageOnOffPrev=FALSE;
long nI=0;
BOOL firstTher=FALSE;
BOOL SynapseWeightShow=FALSE;
CString strTmp;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
//	ON_WM_UPDATEUISTATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveInFFTDlg dialog

CWaveInFFTDlg::CWaveInFFTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveInFFTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaveInFFTDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWaveInFFTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveInFFTDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB2, m_ctrlTAB);
	DDX_Control(pDX, IDC_FrmScope, m_FrmScope);
	DDX_Control(pDX, IDC_OSCCheck, m_CheckPreview);
	DDX_Control(pDX, IDC_PicShowBox, m_CourtPic);
}

BEGIN_MESSAGE_MAP(CWaveInFFTDlg, CDialog)
	//{{AFX_MSG_MAP(CWaveInFFTDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_OSCCheck, OnBnClickedOsccheck)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

inline double GetFrequencyIntensity(double re, double im)
{
	return sqrt((re*re)+(im*im));
}

BOOL Process(void* lpData, LPWAVEHDR pwh)
{
double fin[FFT_LEN],fout[FFT_LEN],foutimg[FFT_LEN],fdraw[FFT_LEN/2];
	for (DWORD dw = 0; dw < FFT_LEN; dw++)
	{
		{
			fin[dw] = (double)((short*)pwh->lpData)[dw];
		}
	}
	fft_double(FFT_LEN,0,fin,NULL,fout,foutimg);
	float re,im;
	for(int i=0;i<FFT_LEN/2;i++)
	{
		re = fout[i];
		im = foutimg[i];
		fdraw[i]=(GetFrequencyIntensity(re,im))/256;
		if(fdraw[i]>RealTimeWeight[i])
				{
						RealTimeWeight[i]=fdraw[i];
				}

	}
	
	memcpy(f_Buffer,fdraw,COPY_MEM_SIZE);
	memcpy(NurlStk,fdraw,COPY_MEM_SIZE);
		
		Listener T;
		if (T.WhistleListener(250,CUT_OFF_Freq,0)==TRUE)
		{
			nI++;
			firstTher=TRUE;
		}
	
	if (PreviewToggle == TRUE)
	{
		CFrequencyGraph* pPeak = (CFrequencyGraph*)lpData;
		if (::IsWindow(pPeak->GetSafeHwnd()))
		{
			if(SynapseWeightShow==FALSE)
				pPeak->Update(FFT_LEN/2,fdraw);
			else
				pPeak->Update(FFT_LEN/2,RealTimeWeight);
		}	
	}

		return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWaveInFFTDlg message handlers
BOOL CWaveInFFTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	InitApp();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWaveInFFTDlg::InitApp()
{
	mCourtWnd=m_CourtPic.m_hWnd; 
	AppTools TiTi;
	App_Path=TiTi.GetAppPath(); 
	CRect rct;
	m_FrmScope.GetClientRect(rct);
	CWnd* Cwin = GetDlgItem(IDC_FrmScope);
	m_graph.Create("STATIC","",WS_VISIBLE|WS_CHILD,rct,Cwin,1001);
	m_rec.Open();
	m_rec.SetBufferFunction((void*)&m_graph,Process);
	m_rec.Start();

	m_CheckPreview.SetCheck(TRUE); 
	PreviewToggle=TRUE;

	p1 = new CTabPageOne();
	p1->Create(IDD_DLG1,m_ctrlTAB.GetWindow(IDD_DLG1));
	p2 = new CTabPageTwo();
	p2->Create(IDD_DLG2,m_ctrlTAB.GetWindow(IDD_DLG2));
	p3 = new CTabPageThree();
	p3->Create(IDD_DLG3,m_ctrlTAB.GetWindow(IDD_DLG3));

	m_ctrlTAB.AddTabPane("Sampler",p1);
	m_ctrlTAB.AddTabPane("Processor",p2);
	m_ctrlTAB.AddTabPane("Listener",p3);
	p1->InitApp(); 
	for(int i=0;i<FFT_LEN/2;i++)
			RealTimeWeight[i]=0;
	
	
}

void CWaveInFFTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWaveInFFTDlg::OnPaint() 
{

	p1->OnPaint(); 
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CWaveInFFTDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_ctrlTAB.SetDefaultPane(0);
}

void CWaveInFFTDlg::OnMove(int x, int y)
{
	m_ctrlTAB.OnMove(x,y); 
}

void CWaveInFFTDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void CWaveInFFTDlg::OnBnClickedOsccheck()
{
	
	//SetTimer(1, 1000, 0);
}

void CWaveInFFTDlg::OnTimer(UINT nIDEvent)
{
	KillTimer(1);
	firstTher=FALSE;	
	CString T;
	T.Format("%d",nI );
	AfxMessageBox(T);
	nI=0;
	CDialog::OnTimer(nIDEvent);
}

void CWaveInFFTDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CWaveInFFTDlg::OnBnClickedButton1()
{
	sndPlaySound("d:\\FinalSample.wav",SND_ASYNC);
}
