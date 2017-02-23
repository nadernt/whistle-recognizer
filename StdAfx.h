// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__502FFB82_D86B_4BA6_833D_B8D4758B5BE9__INCLUDED_)
#define AFX_STDAFX_H__502FFB82_D86B_4BA6_833D_B8D4758B5BE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

extern CString App_Path;
extern double f_Buffer[1024/2];
extern double LearnedSynapse[1024/2];
extern double NurlStk[1024/2];
extern double RealTimeWeight[1024/2];
extern float  AvrgWeightPos,AvrgWeightNeg,AvrgWeightTotal;
extern double OldWeight[1024/2];
extern CString strTmp;
extern HWND mCourtWnd;
extern BOOL PreviewToggle;
extern BOOL firstTher;
extern BOOL SynapseWeightShow;
extern long nI;
extern int CUT_OFF_Freq;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__502FFB82_D86B_4BA6_833D_B8D4758B5BE9__INCLUDED_)
 