// SpectrumGraph.cpp : implementation file
//
#include "stdafx.h"
#include "waveInFFT.h"
#include "SpectrumGraph.h"
#include <math.h>
#include "memoryDc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
double *m_dArray;
/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraph

CFrequencyGraph::CFrequencyGraph()
{
	m_nLength = 0;
	m_dArray = NULL;
	m_nMinValue = 0;
	m_nMaxValue = 10;
	m_nHighLevel = 80;
	m_nMediumLevel = 60;
	m_wSteps = 10;
	m_clrLow = RGB(0,0,255);
	m_clrMedium = RGB(0,255,0);
	m_clrHigh = RGB(255,0,0);
	m_bGrid = TRUE;
	m_graphType = FG_OSCILLOSCOPE;
}

CFrequencyGraph::~CFrequencyGraph()
{
	if (m_dArray)
		delete []m_dArray;
}

BEGIN_MESSAGE_MAP(CFrequencyGraph, CWnd)
	//{{AFX_MSG_MAP(CFrequencyGraph)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraph message handlers
void CFrequencyGraph::Point(HDC hDC, int x, int y, COLORREF color)
{
	::SetPixel (hDC, x, y, color);
}
BOOL CFrequencyGraph::Line(HDC hDC, int x1, int y1, int x2, int y2)
{
	::MoveToEx(hDC, x1, y1, 0);
	return ::LineTo(hDC, x2, y2);
}
void CFrequencyGraph::OnPaint()
{
	CRect rct;
	GetClientRect(&rct);
	//Needs to use CMemoryDC or else CMemDC...
	//using either class will get ride of the 
	//flicker problem that otherwise presents itself...
	CPaintDC paintdc(this);
	CMemDC dc(&paintdc,&rct);

// graph
	if (m_nLength == 0)
		return;

	CBrush* pOldBrush = NULL;
	CBrush* pbbr = NULL;
	CBrush* pabr = NULL;
	
	//Back colore
	CBrush br(RGB(0,80,0));
	//Brush of line
	CPen pen(PS_SOLID,1,RGB(255,255,255));
	CPen bpen(PS_SOLID,1,RGB(0,255,180));

	CPen* pOldPen = NULL;
	CPen* pbOldPen = NULL;

	int i, x,y=0;
	int nBars = 36;
	int leftzerolevel = (rct.Height()/2-10)/2;
	int rightzerolevel = rct.Height()/2+10+leftzerolevel;
	int StereoY = rct.Height()/2;
	int StereoX = rct.Width()/2;

	double timescale = leftzerolevel/32768.0;
	double ts = static_cast<double>(rct.Width())/static_cast<double>(m_nLength);

	switch (m_graphType)
	{
	case FG_OSCILLOSCOPE:
		pOldBrush = dc.SelectObject(&br);
		pOldPen = dc.SelectObject(&pen);
		dc.Rectangle( 0, 0, rct.Width(), rct.Height() );
		pbOldPen = dc.SelectObject(&bpen);
		dc.MoveTo(0, leftzerolevel);
		for(i=0; i<m_nLength-1; i+=2)
		{
			x=i*ts;
			int y = leftzerolevel - static_cast<double>(m_dArray[i])*timescale;
			dc.LineTo( x ,y );
				/*if ((m_nLength/2)<i && i < m_nLength-5)
				{
					dc.SetDCPenColor(RGB(255,100,100));
					dc.SelectObject(&pen);
				}
				else
				{*/
					dc.SetDCPenColor(RGB(0,0,255)); 
				//}
			
		}
		
		if(firstTher==TRUE)
			dc.DrawText("Yes",rct,DT_SINGLELINE | DT_CENTER | DT_VCENTER); 
		else
			dc.DrawText("No",rct,DT_SINGLELINE | DT_CENTER | DT_VCENTER); 

		dc.MoveTo(0, rightzerolevel);
		for(i=1; i<m_nLength; i+=2)
		{
			x=i*ts;
			int y = rightzerolevel - static_cast<double>(m_dArray[i])*timescale;
			dc.LineTo( x ,y );
		}
		dc.SelectObject(pbOldPen);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
	break;
	}
	if (pOldPen)
		dc.SelectObject(pOldPen);
	if (pOldBrush)
		dc.SelectObject(pOldBrush);
}

void CFrequencyGraph::Update(int nNumber, double* dArray)
{
	if (m_nLength != nNumber)
	{
		m_nLength = nNumber;
		if (m_dArray)
			delete []m_dArray;
		m_dArray = new double[m_nLength];
	}
	memcpy(m_dArray,dArray,nNumber*sizeof(double));
	Invalidate(FALSE);
}
