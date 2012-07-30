// --------------------------------------------------------------------------
//
// ConnectionManagerDlg.h
//
/// ConnectionManagerDlg interface.
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

#pragma once

#include "ConnectionPage.h"
#include "ProfilesPage.h"
#include "StatisticsPage.h"
#include "StatusPage.h"
#include "TSQueue.h"
#include "Notice.h"
#include "Resource.h"



// ConnectionManagerDlg dialog
class ConnectionManagerDlg : public CDialog, public NoticeSubscriber
{
// Construction
public:
	ConnectionManagerDlg(CWnd* pParent = NULL);
    ~ConnectionManagerDlg();

    void SelectDevice(std::string& deviceName);
    void DeselectDevice();
    void SendRequest(std::string& msgStr);
    void OnPublisherNotify
    (
        const Publisher<NoticeRCP>& publisher,
        const NoticeRCP& noticeRCP
    );

// Dialog Data
	enum { IDD = IDD_CONNECTIONMANAGER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
    afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnNewNotice(WPARAM wParam,LPARAM lParam);
    bool OpenUserGuide(const char* relativePath);
    void ProcessMsgNotice(NoticeRCP noticeRCP);
    void ProcessReqMsgNotice(MessageRCP msgRCP,uint32 msgUID);
    void ProcessRspMsgNotice(MessageRCP msgRCP,uint32 msgUID);
    void ProcessIndMsgNotice(MessageRCP msgRCP,uint32 msgUID);
    void ProcessWDSStartReq(MessageRCP msgRCP);
    void ProcessWDSDeleteProfileReq(MessageRCP msgRCP);
    void ProcessWDSGetProfileSettingsReq(MessageRCP msgRCP);
    void ProcessWDSGetDefaultSettingsReq(MessageRCP msgRCP);
    void ProcessWDSSetEventReportRsp(MessageRCP msgRCP);
    void ProcessDMSSetEventReportRsp(MessageRCP msgRCP);
    void ProcessNASSetEventReportRsp(MessageRCP msgRCP);
    void ProcessWDSEventReportInd(MessageRCP msgRCP);
    void ProcessDMSEventReportInd(MessageRCP msgRCP);
    void ProcessNASEventReportInd(MessageRCP msgRCP);
    void ProcessDMSGetPowerStateRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceMfrRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceModelIdRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceRevIdRsp(MessageRCP msgRCP);
    void ProcessDMSGetMsisdnRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceSerialNumbersRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceCapRsp(MessageRCP msgRCP);
    void ProcessNASGetSignalStrengthRsp(MessageRCP msgRCP);
    void ProcessNASGetServingSystemRsp(MessageRCP msgRCP);
    void ProcessNASServingSystemInd(MessageRCP msgRCP);
    void ProcessWDSAbortRsp(MessageRCP msgRCP);
    void ProcessWDSStartRsp(MessageRCP msgRCP);
    void ProcessWDSStopRsp(MessageRCP msgRCP);
    void ProcessWDSPktSrvcStatusInd(MessageRCP msgRCP);
    void ProcessWDSGetCurrentChannelRateRsp(MessageRCP msgRCP);
    void ProcessWDSCreateProfileRsp(MessageRCP msgRCP);
    void ProcessWDSDeleteProfileRsp(MessageRCP msgRCP);
    void ProcessWDSModifyProfileSettingsRsp(MessageRCP msgRCP);
    void ProcessWDSGetProfileListRsp(MessageRCP msgRCP);
    void ProcessWDSGetProfileSettingsRsp(MessageRCP msgRCP);
    void ProcessWDSGetDefaultSettingsRsp(MessageRCP msgRCP);
    void ProcessStatusNotice(NoticeRCP noticeRCP);
    void ProcessDeviceNotice(NoticeRCP noticeRCP);
    void LogMessage(MessageRCP msgRCP);
    void ReportMsgFailed(std::string name,uint16 error,bool isMsgBox);
	DECLARE_MESSAGE_MAP()

private:
    CPropertySheet m_propSheet;
    ConnectionPage m_connectionPage;
    ProfilesPage m_profilesPage;
// !!! SimPage m_simPage;
    StatisticsPage m_statisticsPage;
    StatusPage m_statusPage;
    TSQueue<NoticeRCP> m_noticeRCPs;
};
