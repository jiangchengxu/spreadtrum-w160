// --------------------------------------------------------------------------
//
// StatusPage.cpp
//
/// StatusPage implementation.
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

#include "StatusPage.h"
#include "ConnectionManagerDlg.h"
#include "Resource.h"

// @@@@@@
#ifndef OPEN_PAGE_UI
#include "HSDPADlg.h"
#endif

BEGIN_MESSAGE_MAP(StatusPage, CPropertyPage)
END_MESSAGE_MAP()

/*
standard red,orange,black
static const COLORREF textColor[] = { RGB(0xFF,0x00,0x00),
                                      RGB(0xFF,0xA5,0x00),
                                      RGB(0x00,0x00,0x00) };
*/

// firebrick, dark orange, black
static const COLORREF textColor[] = { RGB(0xB2,0x22,0x22),
                                      RGB(0xFF,0x7F,0x50),
                                      RGB(0x00,0x00,0x00) };

// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
StatusPage::StatusPage() : 
    CPropertyPage(IDD_STATUS_PAGE)
{}

// --------------------------------------------------------------------------
// SetCMDlg
// --------------------------------------------------------------------------
// @@@@@@
#ifndef OPEN_PAGE_UI
void StatusPage::SetHSDPADlg(CHSDPADlg* pDlg)
{	
    m_pCMDlg = pDlg;
}
#else
void StatusPage::SetCMDlg(ConnectionManagerDlg* pCMDlg)
{
    m_pCMDlg = pCMDlg;
}
#endif

// --------------------------------------------------------------------------
// DoDataExchange
// --------------------------------------------------------------------------
void StatusPage::DoDataExchange(CDataExchange* pDX)
{
	// @@
	CPropertyPage::DoDataExchange(pDX);

// @@@@@@
#ifdef OPEN_PAGE_UI
    DDX_Control(pDX, IDC_DEVICE_INFORMATION_GROUP, m_deviceInformationGroup);
    DDX_Control(pDX, IDC_MFR_STATIC, m_mfrStatic);
    DDX_Control(pDX, IDC_MODEL_ID_STATIC, m_modelIdStatic);
    DDX_Control(pDX, IDC_REV_ID_STATIC, m_revIdStatic);
    DDX_Control(pDX, IDC_SERIAL_NUMBER_STATIC, m_serialNumberStatic);
    DDX_Control(pDX, IDC_ISDN_STATIC, m_isdnStatic);
    DDX_Control(pDX, IDC_SIM_STATIC, m_simStatic);
    DDX_Control(pDX, IDC_MAX_TX_RATE_CAP_STATIC, m_maxTxRateCapStatic);
    DDX_Control(pDX, IDC_MAX_RX_RATE_CAP_STATIC, m_maxRxRateCapStatic);
    DDX_Control(pDX, IDC_EVENTS_STATIC, m_eventsStatic);
#endif

	// @@
#if 0
    DDX_Control(pDX, IDC_EVENTS_RICHEDIT, m_eventsRichEdit);
#endif
}

// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL StatusPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// --------------------------------------------------------------------------
// EnableSelectedState
//
/// Show availabe fields and send requests to populate them.
// --------------------------------------------------------------------------
void StatusPage::EnableSelectedState()
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // enable fields
    m_deviceInformationGroup.EnableWindow(TRUE);
    m_mfrStatic.EnableWindow(TRUE);
    m_modelIdStatic.EnableWindow(TRUE);
    m_revIdStatic.EnableWindow(TRUE);
    m_serialNumberStatic.EnableWindow(TRUE);
    m_isdnStatic.EnableWindow(TRUE);
    m_simStatic.EnableWindow(TRUE);
    m_maxTxRateCapStatic.EnableWindow(TRUE);
    m_maxRxRateCapStatic.EnableWindow(TRUE);
#endif

    std::string msgStr;

    // send device manufacturer request
    msgStr = BuildDMSGetDeviceMfrString();
    m_pCMDlg->SendRequest(msgStr);

    // send device model id request
    msgStr = BuildDMSGetDeviceModelIdString();
    m_pCMDlg->SendRequest(msgStr);

    // send device revision id request
    msgStr = BuildDMSGetDeviceRevIdString();
    m_pCMDlg->SendRequest(msgStr);

    // send voice number request
    msgStr = BuildDMSGetMsisdnString();
    m_pCMDlg->SendRequest(msgStr);

    // send device serial number request
    msgStr = BuildDMSGetDeviceSerialNumbersString();
    m_pCMDlg->SendRequest(msgStr);

    // send device capabilities request
    msgStr = BuildDMSGetDeviceCapString();
    m_pCMDlg->SendRequest(msgStr);
}

// --------------------------------------------------------------------------
// DisableSelectedState
//
/// Dim fields that should not be accessed.
// --------------------------------------------------------------------------
void StatusPage::DisableSelectedState()
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // disable fields
    m_deviceInformationGroup.EnableWindow(FALSE);
    m_mfrStatic.EnableWindow(FALSE);
    m_mfrStatic.SetWindowText(_T("Manufacturer:"));
    m_modelIdStatic.EnableWindow(FALSE);
    m_modelIdStatic.SetWindowText(_T("Model ID:"));
    m_revIdStatic.EnableWindow(FALSE);
    m_revIdStatic.SetWindowText(_T("Revision ID:"));
    m_serialNumberStatic.EnableWindow(FALSE);
    m_serialNumberStatic.SetWindowText(_T("ESN/IMEI/MEID:"));
    m_isdnStatic.EnableWindow(FALSE);
    m_isdnStatic.SetWindowText(_T("ISDN:"));
    m_simStatic.EnableWindow(FALSE);
    m_simStatic.SetWindowText(_T("SIM:"));
    m_maxTxRateCapStatic.EnableWindow(FALSE);
    m_maxTxRateCapStatic.SetWindowText(_T("Max TX Rate Cap:"));
    m_maxRxRateCapStatic.EnableWindow(FALSE);
    m_maxRxRateCapStatic.SetWindowText(_T("Max RX Rate Cap:"));
	// @@
#endif
}

// --------------------------------------------------------------------------
// BuildDMSGetDeviceMfrString
//
/// Build a string of message type and name/value pairs representing a
/// DMSGetDeviceMfrReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string StatusPage::BuildDMSGetDeviceMfrString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_DMS_GET_DEVICE_MFR_REQ" << std::endl 
           << "{}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildDMSGetDeviceModelIdString
//
/// Build a string of message type and name/value pairs representing a
/// DMSGetDeviceModelIdReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string StatusPage::BuildDMSGetDeviceModelIdString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_DMS_GET_DEVICE_MODEL_ID_REQ" << std::endl 
           << "{}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildDMSGetDeviceRevIdString
//
/// Build a string of message type and name/value pairs representing a
/// DMSGetDeviceRevIdReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string StatusPage::BuildDMSGetDeviceRevIdString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_DMS_GET_DEVICE_REV_ID_REQ" << std::endl 
           << "{}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildDMSGetMsisdnString
//
/// Build a string of message type and name/value pairs representing a
/// DMSGetMsisdnRsp message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string StatusPage::BuildDMSGetMsisdnString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_DMS_GET_MSISDN_REQ" << std::endl 
           << "{}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildDMSGetDeviceSerialNumbersString
//
/// Build a string of message type and name/value pairs representing a
/// DMSGetDeviceSerialNumbersRsp message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string StatusPage::BuildDMSGetDeviceSerialNumbersString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_REQ" << std::endl 
           << "{}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildDMSGetDeviceCapString
//
/// Build a string of message type and name/value pairs representing a
/// DMSGetDeviceCapReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string StatusPage::BuildDMSGetDeviceCapString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_DMS_GET_DEVICE_CAP_REQ" << std::endl 
           << "{}";

    return stream.str();
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceMfrRsp
//
/// Actions to take when a DMSGetDeviceMfrRspRCP message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void StatusPage::ProcessDMSGetDeviceMfrRsp(DMSGetDeviceMfrRspRCP rspRCP)
{
    std::stringstream stream;
    stream << _T("Manufacturer: ") << rspRCP->GetDeviceManufacturer();

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_mfrStatic.SetWindowText(stream.str().c_str());
#endif
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceModelIdRsp
//
/// Actions to take when a DMSGetDeviceModelIdRspRCP message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void StatusPage::ProcessDMSGetDeviceModelIdRsp(DMSGetDeviceModelIdRspRCP rspRCP)
{
    std::stringstream stream;
    stream << _T("Model ID: ") << rspRCP->GetDeviceModelId();

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_modelIdStatic.SetWindowText(stream.str().c_str());
#endif
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceRevIdRsp
//
/// Actions to take when a DMSGetDeviceRevIdRspRCP message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void StatusPage::ProcessDMSGetDeviceRevIdRsp(DMSGetDeviceRevIdRspRCP rspRCP)
{
    std::stringstream stream;
    stream << _T("Revision ID: ") << rspRCP->GetDeviceRevId();

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_revIdStatic.SetWindowText(stream.str().c_str());
#endif
}

// --------------------------------------------------------------------------
// ProcessDMSGetMsisdnRsp
//
/// Actions to take when a DMSGetMsisdnRspRCP message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void StatusPage::ProcessDMSGetMsisdnRsp(DMSGetMsisdnRspRCP rspRCP)
{
    std::stringstream stream;
    stream << _T("ISDN: ") << rspRCP->GetVoiceNumber();

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_isdnStatic.SetWindowText(stream.str().c_str());
#endif
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceSerialNumbersRsp
//
/// Actions to take when a DMSGetDeviceSerialNumbersRspRCP message is
/// received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void StatusPage::ProcessDMSGetDeviceSerialNumbersRsp(DMSGetDeviceSerialNumbersRspRCP rspRCP)
{
    std::stringstream stream;

    if (rspRCP->IsEsn())
    {
        // display esn
        stream << _T("ESN: ") << rspRCP->GetEsn();

    }

    if (rspRCP->IsImei())
    {
        // display imei
        stream << _T("IMEI: ") << rspRCP->GetImei();
    }

    if (rspRCP->IsMeid())
    {
        // display meid
        stream << _T("  MEID: ") << rspRCP->GetMeid();
    }

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_serialNumberStatic.SetWindowText(stream.str().c_str());
#endif
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceCapRsp
//
/// Actions to take when a DMSGetDeviceCapRspRCP message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void StatusPage::ProcessDMSGetDeviceCapRsp(DMSGetDeviceCapRspRCP rspRCP)
{
    std::stringstream stream;
    
    stream << _T("SIM: ");

    if (rspRCP->GetSimCapability() == 1)
    {
        stream << _T("Not Supported");
    }
    else
    {
        stream << _T("Supported");
    }

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_simStatic.SetWindowText(stream.str().c_str());
#endif

    stream.str("");
    stream << _T("Max TX Rate Cap: ") << (int)rspRCP->GetMaxTxChannelRate();

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_maxTxRateCapStatic.SetWindowText(stream.str().c_str());
#else
#endif

    stream.str("");
    stream << _T("Max RX Rate Cap: ") << (int)rspRCP->GetMaxRxChannelRate();

// @@@@@@
#ifdef OPEN_PAGE_UI
    m_maxRxRateCapStatic.SetWindowText(stream.str().c_str());
#endif
}

// --------------------------------------------------------------------------
// AddEventMessage
//
/// Add a message to the event text box.
///
/// @param event - the event to be displayed.
///
/// @param severity - the severity of the event.
// --------------------------------------------------------------------------
void StatusPage::AddEventMessage(std::string& event,uint8 severity)
{
    CHARFORMAT cf;

    // Initialize character format structure
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_COLOR;
    cf.dwEffects = 0; // To disable CFE_AUTOCOLOR
    cf.crTextColor = textColor[severity];

	// @@
#if 0
    m_eventsRichEdit.SetSel(m_eventsRichEdit.GetWindowTextLength(), -1);
    m_eventsRichEdit.SetSelectionCharFormat(cf);
    m_eventsRichEdit.ReplaceSel(event.c_str());
#endif
}
