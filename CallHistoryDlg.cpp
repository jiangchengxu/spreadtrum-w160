/*
File: HSDPADlg.cpp
Desc:
    通话历史记录

    显示历史通话记录，包括呼入和拨出等
    采用异步的方式，逐条返回，所以最好是在显示的时候开一个线程来做
*/
// CallHistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "CallHistoryDlg.h"
#include "SmsWriteDlg.h"
#include "ContactDlg.h"
#include "SelPosition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallHistoryDlg dialog
CCallHistoryDlg::CCallHistoryDlg(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CCallHistoryDlg::IDD, pParent)
{
    m_pHistData = ((CHSDPAApp *)AfxGetApp())->GetHistData();
    m_pPbData   = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    m_nBkTag    = 1;
}

void CCallHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCallHistoryDlg)
    DDX_Control(pDX, IDC_LIST_CALLHISTORY, m_lstHist);
    DDX_Control(pDX, IDC_TAB_CALLHISTORY, m_tabHist);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCallHistoryDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CCallHistoryDlg)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CALLHISTORY, OnSelchangeTabHist)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CALLHISTORY, OnColumnclickListHist)
    ON_BN_CLICKED(IDC_BUTTON_REPLY, OnButtonReply)
    ON_BN_CLICKED(IDC_BUTTON_SENDSMS, OnButtonSendsms)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
    ON_BN_CLICKED(IDC_BUTTON_ABSTRACT, OnButtonAbstract)
    ON_BN_CLICKED(IDC_BUTTON_DELETEALL, OnButtonDeleteall)
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_HIST_MENUITEM_CLICK, OnMenuItemClick)
    ON_MESSAGE(WM_ADD_CONTACT, OnAbstractPb)
    ON_MESSAGE(WM_HIST_SAVE_MSG, OnSaveHist)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallHistoryDlg message handlers
BOOL CCallHistoryDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();
    m_bFirst = TRUE;

//	SetSkin(g_SetData.Setup_nSkinStyle);

    UpdateData(FALSE);
    InitTabCtrl();
    InitListCtrl();
    InitDspIndexArr();
    DispCallHistory();
    return TRUE;
}

void CCallHistoryDlg::InitTabCtrl()
{
    if(m_ilTabs.Create(IDB_CALL_STATE, 16, 1, 255)) {
        m_tabHist.SetImageList(&m_ilTabs);
    }

    m_tabHist.SetParent(this);

    CString strTabCaption;
    strTabCaption.LoadString(IDS_CALLHIST_MISSED);
    m_tabHist.InsertItem(0, strTabCaption, 0);
    strTabCaption.LoadString(IDS_CALLHIST_RECEIVED);
    m_tabHist.InsertItem(1, strTabCaption, 1);
    strTabCaption.LoadString(IDS_CALLHIST_DIALED);
    m_tabHist.InsertItem(2, strTabCaption, 2);

    DWORD dwFlags = 0;
    dwFlags |= ETC_FLAT;
    dwFlags |= ETC_SELECTION;
    dwFlags |= ETC_GRADIENT;

    m_tabHist.SetBkgndColor(RGB(238,241,243));

    m_tabHist.EnableDraw(BTC_ALL);

    CEnTabCtrl::EnableCustomLook(dwFlags, dwFlags);

    //列表控件类型初始化为未接电话
    m_DispFilter = HISTKIND_MIS;
}

void CCallHistoryDlg::InitListCtrl()
{
    if(m_ilLsts.Create(IDB_CALL_STATE, 16, 1, 255)) {
        m_lstHist.SetImageList(&m_ilLsts, LVSIL_SMALL);
    }

    m_lstHist.SetParent(&m_tabHist);

    CRect rect;
    m_tabHist.GetItemRect(0, &rect);
    m_lstHist.SetWindowPos(NULL, 4, 28, rect.right, rect.bottom, TRUE);
    m_lstHist.GetWindowRect(&rect);

    BYTE arr_cols[HISTCOL_MAX][2] = {
        IDS_NAME, 80,
        IDS_NUMBER, 130,
        IDS_DATETIME, 120,
        IDS_CALLHIST_DURATION, 100,
        IDS_CALLHIST_FREQUENCY, rect.Width() - 40*3,
    };

    CString strColTitle;
    for(int i = 0; i < HISTCOL_MAX; i++) {
        strColTitle.LoadString(arr_cols[i][0]);
        m_lstHist.InsertColumn(i, strColTitle, LVCFMT_LEFT, arr_cols[i][1]);
    }

    m_lstHist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_lstHist.m_headerCtrl.DrawHeader();
}

void CCallHistoryDlg::OnSelchangeTabHist(NMHDR* pNMHDR, LRESULT* pResult)
{
    switch(m_tabHist.GetCurSel()) {
    case 0:
        m_DispFilter = HISTKIND_MIS;
        break;
    case 1:
        m_DispFilter = HISTKIND_RCV;
        break;
    case 2:
        m_DispFilter = HISTKIND_SND;
        break;
    default:
        m_DispFilter = HISTKIND_MIS;
        break;
    }
    DispCallHistory();
    *pResult = 0;
}

void CCallHistoryDlg::DispCallHistory()
{
    WORD  cnt;
    WORD  nIndex;
    WORD  nHistNum;
    stHistItem HistItem;
    TCHAR  szSeq[10];
    TCHAR  szName[PB_NAME_MAX + 2];
    CString strUnknown;

    strUnknown.LoadString(IDS_UNKNOWN);

    ASSERT(m_DispFilter >= HISTKIND_MIS && m_DispFilter < HISTKIND_MAX);

    nHistNum = m_pHistData->GetCount(m_DispFilter);
    ASSERT(nHistNum <= HIST_ITEM_MAX);

    strUnknown.LoadString(IDS_UNKNOWN);

    m_lstHist.SetRedraw(FALSE);
    m_lstHist.DeleteAllItems();

    for(cnt = 0; cnt < nHistNum; cnt++) {
        nIndex = m_DspIndexArr[m_DispFilter][cnt];
        ASSERT(nIndex < nHistNum);

        memset(szSeq, 0x00, sizeof(szSeq));
        memset(szName, 0x00, sizeof(szName));
        memset(&HistItem, 0x00, sizeof(stHistItem));

        HistItem = m_pHistData->GetItem(m_DispFilter, nIndex);

        if(wcscmp(HistItem.szNumber, strUnknown) == 0
                || !m_pPbData->SearchNameByNumber(HistItem.szNumber, szName))
            wcsncpy(szName, strUnknown, PB_NAME_MAX);

        m_lstHist.InsertItem(cnt, _T(""), (int)m_DispFilter);
        m_lstHist.SetItemText(cnt, HISTCOL_NAME, szName);
        m_lstHist.SetItemText(cnt, HISTCOL_NUMBER, HistItem.szNumber);
        m_lstHist.SetItemText(cnt, HISTCOL_TIMESTAMP, HistItem.cTimeStamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT));
        m_lstHist.SetItemText(cnt, HISTCOL_DURATION, HistItem.cDuration.Format(_T("%H:%M:%S")));
        m_lstHist.SetItemText(cnt, HISTCOL_FREQUENCY, _itot(HistItem.nSeqNum, szSeq, sizeof(szSeq)));
    }

    m_lstHist.m_headerCtrl.SetSortImage(m_SortColArr[m_DispFilter], m_bSortAscendArr[m_DispFilter]);

    m_lstHist.SetRedraw(TRUE);
    m_lstHist.Invalidate();
    m_lstHist.UpdateWindow();
}

int CCallHistoryDlg::DealwithResponseProc(LPARAM lParam, WPARAM wParam)
{
    DispCallHistory();
    return 1;
}

void CCallHistoryDlg::OnButtonReply()
{
    int nCurSel;

    if(m_pHistData->IsEmpty(m_DispFilter))
        return;
    else if((nCurSel = m_lstHist.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return;
    }

    CString strUnknown;
    strUnknown.LoadString(IDS_UNKNOWN);

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);

    if(pComm->CommIsReady()) {
        WORD nIndex = m_DspIndexArr[m_DispFilter][nCurSel];
        ASSERT(nIndex < m_pHistData->GetCount(m_DispFilter));

        WPARAM wParam = 0;
        LPARAM lParam = (LPARAM)(LPCTSTR)(m_pHistData->GetItem(m_DispFilter, nIndex)).szNumber;

        if(wcslen((LPCTSTR)lParam) > 0 && wcscmp((LPCTSTR)lParam, strUnknown)) {
            ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam, lParam);
            OnOK();
        } else
            AfxMessageBox(IDS_SELCONNECT_NULLNUM);
    }
}

void CCallHistoryDlg::OnButtonSendsms()
{
    int nCurSel;

    if(m_pHistData->IsEmpty(m_DispFilter))
        return;
    else if((nCurSel = m_lstHist.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return;
    }

    CString strUnknown;
    strUnknown.LoadString(IDS_UNKNOWN);

    WORD nIndex = m_DspIndexArr[m_DispFilter][nCurSel];
    ASSERT(nIndex < m_pHistData->GetCount(m_DispFilter));

    LPARAM lParam = (LPARAM)(LPCTSTR)(m_pHistData->GetItem(m_DispFilter, nIndex)).szNumber;

    if(wcslen((LPCTSTR)lParam) > 0 && wcscmp((LPCTSTR)lParam, strUnknown)) {
        CSmsWriteDlg dlg(this, (LPCTSTR)lParam, NULL);
        dlg.DoModal();
    } else
        AfxMessageBox(IDS_SELCONNECT_NULLNUM);
}

void CCallHistoryDlg::OnButtonDelete()
{
    int nCurSel;

    if(m_pHistData->IsEmpty(m_DispFilter))
        return;
    else if((nCurSel = m_lstHist.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return;
    }

    CString str;
    str.LoadString(IDS_DELETECONFIRM);

    if(MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
        return;
    else {
        WORD nIndex;
        UINT nDelCount = m_lstHist.GetSelectedCount();

        do {
            nIndex = m_DspIndexArr[m_DispFilter][nCurSel];
            ASSERT(nIndex < m_pHistData->GetCount(m_DispFilter));

            int i;
            for(i = nCurSel; i < m_pHistData->GetCount(m_DispFilter) - 1; i++) {
                m_DspIndexArr[m_DispFilter][i] = m_DspIndexArr[m_DispFilter][i+1];
            }
            m_DspIndexArr[m_DispFilter][i] = 0;

            m_lstHist.DeleteItem(nCurSel);
            m_pHistData->RemoveItem(m_DispFilter, nIndex);

            for(i = 0; i < m_pHistData->GetCount(m_DispFilter); i++)
                if(m_DspIndexArr[m_DispFilter][i] > nIndex)
                    m_DspIndexArr[m_DispFilter][i]--;

            if(nDelCount == 1 && !m_pHistData->IsEmpty(m_DispFilter)) {
                if(nCurSel == m_pHistData->GetCount(m_DispFilter))
                    m_lstHist.SetItemState(nCurSel-1, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
                else
                    m_lstHist.SetItemState(nCurSel, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
                break;
            }
        } while((nCurSel = m_lstHist.GetNextItem(nCurSel-1, LVNI_SELECTED)) != -1);
    }
}

LRESULT CCallHistoryDlg::OnMenuItemClick(WPARAM wParam, LPARAM lParam)
{
    UINT nID = wParam;

    if(nID == IDC_BUTTON_REPLY)
        OnButtonReply();
    else if(nID == IDC_BUTTON_ABSTRACT)
        OnButtonAbstract();
    else if(nID == IDC_BUTTON_SENDSMS)
        OnButtonSendsms();
    else if(nID == IDC_BUTTON_DELETE)
        OnButtonDelete();
    else if(nID == IDC_BUTTON_DELETEALL)
        OnButtonDeleteall();

    return 0;
}

void CCallHistoryDlg::OnButtonAbstract()
{
    int nCurSel;

    if(m_pHistData->IsEmpty(m_DispFilter))
        return;

    if((nCurSel = m_lstHist.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return;
    }

    if(m_pPbData->CheckFull()) {
        AfxMessageBox(IDS_PB_FULL);
        return;
    }

    CString strUnknown;
    strUnknown.LoadString(IDS_UNKNOWN);

    WORD nIndex = m_DspIndexArr[m_DispFilter][nCurSel];
    ASSERT(nIndex < m_pHistData->GetCount(m_DispFilter));

    stHistItem HistItem;
    memset(&HistItem, 0x00, sizeof(stHistItem));
    HistItem = m_pHistData->GetItem(m_DispFilter, nIndex);

    if(wcslen((TCHAR*)HistItem.szNumber) > 0
            && wcscmp((TCHAR*)HistItem.szNumber, strUnknown)) {
        if(CompareChar((TCHAR*)HistItem.szNumber)) {
            CSelPosition dlg(this, (TCHAR *)HistItem.szNumber);
            dlg.DoModal();
            DispCallHistory();
        }
    } else
        AfxMessageBox(IDS_SELCONNECT_NULLNUM);
}

void CCallHistoryDlg::OnButtonDeleteall()
{
    if(m_pHistData->IsEmpty(m_DispFilter))
        return;

    CString strMessage;
    strMessage.LoadString(IDS_DELALLCONFIRM);
    if(AfxMessageBox(strMessage, MB_YESNO|MB_ICONQUESTION| MB_DEFBUTTON2) == IDYES) {
        m_lstHist.DeleteAllItems();
        m_pHistData->RemoveAllItem(m_DispFilter);
        InitDspIndexArr();
    }
}

LRESULT CCallHistoryDlg::OnAbstractPb(WPARAM wParam, LPARAM lParam)
{
    this->GetParent()->SendMessage(WM_ADD_CONTACT, wParam, lParam);
    return 0;
}

void CCallHistoryDlg::OnColumnclickListHist(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // TODO: Add your control notification handler code here
    int col = pNMListView->iSubItem;

    ASSERT(col >= (int)HISTCOL_NAME && col <= (int)HISTCOL_FREQUENCY);

    if(col == (int)m_SortColArr[m_DispFilter]) {
        m_bSortAscendArr[m_DispFilter] = !m_bSortAscendArr[m_DispFilter];
        ReverseSortIndexArr(m_DispFilter);
    } else {
        m_bSortAscendArr[m_DispFilter] = TRUE;
        m_SortColArr[m_DispFilter] = (EnHistCol)col;
        SortHistList(m_DispFilter, m_SortColArr[m_DispFilter]);
    }

    m_lstHist.m_headerCtrl.SetSortImage(col, m_bSortAscendArr[m_DispFilter]);
    DispCallHistory();

    *pResult = 0;
}

//默认以时间递减顺序显示
void CCallHistoryDlg::InitDspIndexArr()
{
    int i;

    for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++) {
        m_bSortAscendArr[i] = FALSE;
        m_SortColArr[i] = HISTCOL_TIMESTAMP;
    }

    ResetDspIndexArr(HISTKIND_MAX);
}

//各字段以从小到大次序递增排序
void CCallHistoryDlg::SortHistList(EnHistKind type, EnHistCol col)
{
    ASSERT(type >= HISTKIND_MIS && type < HISTKIND_MAX);
    ASSERT(col >= HISTCOL_NAME && col <= HISTCOL_FREQUENCY);

    WORD j;
    WORD nNum = m_pHistData->GetCount(type);

    if(col == HISTCOL_TIMESTAMP) {
        for(j = 0; j < nNum; j++)
            m_DspIndexArr[type][j] = nNum - 1 - j;
    } else {
        BubbleSort(type, col);
    }
}

//以当前选中的排序关键字逆序排序
void CCallHistoryDlg::ReverseSortIndexArr(EnHistKind type)
{
    WORD tmp;
    WORD nNum = m_pHistData->GetCount(type);

    for(WORD i = 0; i < nNum/2; i++) {
        tmp = m_DspIndexArr[type][i];
        m_DspIndexArr[type][i] = m_DspIndexArr[type][nNum-1-i];
        m_DspIndexArr[type][nNum-1-i] = tmp;
    }
}

//非递减冒泡排序
void CCallHistoryDlg::BubbleSort(EnHistKind type, EnHistCol col)
{
    ASSERT(type >= HISTKIND_MIS && type < HISTKIND_MAX);
    ASSERT(col >= HISTCOL_NAME && col <= HISTCOL_FREQUENCY);

    WORD nNum = m_pHistData->GetCount(type);

    int  nSubItem = (int)col;
    int  change = 1;
    WORD tmp;
    WORD i, j;
    int  res;
    TCHAR  szName1[PB_NAME_MAX + 2];
    TCHAR  szName2[PB_NAME_MAX + 2];

    for(i = 0; i < nNum - 1 && change; i++) {
        change = 0;
        for(j = nNum - 1; j > i; j--) {
            res = 0;
            switch(col) {
            case HISTCOL_NAME:
                memset(szName1, 0x00, sizeof(szName1));
                memset(szName2, 0x00, sizeof(szName2));
                m_pPbData->SearchNameByNumber(CString(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).szNumber), szName1);
                m_pPbData->SearchNameByNumber(CString(m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).szNumber), szName2);
                res = wcscmp(szName1, szName2);
                break;
            case HISTCOL_NUMBER:
                res = strcmp((const char *)(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).szNumber),
                             (const char *)(m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).szNumber));
                break;
            case HISTCOL_TIMESTAMP:
                if(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).cTimeStamp
                        < m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).cTimeStamp)
                    res = -1;
                break;
            case HISTCOL_DURATION:
                if(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).cDuration
                        < m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).cDuration)
                    res = -1;
                break;
            case HISTCOL_FREQUENCY:
                if(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).nSeqNum
                        < m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).nSeqNum)
                    res = -1;
                break;
            }

            if(res < 0) {
                tmp = m_DspIndexArr[type][j];
                m_DspIndexArr[type][j] = m_DspIndexArr[type][j-1];
                m_DspIndexArr[type][j-1] = tmp;
                change   = 1;
            }
        }
    }
}

void CCallHistoryDlg::UpdateHistSortList(EnHistKind type)
{
    ResetDspIndexArr(type);

    if(type == HISTKIND_MAX) {
        for(WORD i=0; i<HISTKIND_MAX; i++) {
            SortHistList((EnHistKind)i, m_SortColArr[i]);
            if(!m_bSortAscendArr[i])
                ReverseSortIndexArr((EnHistKind)i);
        }
    } else {
        SortHistList(type, m_SortColArr[type]);
        if(!m_bSortAscendArr[type])
            ReverseSortIndexArr(type);
    }
    if(type == HISTKIND_MAX || type == m_DispFilter)
        DispCallHistory();
}

void CCallHistoryDlg::ResetDspIndexArr(EnHistKind type)
{
    int i, j;

    if(type == HISTKIND_MAX) {
        for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++)
            for(j = 0; j < HIST_ITEM_MAX; j++)
                m_DspIndexArr[i][j] = 0;

        for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++)
            for(j = 0; j < m_pHistData->GetCount((EnHistKind)i); j++)
                m_DspIndexArr[i][j] = j;
    } else {
        for(j = 0; j < HIST_ITEM_MAX; j++)
            m_DspIndexArr[type][j] = 0;
        for(j = 0; j < m_pHistData->GetCount(type); j++)
            m_DspIndexArr[type][j] = j;
    }
}

LRESULT CCallHistoryDlg::OnSaveHist(WPARAM wParam, LPARAM lParam)
{
    EnHistKind type = (EnHistKind)wParam;
    ASSERT(type >= HISTKIND_MIS && type <= HISTKIND_MAX);
    UpdateHistSortList(type);
    return 0;
}