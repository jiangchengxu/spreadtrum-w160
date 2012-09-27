// --------------------------------------------------------------------------
//
// StatusPage.h
//
/// StatusPage interface.
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
#include "CommonDefs.h"
#include "DMSGetDeviceMfrMsg.h"
#include "DMSGetDeviceModelIdMsg.h"
#include "DMSGetDeviceRevIdMsg.h"
#include "DMSGetMsisdnMsg.h"
#include "DMSGetDeviceSerialNumbersMsg.h"
#include "DMSGetDeviceCapMsg.h"


// @@@@@@
#ifndef OPEN_PAGE_UI
class CHSDPADlg;
#else
class ConnectionManagerDlg;
#endif


// --------------------------------------------------------------------------
// StatusPage
//
/// The StatusPage class represents the controls under the status tab of the
/// ConnectionManager dialog. Input from the user and output to the user
/// under the status tab is controlled by this class.
// --------------------------------------------------------------------------

class StatusPage : public CPropertyPage
{
public:
    StatusPage();
// @@@@@@
#ifndef OPEN_PAGE_UI
    void SetHSDPADlg(CHSDPADlg* pDlg);
#else
    void SetCMDlg(ConnectionManagerDlg* pCMDlg);
#endif
    void EnableSelectedState();
    void DisableSelectedState();
    void ProcessDMSGetDeviceMfrRsp(DMSGetDeviceMfrRspRCP rspRCP);
    void ProcessDMSGetDeviceModelIdRsp(DMSGetDeviceModelIdRspRCP rspRCP);
    void ProcessDMSGetDeviceRevIdRsp(DMSGetDeviceRevIdRspRCP rspRCP);
    void ProcessDMSGetMsisdnRsp(DMSGetMsisdnRspRCP rspRCP);
    void ProcessDMSGetDeviceSerialNumbersRsp(DMSGetDeviceSerialNumbersRspRCP rspRCP);
    void ProcessDMSGetDeviceCapRsp(DMSGetDeviceCapRspRCP rspRCP);
    void AddEventMessage(std::string& event,uint8 severity);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    std::string BuildDMSGetDeviceMfrString();
    std::string BuildDMSGetDeviceModelIdString();
    std::string BuildDMSGetDeviceRevIdString();
    std::string BuildDMSGetMsisdnString();
    std::string BuildDMSGetDeviceSerialNumbersString();
    std::string BuildDMSGetDeviceCapString();
    DECLARE_MESSAGE_MAP()

private:
// @@@@@@
#ifndef OPEN_PAGE_UI
    CHSDPADlg* m_pCMDlg;
#else
    ConnectionManagerDlg* m_pCMDlg;
#endif

// @@@@@@
#ifdef OPEN_PAGE_UI
    CStatic m_deviceInformationGroup;
    CStatic m_mfrStatic;
    CStatic m_modelIdStatic;
    CStatic m_revIdStatic;
    CStatic m_serialNumberStatic;
    CStatic m_isdnStatic;
    CStatic m_simStatic;
    CStatic m_maxTxRateCapStatic;
    CStatic m_maxRxRateCapStatic;
    CStatic m_eventsStatic;
#endif

    // @@
#if 0
    CRichEditCtrl m_eventsRichEdit;
#endif
};
