#include "afxwin.h"
#if !defined(AFX_TABPAGEONE_H__9190CC26_72A3_4552_BD0A_A721AAE4CB38__INCLUDED_)
#define AFX_TABPAGEONE_H__9190CC26_72A3_4552_BD0A_A721AAE4CB38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include ".\waveiolib\waverecorder.h"
#include ".\waveiolib\wavewriter.h"
#endif // _MSC_VER > 1000
#include "afxcmn.h"
#include "SndCardServices.h"
// TabPageOne.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTabPageOne dialog

class CTabPageOne : public CDialog
{
// Construction
public:
	CTabPageOne(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabPageOne();
	void InitApp();
// Dialog Data
	//{{AFX_DATA(CTabPageOne)
	enum { IDD = IDD_DLG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabPageOne)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabPageOne)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedNew();
	int nGetPeak();
	CProgressCtrl m_Peak;
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedPeakmon();
	CButton m_PeakmonBtn;
	CStatic m_TxtPeak;
	void LoadBitmapfromFile(HWND hWnd, CString sFilename);
	afx_msg void OnBnClickedSorcesel();
	void ChangeCtrlColore(HWND hWnd,COLORREF Crgb);
	void WaveRecorder();
	void NewWaveForRecord();
	void DiskWaveFileHandler(CString WaveNameFile,CString OldWavfile,int iState); //State 1: rename. 2: delete
	void CTabPageOne::DoEvents();
	void WhenIamDestroying();

private:
	CWaveRecorder recorder;
	CWaveWriter  * writer;
	BOOL m_paused;
	COLORREF Ctrlrgb;
	BOOL IsMaxPeak;
	BOOL RecordCommand;
	BOOL IsAtRecord;
	BOOL breakWizardLoop;
	BOOL IsNormalEndWizard;
	CString Wavfilename;
	CString PicFileName;
	int Number_Mic_Sources;
	BOOL blPeakLighten;
	UINT nIDEventTimer1;
	BOOL mToggle;
	SndCard Snd;
	int ListenPeakRangMin;
	BOOL blPeakAreaSound;
public:
	int TrigThreshold;
	afx_msg void OnBnClickedSourcesel();
	afx_msg void OnPaint();
	CStatic m_HotPoint;
	CStatic m_TxtLogs;
	CButton m_NewBtn;
	CButton m_SelSourcBtn;
	CButton m_PeakmonLightBtn;
	afx_msg void OnBnClickedPeakmon2();
	CProgressCtrl m_EnvPeak;
	CSliderCtrl m_LineInRec;
	CSliderCtrl m_OutMixRec;
	CSliderCtrl m_MicRec;
	CSliderCtrl m_SpeakerVol;
	CSliderCtrl m_WaveVol;
	CSliderCtrl m_MicVol;
	CSliderCtrl m_LineInVol;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioline();
	afx_msg void OnBnClickedRadiooutmix();
	afx_msg void OnBnClickedRadiomic();
	afx_msg void OnBnClickedChksp();
	CButton m_ChkSP;
	CButton m_ChkWav;
	CButton m_ChkMic;
	CButton m_ChkLine;
	afx_msg void OnBnClickedChkwav();
	afx_msg void OnBnClickedChkmic();
	afx_msg void OnBnClickedChkline();
	CButton m_chkAreaMon;
	CEdit m_txtThreshold;
	CSpinButtonCtrl m_SpinThreshold;
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedPeakmon3();
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeEdit2();
	BOOL m_FirsttxtEntry1;
	BOOL m_FirsttxtEntry2;
	CEdit m_txtTrigThreshold;
	CSpinButtonCtrl m_SpinTrigThreshold;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	//CEdit m_t1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPAGEONE_H__9190CC26_72A3_4552_BD0A_A721AAE4CB38__INCLUDED_)
