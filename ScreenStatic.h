#if !defined(AFX_SCREENSTATIC_H__58ADC609_FF29_455E_A076_740B18170EEF__INCLUDED_)
#define AFX_SCREENSTATIC_H__58ADC609_FF29_455E_A076_740B18170EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreenStatic window

class CScreenStatic : public CStatic
{
public:
	BOOL		m_bFillBk;		//ÊÇ·ñÒªÌî³ä±³¾°É«
    UINT        m_nBmpID;		//±³¾°Í¼Æ¬ID
    CBitmap     m_bmp;
    int         m_nWidth;
    int         m_nHeight;
    BITMAP      m_bmInfo;
    CString        m_strTitle;
    CString        m_strDate;
    CString        m_strText;
    CString        m_strTick;
    BOOL SetBitmap(UINT nIDResource);
    void SetText(LPCTSTR szText);
	    void SetTextA(LPCSTR szText);
protected:
// Construction
public:
    CScreenStatic();

// Attributes
public:
protected:


// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScreenStatic)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CScreenStatic();

    // Generated message map functions
protected:
    //{{AFX_MSG(CScreenStatic)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    
    //}}AFX_MSG
    
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENSTATIC_H__58ADC609_FF29_455E_A076_740B18170EEF__INCLUDED_)
