// DlgSetupGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "DlgSetupGeneral.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupGeneral dialog


CDlgSetupGeneral::CDlgSetupGeneral(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CDlgSetupGeneral::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSetupGeneral)
    //}}AFX_DATA_INIT
    m_nBkTag = 1;
    m_pHandlePin = &(((CHSDPADlg*)AfxGetMainWnd())->m_cHandlePin);
}


void CDlgSetupGeneral::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSetupGeneral)
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetupGeneral, CBaseDialog)
    //{{AFX_MSG_MAP(CDlgSetupGeneral)
    ON_MESSAGE(WM_SETUP_UPDATE_GENERALDLG, OnUpdateGeneralDlg)
    ON_BN_CLICKED(IDC_BUTTON_ENABLEPIN, OnButtonEnablepin)
    ON_BN_CLICKED(IDC_BUTTON_DISABLE_PIN, OnButtonDisablePin)
    ON_BN_CLICKED(IDC_BUTTON_MODIFYPIN, OnButtonModifypin)
    ON_BN_CLICKED(IDC_BUTTON_UNLOCKPIN, OnButtonUnlockpin)
    ON_BN_CLICKED(IDC_BUTTON_SETUP_POWERMNG_POWERON, OnButtonSetupPowermngPoweron)
    ON_BN_CLICKED(IDC_BUTTON_SETUP_POWERMNG_POWEROFF, OnButtonSetupPowermngPoweroff)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupGeneral message handlers

//DEL void CDlgSetupGeneral::OnSelchangeListSetupGeneralConnlist()
//DEL {
//DEL     // TODO: Add your control notification handler code here
//DEL     //根据选择的连接是否为默认连接，将SetDefault按钮切换状态
//DEL     CString strEntryName;
//DEL     m_ctlConnList.GetText(m_ctlConnList.GetCurSel(),strEntryName);
//DEL     if(strcmp(g_SetData.Internet_szDefaultConnection, strEntryName))
//DEL     {
//DEL 		((CButton *)GetDlgItem(IDC_BUTTON_SETUP_GENERAL_SETDEFAULT))->EnableWindow(TRUE);
//DEL     }
//DEL     else
//DEL     {
//DEL 		((CButton *)GetDlgItem(IDC_BUTTON_SETUP_GENERAL_SETDEFAULT))->EnableWindow(FALSE);
//DEL     }
//DEL }

//DEL void CDlgSetupGeneral::OnButtonSetupGeneralSetdefault()
//DEL {
//DEL     // TODO: 设置默认连接
//DEL     CString strEntryName;
//DEL     ((CButton *)GetDlgItem(IDC_BUTTON_SETUP_GENERAL_SETDEFAULT))->EnableWindow(FALSE);
//DEL     m_ctlConnList.GetText(m_ctlConnList.GetCurSel(),strEntryName);
//DEL     strcpy(g_SetData.Internet_szDefaultConnection, strEntryName);
//DEL     SaveIniFile();
//DEL }

BOOL CDlgSetupGeneral::OnInitDialog()
{
    CBaseDialog::OnInitDialog();

    // TODO: Add extra initialization here
    InitData();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgSetupGeneral::InitData()
{
    LoadIniFile();
    m_nPowerStatus=g_SetData.Setup_nPowerManagement;
    if (m_nPowerStatus) {
        //原状态为上电,
        //将状态设置为断电,显示OFF按钮,保存状态
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWEROFF)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWERON)->EnableWindow(FALSE);

    } else {
        //原状态为断电,
        //显示ON按钮,保存状态
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWEROFF)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWERON)->EnableWindow(TRUE);
    }
    UpdateData(FALSE);
    /*
        m_ctlConnList.ResetContent();

        int i;
        for (i=0; i<nConnNumber; i++)
        {
            m_ctlConnList.AddString(m_szConnection[i]);
        }

        m_ctlConnList.SetCurSel(m_ctlConnList.FindStringExact(
            -1, g_SetData.Internet_szDefaultConnection));
    	m_strConnPoint="";
    	m_strSCA="";
    	GetDlgItem(IDC_BUTTON_SETUP_SETCONNPOINT_SET)->EnableWindow(FALSE);
    	GetDlgItem(IDC_BUTTON_SETUP_SCA_SET)->EnableWindow(FALSE);
        UpdateData(FALSE);
    	if (g_SetData.Main_nCompany==COMP_TELIT)
    	{
    		SndAtCGDCONT("?");
    		nCGDCONTType=1;
    	}
    */
    return true;
}

void CDlgSetupGeneral::OnCancel()
{
    //CBaseDialog::OnCancel();
}

void CDlgSetupGeneral::OnOK()
{
    //CBaseDialog::OnOK();
}

//DEL void CDlgSetupGeneral::OnButtonSetupSetconnpointSet()
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	UpdateData(TRUE);
//DEL 	CString strTemp;
//DEL 	strTemp.Format("=1,\"IP\",\"%s\"", m_strConnPoint);
//DEL 	if (SndAtCGDCONT(strTemp))
//DEL 	{
//DEL 		GetDlgItem(IDC_EDIT_SETUP_CONNPOINT)->EnableWindow(FALSE);
//DEL 		GetDlgItem(IDC_BUTTON_SETUP_SETCONNPOINT_SET)->EnableWindow(FALSE);
//DEL 		nCGDCONTType=0;
//DEL 	}
//DEL }

BOOL CDlgSetupGeneral::SndAtCGDCONT(CString strConnPoint)
{
    const char szATSetConnPoint[]="AT+CGDCONT";
    char szAtBuf[50] = {0};

    sprintf(szAtBuf, "%s%s\r", szATSetConnPoint, strConnPoint);

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf))) {
        SetTimer(IDT_CGDCONT_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtCGDCONT, this);
        return TRUE;
    }
    return FALSE;
}

void CDlgSetupGeneral::RspAtCGDCONT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgSetupGeneral* pDlg = (CDlgSetupGeneral*)pWnd;
    pDlg->KillTimer(IDT_CGDCONT_TIMEOUT);

    CString RetStr;
    RetStr = (const TCHAR*)strArr[0];
    int nHead, nTail;

    BOOL bResult=FALSE;

    switch(pDlg->nCGDCONTType) {
    case 0:
        if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0) {
            AfxMessageBox(IDS_SETUP_CONNPOINTOK, MB_OK | MB_ICONINFORMATION);
        } else {
            //设置失败
            AfxMessageBox(IDS_SETUP_CGDCONT_ERROR);
        }
        break;
    case 1:
        if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0) {
            //fail
            AfxMessageBox(IDS_SETUP_CGDCONT_ERROR);
        } else {
            //success
            //+CGDCONT: 1,"IP","","",0,0
            //+CGDCONT: 1,"IP","cmwap","",0,0
            CString strAPN;
            nHead=RetStr.Find(_T("\",\""),0);
            if (nHead!=-1) {
                nTail=RetStr.Find(_T("\",\""),nHead+1);
                if (nTail!=-1) {
                    RetStr=strArr[0]+nHead+3;
                    strAPN=RetStr.Left(nTail-nHead-3);
                    bResult=TRUE;
                }
            } else {
            }
            pDlg->m_strConnPoint=strAPN;
        }
        break;
    case 2:
        break;
    }
    pDlg->nCGDCONTType=-1;
    pDlg->PostMessage(WM_SETUP_UPDATE_GENERALDLG, (WPARAM)TRUE, (LPARAM)TRUE);
}

LRESULT CDlgSetupGeneral::OnUpdateGeneralDlg(WPARAM wParam, LPARAM lParam)
{
    //设置成功
    if (m_nPowerStatus) {
        //原状态为上电,
        //将状态设置为断电,显示ON按钮,保存状态
        m_nPowerStatus=0;
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWEROFF)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWERON)->EnableWindow(TRUE);
    } else {
        //原状态为断电,
        //将状态设置为上电,显示OFF按钮,保存状态
        m_nPowerStatus=1;
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWEROFF)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWERON)->EnableWindow(FALSE);
        //RF on, enable PIN button
        EnableButton(TRUE);
    }
    g_SetData.Setup_nPowerManagement=m_nPowerStatus;
    SaveIniFile();
    EndWaitCursor();
    return TRUE;
}

//DEL void CDlgSetupGeneral::OnUpdateEditSetupConnpoint()
//DEL {
//DEL 	// TODO: If this is a RICHEDIT control, the control will not
//DEL 	// send this notification unless you override the CBaseDialog::OnInitDialog()
//DEL 	// function to send the EM_SETEVENTMASK message to the control
//DEL 	// with the ENM_UPDATE flag ORed into the lParam mask.
//DEL
//DEL 	// TODO: Add your control notification handler code here
//DEL 	UpdateData(TRUE);
//DEL 	if (m_strConnPoint.GetLength()>0)
//DEL 	{
//DEL 		GetDlgItem(IDC_BUTTON_SETUP_SETCONNPOINT_SET)->EnableWindow(TRUE);
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		GetDlgItem(IDC_BUTTON_SETUP_SETCONNPOINT_SET)->EnableWindow(FALSE);
//DEL 	}
//DEL }

//DEL void CDlgSetupGeneral::OnUpdateEditSetupSca()
//DEL {
//DEL 	// TODO: If this is a RICHEDIT control, the control will not
//DEL 	// send this notification unless you override the CBaseDialog::OnInitDialog()
//DEL 	// function to send the EM_SETEVENTMASK message to the control
//DEL 	// with the ENM_UPDATE flag ORed into the lParam mask.
//DEL
//DEL 	// TODO: Add your control notification handler code here
//DEL 	UpdateData(TRUE);
//DEL 	if (m_strSCA.GetLength()>0)
//DEL 	{
//DEL 		GetDlgItem(IDC_BUTTON_SETUP_SCA_SET)->EnableWindow(TRUE);
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		GetDlgItem(IDC_BUTTON_SETUP_SCA_SET)->EnableWindow(FALSE);
//DEL 	}
//DEL }

//DEL void CDlgSetupGeneral::OnButtonSetupScaGet()
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	GetDlgItem(IDC_EDIT_SETUP_SCA)->EnableWindow(FALSE);
//DEL 	GetDlgItem(IDC_BUTTON_SETUP_SCA_GET)->EnableWindow(FALSE);
//DEL 	GetDlgItem(IDC_BUTTON_SETUP_SCA_SET)->EnableWindow(FALSE);
//DEL 	SndAtCSCA("?");
//DEL }

//DEL void CDlgSetupGeneral::OnButtonSetupScaSet()
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	UpdateData(TRUE);
//DEL 	GetDlgItem(IDC_EDIT_SETUP_SCA)->EnableWindow(FALSE);
//DEL 	GetDlgItem(IDC_BUTTON_SETUP_SCA_GET)->EnableWindow(FALSE);
//DEL 	GetDlgItem(IDC_BUTTON_SETUP_SCA_SET)->EnableWindow(FALSE);
//DEL 	SndAtCSCA(m_strSCA);
//DEL }



/*发送AT$CPINS?命令检查卡是否报废*/
bool CDlgSetupGeneral::CheckSimStat()
{
    if(!(m_pHandlePin->GetPinRemainTimes())) {
        EnableButton(FALSE);
        return false;
    }
    return true;
}

//AT+CLCK=\"SC\",2命令获取PIN码状态
bool CDlgSetupGeneral::LoadPinStat()
{
    if(!(m_pHandlePin->GetPinStat())) {
        EnableButton(FALSE);
        return false;
    }
    return true;
}

/*AT+CPIN?命令,检查SIM卡状态，bReady为true代表Enable PIN,Disable PIN,Modify PIN;
为false代表Unlock PUK。返回值为0代表执行失败，返回值为1代表状态正常，为-1代表需PUK解锁*/
int CDlgSetupGeneral::LoadSimStat(bool bReady)
{
    if(!(m_pHandlePin->GetSimStat())) { //获取SIM卡状态
        EnableButton(FALSE);
        return 0;
    }
    switch (m_pHandlePin->m_nSimStat) {
    case CPIN_SIM_PIN_REQUIRED: {
        break;
    }

    case CPIN_SIM_PUK_REQUIRED: { //需PUK解锁
        if (bReady) {
            CString strPinLock;
            strPinLock.LoadString(IDS_PIN_LOCK);
            AfxMessageBox(strPinLock);

            return -1;
        }

        break;
    }
    case CPIN_SIM_NONE_REQUIRED: {
        /*
        if (!bReady)
        {
        	AfxMessageBox("SIM Is Not Locked!");

        	return -1;
        }
        */

        break;
    }
    default: {
        break;
    }
    }

    return 1;
}


/*设置界面上四个按钮可见不可见*/
void CDlgSetupGeneral::EnableButton(BOOL BShow)
{
    GetDlgItem(IDC_BUTTON_ENABLEPIN)->EnableWindow(BShow);
    GetDlgItem(IDC_BUTTON_DISABLE_PIN)->EnableWindow(BShow);
    GetDlgItem(IDC_BUTTON_MODIFYPIN)->EnableWindow(BShow);
    //GetDlgItem(IDC_BUTTON_UNLOCKPIN)->EnableWindow(BShow);
}

void CDlgSetupGeneral::OnButtonEnablepin()
{
    EnableButton(FALSE);

    bool bCheckRes =  CheckSimStat();
    if (!bCheckRes) {
        return;
    }
    if (0 == m_pHandlePin->m_nRemainTimes_puk) {
        AfxMessageBox(IDS_PUK_LOCK);

        return;
    }

    int bRes = LoadSimStat(true);
    if (0 >= bRes) {
        if (-1 == bRes) {
            EnableButton(TRUE);
        }

        return;
    }

    bool bRespose = LoadPinStat();  //检查PIN码状态
    if (!bRespose) {
        return;
    }

    if(1 == m_pHandlePin->m_nPinStat) {
        AfxMessageBox(IDS_PIN_WR_ENABLE);
        EnableButton(TRUE);

        return;
    }
    CPinClckDlg EnablePINDlg(0,0);
    int nBack = EnablePINDlg.DoModal();
    if(nBack == IDOK) {
        CheckPINUnlock();
    }
    EnableButton(TRUE);
}

void CDlgSetupGeneral::OnButtonDisablePin()
{
    EnableButton(FALSE);

    bool bCheckRes =  CheckSimStat();
    if (!bCheckRes) {
        return;
    }
    if (0 == m_pHandlePin->m_nRemainTimes_puk) {
        CString strPukLock;
        strPukLock.LoadString(IDS_PUK_LOCK);
        AfxMessageBox(strPukLock);

        return;
    }

    int nRes = LoadSimStat(true);
    if (0 >= nRes) {
        if (-1 == nRes) {
            EnableButton(TRUE);
        }

        return;
    }

    bool bRespose = LoadPinStat();
    if (!bRespose) {
        return;
    }
    if(0 == m_pHandlePin->m_nPinStat) {
        AfxMessageBox(IDS_PIN_WR_DISABLE);
        EnableButton(TRUE);

        return;
    }

    CPinClckDlg DisableDlg(0,1);
    int nBack = DisableDlg.DoModal();
    if(nBack == IDOK) {
        CheckPINUnlock();
    }
    EnableButton(TRUE);
}

void CDlgSetupGeneral::OnButtonUnlockpin()
{
    EnableButton(FALSE);

    bool bCheckRes =  CheckSimStat();
    if (!bCheckRes) {
        return;
    }
    if (0 == m_pHandlePin->m_nRemainTimes_puk) {
        AfxMessageBox(IDS_PUK_LOCK);

        return;
    }

    int bRes = LoadSimStat(false);
    if (0 >= bRes) {
        if (-1 == bRes) {
            EnableButton(TRUE);
        }

        return;
    }
    CModifyPinDlg UnlockPinDlg(0,1);
    int nBack = UnlockPinDlg.DoModal();
    if(nBack == IDOK) {
        CheckPINUnlock();
    }
    EnableButton(TRUE);
}

void CDlgSetupGeneral::CheckPINUnlock()
{
    //m_pHandlePin->GetSimStat();//会导致usb modem busy
    if(CPIN_SIM_PUK_REQUIRED == m_pHandlePin->m_nSimStat) {
        GetDlgItem(IDC_BUTTON_UNLOCKPIN)->EnableWindow(TRUE);
    } else {
        GetDlgItem(IDC_BUTTON_UNLOCKPIN)->EnableWindow(FALSE);
    }
    return;
}

void CDlgSetupGeneral::OnButtonModifypin()
{
    EnableButton(FALSE);

    bool bCheckRes =  CheckSimStat();
    if (!bCheckRes) {
        return;
    }
    if (0 == m_pHandlePin->m_nRemainTimes_puk) {
        CString strPukLock;
        strPukLock.LoadString(IDS_PUK_LOCK);
        AfxMessageBox(strPukLock);

        return;
    }

    int bRes = LoadSimStat(true);
    if (0 >= bRes) {
        if (-1 == bRes) {
            EnableButton(TRUE);
        }

        return;
    }

    bool bRespose = LoadPinStat();
    if (!bRespose) {
        return;
    }

    if(0 == m_pHandlePin->m_nPinStat) {
        AfxMessageBox(IDS_MODIFYPIN_DISABLE);
        EnableButton(TRUE);

        return;
    }

    CModifyPinDlg mpDlg(0,0);
    int nBack = mpDlg.DoModal();
    if(nBack == IDOK) {
        CheckPINUnlock();
    }
    EnableButton(TRUE);
}


//Modified by lgz 2006-06-20
void CDlgSetupGeneral::OnButtonSetupPowermngPoweron()
{
    // TODO: Add your control notification handler code here
    if (SndAtPowerCFUN(POWER_ON)) {
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWERON)->EnableWindow(FALSE);
        //RF change, disable PIN button
        EnableButton(FALSE);
    }
}

void CDlgSetupGeneral::OnButtonSetupPowermngPoweroff()
{
    // TODO: Add your control notification handler code here
    if (SndAtPowerCFUN(POWER_OFF)) {
        GetDlgItem(IDC_BUTTON_SETUP_POWERMNG_POWEROFF)->EnableWindow(FALSE);
        //RF change, disable PIN button
        EnableButton(FALSE);
    }
}

BOOL CDlgSetupGeneral::SndAtPowerCFUN(EnPowerType nPowerType)
{
    ASSERT(nPowerType == POWER_ON || nPowerType == POWER_OFF);
    const char szATSetPower[]="AT+CFUN=";
    char szAtBuf[50] = {0};

    sprintf(szAtBuf, "%s%s\r", szATSetPower, gcstrSetPower[nPowerType]);

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf))) {
        SetTimer(IDT_CFUN_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtPowerCFUN, this);
        return TRUE;
    }
    return FALSE;
}

void CDlgSetupGeneral::RspAtPowerCFUN(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgSetupGeneral* pDlg = (CDlgSetupGeneral*)pWnd;
    pDlg->KillTimer(IDT_CFUN_TIMEOUT);

    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0) {
        pDlg->PostMessage(WM_SETUP_UPDATE_GENERALDLG, (WPARAM)TRUE, (LPARAM)TRUE);
    } else {
        //设置失败
        AfxMessageBox(IDS_RF_SETERROR);
    }
    //((CHSDPADlg*)theApp.GetMainWnd())->AtSndCOPS();
}
