// StepsParameter.cpp : implementation file
//

#include "stdafx.h"
#include "waveInFFT.h"
#include "StepsParameter.h"
#include ".\stepsparameter.h"


// StepsParameter dialog

IMPLEMENT_DYNAMIC(StepsParameter, CDialog)
StepsParameter::StepsParameter(CWnd* pParent /*=NULL*/)
	: CDialog(StepsParameter::IDD, pParent)
	, NumberOfSources(0)
{
}

StepsParameter::~StepsParameter()
{
}

BOOL StepsParameter::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Spiner.SetRange(1,5); 
	m_Spiner.SetPos(3); 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void StepsParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Text);
	DDX_Control(pDX, IDC_SPIN1, m_Spiner);
}


BEGIN_MESSAGE_MAP(StepsParameter, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void StepsParameter::OnBnClickedOk()
{
	NumberOfSources=m_Spiner.GetPos();
	OnOK();
}
