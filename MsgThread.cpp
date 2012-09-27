// MsgThread.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "MsgThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgThread

IMPLEMENT_DYNCREATE(CMsgThread, CWinThread)

CMsgThread::CMsgThread()
{
}

CMsgThread::~CMsgThread()
{
}

BOOL CMsgThread::InitInstance()
{
    // TODO:  perform and per-thread initialization here
    return TRUE;
}

int CMsgThread::ExitInstance()
{
    // TODO:  perform any per-thread cleanup here
    return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMsgThread, CWinThread)
    //{{AFX_MSG_MAP(CMsgThread)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgThread message handlers

//空闲时
BOOL CMsgThread::OnIdle(LONG lCount)
{
    // TODO: Add your specialized code here and/or call the base class
    int i;
    for(i=0; i<50000; i++);
#if 0
//开始操作提示框，函数参数
//NIM_ADD  Adds an icon to the status area.
//NIM_DELETE
//NIM_MODIFY
//    ::Shell_NotifyIcon(NIM_ADD,&m_tnid);

//填充NOTIFYICONDATA结构体
    NOTIFYICONDATA m_tnid;
    m_tnid.cbSize = sizeof(NOTIFYICONDATA);
    m_tnid.hWnd = CHSDPADlg->GetHandle();//估计不能是this
    m_tnid.uID = IDI_ICON_INFO;
    m_tnid.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
    m_tnid.uCallbackMessage = WM_TRAYICON;
    HICON hIcon;
    hIcon = ::LoadIcon(GetHInstance(), MAKEINTRESOURCE(IDI_ICON_INFO));
    m_tnid.hIcon = hIcon;
    szToolTip = "ECast";
    _tcscpy(m_tnid.szTip, szToolTip.c_str());
#endif
    return CWinThread::OnIdle(lCount);
}
