// TabPageThree.cpp : implementation file
//

#include "stdafx.h"
#include "waveInFFT.h"
#include "TabPageThree.h"
#include ".\tabpagethree.h"

// CTabPageThree dialog

IMPLEMENT_DYNAMIC(CTabPageThree, CDialog)
CTabPageThree::CTabPageThree(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPageThree::IDD, pParent)
{
}

CTabPageThree::~CTabPageThree()
{
}

void CTabPageThree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabPageThree, CDialog)
//ON_BN_CLICKED(IDC_Prew, OnBnClickedPrew)
END_MESSAGE_MAP()


// CTabPageThree message handlers

BOOL CTabPageThree::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
          {
              if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
                  pMsg->wParam=NULL ;
          }

	return CDialog::PreTranslateMessage(pMsg);
}

//void CTabPageThree::OnBnClickedPrew()
//{
//	CWaveInFFTDlg A;
//	A.SetPreviewON_OFF();
//}
