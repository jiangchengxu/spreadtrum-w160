#if !defined(AFX_MSGTHREAD_H__48D14DB2_C296_41A6_A393_4F69C3A36A63__INCLUDED_)
#define AFX_MSGTHREAD_H__48D14DB2_C296_41A6_A393_4F69C3A36A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CMsgThread thread

class CMsgThread : public CWinThread
{
    DECLARE_DYNCREATE(CMsgThread)
protected:
    CMsgThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMsgThread)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual BOOL OnIdle(LONG lCount);
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CMsgThread();

    // Generated message map functions
    //{{AFX_MSG(CMsgThread)
        // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGTHREAD_H__48D14DB2_C296_41A6_A393_4F69C3A36A63__INCLUDED_)
