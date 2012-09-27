// --------------------------------------------------------------------------
//
// StatisticsPage.cpp
//
/// StatisticsPage implementation.
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

#include "StatisticsPage.h"
#include "ConnectionManagerDlg.h"
#include "Resource.h"


#ifndef OPEN_PAGE_UI
#include "HSDPADlg.h"
#endif

BEGIN_MESSAGE_MAP(StatisticsPage, CPropertyPage)
END_MESSAGE_MAP()


// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
StatisticsPage::StatisticsPage() :
    CPropertyPage(IDD_STATISTICS_PAGE),
    m_pCMDlg(NULL),
    m_isConnected(false)
{}

// --------------------------------------------------------------------------
// SetCMDlg
// --------------------------------------------------------------------------

#ifndef OPEN_PAGE_UI
void StatisticsPage::SetHSDPADlg(CHSDPADlg* pDlg)
{
    m_pCMDlg = pDlg;
}
#else
void StatisticsPage::SetCMDlg(ConnectionManagerDlg* pCMDlg)
{
    m_pCMDlg = pCMDlg;
}
#endif

// --------------------------------------------------------------------------
// DoDataExchange
// --------------------------------------------------------------------------
void StatisticsPage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);


#ifdef OPEN_PAGE_UI
    DDX_Control(pDX, IDC_TX_GROUP, m_txGroup);
    DDX_Control(pDX, IDC_RX_GROUP, m_rxGroup);
    DDX_Control(pDX, IDC_MAX_TX_RATE_STATIC, m_maxTxRateStatic);
    DDX_Control(pDX, IDC_MAX_RX_RATE_STATIC, m_maxRxRateStatic);
    DDX_Control(pDX, IDC_TX_RATE_STATIC, m_txRateStatic);
    DDX_Control(pDX, IDC_RX_RATE_STATIC, m_rxRateStatic);
    DDX_Control(pDX, IDC_TX_PACKETS_OK_STATIC, m_txPacketsOkStatic);
    DDX_Control(pDX, IDC_RX_PACKETS_OK_STATIC, m_rxPacketsOkStatic);
    DDX_Control(pDX, IDC_TX_PACKET_ERRS_STATIC, m_txPacketErrsStatic);
    DDX_Control(pDX, IDC_RX_PACKET_ERRS_STATIC, m_rxPacketErrsStatic);
    DDX_Control(pDX, IDC_TX_OVERFLOWS_STATIC, m_txOverflowsStatic);
    DDX_Control(pDX, IDC_RX_OVERFLOWS_STATIC, m_rxOverflowsStatic);
    DDX_Control(pDX, IDC_DATA_BEARER_TECH_STATIC, m_dataBearerTechStatic);
#endif
}

// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL StatisticsPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// --------------------------------------------------------------------------
// EnableSelectedState
//
/// Show availabe fields and send requests to populate them.
// --------------------------------------------------------------------------
void StatisticsPage::EnableSelectedState()
{

#ifdef OPEN_PAGE_UI
    // enable fields
    m_txGroup.EnableWindow(TRUE);
    m_rxGroup.EnableWindow(TRUE);
    m_maxTxRateStatic.EnableWindow(TRUE);
    m_maxRxRateStatic.EnableWindow(TRUE);
#endif

    std::string msgStr;

    // send channel rate request
    msgStr = BuildWDSGetCurrentChannelRateString();
    m_pCMDlg->SendRequest(msgStr);
}

// --------------------------------------------------------------------------
// DisableSelectedState
//
/// Dim fields that should not be accessed.
// --------------------------------------------------------------------------
void StatisticsPage::DisableSelectedState()
{
    // disable started state if necessary
    if (m_isConnected) {
        DisableConnectedState();
    }


#ifdef OPEN_PAGE_UI
    // disable fields
    m_txGroup.EnableWindow(FALSE);
    m_rxGroup.EnableWindow(FALSE);
    m_maxTxRateStatic.EnableWindow(FALSE);
    m_maxTxRateStatic.SetWindowText(_T("Max TX Rate:"));
    m_maxRxRateStatic.EnableWindow(FALSE);
    m_maxRxRateStatic.SetWindowText(_T("Max RX Rate:"));
#endif
}

// --------------------------------------------------------------------------
// BuildWDSGetCurrentChannelRateString
//
/// Build a string of message type and name/value pairs representing a
/// WDSGetCurrentChannelRate message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string StatisticsPage::BuildWDSGetCurrentChannelRateString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_WDS_GET_CURRENT_CHANNEL_RATE_REQ" << std::endl
           << "{}";

    return stream.str();
}

// --------------------------------------------------------------------------
// ProcessWDSGetCurrentChannelRateRsp
//
/// Actions to take when a WDSGetCurrentChannelRateRsp message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void StatisticsPage::ProcessWDSGetCurrentChannelRateRsp(WDSGetCurrentChannelRateRspRCP rspRCP)
{
    std::stringstream stream;

    stream << _T("Max TX Rate: ") << (int)rspRCP->GetMaxChannelTxRate();


#ifdef OPEN_PAGE_UI
    m_maxTxRateStatic.SetWindowText(stream.str().c_str());
#else
    // speed
// 	CSetupDlg* pSetupWnd = (CSetupDlg*)(m_pCMDlg->m_pSetupDlg);

// 	pInternetWnd->m_strSpeed.Format(_T("%d"), ((int)rspRCP->GetMaxChannelTxRate()));
// 	pInternetWnd->UpdateData(FALSE);
#endif
    stream.str("");

    stream << _T("Max RX Rate: ") << (int)rspRCP->GetMaxChannelRxRate();

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_maxRxRateStatic.SetWindowText(stream.str().c_str());
#endif
}

// --------------------------------------------------------------------------
// EnableConnectedState
//
/// Show availabe fields.
// --------------------------------------------------------------------------
void StatisticsPage::EnableConnectedState()
{
    // set interface started flag
    m_isConnected = true;


#ifdef OPEN_PAGE_UI
    // enable fields
    m_txRateStatic.EnableWindow(TRUE);
    m_rxRateStatic.EnableWindow(TRUE);
    m_txPacketsOkStatic.EnableWindow(TRUE);
    m_rxPacketsOkStatic.EnableWindow(TRUE);
    m_txPacketErrsStatic.EnableWindow(TRUE);
    m_rxPacketErrsStatic.EnableWindow(TRUE);
    m_txOverflowsStatic.EnableWindow(TRUE);
    m_rxOverflowsStatic.EnableWindow(TRUE);
    m_dataBearerTechStatic.EnableWindow(TRUE);
#endif

    std::string msgStr;

    // send event report request
    msgStr = BuildWDSEventReportString();
    m_pCMDlg->SendRequest(msgStr);



}

// --------------------------------------------------------------------------
// DisableConnectedState
//
/// Dim fields that should not be accessed.
// --------------------------------------------------------------------------
void StatisticsPage::DisableConnectedState()
{
    // set interface started flag
    m_isConnected = false;


#ifdef OPEN_PAGE_UI
    // disable fields
    m_txRateStatic.EnableWindow(FALSE);
    m_txRateStatic.SetWindowText(_T("TX Rate:"));
    m_rxRateStatic.EnableWindow(FALSE);
    m_rxRateStatic.SetWindowText(_T("RX Rate:"));
    m_txPacketsOkStatic.EnableWindow(FALSE);
    m_txPacketsOkStatic.SetWindowText(_T("TX Packets OK:"));
    m_rxPacketsOkStatic.EnableWindow(FALSE);
    m_rxPacketsOkStatic.SetWindowText(_T("RX Packets OK:"));
    m_txPacketErrsStatic.EnableWindow(FALSE);
    m_txPacketErrsStatic.SetWindowText(_T("TX Packet Errs:"));
    m_rxPacketErrsStatic.EnableWindow(FALSE);
    m_rxPacketErrsStatic.SetWindowText(_T("RX Packet Errs:"));
    m_txOverflowsStatic.EnableWindow(FALSE);
    m_txOverflowsStatic.SetWindowText(_T("TX Overflows:"));
    m_rxOverflowsStatic.EnableWindow(FALSE);
    m_rxOverflowsStatic.SetWindowText(_T("RX Overflows:"));
    m_dataBearerTechStatic.EnableWindow(FALSE);
    m_dataBearerTechStatic.SetWindowText(_T("Data Bearer Technology:"));
#endif
}

// --------------------------------------------------------------------------
// BuildWDSEventReportString
//
/// Build a string of message type and name/value pairs representing a
/// WDSSetEventReportReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string StatisticsPage::BuildWDSEventReportString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_WDS_SET_EVENT_REPORT_REQ" << std::endl
           << "{" << std::endl;

    // current channel rate indicator
    stream << "  ReportChannelRate " << 1 << std::endl;

    // transfer statistics indicator
    stream << "  StatsPeriod " << 1 << std::endl
           << "  StatsMask " << 0x3F << std::endl;

    // current data bearer technology indicator
    stream << "  ReportDataBearerTech " << 1 << std::endl;

    stream << "}";

    return stream.str();
}

// --------------------------------------------------------------------------
// ProcessWDSEventReportInd
//
/// Actions to take when a WDSEventReportIndRCP message is received.
///
/// @params indRCP - the reference counted pointer indicator to process.
// --------------------------------------------------------------------------
void StatisticsPage::ProcessWDSEventReportInd(WDSEventReportIndRCP indRCP)
{
    std::stringstream stream;

    // populate fields
    if (indRCP->IsCurrentChannelRate()) {
        stream << _T("TX Rate: ") << (uint32)indRCP->GetCurrentChannelTxRate();

#ifdef OPEN_PAGE_UI
        m_txRateStatic.SetWindowText(stream.str().c_str());
#endif
        stream.str("");
        stream << _T("RX Rate: ") << (uint32)indRCP->GetCurrentChannelRxRate();

#ifdef OPEN_PAGE_UI
        m_rxRateStatic.SetWindowText(stream.str().c_str());
#endif
    }

    if (indRCP->IsTxOkCount()) {
        stream.str("");
        stream << _T("TX Packets OK: ") << (uint32)indRCP->GetTxOkCount();

#ifdef OPEN_PAGE_UI
        m_txPacketsOkStatic.SetWindowText(stream.str().c_str());
#else
        TRACE(_T("TX Packets OK: %u.\n"), (uint32)indRCP->GetTxOkCount());

        // TX
// 		CSetupDlg* pSetupWnd = (CSetupDlg*)(m_pCMDlg->m_pSetupDlg);

// 		pInternetWnd->onTxOkCount((uint32)indRCP->GetTxOkCount());
        m_pCMDlg->m_pConnectDlg->onTxOkCount((uint32)indRCP->GetTxOkCount());
#endif
    }

    if (indRCP->IsRxOkCount()) {
        stream.str("");
        stream << _T("RX Packets OK: ") << (uint32)indRCP->GetRxOkCount();

#ifdef OPEN_PAGE_UI
        m_rxPacketsOkStatic.SetWindowText(stream.str().c_str());
#else
        TRACE(_T("RX Packets OK: %u\n"), (uint32)indRCP->GetRxOkCount());


// 		CSetupDlg* pSetupWnd = (CSetupDlg*)(m_pCMDlg->m_pSetupDlg);

// 		pInternetWnd->onRxOkCount((uint32)indRCP->GetRxOkCount());
        m_pCMDlg->m_pConnectDlg->onRxOkCount((uint32)indRCP->GetRxOkCount());
#endif
    }

    if (indRCP->IsTxErrCount()) {
        stream.str("");
        stream << _T("TX Packet Errs: ") << (uint32)indRCP->GetTxErrCount();

#ifdef OPEN_PAGE_UI
        m_txPacketErrsStatic.SetWindowText(stream.str().c_str());
#endif
    }

    if (indRCP->IsRxErrCount()) {
        stream.str("");
        stream << _T("RX Packet Errs: ") << (uint32)indRCP->GetRxErrCount();

#ifdef OPEN_PAGE_UI
        m_rxPacketErrsStatic.SetWindowText(stream.str().c_str());
#endif
    }

    if (indRCP->IsTxOflCount()) {
        stream.str("");
        stream << _T("TX Overflows: ") << (uint32)indRCP->GetTxOflCount();

#ifdef OPEN_PAGE_UI
        m_txOverflowsStatic.SetWindowText(stream.str().c_str());
#endif
    }


    if (indRCP->IsRxOflCount()) {
        stream.str("");
        stream << _T("RX Overflows: ") << (uint32)indRCP->GetRxOflCount();

#ifdef OPEN_PAGE_UI
        m_rxOverflowsStatic.SetWindowText(stream.str().c_str());
#endif
    }

    if (indRCP->IsDataBearerTech()) {
        stream.str("");
        stream << _T("Data Bearer Technology: ") << DATA_BEARER_TECH_STRINGS[indRCP->GetDataBearerTech()];

#ifdef OPEN_PAGE_UI
        m_dataBearerTechStatic.SetWindowText(stream.str().c_str());
#endif
    }
}
