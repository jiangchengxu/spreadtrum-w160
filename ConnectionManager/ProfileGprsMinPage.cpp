// --------------------------------------------------------------------------
//
// ProfileGprsMinPage.cpp
//
/// ProfileGprsMinPage implementation.
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

#include "ProfileGprsMinPage.h"
#include "Resource.h"


BEGIN_MESSAGE_MAP(ProfileGprsMinPage, CPropertyPage)
END_MESSAGE_MAP()


// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ProfileGprsMinPage::ProfileGprsMinPage(WDSGetDefaultSettingsRspRCP rspRCP) :
    ProfileSheetPage(IDD_PROFILE_GPRS_MIN_PAGE),
    m_defaultSettingsRspRCP(rspRCP),
    m_profileSettingsRspRCP(NULL),
    m_initPrecedenceClass(0),
    m_initDelayClass(0),
    m_initReliabilityClass(0),
    m_initPeakThruClass(0),
    m_initMeanThruClass(0)
{}

// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ProfileGprsMinPage::ProfileGprsMinPage(WDSGetProfileSettingsRspRCP rspRCP) :
    ProfileSheetPage(IDD_PROFILE_GPRS_MIN_PAGE),
    m_defaultSettingsRspRCP(NULL),
    m_profileSettingsRspRCP(rspRCP),
    m_initPrecedenceClass(0),
    m_initDelayClass(0),
    m_initReliabilityClass(0),
    m_initPeakThruClass(0),
    m_initMeanThruClass(0)
{}

// --------------------------------------------------------------------------
// DoDataExchange
// --------------------------------------------------------------------------
void ProfileGprsMinPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROFILE_MIN_PRECEDENCE_CLASS_COMBO, m_precedenceClassCombo);
    DDX_Control(pDX, IDC_PROFILE_MIN_DELAY_CLASS_COMBO, m_delayClassCombo);
    DDX_Control(pDX, IDC_PROFILE_MIN_RELIABILITY_CLASS_COMBO, m_reliabilityClassCombo);
    DDX_Control(pDX, IDC_PROFILE_MIN_PEAK_THRU_CLASS_COMBO, m_peakThruClassCombo);
    DDX_Control(pDX, IDC_PROFILE_MIN_MEAN_THRU_CLASS_COMBO, m_meanThruClassCombo);
}


// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL ProfileGprsMinPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // set precedence class combo box options
    m_precedenceClassCombo.AddString(_T("Subscribed"));
    m_precedenceClassCombo.AddString(_T("High"));
    m_precedenceClassCombo.AddString(_T("Normal"));
    m_precedenceClassCombo.AddString(_T("Low"));
    m_precedenceClassCombo.AddString(_T("Reserved"));

    // set delay class combo box options
    m_delayClassCombo.AddString(_T("Subscribed"));
    m_delayClassCombo.AddString(_T("1"));
    m_delayClassCombo.AddString(_T("2"));
    m_delayClassCombo.AddString(_T("3"));
    m_delayClassCombo.AddString(_T("4 (Best Effort)"));
    m_delayClassCombo.AddString(_T("Reserved"));

    // set reliability class combo box options
    m_reliabilityClassCombo.AddString(_T("Subscribed"));
    m_reliabilityClassCombo.AddString(_T("Unused"));
    m_reliabilityClassCombo.AddString(_T("Unack GTP, Ack LLC/RLC, Pro"));
    m_reliabilityClassCombo.AddString(_T("Unack GTP/LLC, Ack RLC, Pro"));
    m_reliabilityClassCombo.AddString(_T("Unack GTP/LLC/RLC, Pro"));
    m_reliabilityClassCombo.AddString(_T("Unack GTP/LLC/RLC, Unpro"));
    m_reliabilityClassCombo.AddString(_T("Reserved"));

    // set peak throughput class combo box options
    m_peakThruClassCombo.AddString(_T("Subscribed"));
    m_peakThruClassCombo.AddString(_T("1,000 octet/s"));
    m_peakThruClassCombo.AddString(_T("2,000 octet/s"));
    m_peakThruClassCombo.AddString(_T("4,000 octet/s"));
    m_peakThruClassCombo.AddString(_T("8,000 octet/s"));
    m_peakThruClassCombo.AddString(_T("16,000 octet/s"));
    m_peakThruClassCombo.AddString(_T("32,000 octet/s"));
    m_peakThruClassCombo.AddString(_T("64,000 octet/s"));
    m_peakThruClassCombo.AddString(_T("128,000 octet/s"));
    m_peakThruClassCombo.AddString(_T("256,000 octet/s"));
    m_peakThruClassCombo.AddString(_T("Reserved"));

    // set mean throughput class combo box options
    m_meanThruClassCombo.AddString(_T("Subscribed"));
    m_meanThruClassCombo.AddString(_T("100 octet/h"));
    m_meanThruClassCombo.AddString(_T("200 octet/h"));
    m_meanThruClassCombo.AddString(_T("500 octet/h"));
    m_meanThruClassCombo.AddString(_T("1,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("2,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("5,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("10,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("20,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("50,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("100,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("200,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("500,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("1,000,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("2,000,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("5,000,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("10,000,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("20,000,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("50,000,000 octet/h"));
    m_meanThruClassCombo.AddString(_T("Reserved"));
    m_meanThruClassCombo.AddString(_T("Best Effort"));

    // initialize member data
    if (!m_defaultSettingsRspRCP.IsNull()) { DefaultInit(); }
    if (!m_profileSettingsRspRCP.IsNull()) { ProfileInit(); }

    // set initial values for the controls on this page
    m_precedenceClassCombo.SetCurSel(MapPrecedenceVI(m_initPrecedenceClass));
    m_delayClassCombo.SetCurSel(MapDelayVI(m_initDelayClass));
    m_reliabilityClassCombo.SetCurSel(MapReliabilityVI(m_initReliabilityClass));
    m_peakThruClassCombo.SetCurSel(MapPeakVI(m_initPeakThruClass));
    m_meanThruClassCombo.SetCurSel(MapMeanVI(m_initMeanThruClass));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// --------------------------------------------------------------------------
// DefaultInit
//
/// Initialize the values for this page using the default settings rsp rcp.
// --------------------------------------------------------------------------
void ProfileGprsMinPage::DefaultInit()
{
    if (m_defaultSettingsRspRCP->IsGprsMinimumQos())
    {
        m_initPrecedenceClass = m_defaultSettingsRspRCP->GetMinPrecedenceClass();
        m_initDelayClass = m_defaultSettingsRspRCP->GetMinDelayClass();
        m_initReliabilityClass = m_defaultSettingsRspRCP->GetMinReliabilityClass();
        m_initPeakThruClass = m_defaultSettingsRspRCP->GetMinPeakThroughputClass();
        m_initMeanThruClass = m_defaultSettingsRspRCP->GetMinMeanThroughputClass();
    }
}

// --------------------------------------------------------------------------
// ProfileInit
//
/// Initialize the values for this page using the profile settings rsp rcp.
// --------------------------------------------------------------------------
void ProfileGprsMinPage::ProfileInit()
{
    if (m_profileSettingsRspRCP->IsGprsMinimumQos())
    {
        m_initPrecedenceClass = m_profileSettingsRspRCP->GetMinPrecedenceClass();
        m_initDelayClass = m_profileSettingsRspRCP->GetMinDelayClass();
        m_initReliabilityClass = m_profileSettingsRspRCP->GetMinReliabilityClass();
        m_initPeakThruClass = m_profileSettingsRspRCP->GetMinPeakThroughputClass();
        m_initMeanThruClass = m_profileSettingsRspRCP->GetMinMeanThroughputClass();
    }
}

// --------------------------------------------------------------------------
// BuildTlvString
//
/// Build a string of the message tlv's represented on this page.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ProfileGprsMinPage::BuildTlvString()
{
    std::stringstream stream;

    // gprs minimum qos tlv
    int precedenceClass = MapPrecedenceIV(m_precedenceClassCombo.GetCurSel());
    int delayClass = MapDelayIV(m_delayClassCombo.GetCurSel());
    int reliabilityClass = MapReliabilityIV(m_reliabilityClassCombo.GetCurSel());
    int peakThruClass = MapPeakIV(m_peakThruClassCombo.GetCurSel());
    int meanThruClass = MapMeanIV(m_meanThruClassCombo.GetCurSel());

    // add tlv to stream if any value has changed
    if (precedenceClass != m_initPrecedenceClass ||
        delayClass != m_initDelayClass ||
        reliabilityClass != m_initReliabilityClass ||
        peakThruClass != m_initPeakThruClass ||
        meanThruClass != m_initMeanThruClass)
    {
        stream << _T("  MinPrecedenceClass ") << precedenceClass << std::endl
               << _T("  MinDelayClass ") << delayClass << std::endl
               << _T("  MinReliabilityClass ") << reliabilityClass << std::endl
               << _T("  MinPeakThroughputClass ") << peakThruClass << std::endl
               << _T("  MinMeanThroughputClass ") << meanThruClass << std::endl;
    }

    return stream.str();
}

// --------------------------------------------------------------------------
// MapPrecedenceIV
//
/// Map the precedence class combo box index to its value.
///
/// @param index - index of combo box entry
///
/// @returns int  - value of combo box entry at index
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapPrecedenceIV(int index)
{
    if (index == 4) { return 7; }
    return index;
}

// --------------------------------------------------------------------------
// MapPrecedenceVI
//
/// Map the precedence class combo box value to its index.
///
/// @param value - value of combo box entry
///
/// @returns int  - index of combo box entry for value
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapPrecedenceVI(int value)
{
    if (value == 7) { return 4; }
    return value;
}

// --------------------------------------------------------------------------
// MapDelayIV
//
/// Map the delay class combo box index to its value.
///
/// @param index - index of combo box entry
///
/// @returns int  - value of combo box entry at index
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapDelayIV(int index)
{
    if (index == 5) { return 7; }
    return index;
}

// --------------------------------------------------------------------------
// MapDelayVI
//
/// Map the delay class combo box value to its index.
///
/// @param value - value of combo box entry
///
/// @returns int  - index of combo box entry for value
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapDelayVI(int value)
{
    if (value == 7) { return 5; }
    return value;
}

// --------------------------------------------------------------------------
// MapReliabilityIV
//
/// Map the reliability class combo box index to its value.
///
/// @param index - index of combo box entry
///
/// @returns int  - value of combo box entry at index
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapReliabilityIV(int index)
{
    if (index == 6) { return 7; }
    return index;
}

// --------------------------------------------------------------------------
// MapReliabilityVI
//
/// Map the reliability class combo box value to its index.
///
/// @param value - value of combo box entry
///
/// @returns int  - index of combo box entry for value
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapReliabilityVI(int value)
{
    if (value == 7) { return 6; }
    return value;
}

// --------------------------------------------------------------------------
// MapPeakIV
//
/// Map the peak throughput class combo box index to its value.
///
/// @param index - index of combo box entry
///
/// @returns int  - value of combo box entry at index
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapPeakIV(int index)
{
    if (index == 10) { return 15; }
    return index;
}

// --------------------------------------------------------------------------
// MapPeakVI
//
/// Map the peak throughput class combo box value to its index.
///
/// @param value - value of combo box entry
///
/// @returns int  - index of combo box entry for value
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapPeakVI(int value)
{
    if (value == 15) { return 10; }
    return value;
}

// --------------------------------------------------------------------------
// MapMeanIV
//
/// Map the mean throughput class combo box index to its value.
///
/// @param index - index of combo box entry
///
/// @returns int  - value of combo box entry at index
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapMeanIV(int index)
{
    if (index == 19) { return 30; }
    if (index == 20) { return 31; }
    return index;
}

// --------------------------------------------------------------------------
// MapMeanVI
//
/// Map the mean throughput class combo box value to its index.
///
/// @param value - value of combo box entry
///
/// @returns int  - index of combo box entry for value
// --------------------------------------------------------------------------
int ProfileGprsMinPage::MapMeanVI(int value)
{
    if (value == 30) { return 19; }
    if (value == 31) { return 20; }
    return value;
}
