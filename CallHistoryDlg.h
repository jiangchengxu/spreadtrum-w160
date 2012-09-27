#if !defined(AFX_CALLHISTORYDLG_H__E1C04E2E_E7A2_4EA8_B78D_8D57C968B870__INCLUDED_)
#define AFX_CALLHISTORYDLG_H__E1C04E2E_E7A2_4EA8_B78D_8D57C968B870__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallHistoryDlg.h : header file

#include "TabCtrl\entabCtrl.h"
#include "HistListCtrl.h"
#include "HistData.h"
/////////////////////////////////////////////////////////////////////////////
// CCallHistoryDlg dialog
class CCallHistoryDlg : public CBaseDialog
{
private:
    CHistData *m_pHistData;
    CPbData   *m_pPbData;
    EnHistKind m_DispFilter;
    BOOL       m_bSortAscendArr[HISTKIND_MAX];               // Is sort ascending?
    EnHistCol  m_SortColArr[HISTKIND_MAX];                   // current sorted column
    WORD       m_DspIndexArr[HISTKIND_MAX][HIST_ITEM_MAX];  // Êý¾ÝË÷Òý
    void ResetDspIndexArr(EnHistKind type);
    void UpdateHistSortList(EnHistKind type);
    void BubbleSort(EnHistKind type, EnHistCol col);
    void ReverseSortIndexArr(EnHistKind type);
    void SortHistList(EnHistKind type, EnHistCol col);
    void InitDspIndexArr();
public:
    void InitListCtrl();
    void DispCallHistory();
    void InitTabCtrl();
    void UpdateList(void);
    CCallHistoryDlg(CWnd* pParent = NULL);
    virtual int DealwithResponseProc(LPARAM lParam, WPARAM wParam);
    CString m_strNumber;
    //{{AFX_DATA(CCallHistoryDlg)
    enum { IDD = IDD_DIALOG_CALL_HISTORY };
    CHistListCtrl m_lstHist;
    CEnTabCtrl    m_tabHist;
    //}}AFX_DATA
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCallHistoryDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
// Implementation
protected:
    CImageList m_ilTabs;
    CImageList m_ilLsts;
    // Generated message map functions
    //{{AFX_MSG(CCallHistoryDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeTabHist(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnColumnclickListHist(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonReply();
    afx_msg void OnButtonSendsms();
    afx_msg void OnButtonDelete();
    afx_msg void OnButtonAbstract();
    afx_msg void OnButtonDeleteall();
    //}}AFX_MSG
    afx_msg LRESULT OnMenuItemClick(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAbstractPb(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSaveHist(WPARAM wParam, LPARAM lParam = 0);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_CALLHISTORYDLG_H__E1C04E2E_E7A2_4EA8_B78D_8D57C968B870__INCLUDED_)
