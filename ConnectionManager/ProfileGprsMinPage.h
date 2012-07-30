// --------------------------------------------------------------------------
//
// ProfileGprsMinPage.h
//
/// ProfileGprsMinPage interface.
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

#include "CommonDefs.h"
#include "ProfileSheetPage.h"
#include "WDSGetProfileSettingsMsg.h"
#include "WDSGetDefaultSettingsMsg.h"



// --------------------------------------------------------------------------
// ProfileGprsMinPage
//
/// The ProfileGprsMinPage class represents the controls under the gprs min
/// qos tab of the profile sheet. Input from the user and output to the user 
/// under the general tab is controlled by this class. 
// --------------------------------------------------------------------------

class ProfileGprsMinPage : public ProfileSheetPage
{
public:
    ProfileGprsMinPage(WDSGetDefaultSettingsRspRCP rspRCP);
    ProfileGprsMinPage(WDSGetProfileSettingsRspRCP rspRCP);

    virtual std::string BuildTlvString();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
    virtual void DefaultInit();
    virtual void ProfileInit();

    virtual int MapPrecedenceIV(int index);
    virtual int MapPrecedenceVI(int value);
    virtual int MapDelayIV(int index);
    virtual int MapDelayVI(int value);
    virtual int MapReliabilityIV(int index);
    virtual int MapReliabilityVI(int value);
    virtual int MapPeakIV(int index);
    virtual int MapPeakVI(int value);
    virtual int MapMeanIV(int index);
    virtual int MapMeanVI(int value);
    
	DECLARE_MESSAGE_MAP()

private:
    WDSGetDefaultSettingsRspRCP m_defaultSettingsRspRCP;
    WDSGetProfileSettingsRspRCP m_profileSettingsRspRCP;

    uint32 m_initPrecedenceClass;
    uint32 m_initDelayClass;
    uint32 m_initReliabilityClass;
    uint32 m_initPeakThruClass;
    uint32 m_initMeanThruClass;

    CComboBox m_precedenceClassCombo;
    CComboBox m_delayClassCombo;
    CComboBox m_reliabilityClassCombo;
    CComboBox m_peakThruClassCombo;
    CComboBox m_meanThruClassCombo;
};