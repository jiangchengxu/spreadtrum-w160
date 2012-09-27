// LogListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "LogListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*
  Button
  nRange	nID						topicInfo
  0			IDC_BUTTON_DELETE	    "Delete"
  1			IDC_BUTTON_DELETEALL	"Delete All"
  2			IDC_BUTTON_EXPORT		"Export"
  Menu
  nRange	nID						topicInfo
  0			ID_MLOG_DELETE			"Delete"
  1			ID_MLOG_DELETEALL		"Delete All"
  2			ID_MLOG_EXPORT			"Export"
*/
/////////////////////////////////////////////////////////////////////////////
// CLogListCtrl

CLogListCtrl::CLogListCtrl()
{
    m_pszSeparator = NULL;
    m_nSortCol = -1;
    m_bSortAscending = TRUE;
}

CLogListCtrl::~CLogListCtrl()
{
    if (m_pszSeparator != NULL)
        delete [] m_pszSeparator;
}
void CLogListCtrl::AdjustColumnWidth()
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
int CLogListCtrl::GetColumnCount()
{
    CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
    return (pHeaderCtrl->GetItemCount());
    //m_headerCtrl = *pHeaderCtrl;
    //m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd());
    //return (m_headerCtrl.GetItemCount());
}

BEGIN_MESSAGE_MAP(CLogListCtrl, CListCtrl)
    //{{AFX_MSG_MAP(CHistListCtrl)
    ON_WM_CONTEXTMENU()
    ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_COMMAND_RANGE(ID_MLOG_DELETE, ID_MLOG_EXPORT, OnMenuClicked)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistListCtrl message handlers

void CLogListCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
    /*
    if( point.x == -1 && point.y == -1 ){
        CRect rect;
        GetClientRect( &rect );
        point = rect.TopLeft();
        point.Offset( 5, 5 );
        ClientToScreen( &point );
    }
    */
    if(GetNextItem(-1, LVNI_SELECTED) == -1)
        return;

    CMenu logMenu;
    logMenu.LoadMenu( IDR_MENU_LOG_RCLICK );

    CMenu* pPopup = logMenu.GetSubMenu( 0 );
    ASSERT_VALID( pPopup );

    pPopup->TrackPopupMenu(    TPM_LEFTALIGN | TPM_LEFTBUTTON,
                               point.x, point.y,
                               this, NULL );
}


void CLogListCtrl::OnMenuClicked(UINT nID)
{
    UINT arID[]= {
        IDC_BUTTON_DELETE,
        IDC_BUTTON_DELETEALL,
        IDC_BUTTON_EXPORT,
    };

    CWnd *pParent = GetParent();
    ASSERT(pParent != NULL);
    pParent->SendMessage(WM_LOG_MENUITEM_CLICK, arID[nID-ID_MLOG_DELETE], 0);
}

BOOL CLogListCtrl::IsValidIndex(int nIndex) const
{
    return nIndex >= 0 && nIndex < CListCtrl::GetItemCount();
}
void CLogListCtrl::ResetSortCol()
{
    m_nSortCol = -1;
    m_bSortAscending = TRUE;
}
DWORD CLogListCtrl::GetItemStates(int nItem) const
{
    DWORD dwStates = 0;

    if (CListCtrl::GetItemState(nItem, LVIS_SELECTED))
        dwStates |= RC_ITEM_SELECTED;
    else
        dwStates |= RC_ITEM_UNSELECTED;

    return dwStates;
}
BOOL CLogListCtrl::SetItemStates(int nItem, DWORD dwNewStates)
{
    BOOL bResult;
    if (dwNewStates & RC_ITEM_SELECTED)
        bResult &= CListCtrl::SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);

    if (dwNewStates & RC_ITEM_UNSELECTED)
        bResult &= CListCtrl::SetItemState(nItem, 0, LVIS_SELECTED);

    return bResult;
}
BOOL CLogListCtrl::SwapItems(int nItem1, int nItem2)
{
    if (!IsValidIndex(nItem1) || !IsValidIndex(nItem2))
        return FALSE;

    if (nItem1 == nItem2)
        return TRUE;

    ResetSortCol();

    // record previous states first
    const DWORD STATES1 = GetItemStates(nItem1);
    const DWORD STATES2 = GetItemStates(nItem2);
    const DWORD DATA1 = CListCtrl::GetItemData(nItem1);
    const DWORD DATA2 = CListCtrl::GetItemData(nItem2);

    // swap item texts and images
    for (int i = 0; i < GetColumnCount(); i++) {
        CString str = GetItemText(nItem1, i);
        CListCtrl::SetItemText(nItem1, i, GetItemText(nItem2, i));
        CListCtrl::SetItemText(nItem2, i, str);
    }

    // swap item data
    CListCtrl::SetItemData(nItem1, DATA2);
    CListCtrl::SetItemData(nItem2, DATA1);

    // restore states
    SetItemStates(nItem1, STATES2);
    SetItemStates(nItem2, STATES1);

    return TRUE;
}
int CLogListCtrl::CompareItems(int nItem1, int nItem2)
{
    const CString s1 = GetItemText(nItem1, m_nSortCol);
    const CString s2 = GetItemText(nItem2, m_nSortCol);

    double f1, f2;
    if(ItemCompareFunc::IsDecNumber(s1, f1) && ItemCompareFunc::IsDecNumber(s2, f2))
        return ItemCompareFunc::DecNumberCompare(f1, f2);

    COleDateTime date1, date2;
    if(ItemCompareFunc::IsDate(s1, date1) && ItemCompareFunc::IsDate(s2, date2))
        return ItemCompareFunc::DateCompare(date1, date2);

    // plain text.
    return s1.Compare(s2);
}

int CLogListCtrl::FindSeparator(int nStartAfter, int nColumn) const
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

void CLogListCtrl::QuickSortRecursive(int* pArr, int nLow, int nHigh)
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

BOOL CLogListCtrl::PartialSort(int nStart, int nEnd)
{
    if (nStart >= nEnd || !IsValidIndex(nStart) || !IsValidIndex(nEnd))
        return FALSE;

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

void CLogListCtrl::SortItems(int nColumn, BOOL bAscending)
{
    // do the sorting
    m_nSortCol = nColumn;
    m_bSortAscending = bAscending;

    BOOL bEnd = FALSE;
    int nSep1 = -1;
    int nSep2 = FindSeparator(-1, nColumn);
    do {
        if (nSep2 < 0) {
            nSep2 = GetItemCount();
            bEnd = TRUE;
        }
        PartialSort(nSep1 + 1, nSep2 - 1);
        nSep1 = FindSeparator(nSep2 - 1, nColumn);
        nSep2 = FindSeparator(nSep1, nColumn);
    } while (!bEnd && nSep1 >= 0);

}

void CLogListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // TODO: Add your control notification handler code here
    const int COL = pNMListView->iSubItem;

    if(COL < 0 || COL >= GetColumnCount())
        return;

    m_headerCtrl.SetSortImage(COL,COL == m_nSortCol ? !m_bSortAscending : TRUE);
    SortItems(COL, COL == m_nSortCol ? !m_bSortAscending : TRUE);
    *pResult = 0;
}


int CLogListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CListCtrl::OnCreate(lpCreateStruct) == -1)
        return -1;
    if (!m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd()))
        return -1;
    return 0;
}

void CLogListCtrl::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();
    m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd());
}
