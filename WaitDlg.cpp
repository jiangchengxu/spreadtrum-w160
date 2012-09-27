// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "WaitDlg.h"
#include "Ds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog


CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/,int nUpper,int nSetp )
    : CDialog(CWaitDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CWaitDlg)
    m_strPrompt = _T("Transfering ...");
    //}}AFX_DATA_INIT
    // m_nBkTag = 1;

    m_bSuccess = false;
    m_nLower = 0;
    m_nUpper = nUpper;
    m_nSetp  = nSetp;

}
CWaitDlg::~CWaitDlg()
{
    ::SetEvent(g_BGPassEvt);
}


void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CWaitDlg)
    DDX_Control(pDX, IDC_PROGRESS_WAIT, m_ProgressWait);
    DDX_Text(pDX, IDC_STATIC_PROMPT, m_strPrompt);
    DDX_Text(pDX, IDC_ITEM, m_strItem);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)
    //{{AFX_MSG_MAP(CWaitDlg)
    ON_MESSAGE(WM_CLOSE,OnClose)
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_Wait_ProgressSet,ProgressSet)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg message handlers
BOOL CWaitDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_CLOSE) {
        return CDialog::PreTranslateMessage(pMsg);
    }
    return true;

}

BOOL CWaitDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

//     m_bFirst = TRUE;
//
// 	SetSkin(g_SetData.Setup_nSkinStyle);

    // TODO: Add extra initialization here
    SetProgressRange(m_nLower,m_nUpper,m_nSetp);


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}



void CWaitDlg::SetProgressRange(int nLower,int nUpper,int nStep)
{
    m_nLower = nLower;
    m_nUpper = nUpper;
    m_nSetp = nStep;
    m_ProgressWait.SetRange(m_nLower, m_nUpper);
    m_ProgressWait.SetStep(m_nSetp);

}

void CWaitDlg::ProgressCompleted(int nPos)
{
    m_ProgressWait.SetPos(nPos);
    Sleep(100);
}

LRESULT CWaitDlg::OnClose(WPARAM wParam, LPARAM lParam)
{
    CDialog::OnCancel();
    return 0;
}

LRESULT CWaitDlg::ProgressSet(WPARAM wParam, LPARAM lParam)
{
    int nPos = (int)wParam;
    m_ProgressWait.SetPos(nPos);

    return 0;
}