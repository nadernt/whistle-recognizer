#if !defined(AFX_TABPAGETWO_H__D3085D68_F76A_47D8_A774_89A823ABBD77__INCLUDED_)
#define AFX_TABPAGETWO_H__D3085D68_F76A_47D8_A774_89A823ABBD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabPageTwo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabPageTwo dialog
#include "afxwin.h"
#include "apptools.h"
#include "afxcmn.h"
class CTabPageTwo : public CDialog
{
// Construction
public:
	CTabPageTwo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabPageTwo)
	enum { IDD = IDD_DLG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabPageTwo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabPageTwo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	static const long SAMPLE_RATE=48000*2;
	BOOL m_paused;
	void LearneFinalFileMixer(const char *InputFileName1,const char *InputFileName2,
						      const char *InputFileName3,const char *OutputResultFile);
	void DieWavSpaces(const char *FileName);
	void ChangeCtrlColore(HWND hWnd,COLORREF Crgb);
	AppTools Aptool;
	int HowMuch;
	int Tm1;
	int Tm2;
	int Tm3;
	bool ISLongCommanded;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedWavcat();
	CButton m_ChkPLSync;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPlaysamplbtn();
	CButton m_chkFinalSampPL;
	afx_msg void OnBnClickedOsccheck();
	CButton m_CheckPreview;
	afx_msg void OnBnClickedbtnlearnlisten();
	CEdit m_txtCutOff;
	CSpinButtonCtrl m_SpinCutOff;
	afx_msg void OnDeltaposSpincutoff(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSynashow();
	CButton m_SynShow;
	afx_msg void OnTimer(UINT nIDEvent);
	CEdit m_txtLogs;
	afx_msg void OnNcDestroy();
	CStatic m_LblShort;
	CStatic m_LblLong;
	afx_msg void OnBnClickedbtnlearnlisten2();
	afx_msg void OnBnClickedOsccheck3();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPAGETWO_H__D3085D68_F76A_47D8_A774_89A823ABBD77__INCLUDED_)
