#if !defined(AFX_SLIDERCTRLEX_H__1F6D53E0_80B5_45F2_AD66_C68D0CA1FA53__INCLUDED_)
#define AFX_SLIDERCTRLEX_H__1F6D53E0_80B5_45F2_AD66_C68D0CA1FA53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SliderCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlEx window
#include "Ds.h"
#include "SkinWin\EnBitmap.h"
class CSliderCtrlEx : public CSliderCtrl
{
// Construction
public:
    CSliderCtrlEx();
	void SetVolume(int nVol);
	static void AtRespVol(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);

// Attributes
public:
	BOOL m_bVolSet;//音量下位机交互使能
    int m_nVolValue;
    BOOL m_bDragging;
	CMyBitmap *m_bmpBack;
	CMyBitmap *m_bmpTic;
// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSliderCtrlEx)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CSliderCtrlEx();
	void SetSkin(UINT uiBackGround, UINT uiTickImage/*,short nCursor*/);
	void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);
    // Generated message map functions
protected:
    //{{AFX_MSG(CSliderCtrlEx)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	//afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERCTRLEX_H__1F6D53E0_80B5_45F2_AD66_C68D0CA1FA53__INCLUDED_)
