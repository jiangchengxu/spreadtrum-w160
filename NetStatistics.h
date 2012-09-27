#if !defined(AFX_NETSTATISTICS_H__DA9B3056_325E_4459_BFC1_21BE7679C2E6__INCLUDED_)
#define AFX_NETSTATISTICS_H__DA9B3056_325E_4459_BFC1_21BE7679C2E6__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetStatistics.h : header file
//

#include "MFTrafficButton.h"
#include "CommonStructs.h"
#include "Ds.h"

#include "TabedDialog.h"


#include "dlgconnect.h"
#include "NetLog.h"







/////////////////////////////////////////////////////////////////////////////
// CNetStatistics dialog

class CNetStatistics : public CTabedDialog
{
// Construction
public:
    DECLARE_DYNAMIC(CNetStatistics)
    CNetStatistics(CWnd* pParent = NULL);   // standard constructor
    ~CNetStatistics();
// Dialog Data
    //{{AFX_DATA(CNetStatistics)
    enum { IDD = IDD_DIALOG_NET_STATISTICS };
    CTreeCtrl	m_Tree;
    CImageList *m_pImageList;

    MFTrafficButton	m_cOutgoing;
    MFTrafficButton	m_cIncoming;
    MFTrafficButton	m_cTotalTraffic;
    CDlgConnect* pDlgConnect;
    HTREEITEM ht_transferTime,ht_uploadSpeed,ht_downloadSpeed,ht_downloadedData,ht_uploadedData;
    HTREEITEM ht_dailyDownload,ht_dailyUpload,ht_monthlyDownload,ht_monthlyUpload,ht_timeLastReset;
    HTREEITEM ht_yearlyDownload,ht_yearlyUpload;
    CString strTransferTime,strTransfer,strSpeed,strData,strDownload,strUpload,strStatistics;
    CString strDaily,strMonthly,strYearly,StrTransTime,strUploadedData,strDownloadedData;
    CString strTimeLastReset;


    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNetStatistics)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    // Generated message map functions
    //{{AFX_MSG(CNetStatistics)
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnRightClickTree(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonExpandAll();

    afx_msg void OnButtonNetLog();
    void ExpandBranch(HTREEITEM   hitem);
    afx_msg void OnButtonCollapseAll();
    void CollapseBranch(HTREEITEM   hitem);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:

    int ConnectionType;
    void setConnection(int connectType);
    void setDrawFlag(BOOL);
    afx_msg void OnButtonResetStatistics();
    afx_msg void OnResetStatistics(WPARAM wParam, LPARAM lParam);
private:


public:
    DWORD m_dCurrDownTraffic,m_dCurrUpTraffic;
    stThroughput m_stThroughput;
};



#endif
