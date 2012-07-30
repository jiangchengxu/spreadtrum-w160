#if !defined(AFX_PREMSG_H__2C79EC49_78C0_4FF0_933E_38E009022403__INCLUDED_)
#define AFX_PREMSG_H__2C79EC49_78C0_4FF0_933E_38E009022403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PreMsg dialog

class PreMsg : public CDialog
{
public:
    BOOL SetBitmap(UINT nBitmapID,short red=-1,short green=-1,short blue=-1);
    BOOL SetBitmap(LPCTSTR szFileName,short red=-1,short green=-1,short blue=-1);
    void SetText(LPCTSTR szText);
    void FormatRgn(LPCTSTR szFileName);
protected:
    HRGN    m_hRegion;
    HRGN    CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color);
    void    DrawWindow(CDC *pDC);
    CBitmap m_bitmap;
    int m_nBitmapWidth;
    int m_nBitmapHeight;
    int m_nxPos;
    int m_nyPos;
    CRect m_rcText;
    CString m_strText;
    COLORREF m_crTextColor;

// Construction
public:
    PreMsg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(PreMsg)
    enum { IDD = IDD_DIALOG_PRE_TIP };
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(PreMsg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(PreMsg)
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREMSG_H__2C79EC49_78C0_4FF0_933E_38E009022403__INCLUDED_)
