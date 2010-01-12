// waveInFFTDlg.h : header file
//

#if !defined(AFX_WAVEINFFTDLG_H__B35B1921_6188_488A_9283_39FCE9E3DA70__INCLUDED_)
#define AFX_WAVEINFFTDLG_H__B35B1921_6188_488A_9283_39FCE9E3DA70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "SpectrumGraph.h"
#include "afxcmn.h"
#include "Recorder.h"
#include "ibtabctrl.h"
#include "TabPageOne.h"
#include "TabPageTwo.h"
#include "TabPageThree.h"
#include "afxwin.h"
#define FFT_LEN 1024
#define COPY_MEM_SIZE (FFT_LEN/2)*sizeof(double)

/////////////////////////////////////////////////////////////////////////////
// CWaveInFFTDlg dialog

class CWaveInFFTDlg : public CDialog
{
// Construction
public:
	CWaveInFFTDlg(CWnd* pParent = NULL);	// standard constructor
	CFrequencyGraph m_graph;
	void InitApp();
// Dialog Data
	//{{AFX_DATA(CWaveInFFTDlg)
	enum { IDD = IDD_WAVEINFFT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveInFFTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	Recorder m_rec;
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CWaveInFFTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
//	afx_msg HCURSOR OnQueryDragIcon();
  	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CTabPageOne *p1;
	CTabPageTwo *p2;
	CTabPageThree *p3;
	int m_bInit;
	double *LearnedSound;
public:
	CibTabCtrl m_ctrlTAB;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMove(int x, int y);
	CStatic m_FrmScope;
	CButton m_CheckPreview;
	afx_msg void OnBnClickedCancel();
	CStatic m_CourtPic;
	afx_msg void OnBnClickedOsccheck();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEINFFTDLG_H__B35B1921_6188_488A_9283_39FCE9E3DA70__INCLUDED_)
