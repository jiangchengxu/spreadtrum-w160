// HSDPA.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "HSDPA.h"
#include "ResDllMiss.h"
#include "HSDPADlg.h"
#include "SplashScreenEx.h"
#include "Ds.h"
#include "Dev.h"

#include "ConnectionManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//static 声明
CWinThread * CHSDPAApp::MsgTrd;

/////////////////////////////////////////////////////////////////////////////
// CHSDPAApp

BEGIN_MESSAGE_MAP(CHSDPAApp, CWinApp)
    //{{AFX_MSG_MAP(CHSDPAApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHSDPAApp construction

CHSDPAApp::CHSDPAApp()
{
    m_pSerialPort = NULL;
    m_pDsThread = NULL;
    m_pBgThread = NULL;
    m_hNew = NULL;
    m_hOld = NULL;
    m_lstLang.RemoveAll();
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
//	m_PreMsgDlg = NULL;
}

CHSDPAApp::~CHSDPAApp()
{
    // TODO: add construction code here,
    DELETEIF(m_pConnProf);
    DELETEIF(m_pNdisConnProf);
    DELETEIF(m_pLogData);
    DELETEIF(m_pHistData);
    DELETEIF(m_pPbData);
    DELETEIF(m_pSmsData);
    DELETEIF(m_pSmsTemplateData);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHSDPAApp object

CHSDPAApp theApp;
//全局的MSGBOX

int CHSDPAApp::MsgBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
{
#if 1
    CMsgBox gMsgBox(0,lpszPrompt,nType,nIDPrompt);
    return gMsgBox.DoModal();

#else
    CMsgBox* pmsgbox = new CMsgBox();
    pmsgbox->Create(pmsgbox->IDD,NULL);
    //pmsgbox->SetText(lpszPrompt);
    pmsgbox->SetDlgItemText(IDC_STATIC_MSGBOX_INFO,lpszPrompt);
    pmsgbox->ShowWindow(SW_SHOW);

    /*
    if(WAIT_OBJECT_0 == WaitForSingleObject(pmsgbox->m_hMsgEvt, INFINITE)){
    	::CloseHandle(pmsgbox->m_hMsgEvt);
    	return 0;
    }else{
    	pmsgbox->DestroyWindow();
    	::CloseHandle(pmsgbox->m_hMsgEvt);
    	return 0;
    }
    */
    return pmsgbox->m_nRet;
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CHSDPAApp initialization

extern CHSDPADlg *pDsMainDlg;
extern CHSDPADlg *pSpMainDlg;

//typedef int(*lpGetFuncCall)();
typedef int(*lpGetLang)();
typedef int(*lpGetComp)();

void CHSDPAApp::SetCurDir()
{
    TCHAR szfilename[256] = {0};
    GetModuleFileName(NULL, szfilename, 256);
    CString strfilename(szfilename);
    CString strRunDir;
    strRunDir = strfilename.Left(strfilename.ReverseFind('\\'));
    SetCurrentDirectory(strRunDir);
}

//设置语言
BOOL CHSDPAApp::InitResource()
{
    //配置文件中的g_SetData.Main_nCall
    //系统语言
    //WORD wLangPID = PRIMARYLANGID( GetSystemDefaultLangID() );


    //枚举所有的资源dll


    CFileFind ff;
    CString szDir = _T(".\\lang");
    HINSTANCE hDll; //DLL句柄
    //lpGetFuncCall GetFuncCall; //函数指针
    lpGetLang GetLang;
    lpGetComp GetComp;
    //功能，语言都匹配
    BOOL bFindDll = FALSE;
    //功能匹配，语言不匹配
    BOOL bOtherDll = FALSE;


    CString strDllName=_T("");

    if(szDir.Right(1) != _T("\\"))
        szDir += _T("\\");

    szDir += _T("*.dll");

    BOOL res = ff.FindFile(szDir);
    while(res) {
        res = ff.FindNextFile();

        CString strPath = ff.GetFilePath();
        //得到路径，做为递归调用的开始
// 		CString strTitle = ff.GetFileName();
        //得到目录名

        //先匹配g_SetData.Main_nCompany
        hDll = LoadLibrary(strPath);
        //AfxMessageBox(strPath);

        if (hDll != NULL) {
            GetComp = (lpGetComp)GetProcAddress(hDll, "GetComp");
            GetLang = (lpGetLang)GetProcAddress(hDll, "GetLang");
            ASSERT(GetComp);
            ASSERT(GetLang);
            if (GetComp()==g_SetData.Main_nCompany) {
                //匹配语言
                if (GetLang() == g_SetData.Main_nLanguage) {
                    bFindDll = TRUE;
                    strDllName=strPath;
                }
                m_lstLang.AddTail(strPath);
            }
            //wyw_0104
            FreeLibrary(hDll);
        }
    }
    ff.Close();//关闭

    //特定dll是否存在
    if (bFindDll) {
        //yes，设置之
        SetResource(strDllName);
        return TRUE;
    } else {
        return FALSE;
    }
}

//释放语言DLL
void CHSDPAApp::SetResource(CString strResName)
{
    m_hOld = ::AfxGetResourceHandle();

    m_hNew = LoadLibrary(strResName);

    ::AfxSetResourceHandle(m_hNew);
}

const TCHAR szSwitchAppName[] = _T("switch.exe");


// @@@@@@
#ifdef OPEN_PAGE_UI
BOOL CHSDPAApp::InitInstance()
{
    // InitCommonControls() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    InitCommonControls();

    CWinApp::InitInstance();

    AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    // must call to use rich edit in dialog
    AfxInitRichEdit();

    CString str = _T(".\\lang\\Common_DataCrad.dll");
    AfxMessageBox(str);
    SetResource(str);

    ConnectionManagerDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    } else if (nResponse == IDCANCEL) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}
#else
BOOL CHSDPAApp::InitInstance()
{
    //SetCurDir();

    InitCommonControls();
    AfxInitRichEdit();

    LoadIniFile();

    if(!FirstInstance())
        return FALSE;

    if(!InitResource()) {
        CResDllMiss dlg;
        dlg.DoModal();
        exit(0);
    }

    if(!AfxSocketInit()) {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
    }

    AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

#ifdef _AFXDLL
    Enable3dControls();            // Call this when using MFC in a shared DLL
#else
Enable3dControlsStatic();    // Call this when linking to MFC statically
#endif

    //{{SHANG
    //显示初始化信息

    //{{wk
    m_pSerialPort = new CSerialPort;
    ASSERT(m_pSerialPort != NULL);

    CHSDPADlg dlg;
    pDsMainDlg = &dlg;
    pSpMainDlg = &dlg;

    DsEventCreate();

    //{{wk
    // @@@@@@
    m_pDsThread = AfxBeginThread(DsThreadProc, (LPVOID)m_pSerialPort);
    ASSERT(m_pDsThread != NULL);
    m_pBgThread = AfxBeginThread(BGThreadProc, (LPVOID)(&dlg));
    ASSERT(m_pBgThread != NULL);

    memset(gszHardwareID, 0x0000, MAX__DESC);
    swprintf(gszHardwareID, _T("vid_%s&pid_%s"), g_SetData.Main_szVID, g_SetData.Main_szPID);

    if(FindCdRom()) {
        DWORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;
        dwVersion = GetVersion();
        dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
        dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

        //if((dwWindowsMajorVersion == 5 && dwWindwosMinorVersion == 0) //Windows2000
        //  || (dwWindowsMajorVersion == 5 && dwWindwosMinorVersion == 1)) //WindowsXP
        ShellExecute(NULL, _T("open"), szSwitchAppName, NULL, NULL, SW_SHOWNORMAL);
        //else
        //  ShellExecute(NULL, "open", szDevConAppName, "H:\\", NULL, SW_SHOWNORMAL);
    }


    m_pPbData = new CPbData;
    m_pPbData->ReadDatas("PbData.dat");
    m_pPbData->GetGroups("PbGroup.dat");

    m_pSmsData = new CSmsData(_T("SmsData.dat"));
    m_pSmsData->LoadSmsData();

#ifdef FEATURE_BUTTON_TEMPLATE
    m_pSmsTemplateData = new CSmsData(_T("SmsTemplateData.dat"));
    m_pSmsTemplateData->LoadSmsData();
#endif

    //读通话记录
    if(g_SetData.Main_nCall) {
        m_pHistData = new CHistData;
        m_pHistData->LoadHistData();
    }

    m_pLogData = new CLogData;
    m_pLogData->LoadLogData();
    m_pConnProf = new CConnProfile;
    m_pConnProf->LoadConnProfile();
    m_pNdisConnProf = new CConnProfile(_T(""));
    m_pNdisConnProf->LoadConnProfile();

    dlg.m_bstatus = TRUE;

    m_pMainWnd = &dlg;
    int nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    } else if (nResponse == IDCANCEL) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    return FALSE;
}
#endif

//11-2
BOOL CHSDPAApp::FirstInstance()
{
    CString strTitle;
    CWnd *pWndPrev, *pWndChild;

    strTitle.LoadString(IDS_APPNAME);
    AddPrefixName(strTitle);

    m_hMutex = NULL;
    m_hMutex = ::CreateMutex(NULL, FALSE, (LPCTSTR)strTitle);

    if(m_hMutex == NULL || ERROR_ALREADY_EXISTS == ::GetLastError()) {
        if(pWndPrev = CWnd::FindWindow(_T("#32770"), strTitle)) {
            pWndChild = pWndPrev->GetLastActivePopup();
            pWndPrev->ShowWindow(SW_RESTORE);
            pWndChild->SetForegroundWindow();
        }
        if(m_hMutex != NULL) {
            ReleaseMutex(m_hMutex);
            m_hMutex = NULL;
        }
        return FALSE;
    } else {
        if(m_hMutex != NULL) {
            ReleaseMutex(m_hMutex);
            m_hMutex = NULL;
        }
        return TRUE;
    }

}
//11-2

void CHSDPAApp::WinHelp(DWORD dwData, UINT nCmd)
{
    // TODO: Add your specialized code here and/or call the base class

    //In main window, Press F1
    CWnd *pWndPrev, *pWndChild;
    CString str, strHelpTitle;
    str.LoadString(IDS_APPNAME);
    AddPrefixName(str);
    strHelpTitle.Format(_T("%s %s"), str, _T("Help"));

    if(pWndPrev = CWnd::FindWindow(NULL, strHelpTitle)) {
        pWndChild = pWndPrev->GetLastActivePopup();
        pWndPrev->ShowWindow(SW_RESTORE);
        pWndChild->SetForegroundWindow();
    } else {
        //Press Help button

        //CString str = AfxGetApp()->m_pszHelpFilePath;
        //int iPos = str.ReverseFind('\\');
        CString    strHelpFile;// = str.Left(iPos);
        //strHelpFile += "\\DataCardHelp.chm";
        //strHelpFile = "DataCardHelp.chm";
        strHelpFile=g_SetData.Main_szHelpFile;

        CString CInfo;
        if( (_waccess( strHelpFile, 0 )) == -1 ) {
            CInfo.LoadString(IDS_HELPINFO);
            AfxMessageBox(CInfo);
        }

        ShellExecute(NULL, _T("open"),strHelpFile,    NULL, NULL, SW_SHOWNORMAL);
    }
}

int CHSDPAApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
{
    //CString info = lpszPrompt;

    //return MsgBox(lpszPrompt, nType, nIDPrompt);

    return CWinApp::DoMessageBox(lpszPrompt, nType, nIDPrompt);
}

int CHSDPAApp::ExitInstance()
{
    if(m_hMutex != NULL)
        ::CloseHandle(m_hMutex);

    CloseWorkThreads();

    if (m_hNew) {
        FreeLibrary(m_hNew);
    }

    return CWinApp::ExitInstance();
}

void CHSDPAApp::CloseWorkThreads()
{
    //{{wk
    if(m_pDsThread) {
        // @@@@@@
        ::SetEvent(g_AppRegEvt);

        ::SetEvent(g_EndEvent);

        // @@@@@@
        ::WaitForSingleObject(m_pDsThread,INFINITE);

        //wyw: m_pDsThread在线程结束时会自动释放
        //::CloseHandle(m_pDsThread);

        m_pDsThread = NULL;
    }
    if(m_pBgThread) {
        ::SetEvent(g_BGEvtArr[BGEVT_END]);

        // @@@@@@
        ::WaitForSingleObject(m_pBgThread,INFINITE);

        //wyw: m_pBgThread在线程结束时会自动释放
        //::CloseHandle(m_pBgThread);

        m_pBgThread = NULL;
    }
    if(m_pSerialPort) {
        m_pSerialPort->StopPort();
        DELETEIF(m_pSerialPort);
    }
    //}}wk
}

EnOpenPortRet CHSDPAApp::OpenPort(BOOL bFirstTime, BYTE nRunTimes, USHORT nMilliSecond)	//wyw_0115 modify
{
    BYTE cnt = 0;
    EnOpenPortRet ret = OPENPORT_RET_NOTFOUND;

    memset(m_szDevName, 0, sizeof(m_szDevName));

    for(cnt = 0; cnt < nRunTimes; cnt++) {
        int i = 0;
        while (i<10) {
            MSG msg;
            while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE)) {
                if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
                    break;
                DispatchMessage(&msg);
            }

            Sleep(nMilliSecond/10);

            if(GetPCUIPortName(m_szDevName)) {
                ret = OPENPORT_RET_FAIL;
                Sleep(5000);
                break;
            }
            i++;
        }
        if(ret == OPENPORT_RET_FAIL) {
            break;
        }
    }

    if(ret == OPENPORT_RET_FAIL) {
        for(cnt = 0; cnt < nRunTimes; cnt++) {
            int i = 0;
            while (i<10) {
                MSG msg;
                while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE)) {
                    if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
                        break;
                    DispatchMessage(&msg);
                }

                Sleep(nMilliSecond/10);

                if(m_pSerialPort->StartPort(m_szDevName, !bFirstTime)) {
                    ret = OPENPORT_RET_SUCC;
                    break;
                }
                i++;
            }
            if (ret == OPENPORT_RET_SUCC) {
                break;
            }
        }
    }

    return ret;
}

CString CHSDPAApp::GetOpenPortTip(EnOpenPortRet ret)
{
    CString strTip;

    switch(ret) {
    case OPENPORT_RET_NOTFOUND:
        //strTip.LoadString(IDS_USB_NOTFIND);
        strTip.LoadString(IDS_START_INSERTPCCARD);
        break;
    case OPENPORT_RET_SUCC:
        strTip.LoadString(IDS_USB_START_SUCC);
        break;
    case OPENPORT_RET_FAIL:
        strTip.LoadString(IDS_USB_START_FAIL);
        break;
    }
    return strTip;
}

//启动程序后发送RESET命令，使数据卡进入ONLINE模式
BOOL CHSDPAApp::SndResetCmd()
{
    char szAtBuf[30] = {0};
    strcpy(szAtBuf, "AT+PHMOD=7\r");

    if(m_pSerialPort->WriteToPort(szAtBuf, strlen(szAtBuf))) {
//		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespDummy, (LPVOID)this);
        m_pSerialPort->StopPort();
        return TRUE;
    }
    return FALSE;
}
