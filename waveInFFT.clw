; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTabPageTwo
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "waveinfft.h"
LastPage=0

ClassCount=7
Class1=CFrequencyGraph
Class2=CWaveInFFTApp
Class3=CAboutDlg
Class4=CWaveInFFTDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDD_WAVEINFFT_DIALOG
Resource3=IDD_DLGREC
Class5=CMyTabs
Class6=CTabPageOne
Class7=CTabPageTwo
Resource4=IDD_DLGEXTRA

[CLS:CFrequencyGraph]
Type=0
BaseClass=CWnd
HeaderFile=SpectrumGraph.h
ImplementationFile=SpectrumGraph.cpp

[CLS:CWaveInFFTApp]
Type=0
BaseClass=CWinApp
HeaderFile=waveInFFT.h
ImplementationFile=waveInFFT.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=waveInFFTDlg.cpp
ImplementationFile=waveInFFTDlg.cpp
LastObject=CAboutDlg

[CLS:CWaveInFFTDlg]
Type=0
BaseClass=CDialog
HeaderFile=waveInFFTDlg.h
ImplementationFile=waveInFFTDlg.cpp
LastObject=IDC_TAB2

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_WAVEINFFT_DIALOG]
Type=1
Class=CWaveInFFTDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_OSCILLOSCOPE,button,1342181385
Control4=IDC_TAB2,SysTabControl32,1342177280

[DLG:IDD_DLGREC]
Type=1
Class=CTabPageOne
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DLGEXTRA]
Type=1
Class=CTabPageTwo
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CMyTabs]
Type=0
HeaderFile=MyTabs.h
ImplementationFile=MyTabs.cpp
BaseClass=CTabCtrl
Filter=W

[CLS:CTabPageOne]
Type=0
HeaderFile=TabPageOne.h
ImplementationFile=TabPageOne.cpp
BaseClass=CDialog
Filter=D

[CLS:CTabPageTwo]
Type=0
HeaderFile=TabPageTwo.h
ImplementationFile=TabPageTwo.cpp
BaseClass=CDialog
Filter=D

