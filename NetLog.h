#if !defined(AFX_NETLOG_H__9D00E1E6_4457_49F3_8588_EDF4BEF4C887__INCLUDED_)
#define AFX_NETLOG_H__9D00E1E6_4457_49F3_8588_EDF4BEF4C887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetLog.h : header file
//

#include "LogListCtrl.h"
#include "LogData.h"
#include "ExDataExch.h"

/////////////////////////////////////////////////////////////////////////////
// CNetLog dialog

class CNetLog : public CBaseDialog
{
private:
    CLogData *m_pLogData;
// Interface
public:
    void ReadNetLog();
    void InitListCtrl();
    void DispNetLog();
    void ExportARecord(IGenerator* phbHandle,int iIndex);
    //void UpdateList(void);
// Construction
public:
    CNetLog(CWnd* pParent = NULL);   // standard constructor
    virtual int DealwithResponseProc(LPARAM lParam, WPARAM wParam);
// Dialog Data
    //{{AFX_DATA(CNetLog)
    enum { IDD = IDD_DIALOG_CALL_LOG };
    CLogListCtrl	m_lstCallLog;
    CString    m_strDataOut;
    CString    m_strDataIn;
    CString    m_strDataInOut;
    CString    m_strDuration;
    CString    m_strGPRSDataINOut;
    CString    m_strGPRSDuration;
    CString    m_strUMTSDataInOut;
    CString    m_strUMTSDuration;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNetLog)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CNetLog)
    afx_msg void OnButtonDelete();
    afx_msg void OnButtonDelall();
    virtual BOOL OnInitDialog();
    afx_msg void OnButtonExport();
    //}}AFX_MSG
    afx_msg LRESULT OnMenuItemClick(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETLOG_H__9D00E1E6_4457_49F3_8588_EDF4BEF4C887__INCLUDED_)
