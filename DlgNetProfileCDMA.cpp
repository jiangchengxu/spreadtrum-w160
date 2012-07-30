// DlgNetProfileCDMA.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "resource.h"
#include "Common_DataCrad\Common_DataCrad.h"
#include "DlgNetProfileCDMA.h"
#include "DlgInternetNewConn.h"
#include "DataServiceHistory.h"
#include "DlgNetProfileNewCDMA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNetProfileCDMA dialog


CDlgNetProfileCDMA::CDlgNetProfileCDMA(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgNetProfileCDMA::IDD, pParent)
{
	m_nBkTag = 1;
	
	m_strPassword = _T("");
	m_strUserName = _T("");
	m_strPhoneNumber = _T("");
	m_nAuthRadio = 0;
	
	BApnSet = FALSE;
	m_pConnProf = ((CHSDPAApp *)AfxGetApp())->GetConnProfile();
    m_pNdisConnProf = ((CHSDPAApp *)AfxGetApp())->GetNdisConnProfile();
	// #ifdef FEATURE_NDIS_SUPPORT
	// 	m_connect_type = CONN_TYPE_NDIS;
	// #endif
	m_connect_type = CONN_TYPE_RAS;
}


void CDlgNetProfileCDMA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_PHONE_NUMBER, m_NumEditPhoneNumber);
    DDX_Text(pDX, IDC_EDIT_PHONE_NUMBER, m_strPhoneNumber);
    DDV_MaxChars(pDX, m_strPhoneNumber, RAS_MaxPhoneNumber);
    DDX_Text(pDX, IDC_EDIT_USER_NAME, m_strUserName);
    DDV_MaxChars(pDX, m_strUserName, UNLEN);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
    DDV_MaxChars(pDX, m_strPassword, PWLEN);
   
    DDX_Check(pDX, IDC_CHECK_SETDEFAULT, m_bSetDefault);
    DDX_Control(pDX, IDC_COMBO_CONNECT_NAME, m_cmbConnectionName);
//	DDX_Radio(pDX, IDC_RADIO_CHAP, m_nAuthRadio);
	
	//#ifdef FEATURE_NDIS_SUPPORT
//	DDX_Radio(pDX, IDC_RADIO_NDIS, m_connect_type);
//#endif 
}


BEGIN_MESSAGE_MAP(CDlgNetProfileCDMA, CBaseDialog)
ON_BN_CLICKED(IDC_BUTTON_DETAILS, OnButtonDetails)
ON_CBN_SELCHANGE(IDC_COMBO_CONNECT_NAME, OnSelchangeComboConnectName)
ON_BN_CLICKED(IDC_BUTTON_SAVECONN, OnButtonSaveconn)
ON_BN_CLICKED(IDC_BUTTON_NEWCONN, OnButtonNewconn)
ON_BN_CLICKED(IDC_BUTTON_DELETECONN, OnButtonDeleteconn)
ON_MESSAGE(WM_SETUP_UPDATE_INTERNETDLG, OnUpdateInternetDlg)
//}}AFX_MSG_MAP
/*ndis support*/
#ifdef FEATURE_NDIS_SUPPORT
ON_BN_CLICKED(IDC_RADIO_NDIS, OnRadioNdis)
ON_BN_CLICKED(IDC_RADIO_RAS, OnRadioRas)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNetProfileCDMA message handlers
BOOL CDlgNetProfileCDMA::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
// 	m_bFirst = TRUE;
// 	SetSkin(g_SetData.Setup_nSkinStyle);

	OnRadioRas();
	if (0 == m_connect_type) { 
		OnRadioNdis();
	}
	if (1 == m_connect_type) {
		OnRadioRas();
	}
	if(g_SetData.Setup_nNDIS==1)  //NDIS feature supported
	{
		GetDlgItem(IDC_RADIO_NDIS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_RAS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
	}
	m_NumEditPhoneNumber.m_bFlag  = true;
	
	m_file.Open(_T("NetDebug.log"),CFile::modeCreate | CFile::modeReadWrite);
	m_file.Close();
	return true;
}

BOOL CDlgNetProfileCDMA::OnPaint()
{
	CBaseDialog::OnPaint();
}

void CDlgNetProfileCDMA::OnSelchangeComboConnectName()
{
    // TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
    StConnProfile profile;
    int nIndex = m_cmbConnectionName.GetCurSel();
	
	BOOL bClear = FALSE;
	if(nIndex == CB_ERR)
	{
		bClear = TRUE;
	}
	if (0 == m_connect_type && (nIndex >= m_pNdisConnProf->GetConnNum())) 
	{
		bClear = TRUE;
	} 
	if (1 == m_connect_type && (nIndex >= m_pConnProf->GetConnNum())) 
	{
		bClear = TRUE;
	} 
	
	if (bClear) {
		m_bSetDefault = FALSE;
		m_strPassword = _T("");
		m_strUserName = _T("");
		m_strPhoneNumber =_T("");
	//	m_strAPN = "";
		m_nAuthRadio = 0;
	}
	else
	{
		memset(&profile, 0x00, sizeof(StConnProfile));
		if (0 == m_connect_type) {
			profile = m_pNdisConnProf->ReadConnProfile(nIndex);
		} 
		else if (1 == m_connect_type) {
			profile = m_pConnProf->ReadConnProfile(nIndex);
		}
		else {
			ASSERT(TRUE);
		}
		m_bSetDefault = profile.bDefault;
		m_nAuthRadio = (int)profile.AuthProtocol;
        m_strPhoneNumber.Format(_T("%s"), profile.szPhoneNumber);
        m_strUserName.Format(_T("%s"), profile.szUserName);
        m_strPassword.Format(_T("%s"), profile.szPassword);
        //m_strAPN.Format(_T("%s"), profile.szAPN);
	}
	
    UpdateData(FALSE);
	
	if (0 == m_connect_type) {
	//	OnUpdateInternetDlg(!(nIndex == 0));
		
		GetDlgItem(IDC_EDIT_PHONE_NUMBER)->EnableWindow(FALSE);
	} 
	else if (1 == m_connect_type) {
	//	OnUpdateInternetDlg(!(nIndex == 0));
	} else 
		ASSERT(TRUE);
}

void CDlgNetProfileCDMA::OnButtonSaveconn()
{
    // TODO: Add your control notification handler code here

	UpdateData(TRUE);

    int nIndex = m_cmbConnectionName.GetCurSel();

	if (0 == m_connect_type) {
		if(nIndex == CB_ERR || nIndex >= m_pNdisConnProf->GetConnNum())
		{
			return;
		}
	} else	if (1 == m_connect_type) {
		if(nIndex == CB_ERR || nIndex >= m_pConnProf->GetConnNum())
		{
			return;
		}
	} else
		ASSERT(TRUE);

#if defined(FEATURE_VERSION_ITELCO) || defined(FEATURE_VERSION_TELCEL) 
	CString strName = _T("");
	m_cmbConnectionName.GetLBText(nIndex, strName);
	if((nIndex == 0)&&(!strName.Compare(g_SetData.Internet_szDefaultConnName)))
	{
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strDelErr;
		strDelErr.LoadString(IDS_INTERNET_EDITERR);
		this->MessageBox(strDelErr,strAppName,MB_OK);
		UpdateData(FALSE);
		return;
	}
#endif    

	UpdateData(TRUE);
    
	if (0 == m_connect_type) {
		if((m_strUserName.GetLength() > UNLEN)
			|| (m_strPassword.GetLength() > PWLEN)
			/*|| (m_strAPN.GetLength() > CONNECTION_APN_LEN)*/)
		{
			AfxMessageBox(IDS_INT_ERR_INPUTERROR);
			return;
		}
	}
	else if (1 == m_connect_type) 
	{
		if(m_strPhoneNumber.GetLength() == 0)
		{
			AfxMessageBox(IDS_INT_ERR_INPUTNULL);
			return;
		}
		if((m_strPhoneNumber.GetLength() > RAS_MaxPhoneNumber)
			|| (m_strUserName.GetLength() > UNLEN)
			|| (m_strPassword.GetLength() > PWLEN)
		/*	|| (m_strAPN.GetLength() > CONNECTION_APN_LEN)*/)
		{
			AfxMessageBox(IDS_INT_ERR_INPUTERROR);
			return;
		}
	} else {
		ASSERT(TRUE);
	}
    
    StConnProfile profile;
    
//     if(wcscmp(m_strLastAPN, m_strAPN) != 0)
//     {		
//         if(SndAtCGDCONT(m_strAPN))
//         {
//             //GetDlgItem(IDC_EDIT_APN)->EnableWindow(FALSE);
//             //GetDlgItem(IDC_BUTTON_SAVECONN)->EnableWindow(FALSE);
//             nCGDCONTType = 0;
//         }
//     }

    memset(&profile, 0x00, sizeof(StConnProfile));
	if (0 == m_connect_type) {
		profile = m_pNdisConnProf->ReadConnProfile(nIndex);
	} else if (1 == m_connect_type) {
		profile = m_pConnProf->ReadConnProfile(nIndex);
		wcscpy(profile.szPhoneNumber, m_strPhoneNumber);
	} else {
		ASSERT(TRUE);
	}

    profile.bDefault = m_bSetDefault;
    profile.AuthProtocol = (EnAuthProtocol)m_nAuthRadio;
    wcscpy(profile.szUserName, m_strUserName);
    wcscpy(profile.szPassword, m_strPassword);
    //wcscpy(profile.szAPN, m_strAPN);

	if (0 == m_connect_type) {
		m_pNdisConnProf->EditConnProfile(nIndex, profile);
	} 
	else if (1 == m_connect_type) {
		m_pConnProf->EditConnProfile(nIndex, profile);
	} else {
		ASSERT(TRUE);
	}
	CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
	pMainDlg->m_pConnectDlg->getConnProfile();
	pMainDlg->m_pConnectDlg->resetConnProfile();

    AfxMessageBox(IDS_SAVEOK, MB_ICONINFORMATION);
	OnCancel();
}

void CDlgNetProfileCDMA::OnButtonNewconn()
{
    // TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL bFull = FALSE;
	if (0 == m_connect_type) {
		bFull = m_pNdisConnProf->IsFull();
	}
	else if (1 == m_connect_type) {
		bFull = m_pConnProf->IsFull();
	}
	else {
		ASSERT(TRUE);
	}
	if (bFull)
    {
        CString strMsg;
        //CString strConnLimite;
		strMsg.Format(IDS_INT_CONNLIMITEFORMAT, CONNECTION_PROFILE_MAX);
        //strConnLimite.LoadString(IDS_INT_CONNLIMITEFORMAT);
        //strMsg.Format(strConnLimite, CONNECTION_PROFILE_MAX);
        MessageBox(strMsg);
        return;
    }

	if (0 == m_connect_type) {
		CDlgNetProfileNewCDMA dlg(NULL, 0);
		if(dlg.DoModal() == IDOK)
		{
			StConnProfile profile;
			memset(&profile, 0x00, sizeof(StConnProfile));
			profile.bDefault = dlg.m_bSetDefault;
			profile.AuthProtocol = (EnAuthProtocol)dlg.m_nAuthRadio;
			wcscpy(profile.szEntryName, dlg.m_strConnName);
			//strcpy(profile.szPhoneNumber, dlg.m_strPhoneNumber);
			wcscpy(profile.szUserName, dlg.m_strUserName);
			wcscpy(profile.szPassword, dlg.m_strPassWord);
			profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
			m_pNdisConnProf->AddConnProfile(profile);
			m_cmbConnectionName.AddString(dlg.m_strConnName);
			m_cmbConnectionName.SetCurSel(m_cmbConnectionName.GetCount()-1);
			OnSelchangeComboConnectName();
		}
	}
	else if (1 == m_connect_type) {
		CDlgNetProfileNewCDMA dlg(NULL, 1);
		if(dlg.DoModal() == IDOK)
		{
			StConnProfile profile;
			memset(&profile, 0x00, sizeof(StConnProfile));
			profile.bDefault = dlg.m_bSetDefault;
			profile.AuthProtocol = (EnAuthProtocol)dlg.m_nAuthRadio;
        wcscpy(profile.szEntryName, dlg.m_strConnName);
        wcscpy(profile.szPhoneNumber, dlg.m_strPhoneNumber);
        wcscpy(profile.szUserName, dlg.m_strUserName);
        wcscpy(profile.szPassword, dlg.m_strPassWord);
			profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
			m_pConnProf->AddConnProfile(profile);
			m_cmbConnectionName.AddString(dlg.m_strConnName);
			m_cmbConnectionName.SetCurSel(m_cmbConnectionName.GetCount()-1);
			OnSelchangeComboConnectName();
		}
	}
	else {
		ASSERT(TRUE);
	}
	CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
	pMainDlg->m_pConnectDlg->getConnProfile();
	pMainDlg->m_pConnectDlg->resetConnProfile();
}

void CDlgNetProfileCDMA::OnButtonDeleteconn()
{
	UpdateData(TRUE);
    int nIndex = m_cmbConnectionName.GetCurSel();

#if defined(FEATURE_VERSION_ITELCO) || defined(FEATURE_VERSION_TELCEL)
	CString strName = _T("");
	m_cmbConnectionName.GetLBText(nIndex, strName);
	if((nIndex == 0)&&(!strName.Compare(g_SetData.Internet_szDefaultConnName)))
	{
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strDelErr;
		strDelErr.LoadString(IDS_INTERNET_DELERR);
		this->MessageBox(strDelErr,strAppName,MB_OK);
		return;
	}
#endif  
    
	if (0 == m_connect_type) {
		if(nIndex == CB_ERR || nIndex >= m_pNdisConnProf->GetConnNum())
		{
			return;
		}
	} else if (1 == m_connect_type) {
		if(nIndex == CB_ERR || nIndex >= m_pConnProf->GetConnNum())
		{
			return;
		}
	}

    StConnProfile profile;
    memset(&profile, 0x00, sizeof(StConnProfile));

	if (0 == m_connect_type) {
		profile = m_pNdisConnProf->ReadConnProfile(nIndex);
	} 
	else if (1 == m_connect_type) {
		profile = m_pConnProf->ReadConnProfile(nIndex);
	}
	else {
		ASSERT(TRUE);
	}

        if(wcscmp(m_strActiveProfile, profile.szEntryName) == 0)
    {
        AfxMessageBox(IDS_INT_ERR_DELACTCON);
        return;
    }

    if(AfxMessageBox(IDS_DELETECONFIRM, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
        return;

	if (0 == m_connect_type) {
		m_pNdisConnProf->DeleteConnProfile(nIndex);
	} 
	else if (1 == m_connect_type) {
		m_pConnProf->DeleteConnProfile(nIndex);
	}
	else {
		ASSERT(TRUE);
	}

    m_cmbConnectionName.DeleteString(nIndex);
    m_cmbConnectionName.SetCurSel(0);
    OnSelchangeComboConnectName();
	CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
	pMainDlg->m_pConnectDlg->getConnProfile();
	pMainDlg->m_pConnectDlg->resetConnProfile();
}


BOOL CDlgNetProfileCDMA::SndAtCGDCONT(CString strConnPoint)
{
    char szAtBuf[CONNECTION_APN_LEN+30] = {0};
    
    sprintf(szAtBuf, "AT+CGDCONT=1,\"IP\",\"%s\"\r", strConnPoint);
    
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        SetTimer(IDT_CGDCONT_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtCGDCONT, this);
        return TRUE;
    }
    return FALSE;
}

void CDlgNetProfileCDMA::RspAtCGDCONT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgNetProfileCDMA* pDlg = (CDlgNetProfileCDMA*)pWnd;
    pDlg->KillTimer(IDT_CGDCONT_TIMEOUT);
    
    CString RetStr;
    RetStr = strArr[0];
    int nHead, nTail;
    
    BOOL bResult=FALSE;
    
    switch(pDlg->nCGDCONTType)
    {
    case 0:
        if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
        {
            //AfxMessageBox(IDS_SETUP_CONNPOINTOK, MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            //AfxMessageBox(IDS_SETUP_CGDCONT_ERROR);
        } 
        break;
    case 1:
        if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0)
        {
            //AfxMessageBox(IDS_SETUP_CGDCONT_ERROR);
        }
        else
        {
            //success
            //+CGDCONT: 1,"IP","","",0,0
            //+CGDCONT: 1,"IP","cmwap","",0,0
            CString strAPN;
            nHead=RetStr.Find(_T("\",\""),0);
            if (nHead!=-1)
            {
                nTail=RetStr.Find(_T("\",\""),nHead+1);
                if (nTail!=-1)
                {
                    RetStr=strArr[0]+nHead+3;
                    strAPN=RetStr.Left(nTail-nHead-3);
                    bResult=TRUE;
                }
            } 
            else
            {
            }
            pDlg->m_strAPN=strAPN;
        }
        break;
    case 2:
        break;
    }	
    pDlg->nCGDCONTType=-1;
    
    //save last APN
    //pDlg->m_strLastAPN = pDlg->m_strAPN;
    pDlg->PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)0, (LPARAM)TRUE);   
}

LRESULT CDlgNetProfileCDMA::OnUpdateInternetDlg(WPARAM wParam, LPARAM lParam)
{
	int nIndex = m_cmbConnectionName.GetCurSel();

    switch(wParam)
    {
    case TRUE:
// 		GetDlgItem(IDC_COMBO_CONNECT_NAME)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_PHONE_NUMBER)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(0 != nIndex);
// 		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(0 != nIndex);
// 		GetDlgItem(IDC_EDIT_APN)->EnableWindow(0 != nIndex);
// 		GetDlgItem(IDC_BUTTON_SAVECONN)->EnableWindow(0 != nIndex);
// 		GetDlgItem(IDC_BUTTON_DELETECONN)->EnableWindow(0 != nIndex);
// 		GetDlgItem(IDC_CHECK_SETDEFAULT)->EnableWindow(0 != nIndex);
// 		GetDlgItem(IDC_RADIO_CHAP)->EnableWindow(0 != nIndex);
// 		GetDlgItem(IDC_RADIO_PAP)->EnableWindow(0 != nIndex);
        break;
    case FALSE:
		//GetDlgItem(IDC_COMBO_CONNECT_NAME)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_PHONE_NUMBER)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);
           GetDlgItem(IDC_BUTTON_SAVECONN)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DELETECONN)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_SETDEFAULT)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_CHAP)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_PAP)->EnableWindow(FALSE);
        break;
    }

    UpdateData(FALSE);

    return TRUE;
}


void CDlgNetProfileCDMA::UpdateButton()
{
    if(m_pConnProf->IsEmpty())
    {
        GetDlgItem(IDC_EDIT_PHONE_NUMBER)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SAVECONN)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DELETECONN)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DIAL)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_SETDEFAULT)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_CHAP)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_PAP)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_EDIT_PHONE_NUMBER)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SAVECONN)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_DELETECONN)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_DIAL)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_SETDEFAULT)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_CHAP)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_PAP)->EnableWindow(TRUE);
    }
}


void CDlgNetProfileCDMA::SetAPN(CString strPLMN)
{
	if(BApnSet) //已经设置过
	{
		return;
	}
	CString strCurrAPN = _T("");
	StConnProfile CurrProfile;
	memset(&CurrProfile, 0x00, sizeof(StConnProfile));
	if(!strPLMN.Compare(_T("AMAZONIA")))
	{
		CurrProfile.bDefault = TRUE;
        CurrProfile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
        wcscpy(CurrProfile.szEntryName, _T("Amazonia Cel GPRS"));
        wcscpy(CurrProfile.szPhoneNumber, g_SetData.Internet_szLocalPhoneNumber);
        wcscpy(CurrProfile.szUserName, _T("celular"));
        wcscpy(CurrProfile.szPassword, _T("celular"));
        wcscpy(CurrProfile.szAPN, _T("gprs.amazoniacelular.com.br"));
	}
	else if(!strPLMN.Compare(_T("TELEMIGC")))
	{
		CurrProfile.bDefault = TRUE;
        CurrProfile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
        wcscpy(CurrProfile.szEntryName, _T("Telemig Cel GPRS"));
        wcscpy(CurrProfile.szPhoneNumber, g_SetData.Internet_szLocalPhoneNumber);
        wcscpy(CurrProfile.szUserName, _T("celular"));
        wcscpy(CurrProfile.szPassword, _T("celular"));
        wcscpy(CurrProfile.szAPN, _T("gprs.telemigcelular.com.br"));
	}
	else if(!strPLMN.Compare(_T("Claro")))
	{
		CurrProfile.bDefault = TRUE;
        CurrProfile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
        wcscpy(CurrProfile.szEntryName, _T("Claro GPRS"));
        wcscpy(CurrProfile.szPhoneNumber, g_SetData.Internet_szLocalPhoneNumber);
        wcscpy(CurrProfile.szUserName, _T("claro"));
        wcscpy(CurrProfile.szPassword, _T("claro"));
        wcscpy(CurrProfile.szAPN, _T("Claro.com.br"));
	}
	else if(!strPLMN.Compare(_T("VIVO")))
	{
		CurrProfile.bDefault = TRUE;
        CurrProfile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
        wcscpy(CurrProfile.szEntryName, _T("Vivo GPRS"));
        wcscpy(CurrProfile.szPhoneNumber, g_SetData.Internet_szLocalPhoneNumber);
        wcscpy(CurrProfile.szUserName, _T("vivo"));
        wcscpy(CurrProfile.szPassword, _T("vivo"));
        wcscpy(CurrProfile.szAPN, _T("vivo.com.br"));
	}
	else if(!strPLMN.Compare(_T("Oi")))
	{
		CurrProfile.bDefault = TRUE;
        CurrProfile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
        wcscpy(CurrProfile.szEntryName, _T("Oi GPRS"));
        wcscpy(CurrProfile.szPhoneNumber, g_SetData.Internet_szLocalPhoneNumber);
        wcscpy(CurrProfile.szUserName, _T(""));
        wcscpy(CurrProfile.szPassword, _T(""));
        wcscpy(CurrProfile.szAPN, _T("gprs.oi.com.brr"));
	}
	int cnt = 0;
	int nIndex = -1;
    int nConnNum = m_pConnProf->GetConnNum();
    StConnProfile profile;
	for(cnt = 0; cnt < nConnNum; cnt++)
    {
        memset(&profile, 0x00, sizeof(StConnProfile));
        profile = m_pConnProf->ReadConnProfile(cnt);
        int nres = wcscmp(CurrProfile.szAPN,profile.szAPN);
		if(nres == 0)
		{
			nIndex = cnt;
			break;
		}
    }
	if(nIndex > -1)  //find apn
	{
		m_pConnProf->EditConnProfile(nIndex,CurrProfile);
		m_cmbConnectionName.SetCurSel(nIndex);
	}
	else
	{
        m_pConnProf->AddConnProfile(CurrProfile);
		m_cmbConnectionName.AddString(CurrProfile.szEntryName);
		m_cmbConnectionName.SetCurSel(m_cmbConnectionName.GetCount()-1);
	}    
    OnSelchangeComboConnectName();
    UpdateButton();
	BApnSet = TRUE;
	return;
}

void CDlgNetProfileCDMA::OnButtonDetails() 
{
    // TODO: Add your control notification handler code here
    CDataServiceHistory dlg;
    dlg.DoModal();
}
/* ndis support*/
#ifdef FEATURE_NDIS_SUPPORT
void CDlgNetProfileCDMA::OnRadioNdis()
{
	UpdateData(TRUE);
	CString StrDefault;
	StrDefault.LoadString(IDS_DEFAULT);


	// has default?
	WORD nConnNum = m_pNdisConnProf->GetConnNum();
	if (nConnNum == 0)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
		profile.bDefault = TRUE;
		profile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
		//add by GL
		CString strTempDefault;
        strTempDefault .Format(_T("%s %s"), g_SetData.Internet_szDefaultConnName,StrDefault);

		wcscpy(profile.szEntryName, strTempDefault);
		wcscpy(profile.szUserName, g_SetData.Internet_szUserName);
		wcscpy(profile.szPassword, g_SetData.Internet_szPassword);
	//	wcscpy(profile.szAPN, _T("ibox.tim.it"));
		profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
		m_pNdisConnProf->AddConnProfile(profile);
	} 


#if 0

	if (nConnNum == 1)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
//		profile.bDefault = TRUE;
		profile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
	    CString strTempEdit;
		strTempEdit .Format(_T("%s (Edit.)"), g_SetData.Internet_szDefaultConnName);
		wcscpy(profile.szEntryName, strTempEdit);
		wcscpy(profile.szUserName, g_SetData.Internet_szUserName);
		wcscpy(profile.szPassword, g_SetData.Internet_szPassword);
	//	wcscpy(profile.szAPN, _T("ibox.tim.it"));
		profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
		m_pNdisConnProf->AddConnProfile(profile);
	} 
#endif

	

	// delete combo box
	m_cmbConnectionName.ResetContent();

	// load config file.
	WORD cnt = 0;
	nConnNum = m_pNdisConnProf->GetConnNum();
	for(cnt = 0; cnt < nConnNum; cnt++)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));

		profile = m_pNdisConnProf->ReadConnProfile(cnt);

		// add data to combo box
		m_cmbConnectionName.AddString(profile.szEntryName);
	}

	WORD nIndex = m_pNdisConnProf->GetDefaultConnProfileIndex();
    if(nIndex < nConnNum)
        m_cmbConnectionName.SetCurSel(nIndex);
    else
        m_cmbConnectionName.SetCurSel(0);

    OnSelchangeComboConnectName();
}


void CDlgNetProfileCDMA::OnRadioRas()
{
	UpdateData(TRUE);
	CString StrDefault;
	StrDefault.LoadString(IDS_DEFAULT);

	// has default?
	WORD nConnNum = m_pConnProf->GetConnNum();
	if (nConnNum == 0)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
		profile.bDefault = TRUE;
		profile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
		//add by GL
		CString strTempDefault;
        strTempDefault .Format(_T("%s %s"), g_SetData.Internet_szDefaultConnName,StrDefault);

		wcscpy(profile.szEntryName, strTempDefault);
		wcscpy(profile.szPhoneNumber, g_SetData.Internet_szLocalPhoneNumber);
		wcscpy(profile.szUserName, g_SetData.Internet_szUserName);
		wcscpy(profile.szPassword, g_SetData.Internet_szPassword);
		wcscpy(profile.szAPN, _T("ibox.tim.it"));
		profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
		m_pConnProf->AddConnProfile(profile);
	} 

#if 0
	if (nConnNum == 1)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
//		profile.bDefault = TRUE;
		profile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
	    CString strTempEdit;
		strTempEdit .Format(_T("%s (Edit.)"), g_SetData.Internet_szDefaultConnName);
		wcscpy(profile.szEntryName, strTempEdit);
		wcscpy(profile.szPhoneNumber, g_SetData.Internet_szLocalPhoneNumber);
		wcscpy(profile.szUserName, g_SetData.Internet_szUserName);
		wcscpy(profile.szPassword, g_SetData.Internet_szPassword);
		wcscpy(profile.szAPN, _T("ibox.tim.it"));
		profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
		m_pConnProf->AddConnProfile(profile);
	}
#endif

		

	m_cmbConnectionName.ResetContent();

	// load config file.
	WORD cnt = 0;
	nConnNum = m_pConnProf->GetConnNum();
	for(cnt = 0; cnt < nConnNum; cnt++)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));

		profile = m_pConnProf->ReadConnProfile(cnt);

		// add data to combo box
		m_cmbConnectionName.AddString(profile.szEntryName);
	}

	WORD nIndex = m_pConnProf->GetDefaultConnProfileIndex();
    if(nIndex < nConnNum)
        m_cmbConnectionName.SetCurSel(nIndex);
    else
        m_cmbConnectionName.SetCurSel(0);

    OnSelchangeComboConnectName();
}
#endif



