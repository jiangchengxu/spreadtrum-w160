// SMSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "SmsDlg.h"
#include "SmsWriteDlg.h"
#include "SelContactDlg.h"
#include "ContactDlg.h"
#include "SelPosition.h"
#include "SmsTransferDlg.h"
#include "SmsExportSel.h"
#include "SmsExtractNumDlg.h"
#include "ReadFalshSMS.h"
#include "Ds.h"
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

/////////////////////////////////////////////////////////////////////////////
// CSmsDlg dialog

CSmsDlg::CSmsDlg(CWnd* pParent /*=NULL*/)
    : CTabedDialog(CSmsDlg::IDD, pParent), m_lstSms(TRUE)
{
    m_pReadDlg = NULL;
    m_pSmsData = ((CHSDPAApp *)AfxGetApp())->GetSmsData();
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    memset(m_DelDspIndexArr, 0x00, sizeof(m_DelDspIndexArr));
    memset(m_DelNumArr, 0, sizeof(m_DelNumArr));
//	UnKnowSms = FALSE;

    //wyw
    m_imagelistSMSTree = NULL;
    //init voicemail count
    VoiceMailCountSMSDlg = _T("-1");
    voicesmscount = _T("0");//add by liub for voicemail
}

void CSmsDlg::DoDataExchange(CDataExchange* pDX)
{
    CTabedDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSmsDlg)
    DDX_Control(pDX, IDC_LIST_SMS, m_lstSms);
    DDX_Control(pDX, IDC_TAB_SMS, m_tabSms);
    DDX_Control(pDX, IDC_COMBO_LOC_FILTER, m_cmbLocFilter);
    DDX_Control(pDX, IDC_TREE_SMS,m_treeSms);
    DDX_Text(pDX,IDC_EDIT_SMS_SEARCH,m_strSearch);

    ////////////////////////////////////////////////////////////////////////// add by liub for SMS buttons 1121 begin
    DDX_Control(pDX,IDC_BUTTON_SMS_NEW,m_btnnew);
    DDX_Control(pDX,IDC_BUTTON_SMS_REPLY,m_btnReply);
    DDX_Control(pDX,IDC_BUTTON_SMS_FORWARD,m_btnForward);
    DDX_Control(pDX,IDC_BUTTON_SMS_DELETE,m_btnDelete);
    DDX_Control(pDX,IDC_BUTTON_SMS_ABSTRACT,m_btnExtract);
    DDX_Control(pDX,IDC_BUTTON_SMS_DIAL,m_btnDial);
    DDX_Control(pDX,IDC_BUTTON_SMS_SEARCH,m_btnSearch);
    DDX_Control(pDX,IDC_BUTTON_SMS_EDIT,m_btnEdit);
    DDX_Control(pDX,IDC_BUTTON_SMS_ARCHIVE,m_btnArchive);
    DDX_Control(pDX,IDC_BUTTON_SMS_RESUME,m_btnResume);
    DDX_Control(pDX,IDC_BUTTON_SMS_TRANSFER,m_btnTransfer);
    ////////////////////////////////////////////////////////////////////////// add by liub end
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSmsDlg, CTabedDialog)
    //{{AFX_MSG_MAP(CSmsDlg)
    ON_EN_CHANGE(IDC_EDIT_SMS_SEARCH, OnChangeEditSmsSearch)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SMS, OnSelchangeTabSms)
    ON_BN_CLICKED(IDC_BUTTON_SMS_READ, OnButtonSmsRead)
    ON_BN_CLICKED(IDC_BUTTON_SMS_REPLY, OnButtonSmsReply)
    ON_BN_CLICKED(IDC_BUTTON_SMS_DIAL, OnButtonSmsDial)
    ON_BN_CLICKED(IDC_BUTTON_SMS_DELETE, OnButtonSmsDelete)
    ON_BN_CLICKED(IDC_BUTTON_SMS_NEW, OnButtonSmsNew)
    ON_BN_CLICKED(IDC_BUTTON_SMS_FORWARD, OnButtonSmsForward)
    ON_BN_CLICKED(IDC_BUTTON_SMS_ABSTRACT, OnButtonSmsAbstract)
    ON_BN_CLICKED(IDC_BUTTON_SMS_DELALL, OnButtonSmsDelall)
    ON_NOTIFY(NM_CLICK,IDC_LIST_SMS,OnclkListSms)//liub_modify 添加单击SMS列表显示其内容功能
    ON_BN_CLICKED(IDC_BUTTON_SMS_EDIT, OnButtonSmsEdit)
    ON_BN_CLICKED(IDC_BUTTON_SMS_TRANSFER, OnButtonSmsTransfer)
    ON_BN_CLICKED(IDC_BUTTON_SMS_EXPORT, OnButtonSmsExport)
    ON_BN_CLICKED(IDC_BUTTON_SMS_SEARCH,OnButtonSmsSearch)
    ON_BN_CLICKED(IDC_BUTTON_SMS_ARCHIVE,OnButtonSmsArchive)
    ON_BN_CLICKED(IDC_BUTTON_SMS_RESUME,OnButtonSmsResume)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SMS, OnColumnclickListSms)
    ON_WM_SHOWWINDOW()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SMS, OnCusDrawListSms)
    ON_CBN_SELCHANGE(IDC_COMBO_LOC_FILTER, OnSelchangeCombolocfilter)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SMS, OnSelchangedSMSTree)//libu_modify  创建更改SMS树形结构的响应函数
    ON_WM_DESTROY()
    ON_EN_VSCROLL(IDC_EDIT_SMSCONTENT,OnVScrollContent)
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_SMS_MENUITEM_CLICK, OnMenuItemClick)
    ON_MESSAGE(WM_SMS_SAVE_MSG, OnSaveSms)
    ON_MESSAGE(WM_SMS_READ_MSG, OnReadSms)
    ON_MESSAGE(WM_SMS_VIEW_MSG, OnViewSms)
    ON_MESSAGE(WM_SMS_OPERATE_PROC, OnSmsOperateProc)
    ON_MESSAGE(WM_CREATE_PROGRESS, OnCreateProgress)
    ON_MESSAGE(WM_SMS_READ_FLASHSMS,OnReadFlashSMS)//add by liub
    ON_MESSAGE(WM_SMS_UPSMSDLGTREE,Updatatree)//add by liub
    ON_MESSAGE(WM_NO_MODEM_FOR_SMS, OnModemStausChange)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsDlg message handlers

//功能：初始化对话框
//输入：无
//输出：TRUE/FALSE
BOOL CSmsDlg::OnInitDialog()
{
    CTabedDialog::OnInitDialog();
    CString strButtonTip;
    //////////////////////////////////////////////////////////////////////////add by liub 添加短信功能按钮
    m_btnnew.SetIcon(IDI_ICON_SMSNEW,IDI_ICON_SMSNEW,IDI_ICON_SMSNEW);
    m_btnnew.SetAlign(CButtonST::ST_ALIGN_HORIZ);
    //m_btnnew.DrawBorder(TRUE);
    strButtonTip.LoadString(IDS_SMS_NEW);
    m_btnnew.SetTooltipText(strButtonTip);

    m_btnReply.SetIcon(IDI_ICON_SMSREPLY,IDI_ICON_SMSREPLY,IDI_ICON_SMSREPLY);
    m_btnReply.SetAlign(CButtonST::ST_ALIGN_HORIZ);
//	m_btnReply.DrawBorder(TRUE);
    strButtonTip.LoadString(IDS_SMS_REPLY);
    m_btnReply.SetTooltipText(strButtonTip);

    m_btnForward.SetIcon(IDI_ICON_SMSFORWARD,IDI_ICON_SMSFORWARD,IDI_ICON_SMSFORWARD);
    m_btnForward.SetAlign(CButtonST::ST_ALIGN_HORIZ);
//	m_btnForward.DrawBorder(TRUE);
    strButtonTip.LoadString(IDS_SMS_FORWARD);
    m_btnForward.SetTooltipText(strButtonTip);

    m_btnDelete.SetIcon(IDI_ICON_SMSDELEDT,IDI_ICON_SMSDELEDT,IDI_ICON_SMSDELEDT);
    m_btnDelete.SetAlign(CButtonST::ST_ALIGN_HORIZ);
    //m_btnDelete.DrawBorder(TRUE);
    strButtonTip.LoadString(IDS_SMS_DELETE);
    m_btnDelete.SetTooltipText(strButtonTip);


    m_btnExtract.SetIcon(IDI_ICON_SMSEXTRACT,IDI_ICON_SMSEXTRACT,IDI_ICON_SMSEXTRACT);
    m_btnExtract.SetAlign(CButtonST::ST_ALIGN_HORIZ);
//	m_btnExtract.DrawBorder(TRUE);
    strButtonTip.LoadString(IDS_SMS_ABSTRACT);
    m_btnExtract.SetTooltipText(strButtonTip);


    m_btnSearch.SetIcon(IDI_ICON_SMSSEARCH,IDI_ICON_SMSSEARCH,IDI_ICON_SMSSEARCH);
    m_btnSearch.SetAlign(CButtonST::ST_ALIGN_HORIZ);
//	m_btnSearch.DrawBorder(TRUE);
    strButtonTip.LoadString(IDS_SMS_SEARCH);
    m_btnSearch.SetTooltipText(strButtonTip);

    m_btnDial.SetIcon(IDI_ICON_SMSDAIL,IDI_ICON_SMSDAIL,IDI_ICON_SMSDAIL);
    m_btnDial.SetAlign(CButtonST::ST_ALIGN_HORIZ);
//	m_btnDial.DrawBorder(TRUE);
    strButtonTip.LoadString(IDS_SMS_DIAL);
    m_btnDial.SetTooltipText(strButtonTip);

    m_btnEdit.SetIcon(IDI_ICON_SMS_EDIT,IDI_ICON_SMS_EDIT,IDI_ICON_SMS_EDIT);
    m_btnEdit.SetAlign(CButtonST::ST_ALIGN_HORIZ);
//	m_btnEdit.DrawBorder(TRUE);
    strButtonTip.LoadString(IDS_SMS_EDIT);
    m_btnEdit.SetTooltipText(strButtonTip);

    m_btnArchive.SetIcon(IDI_ICON_SMS_ARCHIVE,IDI_ICON_SMS_ARCHIVE,IDI_ICON_SMS_ARCHIVE);
    m_btnArchive.SetAlign(CButtonST::ST_ALIGN_HORIZ);
//	m_btnArchive.DrawBorder(TRUE);
    strButtonTip.LoadString(IDS_SMS_ARCHIVE);
    m_btnArchive.SetTooltipText(strButtonTip);

    m_btnResume.SetIcon(IDI_ICON_SMS_RESUME,IDI_ICON_SMS_RESUME,IDI_ICON_SMS_RESUME);
    m_btnResume.SetAlign(CButtonST::ST_ALIGN_HORIZ);
    strButtonTip.LoadString(IDS_SMS_RESUME);
    m_btnResume.SetTooltipText(strButtonTip);
//	m_btnResume.DrawBorder(TRUE);

    m_btnTransfer.SetIcon(IDI_ICON_PB_TRANSFER,IDI_ICON_PB_TRANSFER,IDI_ICON_PB_TRANSFER);
    m_btnTransfer.SetAlign(CButtonST::ST_ALIGN_HORIZ);
    strButtonTip.LoadString(IDS_SMS_TRANSFER);
    m_btnTransfer.SetTooltipText(strButtonTip);
//	m_btnTransfer.DrawBorder(TRUE);
    ////////////////////////////////////////////////////////////////////////// add by liub end
    InitFont();
    InitCombo(); //12-8
    InitTabCtrl();
    InitListCtrl();
    InitDspIndexArr(LOCFILTER_PC);

    DispSms();
    m_cmbLocFilter.SetCurSel(LOCFILTER_PC);
    OnSelchangeCombolocfilter();
    GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
    GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_NORMAL);
    GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

//	InitSMSTree();//初始化短信树形结构
    return TRUE;
}

//wyw
void CSmsDlg::OnDestroy()
{
    if (m_imagelistSMSTree) {
        int i = 0;
        for (; i < m_imagelistSMSTree->GetImageCount(); i++) {
            m_imagelistSMSTree->Remove(i);
        }
        delete m_imagelistSMSTree;
        m_imagelistSMSTree = NULL;
    }
}

//for new sms tag display error begin
int InitSMSRecord = 0;
BOOL SMS_FULL_Prompt  = TRUE;
//for new sms tag display error end
void CSmsDlg::OnInitSMSRecord()
{
    if(!g_bSmsSyncComm) {
        //m_cmbLocFilter.SetCurSel(LOCFILTER_ME);
        //OnSelchangeCombolocfilter();
        m_cmbLocFilter.SetCurSel(LOCFILTER_ME);
        OnSelchangeCombolocfilter();
        m_cmbLocFilter.SetCurSel(LOCFILTER_PC);
        OnSelchangeCombolocfilter();
    }

    return ;
}

//功能：初始化标签
//输入：无
//输出：无
void CSmsDlg::InitTabCtrl()
{
    if(m_ilTabs.Create(IDB_SMS_TYPE, 16, 1, 255)) {
        m_tabSms.SetImageList(&m_ilTabs);
    }

    m_tabSms.SetParent(this);

    CString strTabCaption;

    strTabCaption.LoadString(IDS_SMS_INBOX);
    m_tabSms.InsertItem(0, strTabCaption, 0);

    strTabCaption.LoadString(IDS_SMS_OUTBOX);
    m_tabSms.InsertItem(1, strTabCaption, 1);

    strTabCaption.LoadString(IDS_SMS_DRAFT);
    m_tabSms.InsertItem(2, strTabCaption, 2);


    DWORD dwFlags = 0;
    dwFlags |= ETC_FLAT;
    dwFlags |= ETC_SELECTION;
    dwFlags |= ETC_GRADIENT;

    m_tabSms.SetBkgndColor(RGB(238,241,243));
    m_tabSms.EnableDraw(BTC_ALL);

    CEnTabCtrl::EnableCustomLook(dwFlags, dwFlags);

    //列表控件类型初始化为收件箱
    m_DispFilter = SMS_TYPE_INBOX;
    m_lstSms.SetDspFilter(m_DispFilter);
}

void CSmsDlg::OnSelchangeTabSms(NMHDR* pNMHDR, LRESULT* pResult)
{
    int CurSel = m_tabSms.GetCurSel();

    if(m_DispFilter == CurSel)
        return;

    switch(CurSel) {
    case 0:
        m_DispFilter = SMS_TYPE_INBOX;
        break;
    case 1:
        m_DispFilter = SMS_TYPE_OUTBOX;
        break;
    case 2:
        m_DispFilter = SMS_TYPE_DRAFT;
        break;
    default:
        m_DispFilter = SMS_TYPE_INBOX;
        break;
    }

    if(m_DispFilter == SMS_TYPE_INBOX) {
        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);
    } else {
        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_NORMAL);
    }

    //列表控件类型参数更改
    m_lstSms.SetDspFilter(m_DispFilter);

    //显示短信记录列表
    DispSms();

    *pResult = 0;
}

//功能：显示SMS列表
//输入：无
//输出：无
void CSmsDlg::DispSms()
{
    UCHAR chr;
    WORD  cnt;
    WORD  nSmsNum;
    StSmsRecord SmsRecord;
    StDspEntityType SmsDspEntity;
    //TCHAR  szName[PB_NAME_MAX + 2];
//	char szName[PB_NAME_MAX + 2];  //Modified by Zhou Bin 2008.12.30
    TCHAR szName[(PB_NAME_MAX + 2)*2];
    memset(szName, 0x00, (PB_NAME_MAX + 2)*2);		//wyw_0409 add
    CString strUnknown;

    int SMSNumLength;
    int SMSNumLength1;
    int SMSNumLength2;

    memset(&SmsRecord, 0x00, sizeof(StSmsRecord));
    SmsRecord.m_NoATRspCDS = TRUE;
    strUnknown.LoadString(IDS_UNKNOWN);

    ASSERT(m_DispFilter >= SMS_TYPE_INBOX && m_DispFilter < SMS_TYPE_ALL);
    ASSERT(m_LocFilter >= LOCFILTER_PC && m_LocFilter <= LOCFILTER_ALL);

    if(!(g_bSmsSyncComm || m_LocFilter == LOCFILTER_PC))
        return;

    nSmsNum = GetTotalSmsNum(m_DispFilter, m_LocFilter);

    m_lstSms.SetRedraw(FALSE);

    m_lstSms.DeleteAllItems();

    for(cnt = 0; cnt < nSmsNum; cnt++) {
        //memset(szName, 0, sizeof(szName));

        SmsDspEntity = m_DspEntityArr[m_DispFilter][m_LocFilter][cnt];
        SmsRecord = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, cnt);
        SMSNumLength2 = wcslen((TCHAR*)SmsRecord.szNumber);
        SMSNumLength1 = strlen((char*)SmsRecord.szNumber);
        if (SMSNumLength1 == SMSNumLength2)
            SMSNumLength = SMSNumLength1;
        else
            SMSNumLength = SMSNumLength2;

        if(m_DispFilter == SMS_TYPE_DRAFT) {
            m_lstSms.InsertItem(cnt, _T(""), LOC_MAX * SMS_STATE_MAX+24);
        } else {
            if (SmsRecord.SMSpriority == 2 || SmsRecord.SMSpriority == 1) {
                m_lstSms.InsertItem(cnt, _T(""), (int)(SmsRecord.state + 4 * SmsDspEntity.LocType)+12);//add by liub for TATA 短信图标增加优先级
            } else if (SmsRecord.SMSpriority ==3) {
                m_lstSms.InsertItem(cnt, _T(""), (int)(SmsRecord.state + 4 * SmsDspEntity.LocType)+24);//add by liub for TATA 短信图标增加优先级
            } else
// 				if (SmsRecord.m_FlashMessage == TRUE)
// 				{
// 					//m_lstSms.InsertItem(cnt, _T(""), (int)(SmsRecord.state + 4 * SmsDspEntity.LocType)+14);//add by liub for TATA 短信图标增加优先级
// 					m_lstSms.InsertItem(cnt, _T(""), LOC_MAX * SMS_STATE_MAX+25);
// 				}
//        		else
                m_lstSms.InsertItem(cnt, _T(""), (int)(SmsRecord.state + 4 * SmsDspEntity.LocType));
        }

        USES_CONVERSION;
        if(SmsRecord.szNumber[0] != '\0' /*|| m_LocFilter == LOCFILTER_UIM*/) {
            chr = *(SmsRecord.szNumber + 1);
            if(chr) {
                if(m_pPbData->SearchNameByNumber(CString(SmsRecord.szNumber), szName))
                    m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME, szName);
                else
                    m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME,A2W(SmsRecord.szNumber));//liub_modify 若未找到电话本姓名记录，
            } else {
                if(m_pPbData->SearchNameByNumber((TCHAR*)SmsRecord.szNumber, szName))
                    m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME, szName);
                else
                    m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME,(LPCTSTR)SmsRecord.szNumber);
            }
        } else {
            m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME, (LPCWSTR)strUnknown);
            // m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NUMBER, (LPCWSTR)strUnknown); //号码列已经取消
        }

        // @@@

        m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_DATETIME, (LPCWSTR)SmsRecord.timestamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT));
        //m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_DATETIME, SmsRecord.timestamp.Format());
        //@@@

        //Modified by Zhou Bin 2008.12.30
        //TCHAR szHead[30];
        char szHead[30];
        memset(szHead, 0x00, sizeof(szHead));
        //TCHAR szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX + 20];
        char szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX + 20];
        char szContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 2];
        memset(szContent, 0x0000, sizeof(szContent));
        memset(szDspContent, 0x0000, sizeof(szDspContent));

        switch (SmsRecord.SMSpriority) {
        case  1:
            strncpy(szContent, W2A(_T("<Interactive!> \r\n")), 17);
            break;

        case  2:
            //record.szContent = _T("Urgent! ");
            strncpy(szContent, W2A(_T("<Urgent!> \r\n")), 12);
            break;
        case  3:
            //record.szContent = _T("Emergency! ");
            strncpy(szContent, W2A(_T("<Emergency!> \r\n")), 15);
            break;
        default:
            break;

        }
        strncat(szContent,(const char*)SmsRecord.szContent,sizeof(szContent));
        char *p;
        p = szContent;
        for (int i=0; i<sizeof(szContent); i++) {
            if (*p == '\r' || *p == '\n') {
                *p = ' ';
            }
            p++;
        }

        if(SmsRecord.flag & SMS_RECORD_FLAG_CONCATENATE_SEGE) {
            if(DspConcatenateSmsPara(szHead, SmsRecord.nRefCnt, SmsRecord.nSeqCnt, SmsRecord.nTotalCnt))

                //strcpy(szDspContent,szHead);
                wcscpy((TCHAR*)szDspContent, A2W(szHead));
        }

//     	if(chr)
// 			wcsncat((TCHAR*)szDspContent, (const TCHAR*)SmsRecord.szContent, sizeof(szDspContent));
// 		else
// 	    	strncat(szDspContent, SmsRecord.szContent, sizeof(szDspContent));
//////////////////////////////////////////////////////////////////////////
// 1		if(chr)
// 		{
// 			TCHAR *WCharDspContent=A2W(SmsRecord.szContent);
// 			//m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, szDspContent);
// 	    	m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, WCharDspContent);
// 		}
// 		else
// 		{
//////////////////////////////////////////////////////////////////////////
//2 			if (IsAlphabetUnicode((const TCHAR *)SmsRecord.szContent) /*&& m_LocFilter != LOCFILTER_PC*/)
// 			//if (IsAlphabet((const char *)SmsRecord.szContent) /*&& m_LocFilter != LOCFILTER_PC*/)
// 			{
// 				//TCHAR *WCharDspContent=A2W(SmsRecord.szContent);
// 				//m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, WCharDspContent);
// 				m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, (LPCTSTR)SmsRecord.szContent);
//
// 			}
// 			else
// 			{
//
// 				TCHAR *WCharDspContent=A2W(SmsRecord.szContent);
// 				m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, WCharDspContent);
// 				//m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, (LPCTSTR)SmsRecord.szContent);
//
// 			}
//////////////////////////////////////////////////////////////////////////
//  1   		if (SMSNumLength == 1 && m_LocFilter == LOCFILTER_PC && m_DispFilter != SMS_TYPE_INBOX)
// 			{
// 				TCHAR *WCharDspContent=A2W(SmsRecord.szContent);
//                 m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, WCharDspContent);
// 			}
// 			else
//                 m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, (LPCTSTR)SmsRecord.szContent);
// 		}

//////////////////////////////////////////////////////////////////////////
//3
        int SMSNumLength;
        if(SmsRecord.szNumber[0] == '\0' || chr) {
// 			if (m_LocFilter == LOCFILTER_UIM && chr == 0)
// 			{
// 				//GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText((TCHAR*)SmsRecord.szContent);
// 				m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, (TCHAR*)szContent);
// 			}
// 			else
            //GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText((CString)SmsRecord.szContent);
            m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, (CString)szContent);
            //	m_SmsContent=A2W(SmsRecord.szContent);//liub_modify 若未找到电话本姓名记录，
        } else {
            SMSNumLength = wcslen((TCHAR*)SmsRecord.szNumber);
            if (m_LocFilter == LOCFILTER_PC && SMSNumLength == 1 && m_DispFilter != SMS_TYPE_INBOX) {
                m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, (CString)szContent);
                //GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText((CString)SmsRecord.szContent);
            } else {
                //modify by lijl 2009.4.13
                m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, (CString)szContent);
            }
            //GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText((TCHAR*)SmsRecord.szContent);
            //	m_SmsContent=SmsRecord.szContent;
        }

    }

    m_lstSms.m_headerCtrl.SetSortImage(m_SortColArr[m_DispFilter][m_LocFilter], m_bSortAscendArr[m_DispFilter][m_LocFilter]);

    m_lstSms.SetRedraw(TRUE);
    m_lstSms.Invalidate();
    m_lstSms.UpdateWindow();
}

//功能：初始化列表
//输入：无
//输出：无
void CSmsDlg::InitListCtrl()
{
    if(m_ilLsts.Create(IDB_SMS_STATE, 16, 1, 255)) {
        m_lstSms.SetImageList(&m_ilLsts, LVSIL_SMALL);
    }

    //m_lstSms.SetParent(&m_tabSms);

    /* CRect rect;
     m_tabSms.GetItemRect(0, &rect);

     m_lstSms.SetWindowPos(NULL, 4, 28, rect.right+100, rect.bottom+100, TRUE);
     m_lstSms.GetWindowRect(&rect);
    */
    CString strColTitle;
    strColTitle.LoadString(IDS_NAMEANDNUMBER);
    m_lstSms.InsertColumn(SMS_LISTCTRL_HEADERINDEX_NAME, strColTitle,LVCFMT_LEFT,210);
    //liubo_modify 取消号码列显示
    //strColTitle.LoadString(IDS_NUMBER);
    //m_lstSms.InsertColumn(SMS_LISTCTRL_HEADERINDEX_NUMBER, strColTitle,LVCFMT_LEFT,10);
    strColTitle.LoadString(IDS_SMS_DATETIME);
    m_lstSms.InsertColumn(SMS_LISTCTRL_HEADERINDEX_DATETIME, strColTitle,LVCFMT_LEFT,210);
    strColTitle.LoadString(IDS_SMS_CONTENT);
    //m_lstSms.InsertColumn(SMS_LISTCTRL_HEADERINDEX_CONTENT, strColTitle,LVCFMT_LEFT,rect.Width() - 470);
    m_lstSms.InsertColumn(SMS_LISTCTRL_HEADERINDEX_CONTENT, strColTitle,LVCFMT_LEFT,281);
    //m_lstSms.InsertColumn(SMS_LISTCTRL_HEADERINDEX_CONTENT, strColTitle,LVCFMT_LEFT,rect.Width() - 40*3);

    m_lstSms.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_lstSms.m_headerCtrl.DrawHeader();
}

int CSmsDlg::DealwithResponseProc(LPARAM lParam,   // application-defined value
                                  WPARAM wParam   // application-defined value
                                 )
{
    DispSms();
    return 1;
}

LRESULT CSmsDlg::OnSendSms(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CSmsDlg::OnButtonSmsRead()
{

    int nCurSel;

    int nCurSel_Index;
    //for new sms tag display error
    SndAtSmsRead();
    //for new sms tag display error
    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;
    else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    if(m_pReadDlg == NULL) {


        CSmsReadDlg dlg(m_DispFilter, m_LocFilter, nCurSel, m_DspEntityArr, this);
        m_pReadDlg = &dlg;
        dlg.DoModal();
        m_pReadDlg = NULL;
        //for new sms tag display error begin

        nCurSel_Index = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].nIndex;
        m_locType1 = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].LocType;


        SndAtSmsQCMGR(nCurSel_Index);
        Sleep(80);
        SndAtOffSmsRead();


        //for new sms tag display error end
    }

    else {
        WPARAM para = (WPARAM)m_DispFilter;
        para <<= 16;
        para |= (WPARAM)m_LocFilter;
        m_pReadDlg->PostMessage(WM_READSMS_UPDATE, (WPARAM)para, (LPARAM)nCurSel);
    }

}

void CSmsDlg::OnButtonSmsReply()
{
    // TODO: Add your control notification handler code here
    int nCurSel;
    UCHAR chr;

    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;
    else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    StSmsRecord record = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, nCurSel);
    chr = *(record.szNumber + 1);
    if (chr) {
        ///add by liub for Unicode显示，字节转换（短——》宽）
        DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, record.szNumber, -1, NULL, 0);
        wchar_t *pwText;
        pwText = new wchar_t[dwNum];
        if(!pwText) {
            delete []pwText;
        }
        MultiByteToWideChar (CP_ACP, 0, record.szNumber, -1, pwText, dwNum);
        ////////////////
#ifdef FEATURE_GCF_SMSREPLY
        CSmsWriteDlg dlg(this, (LPCTSTR)(record.szNumber, NULL,
                                         SMS_WRITE_REPLY, (LPCTSTR)record.szSCNumber);
#else
        CSmsWriteDlg dlg(this, (LPCTSTR)pwText, NULL,
                         SMS_WRITE_REPLY);
        delete []pwText;
#endif
                         dlg.DoModal();
    } else {
        CSmsWriteDlg dlg(this, (LPCTSTR)record.szNumber, NULL,
                         SMS_WRITE_REPLY);
        dlg.DoModal();
    }


}

void CSmsDlg::OnButtonSmsDial()
{
    // TODO: Add your control notification handler code here
    int nCurSel;

    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;
    else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);

    if(pComm->CommIsReady()) {
        StSmsRecord record = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, nCurSel);
// 		//----------------------------------------------------------
        USES_CONVERSION;
        UCHAR chr;
        chr = *(record.szNumber +1);

        WPARAM wParam;
        LPARAM lParam;

        if(chr) {
            wParam = 0;

        } else {
            wParam = 1;

        }


// //----------------------------------------------------------

        lParam = (LPARAM)(LPCTSTR)record.szNumber;

        if(wcslen((LPCTSTR)lParam) > 0)
            ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam, lParam);
        else
            AfxMessageBox(IDS_SELCONNECT_NULLNUM);
    }
}

void CSmsDlg::SmsDelete_flexi(int index_flexi)
{
    // TODO: Add your control notification handler code here
    int nCurSel;
    nCurSel = index_flexi;

//     if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
//         return;
//     else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1)
//     {
//         AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
//         return;
//     }
//
//     CString str;
//     str.LoadString(IDS_DELETECONFIRM);
//
//     if(MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
//         return;
//     else

    WORD i;
    WORD cnt;
    WORD nIndex;
    EnLocType loctype;

    m_DispFilter = SMS_TYPE_INBOX;
    m_LocFilter = LOCFILTER_PC;


    memset(m_DelDspIndexArr, 0x00, sizeof(m_DelDspIndexArr));
    memset(m_DelNumArr, 0, sizeof(m_DelNumArr));

    do {
        loctype = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].LocType;
        m_DelDspIndexArr[loctype][m_DelNumArr[loctype]++] = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].nIndex;
        //loctype = m_DspEntityArr[SMS_TYPE_INBOX][LOCFILTER_PC][nCurSel].LocType;
        //m_DelDspIndexArr[loctype][m_DelNumArr[loctype]++] = m_DspEntityArr[SMS_TYPE_INBOX][LOCFILTER_PC][nCurSel].nIndex;
    } while((nCurSel = m_lstSms.GetNextItem(nCurSel, LVNI_SELECTED)) != -1);

    for(cnt = 0; cnt < m_DelNumArr[LOC_PC]; cnt++) {
        nIndex = m_DelDspIndexArr[LOC_PC][cnt];
        m_pSmsData->DeleteSmsRecord(m_DispFilter, nIndex);
        for(i = cnt+1; i < m_DelNumArr[LOC_PC]; i++) {
            if(m_DelDspIndexArr[LOC_PC][i] >= nIndex)
                m_DelDspIndexArr[LOC_PC][i]--;
        }
    }

    BOOL bSndRes = FALSE;
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

    if(m_DelNumArr[LOC_ME] + m_DelNumArr[LOC_UIM] > 0) {
        m_ProcType = PROC_TYPE_DELETE;
        m_str_sms_Prompt.LoadString(IDS_WAIT_DELETE_SMS);
        ProgressOpen();

        g_ME_SmsCnt = 0;
        g_SM_SmsCnt = 0;

        if(m_DelNumArr[LOC_ME] > 0) {
            bSndRes = SndAtSmsQCPMS(LOC_ME, LOC_UIM);
        } else {
            bSndRes = SndAtSmsQCPMS(LOC_UIM, LOC_ME);
        }

        if(bSndRes) {
            ::ResetEvent(g_BGPassEvt);
            pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
        }
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }

    if(m_DelNumArr[LOC_PC] > 0 && !bSndRes) {
        UpdateListAfterDelete();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }


    HTREEITEM hRoot = m_treeSms.GetRootItem();
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
}

void CSmsDlg::OnButtonSmsDelete()
{
    // TODO: Add your control notification handler code here
    int nCurSel;

    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;
    else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    CString str;
    str.LoadString(IDS_DELETECONFIRM);

    if(MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
        return;
    else {
        WORD i;
        WORD cnt;
        WORD nIndex;
        EnLocType loctype;


        memset(m_DelDspIndexArr, 0x00, sizeof(m_DelDspIndexArr));
        memset(m_DelNumArr, 0, sizeof(m_DelNumArr));

        do {
            loctype = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].LocType;
            m_DelDspIndexArr[loctype][m_DelNumArr[loctype]++] = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].nIndex;
        } while((nCurSel = m_lstSms.GetNextItem(nCurSel, LVNI_SELECTED)) != -1);

        for(cnt = 0; cnt < m_DelNumArr[LOC_PC]; cnt++) {
            nIndex = m_DelDspIndexArr[LOC_PC][cnt];
            m_pSmsData->DeleteSmsRecord(m_DispFilter, nIndex);
            for(i = cnt+1; i < m_DelNumArr[LOC_PC]; i++) {
                if(m_DelDspIndexArr[LOC_PC][i] >= nIndex)
                    m_DelDspIndexArr[LOC_PC][i]--;
            }
        }

        BOOL bSndRes = FALSE;
        CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

        if(m_DelNumArr[LOC_ME] + m_DelNumArr[LOC_UIM] > 0) {
            m_ProcType = PROC_TYPE_DELETE;
            m_str_sms_Prompt.LoadString(IDS_WAIT_DELETE_SMS);
            ProgressOpen();

            g_ME_SmsCnt = 0;
            g_SM_SmsCnt = 0;

            if(m_DelNumArr[LOC_ME] > 0) {
                bSndRes = SndAtSmsQCPMS(LOC_ME, LOC_UIM);
            } else {
                bSndRes = SndAtSmsQCPMS(LOC_UIM, LOC_ME);
            }

            if(bSndRes) {
                ::ResetEvent(g_BGPassEvt);
                pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
            }
            GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
        }

        if(m_DelNumArr[LOC_PC] > 0 && !bSndRes) {
            UpdateListAfterDelete();
            GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
        }
    }

    HTREEITEM hRoot = m_treeSms.GetRootItem();
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
}

void CSmsDlg::OnButtonSmsNew()
{
    // TODO: Add your control notification handler code here
    CSmsWriteDlg dlg(this);
    dlg.DoModal();
}

void CSmsDlg::OnButtonSmsForward()
{
    // TODO: Add your control notification handler code here
    int nCurSel;
//	int CountLen;
    CString szContentCopy;

    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;
    else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    StSmsRecord record = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, nCurSel);
    ForwardSMSpriority = record.SMSpriority;
    char szContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*4 + 4];
    memset(szContent, 0x0000, sizeof(szContent));
    strncat(szContent,(const char*)record.szContent,sizeof(szContent));
    char *p,*q,*temp;
    p = szContent;
    q = szContent + 1;
    temp = szContent;
    while(*temp != '\0')
        temp++;
    for (int i=0; i<sizeof(szContent); i++) {
        if (*p == '\r' && *q == '\n') {
            p = p +2;
            q = q +2;
        } else if (*p == '\n') {
            while(temp!= p) {
                *temp = *(temp - 1);
                temp--;
            }
            *temp = '\r';
            while(*temp != '\0')
                temp++;

            p = p +2;
            q = q +2;
        } else if (*p == '\r') {
            *p = ' ';
            p++;
            q++;
        } else {
            p++;
            q++;
        }
    }



    UCHAR chr;
    chr = *(record.szNumber + 1);

// 	if (m_LocFilter == LOCFILTER_PC)
// 	{
// 		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, record.szContent, -1, NULL, 0);
// 		wchar_t *pwText;
// 		pwText = new wchar_t[dwNum];
// 		if(!pwText)
// 		{
// 			delete []pwText;
// 		}
// 		MultiByteToWideChar (CP_ACP, 0, record.szContent, -1, pwText, dwNum);
//
// 		CSmsWriteDlg dlg(this, NULL, (LPCTSTR)pwText,
// 						 SMS_WRITE_FORWORD, NULL);
// 	// 	CSmsWriteDlg dlg(this, NULL, (LPCTSTR)record.szContent,
// 	//                      SMS_WRITE_FORWORD, NULL);
// 		delete []pwText;
// 		dlg.DoModal();
// 	}
    /*	else*/
    {
        //modify by lijl 2009.4.13
        //	CountLen = wcslen((CString)szContent);


        /*	const TCHAR **/
        //TCHAR *szRawContent = ;
        gSmsIsAsciiCode = IsAlphabetUnicode((CString)szContent);

        if(gSmsIsAsciiCode) {
            if(GetACSIICharNum((CString)szContent) > SMS_CONCAT8_ASCII_MAX * SMS_CONCAT_SEGMENT_MAX) {
                szContentCopy = (CString)szContent;
                szContentCopy = szContentCopy.Left(SMS_CONCAT8_ASCII_MAX * SMS_CONCAT_SEGMENT_MAX);
            } else
                szContentCopy = (CString)szContent;
        } else {
            if(GetUnicodeCharNum((CString)szContent) > SMS_CONCAT_GB_MAX * SMS_CONCAT_SEGMENT_MAX) {
                szContentCopy = (CString)szContent;
                szContentCopy = szContentCopy.Left(SMS_CONCAT_GB_MAX * SMS_CONCAT_SEGMENT_MAX);
            } else
                szContentCopy = (CString)szContent;
        }

        CSmsWriteDlg dlg(this, NULL, szContentCopy,
                         SMS_WRITE_FORWORD, NULL);
        dlg.DoModal();
    }
}

void CSmsDlg::OnButtonSmsAbstract()
{
    // TODO: Add your control notification handler code here
    int nCurSel;

    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;

    if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    if(m_pPbData->CheckFull()) {
        AfxMessageBox(IDS_PB_FULL);
        return;
    }

    StSmsRecord SmsRecord = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, nCurSel);
    BOOL UnicodeorAsii = IsAlphabetUnicode((TCHAR*)SmsRecord.szContent);

    UCHAR chr;
    chr = *(SmsRecord.szNumber + 1);

// 	if (m_LocFilter == LOCFILTER_PC)
// 	{
// 	///add by liub for Unicode显示，字节转换（短——》宽）
//     DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, SmsRecord.szNumber, -1, NULL, 0);
//
// 	wchar_t *pwText;
// 	pwText = new wchar_t[dwNum];
// 	if(!pwText)
// 	{
// 	   delete []pwText;
// 	}
//     MultiByteToWideChar (CP_ACP, 0, SmsRecord.szNumber, -1, pwText, dwNum);
//     ///////////////////////////////////////////////////
// 	DWORD dwNumContent = MultiByteToWideChar (CP_ACP, 0, SmsRecord.szContent, -1, NULL, 0);
//
// 	wchar_t *pwTextContent;
// 	pwTextContent = new wchar_t[dwNumContent];
// 	if(!pwTextContent)
// 	{
// 		delete []pwTextContent;
// 	}
//     MultiByteToWideChar (CP_ACP, 0, SmsRecord.szContent, -1, pwTextContent, dwNumContent);
//     ///////////////////////////////////////////////////
//     BYTE num;
//     num = ExtractNumFromSmsContent((const TCHAR *)pwTextContent);
//
//     if(num == 0 && wcslen((TCHAR*)pwText) == 0)
//         AfxMessageBox(IDS_SELCONNECT_NULLNUM);
//     else
//     {
//         if(num == 0)
//         {
//             if(CompareChar((TCHAR*)pwText))
//             {
//                 CSelPosition dlg(this, (TCHAR *)pwText);
//                 dlg.DoModal();
//                 DispSms();
//             }
//         }
//         else
//         {
//             CSmsExtractNumDlg dlg((LPCTSTR)pwText);
//             dlg.DoModal();
//             DispSms();
//         }
//     }
// 	delete []pwText;
// 	delete []pwTextContent;
// 	}
// 	else
    {
        BYTE num;
        // num = ExtractNumFromSmsContent((const TCHAR *)SmsRecord.szContent);

        if (UnicodeorAsii) {
            DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,(TCHAR*)SmsRecord.szContent,-1,NULL,0,NULL,FALSE);
            char *psText;
            psText = new char[dwNum+1];
            if(!psText) {
                delete []psText;
            }
            WideCharToMultiByte (CP_OEMCP,NULL,(TCHAR*)SmsRecord.szContent,-1,psText,dwNum,NULL,FALSE);

            num = ExtractNumFromSmsContent((const char *)psText);
            delete []psText;
        } else
            num = ExtractNumFromSmsContent((const char *)SmsRecord.szContent);
        if(num == 0 && wcslen((TCHAR*)SmsRecord.szNumber) == 0)
            AfxMessageBox(IDS_SELCONNECT_NULLNUM);
        else {
            if(num == 0) {
                if(chr) {
                    if(CompareChar((CString)SmsRecord.szNumber)) {
                        CSelPosition dlg(this, (CString)SmsRecord.szNumber);
                        dlg.DoModal();
                        DispSms();
                    }
                } else {
                    if(CompareChar((TCHAR*)SmsRecord.szNumber)) {
                        CSelPosition dlg(this, (TCHAR *)SmsRecord.szNumber);
                        dlg.DoModal();
                        DispSms();
                    }
                }
            } else {
                if(chr) {
                    //	if(CompareChar((CString)SmsRecord.szNumber))
                    {
                        CSmsExtractNumDlg dlg((CString)SmsRecord.szNumber);
                        dlg.DoModal();
                        DispSms();
                    }
                } else {
                    //	if(CompareChar((TCHAR*)SmsRecord.szNumber))

                    CSmsExtractNumDlg dlg((TCHAR*)SmsRecord.szNumber);
                    dlg.DoModal();
                    DispSms();
                }

                //CSmsExtractNumDlg dlg((LPCTSTR)SmsRecord.szNumber);
// 				CSmsExtractNumDlg dlg((CString)SmsRecord.szNumber);
//
// 				dlg.DoModal();
// 				DispSms();
            }
        }

    }

}

void CSmsDlg::OnButtonSmsDelall()
{
    /*
    // TODO: Add your control notification handler code here
    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;

    CString str;
    str.LoadString(IDS_DELALLCONFIRM);
    if(MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)
        == IDNO)
        return;
    else
    {
        m_lstSms.DeleteAllItems();
        m_pSmsData->DelallSmsRecord(m_DispFilter);
        InitDspIndexArr();

        if(m_DispFilter == SMS_TYPE_INBOX)
        {
            ((CHSDPADlg*)AfxGetMainWnd())->UpdateUnreadSmsIcon();
            ((CHSDPADlg*)AfxGetMainWnd())->UpdateFullSmsIcon();
        }
    }
    */
}

////del by liub 1103 for SMS
/*void CSmsDlg::OnDblclkListSms(NMHDR* pNMHDR, LRESULT* pResult)
{
    if(m_DispFilter == SMS_TYPE_DRAFT)
        OnButtonSmsEdit();
    else
        OnButtonSmsRead();
    *pResult = 0;
}
*/
//liub_add_begin
//添加单击SMS列表显示其内容功能
void CSmsDlg::OnclkListSms(NMHDR* pNMHDR, LRESULT* pResult)
{
    WORD curindex;
    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;
    else if((curindex = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1 ||
    curindex > GetTotalSmsNum(m_DispFilter, m_LocFilter)) {
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
        //AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }
    //curindex = m_lstSms.GetNextItem(-1,LVNI_SELECTED);
    /*if(m_DispFilter == SMS_TYPE_DRAFT)
        OnButtonSmsEdit();*///del by liub
    //else
    // OnButtonSmsRead();
    {
        ASSERT(m_DispFilter >= SMS_TYPE_INBOX && m_DispFilter < SMS_TYPE_ALL);
        ASSERT(curindex < GetTotalSmsNum(m_DispFilter, m_LocFilter));
        USES_CONVERSION;

        CString strUnknown;
        TCHAR  szName[PB_NAME_MAX + 2];
        char szContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*4 + 4];
        memset(szContent, 0x0000, sizeof(szContent));
        memset(szName, 0, sizeof(szName));
        StSmsRecord SmsRecord = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, curindex);

        switch (SmsRecord.SMSpriority) {
        case  1:
            strncpy(szContent, W2A(_T("<Interactive!> \r\n")), 17);
            break;
        case  2:
            //record.szContent = _T("Urgent! ");
            strncpy(szContent, W2A(_T("<Urgent!> \r\n")), 12);
            break;
        case  3:
            //record.szContent = _T("Emergency! ");
            strncpy(szContent, W2A(_T("<Emergency!> \r\n")), 15);
            break;
        default:
            break;

        }

        strncat(szContent,(const char*)SmsRecord.szContent,sizeof(szContent));
        char *p,*q,*temp;
        p = szContent;
        q = szContent + 1;
        temp = szContent;
        while(*temp != '\0')
            temp++;
        for (int i=0; i<sizeof(szContent); i++) {
            if (*p == '\r' && *q == '\n') {
                p = p +2;
                q = q +2;
            } else if (*p == '\n') {
                while(temp!= p) {
                    *temp = *(temp - 1);
                    temp--;
                }
                *temp = '\r';
                while(*temp != '\0')
                    temp++;

                p = p +2;
                q = q +2;
            } else if (*p == '\r') {
                *p = ' ';
                p++;
                q++;
            } else {
                p++;
                q++;
            }
        }

//----------------------------------------------------------
//		USES_CONVERSION;
        UCHAR chr;
        chr = *(SmsRecord.szNumber +1);
        int SMSNumLength;
        if(SmsRecord.szNumber[0] == '\0' || chr) {
// 			if (m_LocFilter == LOCFILTER_UIM && chr == 0)
// 			{
// 				GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText((TCHAR*)szContent);
// 			}
// 			else
            GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText((CString)szContent);
            //	m_SmsContent=A2W(SmsRecord.szContent);//liub_modify 若未找到电话本姓名记录，
        } else {
            SMSNumLength = wcslen((TCHAR*)SmsRecord.szNumber);
            if (m_LocFilter == LOCFILTER_PC && SMSNumLength == 1 && m_DispFilter != SMS_TYPE_INBOX) {
                GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText((CString)szContent);
            } else {
                //modify by lijl 2009.4.13
                GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText((CString)szContent);
            }
        }

//----------------------------------------------------------
        if(SmsRecord.state == SMS_STATE_MT_NOT_READ) {
            SmsRecord.state = SMS_STATE_MT_READ;
            EditTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, curindex, SmsRecord);
            //        m_pSmsData->EditSmsRecord(m_DispFilter, nIndex, SmsRecord);
            ((CHSDPADlg*)AfxGetMainWnd())->UpdateUnreadSmsIcon();
        }

        LVITEM item;
        item.iItem = curindex;
        item.iSubItem = 0;
        item.mask = LVIF_IMAGE;
        m_lstSms.GetItem(&item);
        //短信图标未读改为已读
        if((item.iImage != LOC_MAX * SMS_STATE_MAX+24)//add by liub 短信图标更改
        && (item.iImage % 4 == 0)) {
            item.iImage++;
            m_lstSms.SetItem(&item);
            m_lstSms.Invalidate();
        }
        if (SmsRecord.m_NoATRspCDS == FALSE) {
            DeleteCDSSMS();

        }

        UpdateData(FALSE);



    }
    *pResult = 0;


    HTREEITEM hRoot = m_treeSms.GetRootItem();
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);

}
//liub_add_end

void CSmsDlg::OnButtonSmsEdit()
{
    // TODO: Add your control notification handler code here
    int nCurSel;
    USES_CONVERSION;
    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;
    else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    StSmsRecord record = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, nCurSel);

    //Modified by Zhou Bin 2008.12.30
//	CSmsWriteDlg dlg(this, (LPTSTR)record.szNumber, (LPTSTR)record.szContent);
    CSmsWriteDlg dlg(this, A2W(record.szNumber), A2W(record.szContent));
    dlg.DoModal();
}

//分发列表控件发送的弹出菜单项单击消息
LRESULT CSmsDlg::OnMenuItemClick(WPARAM wParam, LPARAM lParam)
{
    UINT nID = wParam;
    ASSERT(nID >= IDC_BUTTON_SMS_READ && nID <= /*IDC_BUTTON_SMS_EXPORT*/IDC_BUTTON_SMS_RESUME);

    if(nID == IDC_BUTTON_SMS_READ)
        OnButtonSmsRead();
    else if(nID == IDC_BUTTON_SMS_NEW)
        OnButtonSmsNew();
    else if(nID == IDC_BUTTON_SMS_REPLY)
        OnButtonSmsReply();
    else if(nID == IDC_BUTTON_SMS_EDIT)
        OnButtonSmsEdit();
    else if(nID == IDC_BUTTON_SMS_FORWARD)
        OnButtonSmsForward();
    else if(nID == IDC_BUTTON_SMS_DIAL)
        OnButtonSmsDial();
    else if(nID == IDC_BUTTON_SMS_ABSTRACT)
        OnButtonSmsAbstract();
    else if(nID == IDC_BUTTON_SMS_DELETE)
        OnButtonSmsDelete();
    else if(nID == IDC_BUTTON_SMS_DELALL)
        OnButtonSmsDelall();
    else if(nID == IDC_BUTTON_SMS_TRANSFER)
        OnButtonSmsTransfer();
    else if(nID == IDC_BUTTON_SMS_EXPORT)
        OnButtonSmsExport();
    else if(nID == IDC_BUTTON_SMS_SEARCH)
        OnButtonSmsSearch();
    else if (nID == IDC_BUTTON_SMS_ARCHIVE)
        OnButtonSmsArchive();
    else if (nID == IDC_BUTTON_SMS_RESUME)
        OnButtonSmsResume();

    return 0;
}

LRESULT CSmsDlg::OnSaveSms(WPARAM wParam, LPARAM lParam)
{
    EnSmsType type = (EnSmsType)wParam;
    EnLocType loc = (EnLocType)lParam;

    UpdateSortListWhenAddDelete(type, loc);
    return 0;
}

LRESULT CSmsDlg::OnReadSms(WPARAM wParam, LPARAM lParam)
{
    WORD nCurSel = (WORD)wParam;
    WORD nSmsNum = GetTotalSmsNum(m_DispFilter, m_LocFilter);

    ASSERT(nCurSel < nSmsNum);

    UINT nState;
    for(WORD i = 0; i < nSmsNum; i++) {
        nState = m_lstSms.GetItemState(i, 0x000F);
        nState &= ~(LVIS_FOCUSED | LVIS_SELECTED);
        m_lstSms.SetItemState(i, nState, 0x000F);
    }

    m_lstSms.SetItemState(nCurSel, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);

    LVITEM item;
    item.iItem = nCurSel;
    item.iSubItem = 0;
    item.mask = LVIF_IMAGE;
    m_lstSms.GetItem(&item);

    //短信图标未读改为已读
    if((item.iImage != LOC_MAX * SMS_STATE_MAX+12)
    && (item.iImage % 4 == 0)) {
        item.iImage++;
        m_lstSms.SetItem(&item);
        m_lstSms.Invalidate();
    }

    EnLocFilter locFilter;
    if(g_bSmsSyncComm)
        //locFilter = LOCFILTER_ALL;
        locFilter = LOCFILTER_PC;
    else
        locFilter = LOCFILTER_PC;
    StSmsRecord smstemp = GetTotalSmsRecordFromDspIndex(SMS_TYPE_INBOX, locFilter, nCurSel);
    if (smstemp.m_NoATRspCDS == FALSE) {
        DeleteCDSSMS();
        m_lstSms.DeleteItem(nCurSel);
    }


    return 0;
}

//View New Sms
LRESULT CSmsDlg::OnViewSms(WPARAM wParam, LPARAM lParam)
{
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();//add by liub
    StSmsRecord sms;
    EnLocFilter locFilter;

    if(g_bSmsSyncComm)
        //locFilter = LOCFILTER_ALL;
        locFilter = LOCFILTER_PC;
    else
        locFilter = LOCFILTER_PC;

    WORD wSmsNum = GetTotalSmsNum(SMS_TYPE_INBOX, locFilter);

    for(int i = 0; i < wSmsNum; i++) {
        memset(&sms, 0x00, sizeof(StSmsRecord));
        sms = GetTotalSmsRecordFromDspIndex(SMS_TYPE_INBOX, locFilter, i);
        if(sms.flag & SMS_RECORD_FLAG_NEW) {
            m_tabSms.SetCurSel((int)SMS_TYPE_INBOX);
            m_cmbLocFilter.SetCurSel((int)locFilter);
            GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);

            GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

            GetDlgItem(IDC_BUTTON_SMS_FORWARD)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_SMS_DIAL)->ShowWindow(SW_NORMAL);
            GetDlgItem(IDC_BUTTON_SMS_DELETE)->ShowWindow(SW_NORMAL);

            NMHDR nh;
            nh.hwndFrom = m_tabSms.GetSafeHwnd();
            nh.idFrom = IDC_TAB_SMS;
            nh.code = TCN_SELCHANGE;

            WPARAM wParam = (WPARAM)MAKELONG(IDC_COMBO_LOC_FILTER, CBN_SELCHANGE);
            LPARAM lParam = (LPARAM)m_cmbLocFilter.GetSafeHwnd();

            this->SendMessage(WM_NOTIFY, IDC_TAB_SMS, (LPARAM)&nh);//修改tab项为index
            this->SendMessage(WM_COMMAND, wParam, lParam);  //修改下拉框为ALL项

            if(m_pReadDlg == NULL) {
                CSmsReadDlg dlg(SMS_TYPE_INBOX, locFilter, i, m_DspEntityArr, this);
                m_pReadDlg = &dlg;
                dlg.DoModal();
                m_pReadDlg = NULL;
                if (sms.m_NoATRspCDS == FALSE) {
                    DeleteCDSSMS();
                }
            } else {
                WPARAM para = (WPARAM)SMS_TYPE_INBOX;
                para <<= 16;
                para |= (WPARAM)locFilter;
                m_pReadDlg->PostMessage(WM_READSMS_UPDATE, (WPARAM)para, (LPARAM)i);
            }
            break;
        }
    }

    if(i >= wSmsNum)
        AfxMessageBox(IDS_NO_NEWSMS);

    HTREEITEM hRoot = m_treeSms.GetRootItem();
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);

    return 0;
}

void CSmsDlg::OnButtonSmsTransfer()
{
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);

    CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    ASSERT(pMainWnd);

    if(CPIN_SIM_NONE_REQUIRED != pMainWnd->m_cHandlePin.m_nSimStat)
        return;

//    if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
//        return;

    if(pComm->CommIsReady()) {
        CSmsTransferDlg dlg(this);
        dlg.DoModal();
    }
}

void CSmsDlg::OnButtonSmsExport()
{
    CSmsExportSel dlg(this);
    dlg.DoModal();
}

void CSmsDlg::OnColumnclickListSms(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // TODO: Add your control notification handler code here
    int col = pNMListView->iSubItem;

    ASSERT(col >= (int)SMS_LISTCTRL_HEADERINDEX_NAME && col <= (int)SMS_LISTCTRL_HEADERINDEX_CONTENT);

#if 0
    if(col == (int)m_SortColArr[m_DispFilter][m_LocFilter]) {
        m_bSortAscendArr[m_DispFilter][m_LocFilter] = !m_bSortAscendArr[m_DispFilter][m_LocFilter];
        ReverseSortIndexArr(m_DispFilter, m_LocFilter);
    } else {
        m_bSortAscendArr[m_DispFilter][m_LocFilter] = TRUE;
        m_SortColArr[m_DispFilter][m_LocFilter] = (SMS_LISTCTRL_HEADERINDEX)col;
        SortSmsList(m_DispFilter, m_LocFilter, m_SortColArr[m_DispFilter][m_LocFilter]);
    }
#else
    m_bSortAscendArr[m_DispFilter][m_LocFilter] = !m_bSortAscendArr[m_DispFilter][m_LocFilter];
    m_SortColArr[m_DispFilter][m_LocFilter] = (SMS_LISTCTRL_HEADERINDEX)col;
    SortSmsList(m_DispFilter, m_LocFilter, m_SortColArr[m_DispFilter][m_LocFilter]);
    if(!m_bSortAscendArr[m_DispFilter][m_LocFilter])
        ReverseSortIndexArr(m_DispFilter, m_LocFilter);
#endif
    m_lstSms.m_headerCtrl.SetSortImage(col, m_bSortAscendArr[m_DispFilter][m_LocFilter]);
    DispSms();

    *pResult = 0;
}

//功能：默认以时间递减顺序显示
//输入：locFilter: 位置过滤
//输出：无
void CSmsDlg::InitDspIndexArr(EnLocFilter locFilter)
{
    int i;

    for(i = SMS_TYPE_INBOX; i < SMS_TYPE_ALL; i++) {
        m_bSortAscendArr[i][locFilter] = FALSE;
        m_SortColArr[i][locFilter] = SMS_LISTCTRL_HEADERINDEX_DATETIME;
    }

    ResetDspIndexArr(SMS_TYPE_ALL, locFilter);

    if(locFilter != LOCFILTER_PC) {
        for(i = SMS_TYPE_INBOX; i < SMS_TYPE_ALL; i++) {
            SortSmsList((EnSmsType)i, locFilter, SMS_LISTCTRL_HEADERINDEX_DATETIME);
            ReverseSortIndexArr((EnSmsType)i, locFilter);
        }
    }
}

//功能：各字段以从小到大次序递增排序
//输入：type:类型过滤，locFilter: 位置过滤，col:列表头
//输出：无
void CSmsDlg::SortSmsList(EnSmsType type, EnLocFilter locFilter, SMS_LISTCTRL_HEADERINDEX col)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(locFilter >= LOCFILTER_PC && locFilter < LOCFILTER_MAX);
    // ASSERT(col >= SMS_LISTCTRL_HEADERINDEX_NAME && col <= SMS_LISTCTRL_HEADERINDEX_CONTENT);

    if(locFilter == LOCFILTER_PC && col == SMS_LISTCTRL_HEADERINDEX_DATETIME) {
        WORD j;
        WORD nNum = m_pSmsData->GetSmsNum(type);

        for(j = 0; j < nNum; j++)
            m_DspEntityArr[type][locFilter][j].nIndex = nNum - 1 - j;
    } else {
        BubbleSort(type, locFilter, col);
    }
}

//功能：以当前选中的排序关键字逆序排序
//输入：type:类型过滤，locFilter: 位置过滤
//输出：无
void CSmsDlg::ReverseSortIndexArr(EnSmsType type, EnLocFilter locFilter)
{
    StDspEntityType tmp;
    WORD nNum = GetTotalSmsNum(type, locFilter);

    for(WORD i = 0; i < nNum/2; i++) {
        tmp = m_DspEntityArr[type][locFilter][i];
        m_DspEntityArr[type][locFilter][i] = m_DspEntityArr[type][locFilter][nNum-1-i];
        m_DspEntityArr[type][locFilter][nNum-1-i] = tmp;
    }
}

//功能：非递减冒泡排序
//输入：type:类型过滤，locFilter: 位置过滤，col:列表头
//输出：无
void CSmsDlg::BubbleSort(EnSmsType type, EnLocFilter locFilter, SMS_LISTCTRL_HEADERINDEX col)
{
    //modify by lijl 2009.4.13 以下多处修改，实现按姓名或号码排序功能
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(locFilter >= LOCFILTER_PC && locFilter < LOCFILTER_MAX);
    //ASSERT(col >= SMS_LISTCTRL_HEADERINDEX_NAME && col <= SMS_LISTCTRL_HEADERINDEX_CONTENT);

    WORD nNum = GetTotalSmsNum(type, locFilter);

    int  nSubItem = (int)col;
    int  change = 1;
    StDspEntityType tmp;
    WORD i, j;
    int  res;
    UCHAR chr;
    BOOL bPrevAscii = true;
    BOOL bNextAscii = true;
    char  szCompareCharPrev[PB_NAME_MAX + 2] = {0};
    char  szCompareCharNext[PB_NAME_MAX + 2] = {0};
    CString strUnknown;
    strUnknown.LoadString(IDS_UNKNOWN);
    StSmsRecord PrevRecord;
    StSmsRecord NextRecord;
    TCHAR szMsgPrev[PB_NAME_MAX + 2] = {0};
    TCHAR szMsgNext[PB_NAME_MAX + 2] = {0};
    USES_CONVERSION;

    char szNumToName[540][PB_NAME_MAX+2];
    memset(szNumToName, 0x00, sizeof(szNumToName));

    for(i = 0; i < nNum - 1 && change; i++) {
        change = 0;
        for(j = nNum - 1; j > i; j--) {
            res = 0;
            memset(&PrevRecord, 0x00, sizeof(StSmsRecord));
            memset(&NextRecord, 0x00, sizeof(StSmsRecord));

            PrevRecord = GetTotalSmsRecordFromDspIndex(type, locFilter, j-1);
            NextRecord = GetTotalSmsRecordFromDspIndex(type, locFilter, j);

            switch(col) {
            case SMS_LISTCTRL_HEADERINDEX_NAME:
                memset(szMsgPrev, 0x00, sizeof(szMsgPrev));
                memset(szMsgNext, 0x00, sizeof(szMsgNext));
                memset(szCompareCharPrev, 0x00, sizeof(szCompareCharPrev));
                memset(szCompareCharNext, 0x00, sizeof(szCompareCharNext));

                if (strlen(szNumToName[j-1]) != 0) {
                    strcpy(szCompareCharPrev,szNumToName[j-1]);
                } else if(PrevRecord.szNumber[0] == '\0') {
                    sprintf(szCompareCharPrev,"%s",W2A(strUnknown));
                } else {
                    chr = *(PrevRecord.szNumber + 1);
                    if(chr) {
                        m_pPbData->SearchNameByNumber((CString)(PrevRecord.szNumber), szMsgPrev);
                        bPrevAscii = true;
                    } else {
                        m_pPbData->SearchNameByNumber((TCHAR*)(PrevRecord.szNumber), szMsgPrev);
                        bPrevAscii = false;
                    }
                    if(wcslen(szMsgPrev) != 0) {
                        DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,szMsgPrev,-1,NULL,0,NULL,FALSE);
                        char *psText;
                        psText = new char[dwNum+1];
                        if(!psText) {
                            delete []psText;
                        }
                        WideCharToMultiByte (CP_OEMCP,NULL,szMsgPrev,-1,psText,dwNum,NULL,FALSE);
                        strcpy(szCompareCharPrev,psText);
                        delete []psText;
                    } else if(bPrevAscii) {
                        strcpy(szCompareCharPrev,PrevRecord.szNumber);
                    } else {
                        DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,(TCHAR*)PrevRecord.szNumber,-1,NULL,0,NULL,FALSE);
                        char *psText;
                        psText = new char[dwNum+1];
                        if(!psText) {
                            delete []psText;
                        }
                        WideCharToMultiByte (CP_OEMCP,NULL,(TCHAR*)PrevRecord.szNumber,-1,psText,dwNum,NULL,FALSE);
                        strcpy(szCompareCharPrev,psText);
                        delete []psText;
                    }
                }

                if (strlen(szNumToName[j]) != 0) {
                    strcpy(szCompareCharNext,szNumToName[j]);
                } else if(NextRecord.szNumber[0] == '\0') {
                    sprintf(szCompareCharNext,"%s",W2A(strUnknown));
                } else {
                    chr = *(NextRecord.szNumber + 1);
                    if(chr) {
                        m_pPbData->SearchNameByNumber((CString)(NextRecord.szNumber), szMsgNext);
                        bNextAscii = true;
                    } else {
                        m_pPbData->SearchNameByNumber((TCHAR*)(NextRecord.szNumber), szMsgNext);
                        bNextAscii = false;
                    }

                    if(wcslen(szMsgNext) != 0) {

                        DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,szMsgNext,-1,NULL,0,NULL,FALSE);
                        char *psText;
                        psText = new char[dwNum+1];
                        if(!psText) {
                            delete []psText;
                        }
                        WideCharToMultiByte (CP_OEMCP,NULL,szMsgNext,-1,psText,dwNum,NULL,FALSE);
                        strcpy(szCompareCharNext,psText);
                        delete []psText;

                    } else if(bNextAscii) {
                        strcpy(szCompareCharNext,NextRecord.szNumber);
                    } else {
                        DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,(TCHAR*)NextRecord.szNumber,-1,NULL,0,NULL,FALSE);
                        char *psText;
                        psText = new char[dwNum+1];
                        if(!psText) {
                            delete []psText;
                        }
                        WideCharToMultiByte (CP_OEMCP,NULL,(TCHAR*)NextRecord.szNumber,-1,psText,dwNum,NULL,FALSE);
                        strcpy(szCompareCharNext,psText);
                        delete []psText;

                    }
                }
                strcpy(szNumToName[j-1], szCompareCharPrev);
                strcpy(szNumToName[j], szCompareCharNext);

                res = strcmp(szCompareCharNext, szCompareCharPrev);

                break;
            case SMS_LISTCTRL_HEADERINDEX_NUMBER:
                //res = strcmp((const char *)(NextRecord.szNumber), (const char *)(PrevRecord.szNumber));
                // res = wcscmp((const TCHAR *)(NextRecord.szNumber), (const TCHAR *)(PrevRecord.szNumber));
                res = strcmp(NextRecord.szNumber, PrevRecord.szNumber);
                break;
            case SMS_LISTCTRL_HEADERINDEX_DATETIME:
                if(NextRecord.timestamp < PrevRecord.timestamp)
                    res = -1;
                break;
            case SMS_LISTCTRL_HEADERINDEX_CONTENT:
                //res = strcmp((const char *)(NextRecord.szContent), (const char *)(PrevRecord.szContent));
                //res = wcscmp((const TCHAR *)(NextRecord.szContent), (const TCHAR *)(PrevRecord.szContent));
                res = strcmp(NextRecord.szContent, PrevRecord.szContent);
                break;
            }

            if(res < 0) {
                tmp = m_DspEntityArr[type][locFilter][j];
                m_DspEntityArr[type][locFilter][j] = m_DspEntityArr[type][locFilter][j-1];
                m_DspEntityArr[type][locFilter][j-1] = tmp;
                change   = 1;

                strcpy(szNumToName[j], szCompareCharPrev);
                strcpy(szNumToName[j-1], szCompareCharNext);
            }
        }
    }
}

void CSmsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CTabedDialog::OnShowWindow(bShow, nStatus);

    if(bShow) {
        DispSms();
    }
    /*
        // TODO: Add your message handler code here
        if(bShow && !g_bSmsSyncComm)
        {
    		m_ProcType = PROC_TYPE_INIT;
    		ProgressOpen();
    		PostMessage(WM_SMS_OPERATE_PROC,0,(LPARAM)TRUE);
        }
    */
}

//功能：更新SMS排序索引
//输入：type:类型过滤，locFilter: 位置过滤
//输出：无
void CSmsDlg::UpdateSmsSortList(EnSmsType type, EnLocFilter locFilter)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(locFilter >= LOCFILTER_PC && locFilter < LOCFILTER_MAX);

    if(locFilter != LOCFILTER_PC && !g_bSmsSyncComm)
        return;

    ResetDspIndexArr(type, locFilter);

    SortSmsList(type, locFilter, m_SortColArr[type][locFilter]);
    if(!m_bSortAscendArr[type][locFilter])
        ReverseSortIndexArr(type, locFilter);

}

//功能：重置SMS排序索引
//输入：type:类型过滤，locFilter: 位置过滤
//输出：无
void CSmsDlg::ResetDspIndexArr(EnSmsType type, EnLocFilter locFilter)
{
    int i, m, cnt;
    EnSmsState state;
    USHORT CardNum = 0;
    StSmsCardRecord CardRecord;

    const EnSmsState state0[2][2] = {
        SMS_STATE_MT_NOT_READ, SMS_STATE_MT_READ,
        SMS_STATE_MO_NOT_SENT, SMS_STATE_MO_SENT
    };

    ASSERT(type >= SMS_TYPE_INBOX && type <= SMS_TYPE_ALL);
    ASSERT(locFilter >= LOCFILTER_PC && locFilter < LOCFILTER_MAX);

    if(type == SMS_TYPE_ALL) {
        for(i = SMS_TYPE_INBOX; i < SMS_TYPE_ALL; i++)
            for(m = 0; m < SMS_TOTAL_RECORD_MAX; m++) {
                m_DspEntityArr[i][locFilter][m].nIndex = 0;
                if(locFilter == LOCFILTER_ALL)
                    m_DspEntityArr[i][locFilter][m].LocType = LOC_PC;
                else
                    m_DspEntityArr[i][locFilter][m].LocType = (EnLocType)locFilter;
            }

        if(locFilter == LOCFILTER_ALL) {
            for(i = SMS_TYPE_INBOX; i < SMS_TYPE_ALL; i++) {
                cnt = 0;
                for(m = 0; m < GetTotalSmsNum((EnSmsType)i, LOCFILTER_PC); m++, cnt++) {
                    m_DspEntityArr[i][locFilter][cnt].LocType = LOC_PC;
                    m_DspEntityArr[i][locFilter][cnt].nIndex = m;
                }

                if(i != SMS_TYPE_DRAFT && GetTotalSmsNum((EnSmsType)i, LOCFILTER_ME) > 0) {
                    for(m=0; m<g_ME_SmsNum; m++) {
                        CardRecord = UE_ReadSmsCardRecord(LOC_ME, m);
                        state = CardRecord.record.state;
                        if(state == state0[i][0] || state == state0[i][1]) {
                            m_DspEntityArr[i][locFilter][cnt].LocType = LOC_ME;
                            m_DspEntityArr[i][locFilter][cnt].nIndex = m;
                            cnt++;
                        }
                    }
                }

                if(i != SMS_TYPE_DRAFT && GetTotalSmsNum((EnSmsType)i, LOCFILTER_UIM) > 0) {
                    for(m=0; m<g_SM_SmsNum; m++) {
                        CardRecord = UE_ReadSmsCardRecord(LOC_UIM, m);
                        state = CardRecord.record.state;
                        if(state == state0[i][0] || state == state0[i][1]) {
                            m_DspEntityArr[i][locFilter][cnt].LocType = LOC_UIM;
                            m_DspEntityArr[i][locFilter][cnt].nIndex = m;
                            cnt++;
                        }
                    }
                }
            }
        } else if(locFilter == LOCFILTER_PC) {
            for(i = SMS_TYPE_INBOX; i < SMS_TYPE_ALL; i++)
                for(m = 0; m < GetTotalSmsNum((EnSmsType)i, locFilter); m++)
                    m_DspEntityArr[i][locFilter][m].nIndex = m;
        } else {
            if(locFilter == LOCFILTER_ME)
                CardNum = g_ME_SmsNum;
            else
                CardNum = g_SM_SmsNum;

            for(i = SMS_TYPE_INBOX; i < SMS_TYPE_ALL; i++) {
                if(i != SMS_TYPE_DRAFT && GetTotalSmsNum((EnSmsType)i, locFilter) > 0) {
                    cnt = 0;
                    for(m=0; m<CardNum; m++) {
                        CardRecord = UE_ReadSmsCardRecord((EnLocType)locFilter, m);
                        state = CardRecord.record.state;
                        if(state == state0[i][0] || state == state0[i][1]) {
                            m_DspEntityArr[i][locFilter][cnt].LocType = (EnLocType)locFilter;
                            m_DspEntityArr[i][locFilter][cnt].nIndex = m;
                            cnt++;
                        }
                    }
                }
            }
        }
    } else {
        for(m = 0; m < SMS_TOTAL_RECORD_MAX; m++) {
            m_DspEntityArr[type][locFilter][m].nIndex = 0;
            if(locFilter == LOCFILTER_ALL)
                m_DspEntityArr[type][locFilter][m].LocType = LOC_PC;
            else
                m_DspEntityArr[type][locFilter][m].LocType = (EnLocType)locFilter;
        }

        if(locFilter == LOCFILTER_ALL) {
            cnt = 0;
            for(m = 0; m < GetTotalSmsNum(type, LOCFILTER_PC); m++, cnt++) {
                m_DspEntityArr[type][locFilter][cnt].LocType = LOC_PC;
                m_DspEntityArr[type][locFilter][cnt].nIndex = m;
            }

            if(type != SMS_TYPE_DRAFT && GetTotalSmsNum(type, LOCFILTER_ME) > 0) {
                for(m=0; m<g_ME_SmsNum; m++) {
                    CardRecord = UE_ReadSmsCardRecord(LOC_ME, m);
                    state = CardRecord.record.state;
                    if(state == state0[type][0] || state == state0[type][1]) {
                        m_DspEntityArr[type][locFilter][cnt].LocType = LOC_ME;
                        m_DspEntityArr[type][locFilter][cnt].nIndex = m;
                        cnt++;
                    }
                }
            }

            if(type != SMS_TYPE_DRAFT && GetTotalSmsNum(type, LOCFILTER_UIM) > 0) {
                for(m=0; m<g_SM_SmsNum; m++) {
                    CardRecord = UE_ReadSmsCardRecord(LOC_UIM, m);
                    state = CardRecord.record.state;
                    if(state == state0[type][0] || state == state0[type][1]) {
                        m_DspEntityArr[type][locFilter][cnt].LocType = LOC_UIM;
                        m_DspEntityArr[type][locFilter][cnt].nIndex = m;
                        cnt++;
                    }
                }
            }
        } else if(locFilter == LOCFILTER_PC) {
            for(m = 0; m < GetTotalSmsNum(type, locFilter); m++)
                m_DspEntityArr[type][locFilter][m].nIndex = m;
        } else {
            if(locFilter == LOCFILTER_ME)
                CardNum = g_ME_SmsNum;
            else
                CardNum = g_SM_SmsNum;

            if(type != SMS_TYPE_DRAFT && GetTotalSmsNum(type, locFilter) > 0) {
                cnt = 0;
                for(m=0; m<CardNum; m++) {
                    CardRecord = UE_ReadSmsCardRecord((EnLocType)locFilter, m);
                    state = CardRecord.record.state;
                    if(state == state0[type][0] || state == state0[type][1]) {
                        m_DspEntityArr[type][locFilter][cnt].LocType = (EnLocType)locFilter;
                        m_DspEntityArr[type][locFilter][cnt].nIndex = m;
                        cnt++;
                    }
                }
            }
        }
    }
}

void CSmsDlg::OnCusDrawListSms(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
    DWORD dwDS = pLVCD->nmcd.dwDrawStage;
    *pResult = CDRF_DODEFAULT;

    if(CDDS_PREPAINT == dwDS)
        *pResult = CDRF_NOTIFYITEMDRAW;
    else if(CDDS_ITEMPREPAINT == dwDS) {
//        StSmsRecord SmsRecord = m_pSmsData->ReadSmsRecord(m_DispFilter, m_DspEntityArr[m_DispFilter][pLVCD->nmcd.dwItemSpec]);
        StSmsRecord SmsRecord = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, pLVCD->nmcd.dwItemSpec);

        if(SmsRecord.state == SMS_STATE_MT_NOT_READ) {
            SelectObject(pLVCD->nmcd.hdc, m_BoldFont);
            *pResult = CDRF_NEWFONT;
        }
    }
}

//功能：初始化字体
//输入：无
//输出：无
void CSmsDlg::InitFont()
{
    LOGFONT lf;
    m_BoldFont.CreatePointFont(0, _T("System"));
    m_BoldFont.GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;
    m_BoldFont.DeleteObject();
    m_BoldFont.CreateFontIndirect(&lf);
}

//功能：初始化组合框
//输入：无
//输出：无
void CSmsDlg::InitCombo()
{
    //消息位置过滤类型初始化为PC
    m_LocFilter = LOCFILTER_PC;

    m_cmbLocFilter.SetCurSel((int)m_LocFilter);
}

void CSmsDlg::OnSelchangeCombolocfilter()
{
    CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
    EnLocFilter LocFilter;

    LocFilter = (EnLocFilter)m_cmbLocFilter.GetCurSel();

    if(m_LocFilter == LocFilter) {
        return;
    } else {
        m_LocFilter = LocFilter;


#if 0
// for new sms tag display error
        if(m_LocFilter == LOCFILTER_ME) {

            SndAtSmsQCPMS(LOC_ME, LOC_UIM);

        } else if(m_LocFilter == LOCFILTER_UIM) {

            SndAtSmsQCPMS(LOC_UIM, LOC_ME);

        }



// for new sms tag display error

#endif
    }

    if(m_LocFilter == LOCFILTER_PC
            || g_bSmsSyncComm)
        DispSms();
    else {
        //非PC端且g_bSmsSyncComm为flase
        m_ProcType = PROC_TYPE_INIT;
        ProgressOpen();
        PostMessage(WM_SMS_OPERATE_PROC,(WPARAM)AT_SMS_QCPMS,(LPARAM)TRUE);//AT_SMS_QCPMS, 存储设置
    }
}

void CSmsDlg::OnSelchangeCombolocfilterForOut()
{
    OnSelchangeCombolocfilter();


}
//功能：进度条设置
//输入：nLower:最小值，nUpper:最大值，nStep:步长
//输出：TRUE/FALSE
BOOL CSmsDlg::ProgressSet(int nLower, int nUpper, int nStep)
{
    if(pDlg) {
        m_nLower = nLower;
        m_nUpper = nUpper;
        m_nStep  = nStep;
        pDlg->m_Progress.SetRange(m_nLower, m_nUpper);
        pDlg->m_Progress.SetStep(m_nStep);
        return TRUE;
    }
    return FALSE;
}

//功能：步进进度条
//输入：无
//输出：TRUE/FALSE
BOOL CSmsDlg::ProgressStep()
{
    if(pDlg && pDlg->m_Progress.GetPos() < m_nUpper) {
        pDlg->m_Progress.StepIt();
        return TRUE;
    }
    return FALSE;
}

//功能：关闭进度框
//输入：无
//输出：TRUE/FALSE
BOOL CSmsDlg::ProgressClose()
{
    if(pDlg) {
        pDlg->PostMessage(WM_CLOSE);
        return TRUE;
    }
    return FALSE;
}

//功能：打开进度框
//输入：nUpper:最大值，nStep:步长
//输出：TRUE/FALSE
BOOL CSmsDlg::ProgressOpen(int nUpper, int nStep)
{
    PostMessage(WM_CREATE_PROGRESS, (WPARAM)nUpper,(LPARAM)nStep);
    return TRUE;
}

//功能：进度条位置
//输入：nPos:位置
//输出：TRUE/FALSE
BOOL CSmsDlg::ProgressPos(int nPos)
{
    if(pDlg && pDlg->m_Progress.GetPos() < nPos && nPos <= m_nUpper) {
        pDlg->m_Progress.SetPos(nPos);
        return TRUE;
    }
    return FALSE;
}

//功能：发送AT$QCPMS
//输入：locType1:位置1，locType2:位置2
//输出：TRUE/FALSE
BOOL CSmsDlg::SndAtSmsQCPMS(EnLocType locType1, EnLocType locType2)
{
    ASSERT(locType1 == LOC_ME || locType1 == LOC_UIM);
    ASSERT(locType2 == LOC_ME || locType2 == LOC_UIM || locType2 == LOC_MAX);

    char szAtBuf[50] = {0};

    if(locType2 == LOC_MAX)
        sprintf(szAtBuf, "%s\"%s\"\r", gcstrAtSms[AT_SMS_QCPMS], gcstrLoc[locType1]);
    else
        sprintf(szAtBuf, "%s\"%s\",\"%s\"\r", gcstrAtSms[AT_SMS_QCPMS], gcstrLoc[locType1], gcstrLoc[locType2]);

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE)) {
        m_locTypeTmp1 = locType1;
        m_locTypeTmp2 = locType2;
        SetTimer(IDT_QCPMS_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCPMS, this);
        return TRUE;
    }
    return FALSE;
}

//功能：发送AT$QCMGR
//输入：index: 位置
//输出：TRUE/FALSE

// for new sms tag display error
BOOL CSmsDlg::SndAtSmsRead()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$SmsRead=1\r");
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE)) {
        SetTimer(IDT_QREAD_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsRead, this);

        return TRUE;
    } else
        return FALSE;
}
BOOL CSmsDlg::SndAtOffSmsRead()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$SmsRead=0\r");
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE)) {
        SetTimer(IDT_QREAD_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsRead, this);

        return TRUE;
    } else
        return FALSE;
}
// for new sms tag display error

BOOL CSmsDlg::SndAtSmsQCMGR(USHORT index)
{
    int i=0;
    if(m_locType1 == LOC_ME)
        ASSERT(index <= g_ME_SmsMax);
    else
        ASSERT(index <= g_SM_SmsMax);

    char szAtBuf[50] = {0};
    sprintf(szAtBuf, "%s%d\r", gcstrAtSms[AT_SMS_QCMGR], index);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    while (i<3) {
        if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE)) {

            m_nReadIndex = index;
            SetTimer(IDT_QCMGR_TIMEOUT, 30000, NULL);
            RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCMGR, this);
            break;
        }
        i++;
    }
    if (i<3) return TRUE;
    else     return FALSE;
}

//功能：发送AT$QCMGD
//输入：index: 位置
//输出：TRUE/FALSE
BOOL CSmsDlg::SndAtSmsQCMGD(USHORT index)
{
    USHORT nIndex;
// 	if (UnKnowSms == TRUE)
// 		{
// 			nIndex = index;
// 		}

//	else
    {
        switch(m_locType1) {
        case LOC_ME:
            nIndex = g_ME_SmsRecord[index].index;
            ASSERT(nIndex < g_ME_SmsMax);
            break;
        case LOC_UIM:
            nIndex = g_SM_SmsRecord[index].index;
            ASSERT(nIndex < g_SM_SmsMax);
            break;
        default:
            ASSERT(false);
        }
    }

    char szAtBuf[50] = {0};
    sprintf(szAtBuf, "%s%d\r", gcstrAtSms[AT_SMS_QCMGD], nIndex);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE)) {
        m_nDeleteIndex = index;
        SetTimer(IDT_QCMGD_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCMGD, this);
        return TRUE;
    }
    return FALSE;
}
// for new sms tag display error
void CSmsDlg::RspAtSmsRead(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    return;
}
// for new sms tag display error

//功能：响应AT$QCPMS
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
void CSmsDlg::RspAtSmsQCPMS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
    CSmsDlg* pDlg = (CSmsDlg*)pWnd;
    pDlg->KillTimer(IDT_QCPMS_TIMEOUT);

    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
            || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
    strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0) {
        //设置存储器失败，再次设置
        pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCPMS, (LPARAM)FALSE);
        return;
    }

    ASSERT(wStrNum == 2);

    if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
    || memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCPMS], strlen(gcstrResSms[AT_SMS_QCPMS]))) {
        //设置存储器失败，再次设置
        pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCPMS, (LPARAM)FALSE);
        return;
    }

    pDlg->m_locType1 = pDlg->m_locTypeTmp1;

    if(pDlg->m_locTypeTmp2 != LOC_MAX)
        pDlg->m_locType2 = pDlg->m_locTypeTmp2;

    int cnt = 0;
    char *ptr[4], *p;
    ptr[0] = p = (char *)&strArr[0][strlen(gcstrResSms[AT_SMS_QCPMS])];

    while(*p != '\0' && cnt < 4) {
        if(*p == ',') {
            *p++ = '\0';
            ptr[++cnt] = p;
        } else {
            p++;
        }
    }

    switch(pDlg->m_locType1) {
    case LOC_ME:
        if (pDlg->VoiceMailCountSMSDlg != _T("-1")) {
            g_ME_SmsNum = atoi((const char*)ptr[0]);
            g_ME_SmsMax = atoi((const char*)ptr[1]) -1;
        } else {
            g_ME_SmsNum = atoi((const char*)ptr[0]);
            g_ME_SmsMax = atoi((const char*)ptr[1]);
        }
        break;
    case LOC_UIM:
        g_SM_SmsNum = atoi((const char*)ptr[0]);
        g_SM_SmsMax = atoi((const char*)ptr[1]);
        break;
    }

    if(pDlg->m_locTypeTmp2 != LOC_MAX) {
        switch(pDlg->m_locType2) {
        case LOC_ME:
            if (pDlg->VoiceMailCountSMSDlg != _T("-1")) {
                g_ME_SmsMax = atoi((const char*)ptr[3]) -1;
            } else
                g_ME_SmsMax = atoi((const char*)ptr[3]);
            g_ME_SmsNum = atoi((const char*)ptr[2]);

            break;
        case LOC_UIM:
            g_SM_SmsNum = atoi((const char*)ptr[2]);
            g_SM_SmsMax = atoi((const char*)ptr[3]);
            break;
        }
    }

    HTREEITEM hRoot = pDlg->m_treeSms.GetRootItem();
    pDlg->OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = pDlg->m_treeSms.GetNextSiblingItem(hRoot);
    pDlg->OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = pDlg->m_treeSms.GetNextSiblingItem(hRoot);
    pDlg->OnSmsUpdataNumforSmsTree(hRoot);
    pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCPMS, (LPARAM)TRUE);
}

//功能：响应AT$QCMGR
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
//modified by wk on 2006-6-5 for gsm/wcdma format
void CSmsDlg::RspAtSmsQCMGR(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsDlg* pDlg = (CSmsDlg*)pWnd;
    pDlg->KillTimer(IDT_QCMGR_TIMEOUT);

// 	if(memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR_500][gc_dsatmode],
// 		strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR_500][gc_dsatmode])) == 0)
// 	{
//
// //		pDlg->UnKnowSms = TRUE;
// 		pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCMGD, (LPARAM)FALSE);
//         return;
//
//
// 	}

    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
            || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CME_ERROR][gc_dsatmode],
    strlen(gc_dsatResCodeTbl[DSAT_CME_ERROR][gc_dsatmode])) == 0) {
        pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)FALSE);
        return;
    }

    //empty sms entry
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0) {
        pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)TRUE);
        return;
    }

    ASSERT(wStrNum == 3 || wStrNum == 2);

    if(strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
    || memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCMGR], strlen(gcstrResSms[AT_SMS_QCMGR]))) {
        pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)FALSE);
        return;
    }

    StSmsRecord record;
    memset(&record, 0x00, sizeof(StSmsRecord));
    record.m_NoATRspCDS = TRUE;

    if(SmsAtCMGRRspProc(strArr, wStrNum, record)) {
        switch(pDlg->m_locType1) {
        case LOC_ME:
            g_ME_SmsRecord[g_ME_SmsCnt].index = pDlg->m_nReadIndex;
            g_ME_SmsRecord[g_ME_SmsCnt].record = record;
            g_ME_SmsCnt++;
            break;
        case LOC_UIM:
            g_SM_SmsRecord[g_SM_SmsCnt].index = pDlg->m_nReadIndex;
            g_SM_SmsRecord[g_SM_SmsCnt].record = record;
            g_SM_SmsCnt++;
            break;
        }
        pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)TRUE);//内存 read error
    } else
        pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)FALSE);
}

//功能：响应AT$QCMGD
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
void CSmsDlg::RspAtSmsQCMGD(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    WORD i;
    CSmsDlg* pDlg = (CSmsDlg*)pWnd;
    pDlg->KillTimer(IDT_QCMGD_TIMEOUT);

    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
            || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
    strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0) {
        pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCMGD, (LPARAM)FALSE);
        return;
    }

    switch(pDlg->m_locType1) {
    case LOC_ME:
        UE_DeleteSmsCardRecord(LOC_ME, pDlg->m_nDeleteIndex);
        for(i = g_ME_SmsCnt+1; i < pDlg->m_DelNumArr[LOC_ME]; i++) {
            if(pDlg->m_DelDspIndexArr[LOC_ME][i] >= pDlg->m_nDeleteIndex)
                pDlg->m_DelDspIndexArr[LOC_ME][i]--;
        }
        g_ME_SmsCnt++;
        break;
    case LOC_UIM:
        UE_DeleteSmsCardRecord(LOC_UIM, pDlg->m_nDeleteIndex);
        for(i = g_SM_SmsCnt+1; i < pDlg->m_DelNumArr[LOC_UIM]; i++) {
            if(pDlg->m_DelDspIndexArr[LOC_UIM][i] >= pDlg->m_nDeleteIndex)
                pDlg->m_DelDspIndexArr[LOC_UIM][i]--;
        }
        g_SM_SmsCnt++;
        break;
    }
//    pDlg->UnKnowSms = FALSE;
    pDlg->PostMessage(WM_SMS_OPERATE_PROC, (WPARAM)AT_SMS_QCMGD, (LPARAM)TRUE);
}

//功能：创建进度窗口
//输入：wParam:最大条，lParam:步进
//输出：结果
LRESULT CSmsDlg::OnCreateProgress(WPARAM wParam, LPARAM lParam)
{
    int nUpper = (int)wParam;
    int nStep = (int)lParam;

    if(nUpper == 0 || nStep == 0) {
        CProgressDlg dlg(this);
        pDlg = &dlg;
        dlg.DoModal();
        pDlg = NULL;
    } else {
        CProgressDlg dlg(this, nUpper, nStep);
        pDlg = &dlg;
        dlg.DoModal();
        pDlg = NULL;
    }

    return 0;
}

//功能：响应操作消息
//输入：wParam:表示处理哪个命令的响应，lParam:表示上次命令处理的结果
//输出：结果
LRESULT CSmsDlg::OnSmsOperateProc(WPARAM wParam, LPARAM lParam)
{
    EnAtSmsType AtType = (EnAtSmsType)wParam;
    BOOL bProcRst = (BOOL)lParam;
    BOOL bSndRes  = TRUE;
    static int iReFrst = 0;//read  record 0 again to fix port status error
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

    switch(m_ProcType) {
    case PROC_TYPE_INIT:
        m_ProcType = PROC_TYPE_READ;
        nReadIndex = 0;
        g_ME_SmsCnt = 0;
        bSndRes = SndAtSmsQCPMS(LOC_ME, LOC_UIM);
        if(bSndRes) {
            ::ResetEvent(g_BGPassEvt); //added by wk
            pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
        }
        break;

    case PROC_TYPE_READ:
// for new sms tag display error
        if(InitSMSRecord == 2)
            return 0;
        if(InitSMSRecord == 1) {

            InitSMSRecord = 2;
        }
// for new sms tag display error
        if(bProcRst || !bProcRst) {
// 			if (AtType == AT_SMS_QCMGD && UnKnowSms == TRUE)
// 			{
//
// 				SndAtSmsQCMGD(m_nReadIndex);
//
//
// 			}
            if(AtType == AT_SMS_QCPMS && m_locType1 == LOC_ME)
                ProgressSet(0, g_ME_SmsNum+g_SM_SmsNum, 1);
            nReadIndex++;
            if(m_locType1 == LOC_ME) {


                ProgressPos(g_ME_SmsCnt);
                if(g_ME_SmsCnt < g_ME_SmsNum && nReadIndex <= g_ME_SmsMax) {
                    if ((nReadIndex == pMainWnd->VoicemailIndex) && (nReadIndex != g_ME_SmsMax)) {
                        nReadIndex++;
                        //	g_ME_SmsCnt++;
                    }
                    bSndRes = SndAtSmsQCMGR(nReadIndex);
                } else {
// 					if (pMainWnd->bInitstatus)
// 					{
// 					//	pMainWnd->PostMessage(WM_CREATE_INTERNET);
// 						pMainWnd->OnInternetSetup();
// 						pMainWnd->bInitstatus = FALSE;
// 					}
                    if (iReFrst == 0) {
                        bSndRes = SndAtSmsQCMGR(0);
                        iReFrst = 1;

                    }

                    //				   AfxMessageBox("Read ME Over!");
                    else {
                        if(g_SM_SmsNum == 0) {

                            InitSMSRecord = 1;//for new sms tag display error
                        }

                        nReadIndex = 0;
                        g_SM_SmsCnt = 0;
#ifndef FEATURE_VERSION_NOSIM
                        bSndRes = SndAtSmsQCPMS(LOC_UIM, LOC_ME);
#else
                        ProgressClose();
                        g_bSmsSyncComm = TRUE;
                        ::SetEvent(g_BGPassEvt);
                        if(m_pSmsData->DelallRecordbyFlag(SMS_TYPE_INBOX, SMS_RECORD_FLAG_COPY_UE))
                            UpdateSmsSortList(SMS_TYPE_INBOX, LOCFILTER_PC);
                        InitDspIndexArr(LOCFILTER_ME);
                        //InitDspIndexArr(LOCFILTER_UIM);
                        //InitDspIndexArr(LOCFILTER_ALL);
                        DispSms();
                        pMainWnd->UpdateUnreadSmsIcon();
                        pMainWnd->UpdateFullSmsIcon();
#endif
                    }
                }
            } else {

                ProgressPos(g_ME_SmsCnt+g_SM_SmsCnt);
                if(g_SM_SmsCnt < g_SM_SmsNum && nReadIndex <= g_SM_SmsMax) {
                    bSndRes = SndAtSmsQCMGR(nReadIndex);
                    // for new sms tag display error

                    if((g_SM_SmsCnt+1) == g_SM_SmsNum) {
                        InitSMSRecord = 1;

                    }

                    // for new sms tag display error
                } else {

                    ProgressClose();
                    g_bSmsSyncComm = TRUE;
                    ::SetEvent(g_BGPassEvt);
                    if(m_pSmsData->DelallRecordbyFlag(SMS_TYPE_INBOX, SMS_RECORD_FLAG_COPY_UE))
                        UpdateSmsSortList(SMS_TYPE_INBOX, LOCFILTER_PC);
                    InitDspIndexArr(LOCFILTER_ME);
                    InitDspIndexArr(LOCFILTER_UIM);
                    InitDspIndexArr(LOCFILTER_ALL);
                    DispSms();
                    pMainWnd->UpdateUnreadSmsIcon();
                    pMainWnd->UpdateFullSmsIcon();


                    //                    AfxMessageBox("Read SM Over!"); .
                    if (pMainWnd->bInitstatus) {
                        //初始化后发送AT命令1
                        pMainWnd->AtSndPCONLINE();
                        //	pMainWnd->PostMessage(WM_CREATE_INTERNET);
                        pMainWnd->OnInternetSetup();
                        pMainWnd->bInitstatus = FALSE;
                    }
                }
            }
        } else {
            ::SetEvent(g_BGPassEvt);
            ProgressClose();


            if (pMainWnd->bInitstatus) {

                //初始化后发送AT命令1
                pMainWnd->AtSndPCONLINE();
                //	pMainWnd->PostMessage(WM_CREATE_INTERNET);
                pMainWnd->OnInternetSetup();
                pMainWnd->bInitstatus = FALSE;
            }
        }
        break;

    case PROC_TYPE_DELETE:
        if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
            bProcRst = FALSE;

        if(bProcRst) {
            if(AtType == AT_SMS_QCPMS
                    && (m_locType1 == LOC_ME || m_DelNumArr[LOC_ME] == 0))
                ProgressSet(0, m_DelNumArr[LOC_ME] + m_DelNumArr[LOC_UIM], 1);

            if(m_locType1 == LOC_ME) {
                ProgressPos(g_ME_SmsCnt);
                if(g_ME_SmsCnt < m_DelNumArr[LOC_ME]) {
                    bSndRes = SndAtSmsQCMGD(m_DelDspIndexArr[LOC_ME][g_ME_SmsCnt]);
                } else {
                    //                AfxMessageBox("Delete ME Over!");
                    if(m_DelNumArr[LOC_UIM] > 0) {
                        g_SM_SmsCnt = 0;
                        bSndRes = SndAtSmsQCPMS(LOC_UIM, LOC_ME);
                    } else
                        bSndRes = FALSE; //删除结束
                }
            } else {
                ProgressPos(m_DelNumArr[LOC_ME]+g_SM_SmsCnt);
                if(g_SM_SmsCnt < m_DelNumArr[LOC_UIM]) {
                    bSndRes = SndAtSmsQCMGD(m_DelDspIndexArr[LOC_UIM][g_SM_SmsCnt]);
                } else {
                    ProgressClose();
                    ::SetEvent(g_BGPassEvt);
                    UpdateListAfterDelete();
//                    AfxMessageBox("Delete SM Over!");
                }
            }
        } else {
            ::SetEvent(g_BGPassEvt);
            ProgressClose();
            UpdateListAfterDelete();
        }
        break;
    }

    if(!bSndRes) {
        ::SetEvent(g_BGPassEvt);
        ProgressClose();
        UpdateListAfterDelete();
    }

    if((InitSMSRecord == 2)&&(SMS_FULL_Prompt  == TRUE)) {
#if 0
        if((m_pSmsData->m_SmsNum[SMS_TYPE_INBOX]== SMS_RECORD_MAX)&&(g_ME_SmsNum==g_ME_SmsMax)&&(g_SM_SmsNum==g_SM_SmsMax))
#else
        if(m_pSmsData->m_SmsNum[SMS_TYPE_INBOX]== SMS_RECORD_MAX)
#endif
        {

            AfxMessageBox(IDS_PC_SMS_FULL, MB_OK | MB_ICONINFORMATION);
            SMS_FULL_Prompt  = FALSE;
        }
    }

    if (m_ProcType != PROC_TYPE_READ) {
        HTREEITEM hRoot = m_treeSms.GetRootItem();
        OnSmsUpdataNumforSmsTree(hRoot);
        hRoot = m_treeSms.GetNextSiblingItem(hRoot);
        OnSmsUpdataNumforSmsTree(hRoot);
        hRoot = m_treeSms.GetNextSiblingItem(hRoot);
        OnSmsUpdataNumforSmsTree(hRoot);
#ifndef FEATURE_VERSION_NOSIM
        hRoot = m_treeSms.GetNextSiblingItem(hRoot);
        OnSmsUpdataNumforSmsTree(hRoot);
#endif

    }
    return 0;
}

//功能：定时器
//输入：nIDEvent: 定时器编号
//输出：无
void CSmsDlg::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    switch(nIDEvent) {
    case IDT_QCPMS_TIMEOUT:
        KillTimer(IDT_QCPMS_TIMEOUT);
        break;
    case IDT_QCMGR_TIMEOUT:
        KillTimer(IDT_QCMGR_TIMEOUT);
        break;
    case IDT_QCMGD_TIMEOUT:
        KillTimer(IDT_QCMGD_TIMEOUT);
        break;
// for new sms tag display error
    case IDT_QREAD_TIMEOUT:
        KillTimer(IDT_QREAD_TIMEOUT);
        break;
// for new sms tag display error

    }

    ::SetEvent(g_BGPassEvt);
    ProgressClose();
    UpdateListAfterDelete();

    DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
    ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);

    AfxMessageBox(IDS_USB_PORT_TIMEOUT);
    CDialog::OnTimer(nIDEvent);
}

//功能：获得SMS总数
//输入：type:类型，locFilter:位置
//输出：SMS数目
WORD CSmsDlg::GetTotalSmsNum(EnSmsType type, EnLocFilter locFilter)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(locFilter >= LOCFILTER_PC && locFilter <= LOCFILTER_ALL);

    WORD nPCNum = 0;
    WORD nMENum = 0;
    WORD nSMNum = 0;

    if(locFilter == LOCFILTER_PC || locFilter == LOCFILTER_ALL)
        nPCNum = m_pSmsData->GetSmsNum(type);

    if(g_bSmsSyncComm && type != SMS_TYPE_DRAFT && locFilter != LOCFILTER_PC) {
        WORD cnt;
        EnSmsState state;
        EnSmsState state1;
        EnSmsState state2;
        StSmsCardRecord CardRecord;

        if(type == SMS_TYPE_INBOX) {
            state1 = SMS_STATE_MT_NOT_READ;
            state2 = SMS_STATE_MT_READ;
        } else if(type == SMS_TYPE_OUTBOX) {
            state1 = SMS_STATE_MO_NOT_SENT;
            state2 = SMS_STATE_MO_SENT;
        }

        if(locFilter != LOCFILTER_UIM) {
            for(cnt=0; cnt<g_ME_SmsNum; cnt++) {
                CardRecord = UE_ReadSmsCardRecord(LOC_ME, cnt);
                state = CardRecord.record.state;
                if(state == state1 || state == state2)
                    nMENum++;
            }
        }

        if(locFilter != LOCFILTER_ME) {
            for(cnt=0; cnt<g_SM_SmsNum; cnt++) {
                CardRecord = UE_ReadSmsCardRecord(LOC_UIM, cnt);
                state = CardRecord.record.state;
                if(state == state1 || state == state2)
                    nSMNum++;
            }
        }
    }

    if(locFilter == LOCFILTER_PC)
        return nPCNum;
    else if(locFilter == LOCFILTER_ME)
        return nMENum;
    else if(locFilter == LOCFILTER_UIM)
        return nSMNum;
    else
        return (nPCNum+nMENum+nSMNum);
}

//功能：从显示索引得到SMS记录
//输入：type:类型，locFilter:位置，dspIndex:索引
//输出：SMS记录
StSmsRecord CSmsDlg::GetTotalSmsRecordFromDspIndex(EnSmsType type, EnLocFilter locFilter, WORD dspIndex)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(locFilter >= LOCFILTER_PC && locFilter <= LOCFILTER_ALL);
    ASSERT(dspIndex < GetTotalSmsNum(type, locFilter));

    StDspEntityType SmsDspEntity = m_DspEntityArr[type][locFilter][dspIndex];

    ASSERT(SmsDspEntity.LocType >= LOC_PC && SmsDspEntity.LocType < LOC_MAX);

    if(SmsDspEntity.LocType == LOC_PC)
        return m_pSmsData->ReadSmsRecord(type, SmsDspEntity.nIndex);
    else {
        StSmsCardRecord CardRecord = UE_ReadSmsCardRecord(SmsDspEntity.LocType, SmsDspEntity.nIndex);
        return CardRecord.record;
    }
}

//功能：增删时更新排序表
//输入：type:类型，loc:位置
//输出：无
void CSmsDlg::UpdateSortListWhenAddDelete(EnSmsType type, EnLocType loc)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(loc >= LOC_PC && loc < LOC_MAX);

    // CHSDPADlg *pDlg = (CHSDPADlg*)AfxGetMainWnd();
    CHSDPADlg* pDlg = (CHSDPADlg*)(theApp.GetMainWnd());

    if(type == SMS_TYPE_INBOX) {
        pDlg->UpdateUnreadSmsIcon();
        pDlg->UpdateFullSmsIcon();
    }
    if(loc == LOC_UIM) {
        pDlg->UpdateFullSmsIcon();
        UpdateSmsSortList(SMS_TYPE_INBOX, (EnLocFilter)loc);
    }

    if(loc == LOC_PC) {
        pDlg->UpdateFullSmsIcon();
        UpdateSmsSortList(type, LOCFILTER_PC);
        //UpdateSmsSortList(type, LOCFILTER_ALL);
    } else {
        UpdateSmsSortList(SMS_TYPE_INBOX, (EnLocFilter)loc);
        //  UpdateSmsSortList(SMS_TYPE_OUTBOX, (EnLocFilter)loc);
        //  UpdateSmsSortList(SMS_TYPE_INBOX, LOCFILTER_ALL);
        // UpdateSmsSortList(SMS_TYPE_OUTBOX, LOCFILTER_ALL);
    }

    DispSms();
}

//功能：删除后更新列表
//输入：无
//输出：无
void CSmsDlg::UpdateListAfterDelete()
{
    if(m_DelNumArr[LOC_PC] > 0)
        UpdateSortListWhenAddDelete(m_DispFilter, LOC_PC);
    if(m_DelNumArr[LOC_ME] > 0)
        UpdateSortListWhenAddDelete(m_DispFilter, LOC_ME);
    if(m_DelNumArr[LOC_UIM] > 0)
        UpdateSortListWhenAddDelete(m_DispFilter, LOC_UIM);

    memset(m_DelDspIndexArr, 0x00, sizeof(m_DelDspIndexArr));
    memset(m_DelNumArr, 0, sizeof(m_DelNumArr));
}


//功能：发起呼叫
//输入：无
//输出：TRUE/FALSE
BOOL CSmsDlg::SetupCall()
{
    // TODO: Add your control notification handler code here
    int nCurSel;

    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return FALSE;
    else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {

        return FALSE;
    }

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);

    if(pComm->CommIsReady()) {
        StSmsRecord record = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, nCurSel);

        WPARAM wParam = 1;
        LPARAM lParam = (LPARAM)(LPCTSTR)record.szNumber;

        if(wcslen((LPCTSTR)lParam) > 0)
            ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam, lParam);
        else
            return FALSE;
        //AfxMessageBox(IDS_SELCONNECT_NULLNUM);
    }
    return TRUE;
}

//liub_add_begin
//初始化树形结构SMS
BOOL CSmsDlg::InitSMSTree()
{
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    m_treeSms.DeleteAllItems();
    //节点的图标
    int i = 0;
    int i_count = 6;
    //载入图标
    HICON icon[6];
    icon[0] = AfxGetApp()->LoadIcon(IDI_ICON_SMS_FILE);
    icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_SMS_INBOX);
    icon[2] = AfxGetApp()->LoadIcon(IDI_ICON_SMS_OUTBOX);
    icon[3] = AfxGetApp()->LoadIcon(IDI_ICON_SMS_DRAFT);
    icon[4] = AfxGetApp()->LoadIcon(IDI_ICON_SMS_ARCHIVE);
    icon[5] = AfxGetApp()->LoadIcon(IDI_ICON_VOICEMAIL);//add by liub for voicemail


    //创建图像列表
    m_imagelistSMSTree = new CImageList;
    m_imagelistSMSTree->Create(16,16,0,7,7);
    m_imagelistSMSTree->SetBkColor(RGB(231,238,248));
    for(int j = 0; j<i_count; j++) {
        m_imagelistSMSTree->Add(icon[j]);//图标载入图像控件
    }
    m_treeSms.SetImageList(m_imagelistSMSTree,TVSIL_NORMAL);//为treesms设置一个图像列表，以便使CtreeCtrl的节点显示不同的图标

    //m_treeSms.SetBkColor(RGB(231,238,248));
    m_treeSms.SetBkColor(RGB(239,242,249));

    //创建节点
    //根节点（Local就是PC上的）
    CString treenodtext;
    treenodtext.LoadString(IDS_SMSTREE_LOCAL);
    HTREEITEM rootLocal = m_treeSms.InsertItem(treenodtext,0,1,TVI_ROOT,TVI_LAST);
    if (g_bIsExist) {

        treenodtext.LoadString(IDS_SMSTREE_USBMODEM);
        HTREEITEM rootUSBModem = m_treeSms.InsertItem(treenodtext,0,1,TVI_ROOT,TVI_LAST);

#ifdef FEATURE_VERSION_NOSIM
#else
        if (pMainWnd->m_bSimReady) { //未插入SIM卡
            treenodtext.LoadString(IDS_SMSTREE_USIM);
            HTREEITEM rootUSIM = m_treeSms.InsertItem(treenodtext,0,1,TVI_ROOT,TVI_LAST);
            m_treeSms.SetItemImage(rootUSIM,0,0);
        }
#endif
        treenodtext.LoadString(IDS_STR_VOICEMAIL);//add by liub for voicemail
        HTREEITEM rootVoiceMail = m_treeSms.InsertItem(treenodtext,0,1,TVI_ROOT,TVI_LAST);

        m_treeSms.SetItemImage(rootVoiceMail,0,0);
        m_treeSms.SetItemImage(rootUSBModem,0,0);

        //VoiceMail子节点
        treenodtext.LoadString(IDS_STR_VOICELISTEN);
        HTREEITEM rootVoicemail_Inbox = m_treeSms.InsertItem(treenodtext,0,1,rootVoiceMail,TVI_LAST);
        treenodtext.LoadString(IDS_STR_LASTVOICEMAIL);
        HTREEITEM rootVoicemail_listen_lastone = m_treeSms.InsertItem(treenodtext,0,1,rootVoiceMail,TVI_LAST);
        treenodtext.LoadString(IDS_STR_CLEAR_VMAILICON);
        HTREEITEM rootVoicemail_clearicon = m_treeSms.InsertItem(treenodtext,0,1,rootVoiceMail,TVI_LAST);

        m_treeSms.SetItemImage(rootVoicemail_Inbox,5,5);

    }


    //LOCAL子节点
    treenodtext.LoadString(IDS_SMSTREE_INBOX);
    HTREEITEM rootLocal_Inbox = m_treeSms.InsertItem(treenodtext,0,1,rootLocal,TVI_LAST);
    treenodtext.LoadString(IDS_SMSTREE_OUTBOX);
    HTREEITEM rootLocal_Outbox = m_treeSms.InsertItem(treenodtext,0,1,rootLocal,TVI_LAST);
    treenodtext.LoadString(IDS_SMSTREE_DRAFT);
    HTREEITEM rootLocal_Draft = m_treeSms.InsertItem(treenodtext,0,1,rootLocal,TVI_LAST);
    treenodtext.LoadString(IDS_SMSTREE_ARCHIVE);
    HTREEITEM rootLocal_Archive = m_treeSms.InsertItem(treenodtext,0,1,rootLocal,TVI_LAST);

    //USIM子节点
    //HTREEITEM rootUSIM_Inbox = m_treeSms.InsertItem(_T("Inbox"),0,1,rootUSIM,TVI_LAST);

    m_treeSms.SetItemImage(rootLocal,0,0);
    m_treeSms.SetItemImage(rootLocal_Inbox,1,1);
    m_treeSms.SetItemImage(rootLocal_Outbox,2,2);
    m_treeSms.SetItemImage(rootLocal_Draft,3,3);
    m_treeSms.SetItemImage(rootLocal_Archive,4,4);

    HTREEITEM hRoot = m_treeSms.GetRootItem();
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);

    return true;
}


//添加面板
void CSmsDlg::OnSelchangedSMSTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    CHSDPADlg *pDlg = (CHSDPADlg*)AfxGetMainWnd();
    //HTREEITEM GetParentItem( HTREEITEM hItem );得到父结点
    HTREEITEM hItree;
    hItree = m_treeSms.GetParentItem(pNMTreeView->itemNew.hItem);
    CString m_tree_ParentItem_name = "";

    CString sms_local;
    CString sms_usbmodem;
    CString sms_RUIM;
    CString sms_inbox;
    CString sms_outbox;
    CString sms_draft;
    CString sms_archive;
    CString sms_voicemail;
    CString sms_listen_lastone;
    CString sms_clearvoicemailicon;


    sms_local.LoadString(IDS_SMSTREE_LOCAL);
    sms_usbmodem.LoadString(IDS_SMSTREE_USBMODEM);
    sms_RUIM.LoadString(IDS_SMSTREE_USIM);
    sms_inbox.LoadString(IDS_SMSTREE_INBOX);
    sms_outbox.LoadString(IDS_SMSTREE_OUTBOX);
    sms_draft.LoadString(IDS_SMSTREE_DRAFT);
    sms_archive.LoadString(IDS_SMSTREE_ARCHIVE);
    sms_voicemail.LoadString(IDS_STR_VOICELISTEN);
    sms_listen_lastone.LoadString(IDS_STR_LASTVOICEMAIL);
    sms_clearvoicemailicon.LoadString(IDS_STR_CLEAR_VMAILICON);

    UpdateData(true);
    smstree_node_name = m_treeSms.GetItemText(pNMTreeView->itemNew.hItem);
    m_tree_ParentItem_name = m_treeSms.GetItemText(hItree);
    // CString rootNodeText;
    if(wcsstr(smstree_node_name,sms_local)) {
        m_cmbLocFilter.SetCurSel(LOC_PC);
        OnSelchangeCombolocfilter();

        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_FORWARD)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_DIAL)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_DELETE)->ShowWindow(SW_NORMAL);
        m_lstSms.SetLocType(LOC_PC);
        //rootNodeText = smstree_node_name;
        ResetDspIndexArr(m_DispFilter,m_LocFilter);
    }
    if(wcsstr(smstree_node_name,sms_RUIM)) {
        m_cmbLocFilter.SetCurSel(LOC_UIM);
        OnSelchangeCombolocfilter();
        m_DispFilter = SMS_TYPE_INBOX;

        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_FORWARD)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_DIAL)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_DELETE)->ShowWindow(SW_NORMAL);

        m_lstSms.SetDspFilter(m_DispFilter);
        m_lstSms.SetLocType(LOC_UIM);
        ResetDspIndexArr(m_DispFilter,m_LocFilter);
        DispSms();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }
    if (wcsstr(smstree_node_name,sms_usbmodem)) {
        m_cmbLocFilter.SetCurSel(LOC_ME);
        OnSelchangeCombolocfilter();
        m_DispFilter = SMS_TYPE_INBOX;
        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_FORWARD)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_DIAL)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_DELETE)->ShowWindow(SW_NORMAL);

        m_lstSms.SetDspFilter(m_DispFilter);
        m_lstSms.SetLocType(LOC_ME);
        ResetDspIndexArr(m_DispFilter,m_LocFilter);
        DispSms();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }
    //列出UIM卡节点的SMS列表
// 	if (smstree_node_name == _T("Inbox") && m_tree_ParentItem_name == _T("USIM"))//根据当前节点的父节点名称来判断是那个位置的收件箱(Local或USIM)
// 	{
// 		m_cmbLocFilter.SetCurSel(LOC_UIM);
// 		OnSelchangeCombolocfilter();
// 		m_DispFilter = SMS_TYPE_INBOX;
// 		GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
//         GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);
// 		m_lstSms.SetDspFilter(m_DispFilter);
// 		DispSms();
// 		GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
// 	}

    //列出PC节点里的SMS列表
    if (wcsstr(smstree_node_name,sms_inbox)) { //根据当前节点的父节点名称来判断是哪个位置的收件箱(Local或USIM)
        m_cmbLocFilter.SetCurSel(LOC_PC);
        OnSelchangeCombolocfilter();
        m_DispFilter = SMS_TYPE_INBOX;

        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_FORWARD)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_DIAL)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_DELETE)->ShowWindow(SW_NORMAL);

        m_lstSms.SetDspFilter(m_DispFilter);
        m_lstSms.SetLocType(LOC_PC);
        ResetDspIndexArr(m_DispFilter,m_LocFilter);
        DispSms();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }
    if (wcsstr(smstree_node_name,sms_outbox)) {
        m_cmbLocFilter.SetCurSel(LOC_PC);
        OnSelchangeCombolocfilter();
        m_DispFilter = SMS_TYPE_OUTBOX;

        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_NORMAL);

        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_FORWARD)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_DIAL)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_DELETE)->ShowWindow(SW_NORMAL);

        m_lstSms.SetDspFilter(m_DispFilter);
        m_lstSms.SetLocType(LOC_PC);
        ResetDspIndexArr(m_DispFilter,m_LocFilter);
        DispSms();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }
    if (wcsstr(smstree_node_name,sms_draft)) {
        m_cmbLocFilter.SetCurSel(LOC_PC);
        OnSelchangeCombolocfilter();
        m_DispFilter = SMS_TYPE_DRAFT;

        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_NORMAL);

        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_DELETE)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_FORWARD)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_DIAL)->ShowWindow(SW_HIDE);

        m_lstSms.SetDspFilter(m_DispFilter);
        m_lstSms.SetLocType(LOC_PC);
        ResetDspIndexArr(m_DispFilter,m_LocFilter);
        DispSms();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }
    if (wcsstr(smstree_node_name,sms_archive)) {
        m_cmbLocFilter.SetCurSel(LOC_PC);
        OnSelchangeCombolocfilter();
        m_DispFilter = SMS_TYPE_ARCHIVE;
        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_NORMAL);

        GetDlgItem(IDC_BUTTON_SMS_DELETE)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_FORWARD)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_DIAL)->ShowWindow(SW_HIDE);

        m_lstSms.SetDspFilter(m_DispFilter);
        m_lstSms.SetLocType(LOC_PC);
        ResetDspIndexArr(m_DispFilter,m_LocFilter);
        DispSms();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));

    }
    if (wcsstr(smstree_node_name,sms_voicemail)) {
        if (voicesmscount == _T("0")) {
            AfxMessageBox(_T("No Voice mail!"));
        } else {
            CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
            ASSERT(pComm);

            if(pComm->CommIsReady()) {


                WPARAM wParam;
                LPARAM lParam;

                wParam = 1;


                lParam = (LPARAM)(LPCTSTR)g_SetData.Messages_szVoiceMailNumber;

                if(wcslen((LPCTSTR)lParam) > 0)
                    ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam, lParam);
                else
                    AfxMessageBox(IDS_SELCONNECT_NULLNUM);
            }
        }
        HTREEITEM hRoot = m_treeSms.GetRootItem();
        hRoot = m_treeSms.GetNextSiblingItem(hRoot);
        hRoot = m_treeSms.GetNextSiblingItem(hRoot);
#ifndef FEATURE_VERSION_NOSIM
        hRoot = m_treeSms.GetNextSiblingItem(hRoot);
#endif
        m_treeSms.Expand(hRoot,TVE_COLLAPSE);
    }
    if (wcsstr(smstree_node_name,sms_listen_lastone)) {
// 		if (voicesmscount == _T("0"))
// 		{
// 			AfxMessageBox(_T("No Voice mail!"));
// 		}
// 		else
        pDlg->VoiceMail = TRUE;
        SndAtCVMR();
        HTREEITEM hRoot = m_treeSms.GetRootItem();
        hRoot = m_treeSms.GetNextSiblingItem(hRoot);
        hRoot = m_treeSms.GetNextSiblingItem(hRoot);
#ifndef FEATURE_VERSION_NOSIM
        hRoot = m_treeSms.GetNextSiblingItem(hRoot);
#endif
        m_treeSms.Expand(hRoot,TVE_COLLAPSE);
    }
    if (wcsstr(smstree_node_name,sms_clearvoicemailicon)) {
// 		if (voicesmscount == _T("0"))
// 		{
// 			AfxMessageBox(_T("No Voice mail"));
// 		}
// 		else
        SndAtCVMD();

    }

    m_lstSms.SetTextBkColor(RGB(255,255,255));
    UpdateData(false);
    *pResult = 0;

}
//liub_add_end
void CSmsDlg::OnChangeEditSmsSearch()
{
    UpdateData(TRUE);
    m_lstSms.SetTextBkColor(RGB(255,255,255));
    m_lstSms.SetDspFilter(m_DispFilter);
    ResetDspIndexArr(m_DispFilter,m_LocFilter);
    DispSms();
    UpdateData(false);
}

void CSmsDlg::OnButtonSmsSearch()
{
    UpdateData(TRUE);
    GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    CString Messbox;
    EnLocType loctype=LOC_MAX;
    memset(m_DelDspIndexArr, 0x00, sizeof(m_DelDspIndexArr));
    memset(m_DelNumArr, 0, sizeof(m_DelNumArr));
    Messbox.LoadString(IDS_SMS_SEARCHCONDITION);
    if(m_strSearch==_T("")) {
        AfxMessageBox(Messbox);
        return;
    }
    int nItemCount;
    nItemCount=m_lstSms.GetItemCount();
    int* delRecords=new int[nItemCount];
    int nIndex=0;
    for(int i=0; i<nItemCount; i++) {
        CString strName;
        CString strContent;
        strName=m_lstSms.GetItemText(i,SMS_LISTCTRL_HEADERINDEX_NAME);
        strContent=m_lstSms.GetItemText(i,SMS_LISTCTRL_HEADERINDEX_CONTENT);
        if((strName.Find(m_strSearch)==-1)&&(strContent.Find(m_strSearch)==-1)) {
            delRecords[nIndex]=i;
            nIndex++;
        } else {
            loctype = m_DspEntityArr[m_DispFilter][m_LocFilter][i].LocType;//
            m_DelDspIndexArr[loctype][m_DelNumArr[loctype]++] = m_DspEntityArr[m_DispFilter][m_LocFilter][i].nIndex;//
        }

    }

    memset(m_DspEntityArr, 0x00, sizeof(m_DspEntityArr));
    ResetDspIndexArr(m_DispFilter,m_LocFilter);
    if (loctype != LOC_MAX) {
        for (int x =0; x < m_DelNumArr[loctype]; x++) {
            m_DspEntityArr[m_DispFilter][m_LocFilter][x].nIndex = m_DelDspIndexArr[loctype][x];

        }
    }
    for(int j=nIndex-1; j>=0; j--) {
        m_lstSms.DeleteItem(delRecords[j]);
    }
    delete[] delRecords;
    m_lstSms.SetTextBkColor(RGB(230,230,230));
    m_lstSms.RedrawWindow();

}

//////////////////////////////////////////////////////////////////////////add by liub begin
//功能：从索引编辑SMS记录
//输入：type:类型，locFilter:位置，dspIndex:索引，record:记录
//输出：无
void CSmsDlg::EditTotalSmsRecordFromDspIndex(EnSmsType type, EnLocFilter locFilter, WORD dspIndex, const StSmsRecord &record)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(locFilter >= LOCFILTER_PC && locFilter <= LOCFILTER_ALL);
    ASSERT(dspIndex < GetTotalSmsNum(type, locFilter));

    StDspEntityType SmsDspEntity = m_DspEntityArr[type][locFilter][dspIndex];

    if(SmsDspEntity.LocType == LOC_PC)
        m_pSmsData->EditSmsRecord(type, SmsDspEntity.nIndex, record);
    else
        UE_EditSmsCardRecord(SmsDspEntity.LocType, SmsDspEntity.nIndex, record);
}
//////////////////////////////////////////////////////////////////////////add by liub end

void CSmsDlg::DeleteCDSSMS()
{
    int nCurSel;
    nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED);

    CString str;
    str.LoadString(IDS_DELETECONFIRM);

    WORD i;
    WORD cnt;
    WORD nIndex;
    EnLocType loctype;

    memset(m_DelDspIndexArr, 0x00, sizeof(m_DelDspIndexArr));
    memset(m_DelNumArr, 0, sizeof(m_DelNumArr));

    do {
        loctype = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].LocType;
        m_DelDspIndexArr[loctype][m_DelNumArr[loctype]++] = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].nIndex;
    } while((nCurSel = m_lstSms.GetNextItem(nCurSel, LVNI_SELECTED)) != -1);

    for(cnt = 0; cnt < m_DelNumArr[LOC_PC]; cnt++) {
        nIndex = m_DelDspIndexArr[LOC_PC][cnt];
        m_pSmsData->DeleteSmsRecord(m_DispFilter, nIndex);
        for(i = cnt+1; i < m_DelNumArr[LOC_PC]; i++) {
            if(m_DelDspIndexArr[LOC_PC][i] >= nIndex)
                m_DelDspIndexArr[LOC_PC][i]--;
        }
    }


    BOOL bSndRes = FALSE;
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

    if(m_DelNumArr[LOC_ME] + m_DelNumArr[LOC_UIM] > 0) {
        m_ProcType = PROC_TYPE_DELETE;
        m_str_sms_Prompt.LoadString(IDS_WAIT_DELETE_SMS);
        ProgressOpen();

        g_ME_SmsCnt = 0;
        g_SM_SmsCnt = 0;

        if(m_DelNumArr[LOC_ME] > 0) {
            bSndRes = SndAtSmsQCPMS(LOC_ME, LOC_UIM);
        } else {
            bSndRes = SndAtSmsQCPMS(LOC_UIM, LOC_ME);
        }

        if(bSndRes) {
            ::ResetEvent(g_BGPassEvt);
            pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
        }
    }

    if(m_DelNumArr[LOC_PC] > 0 && !bSndRes) {
        UpdateListAfterDelete();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }

}

void CSmsDlg::OnButtonSmsArchive()
{
    ResetDspIndexArr(m_DispFilter,m_LocFilter);
    int nCurSel;

    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;
    else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    WORD i;
    WORD cnt;
    WORD nIndex;
    EnLocType loctype;

    memset(m_DelDspIndexArr, 0x00, sizeof(m_DelDspIndexArr));
    memset(m_DelNumArr, 0, sizeof(m_DelNumArr));

    do {
        loctype = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].LocType;
        m_DelDspIndexArr[loctype][m_DelNumArr[loctype]++] = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].nIndex;
    } while((nCurSel = m_lstSms.GetNextItem(nCurSel, LVNI_SELECTED)) != -1);

    for(cnt = 0; cnt < m_DelNumArr[LOC_PC]; cnt++) {
        nIndex = m_DelDspIndexArr[LOC_PC][cnt];

        StSmsRecord record = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, nIndex);
        m_pSmsData->AddSmsRecord(SMS_TYPE_ARCHIVE,record);

        m_pSmsData->DeleteSmsRecord(m_DispFilter, nIndex);
        for(i = cnt+1; i < m_DelNumArr[LOC_PC]; i++) {
            if(m_DelDspIndexArr[LOC_PC][i] >= nIndex)
                m_DelDspIndexArr[LOC_PC][i]--;
        }
    }

    BOOL bSndRes = FALSE;
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

    if(m_DelNumArr[LOC_ME] + m_DelNumArr[LOC_UIM] > 0) {
        m_ProcType = PROC_TYPE_DELETE;
        m_str_sms_Prompt.LoadString(IDS_WAIT_DELETE_SMS);
        ProgressOpen();

        g_ME_SmsCnt = 0;
        g_SM_SmsCnt = 0;

        if(m_DelNumArr[LOC_ME] > 0) {
            bSndRes = SndAtSmsQCPMS(LOC_ME, LOC_UIM);
        } else {
            bSndRes = SndAtSmsQCPMS(LOC_UIM, LOC_ME);
        }

        if(bSndRes) {
            ::ResetEvent(g_BGPassEvt);
            pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
        }
    }

    if(m_DelNumArr[LOC_PC] > 0 && !bSndRes) {
        UpdateListAfterDelete();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }



    if(i > 0) {
        SendMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_ARCHIVE, LOC_PC);
    }
    if(i == m_DelNumArr[LOC_PC])
        AfxMessageBox(IDS_SMS_ARCHIVEFAIL, MB_OK | MB_ICONINFORMATION);
    else
        AfxMessageBox(IDS_SMS_ARCHIVESUCC, MB_OK | MB_ICONINFORMATION);

    HTREEITEM hRoot = m_treeSms.GetRootItem();
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);

}

void CSmsDlg::OnButtonSmsResume()
{
    ResetDspIndexArr(m_DispFilter,m_LocFilter);
    int nCurSel;

    if(GetTotalSmsNum(m_DispFilter, m_LocFilter) == 0)
        return;
    else if((nCurSel = m_lstSms.GetNextItem(-1, LVNI_SELECTED)) == -1) {
        AfxMessageBox(IDS_NOSMSITEMSEL, MB_OK);
        return;
    }

    WORD i;
    WORD cnt;
    WORD nIndex;
    EnLocType loctype;

    memset(m_DelDspIndexArr, 0x00, sizeof(m_DelDspIndexArr));
    memset(m_DelNumArr, 0, sizeof(m_DelNumArr));

    do {
        loctype = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].LocType;
        m_DelDspIndexArr[loctype][m_DelNumArr[loctype]++] = m_DspEntityArr[m_DispFilter][m_LocFilter][nCurSel].nIndex;
    } while((nCurSel = m_lstSms.GetNextItem(nCurSel, LVNI_SELECTED)) != -1);

    for(cnt = 0; cnt < m_DelNumArr[LOC_PC]; cnt++) {
        nIndex = m_DelDspIndexArr[LOC_PC][cnt];

        StSmsRecord record = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, nIndex);
        m_pSmsData->AddSmsRecord(SMS_TYPE_INBOX,record);

        m_pSmsData->DeleteSmsRecord(m_DispFilter, nIndex);
        for(i = cnt+1; i < m_DelNumArr[LOC_PC]; i++) {
            if(m_DelDspIndexArr[LOC_PC][i] >= nIndex)
                m_DelDspIndexArr[LOC_PC][i]--;
        }
    }

    BOOL bSndRes = FALSE;
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

    if(m_DelNumArr[LOC_ME] + m_DelNumArr[LOC_UIM] > 0) {
        m_ProcType = PROC_TYPE_DELETE;
        m_str_sms_Prompt.LoadString(IDS_WAIT_DELETE_SMS);
        ProgressOpen();

        g_ME_SmsCnt = 0;
        g_SM_SmsCnt = 0;

        if(m_DelNumArr[LOC_ME] > 0) {
            bSndRes = SndAtSmsQCPMS(LOC_ME, LOC_UIM);
        } else {
            bSndRes = SndAtSmsQCPMS(LOC_UIM, LOC_ME);
        }

        if(bSndRes) {
            ::ResetEvent(g_BGPassEvt);
            pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
        }
    }

    if(m_DelNumArr[LOC_PC] > 0 && !bSndRes) {
        UpdateListAfterDelete();
        GetDlgItem(IDC_EDIT_SMSCONTENT)->SetWindowText(_T(""));
    }

    if(i > 0) {
        SendMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_INBOX, LOC_PC);
    }
    if(i == m_DelNumArr[LOC_PC])
        AfxMessageBox(IDS_SMS_RESUMEFAIL, MB_OK | MB_ICONINFORMATION);
    else
        AfxMessageBox(IDS_SMS_RESUMESUCC, MB_OK | MB_ICONINFORMATION);

    HTREEITEM hRoot = m_treeSms.GetRootItem();
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
    hRoot = m_treeSms.GetNextSiblingItem(hRoot);
    OnSmsUpdataNumforSmsTree(hRoot);
}

// 根据lpszText查找所给定根的所有项
HTREEITEM CSmsDlg::OnSmsUpdataNumforSmsTree(HTREEITEM hRoot)
{
    StSmsRecord SmsRecord;
    CString sms_node_text;
    DWORD nSmsNumLocal;
    TCHAR str[10];
    int nSmsNotReadNum = 0;
    TCHAR strnoreadnum[10];
    //如果hRoot = NULL，返回NULL
    if (hRoot == NULL)
        return NULL;

    CString sms_local;
    CString sms_usbmodem;
    CString sms_RUIM;
    CString sms_inbox;
    CString sms_outbox;
    CString sms_draft;
    CString sms_archive;
    CString sms_voicemail;

    sms_local.LoadString(IDS_SMSTREE_LOCAL);
    sms_usbmodem.LoadString(IDS_SMSTREE_USBMODEM);
    sms_RUIM.LoadString(IDS_SMSTREE_USIM);
    sms_inbox.LoadString(IDS_SMSTREE_INBOX);
    sms_outbox.LoadString(IDS_SMSTREE_OUTBOX);
    sms_draft.LoadString(IDS_SMSTREE_DRAFT);
    sms_archive.LoadString(IDS_SMSTREE_ARCHIVE);
    sms_voicemail.LoadString(IDS_STR_VOICELISTEN);
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

    //判断hRoot是否符合条件，如果符合，返回hRoot
    sms_node_text = m_treeSms.GetItemText(hRoot);
    if (wcsstr(sms_node_text,sms_local)) {
        sms_node_text = sms_local;
        int nSmsNumInbox = 0;
        nSmsNumLocal = GetTotalSmsNum(SMS_TYPE_INBOX, LOCFILTER_PC);
        nSmsNumInbox = nSmsNumLocal;
        nSmsNumLocal += GetTotalSmsNum(SMS_TYPE_OUTBOX, LOCFILTER_PC);
        nSmsNumLocal += GetTotalSmsNum(SMS_TYPE_DRAFT, LOCFILTER_PC);
        nSmsNumLocal += GetTotalSmsNum(SMS_TYPE_ARCHIVE, LOCFILTER_PC);

        for (int i =0; i<nSmsNumInbox; i++) {
            SmsRecord = GetTotalSmsRecordFromDspIndex(SMS_TYPE_INBOX, LOCFILTER_PC, i);
            if(SmsRecord.state == SMS_STATE_MT_NOT_READ)
                nSmsNotReadNum++;

        }
        _itow(nSmsNotReadNum,strnoreadnum,10);

        _itow(nSmsNumLocal,str,10);

        sms_node_text += _T(" (");
        sms_node_text += (TCHAR*)strnoreadnum;
        sms_node_text +=_T("/");
        sms_node_text += (TCHAR*)str;
        sms_node_text +=_T(")");

        m_treeSms.SetItemText(hRoot,sms_node_text);

        //return hRoot;

    }
    if (wcsstr(sms_node_text,sms_RUIM)) {
        sms_node_text = sms_RUIM;
        //	nSmsNumLocal = GetTotalSmsNum(SMS_TYPE_INBOX, LOCFILTER_UIM);
        nSmsNumLocal = g_SM_SmsNum;
        _itow(nSmsNumLocal,str,10);
        sms_node_text += _T(" (");
        //sms_node_text +=_T("/");
        sms_node_text += (TCHAR*)str;
        sms_node_text +=_T(")");
        m_treeSms.SetItemText(hRoot,sms_node_text);
        //return hRoot;


    }
    if (wcsstr(sms_node_text,sms_usbmodem)) {
        sms_node_text = sms_usbmodem;
        //sms_node_text.LoadString(IDS_SMSTREE_USBMODEM);
        //nSmsNumLocal = GetTotalSmsNum(SMS_TYPE_INBOX, LOCFILTER_ME);
        nSmsNumLocal = g_ME_SmsNum;
        _itow(nSmsNumLocal,str,10);
        sms_node_text += _T(" (");
        sms_node_text += (TCHAR*)str;
        sms_node_text +=_T(")");
        m_treeSms.SetItemText(hRoot,sms_node_text);
        //	return hRoot;


    }
    if (wcsstr(sms_node_text,sms_inbox)) {
        sms_node_text = sms_inbox;

        nSmsNumLocal = GetTotalSmsNum(SMS_TYPE_INBOX, LOCFILTER_PC);

        for (int i =0; i<nSmsNumLocal; i++) {
            SmsRecord = GetTotalSmsRecordFromDspIndex(SMS_TYPE_INBOX, LOCFILTER_PC, i);
            if(SmsRecord.state == SMS_STATE_MT_NOT_READ)
                nSmsNotReadNum++;

        }


        _itow(nSmsNumLocal,str,10);
        _itow(nSmsNotReadNum,strnoreadnum,10);

        sms_node_text += _T(" (");
        sms_node_text += (TCHAR*)strnoreadnum;
        sms_node_text +=_T("/");
        sms_node_text += (TCHAR*)str;
        sms_node_text +=_T(")");
        m_treeSms.SetItemText(hRoot,sms_node_text);
        //return hRoot;


    }
    if (wcsstr(sms_node_text,sms_outbox)) {
        sms_node_text = sms_outbox;
        sms_node_text.LoadString(IDS_SMSTREE_OUTBOX);
        nSmsNumLocal = GetTotalSmsNum(SMS_TYPE_OUTBOX, LOCFILTER_PC);
        _itow(nSmsNumLocal,str,10);
        sms_node_text += _T(" (");
        sms_node_text += (TCHAR*)str;
        sms_node_text +=_T(")");
        m_treeSms.SetItemText(hRoot,sms_node_text);
        //return hRoot;


    }
    if (wcsstr(sms_node_text,sms_draft)) {
        sms_node_text = sms_draft;
        nSmsNumLocal = GetTotalSmsNum(SMS_TYPE_DRAFT, LOCFILTER_PC);
        _itow(nSmsNumLocal,str,10);
        sms_node_text += _T(" (");
        sms_node_text += (TCHAR*)str;
        sms_node_text +=_T(")");
        m_treeSms.SetItemText(hRoot,sms_node_text);
        //return hRoot;


    }
    if (wcsstr(sms_node_text,sms_archive)) {
        sms_node_text = sms_archive;
        sms_node_text.LoadString(IDS_SMSTREE_ARCHIVE);
        nSmsNumLocal = GetTotalSmsNum(SMS_TYPE_ARCHIVE, LOCFILTER_PC);
        _itow(nSmsNumLocal,str,10);
        sms_node_text += _T(" (");
        sms_node_text += (TCHAR*)str;
        sms_node_text +=_T(")");
        m_treeSms.SetItemText(hRoot,sms_node_text);
        //return hRoot;


    }
    if (wcsstr(sms_node_text,sms_voicemail)) {

        sms_node_text = sms_voicemail;
        sms_node_text += _T(" (");
        sms_node_text += voicesmscount;
        sms_node_text +=_T(")");
        m_treeSms.SetItemText(hRoot,sms_node_text);


    }
    //如果hRoot没有子节点，返回NULL
    if (m_treeSms.ItemHasChildren(hRoot) == FALSE)
        return NULL;

    //递归查找hRoot的所有子节点
    HTREEITEM hRes = NULL;
    HTREEITEM hItem = m_treeSms.GetChildItem(hRoot);
    while (hItem) {
        /*	hRes =*/ OnSmsUpdataNumforSmsTree(hItem); //查以hItem为根的枝
// 		if (hRes != NULL)     //如果在以hItem为根的枝里找到，返回结果
// 			return hRes;
// 		else        //否则，查找与hItem同级的下一个枝
        hItem = m_treeSms.GetNextSiblingItem(hItem);
    } // end of while(hItem != NULL, has next item)

    return NULL;
}


void CSmsDlg::OnVScrollContent()
{
    // TODO: Add your message handler code here and/or call default
    GetDlgItem(IDC_EDIT_SMSCONTENT)->Invalidate(true);


}
LRESULT CSmsDlg::OnReadFlashSMS(WPARAM wParam, LPARAM lParam)
{
    CHSDPADlg* pMainWnd = NULL;
    pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
//	pMainWnd->CFTWillView = FALSE;

    if (pMainWnd->m_pReadFalshSMSDlg == NULL) {

        pMainWnd->m_pReadFalshSMSDlg = new CReadFalshSMS();
        pMainWnd->m_pReadFalshSMSDlg->Create(IDD_DIALOG_READ_CFTSMS,NULL);
        pMainWnd->m_pReadFalshSMSDlg->ShowWindow(SW_SHOW);

    } else {
        pMainWnd->m_pReadFalshSMSDlg->UpdataSMS();
    }
    return 0;

}

LRESULT CSmsDlg::OnModemStausChange(WPARAM wParam, LPARAM lParam)
{
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    if (g_bIsExist == FALSE || pMainWnd->m_bSimReady == FALSE) {
        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_EDIT_SMS_SEARCH)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_READ)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_REPLY)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_DIAL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_DELETE)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_NEW)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_FORWARD)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_TRANSFER)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_EXPORT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_SEARCH)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->EnableWindow(TRUE);
        InitSMSTree();
    } else {
        GetDlgItem(IDC_BUTTON_SMS_REPLY)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_SMS_RESUME)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_EDIT_SMS_SEARCH)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_READ)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_REPLY)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_DIAL)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_DELETE)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_NEW)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_FORWARD)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_ABSTRACT)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_EDIT)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_TRANSFER)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_EXPORT)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_SEARCH)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SMS_ARCHIVE)->EnableWindow(TRUE);
        InitSMSTree();

    }
    return 0;
}

BOOL CSmsDlg::SndAtCVMR()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT+CVMR\r");
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE)) {
        SetTimer(IDT_CVMR_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtCVMR, this);

        return TRUE;
    } else
        return FALSE;

}

void CSmsDlg::RspAtCVMR(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    USES_CONVERSION;
    CSmsDlg *pDlg = (CSmsDlg*)pWnd;
    CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
    pDlg->KillTimer(IDT_CVMR_TIMEOUT);
    CString strTemp = (char *)strArr[0];
    int pAsUSFlag = 0;//短信优先级标志,值为3时表示当前字符为优先级的值，解析时使用 add by liub
    char *p = NULL;
// 	int nIndexFrom = strTemp.Find(':');
// 	int nIndexTo = strTemp.GetLength();
//           = strTemp.Mid(nIndexFrom + 1/*, nIndexTo*/);

    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0) {
        AfxMessageBox(_T("View Error!"),MB_OK|MB_SYSTEMMODAL,0);
    }

    if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0
    && memcmp((const char*)strArr[0], gcstrVoicemailRes[0], strlen(gcstrVoicemailRes[0])) == 0) {
        AfxMessageBox(_T("No voice mail!"),MB_OK|MB_SYSTEMMODAL,0);
    } else if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0
    && memcmp((const char*)strArr[0], gcstrVoicemailRes[1], strlen(gcstrVoicemailRes[1])) == 0) {
        AfxMessageBox(_T("No voice mail to view!"),MB_OK|MB_SYSTEMMODAL,0);
    } else {

        //pDlg->SMS_type_CPTCMTCFTCDS = 3;
        StSmsRecord record;
        memset(&record, 0x00, sizeof(StSmsRecord));

        int cnt = 0;
        char *ptr[5], *p;
        p = (char *)&strArr[0][strlen(gcstrResSms[AT_SMS_QCMGR])];
        ptr[0] = ptr[1] = ptr[2] = ptr[3] = ptr[4] = 0;

        BOOL bOutQuot = TRUE;

        while(*p != '\0') {
            if(*p == '\"') {
                if(bOutQuot && cnt < 5)
                    ptr[cnt++] = p;

                bOutQuot = !bOutQuot;
                p++;
            } else if(*p == ',' && bOutQuot) {
                *p++ = '\0';
                pAsUSFlag++;
            } else {
                if (5 == pAsUSFlag) {
                    ptr[cnt++] = p;
                }
                p++;
            }
        }

        for(int i = 0; i < 5; i++) {
            if(ptr[i] != 0) {
                if(*(ptr[i] + strlen(ptr[i]) - 1) == '\"')
                    *(ptr[i] + strlen(ptr[i]) - 1) = '\0';
                if(*ptr[i] == '\"')
                    ptr[i]++;
            }
        }

        //state test begin
//     if(kind == SMS_KIND_MT)
//     {
//         if(record.state != SMS_STATE_MT_NOT_READ
//             && record.state != SMS_STATE_MT_READ)
//         {
//             return FALSE;
//         }
//     }
//     else if(kind == SMS_KIND_MO)
//     {
//         if(record.state != SMS_STATE_MO_NOT_SENT
//             && record.state != SMS_STATE_MO_SENT)
//         {
//             return FALSE;
//         }
//     }
        //state test end
        CString szNumTemp=(char*)ptr[1];
        wcscpy((TCHAR *)record.szNumber, szNumTemp);
        pMainDlg->tempvoicemailASCUS =*ptr[3];

        int time, scnum, concatenate;
        time = scnum = concatenate = -1;

        if(record.state == SMS_STATE_MT_NOT_READ || record.state == SMS_STATE_MT_READ) {
            if (!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000"))) {
                time = 2;
                scnum = 4;
                concatenate = 3;
            } else {
                time = 2;
                scnum = 3;
                concatenate = 4;
            }
        } else if(record.state == SMS_STATE_MO_NOT_SENT || record.state == SMS_STATE_MO_SENT) {
            if (!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000"))) {
                time = -1;
                scnum = 4;
                concatenate = 2;
            } else {
                time = 2;
                scnum = 4;
                concatenate = 3;
            }
            record.state = SMS_STATE_MT_READ;
        }

        if(time == -1 || !(ptr[time] && *ptr[time]))
            record.timestamp = COleDateTime::GetCurrentTime();
        else {
            if(!GetTimeFromStr((LPCSTR)ptr[time], record.timestamp))
                record.timestamp = COleDateTime::GetCurrentTime();
        }
        CString szSCNumTemp=(char*)ptr[scnum];
        if(scnum == -1 || !(ptr[scnum] && *ptr[scnum]))
            memset(record.szSCNumber, 0x00, SMS_SC_NUM_MAX);
        else
            wcsncpy((TCHAR*)record.szSCNumber, szSCNumTemp, SMS_SC_NUM_MAX);

        if(concatenate != -1 && ptr[concatenate] && *ptr[concatenate]) {
            if(ExtractConcatenateSmsPara(ptr[concatenate], &record.nRefCnt, &record.nSeqCnt, &record.nTotalCnt))
                record.flag |= SMS_RECORD_FLAG_CONCATENATE_SEGE;
        }
        //modified by wk end on 2006-8-22

        USES_CONVERSION;
        if(wStrNum == 3) {
            //modify by lijl 2009.4.13 以ascii码形式存储短信内容
            CString strGb = UCS2ToGB(A2W((char*)strArr[1]));
            strncpy(record.szContent, W2A(strGb), SMS_CHAR_MAX);
        } else
            memset((TCHAR *)record.szContent, 0, SMS_CHAR_MAX*2);


        if ((CString)record.szNumber !=_T("166")) {
            record.voicemail = 1;
            pMainDlg->VoicemailRecord = record;
            pDlg->PostMessage(WM_SMS_READ_FLASHSMS,0,0);
        }
    }



}

BOOL CSmsDlg::SndAtCVMD()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT+CVMD\r");
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE)) {
        SetTimer(IDT_CVMD_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtCVMD, this);

        return TRUE;
    } else
        return FALSE;

}

void CSmsDlg::RspAtCVMD(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
//	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0) {
        AfxMessageBox(_T("Clear icon Error!"));
    }
    if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])) {

        pMainDlg->m_pSmsDlg->voicesmscount =_T("0");
        pMainDlg->VoiceMailCount = _T("-1");
        pMainDlg->m_pSmsDlg->VoiceMailCountSMSDlg = _T("-1");
        g_ME_SmsMax++;

        HTREEITEM hRoot = pMainDlg->m_pSmsDlg->m_treeSms.GetRootItem();
        hRoot = pMainDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
//		hRoot = pMainDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
#ifdef FEATURE_VERSION_NOSIM
        hRoot = pMainDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
#endif
        pMainDlg->m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
        pMainDlg->PostMessage(WM_ICON_UPDATE,ICON_TYPE_VOICEMAIL,0);
    }



}

void CSmsDlg::Updatatree()
{
    CHSDPADlg* pDlg = NULL;
    pDlg = (CHSDPADlg*)AfxGetMainWnd();
    HTREEITEM hRoot = pDlg->m_pSmsDlg->m_treeSms.GetRootItem();
    hRoot = pDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
//	hRoot = pDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
#ifdef FEATURE_VERSION_NOSIM
    hRoot = pDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
#endif
    pDlg->m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);

}

