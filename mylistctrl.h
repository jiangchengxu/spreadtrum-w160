#if !defined(AFX_MYLISTCTRL_H__C1AC212D_8ABF_11D1_B145_000000000000__INCLUDED_)
#define AFX_MYLISTCTRL_H__C1AC212D_8ABF_11D1_B145_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl.h : header file
//
#include "HeaderCtrlEx.h"

#define    RC_ITEM_SELECTED    0x0002 // Selected items
#define    RC_ITEM_UNSELECTED    0x0004 // Unselected items

class CMyListCtrl;

struct SortInfo {
    int iColumn;
    CMyListCtrl *pList;
};

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl window

typedef void (CALLBACK * LPFNLCOP)(NMHDR* pNMHDR, LRESULT* pResult, LPARAM lParam);
typedef void (CALLBACK * LPFNLCSOP)(NMHDR* pNMHDR, LRESULT* pResult, LPARAM lParam, SortInfo *pSortInfo); // ListControl Sort OPeration

class CMyListCtrl : public CListCtrl
{
// Construction
public:
    CMyListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyListCtrl)
protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

// Implementation
public:
    void AdjustColumnWidth();
    void SetSort(PFNLVCOMPARE pfnSort, LPARAM lParam);
    void SetColClick(LPFNLCSOP pfnLCOpHandler, LPARAM lParam);
    //void SortItems(int nColumn, BOOL bAscending);

    int GetColumnCount();
    virtual ~CMyListCtrl();
    void SetWidths();
    int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
                     int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
    int GetCurrentIndex();
    int InsertItem(int nItem, LPCTSTR lpszItem, LPARAM lItemData,int nImage = -1);
    int CompareItems(int nItem1, int nItem2);
    int FindSeparator(int nStartAfter, int nColumn) const;
    BOOL PartialSort(int nStart, int nEnd);
    void QuickSortRecursive(int* pArr, int nLow, int nHigh);

    DWORD GetItemStates(int nItem) const;
    BOOL SetItemStates(int nItem, DWORD dwNewStates);

    int m_nSortCol; // The sorted column, -1 if none
    BOOL m_bSortAscending; // Is sort ascending?
    LPTSTR m_pszSeparator; // Sort separator
    CHeaderCtrlEx m_headerCtrl;
    CImageList  m_imageList;

    // Generated message map functions
protected:
    LPFNLCOP m_pfnLCClick;
    LPARAM m_lParamClick;
    LPFNLCSOP m_pfnLCColClick;
    LPARAM m_lParamColClick;
    LPFNLCOP m_pfnLCDblClick;
    LPARAM m_lParamDblClick;
    PFNLVCOMPARE m_pfnSort;
    LPARAM m_lParamSort;

    int m_cCols;
    int m_iIndexCurrent;

    //{{AFX_MSG(CMyListCtrl)
//    afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
//    afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
//    afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__C1AC212D_8ABF_11D1_B145_000000000000__INCLUDED_)
