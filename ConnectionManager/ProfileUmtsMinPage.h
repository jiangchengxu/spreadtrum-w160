// --------------------------------------------------------------------------
//
// ProfileUmtsMinPage.h
//
/// ProfileUmtsMinPage interface.
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
// ProfileUmtsMinPage
//
/// The ProfileUmtsMinPage class represents the controls under the umts min
/// qos tab of the profile sheet. Input from the user and output to the user 
/// under the general tab is controlled by this class. 
// --------------------------------------------------------------------------

class ProfileUmtsMinPage : public ProfileSheetPage
{
public:
    ProfileUmtsMinPage(WDSGetDefaultSettingsRspRCP rspRCP);
    ProfileUmtsMinPage(WDSGetProfileSettingsRspRCP rspRCP);

    virtual std::string BuildTlvString();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
    virtual void DefaultInit();
    virtual void ProfileInit();
    
	DECLARE_MESSAGE_MAP()

private:
    WDSGetDefaultSettingsRspRCP m_defaultSettingsRspRCP;
    WDSGetProfileSettingsRspRCP m_profileSettingsRspRCP;

    uint8 m_initTrafficClass;
    uint32 m_initMaxUplinkBitrate;
    uint32 m_initMaxDownlinkBitrate;
    uint32 m_initGtdUplinkBitrate;
    uint32 m_initGtdDownlinkBitrate;
    uint8 m_initQosDeliveryOrder;
    uint32 m_initMaxSduSize;
    uint8 m_initSduErrorRatio;
    uint8 m_initResidualBerRatio;
    uint8 m_initErroneousSdu;
    uint32 m_initTransferDelay;
    uint32 m_initHandlingPriority;

    CComboBox m_trafficClassCombo;
    CComboBox m_qosDeliveryOrderCombo;
    CEdit m_maxUplinkBitrateEdit;
    CEdit m_maxDownlinkBitrateEdit;
    CEdit m_gtdUplinkBitrateEdit;
    CEdit m_gtdDownlinkBitrateEdit;
    CEdit m_maxSduSizeEdit;
    CComboBox m_sduErrorRatioCombo;
    CComboBox m_residualBerRatioCombo;
    CComboBox m_erroneousSduCombo;
    CEdit m_transferDelayEdit;
    CEdit m_handlingPriorityEdit;
};