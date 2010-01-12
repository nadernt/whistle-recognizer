#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// StepsParameter dialog

class StepsParameter : public CDialog
{
	DECLARE_DYNAMIC(StepsParameter)

public:
	StepsParameter(CWnd* pParent = NULL);   // standard constructor
    virtual ~StepsParameter();

// Dialog Data
	enum { IDD = IDD_DlgSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int NumberOfSources;
	CEdit m_Text;
	CSpinButtonCtrl m_Spiner;
	afx_msg void OnBnClickedOk();

};
