#if !defined(AFX_HEADERCTRLEX_H__E12F3E10_F370_454F_979E_D22A0C5C1833__INCLUDED_)
#define AFX_HEADERCTRLEX_H__E12F3E10_F370_454F_979E_D22A0C5C1833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderCtrlEx.h : header file
//
//#define HDCTRL_BK_COLOR (RGB(238,241,243))
#define HDCTRL_BK_COLOR (RGB(233,230,213))//定义Tablist控件标题栏的颜色  modify by liubs
namespace ItemCompareFunc
{
BOOL IsDecNumber(const CString& str, double& f);
int DecNumberCompare(double f1, double f2);

BOOL IsDate(const CString& str, COleDateTime& date);
int DateCompare(const COleDateTime& date1, const COleDateTime& date2);
}
/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx window

class CHeaderCtrlEx : public CHeaderCtrl
{
public:
    BOOL  m_bHeader;
    int      m_nOffset;
    int SetSortImage( int nCol, BOOL bAsc );
    void DrawHeader();
public:
    int  m_nSortCol;
    BOOL m_bSortAsc;
// Construction
public:
    CHeaderCtrlEx();

// Attributes
public:

// Operations
public:
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHeaderCtrlEx)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CHeaderCtrlEx();

    // Generated message map functions
protected:
    //{{AFX_MSG(CHeaderCtrlEx)
    afx_msg void OnPaint();
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERCTRLEX_H__E12F3E10_F370_454F_979E_D22A0C5C1833__INCLUDED_)
