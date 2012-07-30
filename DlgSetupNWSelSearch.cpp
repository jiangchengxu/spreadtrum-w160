// DlgSetupNWSelSearch.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "HSDPADlg.h"
//#include "DlgSetupNWSel.h"
#include "DlgSetupNWSelSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupNWSelSearch dialog


CDlgSetupNWSelSearch::CDlgSetupNWSelSearch(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSetupNWSelSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetupNWSelSearch)
		// NOTE: the ClassWizard will add member initialization here
	pstNetWork = NULL;
	m_nSelMode = 0;
	m_nBkTag = 1;
	//}}AFX_DATA_INIT

	//wyw
	pdlgMsg = NULL;
}


void CDlgSetupNWSelSearch::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetupNWSelSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_NWSEL_SEARCH_NWLIST, m_NWListSearch);
	DDX_Radio(pDX, IDC_RADIO_NWSEL_SEARCH_AUTOSEL, m_nSelMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetupNWSelSearch, CBaseDialog)
	//{{AFX_MSG_MAP(CDlgSetupNWSelSearch)
		// NOTE: the ClassWizard will add message map macros here
		ON_BN_CLICKED(IDC_BUTTON_NWSEL_SEARCH_SELECT, OnButtonNwselSearchSelect)
		ON_BN_CLICKED(IDC_BUTTON_NWSEL_SEARCH_REFRESH, OnButtonNwselSearchRefresh)
		ON_MESSAGE(WM_SETUP_UPDATE_NWSELDLG, OnUpdateNWSelSearchDlg)
		ON_BN_CLICKED(IDC_RADIO_NWSEL_SEARCH_AUTOSEL, OnRadioNwselSearchAutosel)
		ON_BN_CLICKED(IDC_RADIO_NWSEL_SEARCH_MANSEL, OnRadioNwselSearchMansel)
		ON_WM_TIMER()
		ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupNWSelSearch message handlers

//select newwork
BOOL CDlgSetupNWSelSearch::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	

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
	//m_NWListSearch.SetParent(this);/
	m_NWListSearch.SetParent(this);
    //CRect rect;
    //m_tabSms.GetItemRect(0, &rect);
	
    //m_lstNW.SetWindowPos(NULL, 4, 28, rect.right, rect.bottom, TRUE);
    //m_lstNW.GetWindowRect(&rect);
    CString strColTitle;
    //设置列名
    for(int i=0; i<2; i++)
    {
        strColTitle.LoadString(arr_cols[i]);
        m_NWListSearch.InsertColumn(i, strColTitle, LVCFMT_LEFT, arr_width[i]);
    }
	
	//ONDA del IDS_NWLIST_NUMBER
	if (g_SetData.Main_nCompany==COMP_ONDA)
	{
		strColTitle.LoadString(arr_cols[3]);
		m_NWListSearch.InsertColumn(2, strColTitle, LVCFMT_LEFT, arr_width[3]);
	}
	else
	{
		strColTitle.LoadString(arr_cols[2]);
		m_NWListSearch.InsertColumn(2, strColTitle, LVCFMT_LEFT, arr_width[2]);
		strColTitle.LoadString(arr_cols[3]);
		m_NWListSearch.InsertColumn(3, strColTitle, LVCFMT_LEFT, arr_width[3]);
	}
	
    m_NWListSearch.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_NWListSearch.m_headerCtrl.DrawHeader();

	m_nMsgBeat=0;
	pdlgMsg = new CDlgMsg(this);

	return true;

}

//wyw
void CDlgSetupNWSelSearch::OnDestroy() 
{
	if (pdlgMsg)
	{
		delete pdlgMsg;
		pdlgMsg = NULL;
	}
}

void CDlgSetupNWSelSearch::OnButtonNwselSearchSelect()
{
	int nCurSel;
	
	CString strCmd;
	if (m_nSelMode==0)
	{
		//Auto Select
		//send cops=0
		nCOPSType=0;
		//strCmd.Format("=1,0,\"%s\"\r", pstNetWork[nCurSel].szOperLong);
		strCmd="=0\r";
	} 
	else if(m_nSelMode==1)
	{
		//Manual Select
		if(!nNWnumber)
			return;
		else if((nCurSel = m_NWListSearch.GetNextItem(-1, LVNI_SELECTED)) == -1)
		{
			AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
			return;
		}
		//send cops=select
		nCOPSType=0;
		
        //Manual select network in numeric format
        CString strNWNum = m_NWListSearch.GetItemText(nCurSel, 2);
		strCmd.Format(_T("=1,2,\"%s\""), strNWNum);
	}
	
	if (SndAtCOPS(strCmd))
	{
		GetDlgItem(IDC_RADIO_NWSEL_SEARCH_AUTOSEL)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_RADIO_NWSEL_SEARCH_MANSEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_NWSEL_SEARCH_NWLIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_REFRESH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_SELECT)->EnableWindow(FALSE);
		pdlgMsg->m_strMsg.LoadString(IDS_SEARCH_NWLIST);
		pdlgMsg->DoModal();
		if(bSetNWSucc)
		{
			GetCurAutoPrefer();
			
			
			//OnButtonSetupNwselRefresh();
			CString strSetsucc;
			strSetsucc.Format(IDS_SETUP_COPS_SETSUCC);
			this->MessageBox(strSetsucc);
		}
		else
		{
			CString strSetsucc;
			strSetsucc.Format(IDS_SETUP_COPS_SETERROR);
			this->MessageBox(strSetsucc);
		}
		
	}


}

void CDlgSetupNWSelSearch::OnButtonNwselSearchRefresh()
{
	// TODO: Add your control notification handler code here
	
	//TODO: send AT+COPS=? to get network list
	//first: set PHPREF=0
	
	//	if(SndAtPHPREF("0"))
	if (SndAtCOPS(_T("=?")))
	{		
		//TODO: disable all button
		GetDlgItem(IDC_RADIO_NWSEL_SEARCH_AUTOSEL)->EnableWindow(FALSE);
		//GetDlgItem(IDC_STATIC_MODEPRE)->EnableWindow(FALSE);
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSPRE)->EnableWindow(FALSE);
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEPRE)->EnableWindow(FALSE);
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSONLY)->EnableWindow(FALSE);
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEONLY)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SET)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_RADIO_NWSEL_SEARCH_MANSEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_NWSEL_SEARCH_NWLIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_REFRESH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_SELECT)->EnableWindow(FALSE);
		
		//nSetPREFOK=1;
		nCOPSType=2;
		{
			pdlgMsg->m_strMsg.LoadString(IDS_REFRESH_NWLIST);
			pdlgMsg->DoModal();
		}		
	}

}

void CDlgSetupNWSelSearch::OnRadioNwselSearchAutosel()
{
	GetDlgItem(IDC_LIST_NWSEL_SEARCH_NWLIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_REFRESH)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SELECT)->EnableWindow(FALSE);
	m_NWListSearch.DeleteAllItems();
	m_nSelMode=0;
	


}

void CDlgSetupNWSelSearch::OnRadioNwselSearchMansel()
{
	GetDlgItem(IDC_LIST_NWSEL_SEARCH_NWLIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_REFRESH)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SELECT)->EnableWindow(TRUE);
	m_nSelMode=1;

}

int CDlgSetupNWSelSearch::AnalyseNWList(CString strNWList)
{
	int nNum=0;
	int nHead, nTail;
	CString strNetwork;
	int i;
	
	int arr_stat[]=
	{
		IDS_NWSTAT_UNKNOWN,
			IDS_NWSTAT_AVAILABLE,
			IDS_NWSTAT_CURRENT,
			IDS_NWSTAT_FORBIDDEN,
	};
	
	int arr_act[]=
	{
		IDS_NWACT_GSM,
			IDS_NWACT_GSMCOM,
			IDS_NWACT_UTRAN,
	};
	
	m_NWListSearch.DeleteAllItems();
	
	for (i=0; i<strNWList.GetLength(); i++)
	{
		if (strNWList[i]=='(')
		{
			nNum++;
		}
	}
	nNWnumber = nNum;
	if (nNum==0)
	{
		return nNum;
	}
	
	pstNetWork=new StNetWork[nNum];
	memset(pstNetWork, 0x00, sizeof(StNetWork)*nNum);
	
	for (i=0; i<nNum; i++)
	{
		nHead=strNWList.Find(_T("("),0);
		nTail = strNWList.Find(_T(")"),nHead);
		nHead=nTail;
		if (nTail!=-1)
		{
			strNWList.TrimLeft(_T("("));
			strNetwork=strNWList.Left(nTail-1);
			//strNetwork="2,"China Mobile Com","DGT MPT","46000",0";
			
			nTail=strNetwork.Find(_T(","), 0);
			
			pstNetWork[i].enNWStat=(EnNWStat)(strNetwork[0]-48);//网络状态
			
			strNetwork.Delete(0, nTail+2);
			nTail=strNetwork.Find(_T("\""), 0);
			wcscpy((TCHAR*)pstNetWork[i].szOperLong, strNetwork.Left(nTail));
			
			strNetwork.Delete(0, nTail+3);
			nTail=strNetwork.Find(_T("\""), 0);
			wcscpy((TCHAR*)pstNetWork[i].szOperShort, strNetwork.Left(nTail));
			
			strNetwork.Delete(0, nTail+3);
			nTail=strNetwork.Find(_T("\""), 0);
			pstNetWork[i].nOperNum=_wtoi(strNetwork.Left(nTail));
			
			strNetwork.Delete(0, nTail+2);
			pstNetWork[i].enNWAct=(EnNWACT)(strNetwork[0]-48);
			
			m_NWListSearch.InsertItem(i, _T(""));
			CString strStat, strNum, strAct;
			strStat.LoadString(arr_stat[pstNetWork[i].enNWStat]);
			m_NWListSearch.SetItemText(i, 0, strStat);
			//使用短名称
			//m_lstNW.SetItemText(i, 1, (const char *)pstNetWork[i].szOperLong);
			m_NWListSearch.SetItemText(i, 1, (const TCHAR *)pstNetWork[i].szOperShort);
			
			//ONDA del nOperNum
			if (g_SetData.Main_nCompany==COMP_ONDA)
			{
				strAct.LoadString(arr_act[pstNetWork[i].enNWAct]);
				m_NWListSearch.SetItemText(i, 2, strAct);
			} 
			else
			{
				strNum.Format(_T("%d"), pstNetWork[i].nOperNum);
				m_NWListSearch.SetItemText(i, 2, strNum);
				strAct.LoadString(arr_act[pstNetWork[i].enNWAct]);
				m_NWListSearch.SetItemText(i, 3, strAct);
				
			}
		}
		strNWList.Delete(0, nHead+1);
	}
	delete []pstNetWork;
	return nNum;
}

void CDlgSetupNWSelSearch::RspAtCOPS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgSetupNWSelSearch* pDlg = (CDlgSetupNWSelSearch*)pWnd;	
    pDlg->KillTimer(IDT_COPS_TIMEOUT);
	
/*	CString RetStr=(char*)strArr[0];*/
	CString RetStr;
	RetStr = strArr[0];
	int nHead, nTail;
	
	BOOL bResult=FALSE;
	
	switch(pDlg->nCOPSType)
	{
	case 0:
		{
			//cops set handle  
			if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CME_ERROR][gc_dsatmode]) == 0)
			{
				//set fail
				AfxMessageBox(IDS_SETUP_COPS_SETERROR);
			}
			else
			{
				//set success
				//AfxMessageBox("OK");
			}
			break;
		}
	
	case 1:
		//COPS? handle
		break;
	case 2:
		//COPS=? handle
		int nErrorLen = strlen(gc_dsatResCodeTbl[DSAT_CME_ERROR][gc_dsatmode]);
		if(_strnicmp((const char*)strArr[0], 
			(const char*)gc_dsatResCodeTbl[DSAT_CME_ERROR][gc_dsatmode],
			nErrorLen) == 0)
		{	
			//TODO: ERROR handle	
			AfxMessageBox(IDS_SETUP_COPS_ERROR);
		}
		else
		{
			//success
			CString NWList;
			nHead=RetStr.Find(_T("("),0);
			if (nHead!=-1)
			{
				nTail=RetStr.Find(_T(",,"),nHead+1);
				if (nTail!=-1)
				{
					RetStr=(char*)strArr[0]+nHead;
					NWList=RetStr.Left(nTail-nHead);
					bResult=TRUE;
				}
			} 
			else
			{
			}
			pDlg->AnalyseNWList(NWList);
		}
		break;
		}
		pDlg->PostMessage(WM_SETUP_UPDATE_NWSELDLG, (WPARAM)(pDlg->nCOPSType), (LPARAM)TRUE);
		pDlg->nCOPSType=-1;
		pDlg->nSetPREFOK=0;

}

BOOL CDlgSetupNWSelSearch::SndAtCOPS(CString strCOPS)
{
    char szAtBuf[50] = {0};
	char *szbuf = new char[_tcslen(strCOPS)  + 1];
	memset(szbuf, 0, _tcslen(strCOPS)  + 1);
	WCharToChar(strCOPS, szbuf);
	sprintf(szAtBuf, "AT+COPS%s\r", szbuf);
	
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        SetTimer(IDT_COPS_TIMEOUT, 30000, NULL);
		SetTimer(IDT_COPS_MSG, 500, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtCOPS, this);
        return TRUE;
    }
    return FALSE;
}

void CDlgSetupNWSelSearch::GetCurAutoPrefer()
{
	CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
	pMainDlg->AtSndSYSCONFIG();
	m_nAutoMode = gCurAutoPref;
    UpdateData(FALSE);
	return;
}

void CDlgSetupNWSelSearch::ManualUpdateIcon() 
{
	CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
	pMainDlg->m_SrvStatus = (EnSrvStatus)(1);
	
	pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN2, pMainDlg->m_SrvStatus);
	pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NETWORK, pMainDlg->m_NetworkType);
	pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NWSRV, pMainDlg->m_nNwSrv);//NETWORK SERVICE
	pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 1);  //PLMN
	pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PS, pMainDlg->m_bPS);  //PS
	pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_ROAM, pMainDlg->m_RoamStatus);//ROAM
	return;
}

void CDlgSetupNWSelSearch::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    if(nIDEvent == IDT_COPS_MSG)
    {
		CString strTemp;
		CString strMsg;
		UINT nLen;
		
        if(nCOPSType==0)
            strTemp.LoadString(IDS_SEARCH_NWLIST);
        else
			strTemp.LoadString(IDS_REFRESH_NWLIST);
		
		nLen=strTemp.GetLength();
		strMsg=strTemp.Left(nLen-6+m_nMsgBeat);
		//OutputDebugString(strMsg);
		pdlgMsg->m_strMsg=strMsg;
		m_nMsgBeat++;
		if(m_nMsgBeat>6)
		{
			m_nMsgBeat=0;
		}
		if (pdlgMsg->m_hWnd)
		{
			pdlgMsg->UpdateData(FALSE);
		}
    }
    else if(nIDEvent == IDT_COPS_TIMEOUT)
    {
        KillTimer(IDT_COPS_TIMEOUT);
		bSetNWSucc = false;
        DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
        ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
		PostMessage(WM_SETUP_UPDATE_NWSELDLG, (WPARAM)(nCOPSType), (LPARAM)TRUE);		
    }
	CBaseDialog::OnTimer(nIDEvent);
}

LRESULT CDlgSetupNWSelSearch::OnUpdateNWSelSearchDlg(WPARAM wParam, LPARAM lParam)
{
    KillTimer(IDT_COPS_MSG);
    if(pdlgMsg->m_hWnd)
        pdlgMsg->EndDialog(0);
    m_nMsgBeat=0;
	switch((int)wParam)
	{
	case 0:	
		if (m_nSelMode==0)
		{
			GetDlgItem(IDC_RADIO_NWSEL_SEARCH_AUTOSEL)->EnableWindow(TRUE);
			
			GetDlgItem(IDC_RADIO_NWSEL_SEARCH_MANSEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_SELECT)->EnableWindow(TRUE);
		} 
		else if(m_nSelMode==1)
		{
			GetDlgItem(IDC_RADIO_NWSEL_SEARCH_AUTOSEL)->EnableWindow(TRUE);
			
			GetDlgItem(IDC_RADIO_NWSEL_SEARCH_MANSEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_LIST_NWSEL_SEARCH_NWLIST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_REFRESH)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_SELECT)->EnableWindow(TRUE);
		}
		break;
	case 2:
		GetDlgItem(IDC_RADIO_NWSEL_SEARCH_AUTOSEL)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_RADIO_NWSEL_SEARCH_MANSEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_NWSEL_SEARCH_NWLIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_REFRESH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NWSEL_SEARCH_SELECT)->EnableWindow(TRUE);
		break;
	case 3:
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_AUTOSEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MODEPRE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSPRE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEPRE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSONLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEONLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SET)->EnableWindow(TRUE);
        if(lParam)
			AfxMessageBox(IDS_SETOK, MB_OK | MB_ICONINFORMATION);
        else
            AfxMessageBox(IDS_SETUP_NWAUTOSETERROR);
		
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_MANSEL)->EnableWindow(TRUE);
		break;
	}
	return TRUE;
}
