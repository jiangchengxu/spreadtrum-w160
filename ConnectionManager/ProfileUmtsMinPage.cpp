// --------------------------------------------------------------------------
//
// ProfileUmtsMinPage.cpp
//
/// ProfileUmtsMinPage implementation.
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

#include "ProfileUmtsMinPage.h"
#include "Resource.h"




BEGIN_MESSAGE_MAP(ProfileUmtsMinPage, CPropertyPage)
END_MESSAGE_MAP()


// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ProfileUmtsMinPage::ProfileUmtsMinPage(WDSGetDefaultSettingsRspRCP rspRCP) :
    ProfileSheetPage(IDD_PROFILE_UMTS_MIN_PAGE),
    m_defaultSettingsRspRCP(rspRCP),
    m_profileSettingsRspRCP(NULL),
    m_initTrafficClass(0),
    m_initMaxUplinkBitrate(0),
    m_initMaxDownlinkBitrate(0),
    m_initGtdUplinkBitrate(0),
    m_initGtdDownlinkBitrate(0),
    m_initQosDeliveryOrder(0),
    m_initMaxSduSize(0),
    m_initSduErrorRatio(0),
    m_initResidualBerRatio(0),
    m_initErroneousSdu(0),
    m_initTransferDelay(0),
    m_initHandlingPriority(0)
{}

// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ProfileUmtsMinPage::ProfileUmtsMinPage(WDSGetProfileSettingsRspRCP rspRCP) :
    ProfileSheetPage(IDD_PROFILE_UMTS_MIN_PAGE),
    m_defaultSettingsRspRCP(NULL),
    m_profileSettingsRspRCP(rspRCP),
    m_initTrafficClass(0),
    m_initMaxUplinkBitrate(0),
    m_initMaxDownlinkBitrate(0),
    m_initGtdUplinkBitrate(0),
    m_initGtdDownlinkBitrate(0),
    m_initQosDeliveryOrder(0),
    m_initMaxSduSize(0),
    m_initSduErrorRatio(0),
    m_initResidualBerRatio(0),
    m_initErroneousSdu(0),
    m_initTransferDelay(0),
    m_initHandlingPriority(0)
{}

// --------------------------------------------------------------------------
// DoDataExchange
// --------------------------------------------------------------------------
void ProfileUmtsMinPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROFILE_MIN_TRAFFIC_CLASS_COMBO, m_trafficClassCombo);
    DDX_Control(pDX, IDC_PROFILE_MIN_QOS_DELIVERY_ORDER_COMBO, m_qosDeliveryOrderCombo);
    DDX_Control(pDX, IDC_PROFILE_MIN_MAX_UPLINK_BITRATE_EDIT, m_maxUplinkBitrateEdit);
    DDX_Control(pDX, IDC_PROFILE_MIN_MAX_DOWNLINK_BITRATE_EDIT, m_maxDownlinkBitrateEdit);
    DDX_Control(pDX, IDC_PROFILE_MIN_GTD_UPLINK_BITRATE_EDIT, m_gtdUplinkBitrateEdit);
    DDX_Control(pDX, IDC_PROFILE_MIN_GTD_DOWNLINK_BITRATE_EDIT, m_gtdDownlinkBitrateEdit);
    DDX_Control(pDX, IDC_PROFILE_MIN_MAX_SDU_SIZE_EDIT, m_maxSduSizeEdit);
    DDX_Control(pDX, IDC_PROFILE_MIN_SDU_ERROR_RATIO_COMBO, m_sduErrorRatioCombo);
    DDX_Control(pDX, IDC_PROFILE_MIN_RESIDUAL_BER_RATIO_COMBO, m_residualBerRatioCombo);
    DDX_Control(pDX, IDC_PROFILE_MIN_ERRONEOUS_SDU_COMBO, m_erroneousSduCombo);
    DDX_Control(pDX, IDC_PROFILE_MIN_TRANSFER_DELAY_EDIT, m_transferDelayEdit);
    DDX_Control(pDX, IDC_PROFILE_MIN_HANDLING_PRIORITY_EDIT, m_handlingPriorityEdit);
}


// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL ProfileUmtsMinPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // set traffic class combo box options
    m_trafficClassCombo.AddString(_T("Subscribed"));
    m_trafficClassCombo.AddString(_T("Conversational"));
    m_trafficClassCombo.AddString(_T("Streaming"));
    m_trafficClassCombo.AddString(_T("Interactive"));
    m_trafficClassCombo.AddString(_T("Background"));

    // set qos delivery order combo box options
    m_qosDeliveryOrderCombo.AddString(_T("Subscribed"));
    m_qosDeliveryOrderCombo.AddString(_T("Delivery Order On"));
    m_qosDeliveryOrderCombo.AddString(_T("Delivery Order Off"));

    // set sdu error ratio combo box options
    m_sduErrorRatioCombo.AddString(_T("Subscribed"));
    m_sduErrorRatioCombo.AddString(_T("1x10-2"));
    m_sduErrorRatioCombo.AddString(_T("7x10-3"));
    m_sduErrorRatioCombo.AddString(_T("1x10-3"));
    m_sduErrorRatioCombo.AddString(_T("1x10-4"));
    m_sduErrorRatioCombo.AddString(_T("1x10-5"));
    m_sduErrorRatioCombo.AddString(_T("1x10-6"));
    m_sduErrorRatioCombo.AddString(_T("1x10-1"));

    // set residual ber ratio combo box options
    m_residualBerRatioCombo.AddString(_T("Subscribed"));
    m_residualBerRatioCombo.AddString(_T("5x10-2"));
    m_residualBerRatioCombo.AddString(_T("1x10-2"));
    m_residualBerRatioCombo.AddString(_T("5x10-3"));
    m_residualBerRatioCombo.AddString(_T("4x10-3"));
    m_residualBerRatioCombo.AddString(_T("1x10-3"));
    m_residualBerRatioCombo.AddString(_T("1x10-4"));
    m_residualBerRatioCombo.AddString(_T("1x10-5"));
    m_residualBerRatioCombo.AddString(_T("1x10-6"));
    m_residualBerRatioCombo.AddString(_T("6x10-8"));

    // set delivery erroneous sdu combo box options
    m_erroneousSduCombo.AddString(_T("Subscribed"));
    m_erroneousSduCombo.AddString(_T("No Detection"));
    m_erroneousSduCombo.AddString(_T("Delivered"));
    m_erroneousSduCombo.AddString(_T("Not Delivered"));

    // initialize member data
    if (!m_defaultSettingsRspRCP.IsNull()) { DefaultInit(); }
    if (!m_profileSettingsRspRCP.IsNull()) { ProfileInit(); }

    // set initial values for the controls on this page
    std::stringstream stream;

    m_trafficClassCombo.SetCurSel(m_initTrafficClass);

   USES_CONVERSION;

	stream << m_initMaxUplinkBitrate;
    m_maxUplinkBitrateEdit.SetWindowText(A2W(stream.str().c_str()));

    stream.str("");
    stream << m_initMaxDownlinkBitrate;
    m_maxDownlinkBitrateEdit.SetWindowText(A2W(stream.str().c_str()));

    stream.str("");
    stream << m_initGtdUplinkBitrate;
    m_gtdUplinkBitrateEdit.SetWindowText(A2W(stream.str().c_str()));

    stream.str("");
    stream << m_initGtdDownlinkBitrate;
    m_gtdDownlinkBitrateEdit.SetWindowText(A2W(stream.str().c_str()));

    m_qosDeliveryOrderCombo.SetCurSel(m_initQosDeliveryOrder);

    stream.str("");
    stream << m_initMaxSduSize;
    m_maxSduSizeEdit.SetWindowText(A2W(stream.str().c_str()));

    m_sduErrorRatioCombo.SetCurSel(m_initSduErrorRatio);
    m_residualBerRatioCombo.SetCurSel(m_initResidualBerRatio);
    m_erroneousSduCombo.SetCurSel(m_initErroneousSdu);

    stream.str("");
    stream << m_initTransferDelay;
    m_transferDelayEdit.SetWindowText(A2W(stream.str().c_str()));

    stream.str("");
    stream << m_initHandlingPriority;
    m_handlingPriorityEdit.SetWindowText(A2W(stream.str().c_str()));


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// --------------------------------------------------------------------------
// DefaultInit
//
/// Initialize the values for this page using the default settings rsp rcp.
// --------------------------------------------------------------------------
void ProfileUmtsMinPage::DefaultInit()
{
    if (m_defaultSettingsRspRCP->IsUmtsMinimumQos())
    {
        m_initTrafficClass = m_defaultSettingsRspRCP->GetMinTrafficClass();
        m_initMaxUplinkBitrate = m_defaultSettingsRspRCP->GetMinMaxUplinkBitrate();
        m_initMaxDownlinkBitrate = m_defaultSettingsRspRCP->GetMinMaxDownlinkBitrate();
        m_initGtdUplinkBitrate = m_defaultSettingsRspRCP->GetMinGtdUplinkBitrate();
        m_initGtdDownlinkBitrate = m_defaultSettingsRspRCP->GetMinGtdDownlinkBitrate();
        m_initQosDeliveryOrder = m_defaultSettingsRspRCP->GetMinQosDeliveryOrder();
        m_initMaxSduSize = m_defaultSettingsRspRCP->GetMinMaxSduSize();
        m_initSduErrorRatio = m_defaultSettingsRspRCP->GetMinSduErrorRatio();
        m_initResidualBerRatio = m_defaultSettingsRspRCP->GetMinResidualBerRatio();
        m_initErroneousSdu = m_defaultSettingsRspRCP->GetMinDeliveryErroneousSdu();
        m_initTransferDelay = m_defaultSettingsRspRCP->GetMinTransferDelay();
        m_initHandlingPriority = m_defaultSettingsRspRCP->GetMinTrafficHandlingPriority();
    }
}

// --------------------------------------------------------------------------
// ProfileInit
//
/// Initialize the values for this page using the profile settings rsp rcp.
// --------------------------------------------------------------------------
void ProfileUmtsMinPage::ProfileInit()
{
    if (m_profileSettingsRspRCP->IsUmtsMinimumQos())
    {
        m_initTrafficClass = m_profileSettingsRspRCP->GetMinTrafficClass();
        m_initMaxUplinkBitrate = m_profileSettingsRspRCP->GetMinMaxUplinkBitrate();
        m_initMaxDownlinkBitrate = m_profileSettingsRspRCP->GetMinMaxDownlinkBitrate();
        m_initGtdUplinkBitrate = m_profileSettingsRspRCP->GetMinGtdUplinkBitrate();
        m_initGtdDownlinkBitrate = m_profileSettingsRspRCP->GetMinGtdDownlinkBitrate();
        m_initQosDeliveryOrder = m_profileSettingsRspRCP->GetMinQosDeliveryOrder();
        m_initMaxSduSize = m_profileSettingsRspRCP->GetMinMaxSduSize();
        m_initSduErrorRatio = m_profileSettingsRspRCP->GetMinSduErrorRatio();
        m_initResidualBerRatio = m_profileSettingsRspRCP->GetMinResidualBerRatio();
        m_initErroneousSdu = m_profileSettingsRspRCP->GetMinDeliveryErroneousSdu();
        m_initTransferDelay = m_profileSettingsRspRCP->GetMinTransferDelay();
        m_initHandlingPriority = m_profileSettingsRspRCP->GetMinTrafficHandlingPriority();
    }
}

// --------------------------------------------------------------------------
// BuildTlvString
//
/// Build a string of the message tlv's represented on this page.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ProfileUmtsMinPage::BuildTlvString()
{
    std::stringstream stream;


    // umts minimum qos tlv
    int trafficClass = m_trafficClassCombo.GetCurSel();
	USES_CONVERSION;

    CString maxUplinkBitrate;
    m_maxUplinkBitrateEdit.GetWindowText(maxUplinkBitrate);
    if (maxUplinkBitrate.Compare(_T("")) == 0) { maxUplinkBitrate = _T("0"); }

    CString maxDownlinkBitrate;
    m_maxDownlinkBitrateEdit.GetWindowText(maxDownlinkBitrate);
    if (maxDownlinkBitrate.Compare(_T("")) == 0) { maxDownlinkBitrate = _T("0"); }

    CString gtdUplinkBitrate;
    m_gtdUplinkBitrateEdit.GetWindowText(gtdUplinkBitrate);
    if (gtdUplinkBitrate.Compare(_T("")) == 0) { gtdUplinkBitrate = _T("0"); }

    CString gtdDownlinkBitrate;
    m_gtdDownlinkBitrateEdit.GetWindowText(gtdDownlinkBitrate);
    if (gtdDownlinkBitrate.Compare(_T("")) == 0) { gtdDownlinkBitrate = _T("0"); }

    int qosDeliveryOrder = m_qosDeliveryOrderCombo.GetCurSel();

    CString maxSduSize;
    m_maxSduSizeEdit.GetWindowText(maxSduSize);
    if (maxSduSize.Compare(_T("")) == 0) { maxSduSize = _T("0"); }

    int sduErrorRatio = m_sduErrorRatioCombo.GetCurSel();
    int residualBerRatio = m_residualBerRatioCombo.GetCurSel();
    int erroneousSdu = m_erroneousSduCombo.GetCurSel();

    CString transferDelay;
    m_transferDelayEdit.GetWindowText(transferDelay);
    if (transferDelay.Compare(_T("")) == 0) { transferDelay = _T("0"); }

    CString handlingPriority;
    m_handlingPriorityEdit.GetWindowText(handlingPriority);
    if (handlingPriority.Compare(_T("")) == 0) { handlingPriority = _T("0"); }

    // add tlv to stream if any value has changed
    if (trafficClass != m_initTrafficClass ||
        _wtoi(maxUplinkBitrate) != m_initMaxUplinkBitrate ||
        _wtoi(maxDownlinkBitrate) != m_initMaxDownlinkBitrate ||
        _wtoi(gtdUplinkBitrate) != m_initGtdUplinkBitrate ||
        _wtoi(gtdDownlinkBitrate) != m_initGtdDownlinkBitrate ||
        qosDeliveryOrder != m_initQosDeliveryOrder ||
        _wtoi(maxSduSize) != m_initMaxSduSize ||
        sduErrorRatio != m_initSduErrorRatio ||
        residualBerRatio != m_initResidualBerRatio ||
        erroneousSdu != m_initErroneousSdu ||
        _wtoi(transferDelay) != m_initTransferDelay ||
        _wtoi(handlingPriority) != m_initHandlingPriority)
    {
        stream << "  MinTrafficClass " << trafficClass << std::endl
               << "  MinMaxUplinkBitrate " << maxUplinkBitrate << std::endl
               << "  MinMaxDownlinkBitrate " << maxDownlinkBitrate << std::endl
               << "  MinGtdUplinkBitrate "<< gtdUplinkBitrate << std::endl
               << "  MinGtdDownlinkBitrate " << gtdDownlinkBitrate << std::endl
               << "  MinQosDeliveryOrder " << qosDeliveryOrder << std::endl
               << "  MinMaxSduSize " << maxSduSize << std::endl
               << "  MinSduErrorRatio " << sduErrorRatio << std::endl
               << "  MinResidualBerRatio " << residualBerRatio << std::endl
               << "  MinDeliveryErroneousSdu " << erroneousSdu << std::endl
               << "  MinTransferDelay " << transferDelay << std::endl
               << "  MinTrafficHandlingPriority " << handlingPriority << std::endl;
    }

    return stream.str();
}
