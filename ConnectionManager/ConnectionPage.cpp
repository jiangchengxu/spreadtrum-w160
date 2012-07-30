// --------------------------------------------------------------------------
//
// ConnectionPage.cpp
//
/// ConnectionPage implementation.
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



#include "ConnectionPage.h"
#include "ConnectionManagerDlg.h"
#include "Resource.h"
#include "Message.h"
#include "MessageFactory.h"
#include "MessageManager.h"

// @@@@@@
#ifndef OPEN_PAGE_UI
#include "HSDPADlg.h"
#endif

static const CString DEVICE_SECTION = "DeviceSection";
static const CString DEVICE_ENTRY = "DeviceEntry";
static const uint8 PWR_EXT = 1;
static const uint8 PWR_BAT = 2;
static const uint8 PWR_CHG = 4;
static const uint8 PWR_FLT = 8;

BEGIN_MESSAGE_MAP(ConnectionPage, CPropertyPage)
    ON_WM_SETCURSOR()
    ON_CBN_SELENDOK(IDC_DEVICE_NAME_COMBO,OnDeviceSelect)
    ON_BN_CLICKED(IDC_TECH_PREF_CHECK,OnTechPrefCheck)
    ON_BN_CLICKED(IDC_3GPP_PROFILE_ID_CHECK,OnProfileIdCheck)
    ON_BN_CLICKED(IDC_PRIMARY_DNS_CHECK,OnPrimaryDnsCheck)
    ON_BN_CLICKED(IDC_SECONDARY_DNS_CHECK,OnSecondaryDnsCheck)
    ON_BN_CLICKED(IDC_PRIMARY_NBNS_CHECK,OnPrimaryNbnsCheck)
    ON_BN_CLICKED(IDC_SECONDARY_NBNS_CHECK,OnSecondaryNbnsCheck)
    ON_BN_CLICKED(IDC_APN_NAME_CHECK,OnApnNameCheck)
    ON_BN_CLICKED(IDC_IP_ADDR_CHECK,OnIpAddressCheck)
    ON_BN_CLICKED(IDC_AUTH_PREF_CHECK,OnAuthPrefCheck)
    ON_BN_CLICKED(IDC_USERNAME_CHECK,OnUsernameCheck)
    ON_BN_CLICKED(IDC_PASSWORD_CHECK,OnPasswordCheck)
    ON_BN_CLICKED(IDC_CONNECT_BUTTON,OnConnect)
END_MESSAGE_MAP()


// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ConnectionPage::ConnectionPage() : 
    CPropertyPage(IDD_CONNECTION_PAGE),
    m_pCMDlg(NULL),
    m_useWaitCursor(false),
    m_isSelected(false),
    m_isConnected(false),
    m_pktDataHandle(0),
    m_txId(0)
{
	memset((void *)&m_NdisProfile, 0, sizeof(StConnProfile));
}

// --------------------------------------------------------------------------
// SetCMDlg
// --------------------------------------------------------------------------
// @@@@@@
#ifndef OPEN_PAGE_UI
void ConnectionPage::SetHSDPADlg(CHSDPADlg* pDlg)
{
    m_pCMDlg = pDlg;
}
#else
void ConnectionPage::SetCMDlg(ConnectionManagerDlg* pCMDlg)
{
    m_pCMDlg = pCMDlg;
}
#endif

// --------------------------------------------------------------------------
// DoDataExchange
// --------------------------------------------------------------------------
void ConnectionPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SELECT_DEVICE_STATIC, m_selectDeviceStatic);
    DDX_Control(pDX, IDC_DEVICE_NAME_COMBO, m_deviceNameCombo);
    DDX_Control(pDX, IDC_OVERRIDE_GROUP, m_overrideGoup);
    DDX_Control(pDX, IDC_TECH_PREF_CHECK, m_techPrefCheck);
    DDX_Control(pDX, IDC_TECH_PREF_COMBO, m_techPrefCombo);
    DDX_Control(pDX, IDC_3GPP_PROFILE_ID_CHECK, m_3gppProfileIdCheck);
    DDX_Control(pDX, IDC_3GPP_PROFILE_ID_COMBO, m_3gppProfileIdCombo);
    DDX_Control(pDX, IDC_PRIMARY_DNS_CHECK, m_primaryDnsCheck);
    DDX_Control(pDX, IDC_PRIMARY_DNS_IPADDRESS, m_primaryDnsIpAddr);
    DDX_Control(pDX, IDC_SECONDARY_DNS_CHECK, m_secondaryDnsCheck);
    DDX_Control(pDX, IDC_SECONDARY_DNS_IPADDRESS, m_secondaryDnsIpAddr);
    DDX_Control(pDX, IDC_PRIMARY_NBNS_CHECK, m_primaryNbnsCheck);
    DDX_Control(pDX, IDC_PRIMARY_NBNS_IPADDRESS, m_primaryNbnsIpAddr);
    DDX_Control(pDX, IDC_SECONDARY_NBNS_CHECK, m_secondaryNbnsCheck);
    DDX_Control(pDX, IDC_SECONDARY_NBNS_IPADDRESS, m_secondaryNbnsIpAddr);
    DDX_Control(pDX, IDC_APN_NAME_CHECK, m_apnNameCheck);
    DDX_Control(pDX, IDC_APN_NAME_EDIT, m_apnNameEdit);
    DDX_Control(pDX, IDC_IP_ADDR_CHECK, m_ipAddressCheck);
    DDX_Control(pDX, IDC_IP_ADDR_IPADDRESS, m_ipAddressIpAddr);
    DDX_Control(pDX, IDC_AUTH_PREF_CHECK, m_authPrefCheck);
    DDX_Control(pDX, IDC_AUTH_PREF_COMBO, m_authPrefCombo);
    DDX_Control(pDX, IDC_USERNAME_CHECK, m_usernameCheck);
    DDX_Control(pDX, IDC_USERNAME_EDIT, m_usernameEdit);
    DDX_Control(pDX, IDC_PASSWORD_CHECK, m_passwordCheck);
    DDX_Control(pDX, IDC_PASSWORD_EDIT, m_passwordEdit);
    DDX_Control(pDX, IDC_CONNECT_BUTTON, m_connectButton);
    DDX_Control(pDX, IDC_BATTERY_STATIC, m_batteryStatic);
    DDX_Control(pDX, IDC_BATTERY_PROGRESS, m_batteryProgress);
    DDX_Control(pDX, IDC_EXT_POWER_STATIC, m_extPowerStatic);
    DDX_Control(pDX, IDC_CS_STATIC, m_csStatic);
    DDX_Control(pDX, IDC_PS_STATIC, m_psStatic);
    DDX_Control(pDX, IDC_SIGNAL_STATIC, m_signalStatic);
    DDX_Control(pDX, IDC_SIGNAL_PROGRESS, m_signalProgress);
}

// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL ConnectionPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

    DisableSelectedState();

    // start with no devices
    m_deviceNameCombo.AddString(_T("No Device Detected"));
    m_deviceNameCombo.SetCurSel(0);

    // add predefined values to the technology preference combo box
    m_techPrefCombo.AddString(_T("3GPP"));
    m_techPrefCombo.AddString(_T("3GPP2"));
    m_techPrefCombo.SetCurSel(0);

    // add predefined values to the authentication preference combo box
    m_authPrefCombo.AddString(_T("PAP"));
    m_authPrefCombo.AddString(_T("CHAP"));
    m_authPrefCombo.AddString(_T("PAP and CHAP"));
    m_authPrefCombo.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// --------------------------------------------------------------------------
// PopulateDeviceNames
//
/// Populate the device name combo box with attached network adapters
///
/// @params networkAdapters - vector of string representations of network
///     adapters.
// --------------------------------------------------------------------------
void ConnectionPage::PopulateDeviceNames(std::vector<std::string>& networkAdapters)
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // clear any existing entries
    m_deviceNameCombo.ResetContent();
    int size = (int)networkAdapters.size();

    // check for an empty list
    if (size == 0)
    {
        m_deviceNameCombo.AddString(_T("No Device Detected"));
        m_deviceNameCombo.SetCurSel(0);
        return;
    }

    // add device names
    for (int i = 0; i < size; i++)
    {
        m_deviceNameCombo.AddString(networkAdapters[i].c_str());
    }
#else
	// @@@@@@
    int size = (int)networkAdapters.size();
    // check for an empty list
    if (0 != size)
    {
		TRACE(_T("%s\n"), networkAdapters[0].c_str());
		m_deviceName = networkAdapters[0];
    }

#endif
}

// --------------------------------------------------------------------------
// OnSetCursor
//
/// Change to custom cursor if m_changeCursor = true.
// --------------------------------------------------------------------------
BOOL ConnectionPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    CPoint point(GetMessagePos());
    ScreenToClient(&point);
    
    // use wait cursor, if set, but not over the connect button
    if (m_useWaitCursor && !(&m_connectButton == ChildWindowFromPoint(point)))
    {
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
        return TRUE;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

// --------------------------------------------------------------------------
// OnDeviceSelect
//
/// Actions to take when a device name is selected from the device name combo
/// box.
// --------------------------------------------------------------------------
void ConnectionPage::OnDeviceSelect()
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // get device name from combo box
    CString deviceName;
    m_deviceNameCombo.GetWindowText(deviceName);   

    // device name required to connect
    if (deviceName.Compare("No Device Detected") == 0 ||
        deviceName.Compare("") == 0)
    {
        return;
    }

    // do nothing if device is already selected
    if (deviceName.Compare(m_deviceName.c_str()) == 0)
    {
        return;
    }

    // disconnect existing conection
    if (m_isSelected)
    {
        m_pCMDlg->DeselectDevice();
        return;
    }

    m_deviceName = deviceName;
#else
	m_deviceName = "Qualcomm Wireless Ethernet Adapter 7002 #8";
#endif

    // connect to device
    m_pCMDlg->SelectDevice(m_deviceName);
}

// @@@@@@
#ifndef OPEN_PAGE_UI
void ConnectionPage::OnDeviceSelect(std::string& deviceName)
{
    m_pCMDlg->SelectDevice(deviceName);
}
#endif

// --------------------------------------------------------------------------
// BuildDeviceNames
//
/// Populate the device name combo box with successful device names from 
/// previous sessions.
// --------------------------------------------------------------------------
void ConnectionPage::BuildDeviceNames()
{
    // get stored device names from previous sessions
    CWinApp* pApp = AfxGetApp();




    CString deviceNames;
	deviceNames= pApp->GetProfileString(DEVICE_SECTION,DEVICE_ENTRY);

    // add them to the device name combo box

	char seps[] = ",";
	char *token = NULL;

	int nLen = deviceNames.GetLength() + 1;
	char *pszTempText = new char[nLen];
	memset((void *)pszTempText, 0, nLen);
	USES_CONVERSION; 
	strcpy(pszTempText, W2A(deviceNames));

	token = strtok(pszTempText, seps);
	while (token != NULL)
	{
		/* While there are tokens in "string" */
	//	TRACE("%s\n", token);
		m_deviceNameCombo.AddString(A2W(token));

		/* Get next token: */
		token = strtok(NULL, seps);
	}
	delete [] pszTempText;





}

// --------------------------------------------------------------------------
// StoreDeviceNames
//
/// Store successful device names for future sessions.
///
/// @param deviceName - the device name to store
// --------------------------------------------------------------------------
void ConnectionPage::StoreDeviceNames(CString& deviceName)
{
    // add comma delimiter
    deviceName += ",";

    // get stored device names from previous sessions
    CWinApp* pApp = AfxGetApp();
    CString deviceNames = pApp->GetProfileString(DEVICE_SECTION,DEVICE_ENTRY);

    // check if device name already stored
    int pos = deviceNames.Find(deviceName);

    // if already at head of list, we're done
    if (pos == 0) { return; }

    // if in the list but not at the head, delete from list
    if (pos != -1)
    {
        deviceNames.Delete(pos,deviceName.GetLength());
    }

    // add entry to head of device names
    deviceName += deviceNames;
    pApp->WriteProfileString(DEVICE_SECTION,DEVICE_ENTRY,deviceName);

    // repopulate the device name combo box
    m_deviceNameCombo.ResetContent();
    BuildDeviceNames();
    m_deviceNameCombo.SetCurSel(0);
}

// --------------------------------------------------------------------------
// EnableSelectedState
//
/// Show availabe fields and send requests to populate them.
// --------------------------------------------------------------------------
void ConnectionPage::EnableSelectedState()
{
    // set the device connected flag
    m_isSelected = true;

// @@@@@@
#ifdef OPEN_PAGE_UI
    // enable/disable appropriate fields
    m_overrideGoup.EnableWindow(TRUE);
    m_techPrefCheck.EnableWindow(TRUE);
    OnTechPrefCheck();
    m_3gppProfileIdCheck.EnableWindow(TRUE);
    OnProfileIdCheck();
    m_primaryDnsCheck.EnableWindow(TRUE);
    OnPrimaryDnsCheck();
    m_secondaryDnsCheck.EnableWindow(TRUE);
    OnSecondaryDnsCheck();
    m_primaryNbnsCheck.EnableWindow(TRUE);
    OnPrimaryNbnsCheck();
    m_secondaryNbnsCheck.EnableWindow(TRUE);
    OnSecondaryNbnsCheck();
    m_apnNameCheck.EnableWindow(TRUE);
    OnApnNameCheck();
    m_ipAddressCheck.EnableWindow(TRUE);
    OnIpAddressCheck();
    m_authPrefCheck.EnableWindow(TRUE);
    OnAuthPrefCheck();
    m_usernameCheck.EnableWindow(TRUE);
    OnUsernameCheck();
    m_passwordCheck.EnableWindow(TRUE);
    OnPasswordCheck();
    m_connectButton.EnableWindow(TRUE);
    m_batteryStatic.SetIcon((HICON)LoadImage(
        AfxGetInstanceHandle(),
			// @@
#if 0
        MAKEINTRESOURCE(IDI_BATTERY_ICON),
#else
        MAKEINTRESOURCE(IDI_ICON_HELP),
#endif
        IMAGE_ICON,0,0,LR_LOADTRANSPARENT
        ));
    m_batteryProgress.EnableWindow(TRUE);
    m_signalStatic.SetIcon((HICON)LoadImage(
        AfxGetInstanceHandle(),
			// @@
#if 0
        MAKEINTRESOURCE(IDI_SIGNAL_ICON),
#else
        MAKEINTRESOURCE(IDI_ICON_HELP),
#endif
        IMAGE_ICON,0,0,LR_LOADTRANSPARENT
        ));
    m_signalProgress.EnableWindow(TRUE);
#endif // OPEN_PAGE_UI

    // send power status request and event report
    m_pCMDlg->SendRequest(BuildDMSPowerStatusString());
    m_pCMDlg->SendRequest(BuildDMSEventReportString());

    // send signal strength request and event report
    m_pCMDlg->SendRequest(BuildNASSignalStrengthString());
    m_pCMDlg->SendRequest(BuildNASEventReportString());

    // send serving system request
    m_pCMDlg->SendRequest(BuildNASServingSystemString());
}

// --------------------------------------------------------------------------
// DisableSelectedState
//
/// Dim fields that should not be accessed.
// --------------------------------------------------------------------------
void ConnectionPage::DisableSelectedState()
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // disable started state if necessary
    if (m_isConnected)
    {
        DisableConnectedState();
    }

    // set the device connected flag, clear the device name
    m_isSelected = false;

	// @@
    //m_deviceName.clear();
	m_deviceName.erase(m_deviceName.begin(), m_deviceName.end());

    // enable/disable appropriate fields
    m_overrideGoup.EnableWindow(FALSE);
    m_techPrefCheck.EnableWindow(FALSE);
    m_techPrefCombo.EnableWindow(FALSE);
    m_3gppProfileIdCheck.EnableWindow(FALSE);
    m_3gppProfileIdCombo.EnableWindow(FALSE);
    m_3gppProfileIdCombo.ResetContent();
    m_primaryDnsCheck.EnableWindow(FALSE);
    m_primaryDnsIpAddr.EnableWindow(FALSE);
    m_secondaryDnsCheck.EnableWindow(FALSE);
    m_secondaryDnsIpAddr.EnableWindow(FALSE);
    m_primaryNbnsCheck.EnableWindow(FALSE);
    m_primaryNbnsIpAddr.EnableWindow(FALSE);
    m_secondaryNbnsCheck.EnableWindow(FALSE);
    m_secondaryNbnsIpAddr.EnableWindow(FALSE);
    m_apnNameCheck.EnableWindow(FALSE);
    m_apnNameEdit.EnableWindow(FALSE);
    m_ipAddressCheck.EnableWindow(FALSE);
    m_ipAddressIpAddr.EnableWindow(FALSE);
    m_authPrefCheck.EnableWindow(FALSE);
    m_authPrefCombo.EnableWindow(FALSE);
    m_usernameCheck.EnableWindow(FALSE);
    m_usernameEdit.EnableWindow(FALSE);
    m_passwordCheck.EnableWindow(FALSE);
    m_passwordEdit.EnableWindow(FALSE);
    m_connectButton.EnableWindow(FALSE);
    m_batteryStatic.SetIcon((HICON)LoadImage(
        AfxGetInstanceHandle(),
			// @@
#if 0
        MAKEINTRESOURCE(IDI_DIM_BATTERY_ICON),
#else
        MAKEINTRESOURCE(IDI_ICON_HELP),
#endif
        IMAGE_ICON,0,0,LR_LOADTRANSPARENT
        ));
    m_batteryProgress.SetPos(0);
    m_batteryProgress.EnableWindow(FALSE);
    m_extPowerStatic.ShowWindow(SW_HIDE);
    m_csStatic.ShowWindow(SW_HIDE);
    m_psStatic.ShowWindow(SW_HIDE);
    m_signalStatic.SetIcon((HICON)LoadImage(
        AfxGetInstanceHandle(),
			// @@
#if 0
        MAKEINTRESOURCE(IDI_DIM_SIGNAL_ICON),
#else
        MAKEINTRESOURCE(IDI_ICON_HELP),
#endif
        IMAGE_ICON,0,0,LR_LOADTRANSPARENT
        ));
    m_signalProgress.SetPos(0);
    m_signalProgress.EnableWindow(FALSE);
#else
    // disable started state if necessary
    if (m_isConnected)
    {
        DisableConnectedState();
    }

    // set the device connected flag, clear the device name
    m_isSelected = false;

	// @@
    //m_deviceName.clear();
	m_deviceName.erase(m_deviceName.begin(), m_deviceName.end());
#endif
}

// --------------------------------------------------------------------------
// BuildDMSPowerStatusString
//
/// Build a string of message type and name/value pairs representing a
/// DMSGetPowerStatusReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ConnectionPage::BuildDMSPowerStatusString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_DMS_GET_POWER_STATE_REQ" << std::endl 
           << "{}";
    
    return stream.str();
}

// --------------------------------------------------------------------------
// BuildDMSEventReportString
//
/// Build a string of message type and name/value pairs representing a
/// DMSSetEventReportReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ConnectionPage::BuildDMSEventReportString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_DMS_SET_EVENT_REPORT_REQ" << std::endl 
           << "{" << std::endl;
    
    // power state reporting mode
    stream << "  PowerStateReportMode " << 1 << std::endl;

    // battery level report limits
    stream << "  BatteryLvlLowerLimit " << 50 << std::endl
           << "  BatteryLvlUpperLimit " << 50 << std::endl;

    stream << "}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildNASSignalStrengthString
//
/// Build a string of message type and name/value pairs representing a
/// NASGetSignalStrengthReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ConnectionPage::BuildNASSignalStrengthString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_NAS_GET_SIGNAL_STRENGTH_REQ" << std::endl 
           << "{}" << std::endl;
    
    return stream.str();
}

// --------------------------------------------------------------------------
// BuildNASEventReportString
//
/// Build a string of message type and name/value pairs representing a
/// NASSetEventReportReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ConnectionPage::BuildNASEventReportString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_NAS_SET_EVENT_REPORT_REQ" << std::endl 
           << "{" << std::endl;
    
    // signal strength reporting mode
    stream << "  ReportSigStrength " << 1 << std::endl;

    // signal strength range limits, max 5
    stream << "  RangeLimit " << -106 << std::endl
           << "  RangeLimit " << -100 << std::endl
           << "  RangeLimit " << -93 << std::endl
           << "  RangeLimit " << -86 << std::endl
           << "  RangeLimit " << -80 << std::endl;

    stream << "}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildNASServingSystemString
//
/// Build a string of message type and name/value pairs representing a
/// NASGetServingSystemReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ConnectionPage::BuildNASServingSystemString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_NAS_GET_SERVING_SYSTEM_REQ" << std::endl 
           << "{}";
    
    return stream.str();
}

// --------------------------------------------------------------------------
// ProcessDMSGetPowerStateRsp
//
/// Actions to take when a DMSGetPowerStateRspRCP message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void ConnectionPage::ProcessDMSGetPowerStateRsp(DMSGetPowerStateRspRCP rspRCP)
{
    // get power state
    uint8 powerStatus = rspRCP->GetPowerStatus();
    uint8 batteryLvl = rspRCP->GetBatteryLvl();

    // activate the appropriate power state indicators
    ProcessPowerState(powerStatus,batteryLvl);
}

// --------------------------------------------------------------------------
// ProcessDMSEventReportInd
//
/// Actions to take when a DMSEventReportIndRCP message is received.
///
/// @params indRCP - reference counted pointer indicator to process.
// --------------------------------------------------------------------------
void ConnectionPage::ProcessDMSEventReportInd(DMSEventReportIndRCP indRCP)
{
    // get power state
    uint8 powerStatus = indRCP->GetPowerStatus();
    uint8 batteryLvl = indRCP->GetBatteryLvl();

    // activate the appropriate power state indicators
    ProcessPowerState(powerStatus,batteryLvl);
}

// --------------------------------------------------------------------------
// ProcessPowerState
//
/// Activate the appropriate power state indicators on the page.
///
/// @params powerStatus - power status of the device.
///
/// @params batteryLvl - battery level of from the device.
// --------------------------------------------------------------------------
void ConnectionPage::ProcessPowerState(uint8 powerStatus,uint8 batteryLvl)
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // set battery progress bar
    CString text;
    text.Format("%d%%",batteryLvl);
    m_batteryProgress.SetText(text);
    m_batteryProgress.SetPos(batteryLvl);

    // show/hide external power source icon
    if (powerStatus & PWR_EXT)
    {
        m_extPowerStatic.ShowWindow(SW_SHOW);
    }
    else
    {
        m_extPowerStatic.ShowWindow(SW_HIDE);
    }

    // set battery connected/disconnected icon
    if (powerStatus & PWR_BAT)
    {
        m_batteryStatic.SetIcon((HICON)LoadImage(
            AfxGetInstanceHandle(),
			// @@
#if 0
            MAKEINTRESOURCE(IDI_BATTERY_ICON),
#else
        MAKEINTRESOURCE(IDI_ICON_HELP),
#endif
            IMAGE_ICON,0,0,LR_LOADTRANSPARENT
            ));
    }
    else
    {
        m_batteryStatic.SetIcon((HICON)LoadImage(
            AfxGetInstanceHandle(),
			// @@
#if 0
            MAKEINTRESOURCE(IDI_NO_BATTERY_ICON),
#else
        MAKEINTRESOURCE(IDI_ICON_HELP),
#endif
            IMAGE_ICON,0,0,LR_LOADTRANSPARENT
            ));
    }

    // set battery connected/disconnected icon
    if (powerStatus & PWR_CHG)
    {
        m_batteryStatic.SetIcon((HICON)LoadImage(
            AfxGetInstanceHandle(),
			// @@
#if 0
            MAKEINTRESOURCE(IDI_CHG_BATTERY_ICON),
#else
        MAKEINTRESOURCE(IDI_ICON_HELP),
#endif
            IMAGE_ICON,0,0,LR_LOADTRANSPARENT
            ));
    }
#endif // OPEN_PAGE_UI
}

// --------------------------------------------------------------------------
// ProcessNASGetSignalStrengthRsp
//
/// Actions to take when a NASGetSignalStrengthRspRCP message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void ConnectionPage::ProcessNASGetSignalStrengthRsp(NASGetSignalStrengthRspRCP rspRCP)
{
    // get signal strength info
    int8 signalStrength = rspRCP->GetSignalStrength();
    uint8 radioIf = rspRCP->GetRadioIf();

    // activate the appropriate signal strength indicators
    ProcessSignalStrength(signalStrength,radioIf);
}

// --------------------------------------------------------------------------
// ProcessNASEventReportInd
//
/// Actions to take when a NASEventReportIndRCP message is received.
///
/// @params indRCP - reference counted pointer indicator to process.
// --------------------------------------------------------------------------
void ConnectionPage::ProcessNASEventReportInd(NASEventReportIndRCP indRCP)
{
    // get signal strength info
    int8 signalStrength = indRCP->GetSignalStrength();
    uint8 radioIf = indRCP->GetRadioIf();

    // activate the appropriate signal strength indicators
    ProcessSignalStrength(signalStrength,radioIf);
}

// --------------------------------------------------------------------------
// ProcessSignalStrength
//
/// Activate the appropriate signal strength indicators on the page.
///
/// @params signalStrength - signal strength level of the device.
///
/// @params radioIf - radio interface technology of from the device.
// --------------------------------------------------------------------------
void ConnectionPage::ProcessSignalStrength(int8 signalStrength,uint8 radioIf)
{
    // set progress text
    m_signalProgress.SetText(RADIO_IF_TECH_STRINGS[radioIf].c_str());

    // per Jeff Dyck, usable signal strengths are in the range
    // -80 (full strength) to -106 dBm (no signal).

    // calculate position
    int pos = (int)((double)(signalStrength + 106)*100/(double)(-80 + 106));

    // set position
    m_signalProgress.SetPos(pos);
}

// --------------------------------------------------------------------------
// ProcessNASGetServingSystemRsp
//
/// Actions to take when a void NASGetServingSystemRspRCP message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void ConnectionPage::ProcessNASGetServingSystemRsp(NASGetServingSystemRspRCP rspRCP)
{
    // get cs and ps attach states
    uint8 csAttachState = rspRCP->GetCsAttachState();
    uint8 psAttachState = rspRCP->GetPsAttachState();

    // activate the appropriate cs/ps attach state indicators
    ProcessAttachState(csAttachState,psAttachState);
}

// --------------------------------------------------------------------------
// ProcessNASServingSystemInd
//
/// Actions to take when a void NASServingSystemIndRCP message is received.
///
/// @params rspRCP - reference counted pointer response to process.
// --------------------------------------------------------------------------
void ConnectionPage::ProcessNASServingSystemInd(NASServingSystemIndRCP indRCP)
{
    // get cs and ps attach states
    uint8 csAttachState = indRCP->GetCsAttachState();
    uint8 psAttachState = indRCP->GetPsAttachState();

    // activate the appropriate cs/ps attach state indicators
    ProcessAttachState(csAttachState,psAttachState);
}

// --------------------------------------------------------------------------
// ProcessAttachState
//
/// Activate the appropriate cs/ps attach state indicators on the page.
///
/// @params csAttachState - circuit switch attach state of the device.
///
/// @params psAttachState - packet switch attach state of the device.
// --------------------------------------------------------------------------
void ConnectionPage::ProcessAttachState(uint8 csAttachState,uint8 psAttachState)
{
    // cs attach state
    if (csAttachState == 1)
    {
        m_csStatic.ShowWindow(SW_SHOW);
    }
    else
    {
        m_csStatic.ShowWindow(SW_HIDE);
    }

    // ps attach state
    if (psAttachState == 1)
    {
        m_psStatic.ShowWindow(SW_SHOW);
    }
    else
    {
        m_psStatic.ShowWindow(SW_HIDE);
    }
}

// --------------------------------------------------------------------------
// ProcessWDSPktSrvcStatusInd
//
/// Actions to take on a change in network interface connectivity.
///
/// @params indRCP - the indicator messge to process
// --------------------------------------------------------------------------
void ConnectionPage::ProcessWDSPktSrvcStatusInd(WDSPktSrvcStatusIndRCP indRCP)
{
    // disable connected state on disconnection with no reconfigure required
    if (m_isConnected &&
        indRCP->GetConnectionStatus() == 1 && 
        indRCP->GetReconfigurationRequired() == 0)
    {
        DisableConnectedState();
    }
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileListRsp
//
/// Actions to take on a change in network interface connectivity.
///
/// @params indRCP - the indicator messge to process
// --------------------------------------------------------------------------
void ConnectionPage::ProcessWDSGetProfileListRsp(WDSGetProfileListRspRCP rspRCP)
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // populate profile id dropdown
    std::vector<ProfileListInstance*> profileList = 
        rspRCP->GetProfileListInstances();

    // clear the current entries
    m_3gppProfileIdCombo.ResetContent();

    // add reported profiles
    size_t numProfiles = profileList.size();
    for (uint32 i = 0; i < numProfiles; i++)
    {
        // filter by type
        if (profileList[i]->GetProfileType() == 0)
        {
            int idx = profileList[i]->GetProfileIndex();

            // build string for combo box
            std::stringstream stream;
            stream << idx << ": " << profileList[i]->GetProfileName() 
                   << std::ends;

            // insert string in combo box
            int result = m_3gppProfileIdCombo.AddString(stream.str().c_str());

            if (result != LB_ERR)
            {
                // set index as item data
                m_3gppProfileIdCombo.SetItemData(result,idx);
            }
        }
     }
#endif
}

// --------------------------------------------------------------------------
// OnTechPrefCheck
//
/// Actions to take when the technology preference check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnTechPrefCheck()
{
    BOOL isChecked = (m_techPrefCheck.GetCheck() == BST_CHECKED);
    m_techPrefCombo.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnProfileIdCheck
//
/// Actions to take when the profile id check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnProfileIdCheck()
{
    BOOL isChecked = (m_3gppProfileIdCheck.GetCheck() == BST_CHECKED);
    m_3gppProfileIdCombo.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnPrimaryDnsCheck
//
/// Actions to take when the primary dns check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnPrimaryDnsCheck()
{
    BOOL isChecked = (m_primaryDnsCheck.GetCheck() == BST_CHECKED);
    m_primaryDnsIpAddr.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnSecondaryDnsCheck
//
/// Actions to take when the secondary dns check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnSecondaryDnsCheck()
{
    BOOL isChecked = (m_secondaryDnsCheck.GetCheck() == BST_CHECKED);
    m_secondaryDnsIpAddr.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnPrimaryNbnsCheck
//
/// Actions to take when the primary nbns check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnPrimaryNbnsCheck()
{
    BOOL isChecked = (m_primaryNbnsCheck.GetCheck() == BST_CHECKED);
    m_primaryNbnsIpAddr.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnSecondaryNbnsCheck
//
/// Actions to take when the secondary nbns check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnSecondaryNbnsCheck()
{
    BOOL isChecked = (m_secondaryNbnsCheck.GetCheck() == BST_CHECKED);
    m_secondaryNbnsIpAddr.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnApnNameCheck
//
/// Actions to take when the apn name check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnApnNameCheck()
{
    BOOL isChecked = (m_apnNameCheck.GetCheck() == BST_CHECKED);
    m_apnNameEdit.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnIpAddressCheck
//
/// Actions to take when the ip address check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnIpAddressCheck()
{
    BOOL isChecked = (m_ipAddressCheck.GetCheck() == BST_CHECKED);
    m_ipAddressIpAddr.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnAuthPrefCheck
//
/// Actions to take when the authentication preference check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnAuthPrefCheck()
{
    BOOL isChecked = (m_authPrefCheck.GetCheck() == BST_CHECKED);
    m_authPrefCombo.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnUsernameCheck
//
/// Actions to take when the username check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnUsernameCheck()
{
    BOOL isChecked = (m_usernameCheck.GetCheck() == BST_CHECKED);
    m_usernameEdit.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnPasswordCheck
//
/// Actions to take when the password check box is clicked.
// --------------------------------------------------------------------------
void ConnectionPage::OnPasswordCheck()
{
    BOOL isChecked = (m_passwordCheck.GetCheck() == BST_CHECKED);
    m_passwordEdit.EnableWindow(isChecked);
}

// --------------------------------------------------------------------------
// OnConnect
//
/// Actions to take when the connect button is pressed.
// --------------------------------------------------------------------------

void ConnectionPage::OnConnect()
{


    std::string msgStr;

    CString text;
    m_connectButton.GetWindowText(text);

    // determine action by button label
    if (text.Compare(_T("Connect")) == 0)
    {
        // build start network interface string, enable abort
        msgStr = BuildWDSStartString();
        EnableAbortState();
    }
    else if (text.Compare(_T("Abort")) == 0)
    {
        // build abort string
        msgStr = BuildWDSAbortString();
    }
    else if (text.Compare(_T("Disconnect")) == 0)
    {
        // build stop network interface string
        msgStr = BuildWDSStopString();
    }
    else
    {
        AfxMessageBox(_T("Connect button has invalid text."));
    }

    m_pCMDlg->SendRequest(msgStr);


}


// @@@@@@
static BOOL bIsNotFirstReportSpeed = TRUE;
#ifndef OPEN_PAGE_UI
void ConnectionPage::OnConnectEx(NdisStateRet ndisStatus, StConnProfile *profile)
{
    std::string msgStr;

	//if (NDIS_STATE_ABORT == ndisStatus) {
	//}

    // determine action by button label
    if (NDIS_STATE_DISCONNECT == ndisStatus)	// 如果当前是Disconnect的话，可以连接
    {
		// 取得当前的profile
		if (NULL != profile) {
			memcpy((void *)(&m_NdisProfile), (void *)profile, sizeof(StConnProfile));
		}
		OnDeviceSelect(m_deviceName);

        // build start network interface string, enable abort
        msgStr = BuildWDSStartString();
        EnableAbortState();

		// @@@@@@
		if (!bIsNotFirstReportSpeed)
			m_pCMDlg->m_statisticsPage.EnableSelectedState();
		else
			bIsNotFirstReportSpeed = FALSE;
    }
    else if (NDIS_STATE_ABORT == ndisStatus)	// 如果当前是Connect的话，可以Abort
    {
        // build abort string
        msgStr = BuildWDSAbortString();
    }
    else if (NDIS_STATE_CONNECT == ndisStatus)	// 如果当前是Connect的话，可以断开
    {
        // build stop network interface string
        msgStr = BuildWDSStopString();
    }
    else
    {
        AfxMessageBox(_T("Connect button has invalid text."));
    }

    m_pCMDlg->SendRequest(msgStr);
}
#endif

// --------------------------------------------------------------------------
// BuildWDSStartString
//
/// Build a string of message type and name/value pairs based on the user
/// input.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
// @@@@@@
#ifdef OPEN_PAGE_UI
std::string ConnectionPage::BuildWDSStartString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_WDS_START_NETWORK_INTERFACE_REQ" << std::endl
           << "{" << std::endl;

    // technology preference
    if (m_techPrefCheck.GetCheck() == BST_CHECKED)
    {
        int idx = m_techPrefCombo.GetCurSel();
        stream << "  TechPref " << (int)(idx + 1) << std::endl;
    }

    // 3gpp profile id
    if (m_3gppProfileIdCheck.GetCheck() == BST_CHECKED)
    {
        int idx = m_3gppProfileIdCombo.GetCurSel();
        if (idx != CB_ERR)
        {
            stream << "  ProfileId " << (int)m_3gppProfileIdCombo.GetItemData(idx) 
                   << std::endl;
        }
    }

    // primary dns
    if (m_primaryDnsCheck.GetCheck() == BST_CHECKED)
    {
        CString primaryDns;
        m_primaryDnsIpAddr.GetWindowText(primaryDns);
        stream << "  PrimaryDnsPref " << primaryDns << std::endl;
    }

    // secondary dns
    if (m_secondaryDnsCheck.GetCheck() == BST_CHECKED)
    {
        CString secondaryDns;
        m_secondaryDnsIpAddr.GetWindowText(secondaryDns);
        stream << "  SecondaryDnsPref " << secondaryDns << std::endl;
    }

    // primary Nbns
    if (m_primaryNbnsCheck.GetCheck() == BST_CHECKED)
    {
        CString primaryNbns;
        m_primaryNbnsIpAddr.GetWindowText(primaryNbns);
        stream << "  PrimaryNbnsPref " << primaryNbns << std::endl;
    }

    // secondary nbns
    if (m_secondaryNbnsCheck.GetCheck() == BST_CHECKED)
    {
        CString secondaryNbns;
        m_secondaryNbnsIpAddr.GetWindowText(secondaryNbns);
        stream << "  SecondaryNbnsPref " << secondaryNbns << std::endl;
    }

    // apn name
    if (m_apnNameCheck.GetCheck() == BST_CHECKED)
    {
	// @@@@@@
	#if 0
        CString apnName;
        m_apnNameEdit.GetWindowText(apnName);

		apnName.TrimRight().TrimLeft();
		apnName.TrimRight();
		apnName.TrimLeft();
        if (apnName == "") { apnName = "NULL"; }
	#else
		char szText[MAX_PATH] = { 0 };
		::SendMessage(m_apnNameEdit.m_hWnd, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM)szText);
	
		if (0 == strcmp(szText, "")) {
			strcpy(szText, "NULL");
		}
	#endif

        stream << "  ApnName " << szText << std::endl;
    }

    // ip address
    if (m_ipAddressCheck.GetCheck() == BST_CHECKED)
    {
        CString ipAddress;
        m_ipAddressIpAddr.GetWindowText(ipAddress);
        stream << "  Ipv4AddrPref " << ipAddress << std::endl;
    }

    // authentication preference
    if (m_authPrefCheck.GetCheck() == BST_CHECKED)
    {
	// @@@@@@
	#if 0
        int idx = m_authPrefCombo.GetCurSel();
	#else
		int idx = 1;
        stream << "  AuthPref " << (int)(idx + 1) << std::endl;
	#endif
    }

    // username
    if (m_usernameCheck.GetCheck() == BST_CHECKED)
    {
	// @@@@@@
	#if 0
        CString username;
        m_usernameEdit.GetWindowText(username);

        username.TrimRight().TrimLeft();
		username.TrimRight();
		username.TrimLeft();

        if (username == "") { username = "NULL"; }
	#else
		char szText[MAX_PATH] = { 0 };
		::SendMessage(m_usernameEdit.m_hWnd, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM)szText);
	
		if (0 == strcmp(szText, "")) {
			strcpy(szText, "NULL");
		}
	#endif

        stream << "  Username " << username << std::endl;
    }

    // password
    if (m_passwordCheck.GetCheck() == BST_CHECKED)
    {

	#if 0
        CString password;
        m_passwordEdit.GetWindowText(password);

        password.TrimRight().TrimLeft();
		password.TrimRight();
		password.TrimLeft();

        if (password == "") { password = "NULL"; }
	#else
		char szText[MAX_PATH] = { 0 };
		::SendMessage(m_passwordEdit.m_hWnd, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM)szText);

		if (0 == strcmp(szText, "")) {
			strcpy(szText, "NULL");
		}
	#endif

        stream << "  Password " << password << std::endl;
    }

    stream << "}";

    return stream.str();
}
#else // OPEN_PAGE_UI
std::string ConnectionPage::BuildWDSStartString()
{
    std::stringstream stream;

	int idx = 0;

    // msg type
    stream << "QMI_WDS_START_NETWORK_INTERFACE_REQ" << std::endl
           << "{" << std::endl;

    // technology preference
    idx = 0; /* 1: 3GPP */
    stream << "  TechPref " << (int)(idx + 1) << std::endl;

    // 3gpp profile id
	// ...

    // primary dns
	// ...

    // secondary dns
	// ...

    // primary Nbns
	// ...

    // secondary nbns
	// ...

    // apn name

USES_CONVERSION;

    if (0 == strcmp(W2A(m_NdisProfile.szAPN), "")) 
	{ 
		wcscpy(m_NdisProfile.szAPN, _T("NULL")); 
	}
	stream << "  ApnName " << W2A(m_NdisProfile.szAPN) << std::endl;

    // ip address
    // ...

    // authentication preference
	idx = 0;
	if(m_NdisProfile.AuthProtocol == AUTHPROTOCOL_CHAP)
	{
		stream << "  AuthPref " << (int)(idx + 2) << std::endl;
	}
	else 
	{
		stream << "  AuthPref " << (int)(idx + 1) << std::endl;
	}

    // username
    if (0 == strcmp(W2A(m_NdisProfile.szUserName) , "")) 
	{ 
		wcscpy(m_NdisProfile.szUserName, _T("NULL")); 
	}
	stream << "  Username " << W2A(m_NdisProfile.szUserName) << std::endl;

    // password
    if (0 == strcmp(W2A(m_NdisProfile.szPassword) , "")) 
	{ 
		wcscpy(m_NdisProfile.szPassword, _T("NULL")); 
	}
	stream << "  Password " << W2A(m_NdisProfile.szPassword) << std::endl;

    stream << "}";


    return stream.str();
}
#endif // OPEN_PAGE_UI
// --------------------------------------------------------------------------
// BuildWDSAbortString
//
/// Build a string of message type and name/value pairs based on the user
/// input.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ConnectionPage::BuildWDSAbortString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_WDS_ABORT_REQ" << std::endl
           << "{" << std::endl;

    // transaction id to abort
    stream << "  TxId " << m_txId << std::endl
           << "}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildWDSStopString
//
/// Build a string of message type and name/value pairs representing a
/// WDSStopNetworkInterfaceReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ConnectionPage::BuildWDSStopString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_WDS_STOP_NETWORK_INTERFACE_REQ" << std::endl
           << "{" << std::endl;
    
    // packet data handle
    stream << "  PktDataHandle " << m_pktDataHandle << std::endl
           << "}";

    return stream.str();
}

// --------------------------------------------------------------------------
// EnableConnectedState
//
/// Show availabe fields.
// --------------------------------------------------------------------------
void ConnectionPage::EnableConnectedState(uint32 pktDataHandle)
{
    // set interface started flag
    m_isConnected = true;

    // store the packet data handle
    m_pktDataHandle = pktDataHandle;

    // use arrow cursor
    m_useWaitCursor = false;

// @@@@@@
#ifdef OPEN_PAGE_UI
    // disable user inputs, change button:start interface to stop interface
    m_selectDeviceStatic.EnableWindow(FALSE);
    m_deviceNameCombo.EnableWindow(FALSE);
    m_overrideGoup.EnableWindow(FALSE);
    m_techPrefCheck.EnableWindow(FALSE);
    m_techPrefCombo.EnableWindow(FALSE);
    m_3gppProfileIdCheck.EnableWindow(FALSE);
    m_3gppProfileIdCombo.EnableWindow(FALSE);
    m_primaryDnsCheck.EnableWindow(FALSE);
    m_primaryDnsIpAddr.EnableWindow(FALSE);
    m_secondaryDnsCheck.EnableWindow(FALSE);
    m_secondaryDnsIpAddr.EnableWindow(FALSE);
    m_primaryNbnsCheck.EnableWindow(FALSE);
    m_primaryNbnsIpAddr.EnableWindow(FALSE);
    m_secondaryNbnsCheck.EnableWindow(FALSE);
    m_secondaryNbnsIpAddr.EnableWindow(FALSE);
    m_apnNameCheck.EnableWindow(FALSE);
    m_apnNameEdit.EnableWindow(FALSE);
    m_ipAddressCheck.EnableWindow(FALSE);
    m_ipAddressIpAddr.EnableWindow(FALSE);
    m_authPrefCheck.EnableWindow(FALSE);
    m_authPrefCombo.EnableWindow(FALSE);
    m_usernameCheck.EnableWindow(FALSE);
    m_usernameEdit.EnableWindow(FALSE);
    m_passwordCheck.EnableWindow(FALSE);
    m_passwordEdit.EnableWindow(FALSE);
    m_connectButton.SetWindowText(_T("Disconnect"));
#else
	//m_pCMDlg->m_pTabDialog->m_btn4.SetWindowText(_T("Disconnect"));

//	CSetupDlg* pSetupWnd = (CSetupDlg*)(m_pCMDlg->m_pSetupDlg);


 	m_pCMDlg->m_pConnectDlg->PostMessage(WM_UPDATE_CONNECT_STATE, NDIS_STATE_CONNECT, NULL);
#endif
}

// --------------------------------------------------------------------------
// EnableAbortState
//
/// Show availabe fields.
// --------------------------------------------------------------------------
void ConnectionPage::EnableAbortState()
{
    // use wait cursor
    m_useWaitCursor = true;

// @@@@@@
#ifdef OPEN_PAGE_UI
    // disable user inputs, change button:start interface to stop interface
    m_selectDeviceStatic.EnableWindow(FALSE);
    m_deviceNameCombo.EnableWindow(FALSE);
    m_overrideGoup.EnableWindow(FALSE);
    m_techPrefCheck.EnableWindow(FALSE);
    m_techPrefCombo.EnableWindow(FALSE);
    m_3gppProfileIdCheck.EnableWindow(FALSE);
    m_3gppProfileIdCombo.EnableWindow(FALSE);
    m_primaryDnsCheck.EnableWindow(FALSE);
    m_primaryDnsIpAddr.EnableWindow(FALSE);
    m_secondaryDnsCheck.EnableWindow(FALSE);
    m_secondaryDnsIpAddr.EnableWindow(FALSE);
    m_primaryNbnsCheck.EnableWindow(FALSE);
    m_primaryNbnsIpAddr.EnableWindow(FALSE);
    m_secondaryNbnsCheck.EnableWindow(FALSE);
    m_secondaryNbnsIpAddr.EnableWindow(FALSE);
    m_apnNameCheck.EnableWindow(FALSE);
    m_apnNameEdit.EnableWindow(FALSE);
    m_ipAddressCheck.EnableWindow(FALSE);
    m_ipAddressIpAddr.EnableWindow(FALSE);
    m_authPrefCheck.EnableWindow(FALSE);
    m_authPrefCombo.EnableWindow(FALSE);
    m_usernameCheck.EnableWindow(FALSE);
    m_usernameEdit.EnableWindow(FALSE);
    m_passwordCheck.EnableWindow(FALSE);
    m_passwordEdit.EnableWindow(FALSE);
    m_connectButton.SetWindowText(_T("Abort"));
#else
	//m_pCMDlg->m_pTabDialog->m_btn4.SetWindowText(_T("Abort"));

//	CSetupDlg* pSetupWnd = (CSetupDlg*)(m_pCMDlg->m_pSetupDlg);

 m_pCMDlg->m_pConnectDlg->PostMessage(WM_UPDATE_CONNECT_STATE, NDIS_STATE_ABORT, NULL);
#endif
}

// --------------------------------------------------------------------------
// DisableConnectedState
//
/// Dim fields that should not be accessed.
// --------------------------------------------------------------------------
void ConnectionPage::DisableConnectedState()
{
    // set interface started flag
    m_isConnected = false;

    // reset the packet data handle
    m_pktDataHandle = 0;

    // use arrow cursor
    m_useWaitCursor = false;

// @@@@@@
#ifdef OPEN_PAGE_UI
    // enable user inputs, change button:start interface to stop interface
    m_selectDeviceStatic.EnableWindow(TRUE);
    m_deviceNameCombo.EnableWindow(TRUE);
    m_overrideGoup.EnableWindow(TRUE);
    m_techPrefCheck.EnableWindow(TRUE);
    OnTechPrefCheck();
    m_3gppProfileIdCheck.EnableWindow(TRUE);
    OnProfileIdCheck();
    m_primaryDnsCheck.EnableWindow(TRUE);
    OnPrimaryDnsCheck();
    m_secondaryDnsCheck.EnableWindow(TRUE);
    OnSecondaryDnsCheck();
    m_primaryNbnsCheck.EnableWindow(TRUE);
    OnPrimaryNbnsCheck();
    m_secondaryNbnsCheck.EnableWindow(TRUE);
    OnSecondaryNbnsCheck();
    m_apnNameCheck.EnableWindow(TRUE);
    OnApnNameCheck();
    m_ipAddressCheck.EnableWindow(TRUE);
    OnIpAddressCheck();
    m_authPrefCheck.EnableWindow(TRUE);
    OnAuthPrefCheck();
    m_usernameCheck.EnableWindow(TRUE);
    OnUsernameCheck();
    m_passwordCheck.EnableWindow(TRUE);
    OnPasswordCheck();
    m_connectButton.SetWindowText(_T("Connect"));
#else
	//m_pCMDlg->m_pTabDialog->m_btn4.SetWindowText(_T("Connect"));

//	CSetupDlg* pSetupWnd = (CSetupDlg*)(m_pCMDlg->m_pSetupDlg);

// 	pInternetWnd->PostMessage(WM_UPDATE_CONNECT_STATE, NDIS_STATE_DISCONNECT, NULL);
	m_pCMDlg->m_pConnectDlg->PostMessage(WM_UPDATE_CONNECT_STATE, NDIS_STATE_DISCONNECT, NULL);
#endif
}
