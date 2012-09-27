// SmsTemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmsTemplateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int TEMPLATECOUNT = 20;

const int ITEMCOUNT = 10;
const TCHAR ITEMCONTENT[][160] = {
    _T("Please call."),
    _T("I'm in a meeting. Call me later."),
    _T("I'm late. I will be there soon."),
    _T("Where are you? Can we talk?"),
    _T("I'm busy. I will call you later."),
    _T("Jokes <Send Msg to >"),
    _T("Win Prizes <Send Msg to >"),
    _T("Cricket Score <Send Msg to >"),
    _T("Astrology <Send Msg to >"),
    _T("Menu of SMS Services <Send Msg to >"),
};

/////////////////////////////////////////////////////////////////////////////
// CSmsTemplateEditDlg dialog


CSmsTemplateEditDlg::CSmsTemplateEditDlg(const TCHAR *pSzText, CWnd* pParent /*=NULL*/)
    : CBaseDialog(CSmsTemplateEditDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSmsTemplateDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_nBkTag = 1;
    m_strEditContent = pSzText;

    if (pSzText) {
        m_nType = 1;		//1代表 Edit
    } else {
        m_nType = 0;		//0代表 New
    }
}


void CSmsTemplateEditDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSmsTemplateDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmsTemplateEditDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CSmsTemplateDlg)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsTemplateDlg message handlers
void CSmsTemplateEditDlg::OnOK()
{
    // TODO: Add extra cleanup here
    GetDlgItem(IDC_EDIT_CONTENT)->GetWindowText(m_strEditContent);

    if (!m_strEditContent.IsEmpty()) {
        if (m_nType == 0) {
            WPARAM wParam = (WPARAM)(LPCTSTR)(m_strEditContent);
            ::SendMessage(GetParent()->m_hWnd, WM_ADDCONTENT, wParam, 0);
        } else if (m_nType == 1) {
            WPARAM wParam = (WPARAM)(LPCTSTR)(m_strEditContent);
            ::SendMessage(GetParent()->m_hWnd, WM_ADDCONTENT, wParam, 1);
        }
    }

    CBaseDialog::OnOK();
}

BOOL CSmsTemplateEditDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();

    // TODO: Add extra initialization here
    ((CEdit*)GetDlgItem(IDC_EDIT_CONTENT))->SetLimitText(40);
    GetDlgItem(IDC_EDIT_CONTENT)->SetFocus();
    GetDlgItem(IDC_EDIT_CONTENT)->SetWindowText(m_strEditContent);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsTemplateEditDlg::OnButtonClear()
{
    GetDlgItem(IDC_EDIT_CONTENT)->SetWindowText(_T(""));
}

/////////////////////////////////////////////////////////////////////////////
// CSmsTemplateDlg dialog

CSmsTemplateDlg::CSmsTemplateDlg(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CSmsTemplateDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSmsTemplateDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_nBkTag = 1;

    m_pSmsTemplateData = ((CHSDPAApp *)AfxGetApp())->GetSmsTemplateData();

    m_nIndex = -1;
}


void CSmsTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSmsTemplateDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDC_LIST_CONTENT, m_listCtrl);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmsTemplateDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CSmsTemplateDlg)
    ON_BN_CLICKED(IDC_BUTTON_INSERT, OnButtonInsert)
    ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
    ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
    ON_NOTIFY(NM_CLICK, IDC_LIST_CONTENT, OnListSel)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONTENT, OnListDBLCLK)

    ON_MESSAGE(WM_ADDCONTENT, OnTemplateAddOrEdit)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsTemplateDlg message handlers
void CSmsTemplateDlg::OnOK()
{
    // TODO: Add extra cleanup here

    //CBaseDialog::OnOK();
}

BOOL CSmsTemplateDlg::InitList()
{
// 	CListCtrl *pList = (CListCtrl *)GetDlgItem(IDC_LIST_CONTENT);
// 	if (NULL == pList)
// 		return FALSE;

    LV_COLUMN lvc;
    lvc.fmt = LVCFMT_LEFT;
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.iSubItem = 0;
    lvc.pszText = _T("Content");
    lvc.cx = 340;
    m_listCtrl.InsertColumn(0, &lvc);

    DWORD dwStyle = m_listCtrl.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_listCtrl.SetExtendedStyle(dwStyle);

    StSmsRecord smsRecord;
    memset(&smsRecord, 0x00, sizeof(StSmsRecord));

    int count = m_pSmsTemplateData->GetSmsNum(SMS_TYPE_ARCHIVE);
    if (count == 0) {
        for (int i=0; i<ITEMCOUNT; i++) {
            m_listCtrl.InsertItem(i, ITEMCONTENT[i]);

            COleDateTime time;
            m_pSmsTemplateData->AddSmsRecord(SMS_TYPE_ARCHIVE, SMS_STATE_MT_READ, _T(""), time, ITEMCONTENT[i]);
        }
    }
    for (int i=0; i<count; i++) {
        smsRecord = m_pSmsTemplateData->ReadSmsRecord(SMS_TYPE_ARCHIVE, count-1-i);

        DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, smsRecord.szContent, -1, NULL, 0);
        wchar_t *pwText;
        pwText = new wchar_t[dwNum];
        if(!pwText) {
            delete []pwText;
            continue;
        }
        MultiByteToWideChar (CP_ACP, 0, smsRecord.szContent, -1, pwText, dwNum);

        m_listCtrl.InsertItem(i, pwText);

        delete []pwText;
    }
    if (count == TEMPLATECOUNT) {
        GetDlgItem(IDC_BUTTON_NEW)->EnableWindow(FALSE);
    }

    m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE);
    int nColumnWidth = m_listCtrl.GetColumnWidth(0);
    m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
    int nHeaderWidth = m_listCtrl.GetColumnWidth(0);
    m_listCtrl.SetColumnWidth(0, max(nColumnWidth+20, nHeaderWidth));

    return TRUE;
}

BOOL CSmsTemplateDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();

    // TODO: Add extra initialization here
    InitList();

// 	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsTemplateDlg::OnButtonInsert()
{
    int count = m_pSmsTemplateData->GetSmsNum(SMS_TYPE_ARCHIVE);
    if (m_nIndex>=count || m_nIndex<0) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    CString strText = m_listCtrl.GetItemText(m_nIndex, 0);

    if (!strText.IsEmpty()) {
        WPARAM wParam = (WPARAM)(LPCTSTR)(strText);
        ::SendMessage(GetParent()->m_hWnd, WM_SELCONTENT, wParam, 0);
    }
    CBaseDialog::OnOK();
}

void CSmsTemplateDlg::OnListSel(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_nIndex = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);

    *pResult = 0;
}

void CSmsTemplateDlg::OnButtonNew()
{
    CSmsTemplateEditDlg dlg(NULL, this);
    dlg.DoModal();
}

void CSmsTemplateDlg::OnButtonEdit()
{
    int count = m_pSmsTemplateData->GetSmsNum(SMS_TYPE_ARCHIVE);
    if (m_nIndex>=count || m_nIndex<0) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    CString strText = m_listCtrl.GetItemText(m_nIndex, 0);

    CSmsTemplateEditDlg dlg(strText, this);
    dlg.DoModal();
}

void CSmsTemplateDlg::OnButtonDelete()
{
    int count = m_pSmsTemplateData->GetSmsNum(SMS_TYPE_ARCHIVE);
    if (m_nIndex>=count || m_nIndex<0) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    m_listCtrl.DeleteItem(m_nIndex);

    m_pSmsTemplateData->DeleteSmsRecord(SMS_TYPE_ARCHIVE, count-1-m_nIndex);

    GetDlgItem(IDC_BUTTON_NEW)->EnableWindow(TRUE);

    m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE);
    int nColumnWidth = m_listCtrl.GetColumnWidth(0);
    m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
    int nHeaderWidth = m_listCtrl.GetColumnWidth(0);
    m_listCtrl.SetColumnWidth(0, max(nColumnWidth+20, nHeaderWidth));

    m_nIndex = -1;
}

void CSmsTemplateDlg::OnTemplateAddOrEdit(WPARAM wp, LPARAM lp)
{
    TCHAR *szText = (TCHAR*)wp;

    int count = m_listCtrl.GetItemCount();

    if (lp == 0) {
        m_listCtrl.InsertItem(count, szText);

        COleDateTime time;
        m_pSmsTemplateData->AddSmsRecord(SMS_TYPE_ARCHIVE, SMS_STATE_MT_READ, _T(""), time, szText);

        if (m_pSmsTemplateData->GetSmsNum(SMS_TYPE_ARCHIVE) == TEMPLATECOUNT) {
            GetDlgItem(IDC_BUTTON_NEW)->EnableWindow(FALSE);
        }
    } else if (lp == 1) {
        m_listCtrl.SetItemText(m_nIndex, 0, szText);

        StSmsRecord smsRecord;
        memset(&smsRecord, 0x00, sizeof(StSmsRecord));

        DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL, szText, -1, NULL, 0, NULL, FALSE);
        char *psText;
        psText = new char[dwNum+1];
        if(!psText) {
            delete []psText;
        }
        WideCharToMultiByte (CP_OEMCP,NULL,szText,-1,psText,dwNum,NULL,FALSE);
        strcpy(smsRecord.szContent, psText);
        delete []psText;

        m_pSmsTemplateData->EditSmsRecord(SMS_TYPE_ARCHIVE, count-m_nIndex-1, smsRecord);
    }

    m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE);
    int nColumnWidth = m_listCtrl.GetColumnWidth(0);
    m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
    int nHeaderWidth = m_listCtrl.GetColumnWidth(0);
    m_listCtrl.SetColumnWidth(0, max(nColumnWidth+20, nHeaderWidth));
}

void CSmsTemplateDlg::OnListDBLCLK(NMHDR *pNMHDR, LRESULT *pResult)
{
    OnButtonInsert();

    *pResult = 0;
}
