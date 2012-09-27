// SmsExtractNumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "SmsExtractNumDlg.h"
#include "Ds.h"
#include "SelPosition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmsExtractNumDlg dialog


CSmsExtractNumDlg::CSmsExtractNumDlg(LPCTSTR lpNum, CWnd* pParent /*=NULL*/)
    : CBaseDialog(CSmsExtractNumDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSmsExtractNumDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    memset(m_szNumber, 0x0000, sizeof(m_szNumber));
    if(lpNum) {
        wcsncpy(m_szNumber, lpNum, PB_NUM_MAX);
    }
}


void CSmsExtractNumDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSmsExtractNumDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_LIST_EXTRACT_NUM, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CSmsExtractNumDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CSmsExtractNumDlg)
    ON_MESSAGE(WM_ABSTRACT_NUM, OnAbstractNum)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_EXTRACT_NUM, OnDblclkListExtractNum)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsExtractNumDlg message handlers

BOOL CSmsExtractNumDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_bFirst = TRUE;

//	SetSkin(g_SetData.Setup_nSkinStyle);

    InitListCtrl();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsExtractNumDlg::InitListCtrl()
{
    BYTE    i, j;
    CRect   rect;
    CString strColTitle;

    m_listCtrl.GetWindowRect(&rect);

    //设置列名
    strColTitle.LoadString(IDS_MOBILE);
    m_listCtrl.InsertColumn(0, strColTitle, LVCFMT_LEFT, rect.Width());

    //填入数据
    i = 0;
    if(wcslen(m_szNumber) > 0) {
        m_listCtrl.InsertItem(i, m_szNumber, i);
        i++;
    }

    for(j=0; j < gSmsExtractNumCnt; i++,j++) {
        m_listCtrl.InsertItem(i, (CString)gSmsExtractNumBuf[j], i);//liub_0401
        //m_listCtrl.InsertItem(i, gSmsExtractNumBuf[j], i);
    }

    m_listCtrl.AdjustColumnWidth();
    m_listCtrl.m_headerCtrl.DrawHeader();
    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CSmsExtractNumDlg::OnOK()
{
    int nSel;
    CString pSelNum;

    if((nSel = m_listCtrl.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return;
    } else {
        if(wcslen(m_szNumber) > 0) {
            if(nSel > 0)
                pSelNum = gSmsExtractNumBuf[nSel-1];
            else
                pSelNum = m_szNumber;
        } else {
            pSelNum = gSmsExtractNumBuf[nSel];
        }

        ShowWindow(SW_HIDE);
        CSelPosition dlg(this, pSelNum);
        dlg.DoModal();
    }
}

LRESULT CSmsExtractNumDlg::OnAbstractNum(WPARAM wParam, LPARAM lParam)
{
    //Cancel
    if(wParam == 1)
        ShowWindow(SW_NORMAL);
    else //Ok
        OnCancel();
    return 0;
}

void CSmsExtractNumDlg::OnDblclkListExtractNum(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnOK();
}
