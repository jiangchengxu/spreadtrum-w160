// CTransferDlg.cpp : implementation file
/*=================================================================
FILE：   CTransferDlg.cpp                             VERSION : 


DESCRIPTION
  实现电话薄数据转储功能。PC，DATACARD，USIM三端之间的部分和全部的复制或移动功能。


=================================================================*/

/*=================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
 Notice that changes are listed in reverse chronological order.

 when        who         what, where, why
 --------   -----       ---------------------------------------------
 
=================================================================*/
//Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明：
	/*
	CTransferDlg::AtRespReadState函数：初始化时读取DataCard和Usim存储器记录时的回调函数。
	CTransferDlg::AtRespWriteRecordState函数：向DataCard或Usim存储器写电话薄记录或删除电话薄记录时的回调函数。
	CTransferDlg::AtWriteARecord函数：向DataCard或Usim存储器写电话薄记录或删除电话薄记录。
	CTransferDlg::DataCardToUsim函数：用于处理DataCard至Usim存储器或Usim至DataCard存储器的复制和移动操作。
	CTransferDlg:: DataCradOrUsimToPc函数：用于处理DataCard至Pc存储器或者Usim至PC端存储器的复制和移动操作。
	CTransferDlg:: GetFirstIdleIndex函数：用于查找DataCrad或Usim存储器中是否还有未用存储位置。
	CTransferDlg::InitListCtrlDCAndCard函数：用于初始化DataCrad或Usim存储器的ListCtrl控件。
	CTransferDlg::InitListCtrlPC函数：用于初始化PC存储器的ListCtrl控件。
	CTransferDlg::MallocDlgMem函数：用于new一个的CwaitDlg。
	CTransferDlg::OnButtonCopyAll函数：响应拷贝所有的电话本记录。由上部的ListCtrl控件至下部的ListCtrl控件。
	CTransferDlg:: OnButtonCopyAllUp函数：响应拷贝所有的电话本记录。由下部的ListCtrl控件至上部的ListCtrl控件。
	CTransferDlg::OnButtonCutAll函数：响应移动所有的电话本记录。由上部的ListCtrl控件至下部的ListCtrl控件。
	CTransferDlg::OnButtonCutAllUp函数：响应移动所有的电话本记录。由下部的ListCtrl控件至上部的ListCtrl控件。
	CTransferDlg::OnButtonCopySome函数：响应拷贝一条或部分的电话本记录。由上部的ListCtrl控件至下部的ListCtrl控件。
	CTransferDlg::OnButtonCopySomeUp函数：响应拷贝一条或部分的电话本记录。由下部的ListCtrl控件至上部的ListCtrl控件。
	CTransferDlg::OnButtonCutSome函数：响应移动一条或部分的电话本记录。由上部的ListCtrl控件至下部的ListCtrl控件。
	CTransferDlg::OnButtonCutSomeUp函数：响应移动一条或部分的电话本记录。由下部的ListCtrl控件至上部的ListCtrl控件。
	CTransferDlg::PcToDataCradOrUsim函数：用于处理PC至DATACARD或PC至USIM存储器的复制和移动操作。
	CTransferDlg::ReadDataFromDCOrCard函数：用于在初始化时读取DATACARD和USIM存储器中的电话薄记录。
	CTransferDlg:: UpdateButtonStatus函数：因8个拷贝和移动按钮两两合并在一起的（如Copysome和CopySomeUP）根据用户在上部或下部的ListCtrl选择了电话薄记录，
	                                       只显示其中一个，另一个隐藏。如未选择任何电话薄记录，则将按钮置灰.
	*/


#include "stdafx.h"
#include "HSDPA.h"
#include "WaitDlg.h"
#include "TransferDlg.h"
#include "PbSelPositon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define IDT_CPBR_READ_ALL 500

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg dialog

CTransferDlg::CTransferDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTransferDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTransferDlg)
        // NOTE: the ClassWizard will add member initialization here
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    m_pDlg = NULL;
    //}}AFX_DATA_INIT
   // m_nBkTag = 1;

    //pUSIMPbRecord = NULL;
    //pDataCardPbRecord = NULL;
    m_nPbSelPosDlgFlag = 0;
    m_nPbSelPosDlgSelFlag = 1;

    m_nCurSelCtrl = 0;
    m_nUp = 3;
    m_nStep = 1;

    m_nSelTotalItem = 0;
    m_nCurItem = 0;
	m_HsdpaDlg = (CHSDPADlg*)AfxGetMainWnd();
	m_bEndPbTransfer = false;
	
	m_nStoreFlag = 0;
	m_ReadDataFromDCState = 0;
	m_StrSource = _T("");
	m_nIndex = 2;
	m_ReadDataCardAndUSIMStatus = false;
	m_PBSelect=TRUE;
}

CTransferDlg::~CTransferDlg()
{
	/*
    if (NULL != pUSIMPbRecord)
    {
        delete []pUSIMPbRecord;
    }

    if (NULL != pDataCardPbRecord)
    {
        delete []pDataCardPbRecord;
    }
	*/

    if (NULL != m_pDlg)
    {
        delete m_pDlg;
    }
	::SetEvent(g_BGPassEvt);
}

void CTransferDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTransferDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
    DDX_Control(pDX, IDC_PIC_UP, m_StaUp);
    DDX_Control(pDX, IDC_PIC_DOWN, m_StaDown);
    DDX_Control(pDX, IDC_LIST_PC, m_lstPhoneBookUP);
    DDX_Control(pDX, IDC_LIST_CARD, m_lstPhoneBookDOWN);
    DDX_Control(pDX, IDC_COMBO_SELUP, m_ComboSelUp);
    DDX_Control(pDX, IDC_COMBO_SELDOWN, m_ComboSelDown);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTransferDlg, CDialog)
    //{{AFX_MSG_MAP(CTransferDlg)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_COPY_SOME, OnButtonCopySome)
    ON_BN_CLICKED(IDC_BUTTON_COPY_SOME_UP, OnButtonCopySomeUp)
    ON_BN_CLICKED(IDC_BUTTON_CUT_SOME, OnButtonCutSome)
    ON_BN_CLICKED(IDC_BUTTON_CUT_SOME_UP, OnButtonCutSomeUp)
    ON_CBN_SELCHANGE(IDC_COMBO_SELDOWN, OnSelchangeComboSeldown)
    ON_CBN_SELCHANGE(IDC_COMBO_SELUP, OnSelchangeComboSelup)
    ON_NOTIFY(NM_CLICK, IDC_LIST_PC, OnClickListUp)
    ON_NOTIFY(NM_CLICK, IDC_LIST_CARD, OnClickListDown)
    ON_BN_CLICKED(IDC_BUTTON_COPY_ALL, OnButtonCopyAll)
    ON_BN_CLICKED(IDC_BUTTON_COPY_ALL_UP, OnButtonCopyAllUp)
    ON_BN_CLICKED(IDC_BUTTON_CUT_ALL, OnButtonCutAll)
    ON_BN_CLICKED(IDC_BUTTON_CUT_ALL_UP, OnButtonCutAllUp)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_PC, OnDblclkListUp)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_CARD, OnDblclkListDown)
    //}}AFX_MSG_MAP
	ON_MESSAGE(WM_PB_ProgressOpen, ProgressOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg message handlers
BOOL CTransferDlg::OnInitDialog() 
{
     CDialog::OnInitDialog();
    // TODO: Add extra initialization here
//     m_bFirst = TRUE;
// 
// 	SetSkin(g_SetData.Setup_nSkinStyle);

	m_Hwnd = GetSafeHwnd();
	
	CString StrSim;
	if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
		(wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0))
	{
		StrSim.LoadString(IDS_STATIC_USIM);
		GetDlgItem(IDC_STATIC_USIM)->SetWindowText(StrSim);	
	}
	else if((wcscmp(g_SetData.Setup_sz3GType,_T("CDMA2000"))==0)||
		(wcscmp(g_SetData.Setup_sz3GType,_T("cdma2000"))==0))
	{

		StrSim.LoadString(IDS_STATIC_RUIM);
		GetDlgItem(IDC_STATIC_USIM)->SetWindowText(StrSim);	
	}
    CString CSelUP;
    CString CSelDown;
    CSelUP.LoadString(IDS_PC);
    m_ComboSelUp.AddString(CSelUP);
    CSelUP.LoadString(IDS_DATACARD);
     m_ComboSelUp.AddString(CSelUP);
	#ifndef FEATURE_VERSION_NOSIM
	     m_ComboSelUp.AddString(StrSim);
	#endif

    CSelDown.LoadString(IDS_PC);
    m_ComboSelDown.AddString(CSelDown);
	CSelDown.LoadString(IDS_ME);
    m_ComboSelDown.AddString(CSelDown);
    #ifndef FEATURE_VERSION_NOSIM
		m_ComboSelDown.AddString(StrSim);
	#endif
    
        
    m_ComboxCurIndexUp = 0;    
    m_ComboSelUp.SetCurSel(m_ComboxCurIndexUp);

  
	m_ComboxCurIndexDown = 1; //USB Modem
		m_ComboSelDown.SetCurSel(m_ComboxCurIndexDown);
	

	InitListCtrlUp(0);
	ReDrawListCtrlUp(0);
	
	::ResetEvent(g_BGPassEvt);  //设置读取记录时来电或短信，需等待读取结束的标记
#ifndef FEATURE_VERSION_NOSIM
	if(!gbSimInit)
	{
		m_ReadDataCardAndUSIMStatus = true;
			ReadDataFromDCOrCard(1);
	
		if (!m_ReadDataCardAndUSIMStatus)
		{
			OnCancel();
			//AfxMessageBox("数据读取失败");  //debug

			::SetEvent(g_BGPassEvt);  //恢复进行来电来短信时需等待的标记
			
			return FALSE;
		}
		gbSimInit = true;
	}
#endif

	if(!gbPcCardInit)
	{
		m_ReadDataCardAndUSIMStatus = true;
		ReadDataFromDCOrCard(0);
		if (!m_ReadDataCardAndUSIMStatus)
		{
			OnCancel();
			//AfxMessageBox("数据读取失败");  //debug

			::SetEvent(g_BGPassEvt);  
			
			return FALSE;

		}
		gbPcCardInit = true;
	}
	::SetEvent(g_BGPassEvt);  

//     if (NULL == pUSIMPbRecord)
//     {
//         OnCancel();
//         //AfxMessageBox("数据读取失败"); //debug
// 		
//         return FALSE;
//     }

	InitListCtrlDown(1);
	ReDrawListCtrlDown(1);

    m_nCurSelCtrl = 0;
    UpdateButtonStatus();
        
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTransferDlg::UpdateButtonStatus()
{
    switch (m_nCurSelCtrl)
    {
        case 1:
        {
            GetDlgItem(IDC_BUTTON_COPY_SOME_UP)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BUTTON_COPY_SOME)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_COPY_SOME)->EnableWindow(true);

            GetDlgItem(IDC_BUTTON_CUT_SOME_UP)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BUTTON_CUT_SOME)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_CUT_SOME)->EnableWindow(true);

            GetDlgItem(IDC_BUTTON_COPY_ALL_UP)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BUTTON_COPY_ALL)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_COPY_ALL)->EnableWindow(true);

            GetDlgItem(IDC_BUTTON_CUT_ALL_UP)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BUTTON_CUT_ALL)->ShowWindow(SW_NORMAL);    
            GetDlgItem(IDC_BUTTON_CUT_ALL)->EnableWindow(true);    

            m_StaUp.ShowWindow(SW_HIDE);
            m_StaDown.ShowWindow(SW_SHOW);

			m_Progress.ShowWindow(SW_HIDE);

            break;
        }

        case 2:
        {
            GetDlgItem(IDC_BUTTON_COPY_SOME)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BUTTON_COPY_SOME_UP)->ShowWindow(SW_NORMAL);    
            GetDlgItem(IDC_BUTTON_COPY_SOME_UP)->EnableWindow(true);    

            GetDlgItem(IDC_BUTTON_CUT_SOME)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BUTTON_CUT_SOME_UP)->ShowWindow(SW_NORMAL);    
            GetDlgItem(IDC_BUTTON_CUT_SOME_UP)->EnableWindow(true);    

            GetDlgItem(IDC_BUTTON_COPY_ALL)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BUTTON_COPY_ALL_UP)->ShowWindow(SW_NORMAL);    
            GetDlgItem(IDC_BUTTON_COPY_ALL_UP)->EnableWindow(true);    

            GetDlgItem(IDC_BUTTON_CUT_ALL)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BUTTON_CUT_ALL_UP)->ShowWindow(SW_NORMAL);    
            GetDlgItem(IDC_BUTTON_CUT_ALL_UP)->EnableWindow(true);    
    
            m_StaDown.ShowWindow(SW_HIDE);
            m_StaUp.ShowWindow(SW_SHOW);

			m_Progress.ShowWindow(SW_HIDE);
        
            break;
        }
        case 0:  
        {  
            GetDlgItem(IDC_BUTTON_COPY_SOME)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_COPY_SOME)->EnableWindow(false);
            GetDlgItem(IDC_BUTTON_COPY_SOME_UP)->ShowWindow(SW_HIDE);

            GetDlgItem(IDC_BUTTON_CUT_SOME)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_CUT_SOME)->EnableWindow(false);
            GetDlgItem(IDC_BUTTON_CUT_SOME_UP)->ShowWindow(SW_HIDE);

            GetDlgItem(IDC_BUTTON_COPY_ALL)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_COPY_ALL)->EnableWindow(false);
            GetDlgItem(IDC_BUTTON_COPY_ALL_UP)->ShowWindow(SW_HIDE);

            GetDlgItem(IDC_BUTTON_CUT_ALL)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_CUT_ALL)->EnableWindow(false);
            GetDlgItem(IDC_BUTTON_CUT_ALL_UP)->ShowWindow(SW_HIDE);

            m_StaDown.ShowWindow(SW_HIDE);
            m_StaUp.ShowWindow(SW_HIDE);
            
			m_Progress.ShowWindow(SW_HIDE);

            break;
        }

        default:
        {
            break;
        }
    }
	::SetEvent(g_BGPassEvt);
}

void CTransferDlg::InitListCtrlUp(int flag)
{
    CMyListCtrl *ptMyListCtrl = &m_lstPhoneBookUP;
	ptMyListCtrl->DeleteAllItems();
    for (int index = ptMyListCtrl->GetColumnCount() - 1; index >= 0; index--)
    {
        ptMyListCtrl->DeleteColumn(index);
    }
	
    switch (flag)
    {
	case 0: //PC
        {
            //表格列数
			int cols = 4;
			int arr_cols[] = {IDS_NAME,IDS_MOBILE,IDS_COMPANY,IDS_HOME};
			
			CRect rect;
			 ptMyListCtrl->GetWindowRect(&rect);
			 CString strColTitle;
			 //设置列名
			 for(int i = 0;i < cols;i++)
			 {
				 strColTitle.LoadString(arr_cols[i]);
				 ptMyListCtrl->InsertColumn(i, strColTitle,LVCFMT_LEFT,rect.Width()/cols);
			}
			
            break;
        }
		
	default:
        {
            int cols = 2;
			int arr_cols[] = {IDS_NAME,IDS_MOBILE};
			
			CRect rect;
			 ptMyListCtrl->GetWindowRect(&rect);
			 CString strColTitle;
			 //设置列名
			 for(int i = 0;i < cols;i++)
			 {
				 strColTitle.LoadString(arr_cols[i]);
				 ptMyListCtrl->InsertColumn(i, strColTitle,LVCFMT_LEFT,rect.Width()/cols);
			}
			
            break;
        }
		
    }
	
    
    ptMyListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	ptMyListCtrl->m_headerCtrl.DrawHeader();
	
}

void CTransferDlg::InitListCtrlDown(int flag)
{
    CMyListCtrl *ptMyListCtrl = &m_lstPhoneBookDOWN;
	ptMyListCtrl->DeleteAllItems();
    for (int index = ptMyListCtrl->GetColumnCount() - 1; index >= 0; index--)
    {
        ptMyListCtrl->DeleteColumn(index);
    }
	
    switch (flag)
    {
	case 0: //PC
        {
            //表格列数
			int cols = 4;
			int arr_cols[] = {IDS_NAME,IDS_MOBILE,IDS_COMPANY,IDS_HOME};
			
			CRect rect;
			ptMyListCtrl->GetWindowRect(&rect);
			CString strColTitle;
			//设置列名
			for(int i = 0;i < cols;i++)
			{
				strColTitle.LoadString(arr_cols[i]);
				ptMyListCtrl->InsertColumn(i, strColTitle,LVCFMT_LEFT,rect.Width()/cols);
			}
			
            break;
        }
		
	default:
        {
            int cols = 2;
			int arr_cols[] = {IDS_NAME,IDS_MOBILE};
			
			CRect rect;
			ptMyListCtrl->GetWindowRect(&rect);
			CString strColTitle;
			//设置列名
			for(int i = 0;i < cols;i++)
			{
				strColTitle.LoadString(arr_cols[i]);
				ptMyListCtrl->InsertColumn(i, strColTitle,LVCFMT_LEFT,rect.Width()/cols);
			}
			
            break;
        }
		
    }
	
    
    ptMyListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	ptMyListCtrl->m_headerCtrl.DrawHeader();
	
}


/*导入上面ListCtrl的电话薄记录*/
void CTransferDlg::ReDrawListCtrlUp(int flag )
{
    CMyListCtrl *ptMyListCtrl = &m_lstPhoneBookUP;
	 ptMyListCtrl->DeleteAllItems();
	
    switch (flag)
    {
	case 0: //PC
        {
            //填入数据
			int nInedx = 0;
			for(int i = 0; i < PB_RECORD_MAX; i++)
			{
				if(-1 != m_pPbData->PbDatas[i].index)
				{
					ptMyListCtrl->InsertItem(nInedx, m_pPbData->PbDatas[i].strName, (DWORD)(&m_pPbData->PbDatas[i]));
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, m_pPbData->PbDatas[i].strMobile);
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, m_pPbData->PbDatas[i].strCompany);
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, m_pPbData->PbDatas[i].strHome);
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, m_pPbData->PbDatas[i].strEmail);
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, m_pPbData->PbDatas[i].strMark);
					nInedx ++;
				}
			}    
			
            break;
        }
	case 1: //USB Modem
        {
            //填入数据
			int nIndex = 0;
			for(int i = 0; i < g_DataCardTotalNum; i++)
			{
				if(-1 != pDataCardPbRecord[i].index)
				{
					int index = ptMyListCtrl->InsertItem(nIndex, pDataCardPbRecord[i].strName,
						(DWORD)(&pDataCardPbRecord[i]));
					ptMyListCtrl->SetItemText(nIndex, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE, 
						pDataCardPbRecord[i].strMobile);
					nIndex ++;
				}
			}    
            
            break;
        }
	case 2: //USIM
		{
			//填入数据
			int nIndex = 0;
			for(int i = 0; i < g_USimTotalNum; i++)
			{
				if(-1 != pUSIMPbRecord[i].index)
				{
					int index = ptMyListCtrl->InsertItem(nIndex, pUSIMPbRecord[i].strName,
						(DWORD)(&pUSIMPbRecord[i]));
					ptMyListCtrl->SetItemText(nIndex, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE, 
						pUSIMPbRecord[i].strMobile);
					nIndex ++;
				}
			}    
			break;
		}

	default:
        {
            return;
        }
    }
	
   
    
}

void CTransferDlg::ReDrawListCtrlDown(int flag )
{
    CMyListCtrl *ptMyListCtrl = &m_lstPhoneBookDOWN;
	ptMyListCtrl->DeleteAllItems();
	
    switch (flag)
    {
	case 0: //PC
        {
            //填入数据
			int nInedx = 0;
			for(int i = 0; i < PB_RECORD_MAX; i++)
			{
				if(-1 != m_pPbData->PbDatas[i].index)
				{
					ptMyListCtrl->InsertItem(nInedx, m_pPbData->PbDatas[i].strName, (DWORD)(&m_pPbData->PbDatas[i]));
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, m_pPbData->PbDatas[i].strMobile);
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, m_pPbData->PbDatas[i].strCompany);
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, m_pPbData->PbDatas[i].strHome);
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, m_pPbData->PbDatas[i].strEmail);
					ptMyListCtrl->SetItemText(nInedx, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, m_pPbData->PbDatas[i].strMark);
					nInedx ++;
				}
			}    
			
            break;
        }
	case 1: //USB Modem
        {
            //填入数据
			int nIndex = 0;
			for(int i = 0; i < g_DataCardTotalNum; i++)
			{
				if(-1 != pDataCardPbRecord[i].index)
				{
					int index = ptMyListCtrl->InsertItem(nIndex, pDataCardPbRecord[i].strName,
						(DWORD)(&pDataCardPbRecord[i]));
					ptMyListCtrl->SetItemText(nIndex, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE, 
						pDataCardPbRecord[i].strMobile);
					nIndex ++;
				}
			}    
            
            break;
        }
	case 2: //USIM
		{
			//填入数据
			int nIndex = 0;
			for(int i = 0; i < g_USimTotalNum; i++)
			{
				if(-1 != pUSIMPbRecord[i].index)
				{
					int index = ptMyListCtrl->InsertItem(nIndex, pUSIMPbRecord[i].strName,
						(DWORD)(&pUSIMPbRecord[i]));
					ptMyListCtrl->SetItemText(nIndex, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE, 
						pUSIMPbRecord[i].strMobile);
					nIndex ++;
				}
			}    
			break;
		}
		
	default:
        {
            return;
        }
    }
	
    
}



/*从Datacard和Usim存储器中读取电话薄记录至内存中*/
void CTransferDlg::ReadDataFromDCOrCard(int flag)
{
    Sleep(100);
    m_nIndex = 2;

	BOOL bResponse = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->CommIsReady();
	if (!bResponse)
	{
		m_ReadDataCardAndUSIMStatus = false;
		return ;
	}
	
	MallocDlgMem();    
    m_nStoreFlag = flag;
    CString StrSource = _T("AT+CPBS=\"SM\"");
    switch (flag)
    {
        case 0:
        {
            StrSource = _T("AT+CPBS=\"ME\"");
            m_pDlg->m_strPrompt.LoadString(IDS_PBTRANSFER_INITCARDINFO);

            break;
        }
        case 1:
        {
            StrSource = _T("AT+CPBS=\"SM\"");
            m_pDlg->m_strPrompt.LoadString(IDS_PBTRANSFER_INITUSIMINFO);

            break;
        }
        
        default:
        {
            return ;
        }
    }
    m_ReadDataFromDCState = SEND_AT_SEL_STORE;
    TCHAR szAtBuf[512];

    memset(szAtBuf,0,sizeof(TCHAR)*512);

    char szAtAnsBuf[512];
            memset(szAtAnsBuf,0,sizeof(char)*512);
            wcscpy(szAtBuf,StrSource);
            szAtBuf[wcslen(szAtBuf)] = 0x000D;  
            szAtBuf[wcslen(szAtBuf)] = 0; 
            WCharToChar(szAtBuf,szAtAnsBuf);

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtAnsBuf, wcslen(szAtBuf)))
    {        
        SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE, 30000, NULL);        
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, this);    

        m_pDlg->DoModal(); 
		
		Sleep(10);
    }
    else 
    {
		m_pDlg->m_bSuccess = false;
        m_pDlg->SendMessage(WM_CLOSE, 0, 0);
		//AfxMessageBox("Write to port failed! ");//Debug info
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);	
		AddPrefixName(strAppName);
		CString strTransfail;
		strTransfail.LoadString(IDS_PBTRANSFER_FAILED);
		//AfxMessageBox(strTransfail);
		::MessageBox(m_Hwnd,strTransfail,strAppName,MB_OK);
        m_ReadDataCardAndUSIMStatus = false;        
    }
}


/*读取Datacard和Usim存储器中电话薄记录时注册的回调函数，用于处理底层返回的结果和向底层发送后续的AT命令*/
void CTransferDlg::AtRespReadState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    int Len = 512;
    char szAtBuf[512];
    char buf[10240] = {0};
    CTransferDlg* pDlg = (CTransferDlg*)pWnd;
    CSerialPort* pComm;
    CString buffer = _T(""); 
    char *pbuf;

    switch(pDlg->m_ReadDataFromDCState)
    {
		//选择存储器    
        case SEND_AT_SEL_STORE:                                       
        {
            pDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE);
            if (0 == wStrNum) 
            {
                pDlg->m_ReadDataCardAndUSIMStatus = false;
                pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
                //AfxMessageBox("选择存储器失败");  //debug

                return;
            }
            for(int Nline = 0; Nline < wStrNum;Nline++)
            {
                CString strResult = strArr[Nline];
    
                if (0 == strResult.CompareNoCase(_T("ERROR")))
                {
                    pDlg->m_ReadDataCardAndUSIMStatus = false;
                    pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
                    //AfxMessageBox("选择存储器失败");  //debug

                    return;
                }
            }    
            memset(szAtBuf,0,sizeof(char)*Len);
            strcpy(szAtBuf,"AT+CPBS?");
            szAtBuf[strlen(szAtBuf)] = 0x0D;
            szAtBuf[strlen(szAtBuf)] = 0;
            pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
            ASSERT(pComm);
            if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
            {        
                pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_CHECK_TOTAL, 30000, NULL);
                RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd);
				pDlg->m_ReadDataFromDCState = SEND_STORE_CHECK_TOTAL;
            }
			else
			{
				pDlg->m_ReadDataCardAndUSIMStatus = false;
				pDlg->m_pDlg->m_bSuccess = false;
				pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
			}            
            
            break;
        }

        //获取STORE中的已使用条数和最大条数
        case SEND_STORE_CHECK_TOTAL:
        {
            pDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_CHECK_TOTAL);
            if (0 == wStrNum) 
            {
                pDlg->m_ReadDataCardAndUSIMStatus = false;
                pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
                //AfxMessageBox("获取STORE中的最大条数失败");  //debug
                
                return;
            }    
            for(int Nline = 0; Nline < wStrNum; Nline ++)
            {
                CString strResult = strArr[Nline];
    
                if (0 == strResult.CompareNoCase(_T("ERROR")))
                {
                    pDlg->m_ReadDataCardAndUSIMStatus = false;
                    pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
                    //AfxMessageBox("获取STORE中的最大条数失败");   //debug    
					
                    return;
                }
            }
            //格式：AT+CPBS: "SM",1,200
            int from = 0;
			int to = 0;
            char temp[4];
            memset(temp,0,sizeof(char)*4);

            buffer = (char *)strArr[0];
            pbuf = (char *)strArr[0];

            //得到总条数
            from = buffer.ReverseFind(',')+1;
            to = buffer.GetLength() - 1;
            memcpy(temp,pbuf+from,(to - from + 1));
            int nStoreTotalNumer = atoi(temp);

			//得到已用条数
			memset(temp,0,sizeof(char)*4);
			CString strTemp = buffer.Left(from - 1);
            from = strTemp.ReverseFind(',')+1;
            to = strTemp.GetLength() - 1;
            memcpy(temp,pbuf+from,(to - from + 1));
            int nStoreUserNumer = atoi(temp);

			if(0 != nStoreUserNumer)
			{
				if (0 == pDlg->m_nStoreFlag) //DataCard存储器存放电话薄数据的内存进行初始化
				{
					g_DataCardUsedNum = nStoreUserNumer;
				}
				else if (1 == pDlg->m_nStoreFlag)
				{
					g_USimUsedNum = nStoreUserNumer;
				}

			}

			if (0 != nStoreTotalNumer)
			{
				if (0 == pDlg->m_nStoreFlag) //DataCard存储器存放电话薄数据的内存进行初始化
				{                
					g_DataCardTotalNum = nStoreTotalNumer;

					if (NULL != pDataCardPbRecord)
					{
						delete []pDataCardPbRecord;
					}

					pDataCardPbRecord = new StPbRecord[g_DataCardTotalNum];
					memset(pDataCardPbRecord,0,(sizeof(StPbRecord) * g_DataCardTotalNum));

					for (int i = 0;i < g_DataCardTotalNum; i++)
					{
						pDataCardPbRecord[i].index = -1;
					}

					pDlg->m_pDlg->SetProgressRange(0,g_DataCardTotalNum,1);                
				}
				else if (1 == pDlg->m_nStoreFlag)  //Usim存储器存放电话薄数据的内存进行初始化
				{                
					g_USimTotalNum = nStoreTotalNumer;

					if (NULL != pUSIMPbRecord)
					{
						delete []pUSIMPbRecord;
					}

					pUSIMPbRecord = new StPbRecord[g_USimTotalNum];
					memset(pUSIMPbRecord,0,(sizeof(StPbRecord) * g_USimTotalNum));

					for (int i = 0; i < g_USimTotalNum; i++)
					{
						pUSIMPbRecord[i].index = -1;
					}

					pDlg->m_pDlg->SetProgressRange(0,g_USimTotalNum,1);
				}
			}
            else
            {               
                g_DataCardTotalNum = 0;                
                g_USimTotalNum = 0;

				pDlg->m_ReadDataCardAndUSIMStatus = false;
				pDlg->m_pDlg->m_bSuccess = false;
				pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);

				break;
            }			
            memset(szAtBuf,0,sizeof(char)*Len);
            strcpy(szAtBuf,"AT+CPBR=1");            
            szAtBuf[strlen(szAtBuf)] = 0x0D;
            szAtBuf[strlen(szAtBuf)] = 0;
            pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
            ASSERT(pComm);
            if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
            {        
                pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL, 30000, NULL);
                RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd);  
				pDlg->m_ReadDataFromDCState = SEND_STORE_READ_ALL;
            }
			else
			{
				pDlg->m_ReadDataCardAndUSIMStatus = false;
				pDlg->m_pDlg->m_bSuccess = false;
				pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
			}             
                
            break;
        }

		//获取存储器中电话本信息
        case SEND_STORE_READ_ALL:
        {
            pDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL);
            
            if (0 == wStrNum) 
            {
                pDlg->m_ReadDataCardAndUSIMStatus = false;
                pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
                //AfxMessageBox("获取存储器中电话本信息失败");  //Debug

                return;
            }
            
            for(int Nline = 0; Nline < wStrNum;Nline++)
            {
                CString strResult = strArr[Nline];
    
                if (0 == strResult.CompareNoCase(_T("ERROR")))
                {
                    pDlg->m_ReadDataCardAndUSIMStatus = false;
                    pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
                    //AfxMessageBox("获取存储器中电话本信息失败");  //Debug
                    
                    return;
                }
            }        
            
            StPbRecord *pStDCPbRecord = NULL;
            int nStoreTotalNum = 0;

            if (0 == pDlg->m_nStoreFlag)
            {                
                pStDCPbRecord = pDataCardPbRecord;
                nStoreTotalNum = g_DataCardTotalNum;                
            }

            else if (1 == pDlg->m_nStoreFlag)
            {                
                pStDCPbRecord = pUSIMPbRecord;
                nStoreTotalNum = g_USimTotalNum;            
            }

            if (wStrNum == 2)
            {                   
                CString strResult = strArr[wStrNum - 1];

				//返回格式为：+CPBR: 1,"123",129,"test"
                if (0 == strResult.CompareNoCase(_T("OK")))
                {
                    CString strTemp = (char *)strArr[0];
                    
					//获取电话号码的index
                    int nIndexFrom = strTemp.Find(':');
                    int nIndexTo = strTemp.Find(',');
                    CString strIndex = strTemp.Mid(nIndexFrom + 1, nIndexTo - nIndexFrom - 1);
                    strIndex.TrimLeft();
                    strIndex.TrimRight();
                    int nIndex = _wtoi(strIndex);
                    pStDCPbRecord[nIndex - 1].index = nIndex;

					//获取电话号码
					int nNumFrom = strTemp.Find('\"');  //=2
					int nNumTo = strTemp.Find('\"',nNumFrom + 1);  
					CString strNum = strTemp.Mid(nNumFrom + 1, nNumTo - nNumFrom - 1);
					strNum.TrimLeft();
                    strNum.TrimRight();
					memcpy(pStDCPbRecord[nIndex - 1].strMobile, strNum, PB_NUM_MAX);

					//获取姓名
					strTemp.Delete(strTemp.GetLength() - 1, 1);
					int nNameFrom = strTemp.ReverseFind('\"');
					CString strName = strTemp.Mid(nNameFrom + 1, (strTemp.GetLength() - 1));
					strName.TrimLeft();
                    strName.TrimRight();
					if (_T("") != strName)
					{
						strName = UCS2ToGB(strName);
					}
					else
					{
						strName.Format(IDS_UNKNOWN_NAME);
					}
					memcpy(pStDCPbRecord[nIndex - 1].strName, strName, PB_NAME_MAX);
                }
            }
                
            char  StrIndex[4];

            if (pDlg->m_nIndex <= nStoreTotalNum)
            {
				memset(StrIndex,0,sizeof(char) * 4);
                _itoa( pDlg->m_nIndex, StrIndex, 10 );

				pDlg->m_nIndex++;
				pDlg->m_pDlg->m_ProgressWait.StepIt();

				memset(szAtBuf,0,sizeof(char)*Len);
                strcpy(szAtBuf,"AT+CPBR=");
                strcat(szAtBuf, StrIndex);
                szAtBuf[strlen(szAtBuf)] = 0x0D;
                szAtBuf[strlen(szAtBuf)] = 0;
                pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
                ASSERT(pComm);
                if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
                {  					
                    pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL, 30000, NULL);
                    RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd); 
					pDlg->m_ReadDataFromDCState = SEND_STORE_READ_ALL;
                }
				else
				{
					pDlg->m_ReadDataCardAndUSIMStatus = false;
					pDlg->m_pDlg->m_bSuccess = false;
					pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
				}				
            }
            else
            {                
                pDlg->m_pDlg->ProgressCompleted(pDlg->m_pDlg->m_nUpper);
                pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
				pDlg->m_ReadDataFromDCState = 0;
            }            
            
            break;
        }
        default:
        {
			//AfxMessageBox("m_ReadDataFromDCState status error!"); //debug info

            break;
        }
    }

}

void CTransferDlg::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default
	this->EnableWindow(true);
	UpdateButtonStatus();
	switch(nIDEvent)
	{
		case TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE:
		{
			KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE);
			m_ReadDataCardAndUSIMStatus = false;
			//AfxMessageBox("Select Store TimeOut!"); //debug info
			OnCancel();

			break;
		}
		case TIMER_ID_ATRESP_TIMEOUT_CPBR_CHECK_TOTAL:
		{
			KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_CHECK_TOTAL);
			m_ReadDataCardAndUSIMStatus = false;
			//AfxMessageBox("Check Total TimeOut!");   //debug info     			
			OnCancel();

			break;
		}
		case TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL:
		{
			KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL);
			m_ReadDataCardAndUSIMStatus = false;
			//AfxMessageBox("Read All TimeOut!");      //debug info  
			int nindex = m_nIndex;

			OnCancel();

			break;
		}
		case TIMER_ID_ATRESP_TIMEOUT_CPBR_SET_STORE:
		{
			KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_SET_STORE);
			//AfxMessageBox("Set store TimeOut!");        //debug info

			break;
		}
		case TIMER_ID_ATRESP_TIMEOUT_CPBW_WRITE_RECORD:
		{
			KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBW_WRITE_RECORD);
			//AfxMessageBox("Write a Record TimeOut!");     //debug info   

			break;
		}
		default:
		{
			break;
		}
	}
	DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
	((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
    if (NULL != m_pDlg)
    {
        m_pDlg->m_bSuccess = false;
        m_pDlg->PostMessage(WM_CLOSE,0,0);
    }

	//AfxMessageBox(IDS_USB_PORT_TIMEOUT);

	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);
	AddPrefixName(strAppName);
	CString strTransTimeout;
	strTransTimeout.LoadString(IDS_USB_PORT_TIMEOUT);
	::MessageBox(m_Hwnd,strTransTimeout,strAppName,MB_OK);

	::SetEvent(g_BGPassEvt);

    CDialog::OnTimer(nIDEvent);
}


void CTransferDlg::OnButtonCopySome() 
{
    // TODO: Add your control notification handler code here
        //单击若干联系人，点击CopyAll；或者双击某一联系人。响应CopyAll事件
    m_nCurSelCtrl = 0;
    POSITION pos = m_lstPhoneBookUP.GetFirstSelectedItemPosition();
    if (!pos)
    {
        UpdateButtonStatus();
        return;
    }

    int nItem = -1;
    m_nSelTotalItem = m_lstPhoneBookUP.GetSelectedCount();
    m_nCurItem      = 0;

	this->EnableWindow(false);
	ProgressOpen(m_nSelTotalItem,1);  
		CWnd*  cWnd = GetParent();
		m_PBSelect=FALSE;

    switch (m_ComboxCurIndexUp)
    {
		case 0: //PC
        {			
            while (pos)
            {
                m_nCurItem++;
                nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);
				bool status;
				switch(m_ComboxCurIndexDown)
				{
				 case 1: //PC to USB Modem
					 status = PcToDataCradOrUsim(0,nItem,0,0);
					 break;
				 case 2: //PC to USIM
					 status = PcToDataCradOrUsim(1,nItem,0,0);
					 break;
				}
                if (!status)
                {
					this->EnableWindow(true);
					UpdateButtonStatus();

					return;
                }
			
				cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
				DispatchMsg();
				m_Progress.SetPos(m_nCurItem);
            }		
			ProgressClose(m_nSelTotalItem);
		
            break;
        }
		case 1: //USB Modem 
        {
            switch (m_ComboxCurIndexDown)
            {
				case 0:  //USB Modem to PC
                {            
                    while (pos)
                    {
                        m_nCurItem++;
                        nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);

                        bool status = DataCradOrUsimToPc(0,nItem,0,0);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            m_nPbSelPosDlgFlag = 0;

                            return;
                        }
				//		CWnd*  cWnd = GetParent();
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

						DispatchMsg();

						m_Progress.SetPos(m_nCurItem);
                    }  
					ProgressClose(m_nSelTotalItem);

                    m_nPbSelPosDlgFlag = 0;                

					break;
				}
				case 2: //USB Modem to USIM
                {  
                    while (pos)
                    {
                        m_nCurItem++;
                        nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);    

                        bool status = DataCardToUsim(0,nItem,0,0);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();

                            return;
                        }

						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						m_Progress.SetPos(m_nCurItem);
                    }

					ProgressClose(m_nSelTotalItem);

                    break;
                }
			}
			case 2: //USIM
				{
					switch (m_ComboxCurIndexDown)
					{
					case 0: //USIM to PC
						while (pos)
						{
							nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);
							m_nCurItem++;
							bool status = DataCradOrUsimToPc(1,nItem,0,0);
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								m_nPbSelPosDlgFlag = 0;
								return;
							}
						//	CWnd*  cWnd = GetParent();
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							
							DispatchMsg();
							
							m_Progress.SetPos(m_nCurItem);                        
						}
						
						ProgressClose(m_nSelTotalItem);
						
						m_nPbSelPosDlgFlag = 0;         
						break;
					case 1: //USIM to USB modem
						while (pos)
						{
							nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);
							m_nCurItem++;
							bool status = DataCardToUsim(1,nItem,0,0);
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								return;
							}
						//	CWnd*  cWnd = GetParent();
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							
							DispatchMsg();
							m_Progress.SetPos(m_nCurItem);
						}
						ProgressClose(m_nSelTotalItem);
						break;
					}
					break;
				}
       }
 }
	this->EnableWindow(true);
    UpdateButtonStatus();
}

/*响应从下往上的copysome事件*/
void CTransferDlg::OnButtonCopySomeUp() 
{
    // TODO: Add your control notification handler code here
    m_nCurSelCtrl = 0;
    POSITION pos = m_lstPhoneBookDOWN.GetFirstSelectedItemPosition();
    if (!pos)
    {
        UpdateButtonStatus();
        return;
    }
    int nItem;
    m_nSelTotalItem = m_lstPhoneBookDOWN.GetSelectedCount();
    m_nCurItem      = 0;
	this->EnableWindow(false);
	ProgressOpen(m_nSelTotalItem,1); 
    m_PBSelect=FALSE;
	CWnd*  cWnd = GetParent();
    switch (m_ComboxCurIndexDown)
    {
    case 0: //PC
        {
            while (pos)
            {
                nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
                m_nCurItem++;
                bool status;
				switch(m_ComboxCurIndexUp)
				{
				case 1://PC to USB Modem 
					status= PcToDataCradOrUsim(0,nItem,0,1);
					break;
				case 2: //PC to USIM
					status= PcToDataCradOrUsim(1,nItem,0,1);
					break;
				}
				
                if (!status)
                {
					this->EnableWindow(true);
                    UpdateButtonStatus();
                    return;
                }
				
				cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
				
						DispatchMsg();
				m_Progress.SetPos(m_nCurItem);
            }
			ProgressClose(m_nSelTotalItem);
            
            break;
        }

    case 1: //USB Modem
        {
            switch (m_ComboxCurIndexUp)
            {
            case 0:  //USB Modem to PC
                {
                    while (pos)
                    {
                        nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
                        m_nCurItem++;
                        bool status = DataCradOrUsimToPc(0,nItem,0,1);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            m_nPbSelPosDlgFlag = 0;
                            return;
                        }
						CWnd*  cWnd = GetParent();
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

						DispatchMsg();

						m_Progress.SetPos(m_nCurItem);                        
                    }

					ProgressClose(m_nSelTotalItem);

                    m_nPbSelPosDlgFlag = 0;                    

                    break;
                }

            case 2:  //USB Modem to USIM
                {
                    while (pos)
                    {
                        nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
                        m_nCurItem++;
                        bool status = DataCardToUsim(0,nItem,0,1);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            return;
                        }
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						m_Progress.SetPos(m_nCurItem);
                    }
					ProgressClose(m_nSelTotalItem);
                    
                    break;
                }
			
			}
                
       case 2: //USIM 
       {
		   switch (m_ComboxCurIndexUp)
            {
			   case 0: //USIM to PC
				   while (pos)
				   {
					   nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
					   m_nCurItem++;
					   bool status = DataCradOrUsimToPc(1,nItem,0,1);
					   if (!status)
					   {
						   this->EnableWindow(true);
						   UpdateButtonStatus();
						   m_nPbSelPosDlgFlag = 0;
						   return;
					   }
					   CWnd*  cWnd = GetParent();
					   cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
					   
					   DispatchMsg();
					   
					   m_Progress.SetPos(m_nCurItem);                        
				   }
				   
				   ProgressClose(m_nSelTotalItem);
				   
                    m_nPbSelPosDlgFlag = 0;         
				   break;
			   case 1: //USIM to USB modem
				   while (pos)
				   {
					   nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
					   m_nCurItem++;
					   bool status = DataCardToUsim(1,nItem,0,1);
					   if (!status)
					   {
						   this->EnableWindow(true);
						   UpdateButtonStatus();
						   return;
					   }
					   cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
					   m_Progress.SetPos(m_nCurItem);
				   }
					ProgressClose(m_nSelTotalItem);
				   break;
			}


           break;
        }

      }                
   }
    
	
	this->EnableWindow(true);
    UpdateButtonStatus();
}

/*响应从上向下的CutSome事件*/
void CTransferDlg::OnButtonCutSome() 
{
    // TODO: Add your control notification handler code here
    m_nCurSelCtrl = 0;
    POSITION pos = m_lstPhoneBookUP.GetFirstSelectedItemPosition();
    if (!pos)
    {
		UpdateButtonStatus();
        return;
    }
    
    int nItem;  //联系人列表中的序号

    m_nSelTotalItem = m_lstPhoneBookUP.GetSelectedCount();
    m_nCurItem      = 0;
	this->EnableWindow(false);
	ProgressOpen(m_nSelTotalItem,1); 
	m_PBSelect=FALSE;
		CWnd*  cWnd = GetParent();
    switch (m_ComboxCurIndexUp)
    {
    case 0: //PC
        {
            while (true)
            {
                pos = m_lstPhoneBookUP.GetFirstSelectedItemPosition();

                if (!pos)
                {
                    break;
                }
                m_nCurItem ++;
                nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);
                bool status;
				switch(m_ComboxCurIndexDown)
				{
				case 1: //PC to USB Modem
					status = PcToDataCradOrUsim(0,nItem,1,0);
					break;
				case 2: //PC to USIM
					status = PcToDataCradOrUsim(1,nItem,1,0);
					break;
				}

                if (!status)
                {
					this->EnableWindow(true);
                    UpdateButtonStatus();

                    return;
                }
				m_Progress.SetPos(m_nCurItem);

                CWnd*  cWnd = GetParent();
                cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);                
            }
			ProgressClose(m_nSelTotalItem);

            break;
        }

    case 1: // USB Modem
        {
            switch (m_ComboxCurIndexDown)
            {
            case 0: //USB Modem to PC
                {            
                    while(true)                    
                    {                    
                        pos = m_lstPhoneBookUP.GetFirstSelectedItemPosition();

                        if (!pos)
                        {
                            break;
                        }

                        m_nCurItem ++;
					

                        nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);
                        bool status = DataCradOrUsimToPc(0,nItem,1,0);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            m_nPbSelPosDlgFlag = 0;
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

                            return;
                        }
						
                        cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						
						DispatchMsg();

						m_Progress.SetPos(m_nCurItem);                      
                    }
					ProgressClose(m_nSelTotalItem);

                    m_nPbSelPosDlgFlag = 0;                    

                    break;
                }

            case 2: //USB Modem to USIM
                {                
                    while(true)                    
                    {                    
                        pos = m_lstPhoneBookUP.GetFirstSelectedItemPosition();
                        if (!pos)
                        {
                            break;
                        }

                        m_nCurItem ++;
                        nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);
                        bool status = DataCardToUsim(0,nItem,1,0);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            return;
                        }
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						m_Progress.SetPos(m_nCurItem);
                    }
					ProgressClose(m_nSelTotalItem);

                    break;
                }
            }
			break;
        }
		case 2://USIM
		{
			switch (m_ComboxCurIndexDown)
            {
            case 0: //USIM to PC
                {            
                    while(true)                    
                    {                    
                        pos = m_lstPhoneBookUP.GetFirstSelectedItemPosition();
						
                        if (!pos)
                        {
                            break;
                        }
						
                        m_nCurItem ++;
						CWnd*  cWnd = GetParent();
						
                        nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);
                        bool status = DataCradOrUsimToPc(1,nItem,1,0);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            m_nPbSelPosDlgFlag = 0;
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							
                            return;
                        }
						
                        cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						
						DispatchMsg();
						
						m_Progress.SetPos(m_nCurItem);                      
                    }
					ProgressClose(m_nSelTotalItem);
					
                    m_nPbSelPosDlgFlag = 0;                    
					
                    break;
                }
				
            case 1: // USIM to USB Modem 
                {                
                    while(true)                    
                    {                    
                        pos = m_lstPhoneBookUP.GetFirstSelectedItemPosition();
                        if (!pos)
                        {
                            break;
                        }
						
                        m_nCurItem ++;
                        nItem = m_lstPhoneBookUP.GetNextSelectedItem(pos);
                        bool status = DataCardToUsim(1,nItem,1,0);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            return;
                        }
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						m_Progress.SetPos(m_nCurItem);
                    }
					ProgressClose(m_nSelTotalItem);
					
                    break;
                }
            }
			break;
		}
	}
	this->EnableWindow(true);
    UpdateButtonStatus();
}

/*响应从下向上的CutSome事件*/
void CTransferDlg::OnButtonCutSomeUp() 
{    
    // TODO: Add your control notification handler code here
	m_nCurSelCtrl = 0;
    POSITION pos = m_lstPhoneBookDOWN.GetFirstSelectedItemPosition();
    if (!pos)
    {
        UpdateButtonStatus();
        return;
    }

    int nItem;  //联系人列表中的序号
    m_nSelTotalItem = m_lstPhoneBookDOWN.GetSelectedCount();
    m_nCurItem      = 0;
	this->EnableWindow(false);
	ProgressOpen(m_nSelTotalItem,1); 
	CWnd*  cWnd = GetParent();
    switch (m_ComboxCurIndexDown)
    {
    case 0:  //PC
        {
            
            while (true)
            {
                pos = m_lstPhoneBookDOWN.GetFirstSelectedItemPosition();
                if (!pos)
                {
                    break;
                }

                m_nCurItem++;
                nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
                bool status;
				switch(m_ComboxCurIndexUp)
				{
					case 1: //PC to USB Modem
						status= PcToDataCradOrUsim(0,nItem,1,1);
						break;
					case 2: //PC to USIM
						status= PcToDataCradOrUsim(1,nItem,1,1);
						break;

				}
				
                if (!status)
                {
					this->EnableWindow(true);
                    UpdateButtonStatus();
                    return;
                }
				CWnd*  cWnd = GetParent();
                cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

				DispatchMsg();

				m_Progress.SetPos(m_nCurItem);       
            }
			ProgressClose(m_nSelTotalItem);

            break;
        }

    case 1: //USB Modem
        {
            switch (m_ComboxCurIndexUp)
            {
            case 0: //USB Modem to PC
                {
                    while (true)
                    {
                        pos = m_lstPhoneBookDOWN.GetFirstSelectedItemPosition();
                        if (!pos)
                        {
                            break;
                        }

                        m_nCurItem++;
						

                        nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
                        bool status = DataCradOrUsimToPc(0,nItem,1,1);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            m_nPbSelPosDlgFlag = 0;
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

                            return;
                        }
                        
                        cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

						m_Progress.SetPos(m_nCurItem);
                    }
					ProgressClose(m_nSelTotalItem);
                    m_nPbSelPosDlgFlag = 0;                    
                    
                    break;
                } 

            case 2: //USB Modem to USIM
                {
                    while (true)
                    {
                        pos = m_lstPhoneBookDOWN.GetFirstSelectedItemPosition();
                        if (!pos)
                        {
                            break;
                        }

                        m_nCurItem++;
                        nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
                        bool status = DataCardToUsim(0,nItem,1,1);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            return;
                        }
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						m_Progress.SetPos(m_nCurItem);
                    }
                    
					ProgressClose(m_nSelTotalItem);
                    break;
                }
		  }
		}
		case 2: //USIM 
				{
					switch (m_ComboxCurIndexUp)
					{
					case 0: //USIM to PC
						while (true)
						{
							pos = m_lstPhoneBookDOWN.GetFirstSelectedItemPosition();
							if (!pos)
							{
								break;
							}
							m_nCurItem++;
							nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
							
							bool status = DataCradOrUsimToPc(1,nItem,1,1);
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								m_nPbSelPosDlgFlag = 0;
								return;
							}
							CWnd*  cWnd = GetParent();
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							
							DispatchMsg();
							
							m_Progress.SetPos(m_nCurItem);                        
						}
						
						ProgressClose(m_nSelTotalItem);
						
						m_nPbSelPosDlgFlag = 0;         
						break;
					case 1: //USIM to USB modem
						while (true)
						{
							pos = m_lstPhoneBookDOWN.GetFirstSelectedItemPosition();
							if (!pos)
							{
								break;
							}
							m_nCurItem++;
							nItem = m_lstPhoneBookDOWN.GetNextSelectedItem(pos);
							
							bool status = DataCardToUsim(1,nItem,1,1);
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								return;
							}
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							m_Progress.SetPos(m_nCurItem);
						}
						ProgressClose(m_nSelTotalItem);
						break;
					}
					
					break;
		}//case end	

  }
	this->EnableWindow(true);
    UpdateButtonStatus();
}

/* 下拉菜单操作*/
void CTransferDlg::OnSelchangeComboSeldown() 
{    
    // TODO: Add your control notification handler code here
	m_nCurSelCtrl = 0;
    int nIndex = m_ComboSelDown.GetCurSel();
    if (nIndex == m_ComboxCurIndexDown)
    {
        UpdateButtonStatus();
        return;
    }

    switch (nIndex)
    {
    case 0: //PC
        {
//             InitListCtrlPC(1);
//             ReDrawListCtrlPC(1);
			InitListCtrlDown(0);
	          ReDrawListCtrlDown(0);

//             if (0 == m_ComboxCurIndexUp)
//             {
//                 m_ComboxCurIndexUp = 1;
//                 m_ComboSelUp.SetCurSel(m_ComboxCurIndexUp);
// 
//                 InitListCtrlDCAndCard(0);
//                 ReDrawListCtrlDC(0);
//             }

            break;
        }

    case 1: //USB Modem
        {
//             InitListCtrlDCAndCard(1);
//             ReDrawListCtrlDC(1);
			InitListCtrlDown(1);
	          ReDrawListCtrlDown(1);
            
            break;
        }

	case 2: //USIM
        {
			//             InitListCtrlDCAndCard(1);
			//             ReDrawListCtrlDC(1);
			InitListCtrlDown(2);
			ReDrawListCtrlDown(2);
            
            break;
        }
    default:
        {
            break;
        }
    }

    m_ComboxCurIndexDown = nIndex;

    UpdateButtonStatus();
}

void CTransferDlg::OnSelchangeComboSelup() 
{    
    // TODO: Add your control notification handler code here
	m_nCurSelCtrl = 0;
    int nIndex = m_ComboSelUp.GetCurSel();
    if (nIndex == m_ComboxCurIndexUp)
    {
        UpdateButtonStatus();
        return;
    }

    switch (nIndex)
    {
    case 0: //PC
        {
//             InitListCtrlPC(0);
//             ReDrawListCtrlPC(0);

            InitListCtrlUp(0);
	          ReDrawListCtrlUp(0);

//             if (0 == m_ComboxCurIndexDown)
//             {
//                 m_ComboxCurIndexDown = 1;
//                 m_ComboSelDown.SetCurSel(m_ComboxCurIndexDown);
// 
//                 InitListCtrlDCAndCard(1);
//                 ReDrawListCtrlDC(1);
//             }

            break;
        }

    case 1: //USB Modem
        {
//             InitListCtrlDCAndCard(0);
//             ReDrawListCtrlDC(0);
			
            InitListCtrlUp(1);
	          ReDrawListCtrlUp(1);
            
            break;
        }
	case 2: //USIM
        {
			//             InitListCtrlDCAndCard(0);
			//             ReDrawListCtrlDC(0);
			
            InitListCtrlUp(2);
			ReDrawListCtrlUp(2);
            
            break;
        }

    default:
        {
            break;
        }
    }

    m_ComboxCurIndexUp = nIndex;

    UpdateButtonStatus();
}

/*通过发送AT命令，实现在DATACARD和USIM端新增一条电话薄记录和删除一条电话薄记录的功能。
nflag为0代表datacard存储器，nflag为1代表usim存储器*/


bool CTransferDlg::AtWriteARecord(CString Name,CString Num, int index,int nflag)
{
//    AT+CPBW=1,"123",129,"ljl"
//    AT+CPBS="SM"
  //  Sleep(100);

	BOOL bResponse = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->CommIsReady();
	if (!bResponse)
	{
		return false;
	}

    char cIndexTemp[4] = {0};
    itoa(index, cIndexTemp, 10);
    CString strIndexTemp = cIndexTemp;
    strIndexTemp.TrimLeft();
    strIndexTemp.TrimRight();
   
#ifdef FEATURE_HAIER_PHONEBOOK
	m_StrSource = _T("AT^CPBW=");
#else
	m_StrSource = _T("AT+CPBW=");
#endif
    m_StrSource.Insert(m_StrSource.GetLength(),strIndexTemp);
    
    if (_T("") != Num)
    {    
#ifdef FEATURE_HAIER_PHONEBOOK
		bool bCheckRes = false;
#endif
        m_StrSource.Insert(m_StrSource.GetLength(),_T(",\""));
        m_StrSource.Insert(m_StrSource.GetLength(),Num);
#ifdef FEATURE_HAIER_PHONEBOOK
		m_StrSource.Insert(m_StrSource.GetLength(),_T("\",255,"));
#else
        m_StrSource.Insert(m_StrSource.GetLength(),_T("\",129,"));
#endif
        if (_T("") != Name)
        {        
            //Name.Insert(strlen(Name),"\"");
			m_StrSource.Insert(m_StrSource.GetLength(),_T("\""));
#ifdef FEATURE_HAIER_PHONEBOOK
			int iByte;
			bCheckRes = CheckNameUnicode(Name, &iByte);
			if(bCheckRes){//是中文
#endif
			CString CodeUnicode=BTToUCS2(Name);
			UCS2ToUCS2(Name,CodeUnicode);
#ifdef FEATURE_HAIER_PHONEBOOK
			CodeUnicode.Insert(0, _T("80")); //add unicode character encoding identifier
#endif
			m_StrSource.Insert(m_StrSource.GetLength(),CodeUnicode);
#ifdef FEATURE_HAIER_PHONEBOOK
			}else{
			m_StrSource.Insert(m_StrSource.GetLength(),Name);
			}
#endif
			m_StrSource.Insert(m_StrSource.GetLength(),_T("\""));
			
        }
        else
        {
           m_StrSource.Insert(m_StrSource.GetLength(),_T("\"\""));
        }
#ifdef FEATURE_HAIER_PHONEBOOK
		m_StrSource.Insert(m_StrSource.GetLength(), bCheckRes ? _T(", 1") : _T(", 2"));
#endif
    }
    
    TCHAR szAtBuf[512]={0};
    //memset(szAtBuf,0,sizeof(TCHAR)*512);
#ifdef FEATURE_VERSION_NOSIM
    CString strAtSetStore=_T("AT");
 	if(!m_PBSelect)
     {
		if (0 == nflag)
		{
			strAtSetStore = _T("AT+CPBS=\"ME\"");
		}
		else if (1 == nflag)
		{
			strAtSetStore = _T("AT+CPBS=\"SM\"");
		}
		m_PBSelect=TRUE;
	}
#else
    CString strAtSetStore=_T("AT");
//  	if(!m_PBSelect)
//      {
		if (0 == nflag)
		{
			strAtSetStore = _T("AT+CPBS=\"ME\"");
		}
		else if (1 == nflag)
		{
			strAtSetStore = _T("AT+CPBS=\"SM\"");
		}
 //	if(!nflag)
 //		m_PBSelect=FALSE;
//	}

#endif


    wcscpy(szAtBuf,strAtSetStore);
    char szAtAnsBuf[512]={0};
    //memset(szAtAnsBuf,0,sizeof(char)*512);
	wcscpy(szAtBuf,strAtSetStore);
    szAtBuf[wcslen(szAtBuf)] = 0x000D;  
    szAtBuf[wcslen(szAtBuf)] = 0; 
    WCharToChar(szAtBuf,szAtAnsBuf);
    m_ReadDataFromDCState = SEND_AT_SET_STORE;
    Sleep(100);
	
	m_ReadDataCardAndUSIMStatus = true; //用于判断回调函数是否正确执行
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
	if(pComm->WriteToPort(szAtAnsBuf, wcslen(szAtBuf)))
    {        
		SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_SET_STORE,30000, NULL);        
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespWriteRecordState, this);

		int nNumPrinting = 1;
		MSG msg;

		while (true)
		{   // Wait for next message or object being signaled
			DWORD   dwWake;
			dwWake = MsgWaitForMultipleObjects(
									nNumPrinting,
									&m_Event,
									FALSE,
									INFINITE,
									QS_ALLEVENTS);

			if (dwWake >= WAIT_OBJECT_0 && dwWake < WAIT_OBJECT_0 + nNumPrinting)
			{  
				//对象被触发
				break;
			}
			else if (dwWake == WAIT_OBJECT_0 + nNumPrinting)
			{
				//有消息到达
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{   // Get Next message in queue
					 if (msg.message == WM_QUIT)
					 {                     
						 break;
					 }
					 TranslateMessage(&msg);
					 DispatchMessage(&msg);
			   }
			}
		}

		::ResetEvent(m_Event);
		Sleep(100);
		if (!m_ReadDataCardAndUSIMStatus)
		{
			return false;
		}
		if(0 == Num.GetLength())
		{
			if(0 == nflag)
			{
				g_DataCardUsedNum --;
			}
			else if (1 == nflag)
			{
				g_USimUsedNum --;
			}
		}
		else
		{
			if(0 == nflag)
			{
				g_DataCardUsedNum --;
			}
			else if (1 == nflag)
			{
				g_USimUsedNum --;
			}
		}
        return true;
}

 return TRUE;
}

//AT命令"AT+CPBW"的回调函数
void CTransferDlg::AtRespWriteRecordState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CTransferDlg* pDlg = (CTransferDlg*)pWnd;
    TCHAR szAtBuf[512]={0};
    //memset(szAtBuf,0,sizeof(char) * 512);    

	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);
	AddPrefixName(strAppName);

    switch(pDlg->m_ReadDataFromDCState)
    {
        case SEND_AT_SET_STORE:
        {    
            pDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_SET_STORE);

            if (0 == wStrNum) 
            {
				//AfxMessageBox("设置存储器失败"); //debug
				pDlg->m_ReadDataCardAndUSIMStatus = false;
				::SetEvent(pDlg->m_Event);

                return;
            }

            CString strResult = strArr[wStrNum - 1];
    
            if (0 != strResult.CompareNoCase(_T("OK")))
            {
				//AfxMessageBox("设置存储器失败"); //debug
				pDlg->m_ReadDataCardAndUSIMStatus = false;
				::SetEvent(pDlg->m_Event);

                return;
            }
    char szAscAtBuf[512]={0};
    
    wcscpy(szAtBuf,pDlg->m_StrSource);	
    szAtBuf[wcslen(szAtBuf)] = 0x0D;  
    szAtBuf[wcslen(szAtBuf)] = 0; 

    WCharToChar(szAtBuf, szAscAtBuf);
    Sleep(100);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAscAtBuf, wcslen(szAtBuf)))
    {        
        pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBW_WRITE_RECORD, 30000, NULL);        
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespWriteRecordState, pDlg);    
        pDlg->m_ReadDataFromDCState = SEND_WRITE_RECORD;
    } 
	else 
	{
		//AfxMessageBox("Write to port fail! ");  //Debug info
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strTransfail;
		strTransfail.LoadString(IDS_PBTRANSFER_FAILED);
		pDlg->MessageBox(strTransfail,strAppName,MB_OK);
		pDlg->m_ReadDataCardAndUSIMStatus = false;
		::SetEvent(pDlg->m_Event);

		return;
	}
	pDlg->m_ReadDataCardAndUSIMStatus = true;
	
    break;        
        }
        case SEND_WRITE_RECORD:
        {            
            CString strTransFail;
			pDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBW_WRITE_RECORD);

            if (0 == wStrNum) 
            {                
               	//AfxMessageBox("写存储器失败"); //debug	
				CString strAppName;
				strAppName.LoadString(IDS_APPNAME);
				AddPrefixName(strAppName);
				strTransFail.LoadString(IDS_PBTRANSFER_FAILED);
				pDlg->MessageBox(strTransFail,strAppName,MB_OK);
				//AfxMessageBox(strTransFail);

				pDlg->m_ReadDataCardAndUSIMStatus = false;
				::SetEvent(pDlg->m_Event);

                return;
            }
            
            CString strResult = strArr[wStrNum - 1];
            if (0 != strResult.CompareNoCase(_T("OK")))
            {                
                //AfxMessageBox("写存储器失败"); //debug
				CString strAppName;
				strAppName.LoadString(IDS_APPNAME);
				AddPrefixName(strAppName);
				strTransFail.LoadString(IDS_PBTRANSFER_FAILED);
				pDlg->MessageBox(strTransFail,strAppName,MB_OK);
				//AfxMessageBox(strTransFail);

				pDlg->m_ReadDataCardAndUSIMStatus = false;
				::SetEvent(pDlg->m_Event);

				return;
                                
            }
         	Sleep(10);
            pDlg->m_ReadDataFromDCState = 0;
			pDlg->m_ReadDataCardAndUSIMStatus = true;
			::SetEvent(pDlg->m_Event);
            
            break;
        }
        default:
        {
			//AfxMessageBox("m_ReadDataFromDCState status error!"); //debug info
			pDlg->m_ReadDataCardAndUSIMStatus = false;
			::SetEvent(pDlg->m_Event);

            break;
        }
    }
}

/*获取USIM或者DATACARD的内存数组中第一个空闲的存储位置。返回值为其下标，-1代表已满*/
int CTransferDlg::GetFirstIdleIndex(StPbRecord *pStDCPbRecord, int nTotalNum)
{
    for (int i = 0; i < nTotalNum; i++)
    {
        if (-1 == pStDCPbRecord[i].index)
        {
            return i;
        }
    }
    return -1;
}

void CTransferDlg::OnCancel() 
{
    // TODO: Add extra cleanup here
	//CWnd*  cWnd = GetParent();   
	m_HsdpaDlg->m_pPhoneBookDlg->nCurSelIndex=m_nGroupSelected;
	m_HsdpaDlg->m_pPhoneBookDlg->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
    m_HsdpaDlg->m_pPhoneBookDlg->PostMessage(WM_PB_UPDATECONTACTNUMBER, 1000, NULL); //更新存储介质中的联系人数量
    CDialog::OnCancel();
}


void CTransferDlg::OnClickListUp(NMHDR* pNMHDR, LRESULT* pResult) 
{
   int nCount = m_lstPhoneBookUP.GetItemCount();
   if (0 != nCount)
   {
		m_nCurSelCtrl = 1;
		if(m_ComboxCurIndexUp==m_ComboxCurIndexDown)
			return;
	    UpdateButtonStatus();
   }
}

void CTransferDlg::OnClickListDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
    int nCount = m_lstPhoneBookDOWN.GetItemCount();
	if (0 != nCount)
	{
		m_nCurSelCtrl = 2;
		if(m_ComboxCurIndexUp==m_ComboxCurIndexDown)
			return;
		UpdateButtonStatus();
	}
}

//分发队列中堆积的消息
void CTransferDlg::DispatchMsg()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{   // Get Next message in queue
		 if (msg.message == WM_QUIT)
		 {                     
			 break;

		 }
		 TranslateMessage(&msg);
		 DispatchMessage(&msg);
   }
}

/*响应CopyAll操作，将窗口上部的下拉框中选择的存储器中的数据全部转储至窗口下部的存储器中*/
void CTransferDlg::OnButtonCopyAll() 
{
    // TODO: Add your control notification handler code here

    
//    StPbRecord* DCPbRecordTemp = NULL;
//    StPbRecord PbBookPC;
    m_nCurSelCtrl  = 0;
    int nItemCount = m_lstPhoneBookUP.GetItemCount();
    m_nSelTotalItem = nItemCount;
    m_nCurItem = 0;
	this->EnableWindow(false);
	ProgressOpen(m_nSelTotalItem,1); 
	m_PBSelect=FALSE;
	CWnd*  cWnd = GetParent();   
    switch (m_ComboxCurIndexUp)
    {
        case 0: //PC
        {            
			
            for (int i = 0;i < nItemCount; i++)
            {
                Sleep(100);
                m_nCurItem++;
                bool status;
				switch(m_ComboxCurIndexDown)
				{
				case 1://PC to USB Modem
					status= PcToDataCradOrUsim(0,i,0,0);
					break;
				case 2: //PC to USIM
					status= PcToDataCradOrUsim(1,i,0,0);
					break;

				}
				
                if (!status)
                {                    
                    this->EnableWindow(true);
					UpdateButtonStatus();
                    return;
                }
				
				cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
				
				DispatchMsg();

				m_Progress.SetPos(m_nCurItem);
            }
			
			ProgressClose(m_nSelTotalItem);
            
            break;
        }
        case 1: //USB Modem
        {
            switch (m_ComboxCurIndexDown)
            {
            case 0 : //USB Modem to PC
                {
                    for (int i = 0;i < nItemCount;i++)
                    {
                        Sleep(100);
                        m_nCurItem++;
                        bool status = DataCradOrUsimToPc(0,i,0,0);
                        if (!status)
                        {
                            this->EnableWindow(true);
							UpdateButtonStatus();
                            m_nPbSelPosDlgFlag = 0;
                            return;
                        }
					//	CWnd*  cWnd = GetParent();   
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

						DispatchMsg();

						m_Progress.SetPos(m_nCurItem);
                    }
					ProgressClose(nItemCount);

                    m_nPbSelPosDlgFlag = 0;                                     

                    break;
                }
            case 2: //USB Modem to USIM
                {
                    for (int i = 0;i < nItemCount;i++)
                    {
                        Sleep(100);
                        m_nCurItem++;
                        bool status = DataCardToUsim(0,i,0,0); 
                        if (!status)
                        {
                            this->EnableWindow(true);
							UpdateButtonStatus();

                            return;
                        }
					//	CWnd*  cWnd = GetParent();   
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						DispatchMsg();

						m_Progress.SetPos(m_nCurItem);
                    }  
					ProgressClose(nItemCount);

                    break;
                }
	
            }
			break;
		}

		case 2: //USIM
		{
			switch (m_ComboxCurIndexDown)
			{
				case 0 : //USIM Modem to PC
				{
					for (int i = 0;i < nItemCount;i++)
					{
							Sleep(100);
							m_nCurItem++;
							bool status = DataCradOrUsimToPc(1,i,0,0);
							if (!status)
							{
									this->EnableWindow(true);
									UpdateButtonStatus();
									m_nPbSelPosDlgFlag = 0;
									return;
							}
							//	CWnd*  cWnd = GetParent();   
								cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
								
								DispatchMsg();
								
								m_Progress.SetPos(m_nCurItem);
					}
						ProgressClose(nItemCount);	
						m_nPbSelPosDlgFlag = 0;                                     	
						break;
				}
				case 1: //USIM to USB Modem
				{
					for (int i = 0;i < nItemCount;i++)
					{
							Sleep(100);
							m_nCurItem++;
							bool status = DataCardToUsim(1,i,0,0); 
							if (!status)
							{
									this->EnableWindow(true);
									UpdateButtonStatus();
									
									return;
							}
								cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							DispatchMsg();
								
								m_Progress.SetPos(m_nCurItem);
						}  
						ProgressClose(nItemCount);	
							break;
				}
            }	
		} //case end

  }    
	this->EnableWindow(true);
    UpdateButtonStatus();
}

/*响应CopyAll操作，将窗口下部的下拉框中选择的存储器中的数据全部转储至窗口下部的存储器中*/
void CTransferDlg::OnButtonCopyAllUp() 
{
    // TODO: Add your control notification handler code here
    m_nCurSelCtrl = 0;
    int nItemCount = m_lstPhoneBookDOWN.GetItemCount();
    m_nSelTotalItem  = nItemCount;
    m_nCurItem = 0;
	this->EnableWindow(false);
	ProgressOpen(m_nSelTotalItem,1); 
	m_PBSelect=FALSE;
		CWnd*  cWnd = GetParent();
    switch (m_ComboxCurIndexDown)
    {
        case 0: //PC 
        {
            for (int i = 0;i < nItemCount; i++)
            {
                Sleep(100);    
                m_nCurItem++;
                bool status;
				switch(m_ComboxCurIndexUp)
				{
				case 1: //PC to USB Modem
					status= PcToDataCradOrUsim(0,i,0,1);   
					break;
				case 2: //PC to USIM
					status= PcToDataCradOrUsim(1,i,0,1);   
					break;
				}
				         
                if (!status)
                {
					this->EnableWindow(true);
                    UpdateButtonStatus();

                    return;
                }
			
				cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
				
						DispatchMsg();
				m_Progress.SetPos(m_nCurItem);
            }

			ProgressClose(m_nSelTotalItem);

            break;
        }
        case 1: //USB Modem
        {
            switch (m_ComboxCurIndexUp)
            {
            case 0 : //USB Modem to PC
                {
                    for (int i = 0;i < nItemCount;i++)
                    {
                        Sleep(100);
                        m_nCurItem++;
                        bool status = DataCradOrUsimToPc(0,i,0,1);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            m_nPbSelPosDlgFlag = 0;
                            return;
                        }
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

						DispatchMsg();

						m_Progress.SetPos(m_nCurItem);
                    }
					
					ProgressClose(m_nSelTotalItem);

                    m_nPbSelPosDlgFlag = 0;                    

                    break;
                }
            case 2: //USB Modem to USIM
                {
                    for (int i = 0;i < nItemCount;i++)
                    {
                        Sleep(100);
                        m_nCurItem++;
                        bool status = DataCardToUsim(0,i,0,1); 
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            return;
                        }
						cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						m_Progress.SetPos(m_nCurItem);
                    }

					ProgressClose(m_nSelTotalItem);

                    break;
                }
            }
			break;
        }

		case 2: //USIM
			{
				switch (m_ComboxCurIndexUp)
				{
				case 0 : //USIM to PC
					{
						for (int i = 0;i < nItemCount;i++)
						{
							Sleep(100);
							m_nCurItem++;
							bool status = DataCradOrUsimToPc(1,i,0,1);
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								m_nPbSelPosDlgFlag = 0;
								return;
							}
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							
							DispatchMsg();
							
							m_Progress.SetPos(m_nCurItem);
						}
						
						ProgressClose(m_nSelTotalItem);
						
						m_nPbSelPosDlgFlag = 0;                    
						
						break;
					}
				case 1: //USIM to USB Modem
					{
						for (int i = 0;i < nItemCount;i++)
						{
							Sleep(100);
							m_nCurItem++;
							bool status = DataCardToUsim(1,i,0,1); 
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								return;
							}
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							m_Progress.SetPos(m_nCurItem);
						}
						
						ProgressClose(m_nSelTotalItem);
						
						break;
					}
            }
		}
    }
	this->EnableWindow(true);
    UpdateButtonStatus();
}

/*响应CopyAll操作，将窗口上部的下拉框中选择的存储器中的数据全部转储至窗口下部的存储器中，
在转储过程中将上部的存储器中的数据逐条删除*/
void CTransferDlg::OnButtonCutAll() 
{
    // TODO: Add your control notification handler code here
    m_nCurSelCtrl = 0;
    int nItemCount = m_lstPhoneBookUP.GetItemCount();
    m_nSelTotalItem = nItemCount;
    m_nCurItem = 0;
	
	this->EnableWindow(false);
	ProgressOpen(m_nSelTotalItem,1); 
	m_PBSelect=FALSE;
			CWnd*  cWnd = GetParent();
    switch (m_ComboxCurIndexUp)
    {
        case 0: //PC
        {
            int nCount = 0;

            for (int i = 0;i < nItemCount; i++)
            {
                Sleep(100);    
                m_nCurItem++;
                bool status;
				switch(m_ComboxCurIndexDown)
				{
					case 1: //PC to USB Modem
						status= PcToDataCradOrUsim(0,i - nCount,1,0);
						break;
					case 2: //PC to USIM
						status= PcToDataCradOrUsim(1,i - nCount,1,0);
						break;

				}
				
                if (!status)
                {
					this->EnableWindow(true);
                    UpdateButtonStatus();
                    return;
                }
				m_Progress.SetPos(m_nCurItem);
                nCount++;

                CWnd*  cWnd = GetParent();
                cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
            }
			ProgressClose(m_nSelTotalItem);            

            break;
        }
        case 1: //USB Modem
        {
            switch (m_ComboxCurIndexDown)
            {
            case 0 : //USB Modem to PC
                {
                    int nCount = 0;
					m_PBSelect=FALSE;
                    for (int i = 0;i < nItemCount;i++)
                    {
						Sleep(100);
                        m_nCurItem++;
						CWnd*  cWnd = GetParent();

                        bool status = DataCradOrUsimToPc(0,i - nCount,1,0);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            m_nPbSelPosDlgFlag = 0;
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

                            return;
                        }
                        nCount++;
                        
                        cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

						DispatchMsg();

						m_Progress.SetPos(m_nCurItem);
                    }
					ProgressClose(m_nSelTotalItem);
                    m_nPbSelPosDlgFlag = 0;                    

                    break;
                }
            case 2: //USB Modem to USIM
                {
                    int nCount = 0;
                    for (int i = 0;i < nItemCount;i++)
                    {
                        Sleep(100);
                        m_nCurItem++;
                        bool status = DataCardToUsim(0,i - nCount,1,0); 
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();

                            return;
                        }
						 cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						m_Progress.SetPos(m_nCurItem);

                        nCount++;
                    }  
					ProgressClose(m_nSelTotalItem);

                    break;
                }
            }
		  break;
        }
		case 2: //USIM 
			{
				switch (m_ComboxCurIndexDown)
				{
				case 0 : //USIM to PC
					{
						int nCount = 0;
						for (int i = 0;i < nItemCount;i++)
						{
							Sleep(100);
							m_nCurItem++;
							CWnd*  cWnd = GetParent();
							
							bool status = DataCradOrUsimToPc(1,i - nCount,1,0);
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								m_nPbSelPosDlgFlag = 0;
								cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
								
								return;
							}
							nCount++;
							
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							
							DispatchMsg();
							
							m_Progress.SetPos(m_nCurItem);
						}
						ProgressClose(m_nSelTotalItem);
						m_nPbSelPosDlgFlag = 0;                    
						
						break;
					}
				case 1: //USIM to USB Modem
					{
						int nCount = 0;
						for (int i = 0;i < nItemCount;i++)
						{
							Sleep(100);
							m_nCurItem++;
							bool status = DataCardToUsim(1,i - nCount,1,0); 
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								
								return;
							}
							 cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							m_Progress.SetPos(m_nCurItem);
							
							nCount++;
						}  
						ProgressClose(m_nSelTotalItem);
						
						break;
					}
				}
        }
    }
    this->EnableWindow(true);
    UpdateButtonStatus();
}

/*响应CopyAll操作，将窗口下部的下拉框中选择的存储器中的数据全部转储至窗口上部的存储器中，
在转储过程中将下部的存储器中的数据逐条删除*/
void CTransferDlg::OnButtonCutAllUp() 
{
    // TODO: Add your control notification handler code here
    m_nCurSelCtrl = 0;
    int nItemCount = m_lstPhoneBookDOWN.GetItemCount();
    m_nSelTotalItem = nItemCount;
    m_nCurItem = 0;
	this->EnableWindow(false);
	ProgressOpen(m_nSelTotalItem,1); 
	m_PBSelect=FALSE;
			CWnd*  cWnd = GetParent();
    switch (m_ComboxCurIndexDown)
    {
        case 0: //PC
        {
            int nCount = 0;
			m_PBSelect=FALSE;
            for (int i = 0;i < nItemCount; i++)
            {
                Sleep(100);
                m_nCurItem++;
                bool status;
				switch(m_ComboxCurIndexUp)
				{
				case 1:// PC to USB Modem
					status= PcToDataCradOrUsim(0,i - nCount,1,1);
					break;
				case 2:// PC to USIM
					status= PcToDataCradOrUsim(1,i - nCount,1,1);
					break;
				}
				
                if (!status)
                {
					this->EnableWindow(true);
                    UpdateButtonStatus();

                    return;
                }
				m_Progress.SetPos(m_nCurItem);

                nCount++;
                CWnd*  cWnd = GetParent();
                cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
            }
			ProgressClose(m_nSelTotalItem);

            break;
        }
        case 1: //USB Modem 
        {
            switch (m_ComboxCurIndexUp)
            {
            case 0 : //USB Modem to PC
                {
                    int nCount = 0;
						m_PBSelect=FALSE;
                    for (int i = 0;i < nItemCount;i++)
                    {
                        Sleep(100);
                        m_nCurItem++;
						CWnd*  cWnd = GetParent();
					    bool status = DataCradOrUsimToPc(0,i - nCount,1,1);
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            m_nPbSelPosDlgFlag = 0;
						//	cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

                            return;
                        }
                        nCount++;
                        
                        cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);

						DispatchMsg();

						m_Progress.SetPos(m_nCurItem);
                    }
					ProgressClose(m_nSelTotalItem);

                    m_nPbSelPosDlgFlag = 0;                    

                    break;
                }
            case 2: //USB Modem to USIM
                {
                    int nCount = 0;
					m_PBSelect=FALSE;
                    for (int i = 0;i < nItemCount;i++)
                    {
                        Sleep(100);
                        m_nCurItem++;
                        bool status = DataCardToUsim(0,i - nCount,1,1); 
                        if (!status)
                        {
							this->EnableWindow(true);
                            UpdateButtonStatus();
                            return;
                        }
						 cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
						m_Progress.SetPos(m_nCurItem);

                        nCount++;
                    }
					ProgressClose(m_nSelTotalItem);

                    break;
                }
            }
		  break;
		}
		

		case 2: //USIM  
			{
				switch (m_ComboxCurIndexUp)
				{
				case 0 : //USIM to PC
					{
						int nCount = 0;
						for (int i = 0;i < nItemCount;i++)
						{
							Sleep(100);
							m_nCurItem++;
					
							
							bool status = DataCradOrUsimToPc(1,i - nCount,1,1);
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								m_nPbSelPosDlgFlag = 0;
								//	cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
								
								return;
							}
							nCount++;
							
							cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							
							DispatchMsg();
							
							m_Progress.SetPos(m_nCurItem);
						}
						ProgressClose(m_nSelTotalItem);
						
						m_nPbSelPosDlgFlag = 0;                    
						
						break;
					}
				case 1: //USIM to USB Modem 
					{
						int nCount = 0;
						for (int i = 0;i < nItemCount;i++)
						{
							Sleep(100);
							m_nCurItem++;
							bool status = DataCardToUsim(1,i - nCount,1,1); 
							if (!status)
							{
								this->EnableWindow(true);
								UpdateButtonStatus();
								return;
							}
							 cWnd->PostMessage(WM_PB_ReDrawListCtrl, NULL, NULL);
							m_Progress.SetPos(m_nCurItem);
							
							nCount++;
						}
						ProgressClose(m_nSelTotalItem);
						
						break;
					}
				}
				break;
		}


    }
	this->EnableWindow(true);
    UpdateButtonStatus();
}

bool CTransferDlg::TransferBreak()
{
	if ((m_HsdpaDlg->m_bInComSms) || (m_HsdpaDlg->m_bInComCall))
	{		
		this->PostMessage(WM_CLOSE,0,0);
		m_bEndPbTransfer = true;
		::SetEvent(g_BGPassEvt);

		return false;
	}
	return true;
}

/*实现Pc至DataCrad或者Pc至Usim存储器的数据转储操作，如果是剪切的话，将实现删除功能*/
//case0.  flag为0代表PCToDataCrad(DowntoUp ),flag为1代表PcToUsim(UpToDown)
//Direction 为copy方向, 0为UpToDown, 1为DownToUp
bool CTransferDlg::PcToDataCradOrUsim(bool flag,int nItem,bool Cut,int Direction)   
{
	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);
	AddPrefixName(strAppName);

	CString strTemp = _T(""); 

	::ResetEvent(g_BGPassEvt);

	if(!TransferBreak())
	{
		return false;
	}

    CMyListCtrl* MyListCtrlFrom = NULL;
    CMyListCtrl* MyListCtrlTo = NULL;
    StPbRecord * UsimOrDataCardRecord;
    int nPbItemCount;
    int nTotalNum;
	if(Direction==0) //Up to Down
	{
		if (flag) //to USIM
		{
			MyListCtrlFrom         = &m_lstPhoneBookUP;
			MyListCtrlTo           = &m_lstPhoneBookDOWN;        
			UsimOrDataCardRecord   = pUSIMPbRecord;
			nTotalNum              = g_USimTotalNum;
		}
		else  //to USB Modem
		{
			MyListCtrlFrom         = &m_lstPhoneBookUP;
			MyListCtrlTo           = &m_lstPhoneBookDOWN;        
			UsimOrDataCardRecord   = pDataCardPbRecord;
			nTotalNum              = g_DataCardTotalNum;
		}
	}
	else if(Direction==1) //Down to up
	{
		if (flag) //to USIM
		{
			MyListCtrlFrom         = &m_lstPhoneBookDOWN;
			MyListCtrlTo           = &m_lstPhoneBookUP;        
			UsimOrDataCardRecord   = pUSIMPbRecord;
			nTotalNum              = g_USimTotalNum;
		}
		else  //to USB Modem
		{
			MyListCtrlFrom         = &m_lstPhoneBookDOWN;
			MyListCtrlTo           = &m_lstPhoneBookUP;        
			UsimOrDataCardRecord   = pDataCardPbRecord;
			nTotalNum              = g_DataCardTotalNum;
		}

	}

                
	StPbRecord *PbBookPC = (StPbRecord *)MyListCtrlFrom->GetItemData(nItem);
	
	CString strDataCardFull;
	strDataCardFull.LoadString(IDS_PBTRANSFER_CARDFULL);
	CString strUsimFull;
	strUsimFull.LoadString(IDS_PBTRANSFER_USIMFULL);

	//当前记录的名称为空时，自动为其命名为UNKNOWN
	CString temp = _T("");
	if (temp == PbBookPC->strName)
	{
		CString strNullNume;
		strNullNume.LoadString(IDS_UNKNOWN_NAME);
		wcscat(PbBookPC->strName,strNullNume);
	}

	CString strNameLenTemp;	
	strNameLenTemp.Format(PbBookPC->strName);

#ifdef FEATURE_HAIER_PHONEBOOK
if(1)	//because via code will check cpbw paramter length
#else
if(flag==1) //PC to USIM
#endif
{
#ifndef FEATURE_VERSION_NOSIM
	if(strNameLenTemp.GetLength() > PB_NAME_SIM_MAX)
	{
		strTemp = strNameLenTemp.Left(PB_NAME_SIM_MAX);
		memcpy(PbBookPC->strName,strTemp,PB_NAME_MAX);
	}

	/*判断名字中是否有中文，有则判断名字长度是否超过6，超过则截取前6位*/
	CString strMaxName;
	int iByte = 0;
	strMaxName.Format(PbBookPC->strName);
	bool bCheckRes = CheckNameUnicode(strMaxName,&iByte);
	if(!bCheckRes)//不是中文
	{
		strTemp = strMaxName.Left(PB_NAME_SIM_MAX);
//		memcpy(PbBookPC->strName,strTemp,PB_NAME_MAX);
	}
	else //是中文
	{
		strTemp = strMaxName.Left(PB_NAME_UCS_MAX);
	//	memcpy(PbBookPC->strName,strTemp,PB_NAME_MAX);
	}

#endif
}
else  //PC to USB modem
   strTemp=PbBookPC->strName;

	//分别判断mobile/company/home是否为空，不为空，则形成一条记录写入下位机中
    if (temp != PbBookPC->strMobile)
    {
        int nIdleIndex = GetFirstIdleIndex(UsimOrDataCardRecord, nTotalNum);
        switch (flag)
        {
            case 0:
            {
                if (-1 == nIdleIndex)
                {	
					//AfxMessageBox(strDataCardFull);
					::MessageBox(m_Hwnd,strDataCardFull,strAppName,MB_OK);

                    return false;
                }
                break;
            }
            case 1:
            {
                if (-1 == nIdleIndex)
                {
                    //AfxMessageBox(strUsimFull);
					::MessageBox(m_Hwnd,strUsimFull,strAppName,MB_OK);

                    return false;
                }
                break;
            }
        }

#ifdef FEATURE_HAIER_PHONEBOOK
		//对号码长度进行检查，如号码长度超过20，则截取前20个字符
		CString strMaxNumber;
		strMaxNumber.Format(PbBookPC->strMobile);
		if(strMaxNumber.GetLength() > PB_NUM_SIM_MAX){
			CString numTemp = strMaxNumber.Left(PB_NUM_SIM_MAX);
			memset(PbBookPC->strMobile, 0, (PB_NUM_MAX + 1)*sizeof(TCHAR));
			wcscat(PbBookPC->strMobile, numTemp);
		}
#endif
		CString NameTemp;
	//	NameTemp = PbBookPC->strName ;
		NameTemp=strTemp;

        bool bMobile = AtWriteARecord(NameTemp,PbBookPC->strMobile, nIdleIndex + 1,flag);
        
        if (!bMobile)
        {
            return false;
        }
        UsimOrDataCardRecord[nIdleIndex].index = nIdleIndex + 1 ;
    //    memcpy(UsimOrDataCardRecord[nIdleIndex].strName,PbBookPC->strName,PB_NAME_MAX);
		 memcpy(UsimOrDataCardRecord[nIdleIndex].strName,strTemp,PB_NAME_MAX*2);
        memcpy(UsimOrDataCardRecord[nIdleIndex].strMobile,PbBookPC->strMobile,PB_NUM_MAX*2);
        
        nPbItemCount = MyListCtrlTo->GetItemCount();
    //   MyListCtrlTo->InsertItem(nPbItemCount, PbBookPC->strName, (DWORD)(&UsimOrDataCardRecord[nIdleIndex]));
		  MyListCtrlTo->InsertItem(nPbItemCount, strTemp, (DWORD)(&UsimOrDataCardRecord[nIdleIndex]));
		MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE,
                                 PbBookPC->strMobile);                    
    }                

    if (temp != PbBookPC->strHome )
    {
        int nIdleIndex = GetFirstIdleIndex(UsimOrDataCardRecord, nTotalNum);

        switch (flag)
        {
            case 0:
            {
                if (-1 == nIdleIndex)
                {
                    AfxMessageBox(strDataCardFull);
                    return false;
                }
                break;
            }
            case 1:
            {
                if (-1 == nIdleIndex)
                {
                    AfxMessageBox(strUsimFull);
                    return false;
                }
                break;
            }
        }

		CString NameTemp;
      //  NameTemp = PbBookPC->strName;
		NameTemp=strTemp;

        bool bHome = AtWriteARecord(NameTemp,PbBookPC->strHome, nIdleIndex + 1,flag);

        if (!bHome)
        {
            return false;
        }
        UsimOrDataCardRecord[nIdleIndex].index = nIdleIndex + 1;        
        memcpy(UsimOrDataCardRecord[nIdleIndex].strName,PbBookPC->strName,PB_NAME_MAX*2);
        memcpy(UsimOrDataCardRecord[nIdleIndex].strMobile,PbBookPC->strHome,PB_NUM_MAX*2);
    
        nPbItemCount = MyListCtrlTo->GetItemCount();
      //  MyListCtrlTo->InsertItem(nPbItemCount, PbBookPC->strName, (DWORD)(&UsimOrDataCardRecord[nIdleIndex]));
		MyListCtrlTo->InsertItem(nPbItemCount, strTemp, (DWORD)(&UsimOrDataCardRecord[nIdleIndex]));
        MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE,
                                 PbBookPC->strHome);
    }                
    
    if (temp != PbBookPC->strCompany)
    {
        int nIdleIndex = GetFirstIdleIndex(UsimOrDataCardRecord, nTotalNum);

        switch (flag)
        {
            case 0:
            {
                if (-1 == nIdleIndex)
                {
                    AfxMessageBox(strDataCardFull);

                    return false;
                }
                break;
            }
            case 1:
            {
                if (-1 == nIdleIndex)
                {
                    AfxMessageBox(strUsimFull);

                    return false;
                }
                break;
            }
        }

		

		CString NameTemp;
      //  NameTemp = PbBookPC->strName;
		NameTemp=strTemp;

        bool bCompany = AtWriteARecord(NameTemp,PbBookPC->strCompany, nIdleIndex + 1,flag);
        
        if (!bCompany)
        {
            return false;
        }
        UsimOrDataCardRecord[nIdleIndex].index = nIdleIndex + 1;
      //  memcpy(UsimOrDataCardRecord[nIdleIndex].strName,PbBookPC->strName,PB_NAME_MAX);
		 memcpy(UsimOrDataCardRecord[nIdleIndex].strName,strTemp,PB_NAME_MAX*2);
        memcpy(UsimOrDataCardRecord[nIdleIndex].strMobile,PbBookPC->strCompany,PB_NUM_MAX*2);
        
        nPbItemCount = MyListCtrlTo->GetItemCount();
     //  MyListCtrlTo->InsertItem(nPbItemCount, PbBookPC->strName, (DWORD)(&UsimOrDataCardRecord[nIdleIndex]));
		  MyListCtrlTo->InsertItem(nPbItemCount, strTemp, (DWORD)(&UsimOrDataCardRecord[nIdleIndex]));
        MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE,
                                 PbBookPC->strCompany);
    }
    if (Cut)
    {
		if(!m_pPbData->DelSigle(PbBookPC))
		{
			return false;
		}
        MyListCtrlFrom->DeleteItem(nItem);
    }    
    return true;
}

/*实现DataCrad至Pc或者Usim至Pc存储器的数据转储操作，如果是剪切的话，将实现删除功能*/
// case1.0.  flag为0代表DataCardToPc(UpToDown),flag为1代表UsimToPc(DowntoUp)
bool CTransferDlg::DataCradOrUsimToPc(bool flag,int nItem,bool Cut, int Direction)    
{
	::ResetEvent(g_BGPassEvt);

	if(!TransferBreak())
	{
		return false;
	}

    CMyListCtrl* MyListCtrlFrom = NULL;
    CMyListCtrl* MyListCtrlTo = NULL;
    StPbRecord * UsimOrDataCardRecord;
    StPbRecord PbBookPC;
    int nPbItemCount = 0;
    int nTotalNum = 0;
	int nUsedNum = 0;
    CString strPbSaveIfo;
	CString temp = _T("");
	DWORD PbArrItemAds = NULL;

    if(Direction==0) //Up to Down
	{
		if (!flag)//USB Modem to PC
		{
			MyListCtrlFrom         = &m_lstPhoneBookUP ;
			MyListCtrlTo           = &m_lstPhoneBookDOWN;
			UsimOrDataCardRecord   = pDataCardPbRecord;
			nTotalNum              = g_DataCardTotalNum;
			
		}
		else //USIM to PC
		{
			MyListCtrlFrom         = &m_lstPhoneBookUP;
			MyListCtrlTo           = &m_lstPhoneBookDOWN;        
			UsimOrDataCardRecord   = pUSIMPbRecord;
			nTotalNum              = g_USimTotalNum;
		}
	}
	else if(Direction==1) //Down to UP
	{
		if (!flag) //USB Modem to PC
		{
			MyListCtrlFrom         = &m_lstPhoneBookDOWN ;
			MyListCtrlTo           = &m_lstPhoneBookUP;        
			UsimOrDataCardRecord   = pDataCardPbRecord;
			nTotalNum              = g_DataCardTotalNum;
		}
		else //USIM to PC
		{
			MyListCtrlFrom         = &m_lstPhoneBookDOWN;
			MyListCtrlTo           = &m_lstPhoneBookUP;        
			UsimOrDataCardRecord   = pUSIMPbRecord;
			nTotalNum              = g_USimTotalNum;
		}
	}

    StPbRecord* DCPbRecordTemp = (StPbRecord*)MyListCtrlFrom->GetItemData(nItem);
    if (-1 == DCPbRecordTemp->index)
    {
        //AfxMessageBox("Read data failed!");  //debug info
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strTransfail;
		strTransfail.LoadString(IDS_PBTRANSFER_FAILED);
		::MessageBox(m_Hwnd,strTransfail,strAppName,MB_OK);
		//AfxMessageBox(strTransfail);

        return false;
    }
	
	CString Nametemp = DCPbRecordTemp->strName;
	CString Numtemp = DCPbRecordTemp->strMobile;

	memset(&PbBookPC,0,sizeof(StPbRecord));

	nPbItemCount = MyListCtrlTo->GetItemCount();

	CPbSelPositon SelPosDlg(Nametemp,Numtemp,this);    
	SelPosDlg.m_nSet = m_nPbSelPosDlgFlag;
	SelPosDlg.m_Flag = m_nPbSelPosDlgSelFlag;

	if (m_pPbData->CheckFull())
	{
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strPbFull;
		strPbFull.LoadString(IDS_PB_FULL);
		::MessageBox(m_Hwnd,strPbFull,strAppName,MB_OK);

		return false;
	}

	if(1 == SelPosDlg.m_nSet) //是否选择了default复选框
	{
		if (temp != Numtemp)
		{
			

			switch (SelPosDlg.m_Flag)
			{
				case 1:
				{
					memcpy(PbBookPC.strName,Nametemp,PB_NAME_MAX*2);
					memcpy(PbBookPC.strMobile,Numtemp,PB_NUM_MAX*2);

					
					bool bResult = m_pPbData->AddTailSingle(PbBookPC,&PbArrItemAds);
					if (!bResult)
					{
						strPbSaveIfo.LoadString(IDS_PB_SAVEFAILED);
						AfxMessageBox(strPbSaveIfo);
						return false;
					}
					MyListCtrlTo->InsertItem(nPbItemCount, Nametemp, PbArrItemAds);
					MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE,
											 Numtemp);

					break;
				}
				case 2:
				{
					memcpy(PbBookPC.strName,Nametemp,PB_NAME_MAX*2);
					memcpy(PbBookPC.strHome,Numtemp,PB_NUM_MAX*2);            
					bool bResult = m_pPbData->AddTailSingle(PbBookPC,&PbArrItemAds);
					if (!bResult)
					{
						strPbSaveIfo.LoadString(IDS_PB_SAVEFAILED);
						AfxMessageBox(strPbSaveIfo);
						return false;
					}
					MyListCtrlTo->InsertItem(nPbItemCount, Nametemp, PbArrItemAds);
					MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME,
											 Numtemp);

					break;
				}
				case 3:
				{
					memcpy(PbBookPC.strName,Nametemp,PB_NAME_MAX*2);
					memcpy(PbBookPC.strCompany,Numtemp,PB_NUM_MAX*2);    
					bool bResult = m_pPbData->AddTailSingle(PbBookPC,&PbArrItemAds);
					if (!bResult)
					{
						strPbSaveIfo.LoadString(IDS_PB_SAVEFAILED);
						AfxMessageBox(strPbSaveIfo);
						return false;
					}
					MyListCtrlTo->InsertItem(nPbItemCount, Nametemp, PbArrItemAds);
					MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY,
											 Numtemp);

					break;
				}
				default:
				{
					break;
				}

			}//END switch
		}

        if (Cut)
        {
            bool bCut = AtWriteARecord(_T(""),_T(""), DCPbRecordTemp->index,flag);
            if (!bCut)
            {
                return false;
            }           
            DCPbRecordTemp->index = -1;
            memset(DCPbRecordTemp->strName,0,PB_NAME_MAX*2);
            memset(DCPbRecordTemp->strMobile,0,PB_NUM_MAX*2);

            MyListCtrlFrom->DeleteItem(nItem);    
        }
    }
    else
    {
        int nsel = SelPosDlg.DoModal();
        switch(nsel)
        {
            case IDOK:
            {
				if (temp != Numtemp)
				{
					this->EnableWindow(false);  //将transfer主窗口置灰

					m_nPbSelPosDlgFlag = SelPosDlg.m_nSet;
					m_nPbSelPosDlgSelFlag = SelPosDlg.m_Flag;

					switch (SelPosDlg.m_Flag)
					{
						case 1:
						{
							memcpy(PbBookPC.strName,Nametemp,PB_NAME_MAX*2);
							memcpy(PbBookPC.strMobile,Numtemp,PB_NUM_MAX*2);    
							bool bResult = m_pPbData->AddTailSingle(PbBookPC,&PbArrItemAds);    
							if (!bResult)
							{
								strPbSaveIfo.LoadString(IDS_PB_SAVEFAILED);
								AfxMessageBox(strPbSaveIfo);
								return false;
							}
							MyListCtrlTo->InsertItem(nPbItemCount, Nametemp, PbArrItemAds);
							MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE,
													 Numtemp);

							break;
						}
						case 2:
						{
							memcpy(PbBookPC.strName,Nametemp,PB_NAME_MAX*2);
							memcpy(PbBookPC.strHome,Numtemp,PB_NUM_MAX*2);                                        
							bool bResult = m_pPbData->AddTailSingle(PbBookPC,&PbArrItemAds);    
							if (!bResult)
							{
								strPbSaveIfo.LoadString(IDS_PB_SAVEFAILED);
								AfxMessageBox(strPbSaveIfo);
								return false;
							}
							MyListCtrlTo->InsertItem(nPbItemCount, Nametemp, PbArrItemAds);
							MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME,
													 Numtemp);

							break;
						}
						case 3:
						{
							memcpy(PbBookPC.strName,Nametemp,PB_NAME_MAX*2);
							memcpy(PbBookPC.strCompany,Numtemp,PB_NUM_MAX*2);    
							bool bResult = m_pPbData->AddTailSingle(PbBookPC,&PbArrItemAds);    
							if (!bResult)
							{
								strPbSaveIfo.LoadString(IDS_PB_SAVEFAILED);
								AfxMessageBox(strPbSaveIfo);
								return false;
							}
							MyListCtrlTo->InsertItem(nPbItemCount, Nametemp, PbArrItemAds);
							MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY,
													 Numtemp);

							break;
						}
						default:
						{
							break;
						}

					}//END switch    
				}
                if(Cut)
                {
                    bool bCutPB = AtWriteARecord(_T(""),_T(""), DCPbRecordTemp->index,flag);
                
                    if (!bCutPB)
                    {
                        return false;
                    }                        
                    DCPbRecordTemp->index = -1;
                    memset(DCPbRecordTemp->strName,0,PB_NAME_MAX*2);
                    memset(DCPbRecordTemp->strMobile,0,PB_NUM_MAX*2);

                    MyListCtrlFrom->DeleteItem(nItem);    
                }

                break;
                    
            }//case IDOK
            case IDCANCEL:
            {
                return false;
            }
        }
    }//end else 

    return true;
}


/*实现DataCrad至Usim或者Usim至DataCrad存储器的数据转储操作，如果是剪切的话，将实现删除功能*/
 //case1.1.  flag为0代表DataCardToUsim(UpToDown)，为1代表UsimToDataCard(DownToUp)。
bool CTransferDlg::DataCardToUsim(bool flag,int nItem,bool Cut, int Direction)     
{  
	::ResetEvent(g_BGPassEvt);

	if(!TransferBreak())
	{
		return false;
	}

    CMyListCtrl* MyListCtrlFrom = NULL;
    CMyListCtrl* MyListCtrlTo = NULL;
    StPbRecord * RecordFrom = NULL;
    StPbRecord * RecordTo = NULL;
    int nPbItemCount = 0;
    int nTotalNum = 0;
    if(Direction==0) //Up to Down
	{
		if (flag) //USIM to USB modem
		{
			MyListCtrlFrom         = &m_lstPhoneBookUP  ;
			MyListCtrlTo           = &m_lstPhoneBookDOWN;        
			RecordFrom             = pUSIMPbRecord ;
			RecordTo               = pDataCardPbRecord;
			nTotalNum              = g_DataCardTotalNum;
		}
		else //USB Modem to USIM
		{
			MyListCtrlFrom        = &m_lstPhoneBookUP;
			MyListCtrlTo          = &m_lstPhoneBookDOWN;
			RecordFrom            = pDataCardPbRecord;
			RecordTo              = pUSIMPbRecord;
			nTotalNum             = g_USimTotalNum;
		}
	}
	else if(Direction==1) //Down to Up
	{
		if (flag) //USIM to USB Modem
		{
			MyListCtrlFrom         = &m_lstPhoneBookDOWN  ;
			MyListCtrlTo           = &m_lstPhoneBookUP;        
			RecordFrom             = pUSIMPbRecord ;
			RecordTo               = pDataCardPbRecord;
			nTotalNum              = g_DataCardTotalNum;
		}
		else //USB Modem to USIM
		{
			MyListCtrlFrom        = &m_lstPhoneBookDOWN;
			MyListCtrlTo          = &m_lstPhoneBookUP;
			RecordFrom            = pDataCardPbRecord;
			RecordTo              = pUSIMPbRecord;
			nTotalNum             = g_USimTotalNum;
		}

	}

    StPbRecord* DCPbRecordTemp = (StPbRecord*)MyListCtrlFrom->GetItemData(nItem);
    if (-1 == DCPbRecordTemp->index)
    {
        //AfxMessageBox("Read data failed!");  //debug info
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strTransfail;
		strTransfail.LoadString(IDS_PBTRANSFER_FAILED);
		::MessageBox(m_Hwnd,strTransfail,strAppName,MB_OK);
		//AfxMessageBox(strTransfail);

        return false ;
    }
    CString Nametemp = DCPbRecordTemp->strName;
    CString Numtemp = DCPbRecordTemp->strMobile;                        

    int nIdleIndex = GetFirstIdleIndex(RecordTo, nTotalNum);
    switch (flag)
    {
        case 0:
        {
            if (-1 == nIdleIndex)
            {
				CString strUsimFull;
				strUsimFull.LoadString(IDS_PBTRANSFER_USIMFULL);
                AfxMessageBox(strUsimFull);

                return false;
            }
            break;
        }
        case 1: 
        {
            if (-1 == nIdleIndex)
            {
				CString strDataCardFull;
				strDataCardFull.LoadString(IDS_PBTRANSFER_CARDFULL);
                AfxMessageBox(strDataCardFull);				
                
                return false;
            }
            break;
        }
    }

	CString strNameTemp;
	//strNameTemp = GBToUCS2(DCPbRecordTemp->strName);
    strNameTemp = DCPbRecordTemp->strName;
    bool bWrite = AtWriteARecord(strNameTemp,Numtemp, nIdleIndex + 1,!(flag));
    
    if (!bWrite)
    {
        return false;
    }

    RecordTo[nIdleIndex].index = nIdleIndex + 1;
    memcpy(RecordTo[nIdleIndex].strName,Nametemp,PB_NAME_MAX);
    memcpy(RecordTo[nIdleIndex].strMobile,Numtemp,PB_NUM_MAX);        
                            
    nPbItemCount = MyListCtrlTo->GetItemCount();
    MyListCtrlTo->InsertItem(nPbItemCount, Nametemp, (DWORD)(&RecordTo[nIdleIndex]));
    MyListCtrlTo->SetItemText(nPbItemCount, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE,Numtemp);

    if (Cut)
    {
        bool bCut = AtWriteARecord(_T(""),_T(""), DCPbRecordTemp->index,flag);
        if (!bCut)
        {
            return false;
        }
        DCPbRecordTemp->index = -1;
        memset(DCPbRecordTemp->strName,0,PB_NAME_MAX);
        memset(DCPbRecordTemp->strMobile,0,PB_NUM_MAX);

        MyListCtrlFrom->DeleteItem(nItem);
    }
    return true;
}

void CTransferDlg::OnDblclkListUp(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    OnButtonCopySome();
    *pResult = 0;
}

void CTransferDlg::OnDblclkListDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    OnButtonCopySomeUp();
    *pResult = 0;
}

void CTransferDlg::MallocDlgMem(int nUpper,int nStep)
{
    if (NULL != m_pDlg)
    {
        delete m_pDlg;
    }
    m_pDlg = new CWaitDlg(this,nUpper,nStep);
}

/*设置转储过程中的进度条显示*/
void CTransferDlg::ProgressOpen(int nUpper,int nSetp)
{
	m_Progress.ShowWindow(SW_SHOWNA);
	m_Progress.SetRange(0,nUpper);
	m_Progress.SetStep(nSetp);
	m_Progress.SetPos(0);
	
	m_Event = ::CreateEvent(NULL,FALSE,FALSE,NULL);
}
void CTransferDlg::ProgressClose(int nSetp)
{
	m_Progress.SetPos(nSetp);
	::CloseHandle(m_Event);
}

BOOL CTransferDlg::isChineseCharUnicode(const TCHAR ch)
{
//     if( ((ch>>8) & 0x0080 )&&(ch & 0x80))
//     {
//         // ch 为汉字或全角字符
//         return TRUE;
//     }
	if(ch>0xa0)
    {
        // ch 为汉字或全角字符
        return TRUE;
    }
    return FALSE;
}

bool CTransferDlg::CheckNameUnicode(const TCHAR* ch,int* iByte)
{
	bool bNameUcs = false;  //用于区分姓名中是否有中文，默认为英文
	*iByte = 0;
	int nLen = 0;
	int nCharLen = wcslen(ch);
/*
	for (int i = 0;i < nCharLen;i++)
	{
		nLen++;

		if(IsDBCSLeadByte((BYTE)ch[i]))
		{
			i++;
			if(nLen <= PB_NAME_UCS_MAX)
			{
				*iByte = (*iByte) + 2;
			}
			bNameUcs = true;
		}
		else
		{
			if(nLen <= PB_NAME_UCS_MAX)
			{
				(*iByte)++;
			}
		}
	}
	*/
	while(*ch)
	{
		BOOL bRes = isChineseCharUnicode(*ch);
		if(bRes)
		{
			ch++;
			if(nLen < PB_NAME_UCS_MAX)
 			{
				*iByte = (*iByte)+2;
		 	}			
			bNameUcs = true;
		}
		else
		{
			if(nLen < PB_NAME_UCS_MAX)
			{
				(*iByte)++;
			}
			ch++;
		
		}
			nLen++;
	}
	if(bNameUcs)
	{
		if (PB_NAME_UCS_MAX < nLen)
		{
			*iByte=PB_NAME_UCS_MAX;
		}
	}

	return bNameUcs;
}
BOOL CTransferDlg::isChineseChar(const char* ch)
{
    if( (*ch & 0x80 )&&(*(ch+1)&0x80))
    {
        // ch 为汉字或全角字符
        return TRUE;
    }
    return FALSE;
}

bool CTransferDlg::CheckName(const char* ch,int* iByte)
{
	bool bNameUcs = false;  //用于区分姓名中是否有中文，默认为英文
	*iByte = 0;
	int nLen = 0;
	int nCharLen = strlen(ch);
	for (int i = 0;i < nCharLen;i++)
	{
		nLen++;

		if(IsDBCSLeadByte((BYTE)ch[i]))
		{
			i++;
			if(nLen <= PB_NAME_UCS_MAX)
			{
				*iByte = (*iByte) + 2;
			}
			bNameUcs = true;
		}
		else
		{
			if(nLen <= PB_NAME_UCS_MAX)
			{
				(*iByte)++;
			}
		}
	}
	/*
	while(*ch)
	{
		BOOL bRes = isChineseChar(ch);
		if(bRes)
		{
			ch = ch + 2;
			if(nLen < PB_NAME_UCS_MAX)
			{
				*iByte = (*iByte) + 2;
			}			
			bNameUcs = true;
		}
		else
		{
			if(nLen < PB_NAME_UCS_MAX)
			{
				(*iByte)++;
			}
			ch++;
		}
		nLen++;
	}
	*/
	if(bNameUcs)
	{
		if (PB_NAME_UCS_MAX < nLen)
		{
			return false;
		}
	}
	return true;
}