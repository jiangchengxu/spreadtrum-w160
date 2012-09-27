/*
File: HSDPADlg.cpp
Desc:
    设置


Note:


*/

// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "SetupDlg.h"

#include "DlgInternetNewConn.h"
#include "PinClckDlg.h"
#include "ModifyPinDlg.h"
#include "DlgSetupGeneral.h"
#include "resource.h"
#include "DlgNetProfile.h"
#include "DlgNetProfileCDMA.h"
#include "AboutDialog.h"
#include "PBGroupDlg.h"
#include "DlgNetSettingCDMA.h"
#include "SetupCallDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
    : CTabedDialog(CSetupDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSetupDlg)
    //}}AFX_DATA_INIT
    m_nBkTag = 1;
    m_pHandlePinSetup = &(((CHSDPADlg*)AfxGetMainWnd())->m_cHandlePin);
}


void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
    CTabedDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSetupDlg)
    DDX_Control(pDX, IDC_TAB_SETUP, m_tabCtrl);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupDlg, CTabedDialog)
    //{{AFX_MSG_MAP(CSetupDlg)

    ON_BN_CLICKED(IDC_BUTTON_SETUP_CALL, OnButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_BTNPROFILES, OnButtonProfiles)
    ON_BN_CLICKED(IDC_BTN_PINENABLE, OnButtonEnablePIN)
    ON_BN_CLICKED(IDC_BTN_PINDISABLE, OnButtonDisablePIN)
    ON_BN_CLICKED(IDC_BTN_PINMODIFY, OnButtonModifyPIN)
    ON_BN_CLICKED(IDC_BTN_PINUNLOCK, OnButtonUnlockPIN)
    ON_BN_CLICKED(IDC_BUTTON_BTNTEXT, OnButtonText)
    ON_BN_CLICKED(IDC_BUTTON_BTNNETWORK, OnButtonNetWork)

    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETUP, OnSelchangeTabSetup)
    ON_BN_CLICKED(IDC_BUTTON_BTNPREFERENCE,OnButtonPreference)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers

BOOL CSetupDlg::CanSwitchMe()
{
    if (m_dlgSetupNWSel.nCOPSType==2) {
        return false;
    } else {
        return true;
    }
}

BOOL CSetupDlg::OnInitDialog()
{
    CTabedDialog::OnInitDialog();

    // TODO: Add extra initialization here
    //TCITEM item;
    CString strTabCaption;

    CRect r;
    m_tabCtrl.GetClientRect (&r);

    strTabCaption.LoadString(IDS_SET_TAB_GENERAL);
    m_tabCtrl.InsertItem(0, strTabCaption);

    //add Messages Management


    //add Network Select
#ifdef SETUP_NETWORK
    strTabCaption.LoadString(IDS_SET_TAB_NETWORK);
    m_tabCtrl.InsertItem(2, strTabCaption);
#endif

    strTabCaption.LoadString(IDS_SET_TAB_PROFILES);
    m_tabCtrl.InsertItem(3, strTabCaption);

    strTabCaption.LoadString(IDS_SET_TAB_PREFER);
    m_tabCtrl.InsertItem(4, strTabCaption);

    //re-add Skin
#ifdef SETUP_SKIN
    strTabCaption.LoadString(IDS_SET_TAB_SKIN);
    m_tabCtrl.InsertItem(3, strTabCaption);
#endif

    m_dlgSetupGeneral.Create (IDD_DIALOG_SETUP_GENERAL,&m_tabCtrl);
// 	m_dlgUsage.Create(IDD_DIALOG_NET_STATISTICS, &m_tabCtrl);
// 	m_dlgUsage.ShowWindow(SW_SHOW);

#ifdef SETUP_NETWORK
    m_dlgSetupNWSel.Create(IDD_DIALOG_SETUP_NWSEL, &m_tabCtrl);
#endif

#ifdef SETUP_SKIN
    m_dlgSetupSkin.Create(IDD_DIALOG_SETUP_SKIN, &m_tabCtrl);
#endif

    m_dlgPreferences.Create(IDD_DIALOG_SETUP_PREFER, &m_tabCtrl);
    m_dlgPreferences.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);


#ifdef SETUP_NETWORK
    m_dlgSetupNWSel.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
#ifdef SETUP_SKIN
    m_dlgSetupSkin.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif

    m_dlgSetupGeneral.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_SHOWWINDOW);

#ifdef FEATURE_VERSION_NOSIM
    GetDlgItem(IDC_STATIC_OPPIN)->ShowWindow(FALSE);
    GetDlgItem(IDC_STATIC_PINSTR)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_PINENABLE)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_PINDISABLE)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_PINMODIFY)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_PINUNLOCK)->ShowWindow(FALSE);

    CRect rectCtrl[5];
    CWnd *pWndCtrl[5];
    pWndCtrl[0] = GetDlgItem(IDC_STATIC_OPSETTING);
    pWndCtrl[1] = GetDlgItem(IDC_BUTTON_BTNTEXT);
    //pWndCtrl[2] = GetDlgItem(IDC_BUTTON_BTNNETWORK);
    pWndCtrl[2] = GetDlgItem(IDC_BUTTON_BTNPROFILES);
    pWndCtrl[3] = GetDlgItem(IDC_BUTTON_BTNPREFERENCE);
    pWndCtrl[4] = GetDlgItem(IDC_BUTTON_SETUP_CALL);

    int iHeight;
    for (int i = 0; i<5; i++) {
        pWndCtrl[i]->GetWindowRect(rectCtrl[i]);

        iHeight = rectCtrl[i].Height();
        rectCtrl[i].top -=100;
        pWndCtrl[i]->MoveWindow(rectCtrl[i].left, rectCtrl[i].top, rectCtrl[i].Width(), iHeight);

    }

#endif


#ifndef FEATURE_CALL_PRIVACY
    CRect rc;
    GetDlgItem(IDC_STATIC_OPSETTING)->GetWindowRect(rc);
    rc.top = rc.top - 30;
    rc.bottom = rc.bottom - 30;
    rc.right = rc.right - 130;
    GetDlgItem(IDC_STATIC_OPSETTING)->MoveWindow(rc);
    GetDlgItem(IDC_BUTTON_SETUP_CALL)->ShowWindow(SW_HIDE);
#endif


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE

}

void CSetupDlg::OnSelchangeTabSetup(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    CRect r;
    m_tabCtrl.GetClientRect (&r);

    switch(m_tabCtrl.GetCurSel()) {
    case 0:
        m_dlgSetupGeneral.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_SHOWWINDOW);

#ifdef SETUP_NETWORK
        m_dlgSetupNWSel.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
#ifdef SETUP_SKIN
        m_dlgSetupSkin.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
        m_dlgPreferences.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);

        //设置页面切换时，General页面的高亮显示应该切换回默认连接
        m_dlgSetupGeneral.InitData();
        break;
    case 1:
        m_dlgSetupGeneral.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);

#ifdef SETUP_NETWORK
        m_dlgSetupNWSel.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
#ifdef SETUP_SKIN
        m_dlgSetupSkin.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
        m_dlgPreferences.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);

        break;
#ifdef SETUP_NETWORK
    case 2:
        m_dlgSetupGeneral.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);

        m_dlgSetupNWSel.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_SHOWWINDOW);
#ifdef SETUP_SKIN
        m_dlgSetupSkin.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
        m_dlgPreferences.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);

        break;
#ifdef SETUP_SKIN
    case 3:
        m_dlgSetupGeneral.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);

        m_dlgSetupNWSel.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
        m_dlgSetupSkin.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_SHOWWINDOW);
        break;
#endif
#endif
    case 3:
        m_dlgSetupGeneral.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);

#ifdef SETUP_NETWORK
        m_dlgSetupNWSel.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
#ifdef SETUP_SKIN
        m_dlgSetupSkin.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
        m_dlgPreferences.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
        break;

    case 4:
        m_dlgSetupGeneral.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);

#ifdef SETUP_NETWORK
        m_dlgSetupNWSel.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
#ifdef SETUP_SKIN
        m_dlgSetupSkin.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_HIDEWINDOW);
#endif
        m_dlgPreferences.SetWindowPos (NULL,0,25,r.right,r.bottom,SWP_SHOWWINDOW);
        m_dlgPreferences.ReadDataFromFile();
        break;
    }
    *pResult = 0;
}

BOOL CSetupDlg::SndAtCGDCONT(CString strConnPoint)
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

void CSetupDlg::RspAtCGDCONT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    //CDlgSetupGeneral* pDlg = (CDlgSetupGeneral*)pWnd;
    CSetupDlg* pDlg = (CSetupDlg*)pWnd;
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
/*发送AT$CPINS?命令检查卡是否报废*/
bool CSetupDlg::CheckSimStatSetup()
{
    if(!(m_pHandlePinSetup->GetPinRemainTimes())) {
        EnableButton(FALSE);
        return false;
    }
    return true;
}
//AT+CLCK=\"SC\",2命令获取PIN码状态
bool CSetupDlg::LoadPinStatSetup()
{
    if(!(m_pHandlePinSetup->GetPinStat())) {
        EnableButton(FALSE);
        return false;
    }
    return true;
}

/*AT+CPIN?命令,检查SIM卡状态，bReady为true代表Enable PIN,Disable PIN,Modify PIN;
为false代表Unlock PUK。返回值为0代表执行失败，返回值为1代表状态正常，为-1代表需PUK解锁*/
int CSetupDlg::LoadSimStatSetup(bool bReady)
{
    if(!(m_pHandlePinSetup->GetSimStat())) { //获取SIM卡状态
        EnableButton(FALSE);
        return 0;
    }
    switch (m_pHandlePinSetup->m_nSimStat) {
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

void CSetupDlg::CheckPINUnlockSetup()
{
    //m_pHandlePin->GetSimStat();//会导致usb modem busy
    if(CPIN_SIM_PUK_REQUIRED == m_pHandlePinSetup->m_nSimStat) {
        GetDlgItem(IDC_BTN_PINUNLOCK)->EnableWindow(TRUE);
    } else {
        GetDlgItem(IDC_BTN_PINUNLOCK)->EnableWindow(FALSE);
    }
    return;
}

void CSetupDlg::EnableButton(BOOL BShow)
{
    GetDlgItem(IDC_BTN_PINENABLE)->EnableWindow(BShow);
    GetDlgItem(IDC_BTN_PINDISABLE)->EnableWindow(BShow);
    GetDlgItem(IDC_BTN_PINMODIFY)->EnableWindow(BShow);
    GetDlgItem(IDC_BUTTON_BTNNETWORK)->EnableWindow(BShow);
    GetDlgItem(IDC_BUTTON_SETUP_CALL)->EnableWindow(BShow);
}

void CSetupDlg::OnButtonProfiles()
{
    if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
            (wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0)) {
        CDlgNetProfile dlg(this);
        dlg.DoModal();
    } else if((wcscmp(g_SetData.Setup_sz3GType,_T("CDMA2000"))==0)||
              (wcscmp(g_SetData.Setup_sz3GType,_T("cdma2000"))==0)) {
        CDlgNetProfileCDMA dlg(this);
        dlg.DoModal();

    }
}
//liub_add_end

void CSetupDlg::OnButtonPreference()
{
    CSetupPreferDlg dlg(this);
    dlg.DoModal();
}

void CSetupDlg::OnButtonEnablePIN()
{


    EnableButton(FALSE);
    bool bCheckRes =  CheckSimStatSetup();
    if (!bCheckRes) {
        return;
    }
    if (0 == m_pHandlePinSetup->m_nRemainTimes_puk) {
        AfxMessageBox(IDS_PUK_LOCK);

        return;
    }

    int bRes = LoadSimStatSetup(true);
    if (0 >= bRes) {
        if (-1 == bRes) {
            EnableButton(TRUE);
        }

        return;
    }

    bool bRespose = LoadPinStatSetup();  //检查PIN码状态
    if (!bRespose) {
        return;
    }

    if(1 == m_pHandlePinSetup->m_nPinStat) {
        AfxMessageBox(IDS_PIN_WR_ENABLE);
        EnableButton(TRUE);

        return;
    }
    CPinClckDlg EnablePINDlg(0,0);
    int nBack = EnablePINDlg.DoModal();
    if(nBack == IDOK) {
        CheckPINUnlockSetup();
    }
    EnableButton(TRUE);

}

void CSetupDlg::OnButtonDisablePIN()
{
    EnableButton(FALSE);

    bool bCheckRes =  CheckSimStatSetup();
    if (!bCheckRes) {
        return;
    }
    if (0 == m_pHandlePinSetup->m_nRemainTimes_puk) {
        CString strPukLock;
        strPukLock.LoadString(IDS_PUK_LOCK);
        AfxMessageBox(strPukLock);

        return;
    }

    int nRes = LoadSimStatSetup(true);
    if (0 >= nRes) {
        if (-1 == nRes) {
            EnableButton(TRUE);
        }

        return;
    }

    bool bRespose = LoadPinStatSetup();
    if (!bRespose) {
        return;
    }
    if(0 == m_pHandlePinSetup->m_nPinStat) {
        AfxMessageBox(IDS_PIN_WR_DISABLE);
        EnableButton(TRUE);

        return;
    }

    CPinClckDlg DisableDlg(0,1);
    int nBack = DisableDlg.DoModal();
    if(nBack == IDOK) {
        CheckPINUnlockSetup();
    }
    EnableButton(TRUE);


}

void CSetupDlg::OnButtonModifyPIN()
{
    EnableButton(FALSE);

    bool bCheckRes =  CheckSimStatSetup();
    if (!bCheckRes) {
        return;
    }
    if (0 == m_pHandlePinSetup->m_nRemainTimes_puk) {
        CString strPukLock;
        strPukLock.LoadString(IDS_PUK_LOCK);
        AfxMessageBox(strPukLock);

        return;
    }

    int bRes = LoadSimStatSetup(true);
    if (0 >= bRes) {
        if (-1 == bRes) {
            EnableButton(TRUE);
        }

        return;
    }

    bool bRespose = LoadPinStatSetup();
    if (!bRespose) {
        return;
    }

    if(0 == m_pHandlePinSetup->m_nPinStat) {
        AfxMessageBox(IDS_MODIFYPIN_DISABLE);
        EnableButton(TRUE);

        return;
    }

    CModifyPinDlg mpDlg(0,0);
    int nBack = mpDlg.DoModal();
    if(nBack == IDOK) {
        CheckPINUnlockSetup();
    }
    EnableButton(TRUE);

}

void CSetupDlg::OnButtonUnlockPIN()
{
    EnableButton(FALSE);

    bool bCheckRes =  CheckSimStatSetup();
    if (!bCheckRes) {
        return;
    }
    if (0 == m_pHandlePinSetup->m_nRemainTimes_puk) {
        AfxMessageBox(IDS_PUK_LOCK);

        return;
    }

    int bRes = LoadSimStatSetup(false);
    if (0 >= bRes) {
        if (-1 == bRes) {
            EnableButton(TRUE);
        }

        return;
    }
    CModifyPinDlg UnlockPinDlg(0,1);
    int nBack = UnlockPinDlg.DoModal();
    if(nBack == IDOK) {
        CheckPINUnlockSetup();
    }
    EnableButton(TRUE);

}

void CSetupDlg::OnButtonText()
{
// 	if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
// 	   (wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0))
    {

        CDlgSetupPowerMng dlg(this);
        dlg.DoModal();

    }
// 	else if((wcscmp(g_SetData.Setup_sz3GType,_T("CDMA2000"))==0)||
// 		(wcscmp(g_SetData.Setup_sz3GType,_T("cdma2000"))==0))
// 	{
// 		CDlgSMSPriorityCDMA dlg(this);
// 		dlg.DoModal();
// 	}
}

void CSetupDlg::OnButtonNetWork()
{
    if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
            (wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0)) {
// 	CDlgSetupNWSel *temp = new CDlgSetupNWSel;
// 	temp->Create(IDD_DIALOG_SETUP_NWSEL);
// 	temp->ShowWindow(SW_SHOW);
        CDlgSetupNWSel dlg(this);
        dlg.DoModal();

    } else if((wcscmp(g_SetData.Setup_sz3GType,_T("CDMA2000"))==0)||
              (wcscmp(g_SetData.Setup_sz3GType,_T("cdma2000"))==0)) {
// 		CDlgNetSettingCDMA *temp = new CDlgNetSettingCDMA;
// 		temp->Create(IDD_DIALOG_NETWORKSETTING_CDMA);
// 		temp->ShowWindow(SW_SHOW);
        CDlgNetSettingCDMA dlg(this);
        dlg.DoModal();

    }

    //SetSkin(g_SetData.Setup_nSkinStyle);
}

void CSetupDlg::OnButtonCall()
{
    //	if(g_SetData.Setup_nModemLock)
    CSetupCallDlg dlg(this);
    dlg.DoModal();
}
