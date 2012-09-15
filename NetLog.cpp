// NetLog.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "NetLog.h"
#include "Ds.h"
#include "PwdConfirmDlg.h"
#include "NetStatistics.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetLog dialog


CNetLog::CNetLog(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CNetLog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CNetLog)
    m_pLogData = ((CHSDPAApp *)AfxGetApp())->GetLogData();
    m_strDataOut = "";
    m_strDataIn = "";
    m_strDataInOut = "";
    m_strDuration = "";

	m_strGPRSDataINOut = _T("");
	m_strGPRSDuration = _T("");
	m_strUMTSDataInOut = _T("");
	m_strUMTSDuration = _T("");
	//}}AFX_DATA_INIT
#if 0
    stLogItem dTempItem;
    COleDateTime tm = COleDateTime::GetCurrentTime();
    dTempItem.cTimeStamp =tm;
	dTempItem.cDuration = 0;
    //dTempItem.nLogType = 0;
    dTempItem.dwByteIn=13834668;
    dTempItem.dwByteOut=13834668;
    m_pLogData->SaveItem(&dTempItem);
#endif
	m_nBkTag = 1;
}


void CNetLog::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNetLog)
    //Modified by lgz 2006-06-21
	DDX_Control(pDX, IDC_LIST_CALLLOG, m_lstCallLog);
    DDX_Text(pDX, IDC_EDIT_CALLLOG_DATAOUT, m_strDataOut);
    DDX_Text(pDX, IDC_EDIT_CALLLOG_DATAIN, m_strDataIn);
    DDX_Text(pDX, IDC_EDIT_CALLLOG_DATAINOUT, m_strDataInOut);
    DDX_Text(pDX, IDC_EDIT_CALLLOG_DURATION, m_strDuration);

	DDX_Text(pDX,IDC_EDIT_CALLLOG_GPRSDURATION,m_strGPRSDuration);
	DDX_Text(pDX,IDC_EDIT_CALLLOG_GPRSDATAINOUT,m_strGPRSDataINOut);
	DDX_Text(pDX,IDC_EDIT_CALLLOG_UMTSDURATION,m_strUMTSDuration);
	DDX_Text(pDX,IDC_EDIT_CALLLOG_UMTSDATAINOUT,m_strUMTSDataInOut);
	//}}AFX_DATA_MAP
}

void CNetLog::InitListCtrl()
{
    //表格列数

    int cols=5;
    int arr_cols[]={IDS_LOG_NTYPE,IDS_LOG_TIMESTAMP,IDS_LOG_DURATION,
                    IDS_LOG_BYTEIN,IDS_LOG_BYTEOUT};
/*  
	int cols=4;
    int arr_cols[]={IDS_LOG_TIMESTAMP,IDS_LOG_DURATION,
                IDS_LOG_BYTEIN,IDS_LOG_BYTEOUT};
*/  
    CRect rect;
    m_lstCallLog.GetWindowRect(&rect);
    
    CString strColTitle;
    //设置列名
    for(int i=0;i<cols;i++)
    {
        strColTitle.LoadString(arr_cols[i]);
        m_lstCallLog.InsertColumn(i, strColTitle);
    }
    m_lstCallLog.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    //ReDrawListCtrl();
}

BEGIN_MESSAGE_MAP(CNetLog, CBaseDialog)
    //{{AFX_MSG_MAP(CNetLog)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
    ON_BN_CLICKED(IDC_BUTTON_DELALL, OnButtonDelall)
    ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_LOG_MENUITEM_CLICK, OnMenuItemClick)
END_MESSAGE_MAP()

int CNetLog::DealwithResponseProc(LPARAM lParam, WPARAM wParam)
{
    DispNetLog();
    return 1;
}

void CNetLog::DispNetLog()
{
    stLogItem dTempItem;
    CRect rect;
    DWORD m_dGPRSDataIn = 0;
    DWORD m_dGPRSDataOut = 0;
	CTimeSpan tsGPRStotalDura(0);
	DWORD m_dUMTSDataIn = 0;
    DWORD m_dUMTSDataOut = 0;
	CTimeSpan tsUMTStotalDura(0);
	DWORD m_dDataIn = 0;
    DWORD m_dDataOut = 0;
    CTimeSpan totalDura(0);

    m_lstCallLog.GetWindowRect(&rect);
    m_lstCallLog.DeleteAllItems();

	CString strTemp;
    for(int i=0; i < m_pLogData->GetCount(); i++)
    {
        dTempItem = m_pLogData->GetItem(i);

        //m_lstCallLog.InsertItem(i,sDialType[dTempItem.nLogType],i);
		CString strTemp;
		//strTemp.LoadString(gcstrNetWorkSrv[dTempItem.nLogType]);
        //m_lstCallLog.InsertItem(i, strTemp, i);		
        //m_lstCallLog.InsertItem(i, dTempItem.cTimeStamp.Format("%d/%m/%Y %H:%M:%S"),i);

		if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
		{
			switch(dTempItem.nLogType)
			{
				case NW_CALLLOG_GSM:
				{
					strTemp.Format(_T("2G"));

					m_dGPRSDataIn += dTempItem.dwByteIn;
					m_dGPRSDataOut += dTempItem.dwByteOut;
					tsGPRStotalDura += dTempItem.cDuration;

					break;
				}
				case NW_CALLLOG_WCDMA:
				{
					strTemp.Format(_T("3G"));
					
					m_dUMTSDataIn += dTempItem.dwByteIn;
					m_dUMTSDataOut += dTempItem.dwByteOut;
					tsUMTStotalDura += dTempItem.cDuration;

					break;
				}
				default:
				{
					strTemp.Format(_T("%d %s"),dTempItem.nLogType,_T("err"));

					break;
				}			 
			}
		}
		else
		{
			strTemp.Format(_T("1X"));

			m_dGPRSDataIn += dTempItem.dwByteIn;
			m_dGPRSDataOut += dTempItem.dwByteOut;
			tsGPRStotalDura += dTempItem.cDuration;
		}
		m_dDataIn += dTempItem.dwByteIn;
        m_dDataOut += dTempItem.dwByteOut;
        totalDura += dTempItem.cDuration;
		
		m_lstCallLog.InsertItem(i, strTemp,i);
        m_lstCallLog.SetItemText(i, LOGTIMESTAMP, dTempItem.cTimeStamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT));
		m_lstCallLog.SetItemText(i, LOGDURATION, dTempItem.cDuration.Format("%D:%H:%M:%S"));

		//m_lstCallLog.SetItemText(i, LOGBYTEIN, itoa(dTempItem.dwByteIn,temp,10));
        //m_lstCallLog.SetItemText(i, LOGBYTEOUT, itoa(dTempItem.dwByteOut,temp,10));
		strTemp.Format(_T("%.2f"), dTempItem.dwByteIn/1024.0);
		m_lstCallLog.SetItemText(i, LOGBYTEIN, strTemp);
		strTemp.Format(_T("%.2f"), dTempItem.dwByteOut/1024.0);
		m_lstCallLog.SetItemText(i, LOGBYTEOUT, strTemp);		
    }
	m_strGPRSDataINOut.Format(_T("%.2f"), (m_dGPRSDataIn + m_dGPRSDataOut)/1024.0);
    m_strGPRSDuration.Format(_T("%s"), tsGPRStotalDura.Format("%D:%H:%M:%S"));
	
	m_strUMTSDataInOut.Format(_T("%.2f"), (m_dUMTSDataIn + m_dUMTSDataOut)/1024.0);
    m_strUMTSDuration.Format(_T("%s"), tsUMTStotalDura.Format("%D:%H:%M:%S"));

	m_strDataIn.Format(_T("%.2f"), m_dDataIn/1024.0);
	m_strDataOut.Format(_T("%.2f"), m_dDataOut/1024.0);
    m_strDataInOut.Format(_T("%.2f"), (m_dDataIn+m_dDataOut)/1024.0);
    m_strDuration.Format(_T("%s"), totalDura.Format("%D:%H:%M:%S"));
	
    UpdateData(FALSE);
    m_lstCallLog.AdjustColumnWidth();
	m_lstCallLog.m_headerCtrl.DrawHeader();
    m_lstCallLog.Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CNetLog message handlers
BOOL CNetLog::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
	m_nBkTag = 1;
    
    m_bFirst = TRUE;

	//SetSkin(g_SetData.Setup_nSkinStyle);

    UpdateData(FALSE);
    
    InitListCtrl();
    DispNetLog();
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


//删除记录改为EXIT
void CNetLog::OnButtonDelete() 
{
#if 0
	int       nCurSel;
	DWORD     dwRow;
	CString   strMessage;
	stLogItem dTempItem;

    if(m_pLogData->IsEmpty())
        return;

    if((nCurSel=m_lstCallLog.GetNextItem(-1, LVNI_SELECTED)) == -1)
	{
		CString strWarnInf;
        strWarnInf.LoadString(IDS_NOITEMSEL);
        AfxMessageBox(strWarnInf, MB_ICONQUESTION);
		return;
	}
    
    if(AfxMessageBox(IDS_DELETECONFIRM, MB_YESNO) == IDNO) 
    {
        return;
    }

    do {
        CString strStamp = m_lstCallLog.GetItemText(nCurSel, LOGTIMESTAMP);
	    COleVariant vtime(strStamp);
	    vtime.ChangeType(VT_DATE);
	    dTempItem.cTimeStamp=vtime;
    
        dwRow =m_pLogData->SearchList(&dTempItem);

        if(dwRow==-1){
            CString strWarnInf;
            strWarnInf.LoadString(IDS_NOITEMSEL);
            AfxMessageBox(strWarnInf, MB_ICONQUESTION);
            return;
        }

        m_pLogData->RemoveItem(dwRow);
    } while((nCurSel = m_lstCallLog.GetNextItem(nCurSel, LVNI_SELECTED)) != -1);   
        
    m_lstCallLog.DeleteAllItems();
    DispNetLog();    
#else
    OnCancel();
#endif
}


//删除全部记录
void CNetLog::OnButtonDelall()
{
	CPwdConfirmDlg PwdConfirmDlg(this);
	int nRes = PwdConfirmDlg.DoModal();
	if(nRes == IDCANCEL)
	{
		return;
	}
    if(m_pLogData->IsEmpty())
        return;
    
    CString strMessage;
    strMessage.LoadString(IDS_DELALLCONFIRM);

//     if(AfxMessageBox(strMessage, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDYES)
//     {

      
		
		CHSDPADlg* pMainWnd=((CHSDPADlg *)AfxGetMainWnd());
		pMainWnd->m_pUsageDlg->SendMessage(WM_USAGE_RESETNETSTATISTICS,NULL,NULL);
		  m_pLogData->ClearAll();
		  m_lstCallLog.DeleteAllItems();
		DispNetLog();
    // }
}


LRESULT CNetLog::OnMenuItemClick(WPARAM wParam, LPARAM lParam)
{
    UINT nID = wParam;
    //ASSERT(nID >= IDC_BUTTON_SMS_READ && nID <= IDC_BUTTON_SMS_ABSTRACT);
    
    if(nID == IDC_BUTTON_DELETE)
        OnButtonDelete();
    else if(nID == IDC_BUTTON_DELETEALL)
        OnButtonDelall();
	else if(nID == IDC_BUTTON_EXPORT)
		OnButtonExport();
    
    return 0;
}

//导出数据连接日志
void CNetLog::OnButtonExport() 
{
    // TODO: Add your control notification handler code here
    if(m_pLogData->IsEmpty())
        return;
    IGenerator netlogHandle(this);
    if (!netlogHandle.SetPath(EXP_NETLOG)) 
    {
		netlogHandle.RecoverPath();

        return;
    }
    //CString tempCell;
	

	//判断是导出全部还是导出部分电话薄记录
	POSITION pos = m_lstCallLog.GetFirstSelectedItemPosition();
	if (!pos)
	{
		for(int i = 0;i < m_pLogData->GetCount();i++)
		{
			ExportARecord(&netlogHandle,i);      
		}
	}
	else
	{
		while(pos)
		{
			int iLine = m_lstCallLog.GetNextSelectedItem(pos);
			ExportARecord(&netlogHandle,iLine);				
		}		
	}
    netlogHandle.MakeFile();    
	netlogHandle.RecoverPath();
}

//导出一条电话薄记录
void CNetLog::ExportARecord(IGenerator* netlogHandle,int iIndex)
{
    // TODO: Add extra validation here
    stLogItem dTempItem;
    dTempItem = m_pLogData->GetItem(iIndex);
	CString tempCell;

    //netlogHandle->SetPath(EXP_NETLOG);
	//bool bResponse = m_pLogData->ReadSingleLine(m_pPbData->GetRealIndex(iIndex),&phbitem);
	//if (!bResponse)
	//{
		//AfxMessageBox("error when Read data ");
	//}
	if (0 == netlogHandle->GetFormat()) 
	{
		//tempCell.Format("%s", sDialType[dTempItem.nLogType]);
		//资源修改，类型出错。
        //tempCell.LoadString(gcstrNetWorkSrv[dTempItem.nLogType]);
		//netlogHandle->FillCell(tempCell);
		switch(dTempItem.nLogType)
		{
			case NW_SRV_GPRS:
			case NW_SRV_EDGE:
			case NW_SRV_GSM:
			{				
				tempCell.Format(_T("GPRS"));
				break;
			}
			case NW_SRV_HSDPA:
			{
				tempCell.Format(_T("UMTS"));
				break;
			}
			default:
			{
				tempCell.Format(_T("ERR"));
				break;
			}
		}
		netlogHandle->FillCell(tempCell);

        tempCell=dTempItem.cTimeStamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);
		netlogHandle->FillCell(tempCell);

		tempCell=dTempItem.cDuration.Format("%D:%H:%M:%S");
		netlogHandle->FillCell(tempCell);

		tempCell.Format(_T("%d"), dTempItem.dwByteIn);
		netlogHandle->FillCell(tempCell);

		tempCell.Format(_T("%d"), dTempItem.dwByteOut);
		netlogHandle->FillCell(tempCell);

		netlogHandle->FillTrail();
	}
	else
	{
		//其它格式
	}
}








