#if !defined(AFX_NUMEDIT_H__3259CD79_CD89_48F6_85C1_A7933E0B257B__INCLUDED_)
#define AFX_NUMEDIT_H__3259CD79_CD89_48F6_85C1_A7933E0B257B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNumEdit window

class CNumEdit : public CEdit
{
// Construction
public:
    CNumEdit();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNumEdit)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CNumEdit();

    // Generated message map functions
protected:
    //{{AFX_MSG(CNumEdit)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

public:
	bool m_bFlag; //true��ʾ���Ե绰��ģ��ĵ��ã�false��ʾ����ģ�飬Ĭ��Ϊfalse
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMEDIT_H__3259CD79_CD89_48F6_85C1_A7933E0B257B__INCLUDED_)
