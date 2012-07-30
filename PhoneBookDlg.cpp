// CPhoneBookDlg.cpp : implementation file
//

/*=================================================================
FILE：   CPhoneBookDlg.cpp                            VERSION : 

DESCRIPTION
  电话薄功能提供大小共13个子功能，分别为添加、修改、查找、删除、全部删除、
  发送短信、拔打电话、导入电话薄、导出电话薄、电话薄数据转储、短消息模块中的号码提取、
  短消息模块中的调用电话本内容、通话模块中的调用电话本内容。
=================================================================*/

/*=================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
 Notice that changes are listed in reverse chronological order.

 when        who         what, where, why
 --------   -----       ---------------------------------------------
 
=================================================================*/
//Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明1.camera_start()：
	/*
	CPhoneBookDlg::OnAddContact函数：添加联系人。	
	CPhoneBookDlg::OnSearchItem函数：该函数用于在父窗口的ListCtrl控件上反亮显示满足当前查询条件的Item。
	CPhoneBookDlg::ImportFromCsv函数：导入CSV格式文件
	CPhoneBookDlg::ImportFromVcf函数：导入VCF文件		
	*/

// PhoneBookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"

#include "PhoneBookDlg.h"
#include "SmsWriteDlg.h"
#include "ContactDlg.h"
#include "SelContactDlg.h"

#include "SelPosition.h"
#include "SelDialDlg.h"
#include "TransferDlg.h"
#include "DlgPbImport.h"
#include "GetOutlookPBDlg.h"
#include "PBGroupDlg.h"
#include "Dev.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

#define VCARD_NAME_MAX    32
#define VCARD_NUM_MAX     32
//#define VCARD_EMAIL_MAX   32
#define VCARD_NOTE_MAX    128

/////////////////////////////////////////////////////////////////////////////

CPhoneBookDlg::CPhoneBookDlg(CWnd* pParent /*=NULL*/)
    : CTabedDialog(CPhoneBookDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPhoneBookDlg)
        // NOTE: the ClassWizard will add member initialization here
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    ColumnBeClicked = 0;
    //}}AFX_DATA_INIT

    m_pDlg = NULL;
	m_pWaitDlg = NULL;
	m_StrPathName = _T("");
	m_PCRecordAds = NULL;
	pUSIMPbRecord = NULL;
    pDataCardPbRecord = NULL;
	nCurSelIndex = 0;
	m_strCurSelGroup=_T("");
	//gbPcCardInit = false;
	gbSimInit = false;
	
	m_pCRecordIndex = NULL;
	m_PCCardRecordAds = NULL;
	m_USIMRecordAds = NULL;
	m_RecordTempAds = NULL;
	m_nPCCount = 0;
	m_nPCCardCount = 0;  //选择了几条PCCARD记录
	m_nUSIMCount = 0; //选择了几条USIM记录
	m_nDCCount = 0;
	m_StrSource = _T("");
	m_nStoreFlag = 0;
	m_ReadDataFromDCState = 0;
	m_nIndex = 2;
	m_ReadDataCardAndUSIMStatus = false;
	m_bEditSim=FALSE;
	m_bNewGroupFlagUSIM=FALSE;
	m_bNewGroupFlagUSB=FALSE;
	m_iContactType = CONTACTDLG_TYPE_NORMAL;
	if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
		(wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0))
	{
		m_strUSIM.LoadString(IDS_STATIC_USIM);
	}
	else if((wcscmp(g_SetData.Setup_sz3GType,_T("CDMA2000"))==0)||
		(wcscmp(g_SetData.Setup_sz3GType,_T("cdma2000"))==0))
	{
		m_strUSIM.LoadString(IDS_STATIC_RUIM);
	}
m_strPCCard.LoadString(IDS_ME);

}

CPhoneBookDlg::~CPhoneBookDlg()
{
	if (NULL != pUSIMPbRecord)
    {
        delete []pUSIMPbRecord;
    }

    if (NULL != pDataCardPbRecord)
    {
        delete []pDataCardPbRecord;
    }

	if (NULL != m_pCRecordIndex)
    {
        delete []m_pCRecordIndex;
    }

	if (NULL != m_PCRecordAds)
    {
        delete []m_PCRecordAds;
    }

	if (NULL != m_USIMRecordAds)
    {
        delete []m_USIMRecordAds;
    }	
	
    if (NULL != m_pDlg)
    {
        delete m_pDlg;
    }
	if (NULL != m_pWaitDlg)
    {
        delete m_pWaitDlg;
    }

	
	//wyw_0328 add
	if (NULL != m_PCCardRecordAds)
	{
		delete []m_PCCardRecordAds;
	}
}

void CPhoneBookDlg::DoDataExchange(CDataExchange* pDX)
{
    CTabedDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPhoneBookDlg)
	DDX_Control(pDX, IDC_LIST_PHONEBOOK, m_lstPhoneBook);
	DDX_Control(pDX, IDC_TREE_PHONEBOOK, m_TreePhoneBook );
	
	////////////////////////////////////////////////////////////////////////// add by liub for PB buttons begin 1126
	DDX_Control(pDX,IDC_BUTTON_ADD,m_btnNew);
	DDX_Control(pDX,IDC_BUTTON_MODIFY,m_btnEdit);
	DDX_Control(pDX,IDC_BUTTON_DELETE,m_btnDelete);
	DDX_Control(pDX,IDC_BUTTON_GROUP,m_btnGroup);
	DDX_Control(pDX,IDC_BUTTON_SENDSMS,m_btnSMS);
	DDX_Control(pDX,IDC_BUTTON_TRANSFER,m_btnTransfer);
	DDX_Control(pDX,IDC_BUTTON_SEARCH,m_btnSearch);
	DDX_Control(pDX,IDC_BUTTON_CALL,m_btnDial);
	////////////////////////////////////////////////////////////////////////// add by liub end
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhoneBookDlg, CTabedDialog)
    //{{AFX_MSG_MAP(CPhoneBookDlg)
    ON_BN_CLICKED(IDC_BUTTON_SENDSMS, OnButtonSendsms)
    ON_BN_CLICKED(IDC_BUTTON_CALL, OnButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
    ON_BN_CLICKED(IDC_BUTTON_TRANSFER, OnButtonTransfer)
    ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
    ON_BN_CLICKED(IDC_BUTTON_DELETE_ALL, OnButtonDeleteAll)
	ON_BN_CLICKED(IDC_BUTTON_GROUP, OnButtonGroup)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PHONEBOOK, OnRclickListPhonebook)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_PHONEBOOK, OnDblclkListPhonebook)
	ON_NOTIFY(TVN_SELCHANGED,IDC_TREE_PHONEBOOK, OnSelchangeTree)
    ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PHONEBOOK, OnColumnclickListPhonebook)
    ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
    ON_MESSAGE(WM_SEL_CONTACT_INDEX, OnSearchItem)
    ON_MESSAGE(WM_ADD_CONTACT, OnAddContact)
    ON_MESSAGE(WM_PB_ReDrawListCtrl, ReDrawListCtrl)
	ON_MESSAGE(WM_DELETE_DCRECORD, OnDelDCRecord)
	ON_MESSAGE(WM_SEARCH_CONTACT, SearchContactInList)
	ON_MESSAGE(WM_SEARCH_GETTOTALCOUNT, GetListTotalCount)
	ON_MESSAGE(WM_PB_REDRAWTREECTRL, ReDrawTreeCtrl)
	ON_MESSAGE(WM_PB_UPDATECONTACTNUMBER,OnUpdateTreeContactNum)
	ON_MESSAGE(WM_MODEM_IN_OUT,OnModemInOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneBookDlg message handlers

BOOL CPhoneBookDlg::OnInitDialog() 
{
    CTabedDialog::OnInitDialog();
	////////////////////////////////////////////////////////////////////////// add by liub for PB buttons begin 1126
    CString strButtonTip;
	////////////////////////////////////////////////////////////////////////// add by liub for PB buttons begin 1126
    m_btnNew.SetIcon(IDI_ICON_SMSNEW,IDI_ICON_SMSNEW,IDI_ICON_SMSNEW);
	m_btnNew.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	//;m_btnNew.DrawBorder(TRUE);
	strButtonTip.LoadString(IDS_PBMENU_ADDPB);
	m_btnNew.SetTooltipText(strButtonTip);

	m_btnEdit.SetIcon(IDI_ICON_SMS_EDIT,IDI_ICON_SMS_EDIT,IDI_ICON_SMS_EDIT);
	m_btnEdit.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	//m_btnEdit.DrawBorder(TRUE);
	strButtonTip.LoadString(IDS_PBMENU_MODIFY);
	m_btnEdit.SetTooltipText(strButtonTip);

	m_btnSearch.SetIcon(IDI_ICON_SMSSEARCH,IDI_ICON_SMSSEARCH,IDI_ICON_SMSSEARCH);
	m_btnSearch.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	//m_btnSearch.DrawBorder(TRUE);
	strButtonTip.LoadString(IDS_PBMENU_SEARCHPB);
	m_btnSearch.SetTooltipText(strButtonTip);

    m_btnDelete.SetIcon(IDI_ICON_SMSDELEDT,IDI_ICON_SMSDELEDT,IDI_ICON_SMSDELEDT);
	m_btnDelete.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	//m_btnDelete.DrawBorder(TRUE);
	strButtonTip.LoadString(IDS_PBMENU_DELETE);
	m_btnDelete.SetTooltipText(strButtonTip);


	m_btnGroup.SetIcon(IDI_ICON_PB_GROUP,IDI_ICON_PB_GROUP,IDI_ICON_PB_GROUP);
	m_btnGroup.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	//m_btnGroup.DrawBorder(TRUE);
	strButtonTip.LoadString(IDS_PBMENU_GROUP);
	m_btnGroup.SetTooltipText(strButtonTip);

    m_btnSMS.SetIcon(IDI_ICON_PB_SMS,IDI_ICON_PB_SMS,IDI_ICON_PB_SMS);
	m_btnSMS.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	//m_btnSMS.DrawBorder(TRUE);
	strButtonTip.LoadString(IDS_SMS);
	m_btnSMS.SetTooltipText(strButtonTip);
	
	m_btnDial.SetIcon(IDI_ICON_SMSDAIL,IDI_ICON_SMSDAIL,IDI_ICON_SMSDAIL);
	m_btnDial.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	//m_btnDial.DrawBorder(TRUE);
	strButtonTip.LoadString(IDS_PBMENU_CALL);
	m_btnDial.SetTooltipText(strButtonTip);

	m_btnTransfer.SetIcon(IDI_ICON_PB_TRANSFER,IDI_ICON_PB_TRANSFER,IDI_ICON_PB_TRANSFER);
	m_btnTransfer.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	//m_btnTransfer.DrawBorder(TRUE);
	strButtonTip.LoadString(IDS_PBMENU_TRANSFER);
	m_btnTransfer.SetTooltipText(strButtonTip);
	//#ifdef FEATURE_VERSION_NOSIM
// 	  m_btnTransfer.ShowWindow(SW_HIDE);
// 	#endif
	////////////////////////////////////////////////////////////////////////// add by liub end
    
	m_Hwnd = GetSafeHwnd();

    // TODO: Add extra initialization here
    InitListCtrl(1);
		ReDrawTreeCtrl();

	if(!AfxOleInit())  //Initializes the OLE DLLs.
	{
		return FALSE;
	}
	if(g_SetData.Main_nCall)
		GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(TRUE);

//    m_headerCtrl = (CHeaderCtrlEx *)m_lstPhoneBook.GetHeaderCtrl();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPhoneBookDlg::OnModemInOut(WPARAM wParam, LPARAM lParam)
{
		ReDrawTreeCtrl();
		if(g_bIsExist && g_bSimReady)
		{
			m_btnDial.EnableWindow(TRUE);
			m_btnSMS.EnableWindow(TRUE);
			m_btnTransfer.EnableWindow(TRUE);
		}
		else
		{
			m_btnDial.EnableWindow(FALSE);
			m_btnSMS.EnableWindow(FALSE);
			m_btnTransfer.EnableWindow(FALSE);
			
			
		}
		
}

void CPhoneBookDlg::InitListCtrl(int flag)
{
	if(flag==1)
	{
		if(m_ilLsts.Create(IDB_PB_LOCSTATE, 16, 2, 255))
		{
			m_lstPhoneBook.SetImageList(&m_ilLsts, LVSIL_SMALL);
		}
	}
	HICON IconGroup=AfxGetApp()->LoadIcon(IDI_ICON_PB_GROUP);
	m_ilLsts.Add(IconGroup);
//表格列数
    int cols = 7;
    int arr_cols[] = {IDS_NAME,IDS_MOBILE,IDS_COMPANY,IDS_HOME,IDS_PBGROUP,IDS_EMAIL,IDS_REMARK};

    CRect rect;
    m_lstPhoneBook.GetWindowRect(&rect);
	int nWidth = rect.Width() - 18;
    CString strColTitle;
    //设置列名
    for(int i = 0;i < cols;i++)
    {
        strColTitle.LoadString(arr_cols[i]);
        m_lstPhoneBook.InsertColumn(i, strColTitle,LVCFMT_LEFT,nWidth/cols);    //(i==0)?25:rect.Width()/cols);    
    }
    m_lstPhoneBook.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstPhoneBook.m_headerCtrl.DrawHeader();

    ReDrawListCtrl();

}

int CPhoneBookDlg::DealwithResponseProc(LPARAM lParam,   // application-defined value
                                       WPARAM wParam   // application-defined value
                                       )
{
	return 1;
}

void CPhoneBookDlg::OnButtonSendsms() 
{
    // TODO: Add your control notification handler code here

    POSITION pos = m_lstPhoneBook.GetFirstSelectedItemPosition();
	int ilen = 0;
    if(!pos)
    {
        AfxMessageBox(IDS_PB_SELNULL);
        return;
    }

	int nItemNum = 0;
    nItemNum = m_lstPhoneBook.GetSelectedCount();
    if (nItemNum > SMS_GROUPSEND_MAX)
    {
        AfxMessageBox(IDS_PB_SMSSELMAX);
        return;
    }	

	CSmsWriteDlg dlg(this);
	CString strTempNumber = _T(""); //用于存储用户选择的号码

    while (pos)
    {
		int nItem = m_lstPhoneBook.GetNextSelectedItem(pos);
		UpdateData();
		
		//返回false,则中断当前的操作
		bool bResponse = SelContact(nItem, &strTempNumber);
		if (!bResponse)
		{
			return;
		}
		
		/*strTempNumber为空表示选择了cancel，只有不为空，
		即选择了ok按纽，才将其值赋给SMS的dlg.m_strNumber
		*/
		if (0 != strTempNumber.GetLength())
		{
			ilen = dlg.m_strNumber.GetLength();
			if ((ilen > 0) && (dlg.m_strNumber[ilen-1] != ';'))
			{
				dlg.m_strNumber += _T(";");
			}
			
			dlg.m_strNumber += strTempNumber;
		}
	}
	UpdateData(FALSE);
	if (_T("") != dlg.m_strNumber)
	{
		dlg.DoModal();
	}    
}

bool CPhoneBookDlg::SelContact(int nItem, CString* strNumber)
{
	CSelDialDlg SMSSelDlg(this);
	LVITEM item;
	item.mask = LVIF_IMAGE;
	item.iItem = nItem;
	item.iSubItem = 0;
	m_lstPhoneBook.GetItem(&item);
	switch(item.iImage)
	{
		case PCLOC:
		{
			StPbRecord* PbBookTemp = (StPbRecord*)m_lstPhoneBook.GetItemData(nItem);

			int nDialNum=0;
			int SelDial=0;

			if(wcslen(PbBookTemp->strMobile) > 0)
			{
				nDialNum++;
				SelDial=1;
			}
			if(wcslen(PbBookTemp->strHome) > 0)
			{
				nDialNum++;
				SelDial=2;
			}
			if(wcslen(PbBookTemp->strCompany) > 0)
			{
				nDialNum++;
				SelDial=3;
			}
			if (0 == nDialNum)
			{
				CString strNullNum;
				strNullNum.LoadString(IDS_SELCONNECT_NULLNUM);
				AfxMessageBox(strNullNum);
			
				return false;
			}
			if(nDialNum == 1)
			{
				switch ( SelDial )
				{
					case 1:
					{
						strNumber->Format(PbBookTemp->strMobile);
                
						break;
					}
					case 2:
					{
						strNumber->Format(PbBookTemp->strHome); 
               
						break;
					}
					case 3:
					{
						strNumber->Format(PbBookTemp->strCompany); 
    
						break;
					}
					default:
					{
						break;
					}
				}    
			}
			else
			{
				wcscpy(SMSSelDlg.m_strName.GetBuffer(MAX_PATH), PbBookTemp->strName);
				wcscpy(SMSSelDlg.m_strMobile.GetBuffer(MAX_PATH), PbBookTemp->strMobile);
				wcscpy(SMSSelDlg.m_strHome.GetBuffer(MAX_PATH), PbBookTemp->strHome);
				wcscpy(SMSSelDlg.m_strCompany.GetBuffer(MAX_PATH), PbBookTemp->strCompany);

				if (IDOK == SMSSelDlg.DoModal())
				{
					strNumber->Format(SMSSelDlg.m_strSelCallNum);
				}
				else
				{
					*strNumber = _T("");
				}
			}
			break;
		}
		case PCCardLOC:
		case USIMLOC:
		{
			StPbRecord* PbBookTemp = (StPbRecord*)m_lstPhoneBook.GetItemData(nItem);
			strNumber->Format(PbBookTemp->strMobile);
			break;
		}
		default:
			break;
	}
 	return true;
}

void CPhoneBookDlg::OnButtonCall() 
{
    // TODO: Add your control notification handler code here

    int PbNum = m_lstPhoneBook.GetNextItem(-1, LVNI_SELECTED);
    if(PbNum < 0)
    {
        AfxMessageBox(IDS_PB_SELNULL);
        return;
    }    

    StPbRecord* PbBookTemp = (StPbRecord* ) m_lstPhoneBook.GetItemData(PbNum);

    int nDialNum = 0;
    int nSelDial = 0;

    if(wcslen(PbBookTemp->strMobile) > 0)
    {
        nDialNum++;
        nSelDial = 1;
    }
    if(wcslen(PbBookTemp->strHome) > 0)
    {
        nDialNum++;
        nSelDial = 2;
    }
    if(wcslen(PbBookTemp->strCompany) > 0)
    {
        nDialNum++;
        nSelDial = 3;
    }
    if (0 == nDialNum)
    {
        CString strNullNum;
		strNullNum.LoadString(IDS_SELCONNECT_NULLNUM);
        AfxMessageBox(strNullNum);
        return;
    }

    if(nDialNum == 1)
    {
        switch ( nSelDial )
        {
            case 1:
			{  
				//////////////////////////////////////////////////////////////////////////对取出的移动电话号码由宽符转换为短字符
				int Numcount;
				Numcount = WideCharToMultiByte(CP_OEMCP,NULL,PbBookTemp->strMobile,-1,NULL,0,NULL,FALSE);
				char *psText;
				psText = new char[Numcount];
				if(!psText)
				{
					delete []psText;
				}
				WideCharToMultiByte (CP_OEMCP,NULL,PbBookTemp->strMobile,-1,psText,Numcount,NULL,FALSE);
                //////////////////////////////////////////////////////////////////////////
                WPARAM wParam = 0;
                LPARAM lParam = (LPARAM)(LPCTSTR)psText;
                ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam,lParam);
                delete []psText;

                break;
            }
            case 2:
            {
				////////////////////////////////////////////////////////////////////////// 对取出的家庭电话号码由宽符转换为短字符
				int Numcount;
				Numcount = WideCharToMultiByte(CP_OEMCP,NULL,PbBookTemp->strHome,-1,NULL,0,NULL,FALSE);
				char *psText;
				psText = new char[Numcount];
				if(!psText)
				{
					delete []psText;
				}
				WideCharToMultiByte (CP_OEMCP,NULL,PbBookTemp->strHome,-1,psText,Numcount,NULL,FALSE);
                //////////////////////////////////////////////////////////////////////////
                WPARAM wParam = 0;
                LPARAM lParam = (LPARAM)(LPCTSTR)psText;
                ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam,lParam);
                delete []psText;

                break;
            }
            case 3:
            {
				//////////////////////////////////////////////////////////////////////////对取出的公司电话号码由宽符转换为短字符
				int Numcount;
				Numcount = WideCharToMultiByte(CP_OEMCP,NULL,PbBookTemp->strCompany,-1,NULL,0,NULL,FALSE);
				char *psText;
				psText = new char[Numcount];
				if(!psText)
				{
					delete []psText;
				}
				WideCharToMultiByte (CP_OEMCP,NULL,PbBookTemp->strCompany,-1,psText,Numcount,NULL,FALSE);
                //////////////////////////////////////////////////////////////////////////
                WPARAM wParam = 0;
                LPARAM lParam = (LPARAM)(LPCTSTR)psText;
                ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam,lParam);
                delete []psText;

                break;
            }
            default:
            {
                break;
            }

        }
    
    }
    else
    {
        CSelDialDlg SelDlg(this);

		wcscpy(SelDlg.m_strName.GetBuffer(MAX_PATH), PbBookTemp->strName);
        wcscpy(SelDlg.m_strMobile.GetBuffer(MAX_PATH), PbBookTemp->strMobile);
        wcscpy(SelDlg.m_strHome.GetBuffer(MAX_PATH), PbBookTemp->strHome);
        wcscpy(SelDlg.m_strCompany.GetBuffer(MAX_PATH), PbBookTemp->strCompany);

        if (IDOK == SelDlg.DoModal())
        {
			//////////////////////////////////////////////////////////////////////////对取出的号码由宽符转换为短字符
			int Numcount;
			Numcount = WideCharToMultiByte(CP_OEMCP,NULL,SelDlg.m_strSelCallNum,-1,NULL,0,NULL,FALSE);
			char *psText;
			psText = new char[Numcount];
			if(!psText)
			{
				delete []psText;
			}
			WideCharToMultiByte (CP_OEMCP,NULL,SelDlg.m_strSelCallNum,-1,psText,Numcount,NULL,FALSE);
                //////////////////////////////////////////////////////////////////////////
            WPARAM wParam = 0;
            LPARAM lParam = (LPARAM)((LPCTSTR)psText);
            ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam,lParam);
			delete []psText;
        }
    }
}

void CPhoneBookDlg::OnButtonAdd() 
{
    // TODO: Add your control notification handler code here
	CContactDlg dlg(this,CONTACTDLG_TYPE_ADD);
    if(nCurSelIndex == PCLOC)
	{
		//判断PC端是否已满
		if( m_pPbData->CheckFull() )
		{
			CString strPbFull;
			strPbFull.LoadString(IDS_PB_FULL);
			AfxMessageBox(strPbFull);
			return;
		}
		dlg.m_pPbData=m_pPbData;
		dlg.m_bSimCardFlag=FALSE;
	}
	else if(nCurSelIndex == PCGROUP)
	{
		//判断PC端是否已满
		if( m_pPbData->CheckFull() )
		{
			CString strPbFull;
			strPbFull.LoadString(IDS_PB_FULL);
			AfxMessageBox(strPbFull);
			return;
		}
		dlg.m_pPbData=m_pPbData;
		dlg.m_strGroup=m_strCurSelGroup;
		dlg.m_bSimCardFlag=FALSE;
	}
	else if(nCurSelIndex == PCCardLOC)
	{
		//判断USB Modem端是否已满
		int nIdleIndex = GetFirstIdleIndex(pDataCardPbRecord, g_DataCardTotalNum);
		if(-1 == nIdleIndex)
		{
			CString strDataCardFull;
			strDataCardFull.LoadString(IDS_PBTRANSFER_CARDFULL);
            this->MessageBox(strDataCardFull);
            return;
		}
	//	dlg.m_pDataCardPbRecord=pDataCardPbRecord;
		dlg.m_bSimCardFlag=TRUE;
	}
	else if(nCurSelIndex==USIMLOC)
	{
		//判断USIM端是否已满		
		int nIdleIndex = GetFirstIdleIndex(pUSIMPbRecord, g_USimTotalNum);
		if(-1 == nIdleIndex)
		{
			CString strUsimFull;
			strUsimFull.LoadString(IDS_PBTRANSFER_USIMFULL);
            this->MessageBox(strUsimFull);
            return;
		}
		dlg.m_bSimCardFlag=TRUE;
	}	
    dlg.DoModal();
}

LRESULT CPhoneBookDlg::OnAddContact(WPARAM wParam, LPARAM lParam)
{
    StPbRecord *PbBookTemp = new StPbRecord();
    memcpy(PbBookTemp,(StPbRecord *)lParam,sizeof(StPbRecord));    
	DWORD PbArrItemAds = 0;   
	
	switch(nCurSelIndex)
	{
		case PCLOC:
		{
			if( !m_pPbData->AddTailSingle(*PbBookTemp,&PbArrItemAds ) )   
			{        
				if(NULL != PbBookTemp)
				{
					delete PbBookTemp;
				}

				return 0;
			}
			int index = m_lstPhoneBook.GetItemCount(); 

			m_lstPhoneBook.InsertItem(index, PbBookTemp->strName, PbArrItemAds, PCLOC);
			m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, PbBookTemp->strMobile);
			m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, PbBookTemp->strHome);
			m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_GROUP, PbBookTemp->strGroup);
			m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, PbBookTemp->strCompany);
			m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, PbBookTemp->strEmail);
			m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, PbBookTemp->strMark);
			UpdateContactNumForTreeCtrl(m_ht_PC,PCLOC);
			UpdateContactNumForTreeCtrl(m_ht_PC,PCGROUP);
			break;
		}
		case PCGROUP:
			{
				if( !m_pPbData->AddTailSingle(*PbBookTemp,&PbArrItemAds ) )   
				{        
					if(NULL != PbBookTemp)
					{
						delete PbBookTemp;
					}
					
					return 0;
				}
				int index = m_lstPhoneBook.GetItemCount(); 
				
				m_lstPhoneBook.InsertItem(index, PbBookTemp->strName, PbArrItemAds, PCLOC);
				m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, PbBookTemp->strMobile);
				m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, PbBookTemp->strHome);
				m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_GROUP, PbBookTemp->strGroup);
				m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, PbBookTemp->strCompany);
				m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, PbBookTemp->strEmail);
				m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, PbBookTemp->strMark);
				UpdateContactNumForTreeCtrl(m_ht_PC,PCGROUP);
				UpdateContactNumForTreeCtrl(m_ht_PC,PCLOC);
				break;
		}

		case PCCardLOC:  //USM Modem
			{
				m_iContactType = CONTACTDLG_TYPE_ADD;
				::ResetEvent(g_BGPassEvt);
				bool bRes = WriteToUSIM(PbBookTemp,0);
				::SetEvent(g_BGPassEvt); 
				if (!bRes)
				{
					if(NULL != PbBookTemp)
					{
						delete PbBookTemp;
					}
					CString strAppName,strAddFaild;
					strAppName.LoadString(IDS_APPNAME);	
					AddPrefixName(strAppName);
					strAddFaild = _T("WriteToUSB Modem faild!");				
					AfxMessageBox(strAddFaild);
					return 0;
				}
				//写内存
				int nIdleIndex = GetFirstIdleIndex(pDataCardPbRecord, g_DataCardTotalNum);
				if (-1 == nIdleIndex)
				{
					CString strUsimFull;
					strUsimFull.LoadString(IDS_PBTRANSFER_USIMFULL);
					AfxMessageBox(strUsimFull);
					return 0;
				}
				pDataCardPbRecord[nIdleIndex].index = nIdleIndex + 1;
				memcpy(pDataCardPbRecord[nIdleIndex].strName,PbBookTemp->strName,PB_NAME_MAX*2);
				memcpy(pDataCardPbRecord[nIdleIndex].strMobile,PbBookTemp->strMobile,PB_NUM_MAX*2); 
				//写list
				int index = m_lstPhoneBook.GetItemCount();
				m_lstPhoneBook.InsertItem(index, PbBookTemp->strName, (DWORD)(&pDataCardPbRecord[nIdleIndex]), PCCardLOC);
				m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, PbBookTemp->strMobile);
				UpdateContactNumForTreeCtrl(m_ht_PCCard,PCCardLOC);
				break;
		
			}  
		case USIMLOC:
		{
			m_iContactType = CONTACTDLG_TYPE_ADD;
			::ResetEvent(g_BGPassEvt);
			bool bRes = WriteToUSIM(PbBookTemp,1);
			::SetEvent(g_BGPassEvt); 
			if (!bRes)
			{
				if(NULL != PbBookTemp)
				{
					delete PbBookTemp;
				}
				CString strAppName,strAddFaild;
				strAppName.LoadString(IDS_APPNAME);	
				AddPrefixName(strAppName);
				strAddFaild = _T("WriteToUSIM faild!");				
				AfxMessageBox(strAddFaild);
				return 0;
			}
			//写内存
			int nIdleIndex = GetFirstIdleIndex(pUSIMPbRecord, g_USimTotalNum);
			if (-1 == nIdleIndex)
            {
				CString strUsimFull;
				strUsimFull.LoadString(IDS_PBTRANSFER_USIMFULL);
                AfxMessageBox(strUsimFull);
                return 0;
            }
			pUSIMPbRecord[nIdleIndex].index = nIdleIndex + 1;
			memcpy(pUSIMPbRecord[nIdleIndex].strName,PbBookTemp->strName,PB_NAME_MAX);
			memcpy(pUSIMPbRecord[nIdleIndex].strMobile,PbBookTemp->strMobile,PB_NUM_MAX); 
			//写list
			int index = m_lstPhoneBook.GetItemCount();
			m_lstPhoneBook.InsertItem(index, PbBookTemp->strName, (DWORD)(&pUSIMPbRecord[nIdleIndex]), USIMLOC);
			m_lstPhoneBook.SetItemText(index, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, PbBookTemp->strMobile);
			UpdateContactNumForTreeCtrl(m_ht_USIM,USIMLOC);

			break;
		}
		default:
			break;
	}  
    if(NULL != PbBookTemp)
	{
		delete PbBookTemp;
	}
    return 0;
}

void CPhoneBookDlg::OnButtonGroup()
{
	CString strIndex;
	CPBGroupDlg dlg(this);
	dlg.m_nGroupSelected=nCurSelIndex;
    dlg.DoModal();

}

void CPhoneBookDlg::OnButtonTransfer() 
{
    // TODO: Add your control notification handler code here
	
	 if(CheckStatForTrans())
	 {
		CTransferDlg transDlg(this);
		transDlg.m_nGroupSelected=nCurSelIndex;
		transDlg.DoModal();
	}
}

bool CPhoneBookDlg::CheckStatForTrans() 
{
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
 
    CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    ASSERT(pMainWnd);

    if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
	{
        return false;
	}
	if (CPIN_SIM_NONE_REQUIRED != pMainWnd->m_cHandlePin.m_nSimStat)
	{
		return false;
	}
	if(pComm->CommIsReady())
	{
		return true;
	}
	return false;
}

void CPhoneBookDlg::OnButtonModify() 
{
    // TODO: Add your control notification handler code here

	CFile FileTemp;
    int PbNum;
	LVITEM item;

	PbNum = m_lstPhoneBook.GetNextItem(-1, LVNI_SELECTED);
	if(PbNum < 0)
    {
        AfxMessageBox(IDS_PB_SELNULL);
        return;
    }
	item.mask = LVIF_IMAGE;
	item.iItem = PbNum;
	item.iSubItem = 0;
	m_lstPhoneBook.GetItem(&item);

	CContactDlg dlg(this,CONTACTDLG_TYPE_MODIFY);

	switch (item.iImage)
	{
		case PCLOC:
		{
			StPbRecord *PbBookTemp = NULL ;
			PbBookTemp = (StPbRecord*)m_lstPhoneBook.GetItemData(PbNum);
			dlg.m_strName = PbBookTemp->strName;
			dlg.m_strMobile = PbBookTemp->strMobile;
			dlg.m_strHome = PbBookTemp->strHome;
			dlg.m_strCompany = PbBookTemp->strCompany;
			dlg.m_strEmail = PbBookTemp->strEmail;
			dlg.m_strMark = PbBookTemp->strMark;	
			dlg.m_strPicPath = PbBookTemp->strPicPath;
			dlg.m_strGroup=PbBookTemp->strGroup;
			dlg.m_strGroupBeforechange=PbBookTemp->strGroup;
			dlg.m_pPbData=m_pPbData;
		//		dlg.m_nIndex=PbBookTemp->index-1;
			dlg.m_bSimCardFlag=FALSE;
			if (dlg.DoModal() == IDOK)
			{
				wcscpy(PbBookTemp->strName,(LPCTSTR)dlg.m_strName);
				wcscpy(PbBookTemp->strMobile,(LPCTSTR)dlg.m_strMobile);
				wcscpy(PbBookTemp->strHome,(LPCTSTR)dlg.m_strHome);
				wcscpy(PbBookTemp->strGroup,(LPCTSTR)dlg.m_strGroup);
				wcscpy(PbBookTemp->strCompany,(LPCTSTR)dlg.m_strCompany);
				wcscpy(PbBookTemp->strEmail,(LPCTSTR)dlg.m_strEmail);
				wcscpy(PbBookTemp->strMark,(LPCTSTR)dlg.m_strMark);
				wcscpy(PbBookTemp->strPicPath,(LPCTSTR)dlg.m_strPicPath);
			

				m_pPbData->UpdateMidLine(*PbBookTemp,PbBookTemp->index);

// 				m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_NAME, PbBookTemp->strName);
// 				m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, PbBookTemp->strMobile);    
// 				m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, PbBookTemp->strHome);
// 				m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_GROUP, PbBookTemp->strGroup);
// 				m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, PbBookTemp->strCompany);
// 				m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, PbBookTemp->strEmail);
// 				m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, PbBookTemp->strMark);
				OnSelchangeTree(NULL,NULL);//Refresh phone book list
				UpdateContactNumForTreeCtrl(m_ht_PC,PCLOC);
				UpdateContactNumForTreeCtrl(m_ht_PC,PCGROUP);
				UpdateData(FALSE);
			}
			break;
		}
		case PCCardLOC:
			{			
				StPbRecord* PbBookTemp = NULL;
				PbBookTemp = (StPbRecord*)m_lstPhoneBook.GetItemData(PbNum);
				dlg.m_strName = PbBookTemp->strName;
				dlg.m_strMobile = PbBookTemp->strMobile;
// 				dlg.m_strPreName=PbBookTemp->strName;
// 				dlg.m_strPreMobile=PbBookTemp->strMobile;
// 				dlg.m_pDataCardPbRecord=pDataCardPbRecord;
				dlg.m_bSimCardFlag=TRUE;
				if (dlg.DoModal() == IDOK)
				{
					m_iContactType = CONTACTDLG_TYPE_MODIFY;
					wcscpy(PbBookTemp->strName,(LPCTSTR)dlg.m_strName);
					wcscpy(PbBookTemp->strMobile,(LPCTSTR)dlg.m_strMobile);
					bool bRes = WriteToUSIM(PbBookTemp,0);
					if (bRes)
					{
						m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_NAME, PbBookTemp->strName);
						m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, PbBookTemp->strMobile);    
					}
				}
				break;
		}
		case USIMLOC:
		{			
			StPbRecord* PbBookTemp = NULL;
			PbBookTemp = (StPbRecord*)m_lstPhoneBook.GetItemData(PbNum);
			dlg.m_strName = PbBookTemp->strName;
			dlg.m_strMobile = PbBookTemp->strMobile;
			dlg.m_bSimCardFlag=TRUE;
			if (dlg.DoModal() == IDOK)
			{
				m_iContactType = CONTACTDLG_TYPE_MODIFY;
				wcscpy(PbBookTemp->strName,(LPCTSTR)dlg.m_strName);
				wcscpy(PbBookTemp->strMobile,(LPCTSTR)dlg.m_strMobile);
				bool bRes = WriteToUSIM(PbBookTemp,1);
				if (bRes)
				{
					m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_NAME, PbBookTemp->strName);
					m_lstPhoneBook.SetItemText(PbNum, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, PbBookTemp->strMobile);    
				}
			}
			break;
		}
		default:
			break;
	}   
}

void CPhoneBookDlg::OnButtonDelete() 
{
    // TODO: Add your control notification handler code here
    POSITION  pos = m_lstPhoneBook.GetFirstSelectedItemPosition();
    if (!pos)
    {
		AfxMessageBox(IDS_PB_NULLDEL);
        return;
    }
	if(!m_bEditSim)//如果是编辑SIM卡电话本则不显示删除警告对话框
    {
		if (AfxMessageBox(IDS_DELETECONFIRM, MB_YESNO) == IDNO) 
		{
			return;
		}
	}
	// disable button and listctrl
	UpdateButtonStatus(false);

	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);	
	AddPrefixName(strAppName);

	LVITEM item;
	item.mask = LVIF_IMAGE;	
	item.iSubItem = 0;
	int flag = 0; 	
	
	m_nPCCount = 0;
	m_nPCCardCount = 0;
	m_nUSIMCount = 0;

	//获取各存储器中的个数
	while(pos)
	{
		int nItem = m_lstPhoneBook.GetNextSelectedItem(pos);
		item.iItem = nItem;
		m_lstPhoneBook.GetItem(&item);
		switch (item.iImage)
		{
			case PCLOC:
			{
				m_nPCCount++;
				break;
			}
			case PCCardLOC:
			{
				m_nPCCardCount++;
				break;
			}
			case USIMLOC:
			{
				m_nUSIMCount++;
				break;
			}
			default:
				break;
		}
	}

	if(m_nPCCount > 0)
	{
		if (NULL != m_pCRecordIndex)
		{
			delete []m_pCRecordIndex;
			m_pCRecordIndex = NULL;
		}
		
		m_pCRecordIndex = new int[m_nPCCount];
		memset(m_pCRecordIndex, 0, (sizeof(int) * m_nPCCount));

		if (NULL != m_PCRecordAds)
		{
			delete []m_PCRecordAds;
			m_PCRecordAds = NULL;
		}
		
		m_PCRecordAds = new DWORD[m_nPCCount];
		memset(m_PCRecordAds, 0, (sizeof(DWORD) * m_nPCCount));
	}

	if(m_nPCCardCount > 0)
	{
		if (NULL != m_PCCardRecordAds)
		{
			delete []m_PCCardRecordAds;
			m_PCCardRecordAds = NULL;
		}
		
		m_PCCardRecordAds = new DWORD[m_nPCCardCount];
		memset(m_PCCardRecordAds, 0, (sizeof(DWORD) * m_nPCCardCount));
	}

	if(m_nUSIMCount > 0)
	{
		if (NULL != m_USIMRecordAds)
		{
			delete []m_USIMRecordAds;
			m_USIMRecordAds = NULL;
		}
		
		m_USIMRecordAds = new DWORD[m_nUSIMCount]; 
		memset(m_USIMRecordAds, 0, (sizeof(DWORD) * m_nUSIMCount));
	}

	int nPCArrIndex = 0;
	int nPCCardArrIndex = 0;
	int nUSIMArrIndex = 0;
	POSITION  SelDelPos = m_lstPhoneBook.GetFirstSelectedItemPosition();			

	while(SelDelPos)
	{
		int nSelItem = m_lstPhoneBook.GetNextSelectedItem(SelDelPos);
		item.iItem = nSelItem;	
		m_lstPhoneBook.GetItem(&item);
		switch (item.iImage)
		{
			case PCLOC:
			{
				m_pCRecordIndex[nPCArrIndex]            = nSelItem;
				m_PCRecordAds[nPCArrIndex]              = m_lstPhoneBook.GetItemData(nSelItem);
				nPCArrIndex++;

				break;
			}
			case PCCardLOC:
			{
				m_PCCardRecordAds[nPCCardArrIndex ++]   = m_lstPhoneBook.GetItemData(nSelItem);

				break;
			}
			case USIMLOC:
			{
				m_USIMRecordAds[nUSIMArrIndex ++]      = m_lstPhoneBook.GetItemData(nSelItem);
 
				break;
			}
			default:
				break;
		}
	}

	/*delete PC LOC data*/
	int nPCCount = m_nPCCount;
	int nPcArrIndex = 0;
	while(nPCCount > 0)
	{
		CFile FileTemp;
		StPbRecord *PbBookTemp = (StPbRecord *)m_PCRecordAds[nPcArrIndex];

		int nLen = wcslen(PbBookTemp->strPicPath);
		if (nLen > 0)
		{
			BOOL BRes = FileTemp.Open(PbBookTemp->strPicPath, CFile::modeRead);
			if (BRes)
			{
				FileTemp.Close();
				TRY
				{
					FileTemp.Remove( PbBookTemp->strPicPath );
				}
				CATCH(CFileException, e )
				END_CATCH
			}
		}
		if(!m_pPbData->DelSigle(PbBookTemp))
		{
			//AfxMessageBox("delete PC memory error!");					
			CString strDelPcErr;
			strDelPcErr.LoadString(IDS_PB_DELPCERR);
			this->MessageBox(strDelPcErr,strAppName,MB_OK);

			UpdateButtonStatus();
			return;
		}
		
		nPCCount --;
		nPcArrIndex ++;
	}

	/*delete PCCard LOC data*/
	m_nDCCount = m_nPCCardCount;
	if(m_nDCCount > 0)
	{		

		
		m_RecordTempAds = m_PCCardRecordAds;

		StPbRecord *PbBookTemp = (StPbRecord *)*m_RecordTempAds;
		bool bRes = AtWriteARecord(_T(""),_T(""), PbBookTemp->index,0);
		if (!bRes)
		{
			//AfxMessageBox("delete PCCard memory error!");
			CString strDelPcCardErr;
			strDelPcCardErr.LoadString(IDS_PB_DELPCCARDERR);
			this->MessageBox(strDelPcCardErr,strAppName,MB_OK);

			ReDrawListCtrl();

			UpdateButtonStatus();

			return;
		}
	}

	/*delete USIM loc data*/
	m_nDCCount = m_nUSIMCount;
	if(m_nDCCount > 0)
	{		
		m_RecordTempAds = m_USIMRecordAds;

		StPbRecord *PbBookTemp = (StPbRecord *)*m_RecordTempAds;
		bool bRes = AtWriteARecord(_T(""),_T(""), PbBookTemp->index,1);
		if (!bRes)
		{
			//AfxMessageBox("delete USIM LOC error!");
			CString strDelUSIMErr;
			strDelUSIMErr.LoadString(IDS_PB_DELUSIMERR);
			this->MessageBox(strDelUSIMErr,strAppName,MB_OK);

			ReDrawListCtrl();

			UpdateButtonStatus();

			return;
		}
	}
	int nCount = m_lstPhoneBook.GetSelectedCount();
	int* nDelItem = NULL;
	nDelItem = new int[nCount];
	memset(nDelItem,0,sizeof(int) * nCount);
	POSITION  DelSelPos = m_lstPhoneBook.GetFirstSelectedItemPosition();
	int nIndex = 0;
	while(DelSelPos)
	{
		nDelItem[nIndex ++] = m_lstPhoneBook.GetNextSelectedItem(DelSelPos);
	}
	for(int i = nCount - 1; i >= 0; i --)
	{
		m_lstPhoneBook.DeleteItem(nDelItem[i]);

	}
	delete []nDelItem;

	UpdateButtonStatus();
	switch(nCurSelIndex)
	{
	case PCLOC:
		UpdateContactNumForTreeCtrl(m_ht_PC,PCLOC);
		UpdateContactNumForTreeCtrl(m_ht_PC,PCGROUP);
		break;
	case PCGROUP:
		UpdateContactNumForTreeCtrl(m_ht_PC,PCLOC);
		UpdateContactNumForTreeCtrl(m_ht_PC,PCGROUP);
		break;
	case PCCardLOC:
		UpdateContactNumForTreeCtrl(m_ht_PCCard,PCCardLOC);
		break;
	case USIMLOC:
		UpdateContactNumForTreeCtrl(m_ht_USIM,USIMLOC);
		break;
	}

	return;
}

bool CPhoneBookDlg::WriteToUSIM(StPbRecord *PbBookPC,int Flag)   
{
	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);
	AddPrefixName(strAppName);
	CString strTemp = _T("");
	CString strUsimFull;
	if(Flag==1) //USIM card
		strUsimFull.LoadString(IDS_PBTRANSFER_USIMFULL);
	else //USB Modem
		strUsimFull.LoadString(IDS_PBTRANSFER_CARDFULL);
	
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

if(Flag==1)
{
#ifndef FEATURE_VERSION_NOSIM
	if(strNameLenTemp.GetLength() > PB_NAME_PC_MAX)
	{
		strTemp = strNameLenTemp.Left(PB_NAME_SIM_MAX);
		memcpy(PbBookPC->strName,strTemp,PB_NAME_MAX);
	}

	/*判断名字中是否有中文，有则判断名字长度是否超过6，超过则截取前6位*/
	CString strMaxName;
	int iByte = 0;
	strMaxName.Format(PbBookPC->strName);
	bool bCheckRes = CheckNameUnicode(strMaxName,&iByte);
	if(!bCheckRes)
	{
		strTemp = strMaxName.Left(iByte);
		memcpy(PbBookPC->strName,strTemp,PB_NAME_MAX);
	}
#endif
}
else  
   strTemp=PbBookPC->strName;


    if (temp != PbBookPC->strMobile)
    {
		CString NameTemp;
		NameTemp = PbBookPC->strName ;
		::ResetEvent(g_BGPassEvt);

		//区分add还是modify
		if(m_iContactType == CONTACTDLG_TYPE_ADD)
		{
			int nIdleIndex;
			if(Flag==1)
				nIdleIndex= GetFirstIdleIndex(pUSIMPbRecord, g_USimTotalNum);
			else
				nIdleIndex= GetFirstIdleIndex(pDataCardPbRecord, g_DataCardTotalNum);
			if (-1 == nIdleIndex)
			{
				::MessageBox(m_Hwnd,strUsimFull,strAppName,MB_OK);

				return false;
			}
			bool bMobile;
			if(Flag==1) //USIM Card
				 bMobile = AtWriteARecord2(NameTemp,PbBookPC->strMobile, nIdleIndex + 1,1);
			else
				bMobile = AtWriteARecord2(NameTemp,PbBookPC->strMobile, nIdleIndex + 1,0);
			::SetEvent(g_BGPassEvt);
			if (!bMobile)
			{
				return false;
			}
		}
		else if (m_iContactType == CONTACTDLG_TYPE_MODIFY)
		{
			bool bMobile;
			if(Flag==1) //USIM card
				bMobile = AtWriteARecord2(NameTemp,PbBookPC->strMobile, PbBookPC->index,1);        
			else //USB Modem
				bMobile = AtWriteARecord2(NameTemp,PbBookPC->strMobile, PbBookPC->index,0);        
			::SetEvent(g_BGPassEvt);
			if (!bMobile)
			{
				return false;
			}
		}   
    } 
    return true;
}

CString CPhoneBookDlg::GetPicPath() 
{
	TCHAR fPathTemp[MAX_PATH];
	CString strNewPath;
	
	::GetCurrentDirectory(MAX_PATH, fPathTemp);
	strNewPath = fPathTemp;
	strNewPath.Insert(strNewPath.GetLength() + 1,_T("\\pic\\"));

	return strNewPath;
}

bool CPhoneBookDlg::DeleteAllPCLOCItem()
{
	if(!m_pPbData->DeleteAllItems())
	{
		AfxMessageBox(IDS_PB_DELERR);
		ReDrawListCtrl();

		return false;            
	}	

	CString strDirPath;
	CString DirName;
	CFileFind tempFind;	
	BOOL IsFinded;

	strDirPath = GetPicPath();
	DirName.Format(strDirPath);
	DirName = DirName + "*.*";

	IsFinded = (BOOL)tempFind.FindFile(DirName);
	while(IsFinded)
	{
		IsFinded = (BOOL)tempFind.FindNextFile();
		if(!(tempFind.IsDots()))
		{
			TCHAR foundFileName[200]; 
			wcscpy(foundFileName,tempFind.GetFileName().GetBuffer(200)); 
			TCHAR tempFileName[200];
			swprintf(tempFileName,_T("%s\\%s"),strDirPath,foundFileName);
			DeleteFile(tempFileName);
		}
	}
	return true;
}

bool CPhoneBookDlg::DeletePCCardAndUSIMLOCItem(int nDelCount,DWORD* LocAddress,int flag)
{
	m_nDCCount = nDelCount;

	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);	
	AddPrefixName(strAppName);

	if(m_nDCCount > 0)
	{		
		m_RecordTempAds = LocAddress;

		StPbRecord *PbBookTemp = (StPbRecord *)*m_RecordTempAds;
		bool bRes =false;
		if(flag)
		{
			bRes = AtWriteARecord(_T(""),_T(""), PbBookTemp->index,1);

			if (!bRes)
			{
				//AfxMessageBox("delete PCCard LOC error!");
				CString strDelPCCardErr;
				strDelPCCardErr.LoadString(IDS_PB_DELPCCARDERR);
				this->MessageBox(strDelPCCardErr,strAppName,MB_OK);

				ReDrawListCtrl();
				return false;
			}
		}
		else
		{
			bRes = AtWriteARecord(_T(""),_T(""), PbBookTemp->index,0);

			if (!bRes)
			{
				//AfxMessageBox("Error when delete USim memory!");
				CString strDelUSimErr;
				strDelUSimErr.LoadString(IDS_PB_DELUSIMERR);
				this->MessageBox(strDelUSimErr,strAppName,MB_OK);

				ReDrawListCtrl();
				return false;
			}
		}
		
	}
	return true;
}

void CPhoneBookDlg::OnButtonDeleteAll() 
{
    // TODO: Add your control notification handler code here
    if (AfxMessageBox(IDS_DELETECONFIRM, MB_YESNO) == IDNO) 
    {
        return;
    }
	UpdateButtonStatus(false);

	m_nPCCount = 0;
	m_nPCCardCount = 0;
	m_nUSIMCount = 0;

    //delete all datas
	switch (nCurSelIndex)
	{
		case PCLOC:
		{
			DeleteAllPCLOCItem();	
			m_lstPhoneBook.DeleteAllItems();
			break;
		}
		case PCCardLOC:
		{
			for(int i =0;i< g_DataCardTotalNum;i++)
			{
				if(-1 !=pDataCardPbRecord[i].index)
				{
					m_nPCCardCount++;
				}
			}

			if(m_nPCCardCount > 0)
			{
				if (NULL != m_PCCardRecordAds)
				{
					delete []m_PCCardRecordAds;
					m_PCCardRecordAds = NULL;
				}
				
				m_PCCardRecordAds = new DWORD[m_nPCCardCount];
				memset(m_PCCardRecordAds, 0, (sizeof(DWORD) * m_nPCCardCount));
			}

			int nIndex = 0;
			for(int j = 0;j < g_DataCardTotalNum; j++)
			{
				if(-1 != pDataCardPbRecord[j].index)
				{
					m_PCCardRecordAds[nIndex++] = (DWORD)(&pDataCardPbRecord[j]);					
				}
			}
			
			bool bRes = DeletePCCardAndUSIMLOCItem(m_nPCCardCount,m_PCCardRecordAds,0);
			if(!bRes)
			{
				ReDrawListCtrl();
				UpdateButtonStatus();
				return;
			}
			m_lstPhoneBook.DeleteAllItems();
			break;
		}

		case USIMLOC:
		{
			for(int i = 0;i< g_USimTotalNum;i++)
			{
				if(-1 !=pUSIMPbRecord[i].index)
				{
					m_nUSIMCount++;
				}
			}

			if(m_nUSIMCount > 0)
			{
				if (NULL != m_USIMRecordAds)
				{
					delete []m_USIMRecordAds;
					m_USIMRecordAds = NULL;
				}				
				m_USIMRecordAds = new DWORD[m_nUSIMCount];
				memset(m_USIMRecordAds, 0, (sizeof(DWORD) * m_nUSIMCount));
			}

			int nIndex = 0;
			for(int j = 0;j< g_USimTotalNum;j++)
			{
				if(-1 != pUSIMPbRecord[j].index)
				{
					m_USIMRecordAds[nIndex++] = (DWORD)(&pUSIMPbRecord[j]);					
				}
			}

			bool bRes = DeletePCCardAndUSIMLOCItem(m_nUSIMCount,m_USIMRecordAds,1);
			if(!bRes)
			{
				ReDrawListCtrl();
				UpdateButtonStatus();
				return;
			}
			m_lstPhoneBook.DeleteAllItems();
			break;
		}

		case ALLLOC:
		{
			bool bRes = false;
			bRes = DeleteAllPCLOCItem();
			if(!bRes)
			{
				ReDrawListCtrl();
				UpdateButtonStatus();
				return;
			}

			int i = 0;
			for(i = 0;i < g_DataCardTotalNum;i++)
			{
				if(-1 != pDataCardPbRecord[i].index)
				{
					m_nPCCardCount++;
				}
			}

			if(m_nPCCardCount > 0)
			{
				if (NULL != m_PCCardRecordAds)
				{
					delete []m_PCCardRecordAds;
					m_PCCardRecordAds = NULL;
				}
				
				m_PCCardRecordAds = new DWORD[m_nPCCardCount];
				memset(m_PCCardRecordAds, 0, (sizeof(DWORD) * m_nPCCardCount));
			}

			int nIndex = 0;
			int j = 0;
			for(j = 0; j < g_DataCardTotalNum; j++)
			{
				if(-1 !=pDataCardPbRecord[j].index)
				{
					m_PCCardRecordAds[nIndex++] = (DWORD)(&pDataCardPbRecord[j]);					
				}
			}
			
			bRes = DeletePCCardAndUSIMLOCItem(m_nPCCardCount,m_PCCardRecordAds,0);
			if(!bRes)
			{
				ReDrawListCtrl();
				UpdateButtonStatus();
				return;
			}
			
			for(i = 0;i < g_USimTotalNum;i++)
			{
				if(-1 !=pUSIMPbRecord[i].index)
				{
					m_nUSIMCount++;
				}
			}

			if(m_nUSIMCount > 0)
			{
				if (NULL != m_USIMRecordAds)
				{
					delete []m_USIMRecordAds;
					m_USIMRecordAds = NULL;
				}				
				m_USIMRecordAds = new DWORD[m_nUSIMCount];
				memset(m_USIMRecordAds, 0, (sizeof(DWORD) * m_nUSIMCount));
			}

			nIndex = 0;
			for(j = 0; j < g_USimTotalNum ; j++)
			{
				if(-1 != pUSIMPbRecord[j].index)
				{
					m_USIMRecordAds[nIndex++] = (DWORD)(&pUSIMPbRecord[j]);					
				}
			}

			bRes = DeletePCCardAndUSIMLOCItem(m_nUSIMCount,m_USIMRecordAds,1);
			if(!bRes)
			{
				ReDrawListCtrl();
				UpdateButtonStatus();
				return;
			}

			m_lstPhoneBook.DeleteAllItems();

			break;
		}
		default:
			break;
	}   
	
	UpdateButtonStatus();

	return;
}

void CPhoneBookDlg::OnButtonExport() 
{
    IGenerator phbHandle(this);
	bool nRes = phbHandle.SetPath(EXP_PHB);
	phbHandle.RecoverPath();
    if (!nRes) 
    {
        return;
    }

	//判断是导出全部还是导出部分电话薄记录
	POSITION pos = m_lstPhoneBook.GetFirstSelectedItemPosition();
	if (!pos)
	{
		if (m_pPbData->CheckNull())
		{
			CString strAppNameTemp;
			strAppNameTemp.LoadString(IDS_APPNAME);
			AddPrefixName(strAppNameTemp);
			CString strPbNull;
			strPbNull.LoadString(IDS_PB_NULLPB);
			this->MessageBox(strPbNull,strAppNameTemp,MB_OK);

			return;
		}
		int nItemCount = m_lstPhoneBook.GetItemCount();
		for(int i = 0;i < nItemCount;i++)
		{
			ExportARecord(&phbHandle,i);      
		}
	}
	else
	{
		while(pos)
		{
			int iLine = m_lstPhoneBook.GetNextSelectedItem(pos);
			ExportARecord(&phbHandle,iLine);				
		}		
	}
    
    phbHandle.MakeFile();
	phbHandle.RecoverPath();
}

//导出一条电话薄记录
void CPhoneBookDlg::ExportARecord(IGenerator* phbHandle,int iIndex)
{
	CString tempCell;
	USES_CONVERSION;

	StPbRecord* phbitem = (StPbRecord* )m_lstPhoneBook.GetItemData(iIndex);
	if (0 == phbHandle->GetFormat())   //0为csv，1为vcf格式
	{
		tempCell.Format(phbitem->strName);
		phbHandle->FillCell(tempCell);
		tempCell.Format(phbitem->strMobile);
		phbHandle->FillCell(tempCell);
		tempCell.Format(phbitem->strHome);
		phbHandle->FillCell(tempCell);
		tempCell.Format(phbitem->strGroup);
		phbHandle->FillCell(tempCell);
		tempCell.Format(phbitem->strCompany);
		phbHandle->FillCell(tempCell);
		tempCell.Format(phbitem->strEmail);
		phbHandle->FillCell(tempCell);
		tempCell.Format(phbitem->strMark);
		phbHandle->FillCell(tempCell);
		phbHandle->FillTrail();

	}
	else
	{
		phbHandle->FillvCardObj(*phbitem);
	}
}



void CPhoneBookDlg::ImportFromCsv(char **pPara, DWORD nLen,LPVOID pHandle) 
{
    char strText[PB_LINE_MAX + 2];  //放置标题栏内容或电话本记录总内容
    char* pIndex = NULL;
    char* pstrToken = NULL;
    char* pstrPrePos = NULL;  ////整个文件内容
    char* pstrCurrPos = NULL;
    char* pstrQuote = NULL;
    char* pstrComma = NULL;
    char* psrcBuf = NULL;
    char* pdest = NULL;   //放置当前处理行的内容

    int nLineNum = 0;    //nLineNum标识当前处理的行数
    int nTopicLen = 0;
    int nLineLen = 0;
    StPbRecord phbitem;
    int nIndex = -1;
    int nLenflag = 0;
    char LF[2] = {0x0A,0};
    char *pdQuote = NULL;
    bool comptag = false;
    
    short nName = 0;
	short nMPhone = 0;
	short nHPhone = 0;
	short nGroup =0;
	short nOPhone = 0;
	short nEmail = 0;
	short nNote = 0;

	CString strName;
	CString strMobile;
	CString strHome;
	CString strGroup;
	CString strCompany;
	CString strEmail;
	CString strRemark;

	//strName.LoadString(IDS_NAME);
	strName=_T("Name");
	strMobile.LoadString(IDS_MOBILE);
	strHome.LoadString(IDS_HOME);
	strGroup.LoadString(IDS_PBGROUP);
	strCompany.LoadString(IDS_COMPANY);
	strEmail.LoadString(IDS_EMAIL);
	strRemark.LoadString(IDS_REMARK);

    psrcBuf = *pPara;
    pstrCurrPos = psrcBuf; 
	pstrPrePos = psrcBuf;  

	bool bRight = true;

	CString strTemp = _T("");
		USES_CONVERSION;
    
	//nLenflag小于文件总长度则仍有数据未处理
	while(nLenflag < nLen)  
	{
        pdest = strstr(pstrCurrPos, LF);

        if(pdest == NULL)
        {
            break;
        }
        else
        {
            nLineNum++;
            if(nLineNum == 1)  //电话本标题栏
            {
                nTopicLen = pdest - psrcBuf + 1;
				if (nTopicLen>PB_LINE_MAX + 2)
				{
					m_pWaitDlg->DestroyWindow();
					CString strAppNameTemp;
					strAppNameTemp.LoadString(IDS_APPNAME);
					AddPrefixName(strAppNameTemp);
					CString strFileErr;
					strFileErr.LoadString(IDS_FILEERR);
					this->MessageBox(strFileErr,strAppNameTemp,MB_OK);  //文件标题无关键字匹配项
					ReDrawListCtrl();
					break;
				}
                memset(strText,0,PB_LINE_MAX + 2);
                strncpy(strText,(const char *)psrcBuf,nTopicLen - 1);
                *(strText + nTopicLen - 1) = ',';
                pstrToken = strtok(strText,",");  //逗号以前的字符串
                pstrCurrPos = pdest + 1;
                nIndex = 1;
				
				/*bFlag:标记导入的文件是否有与电话本的关键项的匹配项，如无则提示文件错误。
				关键项包括Name、Mobile、Home、Office*/
			
				bool bFlag = false;  
                do 
				{
					if (pstrToken)
					{					
						if (strcmp(pstrToken,W2A(strName))== 0) 
						{
							nName = nIndex;
							bFlag = true;
						}
						if (strcmp(pstrToken,W2A(strMobile))== 0) 
						{
							nMPhone = nIndex;
							bFlag = true;
						}
						if (strcmp(pstrToken,W2A(strHome))== 0) 
						{
							nHPhone = nIndex;
							bFlag = true;
						}
						if (strcmp(pstrToken,W2A(strGroup))== 0) 
						{
							nGroup = nIndex;
							bFlag = true;
						}
						if (strcmp(pstrToken,W2A(strCompany))== 0) 
						{
							nOPhone = nIndex;
							bFlag = true;
						}
						if (strcmp(pstrToken,W2A(strEmail))== 0)
						{
							nEmail = nIndex;
						}
						if (strcmp(pstrToken,W2A(strRemark))== 0)
						{
							nNote = nIndex;
						}
					}

                    nIndex++;
					pstrToken = strtok(NULL,",");
                } 
				while ( pstrToken != NULL);

				if (!bFlag)
				{
					CString strAppNameTemp;
					strAppNameTemp.LoadString(IDS_APPNAME);
					AddPrefixName(strAppNameTemp);
					CString strFileErr;
					strFileErr.LoadString(IDS_FILEERR);
					this->MessageBox(strFileErr,strAppNameTemp,MB_OK);  //文件标题无关键字匹配项
					ReDrawListCtrl();

					return;
				}

                nLenflag += nTopicLen;
            }
			else  //处理电话本记录
			{
                nIndex = 1;
                pstrCurrPos = pdest + 1;
                nLineLen = pstrCurrPos - pstrPrePos;
                memset(strText,0,PB_LINE_MAX + 2);
                strncpy(strText,pstrPrePos,nLineLen - 1);
                *(strText + nLineLen - 1) = ',';
                StPbRecord mphb;
                memset(&mphb,0,sizeof(StPbRecord));

                pIndex = strText;
                while (pIndex) 
				{
                    comptag = false;
                    pstrToken = pIndex;
                    pstrQuote = strstr(pIndex,"\"");
                    pstrComma =  strstr(pIndex,",");
                    
					//有引号的格式
                    if ((pstrQuote && (pstrQuote  < pstrComma)) || (pstrQuote && (pstrComma == NULL)))
					{
                        comptag = true;
                        pstrToken = pstrQuote + 1;
						pIndex = pstrQuote + 1;
                        #if 0
                        while ((pstrQuote = strstr(pIndex,"\"")) == NULL)
						{
							/*Not Find The Next Quote*/
                            strcpy(pIndex + strlen(pIndex) - 1, LF);
                        }
                        #endif
                        pstrQuote = strstr(pIndex,"\"");
                        while ((pdQuote = strstr(pIndex,"\"\"")) && (pdQuote < pstrComma)) 
						{                            
                            strncpy(pdQuote, pdQuote + 1,strlen(pdQuote + 1));
                            //pdQuote = NULL;
                            if(pdQuote == pstrQuote)
							{
                                pstrQuote = strstr(pdQuote+1,"\"");
                            }
                        }

                        pIndex = pstrQuote + 1;
                        *pstrQuote = 0;
                    }
					else
					{
                        pIndex = pstrComma;
                    }
                    
                    if(pIndex)
					{
                        *pIndex = 0;
                        pIndex++;
                    }

					strTemp = A2W(pstrToken);
					strTemp.TrimLeft();
					strTemp.TrimRight();

                    if (nName == nIndex)
					{
						//检查姓名是否符合规范
						bool bCheckRes = CheckPbName(W2A(strTemp));
						if(!bCheckRes)
						{
							ReDrawListCtrl();

							return;
						}

						if(VCARD_NAME_MAX < strTemp.GetLength())
						{
							strTemp = strTemp.Left(VCARD_NAME_MAX);							
						}

                        memcpy(mphb.strName,strTemp,strTemp.GetLength()*2);
						//AfxMessageBox(mphb.strName);
					}
					//break
                    if (nMPhone == nIndex )
					{
						//检查号码是否符合规范
						if(!CheckPbNumber(W2A(strTemp)))
						{
							ReDrawListCtrl();

							return;
						}

						if(VCARD_NUM_MAX < strTemp.GetLength())
						{
							strTemp = strTemp.Left(VCARD_NUM_MAX);							
						}
						memset(pstrToken,0,strlen(pstrToken));
						memcpy(pstrToken,strTemp,strTemp.GetLength());
						
						if (!CompareChar(A2W(pstrToken)))
						{
							ReDrawListCtrl();

							return;
						}
                        memcpy(mphb.strMobile,strTemp,strTemp.GetLength()*2);
					}
                    if (nHPhone == nIndex)
					{
						//检查号码是否符合规范
						if(!CheckPbNumber(strTemp))
						{
							ReDrawListCtrl();

							return;
						}

						if(VCARD_NUM_MAX < strTemp.GetLength())
						{
							strTemp = strTemp.Left(VCARD_NUM_MAX);							
						}

						memset(pstrToken,0,strlen(pstrToken));
						memcpy(pstrToken,strTemp,strTemp.GetLength());
						if (!CompareChar(A2W(pstrToken)))
						{
							ReDrawListCtrl();

							return;
						}
                        memcpy(mphb.strHome,strTemp,strTemp.GetLength()*2);
					}
					if (nGroup == nIndex)
					{
						
						if(VCARD_GROUP_MAX < strTemp.GetLength())
						{
							strTemp = strTemp.Left(VCARD_GROUP_MAX);							
						}
						
                        memcpy(mphb.strGroup,strTemp,strTemp.GetLength()*2);
					}

                    if (nOPhone == nIndex)
					{
						//检查号码是否符合规范
						if(!CheckPbNumber(strTemp))
						{
							ReDrawListCtrl();

							return;
						}

						if(VCARD_NUM_MAX < strTemp.GetLength())
						{
							strTemp = strTemp.Left(VCARD_NUM_MAX);							
						}

						memset(pstrToken,0,strlen(pstrToken));
						memcpy(pstrToken,strTemp,strTemp.GetLength());
						if (!CompareChar(A2W(pstrToken)))
						{
							ReDrawListCtrl();

							return;
						}

                        memcpy(mphb.strCompany,strTemp,strTemp.GetLength()*2);
					}
                    if (nEmail == nIndex)
					{						
						if(VCARD_EMAIL_MAX < strTemp.GetLength())
						{
							strTemp = strTemp.Left(VCARD_EMAIL_MAX);							
						}

                        memcpy(mphb.strEmail,strTemp,strTemp.GetLength()*2);
					}
                    if (nNote == nIndex)
					{
						if(VCARD_NOTE_MAX < strTemp.GetLength())
						{
							strTemp = strTemp.Left(VCARD_NOTE_MAX);							
						}

                        memcpy(mphb.strMark,strTemp,strTemp.GetLength()*2);
					}

                    nIndex++;
                }

				//记录为空则不导入此条记录
				if( strlen( W2A(mphb.strName) ) == 0 && strlen( W2A(mphb.strMobile) ) == 0 && 
				strlen( W2A(mphb.strHome) ) == 0 && strlen( W2A(mphb.strCompany) ) == 0)
				{
					bRight = false;
				}
				else
				{
					//((IParser*)pHandle)->RecoverPath();

					phbitem = mphb;
					DWORD PbArrItemAds = NULL;
					if (!m_pPbData->AddTailSingle(phbitem,&PbArrItemAds))
					{
						ReDrawListCtrl();

						return;
					}
				}
                nLenflag += nLineLen;
            }
        }
        pstrPrePos = pdest + 1;
    }
    ReDrawListCtrl();
	if (!bRight)
	{
		CString strAppNameTemp;
		strAppNameTemp.LoadString(IDS_APPNAME);
		AddPrefixName(strAppNameTemp);
		CString strImportErr;
		strImportErr.LoadString(IDS_PB_INPORTERR);		
		this->MessageBox(strImportErr,strAppNameTemp,MB_OK);
	}
}




/*void CPhoneBookDlg::ImportFromVcf(TCHAR **pPara, DWORD nLen, LPVOID pHandle)
{
    TCHAR* psrcBuf = *pPara;
    vCardItem tempItem;
    StPbRecord phbitem;
	bool bRight = true;

    bool bRes = ((IParser*)pHandle)->ImportFromVcf(psrcBuf,nLen);
	if (!bRes)
	{
		return;
	}
	int nLine = ((IParser*)pHandle)->GetvCardItemCount();
	if (0 >= nLine)
	{
		CString strAppNameTemp;
		strAppNameTemp.LoadString(IDS_APPNAME);
		AddPrefixName(strAppNameTemp);
		CString strPbNull;
		strPbNull.LoadString(IDS_PB_NULLPB);
		this->MessageBox(strPbNull,strAppNameTemp,MB_OK);

		return;
	}
	if(PB_RECORD_MAX < nLine)
	{
		nLine = PB_RECORD_MAX;
	}
    for(int i = 0;i < nLine;i++)
	{
		((IParser*)pHandle)->GetvCardItem(i,&tempItem);

		//检查姓名是否符合规范
		TCHAR *ucNameTemp = tempItem.strName;
		
		int nLen = strlen(tempItem.strName);
		char *ucNameTemp = new char[nLen];
		memset(ucNameTemp,0,nLen);
		memcpy(ucNameTemp,tempItem.strName,nLen);
		
		bool bCheckRes = CheckPbNameUnicode(ucNameTemp);
		
		if(!bCheckRes)
		{
			ReDrawListCtrl();

			return;
		}
        wcscpy(phbitem.strName,tempItem.strName);

		if(!CheckPbNumber(tempItem.strNumberCell))
		{
			ReDrawListCtrl();

			return;
		}
		if (!CompareChar(tempItem.strNumberCell))
		{
			ReDrawListCtrl();

			return;
		}
        wcscpy(phbitem.strMobile,tempItem.strNumberCell);

		if(!CheckPbNumber(tempItem.strNumberHome))
		{
			ReDrawListCtrl();

			return;
		}
		if (!CompareChar(tempItem.strNumberHome))
		{
			ReDrawListCtrl();

			return;
		}
        wcscpy(phbitem.strHome,tempItem.strNumberHome);

		if(!CheckPbNumber(tempItem.strNumberWork))
		{
			ReDrawListCtrl();

			return;
		}
		if (!CompareChar(tempItem.strNumberWork))
		{
			ReDrawListCtrl();

			return;
		}
        wcscpy(phbitem.strCompany,tempItem.strNumberWork);

        wcscpy(phbitem.strEmail,tempItem.strEmail);
        wcscpy(phbitem.strMark,tempItem.strNote);

		if( wcslen( phbitem.strName ) == 0 && wcslen( phbitem.strMobile ) == 0 && 
			wcslen( phbitem.strHome ) == 0 && wcslen( phbitem.strCompany ) == 0)
		{
			bRight = false;
		}
		else
		{
			DWORD PbArrItemAds = NULL;
			((IParser*)pHandle)->RecoverPath();
			if (!m_pPbData->AddTailSingle(phbitem,&PbArrItemAds))
			{
				ReDrawListCtrl();
				return;
			}
		}
    }
    
    ReDrawListCtrl();
	if (!bRight)
	{
		CString strAppNameTemp;
		strAppNameTemp.LoadString(IDS_APPNAME);
		AddPrefixName(strAppNameTemp);
		CString strImportErr;
		strImportErr.LoadString(IDS_PB_INPORTERR);
		this->MessageBox(strImportErr,strAppNameTemp,MB_OK);
	}
}
*/

void CPhoneBookDlg::ImportFromVcf(char **pPara, DWORD nLen, LPVOID pHandle)
{
    char* psrcBuf = *pPara;
    vCardItem tempItem;
    StPbRecord phbitem;
	bool bRight = true;

    bool bRes = ((IParser*)pHandle)->ImportFromVcf(psrcBuf,nLen);
	if (!bRes)
	{
		return;
	}
	int nLine = ((IParser*)pHandle)->GetvCardItemCount();
	if (0 >= nLine)
	{
		CString strAppNameTemp;
		strAppNameTemp.LoadString(IDS_APPNAME);
		AddPrefixName(strAppNameTemp);
		CString strPbNull;
		strPbNull.LoadString(IDS_PB_NULLPB);
		this->MessageBox(strPbNull,strAppNameTemp,MB_OK);

		return;
	}
	if(PB_RECORD_MAX < nLine)
	{
		nLine = PB_RECORD_MAX;
	}

	USES_CONVERSION;
    for(int i = 0;i < nLine;i++)
	{
		((IParser*)pHandle)->GetvCardItem(i,&tempItem);

		//检查姓名是否符合规范
		char *ucNameTemp = tempItem.strName;
		/*
		int nLen = strlen(tempItem.strName);
		char *ucNameTemp = new char[nLen];
		memset(ucNameTemp,0,nLen);
		memcpy(ucNameTemp,tempItem.strName,nLen);
		*/
		bool bCheckRes = CheckPbName(ucNameTemp);
		/*
		if(NULL != ucNameTemp)
		{
			delete []ucNameTemp;
			ucNameTemp = NULL;
		}
		*/
		if(!bCheckRes)
		{
			ReDrawListCtrl();

			return;
		}
        wcscpy(phbitem.strName,(LPCTSTR)tempItem.strName);

		if(!CheckPbNumber(tempItem.strNumberCell))
		{
			ReDrawListCtrl();

			return;
		}
		if (!CompareChar(A2W(tempItem.strNumberCell)))
		{
			ReDrawListCtrl();

			return;
		}
        wcscpy(phbitem.strMobile,A2W(tempItem.strNumberCell));

		if(!CheckPbNumber(tempItem.strNumberHome))
		{
			ReDrawListCtrl();

			return;
		}
		if (!CompareChar(A2W(tempItem.strNumberHome)))
		{
			ReDrawListCtrl();

			return;
		}
        wcscpy(phbitem.strHome,A2W(tempItem.strNumberHome));
		wcscpy(phbitem.strGroup,(LPCTSTR)tempItem.strGroup);

		if(!CheckPbNumber(tempItem.strNumberWork))
		{
			ReDrawListCtrl();

			return;
		}
		if (!CompareChar(A2W(tempItem.strNumberWork)))
		{
			ReDrawListCtrl();

			return;
		}
        wcscpy(phbitem.strCompany,A2W(tempItem.strNumberWork));
		wcscpy(phbitem.strEmail,(LPCTSTR)tempItem.strEmail);
        wcscpy(phbitem.strMark,(LPCTSTR)tempItem.strNote);

		if( wcslen( phbitem.strName ) == 0 && wcslen( phbitem.strMobile ) == 0 && 
			wcslen( phbitem.strHome ) == 0 && wcslen( phbitem.strCompany ) == 0)
		{
			bRight = false;
		}
		else
		{
			DWORD PbArrItemAds = NULL;
			((IParser*)pHandle)->RecoverPath();
			if (!m_pPbData->AddTailSingle(phbitem,&PbArrItemAds))
			{
				ReDrawListCtrl();
				return;
			}
		}
    }
    
    ReDrawListCtrl();
	if (!bRight)
	{
		CString strAppNameTemp;
		strAppNameTemp.LoadString(IDS_APPNAME);
		AddPrefixName(strAppNameTemp);
		CString strImportErr;
		strImportErr.LoadString(IDS_PB_INPORTERR);
		this->MessageBox(strImportErr,strAppNameTemp,MB_OK);
	}
}

void CPhoneBookDlg::OnDblclkListPhonebook(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    OnButtonModify();
    *pResult = 0;
}

void CPhoneBookDlg::OnRclickListPhonebook(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    POINT myPoint;

    CMenu myMenu;
    CString strMenuName;
    myMenu.CreatePopupMenu();    
    strMenuName.LoadString(IDS_PBMENU_ADDPB);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_ADD,strMenuName);

	strMenuName.LoadString(IDS_PBMENU_MODIFY);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_MODIFY,strMenuName);
    strMenuName.LoadString(IDS_PBMENU_DELETE);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_DELETE,strMenuName);

	strMenuName.LoadString(IDS_PBMENU_GROUP);
	myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_GROUP,strMenuName);

	
	//if(g_SetData.Main_nCall)
	
// 	strMenuName.LoadString(IDS_PBMENU_DELETEALL);
// 	myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_DELETE_ALL,strMenuName);

 //   myMenu.AppendMenu(MF_SEPARATOR);

	//if(g_SetData.Main_nCall)
	strMenuName.LoadString(IDS_PBMENU_CALL);
	myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_CALL,strMenuName);

	
    strMenuName.LoadString(IDS_PB_SENDSMS);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_SENDSMS,strMenuName);

    strMenuName.LoadString(IDS_PBMENU_SEARCHPB);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_SEARCH,strMenuName);
    
    myMenu.AppendMenu(MF_SEPARATOR);

	#ifndef FEATURE_VERSION_NOSIM
	    strMenuName.LoadString(IDS_PBMENU_TRANSFER);
		myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_TRANSFER,strMenuName);
	#endif


		strMenuName.LoadString(IDS_PBMENU_IMPORTPB);
		myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_IMPORT,strMenuName);
	

    strMenuName.LoadString(IDS_PBMENU_EXPORTPB);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_EXPORT,strMenuName);
    int PbNum = m_lstPhoneBook.GetNextItem(-1, LVNI_SELECTED);
    if (-1 == PbNum)
    {
        myMenu.EnableMenuItem(IDC_BUTTON_DELETE,MF_GRAYED);
        myMenu.EnableMenuItem(IDC_BUTTON_MODIFY,MF_GRAYED);        
        
        myMenu.EnableMenuItem(IDC_BUTTON_CALL,MF_GRAYED);
        myMenu.EnableMenuItem(IDC_BUTTON_SENDSMS,MF_GRAYED);        
    }

	if(nCurSelIndex!=PCLOC)
	{
		myMenu.EnableMenuItem(IDC_BUTTON_IMPORT,MF_GRAYED);
	}
    
	if(g_bIsExist && g_bSimReady)
	{
		myMenu.EnableMenuItem(IDC_BUTTON_SENDSMS,MF_ENABLED);
		myMenu.EnableMenuItem(IDC_BUTTON_TRANSFER,MF_ENABLED);
		
	}
	else
	{
		myMenu.EnableMenuItem(IDC_BUTTON_SENDSMS,MF_GRAYED);
		myMenu.EnableMenuItem(IDC_BUTTON_TRANSFER,MF_GRAYED);
		
	}

    NMITEMACTIVATE * newP;
    newP = (NMITEMACTIVATE *)pNMHDR;
    myPoint.x = newP->ptAction.x;
    myPoint.y = newP->ptAction.y;
    ClientToScreen (&myPoint) ;

    myMenu.TrackPopupMenu(TPM_RIGHTBUTTON,myPoint.x + 15,myPoint.y + 15,this,NULL);
    
    *pResult = 0;
}

BOOL CPhoneBookDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    // TODO: Add your specialized code here and/or call the base class
    switch(wParam)
    {
        case IDC_BUTTON_ADD:
        {
            OnButtonAdd();

            return true;
        }
        case IDC_BUTTON_TRANSFER:
        {
            OnButtonTransfer();

            return true;
        }
        case IDC_BUTTON_MODIFY:
        {
            OnButtonModify();

            return true;
        }
        case IDC_BUTTON_DELETE:
        {
            OnButtonDelete(); 

            return true;
        }
        case IDC_BUTTON_EXPORT:
        {
            OnButtonExport();

            return true;
        }
        case IDC_BUTTON_CALL:
        {
            OnButtonCall();

            return true;
        }
        case IDC_BUTTON_SENDSMS:
        {
            OnButtonSendsms();

            return true;
        }
        case IDC_BUTTON_SEARCH:
        {
            OnButtonSearch();

            return true;
        }
        case IDC_BUTTON_DELETE_ALL:
        {
            OnButtonDeleteAll();

            return true;
        }
        case IDC_BUTTON_IMPORT:
        {
            OnButtonImport();

            return true;
        }
		default:
		{
			break;
		}
    }    
    return CTabedDialog::OnCommand(wParam, lParam);
}

void CPhoneBookDlg::OnButtonSearch() 
{
    // TODO: Add your control notification handler code here
    if (NULL != m_pDlg)
    {
        delete m_pDlg;
    }

    m_pDlg = new CSearchContactDlg(this);
    //m_pDlg->Create(IDD_DIALOG_SEARCH,this);
    //m_pDlg->ShowWindow(SW_SHOW);
	m_pDlg->DoModal();
}

LRESULT CPhoneBookDlg::GetListTotalCount(WPARAM wParam, LPARAM lParam)
{
	int* pwParam = (int*)wParam;
	*pwParam = m_lstPhoneBook.GetItemCount();
	return 0;
}

bool CPhoneBookDlg :: CanMatch( CString src, CString substr )
{
	int nIndex = -1;
	nIndex = src.Find( (LPCTSTR)substr);
    if((substr.GetLength() == 0)||(nIndex == 0))
    {
        return true;
    }
    return false;
}

LRESULT CPhoneBookDlg::SearchContactInList(WPARAM wParam, LPARAM lParam)
{
	int* pwParam = (int*)wParam;  //下标数组
	int* plParam = (int*)lParam;  //总数
	m_StrSearchKeyofName = m_pDlg->m_strName;
	m_StrSearchKeyofMobile = m_pDlg->m_strMobile;
	int nTodalCount = m_lstPhoneBook.GetItemCount();

	if(m_StrSearchKeyofName.GetLength() == 0 && m_StrSearchKeyofMobile.GetLength() == 0)
	{
		return 0;
	}
    *plParam = 0;
    for(int nItem = 0; nItem < nTodalCount; nItem++)
    {
        CString strName = m_lstPhoneBook.GetItemText(nItem,0);
        CString strMobile = m_lstPhoneBook.GetItemText(nItem,1);

        if( CanMatch( strName,m_StrSearchKeyofName ) && CanMatch( strMobile,m_StrSearchKeyofMobile ) )
        {
			pwParam[ ( *plParam ) ++] = nItem;			
        }
    }
	return 0;
}

LRESULT CPhoneBookDlg::OnSearchItem(WPARAM wParam, LPARAM lParam)
{
    this->SetFocus();
    m_lstPhoneBook.SetItemState(-1, NULL, LVIS_SELECTED|LVIS_FOCUSED);
    m_lstPhoneBook.SetItem((int)wParam, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, 
                            LVIS_SELECTED, 0);
    m_lstPhoneBook.EnsureVisible((int)wParam,true); 
    return 0;
}

LRESULT CPhoneBookDlg::ReDrawListCtrl(WPARAM wParam, LPARAM lParam)
{
    m_lstPhoneBook.DeleteAllItems();
	m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
	UpdateContactNumForTreeCtrl(m_ht_PC,PCLOC);
	UpdateContactNumForTreeCtrl(m_ht_USIM,USIMLOC);
	UpdateContactNumForTreeCtrl(m_ht_PCCard,PCCardLOC);
    //填入数据
	switch (nCurSelIndex)
	{
		case PCLOC:
		{
			int nindex = 0;
			for(int i = 0; i < PB_RECORD_MAX; i++)
			{
				if(-1 != m_pPbData->PbDatas[i].index)
				{
					m_lstPhoneBook.InsertItem(nindex, m_pPbData->PbDatas[i].strName, (DWORD)(&m_pPbData->PbDatas[i]),PCLOC);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, m_pPbData->PbDatas[i].strMobile);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, m_pPbData->PbDatas[i].strCompany);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, m_pPbData->PbDatas[i].strHome);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_GROUP, m_pPbData->PbDatas[i].strGroup);	
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, m_pPbData->PbDatas[i].strEmail);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, m_pPbData->PbDatas[i].strMark);
					nindex ++;
				}
			}
			break;
		}
		case PCGROUP:
		{
				int nindex = 0;
				for(int i = 0; i < PB_RECORD_MAX; i++)
				{
					if(-1 != m_pPbData->PbDatas[i].index && wcscmp(m_pPbData->PbDatas[i].strGroup,m_strCurSelGroup)==0 )
					{
						m_lstPhoneBook.InsertItem(nindex, m_pPbData->PbDatas[i].strName, (DWORD)(&m_pPbData->PbDatas[i]),PCLOC);
						m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, m_pPbData->PbDatas[i].strMobile);
							m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, m_pPbData->PbDatas[i].strCompany);
						m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, m_pPbData->PbDatas[i].strHome);
						m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_GROUP, m_pPbData->PbDatas[i].strGroup);
						m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, m_pPbData->PbDatas[i].strEmail);
						m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, m_pPbData->PbDatas[i].strMark);
						nindex ++;
					}
				}
				break;
		}

		case PCCardLOC:
		{
			int nindex = 0;
			for(int i = 0; i < g_DataCardTotalNum; i++)
			{
				if (-1 != pDataCardPbRecord[i].index)
				{
					m_lstPhoneBook.InsertItem(nindex, pDataCardPbRecord[i].strName, 
						                      (DWORD)(&pDataCardPbRecord[i]),PCCardLOC);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, 
											  pDataCardPbRecord[i].strMobile);
					nindex ++;
				}
			}
			m_lstPhoneBook.DeleteColumn(6);
			m_lstPhoneBook.DeleteColumn(5);
			m_lstPhoneBook.DeleteColumn(4);
			m_lstPhoneBook.DeleteColumn(3);
			m_lstPhoneBook.DeleteColumn(2);
			break;
		}
		case USIMLOC:
		{
			int nindex = 0;
			for(int i = 0; i < g_USimTotalNum; i++)
			{
				if (-1 != pUSIMPbRecord[i].index)
				{
					m_lstPhoneBook.InsertItem(nindex, pUSIMPbRecord[i].strName, 
						                      (DWORD)(&pUSIMPbRecord[i]),USIMLOC);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, 
											  pUSIMPbRecord[i].strMobile);
					nindex ++;
				}
			}
			m_lstPhoneBook.DeleteColumn(6);
			m_lstPhoneBook.DeleteColumn(5);
			m_lstPhoneBook.DeleteColumn(4);
			m_lstPhoneBook.DeleteColumn(3);
			m_lstPhoneBook.DeleteColumn(2);
		
			break;
		}
		case ALLLOC:
		{
			//USIM
			int i = 0;
			int nindex = 0;
			for(i = 0; i < g_USimTotalNum; i++)
			{
				if (-1 != pUSIMPbRecord[i].index)
				{
					m_lstPhoneBook.InsertItem(nindex, pUSIMPbRecord[i].strName, 
						                      (DWORD)(&pUSIMPbRecord[i]),USIMLOC);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, 
											  pUSIMPbRecord[i].strMobile);
					nindex ++;
				}
			}

			//PCCARD
			nindex = 0;
			for(i = 0; i < g_DataCardTotalNum; i++)
			{
				if (-1 != pDataCardPbRecord[i].index)
				{
					m_lstPhoneBook.InsertItem(nindex, pDataCardPbRecord[i].strName, 
						                      (DWORD)(&pDataCardPbRecord[i]),PCCardLOC);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, 
											  pDataCardPbRecord[i].strMobile);
					nindex ++;
				}
			}	

			//PC
			nindex = 0;
			for(i = 0; i < PB_RECORD_MAX; i++)
			{
				if(-1 != m_pPbData->PbDatas[i].index)
				{
					m_lstPhoneBook.InsertItem (nindex, m_pPbData->PbDatas[i].strName, (DWORD)(&m_pPbData->PbDatas[i]),PCLOC);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, m_pPbData->PbDatas[i].strMobile);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, m_pPbData->PbDatas[i].strCompany);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, m_pPbData->PbDatas[i].strHome);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_GROUP, m_pPbData->PbDatas[i].strGroup);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, m_pPbData->PbDatas[i].strEmail);
					m_lstPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, m_pPbData->PbDatas[i].strMark);
					
					nindex ++;
				}				
			}
			break;
		}
		default:
			break;
	}    
    return 0;
}

void CPhoneBookDlg::OnColumnclickListPhonebook(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    int iColumn = pNMListView->iSubItem;

	//设置排序方式
	if( pNMListView->iSubItem == m_lstPhoneBook.m_headerCtrl.m_nSortCol )
	{
		m_lstPhoneBook.m_headerCtrl.m_bSortAsc = !m_lstPhoneBook.m_headerCtrl.m_bSortAsc;
	}
	else
	{
		m_lstPhoneBook.m_headerCtrl.m_bSortAsc = TRUE;
		m_lstPhoneBook.m_headerCtrl.m_nSortCol = pNMListView->iSubItem;
	}

	m_lstPhoneBook.SortItems(ListCompare,(DWORD)&m_lstPhoneBook );
	m_lstPhoneBook.m_headerCtrl.SetSortImage(iColumn,m_lstPhoneBook.m_headerCtrl.m_bSortAsc);
    *pResult = 0;

	/*
	//PC端数组重写
	if((nCurSelIndex == PCLOC)|| (nCurSelIndex == ALLLOC))
	{
		LVITEM item;
		item.mask = LVIF_IMAGE;	
		item.iSubItem = 0;
		int nArrIndex = 0;
		int nCount = m_lstPhoneBook.GetItemCount();
		for (int nIndex = 0; nIndex < nCount; nIndex ++)
		{
			item.iItem = nIndex;
			m_lstPhoneBook.GetItem(&item);
			if(item.iImage == PCLOC)
			{
				StPbRecord * pTemp = (StPbRecord*)m_lstPhoneBook.GetItemData(nIndex);
				memcpy(&m_pPbData->PbDatas[nArrIndex ++],
					    (StPbRecord*)m_lstPhoneBook.GetItemData(nIndex),
						   sizeof(StPbRecord));
			}			
		}	
		m_pPbData->UpdateFile();
	}
	*/
	return;
    
}

int CALLBACK CPhoneBookDlg::ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nRetVal = -1;
	CMyListCtrl* pNMListCtrl =  (CMyListCtrl*)lParamSort;

	StPbRecord* pData1 = (StPbRecord*)lParam1;
	StPbRecord* pData2 = (StPbRecord*)lParam2;

	switch(pNMListCtrl->m_headerCtrl.m_nSortCol)
	{
	case 0:	// Name

		if((wcslen(pData1->strName) == 0)&&(wcslen(pData2->strName) == 0))
		{
			nRetVal = 0;
		}
		else
		{
			if(pNMListCtrl->m_headerCtrl.m_bSortAsc)
			{
				nRetVal = wcscmp(pData1->strName,
									 pData2->strName);
			}
			else
			{
				nRetVal = wcscmp(pData2->strName,
									  pData1->strName);
			}
		}
		
		break;

	case 1:	// Mobile
		if((wcslen(pData1->strMobile) == 0)&&(wcslen(pData2->strMobile) == 0))
		{
			nRetVal = 0;
		}
		else
		{
			if(pNMListCtrl->m_headerCtrl.m_bSortAsc)
			{
				nRetVal = wcscmp(pData1->strMobile,
									 pData2->strMobile);
			}
			else
			{
				nRetVal = wcscmp(pData2->strMobile,
			                		pData1->strMobile);
			}
		}
		break;
	case 2: // Company
		if((wcslen(pData1->strCompany) == 0)&&(wcslen(pData2->strCompany) == 0))
		{
			nRetVal = 0;
		}
		else
		{
			if(pNMListCtrl->m_headerCtrl.m_bSortAsc)
			{
				nRetVal = wcscmp(pData1->strCompany,
					pData2->strCompany);
			}
			else
			{
				nRetVal = wcscmp(pData2->strCompany,
					pData1->strCompany);
			}
		}
		break;

	case 3: // Home
		if((wcslen(pData1->strHome) == 0)&&(wcslen(pData2->strHome) == 0))
		{
			nRetVal = 0;
		}
		else
		{
			if(pNMListCtrl->m_headerCtrl.m_bSortAsc)
			{
				nRetVal = wcscmp(pData1->strHome,
									 pData2->strHome);
			}
			else
			{
				nRetVal = wcscmp(pData2->strHome,
			                		pData1->strHome);
			}
		}
		break;
	case 4://Group
		if((wcslen(pData1->strGroup)==0)&&(wcslen(pData2->strGroup)==0))
		{
			nRetVal=0;
		}
		else
		{
			if(pNMListCtrl->m_headerCtrl.m_bSortAsc)
			{
				nRetVal = wcscmp(pData1->strGroup,
					pData2->strGroup);
			}
			else
			{
				nRetVal = wcscmp(pData2->strGroup,
					pData1->strGroup);
			}
		}

		break;

	case 5: // Email
		if((wcslen(pData1->strEmail) == 0)&&(wcslen(pData2->strEmail) == 0))
		{
			nRetVal = 0;
		}
		else
		{
			if(pNMListCtrl->m_headerCtrl.m_bSortAsc)
			{
				nRetVal = wcscmp(pData1->strEmail,
									 pData2->strEmail);
			}
			else
			{
				nRetVal = wcscmp(pData2->strEmail,
			                		pData1->strEmail);
			}
		}
		break;

	case 6: // mark
		if((wcslen(pData1->strMark) == 0)&&(wcslen(pData2->strMark) == 0))
		{
			nRetVal = 0;
		}
		else
		{
			if(pNMListCtrl->m_headerCtrl.m_bSortAsc)
			{
				nRetVal = wcscmp(pData1->strMark,
									 pData2->strMark);
			}
			else
			{
				nRetVal = wcscmp(pData2->strMark,
			                		pData1->strMark);
			}
		}
		break;

	default:
		break;
	}

	return nRetVal; 
}

BOOL CPhoneBookDlg::isChsChar(const char* ch)
{
    if( (*ch & 0x80 )&&(*(ch+1)&0x80))
    {
        // ch 为汉字或全角字符
        return TRUE;
    }
    return FALSE;
}

BOOL CPhoneBookDlg::isChsCharUnicode(const TCHAR* ch)
{
    if( ((*ch>>8) & 0x80 )&&(*ch&0x80))
    {
        // ch 为汉字或全角字符
        return TRUE;
    }
    return FALSE;
}

bool CPhoneBookDlg::CheckPbNameUnicode(const TCHAR* ch)
{
	bool bNameUcs = false;  //用于区分姓名中是否有中文，默认为英文

	int nLen = 0;
	while(*ch)
	{
		BOOL bRes = isChsCharUnicode(ch);
		if(bRes)
		{
			ch++;
			bNameUcs = true;
		}
		else
		{
			ch++;
		}
		nLen++;
	}
	if(bNameUcs)
	{
		if (PB_NAME_PC_MAX < nLen)
		{
			AfxMessageBox(IDS_PB_NAMEMAX);

			return false;
		}
	}
	return true;
}
bool CPhoneBookDlg::CheckPbName(const char* ch)
{
	bool bNameUcs = false;  //用于区分姓名中是否有中文，默认为英文

	int nLen = 0;
	while(*ch)
	{
		BOOL bRes = isChsChar(ch);
		if(bRes)
		{
			ch = ch + 2;
			bNameUcs = true;
		}
		else
		{
			ch++;
		}
		nLen++;
	}
	if(bNameUcs)
	{
		if (PB_NAME_PC_MAX < nLen)
		{
			AfxMessageBox(IDS_PB_NAMEMAX);

			return false;
		}
	}
	return true;
}

bool CPhoneBookDlg::CheckPbNumber(CString strNum)
{
	if((-1 != strNum.Find('+'))&&(1 == strNum.GetLength()))
	{
		AfxMessageBox(IDS_CONTACT_NOPLUSONLY);

		return false;
	}
	return true;
}

void CPhoneBookDlg::OnButtonImport() 
{
	if( m_pPbData->CheckFull())
	{
		AfxMessageBox(IDS_PB_FULL);

		return;
	}
	CDlgPbImport Importdlg(this);

	if (Importdlg.DoModal() == IDCANCEL)
	{
		return;
	}
	if ( 2 == Importdlg.m_nCheck)
	{
		CGetOutlookPBDlg GetOutlookPbdlg;
		if( GetOutlookPbdlg.DoModal() == IDOK)
		{
			ReDrawListCtrl();
		}
		return;		
	}	
    IParser FileHandle(this);
    DWORD nFileLen = 0;
    //TCHAR* psrcBuf = NULL;
	char* psrcBuf = NULL;

	//选择导入文件
	bool nRes = FileHandle.SetPath(EXP_PHB);
	FileHandle.RecoverPath();
    if(!nRes)
    {
        return;
    }
    m_StrPathName = FileHandle.GetPath();

    //获取文件长度
    nFileLen = FileHandle.GetFileLen();
	if (0 == nFileLen)
	{
		CString strAppNameTemp;
		strAppNameTemp.LoadString(IDS_APPNAME);
		AddPrefixName(strAppNameTemp);
		CString strFileErr;
		strFileErr.LoadString(IDS_PB_IMPORTFILEERR);
		this->MessageBox(strFileErr,strAppNameTemp,MB_OK);

		return;
	}
	USES_CONVERSION;
	//读取文件内容
    //psrcBuf = (TCHAR*)malloc(nFileLen + 2);
	psrcBuf = (char *)malloc(nFileLen + 2);
    memset(psrcBuf,0,nFileLen + 2);
    if (NULL == FileHandle.ReadData(psrcBuf)) 
    {
		//FileHandle.RecoverPath();

        return;
    }

	//获取文件类型
    int type = FileHandle.GetFType();
	if (-1 == type)
	{
		//FileHandle.RecoverPath();

		return;
	}

    if (0 == type)
    {
        ImportFromCsv(&psrcBuf,nFileLen,&FileHandle);
    }
    else if (1 == type) 
    {
        ImportFromVcf(&psrcBuf,nFileLen,&FileHandle);
    }
	UpdateContactNumForTreeCtrl(m_ht_PC,PCLOC);
	ImportGroup();
    free(psrcBuf);
	//FileHandle.RecoverPath();
}


void CPhoneBookDlg::ReadDataFromLoc(int flag)
{
    Sleep(100);
    m_nIndex = 2;

	BOOL bResponse = CheckStatForTrans();
	if (!bResponse)
	{
		m_ReadDataCardAndUSIMStatus = false;
		return ;
	}
	
	MallocDlgMem(); 
    m_nStoreFlag = flag;
    //CString StrSource = _T("AT+CPBS=\"SM");
	CString StrSource = _T("AT");
    switch (flag)
    {
        case 0:
        {
            StrSource = _T("AT+CPBS=\"ME\"");
            m_pWaitDlg->m_strPrompt.LoadString(IDS_PBTRANSFER_INITCARDINFO);

            break;
        }
        case 1:
        {
            StrSource = _T("AT+CPBS=\"SM\"");
			CString StrPBTransferInfo;
			StrPBTransferInfo.LoadString(IDS_PBTRANSFER_INITUSIMINFO);
			if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
				(wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0))
			{
				m_pWaitDlg->m_strPrompt=StrPBTransferInfo+_T("USIM...");
			}
			else if((wcscmp(g_SetData.Setup_sz3GType,_T("CDMA2000"))==0)||
				(wcscmp(g_SetData.Setup_sz3GType,_T("cdma2000"))==0))
			{
				m_pWaitDlg->m_strPrompt=StrPBTransferInfo+_T("R-UIM...");
			}
            //m_pWaitDlg->m_strPrompt.LoadString(IDS_PBTRANSFER_INITUSIMINFO);

            break;
        }        
        default:
        {
            return ;
        }
    }
    m_ReadDataFromDCState = SEND_AT_SEL_STORE;
    TCHAR szAtBuf[512];
	char szAtAnsBuf[512];

    memset(szAtBuf,0,sizeof(TCHAR)*512);
    memset(szAtAnsBuf,0,sizeof(char)*512);
    wcscpy(szAtBuf,StrSource);
    szAtBuf[wcslen(szAtBuf)] = 0x000D;  
    szAtBuf[wcslen(szAtBuf)] = 0; 
    WCharToChar(szAtBuf,szAtAnsBuf);

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);

    if(pComm->WriteToPort(szAtAnsBuf, wcslen(szAtBuf)))
    {        
        SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE, 10000, NULL);        
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, this);  
		m_ReadDataCardAndUSIMStatus=true;
		m_pWaitDlg->DoModal(); 
		Sleep(10);
    }
    else 
    {
		if(m_pWaitDlg!=NULL)
		{
			m_pWaitDlg->m_bSuccess = false;
			m_pWaitDlg->SendMessage(WM_CLOSE, 0, 0);
		}
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

void CPhoneBookDlg::MallocDlgMem(int nUpper,int nStep)
{

    if (NULL != m_pWaitDlg)
    {
        delete m_pWaitDlg;
    }
    m_pWaitDlg = new CWaitDlg(this,nUpper,nStep);

	return;
}

/*读取Datacard和Usim存储器中电话薄记录时注册的回调函数，用于处理底层返回的结果和向底层发送后续的AT命令*/
void CPhoneBookDlg::AtRespReadState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    int Len = 512;
    char szAtBuf[512];
    char buf[10240] = {0};
    CPhoneBookDlg* pDlg = (CPhoneBookDlg*)pWnd;
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
                pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
                //AfxMessageBox("选择存储器失败");  //debug

                return;
            }
            for(int Nline = 0; Nline < wStrNum;Nline++)
            {
                CString strResult = strArr[Nline];
    
                if (0 == strResult.CompareNoCase(_T("ERROR")))
                {
                    pDlg->m_ReadDataCardAndUSIMStatus = false;
                    pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
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
                pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_CHECK_TOTAL, 10000, NULL);
                RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd);
				pDlg->m_ReadDataFromDCState = SEND_STORE_CHECK_TOTAL;
            }
			else
			{
				pDlg->m_ReadDataCardAndUSIMStatus = false;
				pDlg->m_pWaitDlg->m_bSuccess = false;
				pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
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
                pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
                //AfxMessageBox("获取STORE中的最大条数失败");  //debug
                
                return;
            }    
            for(int Nline = 0; Nline < wStrNum; Nline ++)
            {
                CString strResult = strArr[Nline];
    
                if (0 == strResult.CompareNoCase(_T("ERROR")))
                {
                    pDlg->m_ReadDataCardAndUSIMStatus = false;
                    pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
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

			/*
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
			*/

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

					// !!!!!!
					if (NULL == pDlg->m_pWaitDlg->m_hWnd) {
						pDlg->m_pWaitDlg->m_nUpper = g_DataCardTotalNum; 
						pDlg->m_pWaitDlg->m_nSetp = 1; 
					} else {
						pDlg->m_pWaitDlg->SetProgressRange(0, g_DataCardTotalNum, 1); 
					}
               
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

					// !!!!!!
					if (NULL == pDlg->m_pWaitDlg->m_hWnd) {
						pDlg->m_pWaitDlg->m_nUpper = g_USimTotalNum; 
					} else {
						pDlg->m_pWaitDlg->SetProgressRange(0, g_USimTotalNum, 1);
						pDlg->m_pWaitDlg->m_ProgressWait.SetPos(0);
					}
				}
			}

            else
            {               
                g_DataCardTotalNum = 0;                
                g_USimTotalNum = 0;

				pDlg->m_ReadDataCardAndUSIMStatus = false;
				pDlg->m_pWaitDlg->m_bSuccess = false;
				pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);

				break;
            }			
            memset(szAtBuf,0,sizeof(char)*Len);
			#ifdef FEATURE_HAIER_PHONEBOOK
			strcpy(szAtBuf,"AT^CPBR=1");
			#else
            strcpy(szAtBuf,"AT+CPBR=1");
			#endif
            szAtBuf[strlen(szAtBuf)] = 0x0D;
            szAtBuf[strlen(szAtBuf)] = 0;
            pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
            ASSERT(pComm);
            if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
            {        
                pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL, 10000, NULL);
                RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd);  
				pDlg->m_ReadDataFromDCState = SEND_STORE_READ_ALL;
            }
			else
			{
				pDlg->m_ReadDataCardAndUSIMStatus = false;
				pDlg->m_pWaitDlg->m_bSuccess = false;
				pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
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
                pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
                //AfxMessageBox("获取存储器中电话本信息失败");  //Debug

                return;
            }
            
            for(int Nline = 0; Nline < wStrNum;Nline++)
            {
                CString strResult = strArr[Nline];
    
                if (0 == strResult.CompareNoCase(_T("ERROR")))
                {
                    pDlg->m_ReadDataCardAndUSIMStatus = false;
                    pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
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
				USES_CONVERSION;
				//返回格式为：+CPBR: 1,"123",129,"test"
                if (0 == strResult.CompareNoCase(_T("OK")))
                {
                    CString strTemp;
					strTemp.Format(_T("%s"),A2W((char *)strArr[0]));
                    
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
					memcpy(pStDCPbRecord[nIndex - 1].strMobile, strNum, PB_NUM_MAX*2);

					//获取姓名
					#ifdef FEATURE_HAIER_PHONEBOOK
					CString numType = strTemp.Right(1);
					numType.TrimLeft();
					numType.TrimRight();
					int nNumType = _wtoi(numType);

					int nNameFrom = strTemp.Find('\"', nNumTo + 1);
					int nNameTo = strTemp.ReverseFind('\"');
					strTemp.Delete(nNameTo, strTemp.GetLength()-nNameTo - 1);
					CString strName = strTemp.Mid(nNameFrom + 1, (nNameTo - nNameFrom - 1));
					#else
					strTemp.Delete(strTemp.GetLength() - 1, 1);
					int nNameFrom = strTemp.ReverseFind('\"');
					CString strName = strTemp.Mid(nNameFrom + 1, (strTemp.GetLength() - 1));
					#endif
					strName.TrimLeft();
                    strName.TrimRight();
					if (_T("") != strName)
					{
						//#ifdef FEATURE_HAIER_PHONEBOOK
						//if(nNumType == 1)		//unicode encoding
						//#endif
						strName = UCS2ToGB(strName);
					}
					else
					{
						strName.Format(IDS_UNKNOWN_NAME);
					}
					memcpy(pStDCPbRecord[nIndex - 1].strName, strName, PB_NAME_MAX*2);
                }
            }
                
            char  StrIndex[4];

            if (pDlg->m_nIndex <= nStoreTotalNum)
            {
				memset(StrIndex,0,sizeof(char) * 4);
                _itoa( pDlg->m_nIndex, StrIndex, 10 );

				(pDlg->m_nIndex)++;
				pDlg->m_pWaitDlg->m_ProgressWait.StepIt();

				memset(szAtBuf,0,sizeof(char)*Len);
				#ifdef FEATURE_HAIER_PHONEBOOK
                strcpy(szAtBuf,"AT^CPBR=");
				#else
                strcpy(szAtBuf,"AT+CPBR=");
				#endif
                strcat(szAtBuf, StrIndex);
                szAtBuf[strlen(szAtBuf)] = 0x0D;
                szAtBuf[strlen(szAtBuf)] = 0;
                pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
                ASSERT(pComm);
                if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
                {  					
                    pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL, 10000, NULL);
                    RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd); 
					pDlg->m_ReadDataFromDCState = SEND_STORE_READ_ALL;
                }
				else
				{
					pDlg->m_ReadDataCardAndUSIMStatus = false;
					pDlg->m_pWaitDlg->m_bSuccess = false;
					pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
				}				
            }
            else  //指定存储器读取结束
            {    
				pDlg->m_pWaitDlg->ProgressCompleted(pDlg->m_pWaitDlg->m_nUpper);
				pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
				pDlg->m_ReadDataFromDCState = 0;
#if 0
				if(!gbSimInit)
				{
					pDlg->m_nIndex = 2;
					pDlg->m_nStoreFlag = 1;
					const char* StrSource = "";
					StrSource = "AT+CPBS=\"SM\"";
				    pDlg->m_pWaitDlg->m_strPrompt.LoadString(IDS_PBTRANSFER_INITUSIMINFO);
					((CStatic *)(pDlg->m_pWaitDlg->GetDlgItem(IDC_STATIC_PROMPT)))->SetWindowText(pDlg->m_pWaitDlg->m_strPrompt);
					char szAtBuf[512];

					memset(szAtBuf,0,sizeof(char)*512);
					strcpy(szAtBuf,StrSource);
					szAtBuf[strlen(szAtBuf)] = 0x0D;  
					szAtBuf[strlen(szAtBuf)] = 0; 

					pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
					ASSERT(pComm);
					if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
					{        
						pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE, 10000, NULL);        
						RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd);    
						pDlg->m_ReadDataFromDCState = SEND_AT_SEL_STORE;						
					}
					else 
					{
						pDlg->m_ReadDataCardAndUSIMStatus = false;
						pDlg->m_pWaitDlg->m_bSuccess = false;
						pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
					}
					gbSimInit = true;
				}
				else
				{					
					pDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
					pDlg->m_ReadDataFromDCState = 0;
				}
#endif
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

void CPhoneBookDlg::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default
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
		case TIMER_ID_ATRESP_TIMEOUT_CPBW_DELETE_RECORD:
		{
			KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBW_DELETE_RECORD);
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
    if (NULL != m_pWaitDlg)
    {
        m_pWaitDlg->m_bSuccess = false;
        m_pWaitDlg->PostMessage(WM_CLOSE,0,0);
    }

	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);
	AddPrefixName(strAppName);
	CString strTransTimeout;
	strTransTimeout.LoadString(IDS_USB_PORT_TIMEOUT);
	::MessageBox(m_Hwnd,strTransTimeout,strAppName,MB_OK);

	::SetEvent(g_BGPassEvt);

    CBaseDialog::OnTimer(nIDEvent);
}

bool CPhoneBookDlg::AtWriteARecord(CString Name,CString Num, int index,int nflag)
{
//    AT+CPBW=1,"123",129,"ljl"
//    AT+CPBS="SM"
    Sleep(100);
	
	BOOL bResponse = CheckStatForTrans();
	if (!bResponse)
	{
		return false;
	}



    char cIndexTemp[4] = {0};
    itoa(index, cIndexTemp, 10);
    CString strIndexTemp = (char*)cIndexTemp;
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
        m_StrSource.Insert(m_StrSource.GetLength(),_T(",\""));
        m_StrSource.Insert(m_StrSource.GetLength(),Num);
		#ifdef FEATURE_HAIER_PHONEBOOK
		m_StrSource.Insert(m_StrSource.GetLength(),_T("\",255,"));//modem will auto check the num type
		#else
        m_StrSource.Insert(m_StrSource.GetLength(),_T("\",129,"));
        #endif
    
        if (_T("") != Name)
        {
            m_StrSource.Insert(m_StrSource.GetLength(),_T("\""));

          char *szGPTemp = new char[_tcslen(Name) *2+ 1];
        	memset(szGPTemp, 0, _tcslen(Name)*2 + 1);
		WCharToUnicode(Name, szGPTemp);
		CString szTemp=(char*)szGPTemp;
			
            m_StrSource.Insert(m_StrSource.GetLength(),szTemp);
            m_StrSource.Insert(m_StrSource.GetLength(),_T("\""));

			#ifdef FEATURE_HAIER_PHONEBOOK
			m_StrSource.Insert(m_StrSource.GetLength(),_T(",1"));	//raw character encoding
			#endif
        }
        else
        {
            m_StrSource.Insert(m_StrSource.GetLength(),_T("\"\""));
        }
    }
	if(0 == nflag)
	{
		MallocDlgMem(m_nPCCardCount,1); 
	}
	else
	{
		MallocDlgMem(m_nUSIMCount,1); 
	}
    
    TCHAR szAtBuf[512]={0};
    //memset(szAtBuf,0,sizeof(char)*512);
    CString strAtSetStore;
    if (0 == nflag)
    {
		m_pWaitDlg->m_strPrompt.Format(IDS_PB_DELPCCARD_WAITINFO);
        strAtSetStore = _T("AT+CPBS=\"ME\"");
    }
    else if (1 == nflag)
    {
		m_pWaitDlg->m_strPrompt.Format(IDS_PB_DELUSIM_WAITINFO);
        strAtSetStore = _T("AT+CPBS=\"SM\"");
    }    

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
    //if(pComm->WriteToPort(szAtAnsBuf, wcslen(szAtBuf)))
	if(pComm->WriteToPort(szAtAnsBuf, strlen(szAtAnsBuf)))
    {        
        SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_SET_STORE,30000, NULL);        
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespWriteRecordState, this);
		m_pWaitDlg->DoModal(); 
		Sleep(100);
		if (!m_ReadDataCardAndUSIMStatus)
		{
			return false;
		}        
    }
    else 
    {
        //AfxMessageBox("Write to port fail! ");  //Debug info
		m_pWaitDlg->m_bSuccess = false;
        m_pWaitDlg->SendMessage(WM_CLOSE, 0, 0);

		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strTransfail;
		strTransfail.LoadString(IDS_PBTRANSFER_FAILED);
		::MessageBox(m_Hwnd,strTransfail,strAppName,MB_OK);
    
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


bool CPhoneBookDlg::AtWriteARecord2(CString Name,CString Num, int index,int nflag)
{
//    AT+CPBW=1,"123",129,
//    AT+CPBS="SM"
    Sleep(100);

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
        m_StrSource.Insert(m_StrSource.GetLength(),_T(",\""));
        m_StrSource.Insert(m_StrSource.GetLength(),Num);
		#ifdef FEATURE_HAIER_PHONEBOOK
		m_StrSource.Insert(m_StrSource.GetLength(),_T("\",255,"));//modem will auto check the num type
		#else
        m_StrSource.Insert(m_StrSource.GetLength(),_T("\",129,"));
        #endif
        if (_T("") != Name)
        {        
			//Name.Insert(strlen(Name),"\"");
			m_StrSource.Insert(m_StrSource.GetLength(),_T("\""));
			#ifdef FEATURE_HAIER_PHONEBOOK
			int iByte = 0;
			bool bCheckRes = CheckNameUnicode(Name,&iByte);
			if(!bCheckRes){
			//add name charset check
			#endif
			CString CodeUnicode=BTToUCS2(Name);
			UCS2ToUCS2(Name,CodeUnicode);
			m_StrSource.Insert(m_StrSource.GetLength(),CodeUnicode);
			#ifdef FEATURE_HAIER_PHONEBOOK
			}else
			m_StrSource.Insert(m_StrSource.GetLength(),Name);
			#endif
			m_StrSource.Insert(m_StrSource.GetLength(),_T("\""));
			#ifdef FEATURE_HAIER_PHONEBOOK
			m_StrSource.Insert(m_StrSource.GetLength(),(bCheckRes?_T(",2"):_T(",1")));	//raw character encoding
			#endif
        }
        else
        {
           m_StrSource.Insert(m_StrSource.GetLength(),_T("\"\""));
        }
    }
    
    TCHAR szAtBuf[512];
    memset(szAtBuf,0,sizeof(TCHAR)*512);
    CString strAtSetStore;
    if (0 == nflag)
    {
        strAtSetStore = _T("AT+CPBS=\"ME\"");
    }
    else if (1 == nflag)
    {
        strAtSetStore = _T("AT+CPBS=\"SM\"");
    }    
    wcscpy(szAtBuf,strAtSetStore);
    char szAtAnsBuf[512];
    memset(szAtAnsBuf,0,sizeof(char)*512);
    szAtBuf[wcslen(szAtBuf)] = 0x000D;  
    szAtBuf[wcslen(szAtBuf)] = 0; 
     WCharToChar(szAtBuf,szAtAnsBuf);
	m_ReadDataFromDCState = SEND_AT_SET_STORE;
	//m_ReadDataFromDCState=SEND_WRITE_RECORD;
    Sleep(100);
	m_ReadDataCardAndUSIMStatus = true; //用于判断回调函数是否正确执行
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtAnsBuf, wcslen(szAtBuf)))
    {        
        SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_SET_STORE,30000, NULL);        
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespWriteRecordState2, this);
		//m_pWaitDlg->DoModal();

// 		::ResetEvent(m_Event);
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
    else 
    {
        //AfxMessageBox("Write to port fail! ");  //Debug info

		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strTransfail;
		strTransfail.LoadString(IDS_PBTRANSFER_FAILED);
		::MessageBox(m_Hwnd,strTransfail,strAppName,MB_OK);
    
        return false;
    }
}



//AT命令"AT+CPBW"的回调函数
void CPhoneBookDlg::AtRespWriteRecordState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CPhoneBookDlg* pPBDlg = (CPhoneBookDlg*)pWnd;
    TCHAR szAtBuf[512];
    memset(szAtBuf,0,sizeof(TCHAR) * 512);    

	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);
	AddPrefixName(strAppName);

    switch(pPBDlg->m_ReadDataFromDCState)
    {
        case SEND_AT_SET_STORE:
        {    
            pPBDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_SET_STORE);

            if (0 == wStrNum) 
            {
				pPBDlg->m_ReadDataCardAndUSIMStatus = false;
				pPBDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
				//AfxMessageBox("设置存储器失败"); //debug				
				//::SetEvent(pDlg->m_Event);

                return;
            }

            CString strResult = (char*)strArr[wStrNum - 1];
    
            if (0 != strResult.CompareNoCase(_T("OK")))
            {
				pPBDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
				pPBDlg->m_ReadDataCardAndUSIMStatus = false;
				//AfxMessageBox("设置存储器失败"); //debug				
				//::SetEvent(pDlg->m_Event);

                return;
            }
            char szAtAnsBuf[512];
            memset(szAtAnsBuf,0,sizeof(char)*512);
            wcscpy(szAtBuf,pPBDlg->m_StrSource);
            szAtBuf[wcslen(szAtBuf)] = 0x000D;  
            szAtBuf[wcslen(szAtBuf)] = 0; 
            WCharToChar(szAtBuf,szAtAnsBuf);
            Sleep(100);
            CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
            ASSERT(pComm);
            if(pComm->WriteToPort(szAtAnsBuf, wcslen(szAtBuf)))
            {        
                pPBDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBW_DELETE_RECORD, 30000, NULL);        
                RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespWriteRecordState, pPBDlg);    
                pPBDlg->m_ReadDataFromDCState = SEND_DELETE_RECORD;
				//pPBDlg->m_ReadDataFromDCState = SEND_WRITE_RECORD;
            } 
			else 
			{
				pPBDlg->m_ReadDataCardAndUSIMStatus = false;
				pPBDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
				
				CString strTransfail;
				strTransfail.LoadString(IDS_PBTRANSFER_FAILED);
				pPBDlg->MessageBox(strTransfail,strAppName,MB_OK);
				//AfxMessageBox("Write to port fail! ");  //Debug info				
				//::SetEvent(pDlg->m_Event);

				return;
			}
			pPBDlg->m_ReadDataCardAndUSIMStatus = true;
			
            break;        
        }
        case SEND_WRITE_RECORD:
        {            
            CString strTransFail;
			pPBDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBW_WRITE_RECORD);

            if (0 == wStrNum) 
            {        
				pPBDlg->m_ReadDataCardAndUSIMStatus = false;
				pPBDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
               		
				strTransFail.LoadString(IDS_PBTRANSFER_FAILED);
				pPBDlg->MessageBox(strTransFail,strAppName,MB_OK);
				//AfxMessageBox("写存储器失败"); //debug
				//AfxMessageBox(strTransFail);				
				//::SetEvent(pDlg->m_Event);
                return;
            }
            
            CString strResult = (char*)strArr[wStrNum - 1];
            if (0 != strResult.CompareNoCase(_T("OK")))
            {   
				pPBDlg->m_ReadDataCardAndUSIMStatus = false;
				pPBDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);

				strTransFail.LoadString(IDS_PBTRANSFER_FAILED);  ////错误
				pPBDlg->MessageBox(strTransFail,strAppName,MB_OK);
                //AfxMessageBox("写存储器失败"); //debug				
				//AfxMessageBox(strTransFail);				
				//::SetEvent(pDlg->m_Event);
				return;                                
            }
         	Sleep(10);
            pPBDlg->m_ReadDataFromDCState = 0;
			pPBDlg->m_ReadDataCardAndUSIMStatus = true;
			//::SetEvent(pDlg->m_Event);
            
            break;
        }		
		case SEND_DELETE_RECORD:
		{
			CString strTransFail;
			pPBDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBW_DELETE_RECORD);

            if (0 == wStrNum) 
            {               
				pPBDlg->m_ReadDataCardAndUSIMStatus = false;
				pPBDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);

				strTransFail.LoadString(IDS_PBTRANSFER_FAILED);
				pPBDlg->MessageBox(strTransFail,strAppName,MB_OK);
				//AfxMessageBox("写存储器失败"); //debug
				//::SetEvent(pDlg->m_Event);
                return;
            }            
            CString strResult = (char*)strArr[wStrNum - 1];
            if (0 != strResult.CompareNoCase(_T("OK")))
            {     
				pPBDlg->m_ReadDataCardAndUSIMStatus = false;
				pPBDlg->m_pWaitDlg->PostMessage(WM_CLOSE, 0, 0);
                
				strTransFail.LoadString(IDS_PBTRANSFER_FAILED);
				pPBDlg->MessageBox(strTransFail,strAppName,MB_OK);
				
				//AfxMessageBox("写存储器失败"); //debug
				//::SetEvent(pDlg->m_Event);
				return;                                
            }
 			pPBDlg->m_pWaitDlg->m_ProgressWait.StepIt();
 			pPBDlg->PostMessage(WM_DELETE_DCRECORD);
			
			Sleep(10);
            pPBDlg->m_ReadDataFromDCState = SEND_DELETE_RECORD;
			pPBDlg->m_ReadDataCardAndUSIMStatus = true;
				
			//pPBDlg->m_pWaitDlg->PostMessage(WM_CLOSE,0,0);
			
			::SetEvent(pPBDlg->m_Event);
			
			break;
		}
        default:
        {
			pPBDlg->m_ReadDataCardAndUSIMStatus = false;
			//AfxMessageBox("m_ReadDataFromDCState status error!"); //debug info			
			//::SetEvent(pDlg->m_Event);

            break;
        }
    }
}

void CPhoneBookDlg::AtRespWriteRecordState2(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CPhoneBookDlg* pDlg = (CPhoneBookDlg*)pWnd;
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
				RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespWriteRecordState2, pDlg);    
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


LRESULT CPhoneBookDlg::OnDelDCRecord(WPARAM wParam, LPARAM lParam)
{
	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);
	AddPrefixName(strAppName);	
	
	if(!TransferBreak())
	{
		m_pWaitDlg->SendMessage(WM_CLOSE, 0, 0);
		nCurSelIndex=PCCardLOC;
		ReDrawListCtrl();
		return 0;
	}

	if(!m_ReadDataCardAndUSIMStatus)
	{
		m_pWaitDlg->SendMessage(WM_CLOSE, 0, 0);	
		return 0;
	}
	
	

	//删除当前记录的内存数组数据
	StPbRecord *PbBookTemp = (StPbRecord *)*m_RecordTempAds;
	PbBookTemp->index = -1;
	memset(PbBookTemp->strName,0,PB_NAME_MAX);
	memset(PbBookTemp->strMobile,0,PB_NUM_MAX);

	m_RecordTempAds ++;
	m_nDCCount--;

	if(m_nDCCount > 0)
	{
		//发AT命令删除下一条记录
		StPbRecord *PbBookTemp = (StPbRecord *)*m_RecordTempAds;
		char cIndexTemp[4] = {0};
		itoa(PbBookTemp->index, cIndexTemp, 10);
		CString strIndexTemp =(char*) cIndexTemp;
		strIndexTemp.TrimLeft();
		strIndexTemp.TrimRight();
		m_StrSource = _T("AT+CPBW=");
		m_StrSource.Insert(m_StrSource.GetLength(),strIndexTemp);

		TCHAR szAtBuf[512];
        memset(szAtBuf,0,sizeof(TCHAR)*512);
        char szAtAnsBuf[512];

        memset(szAtAnsBuf,0,sizeof(char)*512);
        wcscpy(szAtBuf,m_StrSource);
        szAtBuf[wcslen(szAtBuf)] = 0x000D;  
        szAtBuf[wcslen(szAtBuf)] = 0; 
        WCharToChar(szAtBuf,szAtAnsBuf);

		Sleep(100);
		CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
		ASSERT(pComm);
		if(pComm->WriteToPort(szAtAnsBuf, wcslen(szAtBuf)))
		{    
			MSG msg;

			SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBW_DELETE_RECORD, 30000, NULL);        
			RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespWriteRecordState, this);    
			m_ReadDataFromDCState = SEND_DELETE_RECORD;		
			
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{   // Get Next message in queue

				if ((msg.message == WM_PAINT))
				 {         
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				 }
		   }
		} 
		else 
		{
			m_ReadDataCardAndUSIMStatus = false;
			m_pWaitDlg->SendMessage(WM_CLOSE, 0, 0);		

			CString strTransfail;
			strTransfail.LoadString(IDS_PBTRANSFER_FAILED);
			MessageBox(strTransfail,strAppName,MB_OK);

			return 0;
		}
	}
	else
	{
		m_ReadDataCardAndUSIMStatus = true;
		m_pWaitDlg->SendMessage(WM_CLOSE, 0, 0);
	}
	return 1;
	
}

void CPhoneBookDlg::UpdateButtonStatus(bool bEnable)
{
	if (bEnable)
	{
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CALL)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_IMPORT)->EnableWindow(true);
		if(g_bSimReady && g_bIsExist)
		{
			GetDlgItem(IDC_BUTTON_SENDSMS)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_TRANSFER)->EnableWindow(true);
		}
		GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_DELETE_ALL)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(true);
		GetDlgItem(IDC_LIST_PHONEBOOK)->EnableWindow(true);

	}
	else
	{
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_CALL)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_IMPORT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_SENDSMS)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_TRANSFER)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_DELETE_ALL)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(false);
		GetDlgItem(IDC_LIST_PHONEBOOK)->EnableWindow(false);
	}
	return;
}


BOOL CPhoneBookDlg::SetupCall()
{
    // TODO: Add your control notification handler code here

    int PbNum = m_lstPhoneBook.GetNextItem(-1, LVNI_SELECTED);
    if(PbNum < 0)
    {
        //AfxMessageBox(IDS_PB_SELNULL);
        return FALSE;
    }    

    StPbRecord* PbBookTemp = (StPbRecord* ) m_lstPhoneBook.GetItemData(PbNum);

    int nDialNum = 0;
    int nSelDial = 0;

    if(wcslen(PbBookTemp->strMobile) > 0)
    {
        nDialNum++;
        nSelDial = 1;
    }
    if(wcslen(PbBookTemp->strHome) > 0)
    {
        nDialNum++;
        nSelDial = 2;
    }
    if(wcslen(PbBookTemp->strCompany) > 0)
    {
        nDialNum++;
        nSelDial = 3;
    }
    if (0 == nDialNum)
    {
        CString strNullNum;
		strNullNum.LoadString(IDS_SELCONNECT_NULLNUM);
        //AfxMessageBox(strNullNum);

        return FALSE;
    }

    if(nDialNum == 1)
    {
        switch ( nSelDial )
        {
            case 1:
            {
                WPARAM wParam = 1;
                LPARAM lParam = (LPARAM)(LPCTSTR)PbBookTemp->strMobile;
                ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam,lParam);

                break;
            }
            case 2:
            {
                WPARAM wParam = 1;
                LPARAM lParam = (LPARAM)(LPCTSTR)PbBookTemp->strHome;
                ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam,lParam);

                break;
            }
            case 3:
            {
                WPARAM wParam = 1;
                LPARAM lParam = (LPARAM)(LPCTSTR)PbBookTemp->strCompany;
                ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam,lParam);
    
                break;
            }
            default:
            {
                break;
            }

        }
    
    }
    else
    {
        CSelDialDlg SelDlg(this);

		wcscpy(SelDlg.m_strName.GetBuffer(MAX_PATH), PbBookTemp->strName);
        wcscpy(SelDlg.m_strMobile.GetBuffer(MAX_PATH), PbBookTemp->strMobile);
        wcscpy(SelDlg.m_strHome.GetBuffer(MAX_PATH), PbBookTemp->strHome);
        wcscpy(SelDlg.m_strCompany.GetBuffer(MAX_PATH), PbBookTemp->strCompany);

        if (IDOK == SelDlg.DoModal())
        {
            WPARAM wParam = 1;
            LPARAM lParam = (LPARAM)((LPCTSTR)(SelDlg.m_strSelCallNum));
            ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam,lParam);
        }
    }

	return TRUE;
}



LRESULT CPhoneBookDlg::ReDrawTreeCtrl(WPARAM wParam, LPARAM lParam)
{
	m_TreePhoneBook.DeleteAllItems();
	m_TreePhoneBook.SetImageList(&m_ilLsts, LVSIL_NORMAL);
	

 	if(wParam==1000)
	{
		m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
		m_TreePhoneBook.DeleteAllItems();
		nCurSelIndex=PCGROUP;
	}

	if(wParam==1) //Update group name in the tree control
	{
		if( m_TreePhoneBook.ItemHasChildren(m_ht_PC) )
		{
			HTREEITEM HtItem=m_TreePhoneBook.GetChildItem(m_ht_PC);
			while(HtItem!=NULL)
			{
				CString itemText=m_TreePhoneBook.GetItemText(HtItem);
				
				CString StrExtractedText;
				CString StrCountPart=_T("");
				int nLeftBracketPos=itemText.ReverseFind('(');
				if(nLeftBracketPos>0)
				{
					int nLen=itemText.GetLength();
					StrExtractedText=itemText.Mid(0,nLen-(nLen-nLeftBracketPos));
					StrCountPart=itemText.Mid(nLeftBracketPos);
				}
				else
					StrExtractedText=itemText;
				
				itemText=StrExtractedText;
				if(itemText.Compare(m_strGPBeforeUpdated)==0)
				{
					m_TreePhoneBook.SetItemText(HtItem,m_strGPAfterUpdated+StrCountPart);
					return 0;
				}
				HtItem=m_TreePhoneBook.GetNextSiblingItem(HtItem);
			}
			
		}
		nCurSelIndex=PCGROUP;
	//	return 0;
	}
	if(wParam==2) //Delete group name from the tree
	{
		if( m_TreePhoneBook.ItemHasChildren(m_ht_PC) )
		{
			HTREEITEM HtItem=m_TreePhoneBook.GetChildItem(m_ht_PC);
			while(HtItem!=NULL)
			{
				CString itemText=m_TreePhoneBook.GetItemText(HtItem);

				CString StrExtractedText;
				int nLeftBracketPos=itemText.ReverseFind('(');
				if(nLeftBracketPos>0)
				{
					int nLen=itemText.GetLength();
					StrExtractedText=itemText.Mid(0,nLen-(nLen-nLeftBracketPos));
				}
				else
					StrExtractedText=itemText;

				itemText=StrExtractedText;
				if(itemText.Compare(m_strGPDelete)==0)
				{
					m_TreePhoneBook.DeleteItem(HtItem);
					return 0;
				}
				HtItem=m_TreePhoneBook.GetNextSiblingItem(HtItem);
			}
			
		}
		nCurSelIndex=PCGROUP;
	//	return 0;

	}
	
	m_ht_PC=m_TreePhoneBook.InsertItem(_T("PC"),0,0,NULL);
	for(int i = 0; i < PB_GROUP_MAX; i++)
	{
		if(-1 != m_pPbData->PbGroups[i].index)
		{
			m_TreePhoneBook.InsertItem(m_pPbData->PbGroups[i].strGroupName,3,3,m_ht_PC);
		}
	}
	m_TreePhoneBook.SortChildren(m_ht_PC);
//	m_ht_USIM=m_TreePhoneBook.InsertItem(_T("USIM"),2,2,NULL);
	if(g_bIsExist && g_bSimReady)
		m_ht_PCCard=m_TreePhoneBook.InsertItem(m_strPCCard,1,1,NULL);

	

	UpdateContactNumForTreeCtrl(m_ht_PC,PCLOC);
 	UpdateContactNumForTreeCtrl(m_ht_PC,PCGROUP);
	if(g_bIsExist && g_bSimReady)
	{
		UpdateContactNumForTreeCtrl(m_ht_PCCard,PCCardLOC);
		#ifndef FEATURE_VERSION_NOSIM
		if(g_bSimReady)
		  {
			m_ht_USIM=m_TreePhoneBook.InsertItem(m_strUSIM,2,2,NULL);		
			UpdateContactNumForTreeCtrl(m_ht_USIM,USIMLOC);
		}
	
		#endif
	}
	
	return 0;

}

void CPhoneBookDlg::OnSelchangeTree(WPARAM wParam, LPARAM lParam)
{
	
	m_strCurSelGroup=m_TreePhoneBook.GetItemText(m_TreePhoneBook.GetSelectedItem());
	if(m_strCurSelGroup.Compare(_T(""))==0)
		m_strCurSelGroup=_T("PC");
 	CString StrExtractedText;
	int nLeftBracketPos=m_strCurSelGroup.ReverseFind('(');
	if(nLeftBracketPos>0)
	{
		int nLen=m_strCurSelGroup.GetLength();
		StrExtractedText=m_strCurSelGroup.Mid(0,nLen-(nLen-nLeftBracketPos));
	}
	else
		StrExtractedText=m_strCurSelGroup;
	


// 	_itow(nLeftBracketPos,str,10);

	m_strCurSelGroup=StrExtractedText;
	int nIndex;
	if(wcscmp(m_strCurSelGroup,_T("PC"))==0)
	{
		nIndex=PCLOC;
	}
	else if(wcscmp(m_strCurSelGroup,m_strPCCard)==0)
	{
		nIndex=PCCardLOC;
	}
#ifndef FEATURE_VERSION_NOSIM
	else if(wcscmp(m_strCurSelGroup,m_strUSIM)==0)
	{
		nIndex=USIMLOC;
	}
#endif
	else
	{
		nIndex=PCGROUP;
	}
	
	m_ReadDataCardAndUSIMStatus = true;
	switch (nIndex)
    {
		case PCLOC:
		{
			nCurSelIndex = nIndex;
			break;
		}
		case PCGROUP:
		{
			nCurSelIndex = nIndex;
			break;
		}
		case PCCardLOC:
		{
			nCurSelIndex = nIndex;
			if(m_bNewGroupFlagUSB) //增加一个群组后，不要读取USB Modem卡 added by Zhou Bin  2008/12/15
			{
				m_bNewGroupFlagUSB=FALSE;
					return;
			}
// 			if(!gbPcCardInit)
// 			{
// 				//init PcCard Location data
// 				::ResetEvent(g_BGPassEvt);
// 				ReadDataFromLoc(0);
// 				if (!m_ReadDataCardAndUSIMStatus)
// 				{
// 					OnCancel();
// 				}	
// 				else
// 				{
// 					gbPcCardInit = true;
// 					UpdateContactNumForTreeCtrl(m_ht_PCCard,PCCardLOC);
// 				}
// 			}
			ReadDataFromSimOrUSB(0);
			break;
		}
		case USIMLOC: 
		{
			nCurSelIndex = nIndex;

			if(m_bNewGroupFlagUSIM) //增加一个群组后，不要读取sim卡 added by Zhou Bin  2008/12/15
			{
				m_bNewGroupFlagUSIM=FALSE;
				
			   return;
			}
// 			if(!gbSimInit)
// 			{
// 				//init PcCard Location data
// 				::ResetEvent(g_BGPassEvt);
// 				ReadDataFromLoc(1);
// 				if (!m_ReadDataCardAndUSIMStatus)
// 				{
// 					OnCancel();
// 				}
// 				else
// 				{
// 					gbSimInit = true;
// 					UpdateContactNumForTreeCtrl(m_ht_USIM,USIMLOC);
// 				}
// 			}
			ReadDataFromSimOrUSB(1);
			break;
		}

	default:
		break;
	}
	
	::SetEvent(g_BGPassEvt);
	m_lstPhoneBook.m_headerCtrl.SetSortImage(SELCONTACT_LISTCTRL_HEADERINDEX_MAX,m_lstPhoneBook.m_headerCtrl.m_bSortAsc);

	m_lstPhoneBook.DeleteColumn(6);
	m_lstPhoneBook.DeleteColumn(5);
	m_lstPhoneBook.DeleteColumn(4);
	m_lstPhoneBook.DeleteColumn(3);
	m_lstPhoneBook.DeleteColumn(2);
	m_lstPhoneBook.DeleteColumn(1);
	m_lstPhoneBook.DeleteColumn(0);
	InitListCtrl(2);
	
	ReDrawListCtrl();

}

void CPhoneBookDlg::UpdateContactNumForTreeCtrl(HTREEITEM HtItem,int groupType)
{
	CString StrItemText=m_TreePhoneBook.GetItemText(HtItem);
// 	CString StrExtractedText;
 	TCHAR strItemCount[5];
// 	int nLeftBracketPos=StrItemText.ReverseFind('(');
// 	_itow(nLeftBracketPos,str,10);
	int nLen;
	CString StrExtractedText;
	int nLeftBracketPos=StrItemText.ReverseFind('(');
	if(nLeftBracketPos>0)
	{
		nLen=StrItemText.GetLength();
		StrExtractedText=StrItemText.Mid(0,nLen-(nLen-nLeftBracketPos));
	}
	else
		StrExtractedText=StrItemText;

	StrItemText=StrExtractedText;
	int nItemCount=0;
	switch(groupType)
	{
		case PCLOC:
		{
			nItemCount=0;
			if(StrItemText.Compare(_T("PC"))==0)
			{
				for(int i = 0; i < PB_RECORD_MAX; i++)
				{
					if(-1 != m_pPbData->PbDatas[i].index)
						nItemCount++;
				}

				_itow(nItemCount,strItemCount,10);
				StrItemText+=_T("(");
				StrItemText+=strItemCount;
				StrItemText+=_T(")");
				m_TreePhoneBook.SetItemText(HtItem,StrItemText);
			}
			break;
		}
		case PCGROUP:
		{
			nItemCount=0;
			if( m_TreePhoneBook.ItemHasChildren(m_ht_PC) )
			{
				HTREEITEM HtChildItem=m_TreePhoneBook.GetChildItem(m_ht_PC);
				while(HtChildItem!=NULL)
				{
					CString itemText=m_TreePhoneBook.GetItemText(HtChildItem);
					nLeftBracketPos=itemText.ReverseFind('(');
					if(nLeftBracketPos>0)
					{
						nLen=itemText.GetLength();
						StrExtractedText=itemText.Mid(0,nLen-(nLen-nLeftBracketPos));
					}
					else
						StrExtractedText=itemText;
					
					itemText=StrExtractedText;

					for(int i = 0; i < PB_RECORD_MAX; i++)
					{
						if(-1 != m_pPbData->PbDatas[i].index && wcscmp(m_pPbData->PbDatas[i].strGroup,itemText)==0 )					
							nItemCount++;
						
					}
					_itow(nItemCount,strItemCount,10);
					itemText+=_T("(");
					itemText+=strItemCount;
					itemText+=_T(")");
					nItemCount=0;
					m_TreePhoneBook.SetItemText(HtChildItem,itemText);
					HtChildItem=m_TreePhoneBook.GetNextSiblingItem(HtChildItem);
				}
				
			}
			
			break;
		}
		case PCCardLOC:
		{
			int nItemCount = 0;
			for(int i = 0; i < g_DataCardTotalNum; i++)
			{
				if (-1 != pDataCardPbRecord[i].index)	
					nItemCount ++;

			}
			_itow(nItemCount,strItemCount,10);
			StrItemText+=_T("(");
			StrItemText+=strItemCount;
			StrItemText+=_T(")");
				m_TreePhoneBook.SetItemText(m_ht_PCCard,StrItemText);
			break;
		}
		case USIMLOC:
		{
			int nItemCount = 0;
			for(int i = 0; i < g_USimTotalNum; i++)
			{
				if (-1 != pUSIMPbRecord[i].index)
						nItemCount ++;
	
			}
			_itow(nItemCount,strItemCount,10);
			StrItemText+=_T("(");
			StrItemText+=strItemCount;
			StrItemText+=_T(")");
				m_TreePhoneBook.SetItemText(m_ht_USIM,StrItemText);
			break;
		}

	}

}


LRESULT CPhoneBookDlg::OnUpdateTreeContactNum(WPARAM wParam, LPARAM lParam)
{
	UpdateContactNumForTreeCtrl(m_ht_PC,PCGROUP);
	if(wParam==1000) //Transfer后，更新存储介质中的联系人数量
	{
		UpdateContactNumForTreeCtrl(m_ht_PC,PCLOC);
		UpdateContactNumForTreeCtrl(m_ht_PC,PCGROUP);
		UpdateContactNumForTreeCtrl(m_ht_PCCard,PCCardLOC);
		UpdateContactNumForTreeCtrl(m_ht_USIM,USIMLOC);
	}
	return 0;

}
void CPhoneBookDlg::ProgressOpen(int nUpper,int nSetp)
{
	m_Progress.ShowWindow(SW_SHOWNA);
	m_Progress.SetRange(0,nUpper);
	m_Progress.SetStep(nSetp);
	m_Progress.SetPos(0);
	
	m_Event = ::CreateEvent(NULL,FALSE,FALSE,NULL);
}



void CPhoneBookDlg::ReadDataFromSimOrUSB(int nFlag)
{
	if(nFlag==0)
	{
		if(!gbPcCardInit)
		{
			//init PcCard Location data
			::ResetEvent(g_BGPassEvt);
			ReadDataFromLoc(0);
			if (!m_ReadDataCardAndUSIMStatus)
			{
				OnCancel();
			}	
			else
			{
				gbPcCardInit = true;
				UpdateContactNumForTreeCtrl(m_ht_PCCard,PCCardLOC);
			}
		}
	}
	else
	{
		if(!gbSimInit)
		{
			//init PcCard Location data
			::ResetEvent(g_BGPassEvt);
			ReadDataFromLoc(1);
			if (!m_ReadDataCardAndUSIMStatus)
			{
				OnCancel();
			}
			else
			{
				gbSimInit = true;
				UpdateContactNumForTreeCtrl(m_ht_USIM,USIMLOC);
			}
		}
	}

}
bool CPhoneBookDlg::CheckNameUnicode(const TCHAR* ch,int* iByte)
{
	bool bNameUcs = false;  //用于区分姓名中是否有中文，默认为英文
	*iByte = 0;
	int nLen = 0;
	int nCharLen = wcslen(ch);

	while(*ch)
	{
		BOOL bRes = isChineseCharUnicode(ch);
		if(bRes)
		{
			ch++;
			if(nLen < PB_NAME_UCS_MAX)
			{
				*iByte = (*iByte)++;
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
			return false;
		}
	}
	return true;
}

BOOL CPhoneBookDlg::isChineseChar(const char* ch)
{
    if( (*ch & 0x80 )&&(*(ch+1)&0x80))
    {
        // ch 为汉字或全角字符
        return TRUE;
    }
    return FALSE;
}

BOOL CPhoneBookDlg::isChineseCharUnicode(const TCHAR* ch)
{
    if( ((*ch>>8) & 0x0080 )&&(*ch & 0x80))
    {
        // ch 为汉字或全角字符
        return TRUE;
    }
    return FALSE;
}

int CPhoneBookDlg::GetFirstIdleIndex(StPbRecord *pStDCPbRecord, int nTotalNum)
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



bool CPhoneBookDlg::TransferBreak()
{
	CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
	if ((pMainWnd->m_bInComSms) || (pMainWnd->m_bInComCall))
	{		
	//	this->PostMessage(WM_CLOSE,0,0);
	//	m_bEndPbTransfer = true;
		::SetEvent(g_BGPassEvt);
		
		return false;
	}
	return true;
}

void CPhoneBookDlg::ImportGroup()
{
   int iItemCount,i,j;
   iItemCount=m_lstPhoneBook.GetItemCount();
   for(i=0;i<iItemCount;i++)
   {
	   CString strGroup = m_lstPhoneBook.GetItemText(i,4);
	
	   if(strGroup.Compare(_T(""))!=0)
	   {
		   BOOL bGroupExist=FALSE;
		   for(j=0;j<PB_GROUP_MAX;j++)
		   {
			   if(wcscmp(m_pPbData->PbGroups[j].strGroupName,(LPCTSTR)strGroup)==0)
			   {
				  bGroupExist=TRUE;
				  break;
			   }
		   }
		   if(!bGroupExist)
		   {
			   StPbGroup *PbGroupTemp = new StPbGroup();
				memset(PbGroupTemp,0,sizeof(StPbGroup));
				wcscpy(PbGroupTemp->strGroupName,(LPCTSTR)strGroup);
				DWORD PbArrItemAds = 0;
				m_pPbData->AddGroup(*PbGroupTemp,&PbArrItemAds);
		   }

		   
	   }
   }
	ReDrawTreeCtrl();

}
