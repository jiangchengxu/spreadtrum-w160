// NetStatistics.cpp : implementation file
//

#include "stdafx.h"
#include "Common_DataCrad\Common_DataCrad.h"
#include <windowsx.h>
#include "LogData.h"
#include "HSDPADlg.h"

#include "NetStatistics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetStatistics dialog


int SelectedInterface;
// A handy callback function...
	void CALLBACK interfaceHasChanged(int interfacenumber);

IMPLEMENT_DYNAMIC(CNetStatistics, CTabedDialog)
CNetStatistics::CNetStatistics(CWnd* pParent /*=NULL*/)
	: CTabedDialog(CNetStatistics::IDD, pParent)
{
	m_nBkTag = 1;
}

CNetStatistics::~CNetStatistics()
{
	if(m_pImageList)
		delete [] m_pImageList;
}


void CNetStatistics::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_NET, m_Tree);
	DDX_Control(pDX, IDC_BUTTON_TRAFFIC_SEND,m_cOutgoing);
	DDX_Control(pDX, IDC_BUTTON_TRAFFIC_REC, m_cIncoming);
	DDX_Control(pDX, IDC_BUTTON_TRAFFIC_TOTAL, m_cTotalTraffic);
	//{{AFX_DATA_MAP(CNetStatistics)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetStatistics, CTabedDialog)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK,IDC_TREE_NET, OnRightClickTree)
	ON_BN_CLICKED(IDC_BUTTON_EXPANDALL,OnButtonExpandAll)
	ON_BN_CLICKED(IDC_BUTTON_COLLAPSEALL,OnButtonCollapseAll)
	ON_BN_CLICKED(IDC_BUTTON_RESETSTATISTICS,OnButtonResetStatistics)
	ON_BN_CLICKED(IDC_BUTTON_NETLOG,OnButtonNetLog)
	ON_MESSAGE(WM_USAGE_RESETNETSTATISTICS,OnResetStatistics)
END_MESSAGE_MAP()


void CNetStatistics::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	
}

BOOL CNetStatistics::OnInitDialog() 
{
	CTabedDialog::OnInitDialog();
	
	CWinApp *winApp=AfxGetApp();
	m_pImageList=new CImageList();

	m_pImageList->Create(16,16,ILC_COLOR16|ILC_MASK,10,10);
 	m_pImageList->Add(winApp->LoadIcon(IDI_ICON_TRANSFER));
 	m_pImageList->Add(winApp->LoadIcon(IDI_ICON_UPLOAD));
 	m_pImageList->Add(winApp->LoadIcon(IDI_ICON_DOWNLOAD));
 	m_pImageList->Add(winApp->LoadIcon(IDI_ICON_NODE));
	m_pImageList->Add(winApp->LoadIcon(IDI_ICON_STATISTICS));
	m_pImageList->Add(winApp->LoadIcon(IDI_ICON_DAILY));
	m_pImageList->Add(winApp->LoadIcon(IDI_ICON_MONTHLY));
	m_pImageList->Add(winApp->LoadIcon(IDI_ICON_YEARLY));

	int count;
	count=m_pImageList->GetImageCount();
	TRACE(_T("%d"),count);
	m_Tree.SetImageList(m_pImageList,TVSIL_NORMAL);
//	m_Tree.SetBkColor(RGB(198,201,208));//modify by liub for modify Tree bkcolor
	
	// TODO: Add extra initialization here
	HTREEITEM ht_transfer,ht_upload,ht_download,ht_statistics,ht_daily,ht_monthly,ht_yearly;

	strTransferTime.LoadString(IDS_TREE_TRANSFERTIME);
	CString strTransferTime1;
	if(g_bDataService)
	{
		strTransferTime1=pDlgConnect->GetDurationTime();
		ConnectionType=pDlgConnect->m_connect_type;
	}
	else
		strTransferTime1=_T("00:00:00");
	strTransfer.LoadString(IDS_TREE_TRANSFER);
	strUpload.LoadString(IDS_TREE_UPLOAD);
	strDownload.LoadString(IDS_TREE_DOWNLOAD);
	strSpeed.LoadString(IDS_TREE_SPEED);
	strData.LoadString(IDS_TREE_DATA);
	strDaily.LoadString(IDS_TREE_DAILY);
	strMonthly.LoadString(IDS_TREE_MONTHLY);
	strYearly.LoadString(IDS_TREE_YEARLY);
	strStatistics.LoadString(IDS_TREE_STATISTICS);
	strUploadedData.LoadString(IDS_TREE_UPLOADED_DATA);
	strDownloadedData.LoadString(IDS_TREE_DOWNLOADED_DATA);
	strTimeLastReset.LoadString(IDS_TIME_LASTRESET);
	ht_transfer=m_Tree.InsertItem(strTransfer,0,0,NULL);
	
	ht_transferTime=m_Tree.InsertItem(strTransferTime+strTransferTime1,3,3,ht_transfer);
	ht_upload=m_Tree.InsertItem(strUpload,1,1,ht_transfer);
	ht_uploadSpeed=m_Tree.InsertItem(strSpeed,3,3,ht_upload);
	ht_uploadedData=m_Tree.InsertItem(strData,3,3,ht_upload);
	ht_download=m_Tree.InsertItem(strDownload,2,2,ht_transfer);
	ht_downloadSpeed=m_Tree.InsertItem(strSpeed,3,3,ht_download);
	ht_downloadedData=m_Tree.InsertItem(strData,3,3,ht_download);
	ht_statistics=m_Tree.InsertItem(strStatistics,4,4,NULL);
    ht_timeLastReset=m_Tree.InsertItem(strTimeLastReset+g_SetData.Internet_szLastTimeReset,3,3,ht_statistics);
	ht_daily=m_Tree.InsertItem(strDaily,5,5,ht_statistics);
	ht_dailyUpload=m_Tree.InsertItem(strUploadedData,3,3,ht_daily);
	ht_dailyDownload=m_Tree.InsertItem(strDownloadedData,3,3,ht_daily);
    ht_monthly=m_Tree.InsertItem(strMonthly,6,6,ht_statistics);
	ht_monthlyUpload=m_Tree.InsertItem(strUploadedData,3,3,ht_monthly);
	ht_monthlyDownload=m_Tree.InsertItem(strDownloadedData,3,3,ht_monthly);
	ht_yearly=m_Tree.InsertItem(strYearly,7,7,ht_statistics);
	ht_yearlyUpload=m_Tree.InsertItem(strUploadedData,3,3,ht_yearly);
	ht_yearlyDownload=m_Tree.InsertItem(strDownloadedData,3,3,ht_yearly);

	m_stThroughput=pDlgConnect->GetThroughtput(); //获得流量

	CString strDailyDownload,strDailyUpload;
	strDailyDownload.Format(_T("%u"),m_stThroughput.dailyIn/1024);
	strDailyUpload.Format(_T("%u"),m_stThroughput.dailyOut/1024);
	m_Tree.SetItemText(ht_dailyDownload,strDownloadedData+strDailyDownload+_T(" KB"));
	m_Tree.SetItemText(ht_dailyUpload,strUploadedData+strDailyUpload+_T(" KB"));
	
	CString strMonthlyDownload,strMonthlyUpload;
	strMonthlyDownload.Format(_T("%u"),m_stThroughput.monthlyIn/1024);
	strMonthlyUpload.Format(_T("%u"),m_stThroughput.monthlyOut/1024);
	m_Tree.SetItemText(ht_monthlyDownload,strDownloadedData+strMonthlyDownload+_T(" KB"));
	m_Tree.SetItemText(ht_monthlyUpload,strUploadedData+strMonthlyUpload+_T(" KB"));
	
	CString strYearlyDownload,strYearlyUpload;
	strYearlyDownload.Format(_T("%u"),m_stThroughput.yearlyIn/1024);
	strYearlyUpload.Format(_T("%u"),m_stThroughput.yearlyOut/1024);
	m_Tree.SetItemText(ht_yearlyDownload,strDownloadedData+strYearlyDownload+_T(" KB"));
	m_Tree.SetItemText(ht_yearlyUpload,strUploadedData+strYearlyUpload+_T(" KB"));
	
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CNetStatistics::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	// TODO: 在此添加消息处理程序代码和/或调用默认值
   
	CString strTransferTime1;
 	if(g_bDataService)
		strTransferTime1=pDlgConnect->GetDurationTime();
 	else
	{
		strTransferTime1=_T("00:00:00");
		return;
	}
	m_Tree.SetItemText(ht_transferTime,strTransferTime+strTransferTime1);
	CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

	CString strUpLoadSpeed;
	strUpLoadSpeed=strSpeed+_T(" ")+m_cOutgoing.CurrentTraffic;
	m_Tree.SetItemText(ht_uploadSpeed,strUpLoadSpeed);
	LONG lUpSpeed=atoi((const char *)(LPCTSTR)m_cOutgoing.CurrentTraffic)*8;
	pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_UPSPEED,(LPARAM)lUpSpeed);

	CString strDownLoadSpeed;
	strDownLoadSpeed=strSpeed+_T(" ")+m_cIncoming.CurrentTraffic;
	m_Tree.SetItemText(ht_downloadSpeed,strDownLoadSpeed);
	LONG lDownSpeed=atoi((const char *)(LPCTSTR)m_cIncoming.CurrentTraffic)*8;

	pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_DWSPEED, (LPARAM)lDownSpeed);


	CString strUpLoadedDataNow;
	strUpLoadedDataNow=strData+_T("")+m_cOutgoing.CurrentTotalTraffic;
	//strUpLoadedData=_T("Data: "+m_cOutgoing.TotalTraffic);
	m_Tree.SetItemText(ht_uploadedData,strUpLoadedDataNow);
	
	

	CString strDownLoadedDataNow;
	strDownLoadedDataNow=strData+_T("")+m_cIncoming.CurrentTotalTraffic;
//	strDownLoadedData=_T("Data: "+m_cIncoming.TotalTraffic);
	m_Tree.SetItemText(ht_downloadedData,strDownLoadedDataNow);


	m_dCurrDownTraffic+=m_cIncoming.m_dCurrTraffic;
	m_dCurrUpTraffic+=m_cOutgoing.m_dCurrTraffic;
	CString strDailyDownload,strDailyUpload;
	strDailyDownload.Format(_T("%.1f"),m_stThroughput.dailyIn/1024.0+m_cIncoming.m_dCurrTraffic);
	strDailyUpload.Format(_T("%.1f"),m_stThroughput.dailyOut/1024.0+m_cOutgoing.m_dCurrTraffic);
	m_Tree.SetItemText(ht_dailyDownload,strDownloadedData+_T(" ")+strDailyDownload+_T(" KB"));
	m_Tree.SetItemText(ht_dailyUpload,strUploadedData+_T(" ")+strDailyUpload+_T(" KB"));

	CString strMonthlyDownload,strMonthlyUpload;
	strMonthlyDownload.Format(_T("%.1f"),m_stThroughput.monthlyIn/1024.0+m_cIncoming.m_dCurrTraffic);
	strMonthlyUpload.Format(_T("%.1f"),m_stThroughput.monthlyOut/1024.0+m_cOutgoing.m_dCurrTraffic);
	m_Tree.SetItemText(ht_monthlyDownload,strDownloadedData+_T(" ")+strMonthlyDownload+_T(" KB"));
	m_Tree.SetItemText(ht_monthlyUpload,strUploadedData+_T(" ")+strMonthlyUpload+_T(" KB"));

	CString strYearlyDownload,strYearlyUpload;
	strYearlyDownload.Format(_T("%.1f"),m_stThroughput.yearlyIn/1024.0+m_cIncoming.m_dCurrTraffic);
	strYearlyUpload.Format(_T("%.1f"),m_stThroughput.yearlyOut/1024.0+m_cOutgoing.m_dCurrTraffic);
	m_Tree.SetItemText(ht_yearlyDownload,strDownloadedData+_T(" ")+strYearlyDownload+_T(" KB"));
	m_Tree.SetItemText(ht_yearlyUpload,strUploadedData+_T(" ")+strYearlyUpload+_T(" KB"));


	CDialog::OnTimer(nIDEvent);
}

void CNetStatistics::setConnection(int connectionType)
{
	ConnectionType=connectionType;
	if(ConnectionType==CONN_TYPE_RAS)
		SelectedInterface= 1;
	else if(ConnectionType==CONN_TYPE_NDIS)
		SelectedInterface= 0;


	UINT timerresolution	= 1000;	// Well, I think a refresh of the netstatus every second is enough
	UINT gridresolution		= 100;	// To fake the Taskmanager....
	
	DWORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;
	dwVersion = GetVersion();
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
// 	if(dwWindowsMajorVersion>5) //Windows vista
// 	{
		m_cTotalTraffic.m_pDlgConnect=pDlgConnect;
		m_cTotalTraffic.SetInterfaceNumber(SelectedInterface);							// Interface to monitor
		m_cTotalTraffic.SelectTrafficType(MFTrafficButton::Traffic_Total);				// In this case the total traffic is monitored
		// Set the callback handler to get informed...
		m_cTotalTraffic.SetUpdateSpeed(timerresolution, gridresolution);
		
		m_cOutgoing.m_pDlgConnect=pDlgConnect;
		m_cOutgoing.SetInterfaceNumber(SelectedInterface);
		m_cOutgoing.SelectTrafficType(MFTrafficButton::Traffic_Outgoing);
		m_cOutgoing.SetUpdateSpeed(timerresolution, gridresolution);
		
		
			m_cIncoming.m_pDlgConnect=pDlgConnect;
		m_cIncoming.SetInterfaceNumber(SelectedInterface);
		m_cIncoming.SelectTrafficType(MFTrafficButton::Traffic_Incoming);
		m_cIncoming.SetUpdateSpeed(timerresolution, gridresolution);
	
		
		m_dCurrDownTraffic=0;
		m_dCurrUpTraffic=0;
/*	}
	else    //Windows XP and others
 	{

		m_cTotalTraffic.SetInterfaceNumber(SelectedInterface);							// Interface to monitor
		m_cTotalTraffic.SelectTrafficType(MFTrafficButton::Traffic_Total);				// In this case the total traffic is monitored
		m_cTotalTraffic.SetInterfaceNumberNotificationFunction(interfaceHasChanged);	// Set the callback handler to get informed...
		m_cTotalTraffic.SetUpdateSpeed(timerresolution, gridresolution);
		
		m_cOutgoing.SetInterfaceNumber(SelectedInterface);
		m_cOutgoing.SelectTrafficType(MFTrafficButton::Traffic_Outgoing);
		m_cOutgoing.SetInterfaceNumberNotificationFunction(interfaceHasChanged);
		m_cOutgoing.SetUpdateSpeed(timerresolution, gridresolution);
		
		
		m_cIncoming.SetInterfaceNumber(SelectedInterface);
		m_cIncoming.SelectTrafficType(MFTrafficButton::Traffic_Incoming);
		m_cIncoming.SetInterfaceNumberNotificationFunction(interfaceHasChanged);
		m_cIncoming.SetUpdateSpeed(timerresolution, gridresolution);
		
		m_dCurrDownTraffic=0;
		m_dCurrUpTraffic=0;
	
	
	}
	*/
	
	SetTimer(NULL,timerresolution,0);  


}

void CALLBACK interfaceHasChanged(int interfacenumber)
{
	SelectedInterface = interfacenumber;
	CString text;
	text.Format(_T("NetTrafficButton using Interface: %d"),SelectedInterface);
}


void CNetStatistics::setDrawFlag(BOOL flag)
{
	m_cTotalTraffic.setDrawFlag(flag);
	m_cIncoming.setDrawFlag(flag);
	m_cOutgoing.setDrawFlag(flag);
}

void CNetStatistics::OnRightClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
//	POINT myPoint;
	
    CMenu myMenu;
    CString strMenuName;
    myMenu.CreatePopupMenu();    
    strMenuName.LoadString(IDS_NETTREE_EXPANDALL);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_EXPANDALL,strMenuName);
	
    strMenuName.LoadString(IDS_NETTREE_COLLAPSEALL);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_COLLAPSEALL,strMenuName);
	
	strMenuName.LoadString(IDS_NETTREE_RESETSTATISTICS);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_RESETSTATISTICS,strMenuName);
    
	strMenuName.LoadString(IDS_NETTREE_NETLOG);
    myMenu.AppendMenu(MF_ENABLED,IDC_BUTTON_NETLOG,strMenuName);

    
	POINT pos ;
	if(!GetCursorPos(&pos))
		return ;
	m_Tree.ScreenToClient(&pos);
	

	UINT uFlags;
	HTREEITEM hItem = m_Tree.HitTest(pos, &uFlags);
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		m_Tree.Select(hItem, TVGN_CARET);
	}
	
	// 显示菜单
	CRect rect ;
	this->GetClientRect(&rect) ;
	ClientToScreen(&rect) ;
	m_Tree.ClientToScreen(&pos);
	
	int iCode= rect.PtInRect(pos) ;
	
	CMenu m_Menu,*p_Menu=NULL ;
	if(iCode)
	{
		myMenu.TrackPopupMenu( TPM_RIGHTBUTTON|TPM_LEFTALIGN, pos.x, pos.y, this);
	}
	*pResult = 0;
}

void CNetStatistics::OnButtonExpandAll()
{
	HTREEITEM   hitem;
	hitem=m_Tree.GetRootItem();
	do 
	{
		ExpandBranch(hitem);
	} while((hitem=m_Tree.GetNextSiblingItem(hitem))!=NULL);
	
//	EnsureVisible( GetSelectedItem() );

}

void CNetStatistics::ExpandBranch(HTREEITEM   hitem)
{

	if( m_Tree.ItemHasChildren( hitem) ){
		m_Tree.Expand( hitem, TVE_EXPAND );
		hitem = m_Tree.GetChildItem( hitem );
		do{
			ExpandBranch( hitem );
		}while( (hitem = m_Tree.GetNextSiblingItem(hitem )) != NULL );
	}
}

void CNetStatistics::OnButtonCollapseAll()
{
	HTREEITEM hitem = m_Tree.GetRootItem();
	do{
		CollapseBranch( hitem );
	}while( (hitem = m_Tree.GetNextSiblingItem( hitem )) != NULL );

}

void CNetStatistics::CollapseBranch(HTREEITEM   hitem)
{
	do{
		m_Tree.Expand( hitem,TVE_COLLAPSE );
	}while( (hitem = m_Tree.GetNextSiblingItem( hitem )) != NULL );

}

void CNetStatistics::OnButtonResetStatistics()
{
	CString strResetWarning;
	strResetWarning.LoadString(IDS_WARNING_RESETSTATISTICS);
	if(AfxMessageBox(strResetWarning,MB_YESNO)==IDYES)
	{
		CString strDailyDownload,strDailyUpload;
		strDailyDownload.Format(_T("%.1f"),0.0);
		strDailyUpload.Format(_T("%.1f"),0.0);
		m_Tree.SetItemText(ht_dailyDownload,strDownloadedData+strDailyDownload+_T(" KB"));
		m_Tree.SetItemText(ht_dailyUpload,strUploadedData+strDailyUpload+_T(" KB"));
		
		CString strMonthlyDownload,strMonthlyUpload;
		strMonthlyDownload.Format(_T("%.1f"),0.0);
		strMonthlyUpload.Format(_T("%.1f"),0.0);
		m_Tree.SetItemText(ht_monthlyDownload,strDownloadedData+strMonthlyDownload+_T(" KB"));
		m_Tree.SetItemText(ht_monthlyUpload,strUploadedData+strMonthlyUpload+_T(" KB"));
		
		CString strYearlyDownload,strYearlyUpload;
		strYearlyDownload.Format(_T("%.1f"),0.0);
		strYearlyUpload.Format(_T("%.1f"),0.0);
		m_Tree.SetItemText(ht_yearlyDownload,strDownloadedData+strYearlyDownload+_T(" KB"));
		m_Tree.SetItemText(ht_yearlyUpload,strUploadedData+strYearlyUpload+_T(" KB"));

		CTime currentTime;
		currentTime=CTime::GetCurrentTime();
		CString strCurrentTime;
		int nYear,nMonth,nDay,nHour,nMinute,nSecond;
		nYear=currentTime.GetYear();
		nMonth=currentTime.GetMonth();
		nDay=currentTime.GetDay();
		nHour=currentTime.GetHour();
		nMinute=currentTime.GetMinute();
		nSecond=currentTime.GetSecond();
		CString strYear,strMonth,strDay,strHour,strMinute,strSecond;
		if(nMonth<10)
			strMonth.Format(_T("0%d"),nMonth);
		else
			strMonth.Format(_T("%d"),nMonth);

		if(nDay<10)
			strDay.Format(_T("0%d"),nDay);
		else
			strDay.Format(_T("%d"),nDay);

		if(nHour<10)
			strHour.Format(_T("0%d"),nHour);
		else
			strHour.Format(_T("%d"),nHour);

		if(nMinute<10)
			strMinute.Format(_T("0%d"),nMinute);
		else
			strMinute.Format(_T("%d"),nMinute);

		if(nSecond<10)
			strSecond.Format(_T("0%d"),nSecond);
		else
			strSecond.Format(_T("%d"),nSecond);

		strYear.Format(_T("%d"),nYear);

		strCurrentTime=strMonth+_T("/")+strDay+_T("/")+strYear+_T(" ")+strHour+_T(":")
						+strMinute+_T(":")+strSecond;
		m_Tree.SetItemText(ht_timeLastReset,strTimeLastReset+strCurrentTime);
		CLogData* LogData;
		LogData = ((CHSDPAApp *)AfxGetApp())->GetLogData();
		LogData->ClearAll();
		WritePrivateProfileString(_T("Internet"),
			_T("Internet_szLastTimeReset"),
			strCurrentTime,
			szInifilename);
		m_stThroughput.dailyIn=0.0;
		m_stThroughput.dailyOut=0.0;
		m_stThroughput.monthlyIn=0.0;
		m_stThroughput.monthlyOut=0.0;
		m_stThroughput.yearlyIn=0.0;
		m_stThroughput.yearlyOut=0.0;
		

	}


}


void CNetStatistics::OnButtonNetLog()
{
	CNetLog netDlg(this);
    netDlg.DoModal();
}

void CNetStatistics::OnResetStatistics(WPARAM wParam, LPARAM lParam)
{
	OnButtonResetStatistics();
}