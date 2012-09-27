// DlgSetupNWSel.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "DlgSetupNWSel.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupNWSel dialog

CDlgSetupNWSel::CDlgSetupNWSel(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CDlgSetupNWSel::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSetupNWSel)
    m_nSelMode = 0;
    pstNetWork = NULL;
    m_nAutoMode = 3;
    nSetPREFOK = 0;
    m_nMsgBeat = 0;
    //}}AFX_DATA_INIT
    m_nBkTag = 1;
    bSetNWSucc = false;
//	CopsState = 0;//非service limited 状态
}

CDlgSetupNWSel::~CDlgSetupNWSel()
{
    //DELETEIF(pdlgMsg);//del by liub 1103 for DlgSetup
}

void CDlgSetupNWSel::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSetupNWSel)
//	DDX_Control(pDX, IDC_LIST_SETUP_NWSEL_NWLIST, m_lstNW);//liub modify
//	DDX_Radio(pDX, IDC_RADIO_SETUP_NWSEL_AUTOSEL, m_nSelMode);//liub modify
//	DDX_Radio(pDX, IDC_RADIO_SETUP_NWSEL_GEONLY, m_nAutoMode);//liub modify
    DDX_Control(pDX, IDC_TAB_NETWORK_SETTINGS, m_tabHistNW);//liub_add 1027
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetupNWSel, CBaseDialog)
    //{{AFX_MSG_MAP(CDlgSetupNWSel)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_NETWORK_SETTINGS, OnSelchangeTabHistNW)
//	ON_BN_CLICKED(IDC_RADIO_SETUP_NWSEL_AUTOSEL, OnRadioSetupNwselAutosel)//liub modify
//	ON_BN_CLICKED(IDC_RADIO_SETUP_NWSEL_MANSEL, OnRadioSetupNwselMansel)//liub modify
//	ON_BN_CLICKED(IDC_BUTTON_SETUP_NWSEL_REFRESH, OnButtonSetupNwselRefresh)//liub modify
//	ON_MESSAGE(WM_SETUP_UPDATE_NWSELDLG, OnUpdateNWSelDlg)//liub modify
//	ON_BN_CLICKED(IDC_BUTTON_SETUP_NWSEL_SET, OnButtonSetupNwselSet)//liub modify
//	ON_BN_CLICKED(IDC_BUTTON_SETUP_NWSEL_SELECT, OnButtonSetupNwselSelect)//liub modify
//	ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupNWSel message handlers
void CDlgSetupNWSel::OnCancel()
{
    // TODO: Add extra cleanup here
    CBaseDialog::OnCancel();
}

void CDlgSetupNWSel::OnOK()
{
    // TODO: Add extra cleanup here
//	CBaseDialog::OnOK();
}

BOOL CDlgSetupNWSel::OnInitDialog()
{
    CBaseDialog::OnInitDialog();

    m_dlgSetupNWSelMode.Create (IDD_DIALOG_SETUP_NW_MODE,&m_tabHistNW);//add by liub_1027
    m_dlgSetupNWSelSearch.Create(IDD_DIALOG_SETUP_NW_SEARCH,&m_tabHistNW);

    m_dlgSetupNWSelSearch.p_dlgSetupNWSelMode = &m_dlgSetupNWSelMode;
    m_dlgSetupNWSelMode.m_nSelMode = m_dlgSetupNWSelSearch.m_nSelMode;
    InitTabCtrl();//liub_add 1027
    CRect r;
    m_tabHistNW.GetClientRect (&r);
    m_dlgSetupNWSelMode.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_SHOWWINDOW);
    m_dlgSetupNWSelSearch.SetWindowPos(NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);




    /*
        //表格列数
        int cols=4;

        int arr_cols[]=
    	{
    		IDS_NWLIST_STAT,
    		IDS_NWLIST_NAME,
    		IDS_NWLIST_ACT,
    		IDS_NWLIST_NUMBER,
    	};
    	int arr_width[]=
    	{
    		80,
    		150,
    		95,
    		110,
    	};
    	// TODO: Add extra initialization here
    	m_lstNW.SetParent(this);
        //CRect rect;
        //m_tabSms.GetItemRect(0, &rect);

        //m_lstNW.SetWindowPos(NULL, 4, 28, rect.right, rect.bottom, TRUE);
        //m_lstNW.GetWindowRect(&rect);
        CString strColTitle;
        //设置列名
        for(int i=0; i<2; i++)
        {
            strColTitle.LoadString(arr_cols[i]);
            m_lstNW.InsertColumn(i, strColTitle, LVCFMT_LEFT, arr_width[i]);
        }

    	//ONDA del IDS_NWLIST_NUMBER
    	if (g_SetData.Main_nCompany==COMP_ONDA)
    	{
    		strColTitle.LoadString(arr_cols[3]);
    		m_lstNW.InsertColumn(2, strColTitle, LVCFMT_LEFT, arr_width[3]);
    	}
    	else
    	{
    		strColTitle.LoadString(arr_cols[2]);
    		m_lstNW.InsertColumn(2, strColTitle, LVCFMT_LEFT, arr_width[2]);
    		strColTitle.LoadString(arr_cols[3]);
    		m_lstNW.InsertColumn(3, strColTitle, LVCFMT_LEFT, arr_width[3]);
    	}

        m_lstNW.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    	m_lstNW.m_headerCtrl.DrawHeader();
    //	if (g_SetData.Main_nCompany==COMP_ONDA)
    	{
    		m_nMsgBeat=0;
    		pdlgMsg = new CDlgMsg(this);
    	}
    	InitData();*/
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSetupNWSel::InitTabCtrl()
{
    CString strTabCaption;
    //strTabCaption.LoadString(IDS_CALLHIST_MISSED);
    m_tabHistNW.InsertItem(0, _T("Mode Preference"), 0);
    // strTabCaption.LoadString(IDS_CALLHIST_RECEIVED);
    m_tabHistNW.InsertItem(1, _T("Registration Mode"), 1);

    DWORD dwFlags = 0;
    dwFlags |= ETC_FLAT;
    dwFlags |= ETC_SELECTION;
    dwFlags |= ETC_GRADIENT;

    m_tabHistNW.SetBkgndColor(RGB(238,241,243));
    m_tabHistNW.EnableDraw(BTC_ALL);

    CEnTabCtrl::EnableCustomLook(dwFlags, dwFlags);




}

void CDlgSetupNWSel::OnSelchangeTabHistNW(NMHDR* pNMHDR, LRESULT* pResult)
{
    CRect r;
    m_tabHistNW.GetClientRect (&r);
    switch(m_tabHistNW.GetCurSel()) {
    case 0:
        m_dlgSetupNWSelMode.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_SHOWWINDOW);
        m_dlgSetupNWSelSearch.SetWindowPos(NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
        m_dlgSetupNWSelMode.m_nSelMode = m_dlgSetupNWSelSearch.m_nSelMode;
        m_dlgSetupNWSelMode.nCOPSType = m_dlgSetupNWSelSearch.nCOPSType;
        m_dlgSetupNWSelMode.bSetNWSucc = m_dlgSetupNWSelSearch.bSetNWSucc;




        break;
    case 1:
        m_dlgSetupNWSelSearch.SetWindowPos(NULL,0,25,r.right,r.bottom,SWP_SHOWWINDOW);
        m_dlgSetupNWSelMode.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
        break;


    }

    *pResult = 0;


}
