/*
File: HSDPADlg.cpp
Desc:
    所有附着在TABDIALOG上的DIALOG父类，用来规范这些窗口的行为和属性
    如：是否可以进行切换：在通话和数据通讯时，窗口不可以切换。
*/

// BaseDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "TabedDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(CTabedDialog, CBaseDialog)

CTabedDialog::CTabedDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
    : CBaseDialog(nIDTemplate, pParent)
{
    //{{AFX_DATA_INIT(CTabedDialog)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nBkTag = 1;
}

CTabedDialog::~CTabedDialog()
{

}

void CTabedDialog::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTabedDialog)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabedDialog, CBaseDialog)
    //{{AFX_MSG_MAP(CTabedDialog)
    ON_WM_DESTROY()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabedDialog message handlers

BOOL CTabedDialog::OnInitDialog()
{

    CBaseDialog::OnInitDialog();
    //SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & (~(WS_CAPTION | WS_BORDER)));
    /*
    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    */
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon


    return TRUE;  // return TRUE  unless you set the focus to a control
}




void CTabedDialog::OnDestroy()
{
    WinHelp(0L, HELP_QUIT);

    //wyw_0104   CBaseDialog::OnDestroy()已Destroy
    //::DestroyIcon(m_hIconInfo);

    CBaseDialog::OnDestroy();
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTabedDialog::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

/*************************************************************************/
/*
Name:    CanSwitchMe
Desc:    是否可以切换窗口
Param:

Return:     TRUE:可以
         FALSE：不可以
*/
/*************************************************************************/

BOOL CTabedDialog::CanSwitchMe()
{
    return TRUE;
}



CShadeButtonST* CTabedDialog::GetButton()
{
    return m_pButton;
}

void CTabedDialog::SetButton(CShadeButtonST *pButton)
{
    m_pButton = pButton;
}

void CTabedDialog::OnCancel()
{
    // TODO: Add extra cleanup here

//	CBaseDialog::OnCancel();
}

void CTabedDialog::OnOK()
{
    // TODO: Add extra cleanup here

//	CBaseDialog::OnOK();
}