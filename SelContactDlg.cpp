// SelContactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "SelContactDlg.h"
#include "SelDialDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef enum {
    //选择存储器
    SEND_AT_SEL_STORE = 1,

    //获取USIM中的已使用条数和最大条数
    SEND_STORE_CHECK_TOTAL,

    //获取USIM中电话本信息
    SEND_STORE_READ_ALL,

} SendATState;

/////////////////////////////////////////////////////////////////////////////
// CSelContactDlg dialog
CSelContactDlg::CSelContactDlg(CWnd* pParent, EnSelContactType SelType)
    : CDialog(CSelContactDlg::IDD, pParent)
{
    m_pPbData = ((CHSDPAApp*)AfxGetApp())->GetPbData();
    m_parent = pParent;
    m_SelType = SelType;

    m_nStoreFlag = 0;
    m_ReadDataCardAndUSIMStatus = false;
    m_pDlg = NULL;
    m_ReadDataFromDCState = 0;
    m_nIndex = 2;
}

void CSelContactDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSelContactDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDC_LIST_SELCONTACT, m_lstSelContact);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSelContactDlg, CDialog)
    //{{AFX_MSG_MAP(CSelContactDlg)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_SELCONTACT, OnDblclkListSelcontact)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelContactDlg message handlers
BOOL CSelContactDlg::OnInitDialog()
{
    //CBaseDialog::OnInitDialog();
    CDialog::OnInitDialog();
    //m_bFirst = TRUE;
    //SetSkin(g_SetData.Setup_nSkinStyle);
    UpdateData(FALSE);

    // wyw_0330 注释掉的下方代码，因为电话本中USB modem中的数据已初始化
    /*
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
    */
    InitListCtrl();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelContactDlg::InitListCtrl()
{
    //表格列数
    int cols=6;
    int arr_cols[]= {IDS_NAME,IDS_MOBILE,IDS_HOME,IDS_COMPANY,IDS_EMAIL,IDS_REMARK};
    int i;
    CRect rect;
    m_lstSelContact.GetWindowRect(&rect);

    CString strColTitle;
    //设置列名
    for(i = 0; i < cols; i++) {
        strColTitle.LoadString(arr_cols[i]);
        m_lstSelContact.InsertColumn(i, strColTitle,LVCFMT_LEFT,rect.Width()/cols);    //(i==0)?25:rect.Width()/cols);
    }

    //added by wk begin on 2006-12-5
    LONG dwValue = GetWindowLong(m_lstSelContact.GetSafeHwnd(), GWL_STYLE);
    if(m_SelType == SELCONTACT_SMSNUMBER)
        dwValue &= (LONG)~LVS_SINGLESEL;
    else
        dwValue |= LVS_SINGLESEL;
    SetWindowLong(m_lstSelContact.GetSafeHwnd(), GWL_STYLE, dwValue);
    //added by wk end on 2006-12-5

    m_lstSelContact.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    //填入数据
    StPbRecord *PbBookTemp = new StPbRecord();
    memset(PbBookTemp,0,sizeof(StPbRecord));
    int nIndex = 0;
    for(i = 0; i < PB_RECORD_MAX; i++) {
        if(-1 != m_pPbData->PbDatas[i].index) {
            m_lstSelContact.InsertItem(nIndex, m_pPbData->PbDatas[i].strName, (DWORD)(&m_pPbData->PbDatas[i]));
            m_lstSelContact.SetItemText(nIndex, SELCONTACT_LISTCTRL_HEADERINDEX_MOBILE, m_pPbData->PbDatas[i].strMobile);
            m_lstSelContact.SetItemText(nIndex, SELCONTACT_LISTCTRL_HEADERINDEX_HOME, m_pPbData->PbDatas[i].strHome);
            m_lstSelContact.SetItemText(nIndex, SELCONTACT_LISTCTRL_HEADERINDEX_COMPANY, m_pPbData->PbDatas[i].strCompany);
            m_lstSelContact.SetItemText(nIndex, SELCONTACT_LISTCTRL_HEADERINDEX_EMAIL, m_pPbData->PbDatas[i].strEmail);
            m_lstSelContact.SetItemText(nIndex, SELCONTACT_LISTCTRL_HEADERINDEX_REMARK, m_pPbData->PbDatas[i].strMark);
            nIndex ++;
        }
    }

    //wyw_0328 add	填入 USB modem 中的电话本数据
    for(i = 0; i < g_DataCardTotalNum; i++) {
        if(-1 != pDataCardPbRecord[i].index) {
            int index = m_lstSelContact.InsertItem(nIndex, pDataCardPbRecord[i].strName,
                                                   (DWORD)(&pDataCardPbRecord[i]));
            m_lstSelContact.SetItemText(nIndex, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE,
                                        pDataCardPbRecord[i].strMobile);
            nIndex ++;
        }
    }
    //wyw_0328 end
#ifndef FEATURE_VERSION_NOSIM
    for(i = 0; i < g_USimTotalNum; i++) {
        if(-1 != pUSIMPbRecord[i].index) {
            int index = m_lstSelContact.InsertItem(nIndex, pUSIMPbRecord[i].strName,
                                                   (DWORD)(&pUSIMPbRecord[i]));
            m_lstSelContact.SetItemText(nIndex, PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE,
                                        pUSIMPbRecord[i].strMobile);
            nIndex ++;
        }
    }
#endif


    m_lstSelContact.AdjustColumnWidth();
    m_lstSelContact.m_headerCtrl.DrawHeader();
    delete PbBookTemp;
}

//选择若干联系人，点击OK
void CSelContactDlg::OnOK()
{
    int nItem;      //联系人列表中的序号
    POSITION pos;   //选择的联系人总数

    pos = m_lstSelContact.GetFirstSelectedItemPosition();

    if(!pos)
        return;

    if(m_SelType == SELCONTACT_SMSNUMBER) {
        int nItemNum=0;
        nItemNum = m_lstSelContact.GetSelectedCount();
        if (nItemNum > SMS_GROUPSEND_MAX) {
            AfxMessageBox(IDS_PB_SMSSELMAX);
            return;
        }
        while(pos) {
            nItem = m_lstSelContact.GetNextSelectedItem(pos);
            if (!(SelContact(nItem))) {
                return;
            }
        }
    } else {
        nItem = m_lstSelContact.GetNextSelectedItem(pos);
        if(!(SelContact(nItem))) {
            return;
        }
    }
    CDialog::OnOK();
}

//双击某一联系人
void CSelContactDlg::OnDblclkListSelcontact(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    int nItem = m_lstSelContact.GetNextItem(-1, LVNI_SELECTED);
    if (-1 == nItem) {
        return;
    }
    if (!(SelContact(nItem))) {
        return;
    }
    *pResult = 0;
    CDialog::OnOK();
}

bool CSelContactDlg::SelContact(int nItem)
{
    StPbRecord* PbBookTemp = (StPbRecord* ) m_lstSelContact.GetItemData(nItem);

    int nDialNum=0;
    int SelDial=0;

    if(wcslen(PbBookTemp->strMobile) > 0) {
        nDialNum++;
        SelDial=1;
    }
    if(wcslen(PbBookTemp->strHome) > 0) {
        nDialNum++;
        SelDial=2;
    }
    if(wcslen(PbBookTemp->strCompany) > 0) {
        nDialNum++;
        SelDial=3;
    }

    if(0 == nDialNum) {
        CString strNullNum;
        strNullNum.LoadString(IDS_SELCONNECT_NULLNUM);
        AfxMessageBox(strNullNum);
        return false;
    }

    WPARAM wParam = 0;
    LPARAM lParam = (LPARAM)m_SelType;
    CWnd *pWnd = m_parent;

    if(nDialNum == 1) {
        switch(SelDial) {
        case 1: {
            wParam = (WPARAM)(LPCTSTR)PbBookTemp->strMobile;
            pWnd->PostMessage(WM_SEL_CONTACT, wParam,lParam);
            break;
        }
        case 2: {
            wParam = (WPARAM)(LPCTSTR)PbBookTemp->strHome;
            pWnd->PostMessage(WM_SEL_CONTACT, wParam,lParam);
            break;
        }
        case 3: {
            wParam = (WPARAM)(LPCTSTR)PbBookTemp->strCompany;
            pWnd->PostMessage(WM_SEL_CONTACT, wParam,lParam);
            break;
        }
        }
    } else {
        CSelDialDlg SelDlg(this);
        wcscpy(SelDlg.m_strName.GetBuffer(MAX_PATH), PbBookTemp->strName);
        wcscpy(SelDlg.m_strMobile.GetBuffer(MAX_PATH), PbBookTemp->strMobile);
        wcscpy(SelDlg.m_strHome.GetBuffer(MAX_PATH), PbBookTemp->strHome);
        wcscpy(SelDlg.m_strCompany.GetBuffer(MAX_PATH), PbBookTemp->strCompany);

        if(IDOK == SelDlg.DoModal()) {
            //wyw_0328 modify for Memory leak!
//    		int iLen = SelDlg.m_strSelCallNum.GetLength();
//			TCHAR* szRs = new TCHAR[iLen];
//			lstrcpy(szRs, SelDlg.m_strSelCallNum.GetBuffer(iLen));
//			SelDlg.m_strSelCallNum.ReleaseBuffer();
//          wParam = (WPARAM)(LPCTSTR)(szRs);
            wParam = (WPARAM)(LPCTSTR)(SelDlg.m_strSelCallNum);

            pWnd->SendMessage(WM_SEL_CONTACT, wParam,lParam);			//wyw_0328 modify, old is PostMessage
        }
    }
    return true;
}

void CSelContactDlg::ReadDataFromDCOrCard(int flag)
{
    Sleep(100);
    m_nIndex = 2;

    BOOL bResponse = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->CommIsReady();
    if (!bResponse) {
        m_ReadDataCardAndUSIMStatus = false;
        return ;
    }

    MallocDlgMem();
    m_nStoreFlag = flag;
    CString StrSource = _T("AT+CPBS=\"SM\"");
    switch (flag) {
    case 0: {
        StrSource = _T("AT+CPBS=\"ME\"");
        m_pDlg->m_strPrompt.LoadString(IDS_PBTRANSFER_INITCARDINFO);

        break;
    }
    case 1: {
        StrSource = _T("AT+CPBS=\"SM\"");
        m_pDlg->m_strPrompt.LoadString(IDS_PBTRANSFER_INITUSIMINFO);

        break;
    }

    default: {
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
    if(pComm->WriteToPort(szAtAnsBuf, wcslen(szAtBuf))) {
        SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, this);

        m_pDlg->DoModal();

        Sleep(10);
    } else {
        m_pDlg->m_bSuccess = false;
        m_pDlg->SendMessage(WM_CLOSE, 0, 0);
        //AfxMessageBox("Write to port failed! ");//Debug info
        CString strAppName;
        strAppName.LoadString(IDS_APPNAME);
        AddPrefixName(strAppName);
        CString strTransfail;
        strTransfail.LoadString(IDS_PBTRANSFER_FAILED);
        //AfxMessageBox(strTransfail);
        ::MessageBox(m_hWnd, strTransfail, strAppName, MB_OK);
        m_ReadDataCardAndUSIMStatus = false;
    }
}

void CSelContactDlg::MallocDlgMem(int nUpper, int nStep)
{
    if (NULL != m_pDlg) {
        delete m_pDlg;
    }
    m_pDlg = new CWaitDlg(this,nUpper,nStep);
}

CSelContactDlg::~CSelContactDlg()
{
    if (NULL != m_pDlg) {
        delete m_pDlg;
    }
    ::SetEvent(g_BGPassEvt);
}

void CSelContactDlg::AtRespReadState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    int Len = 512;
    char szAtBuf[512];
    char buf[10240] = {0};
    CSelContactDlg* pDlg = (CSelContactDlg*)pWnd;
    CSerialPort* pComm;
    CString buffer = _T("");
    char *pbuf;

    switch(pDlg->m_ReadDataFromDCState) {
        //选择存储器
    case SEND_AT_SEL_STORE: {
        pDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE);
        if (0 == wStrNum) {
            pDlg->m_ReadDataCardAndUSIMStatus = false;
            pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
            //AfxMessageBox("选择存储器失败");  //debug

            return;
        }
        for(int Nline = 0; Nline < wStrNum; Nline++) {
            CString strResult = strArr[Nline];

            if (0 == strResult.CompareNoCase(_T("ERROR"))) {
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
        if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf))) {
            pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_CHECK_TOTAL, 30000, NULL);
            RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd);
            pDlg->m_ReadDataFromDCState = SEND_STORE_CHECK_TOTAL;
        } else {
            pDlg->m_ReadDataCardAndUSIMStatus = false;
            pDlg->m_pDlg->m_bSuccess = false;
            pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
        }

        break;
    }

    //获取STORE中的已使用条数和最大条数
    case SEND_STORE_CHECK_TOTAL: {
        pDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_CHECK_TOTAL);
        if (0 == wStrNum) {
            pDlg->m_ReadDataCardAndUSIMStatus = false;
            pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
            //AfxMessageBox("获取STORE中的最大条数失败");  //debug

            return;
        }
        for(int Nline = 0; Nline < wStrNum; Nline ++) {
            CString strResult = strArr[Nline];

            if (0 == strResult.CompareNoCase(_T("ERROR"))) {
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

        if(0 != nStoreUserNumer) {
            if (0 == pDlg->m_nStoreFlag) { //DataCard存储器存放电话薄数据的内存进行初始化
                g_DataCardUsedNum = nStoreUserNumer;
            } else if (1 == pDlg->m_nStoreFlag) {
                g_USimUsedNum = nStoreUserNumer;
            }

        }

        if (0 != nStoreTotalNumer) {
            if (0 == pDlg->m_nStoreFlag) { //DataCard存储器存放电话薄数据的内存进行初始化
                g_DataCardTotalNum = nStoreTotalNumer;

                if (NULL != pDataCardPbRecord) {
                    delete []pDataCardPbRecord;
                }

                pDataCardPbRecord = new StPbRecord[g_DataCardTotalNum];
                memset(pDataCardPbRecord,0,(sizeof(StPbRecord) * g_DataCardTotalNum));

                for (int i = 0; i < g_DataCardTotalNum; i++) {
                    pDataCardPbRecord[i].index = -1;
                }

                pDlg->m_pDlg->SetProgressRange(0,g_DataCardTotalNum,1);
            } else if (1 == pDlg->m_nStoreFlag) { //Usim存储器存放电话薄数据的内存进行初始化
                g_USimTotalNum = nStoreTotalNumer;

                if (NULL != pUSIMPbRecord) {
                    delete []pUSIMPbRecord;
                }

                pUSIMPbRecord = new StPbRecord[g_USimTotalNum];
                memset(pUSIMPbRecord,0,(sizeof(StPbRecord) * g_USimTotalNum));

                for (int i = 0; i < g_USimTotalNum; i++) {
                    pUSIMPbRecord[i].index = -1;
                }

                pDlg->m_pDlg->SetProgressRange(0,g_USimTotalNum,1);
            }
        } else {
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
        if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf))) {
            pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL, 30000, NULL);
            RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd);
            pDlg->m_ReadDataFromDCState = SEND_STORE_READ_ALL;
        } else {
            pDlg->m_ReadDataCardAndUSIMStatus = false;
            pDlg->m_pDlg->m_bSuccess = false;
            pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
        }

        break;
    }

    //获取存储器中电话本信息
    case SEND_STORE_READ_ALL: {
        pDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL);

        if (0 == wStrNum) {
            pDlg->m_ReadDataCardAndUSIMStatus = false;
            pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
            //AfxMessageBox("获取存储器中电话本信息失败");  //Debug

            return;
        }

        for(int Nline = 0; Nline < wStrNum; Nline++) {
            CString strResult = strArr[Nline];

            if (0 == strResult.CompareNoCase(_T("ERROR"))) {
                pDlg->m_ReadDataCardAndUSIMStatus = false;
                pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
                //AfxMessageBox("获取存储器中电话本信息失败");  //Debug

                return;
            }
        }

        StPbRecord *pStDCPbRecord = NULL;
        int nStoreTotalNum = 0;

        if (0 == pDlg->m_nStoreFlag) {
            pStDCPbRecord = pDataCardPbRecord;
            nStoreTotalNum = g_DataCardTotalNum;
        }

        else if (1 == pDlg->m_nStoreFlag) {
            pStDCPbRecord = pUSIMPbRecord;
            nStoreTotalNum = g_USimTotalNum;
        }

        if (wStrNum == 2) {
            CString strResult = strArr[wStrNum - 1];

            //返回格式为：+CPBR: 1,"123",129,"test"
            if (0 == strResult.CompareNoCase(_T("OK"))) {
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
                if (_T("") != strName) {
                    strName = UCS2ToGB(strName);
                } else {
                    strName.Format(IDS_UNKNOWN_NAME);
                }
                memcpy(pStDCPbRecord[nIndex - 1].strName, strName, PB_NAME_MAX);
            }
        }

        char  StrIndex[4];

        if (pDlg->m_nIndex <= nStoreTotalNum) {
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
            if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf))) {
                pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL, 30000, NULL);
                RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadState, pWnd);
                pDlg->m_ReadDataFromDCState = SEND_STORE_READ_ALL;
            } else {
                pDlg->m_ReadDataCardAndUSIMStatus = false;
                pDlg->m_pDlg->m_bSuccess = false;
                pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
            }
        } else {
            pDlg->m_pDlg->ProgressCompleted(pDlg->m_pDlg->m_nUpper);
            pDlg->m_pDlg->PostMessage(WM_CLOSE, 0, 0);
            pDlg->m_ReadDataFromDCState = 0;
        }

        break;
    }
    default: {
        //AfxMessageBox("m_ReadDataFromDCState status error!"); //debug info

        break;
    }
    }

}