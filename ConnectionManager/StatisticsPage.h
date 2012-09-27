// --------------------------------------------------------------------------
//
// StatisticsPage.h
//
/// StatisticsPage interface.
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
#include "DMSGetDeviceCapMsg.h"
#include "WDSSetEventReportMsg.h"
#include "WDSGetCurrentChannelRateMsg.h"

// @@@@@@
#ifndef OPEN_PAGE_UI
class CHSDPADlg;
#else
class ConnectionManagerDlg;
#endif


// --------------------------------------------------------------------------
// StatisticsPage
//
/// The StatisticsPage class represents the controls under the statistics tab
/// of the ConnectionManager dialog. Input from the user and output to the
/// user under the status tab is controlled by this class.
// --------------------------------------------------------------------------

class StatisticsPage : public CPropertyPage
{
public:
    StatisticsPage();
// @@@@@@
#ifndef OPEN_PAGE_UI
    void SetHSDPADlg(CHSDPADlg* pDlg);
#else
    void SetCMDlg(ConnectionManagerDlg* pCMDlg);
#endif
    void EnableSelectedState();
    void DisableSelectedState();
    void EnableConnectedState();
    void DisableConnectedState();
    void ProcessWDSEventReportInd(WDSEventReportIndRCP indRCP);
    void ProcessWDSGetCurrentChannelRateRsp(WDSGetCurrentChannelRateRspRCP rspRCP);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    std::string BuildWDSGetCurrentChannelRateString();
    std::string BuildWDSEventReportString(/*!!!start/stop?*/);
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
    CStatic m_txGroup;
    CStatic m_rxGroup;
    CStatic m_maxTxRateStatic;
    CStatic m_maxRxRateStatic;
    CStatic m_txRateStatic;
    CStatic m_rxRateStatic;
    CStatic m_txPacketsOkStatic;
    CStatic m_rxPacketsOkStatic;
    CStatic m_txPacketErrsStatic;
    CStatic m_rxPacketErrsStatic;
    CStatic m_txOverflowsStatic;
    CStatic m_rxOverflowsStatic;
    CStatic m_dataBearerTechStatic;
#endif

    bool m_isConnected;
};
