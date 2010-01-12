#pragma once


// CTabPageThree dialog

class CTabPageThree : public CDialog
{
	DECLARE_DYNAMIC(CTabPageThree)

public:
	CTabPageThree(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabPageThree();

// Dialog Data
	enum { IDD = IDD_DLG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
