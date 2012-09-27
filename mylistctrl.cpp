// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int  PIXEL_PAD = 20;

/////////////////////////////////////////////////////////////////////////////

CMyListCtrl::CMyListCtrl()
{
    m_cCols = 0;
    m_pfnSort = NULL;

    m_pfnLCClick = NULL;
    m_pfnLCColClick = NULL;
    m_pfnLCDblClick = NULL;

    m_pszSeparator = NULL;
    m_nSortCol = -1;
    m_bSortAscending = TRUE;
}

CMyListCtrl::~CMyListCtrl()
{
}
void CMyListCtrl::AdjustColumnWidth()
{
    SetRedraw(FALSE);
    int nColumnCount = GetColumnCount();

    for (int i = 0; i < nColumnCount; i++) {
        SetColumnWidth(i, LVSCW_AUTOSIZE);
        int nColumnWidth = GetColumnWidth(i);
        SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
        int nHeaderWidth = GetColumnWidth(i);
        SetColumnWidth(i, 20+max(nColumnWidth, nHeaderWidth));
    }
    SetRedraw(TRUE);
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
    //{{AFX_MSG_MAP(CMyListCtrl)
    //Modified by lgz 2006-06-21
//    ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
//    ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
//    ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
    ON_WM_CHAR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers


int CMyListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CListCtrl::OnCreate(lpCreateStruct) == -1)
        return -1;
    if (!m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd()))
        return -1;
    return 0;
}
/*
void CMyListCtrl::SortItems(int nColumn, BOOL bAscending)
{
    if (nColumn < 0 || nColumn >= GetColumnCount())
        return;

    // do the sorting
    m_nSortCol = nColumn;
    m_bSortAscending = bAscending;

    BOOL bEnd = FALSE;
    int nSep1 = -1;
    int nSep2 = FindSeparator(-1, nColumn);
    do
    {
        if (nSep2 < 0)
        {
            nSep2 = GetItemCount();
            bEnd = TRUE;
        }
        PartialSort(nSep1 + 1, nSep2 - 1);
        nSep1 = FindSeparator(nSep2 - 1, nColumn);
        nSep2 = FindSeparator(nSep1, nColumn);
    } while (!bEnd && nSep1 >= 0);

}

void CMyListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
    struct SortInfo si;

    si.iColumn = pNMListView->iSubItem;
    si.pList = this;

    *pResult = 0;
    if (m_pfnLCColClick)
    {
        m_pfnLCColClick(pNMHDR, pResult, m_lParamColClick, &si);
        AfxMessageBox("ok");
    }
    if (m_pfnSort && pNMListView)
    {
        SetSort(m_pfnSort, (LPARAM)&si);
        AfxMessageBox("ok!");
    }
}
*/

/*
void CMyListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    const int COL = pNMListView->iSubItem;
    SortItems(COL, COL == m_nSortCol ? !m_bSortAscending : TRUE);

    m_headerCtrl.SetSortImage(COL,COL == m_nSortCol ? !m_bSortAscending : TRUE);
    //m_headerCtrl.SetSortImage(0,TRUE);
    *pResult = 0;
}
*/

/*
void CMyListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;
    if (m_pfnLCClick)
    {
        m_pfnLCClick(pNMHDR, pResult, m_lParamClick);
    }
}
*/
int CMyListCtrl::InsertItem(int nItem, LPCTSTR lpszItem, LPARAM lItemData,int nImage)
{
    int iRes;
    if(-1 == nImage) {
        iRes = CListCtrl::InsertItem(nItem, lpszItem);
    } else {
        iRes = CListCtrl::InsertItem(nItem, lpszItem,nImage);
    }
    SetItemData(nItem, lItemData);

    return(iRes);
}



int CMyListCtrl::GetCurrentIndex()
{
    // Get the current mouse location and convert it to client
    // coordinates.
    DWORD pos = GetMessagePos();
    CPoint pt(LOWORD(pos), HIWORD(pos));
    ScreenToClient(&pt);

    // Get indexes of the first and last visible items in listview
    // control.
    int index = GetTopIndex();
    int last_visible_index = index + GetCountPerPage();
    if (last_visible_index > GetItemCount())
        last_visible_index = GetItemCount();

    // Loop until number visible items has been reached.
    while (index <= last_visible_index) {
        // Get the bounding rectangle of an item. If the mouse
        // location is within the bounding rectangle of the item,
        // you know you have found the item that was being clicked.
        CRect r;
        GetItemRect(index, &r, LVIR_BOUNDS);
        if (r.PtInRect(pt)) {
            UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
            SetItemState(index, flag, flag);
            break;
        }

        // Get the next item in listview control.
        index++;
    }
    return(index);
}


void CMyListCtrl::SetWidths()
{
    int cCols = GetColumnCount();
    int *pWidths = new int[cCols];
    int i, j, iWidth;
    CString strItem;

    for (j = cCols-1; j >= 0; j--) {
        pWidths[j] = 0;
    }
    if (pWidths) {
        i = GetItemCount()-1;
        for (; i >= 0; i--) {
            for (j = cCols-1; j >= 0; j--) {
                GetItemText(i, j, strItem.GetBuffer(_MAX_PATH), _MAX_PATH);
                strItem.ReleaseBuffer();
                iWidth = GetStringWidth(strItem) + PIXEL_PAD;
                if (pWidths[j] < iWidth) {
                    pWidths[j] = iWidth;
                }
            }
        }
        for (j = cCols-1; j >= 0; j--) {
            iWidth = GetColumnWidth(j);
            if (iWidth <= pWidths[j]) {
                iWidth = pWidths[j];
            }
            SetColumnWidth(j, iWidth);
        }
        delete[] pWidths;
    }
}

int CMyListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
                              int nFormat /*= LVCFMT_LEFT*/, int nWidth /*= -1*/, int nSubItem /* = -1*/)
{
    m_cCols++;
    int iWidth, iRes;

    iWidth = GetStringWidth(lpszColumnHeading) + PIXEL_PAD;
    if (nWidth != -1)
        iWidth = nWidth;
    iRes = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, iWidth, nSubItem);
    SetColumnWidth(nCol, iWidth);

    return(iRes);
}

int CMyListCtrl::GetColumnCount()
{
    return(m_cCols);
}
/*
void CMyListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;
    if (m_pfnLCDblClick)
    {
        m_pfnLCDblClick(pNMHDR, pResult, m_lParamDblClick);
    }
}
*/
void CMyListCtrl::SetSort(PFNLVCOMPARE pfnSort, LPARAM lParam)
{
    m_pfnSort = pfnSort;
    m_lParamSort = lParam;
}

void CMyListCtrl::SetColClick(LPFNLCSOP pfnLCOpHandler, LPARAM lParam)
{
    m_pfnLCColClick = pfnLCOpHandler;
    m_lParamColClick = lParam;
}


void CMyListCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
//    if (nChar == Del)
//    {
//    }
    CListCtrl::OnChar(nChar, nRepCnt, nFlags);
}

void CMyListCtrl::QuickSortRecursive(int* pArr, int nLow, int nHigh)
{
    int i = nHigh, j = nLow;
    int n = pArr[(nLow + nHigh) / 2];
    do {
        if (m_bSortAscending) {
            while (CompareItems(pArr[j], n) < 0)
                j++;

            while (CompareItems(pArr[i], n) > 0)
                i--;
        } else {
            while (CompareItems(pArr[j], n) > 0)
                j++;

            while (CompareItems(pArr[i], n) < 0)
                i--;
        }

        if ( i >= j ) {
            if ( i != j ) {
                int nTemp = pArr[i];
                pArr[i] = pArr[j];
                pArr[j] = nTemp;
            }

            i--;
            j++;
        }

    } while (j <= i);

    if (nLow < i)
        QuickSortRecursive(pArr,nLow,i);

    if (j < nHigh)
        QuickSortRecursive(pArr,j,nHigh);
}

int CMyListCtrl::FindSeparator(int nStartAfter, int nColumn) const
{
    if (m_pszSeparator == NULL)
        return -1;

    const int items = GetItemCount();
    for (int i = nStartAfter + 1; i < items; i++) {
        if (GetItemText(i, nColumn) == m_pszSeparator)
            return i;
    }
    return -1;
}

int CMyListCtrl::CompareItems(int nItem1, int nItem2)
{
    const CString s1 = GetItemText(nItem1, m_nSortCol);
    const CString s2 = GetItemText(nItem2, m_nSortCol);
    /*
    double f1, f2;
    if(ItemCompareFunc::IsDecNumber(s1, f1) && ItemCompareFunc::IsDecNumber(s2, f2))
        return ItemCompareFunc::DecNumberCompare(f1, f2);
    */
    return s1.Compare(s2);
}

BOOL CMyListCtrl::PartialSort(int nStart, int nEnd)
{
    const int COUNT = nEnd - nStart + 1;
    int i = 0;
    int n;
    int* aIndices = new int[COUNT];
    for (i = 0; i < COUNT; i++)
        aIndices[i] = nStart + i;

    QuickSortRecursive(aIndices, 0, COUNT - 1);

    // rearrange items
    const int COLS = GetColumnCount();
    ROWINFO* aRows = new ROWINFO[COUNT];
    for (i = 0; i < COUNT; i++) {
        n = aIndices[i];
        aRows[i].dwStates = GetItemStates(aIndices[i]);
        aRows[i].dwData = CListCtrl::GetItemData(aIndices[i]);
        aRows[i].aTexts.SetSize(COLS);
        for (int j = 0; j < COLS; j++) {
            //aRows[i].aImages[j] = GetItemImage(aIndices[i], j);
            aRows[i].aTexts[j] = GetItemText(aIndices[i], j);
        }
    }

    for (i = 0; i < COUNT; i++) {
        SetItemStates(nStart + i, aRows[i].dwStates);
        CListCtrl::SetItemData(nStart + i, aRows[i].dwData);
        for (int j = 0; j < COLS; j++) {
            //    SetItemImage(nStart + i, j, aRows[i].aImages[j]);
            CListCtrl::SetItemText(nStart + i, j, aRows[i].aTexts[j]);
        }
    }
    delete [] aRows;
    delete [] aIndices;
    return TRUE;
}

DWORD CMyListCtrl::GetItemStates(int nItem) const
{
    DWORD dwStates = 0;

    if (CListCtrl::GetItemState(nItem, LVIS_SELECTED))
        dwStates |= RC_ITEM_SELECTED;
    else
        dwStates |= RC_ITEM_UNSELECTED;

    return dwStates;
}

BOOL CMyListCtrl::SetItemStates(int nItem, DWORD dwNewStates)
{
    BOOL bResult;
    if (dwNewStates & RC_ITEM_SELECTED)
        bResult &= CListCtrl::SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);

    if (dwNewStates & RC_ITEM_UNSELECTED)
        bResult &= CListCtrl::SetItemState(nItem, 0, LVIS_SELECTED);

    return bResult;
}

void CMyListCtrl::PreSubclassWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd());
    CListCtrl::PreSubclassWindow();
}