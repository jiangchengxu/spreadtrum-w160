// HSDPA.h : main header file for the HSDPA application
//

#if !defined(AFX_HSDPA_H__E50FCAC9_1970_4995_A3CB_21E8F1460566__INCLUDED_)
#define AFX_HSDPA_H__E50FCAC9_1970_4995_A3CB_21E8F1460566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include  <io.h>
#include  <stdio.h>
#include "resource.h"        // main symbols
#include "CommonStructs.h"
#include "MsgThread.h"
#include "SmsData.h"
#include "PbData.h"
#include "HistData.h"
#include "LogData.h"
#include "ConnProfile.h"
#include "PreMsg.h"
#include "mmsystem.h"
#include "SerialPort/SerialPort.h"

//#include "DlgTracker.h"
#include "MsgBox.h"
/////////////////////////////////////////////////////////////////////////////
// CHSDPAApp:
// See HSDPA.cpp for the implementation of this class
//

class CHSDPAApp : public CWinApp
{
private:
	HANDLE m_hMutex;
    CLogData  *m_pLogData;
    CHistData *m_pHistData;
    CSmsData  *m_pSmsData;
	CSmsData  *m_pSmsTemplateData;
    CPbData   *m_pPbData;
    CConnProfile *m_pConnProf;
    CConnProfile *m_pNdisConnProf;
public:
	int MsgBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
    /*Interface Function*/

    CConnProfile *GetConnProfile(void){
        return m_pConnProf;
    }
    CConnProfile *GetNdisConnProfile(void){
        return m_pNdisConnProf;
    }
    CLogData *GetLogData(void){
        return m_pLogData;
    }
    CHistData *GetHistData(void){
        return m_pHistData;
    }
    CSmsData * GetSmsData(void){
        return m_pSmsData;
    }
	CSmsData * GetSmsTemplateData(void){
        return m_pSmsTemplateData;
    }
    CPbData * GetPbData(void){
        return m_pPbData;
    }
	BOOL InitResource();
	void SetResource(CString strResName);
public:
	CStringList m_lstLang;
	BOOL SndResetCmd();
	CString GetOpenPortTip(EnOpenPortRet ret);
	EnOpenPortRet OpenPort(BOOL bFirstTime, BYTE nRunTimes, USHORT nMilliSecond);
	EnOpenPortRet OpenPort_Voice(BOOL bFirstTime, BYTE nRunTimes, USHORT nMilliSecond);//wj add test
    TCHAR m_szDevName[50];
    TCHAR m_szDevName_Voice[50];//wj add test
    BYTE m_CardState; //0:ÎÞ¿¨ 1:ÓÐ¿¨
    CSerialPort* m_pSerialPort;
    CWinThread* m_pDsThread;
    CWinThread* m_pBgThread;
    CHSDPAApp();
    ~CHSDPAApp();
    void SetCurDir();
    BOOL FirstInstance();
    void CloseWorkThreads();
	HINSTANCE m_hNew;
	HINSTANCE m_hOld;
    
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHSDPAApp)
	public:
    virtual BOOL InitInstance();
    virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
    static CWinThread *MsgTrd;

// Implementation

    //{{AFX_MSG(CHSDPAApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};
/*
inline CHistData *CHSDPAApp::GetHistData(){
    return m_pHistData;
}
inline CSmsData * CHSDPAApp::GetSmsData(){
return m_pSmsData;
}
*/
extern CHSDPAApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSDPA_H__E50FCAC9_1970_4995_A3CB_21E8F1460566__INCLUDED_)





















