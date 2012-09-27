// SetupPreferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetupPreferDlg.h"
#include "DiagnosticDlg.h"
#include "Internetusagelimitdlg.h"
#include "PwdConfirmDlg.h"
#include "Dev.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupPreferDlg dialog


CSetupPreferDlg::CSetupPreferDlg(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CSetupPreferDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSetupPreferDlg)
// 	m_bAlertTone = FALSE;
// 	m_bAlertWindow = FALSE;
    m_bConncet = FALSE;
    m_bLaunchApp = FALSE;
    m_bStartMini = FALSE;
    m_nSleepRadio = -1;
    //}}AFX_DATA_INIT
    m_nBkTag = 1;
}


void CSetupPreferDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSetupPreferDlg)
//     DDX_Text(pDX, IDC_EDIT_PREFER_SMSSOUND_FILE, m_strSmsSoundFile);
// 	DDX_Check(pDX, IDC_CHECK_PREFER_ALERTTONE, m_bAlertTone);
// 	DDX_Check(pDX, IDC_CHECK_PREFER_ALERTWINDOW, m_bAlertWindow);
    DDX_Check(pDX, IDC_CHECK_PREFER_CONNNET, m_bConncet);
    DDX_Check(pDX, IDC_CHECK_PREFER_LAUNCHAPP, m_bLaunchApp);
    DDX_Check(pDX, IDC_CHECK_PREFER_STARTMINI, m_bStartMini);
    DDX_Radio(pDX, IDC_RADIO_PREFER_SLEEP_PREVENT, m_nSleepRadio);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupPreferDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CSetupPreferDlg)
    // ON_BN_CLICKED(IDC_BUTTON_PREFER_SMSSOUND_BROWSE, OnButtonSmsSoundBrowse)
    // ON_BN_CLICKED(IDC_BUTTON_PREFER_SMSSOUND_PLAY, OnButtonSmsSoundPlay)
    ON_BN_CLICKED(IDC_BUTTON_PREFER_USAGELIMIT, OnButtonUsagelimit)
    ON_BN_CLICKED(IDC_BUTTON_PREFER_DIAGNOSTICS, OnButtonDiagnostics)
    ON_BN_CLICKED(IDC_BUTTON_PREFER_CANCEL,OnCancel)
//    ON_BN_CLICKED(IDC_BUTTON_PREFER_DEFAULT, OnButtonDefault)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupPreferDlg message handlers
/*void CSetupPreferDlg::OnButtonSmsSoundBrowse()
{
    CString strTitle;
    CFileDialog fileDlg(TRUE, _T("wav"), NULL, 0,
                        _T("wav|*.wav||"), this);

    strTitle.LoadString(IDS_CHOOSE_SOUNDFILE);

    fileDlg.m_ofn.lStructSize = 88;
    fileDlg.m_ofn.lpstrTitle = strTitle;

    if(fileDlg.DoModal() == IDOK)
    {
        m_strSmsSoundFile = fileDlg.GetPathName();
        UpdateData(FALSE);
    }
}*/

/*void CSetupPreferDlg::OnButtonSmsSoundPlay()
{
    UpdateData(TRUE);
    PlaySound((LPCTSTR)m_strSmsSoundFile, NULL, SND_ASYNC | SND_NOSTOP);


}*/

void CSetupPreferDlg::OnButtonUsagelimit()
{
    CPwdConfirmDlg PwdConfirmDlg(this);
    int nRes = PwdConfirmDlg.DoModal();
    if(nRes == IDOK) {
        CInternetUsageLimitDlg dlg;
        dlg.DoModal();
    }
}

void CSetupPreferDlg::OnButtonDiagnostics()
{
    CDiagnosticDlg dlg;
    dlg.DoModal();
}

/*void CSetupPreferDlg::OnButtonDefault()
{
    m_strSmsSoundFile = GetDefaultSMSSoundFilePath();
     m_bAlertTone = TRUE;
 	m_bAlertWindow = TRUE;
	m_bConncet = FALSE;
	m_bLaunchApp = FALSE;
	m_bStartMini = FALSE;
	m_nSleepRadio = 1;
    UpdateData(FALSE);
}*/


void CSetupPreferDlg::OnOK()
{
//     if(IDYES == AfxMessageBox(IDS_SAVETIP,MB_YESNO | MB_ICONINFORMATION))
//     {
    UpdateData(TRUE);
    WriteDataToFile();
    AutoStartupApp(g_SetData.Setup_nAutoLaunchApp);
    AfxMessageBox(IDS_SAVEOK, MB_ICONINFORMATION);
    OnCancel();
    //   }
//     else
//         ReadDataFromFile();
}

void CSetupPreferDlg::OnCancel()
{
    CBaseDialog::OnCancel();
}

/*CString CSetupPreferDlg::GetDefaultSMSSoundFilePath()
{
    TCHAR *ptr = NULL;
    TCHAR szAppPath[MAX_PATH];
    CString strAppPath;

    memset(szAppPath, 0x00, MAX_PATH);
    ::GetModuleFileName(NULL, szAppPath, MAX_PATH);

    ptr = szAppPath + wcslen(szAppPath) - 1;

    while(ptr >= szAppPath)
    {
        if(*ptr == '\\')
        {
            *ptr = '\0';
            break;
        }
        else
            ptr--;
    }

    wcsncat(szAppPath, _T("\\wav\\sms.wav"), MAX_PATH);

    strAppPath.Format(_T("%s"), szAppPath);
    return strAppPath;
}*/

BOOL CSetupPreferDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();
    m_bFirst = TRUE;
// 	SetSkin(g_SetData.Setup_nSkinStyle);
    ReadDataFromFile();
    AutoStartupApp(g_SetData.Setup_nAutoLaunchApp);

#ifdef FEATURE_VERSION_NOMENU
    GetDlgItem(IDC_BUTTON_PREFER_USAGELIMIT)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_BUTTON_PREFER_DIAGNOSTICS)->ShowWindow(SW_SHOW);
#endif

    if(!g_bSimReady || !g_bIsExist)
        GetDlgItem(IDC_CHECK_PREFER_CONNNET)->EnableWindow(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupPreferDlg::ReadDataFromFile()
{
    if((g_SetData.Setup_nAutoLaunchApp == 0 || g_SetData.Setup_nAutoLaunchApp == 1)
            && (g_SetData.Setup_nAutoConnect == 0 || g_SetData.Setup_nAutoConnect == 1)
            && (g_SetData.Setup_nStartMini == 0 || g_SetData.Setup_nStartMini == 1)
            && (g_SetData.Setup_nAllowSleep == 0 || g_SetData.Setup_nAllowSleep == 1)
            && (g_SetData.Messages_nAlertWindow == 0 || g_SetData.Messages_nAlertWindow == 1)
            && (g_SetData.Messages_nAlertTone == 0 || g_SetData.Messages_nAlertTone == 1)) {
        m_bLaunchApp = g_SetData.Setup_nAutoLaunchApp;
        m_bConncet = g_SetData.Setup_nAutoConnect;
        m_bStartMini = g_SetData.Setup_nStartMini;
        m_nSleepRadio = g_SetData.Setup_nAllowSleep;
        /*  m_bAlertWindow = g_SetData.Messages_nAlertWindow;
          m_bAlertTone = g_SetData.Messages_nAlertTone;
          if(wcslen(g_SetData.Messages_szSoundFile) > 0)
              m_strSmsSoundFile.Format(_T("%s"), g_SetData.Messages_szSoundFile);
          else
              m_strSmsSoundFile = GetDefaultSMSSoundFilePath();*/
    } else {
        //OnButtonDefault();
        WriteDataToFile();
    }
    UpdateData(FALSE);
}

void CSetupPreferDlg::WriteDataToFile()
{
    g_SetData.Setup_nAutoLaunchApp = m_bLaunchApp;
    g_SetData.Setup_nAutoConnect = m_bConncet;
    g_SetData.Setup_nStartMini = m_bStartMini;
    g_SetData.Setup_nAllowSleep = m_nSleepRadio;
//     g_SetData.Messages_nAlertWindow = m_bAlertWindow;
//     g_SetData.Messages_nAlertTone = m_bAlertTone;
//     memset(g_SetData.Messages_szSoundFile, 0x00, MAX_PATH);
//     wcsncpy(g_SetData.Messages_szSoundFile, m_strSmsSoundFile, MAX_PATH);
    SaveIniFile();
}

BOOL CSetupPreferDlg::AutoStartupApp(BOOL bFlag)
{
    HKEY hKey;
    LONG lRet;
    DWORD dwBufLen = MAX_PATH;
    char szPath[MAX_PATH];
    TCHAR szAppPath[MAX_PATH];
    CString strAppName;
    const TCHAR szRegRunKey[] = {_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")};

    DWORD dwDataSize;
    TCHAR ErrorBuf[1024];
    CString strErrMess;

    memset(szPath, 0x00, MAX_PATH);
    memset(szAppPath, 0x00, MAX_PATH);

    strAppName.LoadString(IDS_APPPRONAME);
    ::GetModuleFileName(NULL, szAppPath, MAX_PATH);

    lRet = RegOpenKeyEx(HKEY_CURRENT_USER, szRegRunKey, 0, KEY_ALL_ACCESS, &hKey);

    if(lRet == ERROR_SUCCESS) {
        if(bFlag) {
            lRet = RegSetValueEx(hKey, (LPCTSTR)strAppName, 0, REG_SZ, (BYTE*)szAppPath, wcslen(szAppPath));
            if(lRet != ERROR_SUCCESS) {
                strErrMess.Format(_T("\nError! code=%d"),lRet);
                AfxMessageBox(strErrMess);
                dwDataSize = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, lRet,
                                           0, ErrorBuf, 1024, NULL);
                strErrMess.Format(_T("\nError Message:%s"), ErrorBuf);
                //AfxMessageBox(strErrMess);
            }
        } else {
            lRet = RegQueryValueEx(hKey, (LPCTSTR)strAppName, NULL, NULL, (LPBYTE)szPath, &dwBufLen);

            if(lRet == ERROR_SUCCESS)
                lRet = RegDeleteValue(hKey, strAppName);
            else
                lRet = ERROR_SUCCESS;
        }
    }
    RegCloseKey(hKey);

    if(lRet == ERROR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}
