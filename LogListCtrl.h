#ifndef _AFX_LOGLISTCTRL_H_
#define _AFX_LOGLISTCTRL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistListCtrl.h : header file
//
//#include "MyListCtrl.h"
#include "HeaderCtrlEx.h"
#define    RC_ITEM_SELECTED    0x0002 // Selected items
#define    RC_ITEM_UNSELECTED    0x0004 // Unselected items
/////////////////////////////////////////////////////////////////////////////
// CLogListCtrl window

class CLogListCtrl : public CListCtrl
{
public:
    void AdjustColumnWidth();
    int  GetColumnCount();
    BOOL SwapItems(int nItem1, int nItem2);    // Swap two items in the list, including all attributes.
    void SortItems(int nColumn, BOOL bAscending); // Sort a specified column.
    int m_nSortCol; // The sorted column, -1 if none
    BOOL m_bSortAscending; // Is sort ascending?
    LPTSTR m_pszSeparator; // Sort separator
    CHeaderCtrlEx m_headerCtrl;
    CImageList  m_imageList;
    DWORD GetItemStates(int nItem) const;
    BOOL SetItemStates(int nItem, DWORD dwNewStates);
protected:
    BOOL IsValidIndex(int nIndex) const;
    void ResetSortCol(void);
    int CompareItems(int nItem1, int nItem2);
    int FindSeparator(int nStartAfter, int nColumn) const;
    BOOL PartialSort(int nStart, int nEnd);
    void QuickSortRecursive(int* pArr, int nLow, int nHigh);

// Construction
public:
    CLogListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLogListCtrl)
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CLogListCtrl();

    // Generated message map functions
protected:
    //{{AFX_MSG(CLogListCtrl)
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    afx_msg void OnMenuClicked(UINT nID);
    
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
