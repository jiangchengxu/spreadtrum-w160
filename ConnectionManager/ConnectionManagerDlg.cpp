// --------------------------------------------------------------------------
//
// ConnectionManagerDlg.cpp
//
/// ConnectionManagerDlg implementation.
///
/// @file
//
// Copyright (c) 2006 QUALCOMM Inc.
//
// All Rights Reserved.  QUALCOMM Proprietary
//
// Export of this technology or software is regulated by the U.S. Government.
// Diversion contrary to U.S. law prohibited.
//
// --------------------------------------------------------------------------

#include "stdafx.h"
#include <sstream>

#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

// @@
#if 0
#include "AboutDlg.h"
#endif

// @@
//#include "ConnectionManager.h"

#include "ConnectionManagerDlg.h"
#include "MessageManager.h"
#include "WDSSetEventReportMsg.h"
#include "DMSSetEventReportMsg.h"
#include "NASSetEventReportMsg.h"
#include "DMSGetPowerStateMsg.h"
#include "DMSGetDeviceMfrMsg.h"
#include "DMSGetDeviceModelIdMsg.h"
#include "DMSGetDeviceRevIdMsg.h"
#include "DMSGetMsisdnMsg.h"
#include "DMSGetDeviceSerialNumbersMsg.h"
#include "DMSGetDeviceCapMsg.h"
#include "NASGetSignalStrengthMsg.h"
#include "NASGetServingSystemMsg.h"
#include "WDSAbortMsg.h"
#include "WDSStartNetworkInterfaceMsg.h"
#include "WDSStopNetworkInterfaceMsg.h"
#include "WDSGetPktSrvcStatusMsg.h"
#include "WDSGetCurrentChannelRateMsg.h"
#include "WDSCreateProfileMsg.h"
#include "WDSDeleteProfileMsg.h"
#include "WDSModifyProfileSettingsMsg.h"
#include "WDSGetProfileListMsg.h"
#include "WDSGetProfileSettingsMsg.h"
#include "WDSGetDefaultSettingsMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// user defined windows message for new notice received
const static uint16 WM_NEW_NOTICE = 0x0401;

// constants used to determine whether or not to display an AfxMsgBox
enum
{
    NO_MSG_BOX = 0,
    MSG_BOX = 1
};


// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ConnectionManagerDlg::ConnectionManagerDlg(CWnd* pParent /*=NULL*/)	:
    CDialog(ConnectionManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// --------------------------------------------------------------------------
// dtor
// --------------------------------------------------------------------------
ConnectionManagerDlg::~ConnectionManagerDlg()
{
    // stop the message manager
    MessageManager::GetInstance().StopMessageManager();
}

// --------------------------------------------------------------------------
// DoDataExchange
// --------------------------------------------------------------------------
void ConnectionManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ConnectionManagerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_NEW_NOTICE,OnNewNotice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL ConnectionManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
        CString strUsersGuideMenu;
		strUsersGuideMenu.LoadString(IDS_USER_GUIDE);
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);

        if (!strAboutMenu.IsEmpty() || !strUsersGuideMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
        }

        if (!strAboutMenu.IsEmpty())
		{
			// @@
#if 0
			pSysMenu->AppendMenu(MF_STRING, IDM_USER_GUIDE, strUsersGuideMenu);
#else
			#define IDM_USER_GUIDE                  0x0020
			pSysMenu->AppendMenu(MF_STRING, IDM_USER_GUIDE, strUsersGuideMenu);
#endif
		}

		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_connectionPage.SetCMDlg(this);
    m_profilesPage.SetCMDlg(this);
    m_statisticsPage.SetCMDlg(this);
    m_statusPage.SetCMDlg(this);

	// add pages to sheet
    m_propSheet.AddPage(&m_connectionPage);
    m_propSheet.AddPage(&m_profilesPage);
// !!!    m_propSheet.AddPage(&m_simPage);
    m_propSheet.AddPage(&m_statisticsPage);
    m_propSheet.AddPage(&m_statusPage);

    // create sheet
    m_propSheet.Create(this, WS_CHILD | WS_VISIBLE);

    // causes dodataexchange call, linking controls to data members
    m_propSheet.SetActivePage(&m_statusPage);
    m_propSheet.SetActivePage(&m_statisticsPage);
    m_propSheet.SetActivePage(&m_profilesPage);
    m_propSheet.SetActivePage(&m_connectionPage);

    // subscribe to MessageManager publications
    SubscribeTo(MessageManager::GetInstance());

    // start the MessageManager
    MessageManager::GetInstance().StartMessageManager();
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// --------------------------------------------------------------------------
// OnClose
// --------------------------------------------------------------------------
void ConnectionManagerDlg::OnClose()
{
    // make sure the device is terminated before closing windows
    MessageManager::GetInstance().TerminateDevice();

    CDialog::OnClose();
}

// --------------------------------------------------------------------------
// OnSysCommand
// --------------------------------------------------------------------------
void ConnectionManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
// @@
#if 0
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
    else if ((nID & 0xFFF0) == IDM_USER_GUIDE)
#else
	if ((nID & 0xFFF0) == IDM_USER_GUIDE)
#endif
	{
        // try to open in production path
        if(OpenUserGuide("\\QMICMUserGuide.pdf")) return;

        // try to open in perforce path
        if (!OpenUserGuide("..\\..\\Documents\\QMICMUserGuide.pdf"))
        {
            AfxMessageBox(_T("QMICM User Guide Unavailable"));
        }
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// --------------------------------------------------------------------------
// OpenUserGuide
//
/// Open the QMICM User Guide pdf
///
/// @param relativePath - string indicating the relative path to the pdf.
///
/// @returns bool - true if pdf successfully opened.
// --------------------------------------------------------------------------
bool ConnectionManagerDlg::OpenUserGuide(const char* relativePath)
{
    std::string modulePath;
    std::string path;
    std::string temp;

    // get the path to the exe


	char filePath[MAX_PATH];

    ::GetModuleFileNameA(NULL, filePath, MAX_PATH);
    // get the long path to the exe
    char longPath[MAX_PATH];
    if(0 < ::GetLongPathNameA(filePath, longPath, MAX_PATH))
    {
        temp = longPath;
    }
    else
    {
        temp =  filePath;
    }


    // trim off the exe
    path = modulePath =  temp.substr(0, temp.find_last_of("\\/"));

    // make sure we have a separator
    if(relativePath[0] != '\\')
    {
        path += '\\';
    }

    // add relative path to path
    path += relativePath;

    // open the qmicm user guide

    int result = (int)ShellExecuteA(AfxGetMainWnd()->m_hWnd,
                                  "open",
                                  path.c_str(),
                                  NULL,
                                  modulePath.c_str(),
                                  SW_SHOWNORMAL);

    // greater than 32 if successful

    return (result > 32);
	

}

// --------------------------------------------------------------------------
// OnPaint
//
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// --------------------------------------------------------------------------
// OnQueryDragIcon
//
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
// --------------------------------------------------------------------------
HCURSOR ConnectionManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// --------------------------------------------------------------------------
// SelectDevice
//
/// SelectDevice establishes the device name with the message manager and
/// collects information about that device to be displayed.
///
/// @param deviceName - string name of the device to connect to.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::SelectDevice(std::string& deviceName)
{
    BeginWaitCursor();
    MessageManager::GetInstance().EstablishDevice(deviceName);
}

// --------------------------------------------------------------------------
// DeselectDevice
//
/// DeselectDevice coordinates with MessageManager to disable an active
/// device and removes information about that device from the gui.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::DeselectDevice()
{
    BeginWaitCursor();
    MessageManager::GetInstance().TerminateDevice();
}

// --------------------------------------------------------------------------
// SendRequest
//
/// SendRequest takes a string representation of a Message and coordinates 
/// with the MessageManager to send the request to the device.
///
/// @param msgStr - string representation of a Message.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::SendRequest(std::string& msgStr)
{
    // send request to MessageManager
    if (!msgStr.empty())
    {
        MessageManager::GetInstance().SendMessage(msgStr);
    }
}

// --------------------------------------------------------------------------
// OnPublisherNotify
//
/// Actions to take when a NoticeRCP is received from a NoticePublisher. Put
/// the NoticeRCP on the notice queue and post a message that a new notice
/// has been received. Notices can be sent from multiple threads and the
/// queue/post paradigm ensures that only the main thread processes notices. 
// --------------------------------------------------------------------------
void ConnectionManagerDlg::OnPublisherNotify
(
    const Publisher<NoticeRCP>& publisher,
    const NoticeRCP& noticeRCP
)
{
    // add the notice reference counting poiniter to the notice queue
    m_noticeRCPs.push(noticeRCP);

    // post a message indicating that a new notice has been received
    ::PostMessage(m_hWnd,WM_NEW_NOTICE,NULL,NULL);
}

// --------------------------------------------------------------------------
// OnNewNotice
//
/// Actions to take when a new notice has been placed on the Notice queue.
// --------------------------------------------------------------------------
LRESULT ConnectionManagerDlg::OnNewNotice(WPARAM wParam,LPARAM lParam)
{
    NoticeRCP noticeRCP = m_noticeRCPs.front();
    m_noticeRCPs.pop();

    // get the notice type
    uint8 noticeType = noticeRCP->GetNoticeType();


    switch(noticeType)
    {
        case NT_MESSAGE:
            {
            ProcessMsgNotice(noticeRCP);
            break;
            }
        case NT_STATUS:
            ProcessStatusNotice(noticeRCP);
            break;
        case NT_DEVICE:
            ProcessDeviceNotice(noticeRCP);
            break;
        default:
            std::stringstream stream;
 USES_CONVERSION;
			stream << "Error: Unknown notice type received:" << std::endl
                   << "  Notice Type: " << (int)noticeType << std::endl;
            AfxMessageBox(A2W(stream.str().c_str()));
    }

    return NULL;
}

// --------------------------------------------------------------------------
// ProcessMsgNotice
//
/// Actions to take when a MsgNotice is received from the MessageManager.
///
/// @params noticeRCP - Notice reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessMsgNotice(NoticeRCP noticeRCP)
{
    MsgNoticeRCP msgNoticeRCP = rcp_cast<MsgNotice*>(noticeRCP);
    MessageRCP msgRCP = msgNoticeRCP->GetMessageRCP();
    uint8 svcType = msgRCP->GetSvcType();
    uint8 ctlType = msgRCP->GetCtlType();
    uint16 msgType = msgRCP->GetMsgType();

    // display msg on status page
    LogMessage(msgRCP);

    // compose UID
    uint32 msgUID = (svcType << 24 | ctlType << 16 | msgType);

    // switch on control type
    switch (ctlType)
    {
        case QMI_CTL_FLAG_TYPE_CMD:
            ProcessReqMsgNotice(msgRCP,msgUID);
            break;
        case QMI_CTL_FLAG_TYPE_RSP:
            ProcessRspMsgNotice(msgRCP,msgUID);
            break;
        case QMI_CTL_FLAG_TYPE_IND:
            ProcessIndMsgNotice(msgRCP,msgUID);
            break;
        default:
            // control type not recognized

            std::stringstream stream;
            stream.str("");
            stream << "Error: Unknown message control type received:" << std::endl
                   << "  Control Type: " << (int)ctlType << std::endl
                   << std::endl;         
			USES_CONVERSION;

            AfxMessageBox(A2W(stream.str().c_str()));
            m_statusPage.AddEventMessage(stream.str(),ST_ERROR);

            break;
    }
}

// --------------------------------------------------------------------------
// ProcessReqMsgNotice
//
/// Actions to take when a request message is received from the MessageManager.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessReqMsgNotice(MessageRCP msgRCP,uint32 msgUID)
{
    // switch on unique message id
    switch (msgUID)
    {
        case WDSStartNetworkInterfaceReqUID:
            ProcessWDSStartReq(msgRCP);
            break;
        case WDSDeleteProfileReqUID:
            ProcessWDSDeleteProfileReq(msgRCP);
            break;
        case WDSGetProfileSettingsReqUID:
            ProcessWDSGetProfileSettingsReq(msgRCP);
            break;
        case WDSGetDefaultSettingsReqUID:
            ProcessWDSGetDefaultSettingsReq(msgRCP);
            break;
        default:
            // most requests do not need processing
            break;
    }
}

// --------------------------------------------------------------------------
// ProcessRspMsgNotice
//
/// Actions to take when a response message is received from the 
/// MessageManager.
///
/// @params msgRCP - Notice reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessRspMsgNotice(MessageRCP msgRCP,uint32 msgUID)
{
    // switch on unique message id
    switch (msgUID)
    {
        case WDSSetEventReportRspUID:
            ProcessWDSSetEventReportRsp(msgRCP);
            break;
        case DMSSetEventReportRspUID:
            ProcessDMSSetEventReportRsp(msgRCP);
            break;
        case NASSetEventReportRspUID:
            ProcessNASSetEventReportRsp(msgRCP);
            break;
        case DMSGetPowerStateRspUID:
            ProcessDMSGetPowerStateRsp(msgRCP);
            break;
        case DMSGetDeviceMfrRspUID:
            ProcessDMSGetDeviceMfrRsp(msgRCP);
            break;
        case DMSGetDeviceModelIdRspUID:
            ProcessDMSGetDeviceModelIdRsp(msgRCP);
            break;
        case DMSGetDeviceRevIdRspUID:
            ProcessDMSGetDeviceRevIdRsp(msgRCP);
            break;
        case DMSGetMsisdnRspUID:
            ProcessDMSGetMsisdnRsp(msgRCP);
            break;
        case DMSGetDeviceSerialNumbersRspUID:
            ProcessDMSGetDeviceSerialNumbersRsp(msgRCP);
            break;
        case DMSGetDeviceCapRspUID:
            ProcessDMSGetDeviceCapRsp(msgRCP);
            break;
        case NASGetSignalStrengthRspUID:
            ProcessNASGetSignalStrengthRsp(msgRCP);
            break;
        case NASGetServingSystemRspUID:
            ProcessNASGetServingSystemRsp(msgRCP);
            break;
        case WDSAbortRspUID:
            ProcessWDSAbortRsp(msgRCP);
            break;
        case WDSStartNetworkInterfaceRspUID:
            ProcessWDSStartRsp(msgRCP);
            break;
        case WDSStopNetworkInterfaceRspUID:
            ProcessWDSStopRsp(msgRCP);
            break;
        case WDSGetCurrentChannelRateRspUID:
            ProcessWDSGetCurrentChannelRateRsp(msgRCP);
            break;
        case WDSCreateProfileRspUID:
            ProcessWDSCreateProfileRsp(msgRCP);
            break;
        case WDSModifyProfileSettingsRspUID:
            ProcessWDSModifyProfileSettingsRsp(msgRCP);
            break;
        case WDSDeleteProfileRspUID:
            ProcessWDSDeleteProfileRsp(msgRCP);
            break;
        case WDSGetProfileListRspUID:
            ProcessWDSGetProfileListRsp(msgRCP);
            break;
        case WDSGetProfileSettingsRspUID:
            ProcessWDSGetProfileSettingsRsp(msgRCP);
            break;
        case WDSGetDefaultSettingsRspUID:
            ProcessWDSGetDefaultSettingsRsp(msgRCP);
            break;
        default:
            // msgUID not recognized, details in status
USES_CONVERSION;
            std::stringstream stream;
            stream << "Error: Unknown message type received:" << std::endl
                   << "  Service Type: " << SERVICE_TYPE_STRINGS[msgRCP->GetSvcType()] << std::endl
                   << "  Control Type: " << (int)msgRCP->GetCtlType() << std::endl
                   << "  Message Type: " << (int)msgRCP->GetMsgType() << std::endl
                   << std::endl;                   
            AfxMessageBox(A2W(stream.str().c_str()));
            m_statusPage.AddEventMessage(stream.str(),ST_ERROR);

            break;
    }
}

// --------------------------------------------------------------------------
// ProcessIndMsgNotice
//
/// Actions to take when an indicator message is received from the 
/// MessageManager.
///
/// @params msgRCP - Notice reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessIndMsgNotice(MessageRCP msgRCP,uint32 msgUID)
{
    // switch on unique message id
    switch (msgUID)
    {
        case WDSEventReportIndUID:
            ProcessWDSEventReportInd(msgRCP);
            break;
        case DMSEventReportIndUID:
            ProcessDMSEventReportInd(msgRCP);
            break;
        case NASEventReportIndUID:
            ProcessNASEventReportInd(msgRCP);
            break;
        case NASServingSystemIndUID:
            ProcessNASServingSystemInd(msgRCP);
            break;
        case WDSPktSrvcStatusIndUID:
            ProcessWDSPktSrvcStatusInd(msgRCP);
            break; 
        default:
            // msgUID not recognized, details in status

            std::stringstream stream;
            stream << "Error: Unknown message type received:" << std::endl
                   << "  Service Type: " << SERVICE_TYPE_STRINGS[msgRCP->GetSvcType()] << std::endl
                   << "  Control Type: " << (int)msgRCP->GetCtlType() << std::endl
                   << "  Message Type: " << (int)msgRCP->GetMsgType() << std::endl
                   << std::endl;     
			USES_CONVERSION;
            AfxMessageBox(A2W(stream.str().c_str()));
            m_statusPage.AddEventMessage(stream.str(),ST_ERROR);

            break;
    }
}

// --------------------------------------------------------------------------
// ProcessWDSStartReq
//
/// Actions to take when a WDSStartNetworkInterfaceReq is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSStartReq(MessageRCP msgRCP)
{
    WDSStartNetworkInterfaceReqRCP reqRCP = 
        rcp_cast<WDSStartNetworkInterfaceReq*>(msgRCP);

    // set the start message transaction id in connection page (for abort)
    m_connectionPage.SetTxId(reqRCP->GetTxId());
}

// --------------------------------------------------------------------------
// ProcessWDSDeleteProfileReq
//
/// Actions to take when a WDSDeleteProfileReq is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSDeleteProfileReq(MessageRCP msgRCP)
{
    WDSDeleteProfileReqRCP reqRCP = rcp_cast<WDSDeleteProfileReq*>(msgRCP);

    // call ProfilesPage process method
    m_profilesPage.ProcessWDSDeleteProfileReq(reqRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileSettingsReq
//
/// Actions to take when a WDSGetProfileSettingsReq is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSGetProfileSettingsReq(MessageRCP msgRCP)
{
    WDSGetProfileSettingsReqRCP reqRCP = 
        rcp_cast<WDSGetProfileSettingsReq*>(msgRCP);

    // call ProfilesPage process method
    m_profilesPage.ProcessWDSGetProfileSettingsReq(reqRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetDefaultSettingsReq
//
/// Actions to take when a WDSGetDefaultSettingsReq is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSGetDefaultSettingsReq(MessageRCP msgRCP)
{
    WDSGetDefaultSettingsReqRCP reqRCP = 
        rcp_cast<WDSGetDefaultSettingsReq*>(msgRCP);

    // call ProfilesPage process method
    m_profilesPage.ProcessWDSGetDefaultSettingsReq(reqRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSSetEventReportRsp
//
/// Actions to take when a WDSSetEventReportRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSSetEventReportRsp(MessageRCP msgRCP)
{
    WDSSetEventReportRspRCP rspRCP = rcp_cast<WDSSetEventReportRsp*>(msgRCP);

    // verify msg successful

    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSSetEventReportReq",rspRCP->GetError(),NO_MSG_BOX);
    }
}

// --------------------------------------------------------------------------
// ProcessDMSSetEventReportRsp
//
/// Actions to take when a DMSSetEventReportRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDMSSetEventReportRsp(MessageRCP msgRCP)
{
    DMSSetEventReportRspRCP rspRCP = rcp_cast<DMSSetEventReportRsp*>(msgRCP);

    // verify msg successful

    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSSetEventReportReq",rspRCP->GetError(),NO_MSG_BOX);
    }

}

// --------------------------------------------------------------------------
// ProcessNASSetEventReportRsp
//
/// Actions to take when a NASSetEventReportRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessNASSetEventReportRsp(MessageRCP msgRCP)
{
    NASSetEventReportRspRCP rspRCP = rcp_cast<NASSetEventReportRsp*>(msgRCP);

    // verify msg successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("NASSetEventReportReq",rspRCP->GetError(),NO_MSG_BOX);
    }
}

// --------------------------------------------------------------------------
// ProcessWDSEventReportInd
//
/// Actions to take when a WDSEventReportInd message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSEventReportInd(MessageRCP msgRCP)
{
    WDSEventReportIndRCP indRCP = rcp_cast<WDSEventReportInd*>(msgRCP);

    // call StatisticsPage process method
    m_statisticsPage.ProcessWDSEventReportInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSEventReportInd
//
/// Actions to take when a DMSEventReportInd message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDMSEventReportInd(MessageRCP msgRCP)
{
    DMSEventReportIndRCP indRCP = rcp_cast<DMSEventReportInd*>(msgRCP);

    // call ConnectionPage process method
    m_connectionPage.ProcessDMSEventReportInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessNASEventReportInd
//
/// Actions to take when a NASEventReportInd message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessNASEventReportInd(MessageRCP msgRCP)
{
    NASEventReportIndRCP indRCP = rcp_cast<NASEventReportInd*>(msgRCP);

    // call ConnectionPage process method
    m_connectionPage.ProcessNASEventReportInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetPowerStateRsp
//
/// Actions to take when a DMSGetPowerStateRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDMSGetPowerStateRsp(MessageRCP msgRCP)
{
    DMSGetPowerStateRspRCP rspRCP = rcp_cast<DMSGetPowerStateRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetPowerStateReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_connectionPage.ProcessDMSGetPowerStateRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceMfrRsp
//
/// Actions to take when a DMSGetDeviceMfrRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDMSGetDeviceMfrRsp(MessageRCP msgRCP)
{
    DMSGetDeviceMfrRspRCP rspRCP = rcp_cast<DMSGetDeviceMfrRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceMfrReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceMfrRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceModelIdRsp
//
/// Actions to take when a DMSGetDeviceModelIdRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDMSGetDeviceModelIdRsp(MessageRCP msgRCP)
{
    DMSGetDeviceModelIdRspRCP rspRCP = rcp_cast<DMSGetDeviceModelIdRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceModelIdReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceModelIdRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceRevIdRsp
//
/// Actions to take when a DMSGetDeviceRevIdRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDMSGetDeviceRevIdRsp(MessageRCP msgRCP)
{
    DMSGetDeviceRevIdRspRCP rspRCP = rcp_cast<DMSGetDeviceRevIdRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceRevIdReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceRevIdRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetMsisdnRsp
//
/// Actions to take when a DMSGetMsisdnRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDMSGetMsisdnRsp(MessageRCP msgRCP)
{
    DMSGetMsisdnRspRCP rspRCP = rcp_cast<DMSGetMsisdnRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetMsisdnReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetMsisdnRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceSerialNumbersRsp
//
/// Actions to take when a DMSGetDeviceSerialNumbersRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDMSGetDeviceSerialNumbersRsp(MessageRCP msgRCP)
{
    DMSGetDeviceSerialNumbersRspRCP rspRCP = rcp_cast<DMSGetDeviceSerialNumbersRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceSerialNumbersReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceSerialNumbersRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceCapRsp
//
/// Actions to take when a DMSGetDeviceCapRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDMSGetDeviceCapRsp(MessageRCP msgRCP)
{
    DMSGetDeviceCapRspRCP rspRCP = rcp_cast<DMSGetDeviceCapRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceCapReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceCapRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessNASGetSignalStrengthRsp
//
/// Actions to take when a NASGetSignalStrengthRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessNASGetSignalStrengthRsp(MessageRCP msgRCP)
{
    NASGetSignalStrengthRspRCP rspRCP = rcp_cast<NASGetSignalStrengthRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("NASGetSignalStrengthReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_connectionPage.ProcessNASGetSignalStrengthRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessNASGetServingSystemRsp
//
/// Actions to take when a NASGetServingSystemRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessNASGetServingSystemRsp(MessageRCP msgRCP)
{
    NASGetServingSystemRspRCP rspRCP = rcp_cast<NASGetServingSystemRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("NASGetServingSystemReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_connectionPage.ProcessNASGetServingSystemRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessNASServingSystemInd
//
/// Actions to take when a NASServingSystemInd message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessNASServingSystemInd(MessageRCP msgRCP)
{
    NASServingSystemIndRCP indRCP = rcp_cast<NASServingSystemInd*>(msgRCP);

    // call Page process methods
    m_connectionPage.ProcessNASServingSystemInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSAbortRsp
//
/// Actions to take when a WDSAbortRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSAbortRsp(MessageRCP msgRCP)
{
    WDSAbortRspRCP rspRCP = rcp_cast<WDSAbortRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSAbortReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page enable methods
    m_connectionPage.DisableConnectedState();
}

// --------------------------------------------------------------------------
// ProcessWDSStartRsp
//
/// Actions to take when a WDSStartNetworkInterfaceRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSStartRsp(MessageRCP msgRCP)
{
    WDSStartNetworkInterfaceRspRCP rspRCP = 
        rcp_cast<WDSStartNetworkInterfaceRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSStartNetworkInterfaceReq",rspRCP->GetError(),MSG_BOX);

        // call Page disable methods
        m_connectionPage.DisableConnectedState();
        return;
    }

    // call Page enable methods
    m_connectionPage.EnableConnectedState(rspRCP->GetPktDataHandle());
    m_statisticsPage.EnableConnectedState();
}

// --------------------------------------------------------------------------
// ProcessWDSStopRsp
//
/// Actions to take when a WDSStopNetworkInterfaceRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSStopRsp(MessageRCP msgRCP)
{
    WDSStopNetworkInterfaceRspRCP rspRCP = 
        rcp_cast<WDSStopNetworkInterfaceRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSStopNetworkInterfaceReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page disable methods
    m_connectionPage.DisableConnectedState();
    m_statisticsPage.DisableConnectedState();
    
    // !!! call msgs to end receiving stats, have stats page handle?
}

// --------------------------------------------------------------------------
// ProcessWDSPktSrvcStatusInd
//
/// Actions to take when a WDSPktServiceStatusInd is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSPktSrvcStatusInd(MessageRCP msgRCP)
{
    WDSPktSrvcStatusIndRCP indRCP = rcp_cast<WDSPktSrvcStatusInd*>(msgRCP);

    // call Page process methods
    m_connectionPage.ProcessWDSPktSrvcStatusInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetCurrentChannelRateRsp
//
/// Actions to take when a WDSGetCurrentChannelRateRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSGetCurrentChannelRateRsp(MessageRCP msgRCP)
{
    WDSGetCurrentChannelRateRspRCP rspRCP = 
        rcp_cast<WDSGetCurrentChannelRateRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSGetCurrentChannelRateReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statisticsPage.ProcessWDSGetCurrentChannelRateRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSCreateProfileRsp
//
/// Actions to take when a WDSCreateProfileRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSCreateProfileRsp(MessageRCP msgRCP)
{
    WDSCreateProfileRspRCP rspRCP = rcp_cast<WDSCreateProfileRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSCreateProfileReq",rspRCP->GetError(),MSG_BOX);
    }

    // call Page process methods
    m_profilesPage.ProcessWDSCreateProfileRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSModifyProfileSettingsRsp
//
/// Actions to take when a WDSModifyProfileSettingsRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSModifyProfileSettingsRsp(MessageRCP msgRCP)
{
    WDSModifyProfileSettingsRspRCP rspRCP = 
        rcp_cast<WDSModifyProfileSettingsRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSModifyProfileSettingsReq",rspRCP->GetError(),MSG_BOX);
    }

    // call Page process methods
    m_profilesPage.ProcessWDSModifyProfileSettingsRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSDeleteProfileRsp
//
/// Actions to take when a WDSDeleteProfileRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSDeleteProfileRsp(MessageRCP msgRCP)
{
    WDSDeleteProfileRspRCP rspRCP = rcp_cast<WDSDeleteProfileRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSDeleteProfileReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page process methods
    m_profilesPage.ProcessWDSDeleteProfileRsp();
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileListRsp
//
/// Actions to take when a WDSGetProfileListRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSGetProfileListRsp(MessageRCP msgRCP)
{
    WDSGetProfileListRspRCP rspRCP = rcp_cast<WDSGetProfileListRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSGetProfileListReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_connectionPage.ProcessWDSGetProfileListRsp(rspRCP);
    m_profilesPage.ProcessWDSGetProfileListRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileSettingsRsp
//
/// Actions to take when a WDSGetProfileSettingsRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSGetProfileSettingsRsp(MessageRCP msgRCP)
{
    WDSGetProfileSettingsRspRCP rspRCP = 
        rcp_cast<WDSGetProfileSettingsRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSGetProfileSettingsReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page process methods
    m_profilesPage.ProcessWDSGetProfileSettingsRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetDefaultSettingsRsp
//
/// Actions to take when a WDSGetDefaultSettingsRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessWDSGetDefaultSettingsRsp(MessageRCP msgRCP)
{
    WDSGetDefaultSettingsRspRCP rspRCP = 
        rcp_cast<WDSGetDefaultSettingsRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSGetDefaultSettingsReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page process methods
    m_profilesPage.ProcessWDSGetDefaultSettingsRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessStatusNotice
//
/// Actions to take when a StatusNotice is received from the MessageManager.
///
/// @params noticeRCP - Notice refetence counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessStatusNotice(NoticeRCP noticeRCP)
{
    StatusNoticeRCP statusNoticeRCP = rcp_cast<StatusNotice*>(noticeRCP);
    uint8 severity = statusNoticeRCP->GetSeverity();
USES_CONVERSION;
    if (severity == ST_ERROR)
    {
        AfxMessageBox(A2W(statusNoticeRCP->GetEvent().c_str()));
    }

    // pass to status page for processing
    m_statusPage.AddEventMessage
                 (
                     statusNoticeRCP->GetEvent(),
                     statusNoticeRCP->GetSeverity()
                 );
}

// --------------------------------------------------------------------------
// ProcessDeviceNotice
//
/// Actions to take when a DeviceNotice is received from the MessageManager.
///
/// @params noticeRCP - Notice refetence counted pointer to process.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ProcessDeviceNotice(NoticeRCP noticeRCP)
{
    DeviceNoticeRCP deviceNoticeRCP = rcp_cast<DeviceNotice*>(noticeRCP);
    uint8 type = deviceNoticeRCP->GetType();
    std::stringstream stream;
    std::vector<std::string> networkAdapters = deviceNoticeRCP->GetNetworkAdapters();
    int size = (int)networkAdapters.size();

	// @@
	int i = 0;
	USES_CONVERSION;

    switch (type)
    {
        case DT_AVAILABLE:
            stream.str("");
            stream << _T("Attached Network Adapters: ") << std::endl;

            if (size == 0)
            {
                stream << _T("None Detected") << std::endl;
            }

            for (/* @@ int */ i = 0; i < size; i++)
            {
                stream << networkAdapters[i] << std::endl;
            }

            stream << std::endl;
            m_statusPage.AddEventMessage(stream.str(),ST_INFORMATION);

            // update the available devices
            m_connectionPage.PopulateDeviceNames(networkAdapters);
            break;

        case DT_CONNECT:
            // add event to status page
            m_statusPage.AddEventMessage(deviceNoticeRCP->GetReason(),ST_INFORMATION);
            
            // direct pages to enable appropriate fields
            m_connectionPage.EnableSelectedState();
            m_profilesPage.EnableSelectedState();
            m_statisticsPage.EnableSelectedState();
            m_statusPage.EnableSelectedState();

            EndWaitCursor();
            break;

        case DT_CONNECT_FAIL:
            m_statusPage.AddEventMessage(deviceNoticeRCP->GetReason(),ST_ERROR);

            // direct pages to disable appropriate fields
            m_connectionPage.DisableSelectedState();
            m_profilesPage.DisableSelectedState();
            m_statisticsPage.DisableSelectedState();
            m_statusPage.DisableSelectedState();

            EndWaitCursor();

            // do all work before displaying message box
           
            AfxMessageBox(A2W(deviceNoticeRCP->GetReason().c_str()));

            break;

        case DT_DISCONNECT:
            m_statusPage.AddEventMessage(deviceNoticeRCP->GetReason(),ST_INFORMATION);

            // direct pages to disable appropriate fields
            m_connectionPage.DisableSelectedState();
            m_profilesPage.DisableSelectedState();
            m_statisticsPage.DisableSelectedState();
            m_statusPage.DisableSelectedState();

            EndWaitCursor();
            break;

        case DT_DETACH:
            m_statusPage.AddEventMessage(deviceNoticeRCP->GetReason(),ST_WARNING);

            // update the available devices
            m_connectionPage.PopulateDeviceNames(networkAdapters);
            
            // direct pages to disable appropriate fields
            m_connectionPage.DisableSelectedState();
            m_profilesPage.DisableSelectedState();
            m_statisticsPage.DisableSelectedState();
            m_statusPage.DisableSelectedState();

            // do all work before displaying message box

            AfxMessageBox(A2W(deviceNoticeRCP->GetReason().c_str()));

            break;

        default:
            AfxMessageBox(_T("Error: Unknown device notice received."));
    }
}

// --------------------------------------------------------------------------
// LogMessage
//
/// Write a message to the status page.
///
/// @params msgRCP - Message reference counted pointer.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::LogMessage(MessageRCP msgRCP)
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    std::stringstream stream;
    stream << _T("Time: ") << (uint32)time.wHour << _T(":")
           << (uint32)time.wMinute << _T(":")
           << (uint32)time.wSecond << _T(".")
           << (uint32)time.wMilliseconds << std::endl
           << _T("Transaction Id: ") << (uint32)msgRCP->GetTxId() << std::endl
           << std::endl;
    msgRCP->Print(stream);
    msgRCP->PrintMsgBuf(stream);
    stream << std::endl;
    m_statusPage.AddEventMessage(stream.str(),ST_INFORMATION);
}

// --------------------------------------------------------------------------
// ReportMsgFailed
//
/// Report a message failure to the status page, popup dialog when requested.
///
/// @params msgRCP - Message reference counted pointer.
// --------------------------------------------------------------------------
void ConnectionManagerDlg::ReportMsgFailed(std::string name,uint16 error,bool isMsgBox)
{
    std::stringstream stream;
	USES_CONVERSION;
    stream << _T("Error: ") << name << _T(" failed with error '")
           << ERROR_STRINGS[error] << _T("'.")<< std::endl;
    
    if (isMsgBox) 
	{
		/*modify by GL on 2008-03-24 begin*/

	CStdioFile file;
	if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
	{
		
		DWORD dwActual = file.SeekToEnd();
		file.WriteString(A2W(stream.str().c_str()));
		file.Close();		
	}

		
		//AfxMessageBox(stream.str().c_str()); 
		/*modify by GL on 2008-03-24 begin*/
	}
    
    stream << std::endl << std::endl;
    m_statusPage.AddEventMessage(stream.str(),ST_ERROR);
}
