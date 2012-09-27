// --------------------------------------------------------------------------
//
// ConnectionPage.h
//
/// ConnectionPage interface.
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

#include <string>
#include <vector>
#include "CommonDefs.h"
#include "TextProgressCtrl.h"
#include "DMSGetPowerStateMsg.h"
#include "DMSSetEventReportMsg.h"
#include "NASGetSignalStrengthMsg.h"
#include "NASSetEventReportMsg.h"
#include "NASGetServingSystemMsg.h"
#include "WDSStartNetworkInterfaceMsg.h"
#include "WDSGetPktSrvcStatusMsg.h"
#include "WDSGetProfileListMsg.h"
#include "dlgconnect.h"


// @@@@@@
#ifndef OPEN_PAGE_UI
class CHSDPADlg;
#else
class ConnectionManagerDlg;
#endif

// --------------------------------------------------------------------------
// ConnectionPage
//
/// The ConnectionPage class represents the controls under the connection
/// tab of the ConnectionManager dialog. Input from the user and output to
/// the user under the connection tab is controlled by this class.
// --------------------------------------------------------------------------

class ConnectionPage : public CPropertyPage
{
public:
    ConnectionPage();

// @@@@@@
#ifndef OPEN_PAGE_UI
    void SetHSDPADlg(CHSDPADlg* pDlg);
    void OnConnectEx(NdisStateRet ndisStatus, StConnProfile *profile);
    void OnDeviceSelect(std::string& deviceName);

    StConnProfile m_NdisProfile;
#else
    void SetCMDlg(ConnectionManagerDlg* pCMDlg);
#endif

    void SetTxId(uint16 txId) {
        m_txId = txId;
    }
    void EnableSelectedState();
    void DisableSelectedState();
    void EnableConnectedState(uint32 pktDataHandle);
    void DisableConnectedState();
    void PopulateDeviceNames(std::vector<std::string>& networkAdapters);
    void ProcessDMSGetPowerStateRsp(DMSGetPowerStateRspRCP rspRCP);
    void ProcessDMSEventReportInd(DMSEventReportIndRCP indRCP);
    void ProcessNASGetSignalStrengthRsp(NASGetSignalStrengthRspRCP rspRCP);
    void ProcessNASEventReportInd(NASEventReportIndRCP indRCP);
    void ProcessNASGetServingSystemRsp(NASGetServingSystemRspRCP rspRCP);
    void ProcessNASServingSystemInd(NASServingSystemIndRCP indRCP);
    void ProcessWDSPktSrvcStatusInd(WDSPktSrvcStatusIndRCP indRCP);
    void ProcessWDSGetProfileListRsp(WDSGetProfileListRspRCP rspRCP);

// @@@@@@
#ifndef OPEN_PAGE_UI
public:
#else
protected:
#endif
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnDeviceSelect();
    afx_msg void OnTechPrefCheck();
    afx_msg void OnProfileIdCheck();
    afx_msg void OnPrimaryDnsCheck();
    afx_msg void OnSecondaryDnsCheck();
    afx_msg void OnPrimaryNbnsCheck();
    afx_msg void OnSecondaryNbnsCheck();
    afx_msg void OnApnNameCheck();
    afx_msg void OnIpAddressCheck();
    afx_msg void OnAuthPrefCheck();
    afx_msg void OnUsernameCheck();
    afx_msg void OnPasswordCheck();
    afx_msg void OnConnect();
    void EnableAbortState();
    void BuildDeviceNames(); // !!! no longer needed

    void StoreDeviceNames(CString& deviceName); // !!! no longer needed

    std::string BuildDMSPowerStatusString();
    std::string BuildDMSEventReportString();
    std::string BuildNASSignalStrengthString();
    std::string BuildNASEventReportString();
    std::string BuildNASServingSystemString();
    std::string BuildWDSStartString();
    std::string BuildWDSAbortString();
    std::string BuildWDSStopString();
    void ProcessPowerState(uint8 powerStatus,uint8 batteryLvl);
    void ProcessSignalStrength(int8 signalStrength,uint8 radioIf);
    void ProcessAttachState(uint8 csAttachState,uint8 psAttachState);

    DECLARE_MESSAGE_MAP()

private:
// @@@@@@
#ifndef OPEN_PAGE_UI
    CHSDPADlg* m_pCMDlg;
#else
    ConnectionManagerDlg* m_pCMDlg;
#endif

    bool m_useWaitCursor;

    CStatic m_selectDeviceStatic;
    CComboBox m_deviceNameCombo;
    CStatic m_overrideGoup;
    CButton m_techPrefCheck;
    CComboBox m_techPrefCombo;
    CButton m_3gppProfileIdCheck;
    CComboBox m_3gppProfileIdCombo;
    CButton m_primaryDnsCheck;
    CIPAddressCtrl m_primaryDnsIpAddr;
    CButton m_secondaryDnsCheck;
    CIPAddressCtrl m_secondaryDnsIpAddr;
    CButton m_primaryNbnsCheck;
    CIPAddressCtrl m_primaryNbnsIpAddr;
    CButton m_secondaryNbnsCheck;
    CIPAddressCtrl m_secondaryNbnsIpAddr;
    CButton m_apnNameCheck;
    CEdit m_apnNameEdit;
    CButton m_ipAddressCheck;
    CIPAddressCtrl m_ipAddressIpAddr;
    CButton m_authPrefCheck;
    CComboBox m_authPrefCombo;
    CButton m_usernameCheck;
    CEdit m_usernameEdit;
    CButton m_passwordCheck;
    CEdit m_passwordEdit;
    CButton m_connectButton;
    CStatic m_batteryStatic;
    TextProgressCtrl m_batteryProgress;
    CStatic m_extPowerStatic;
    CStatic m_csStatic;
    CStatic m_psStatic;
    CStatic m_signalStatic;
    TextProgressCtrl m_signalProgress;

    std::string m_deviceName;
    bool m_isSelected;
    bool m_isConnected;
    uint32 m_pktDataHandle;
    uint16 m_txId;
};