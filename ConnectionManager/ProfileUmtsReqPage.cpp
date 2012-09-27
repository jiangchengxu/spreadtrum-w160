// --------------------------------------------------------------------------
//
// ProfileUmtsReqPage.cpp
//
/// ProfileUmtsReqPage implementation.
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


#ifndef _USECON_H
#define _USECON_H
#include <atlconv.h>
#endif

#include "ProfileUmtsReqPage.h"
#include "Resource.h"




BEGIN_MESSAGE_MAP(ProfileUmtsReqPage, CPropertyPage)
END_MESSAGE_MAP()


// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ProfileUmtsReqPage::ProfileUmtsReqPage(WDSGetDefaultSettingsRspRCP rspRCP) :
    ProfileSheetPage(IDD_PROFILE_UMTS_REQ_PAGE),
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
ProfileUmtsReqPage::ProfileUmtsReqPage(WDSGetProfileSettingsRspRCP rspRCP) :
    ProfileSheetPage(IDD_PROFILE_UMTS_REQ_PAGE),
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
void ProfileUmtsReqPage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROFILE_REQ_TRAFFIC_CLASS_COMBO, m_trafficClassCombo);
    DDX_Control(pDX, IDC_PROFILE_REQ_QOS_DELIVERY_ORDER_COMBO, m_qosDeliveryOrderCombo);
    DDX_Control(pDX, IDC_PROFILE_REQ_MAX_UPLINK_BITRATE_EDIT, m_maxUplinkBitrateEdit);
    DDX_Control(pDX, IDC_PROFILE_REQ_MAX_DOWNLINK_BITRATE_EDIT, m_maxDownlinkBitrateEdit);
    DDX_Control(pDX, IDC_PROFILE_REQ_GTD_UPLINK_BITRATE_EDIT, m_gtdUplinkBitrateEdit);
    DDX_Control(pDX, IDC_PROFILE_REQ_GTD_DOWNLINK_BITRATE_EDIT, m_gtdDownlinkBitrateEdit);
    DDX_Control(pDX, IDC_PROFILE_REQ_MAX_SDU_SIZE_EDIT, m_maxSduSizeEdit);
    DDX_Control(pDX, IDC_PROFILE_REQ_SDU_ERROR_RATIO_COMBO, m_sduErrorRatioCombo);
    DDX_Control(pDX, IDC_PROFILE_REQ_RESIDUAL_BER_RATIO_COMBO, m_residualBerRatioCombo);
    DDX_Control(pDX, IDC_PROFILE_REQ_ERRONEOUS_SDU_COMBO, m_erroneousSduCombo);
    DDX_Control(pDX, IDC_PROFILE_REQ_TRANSFER_DELAY_EDIT, m_transferDelayEdit);
    DDX_Control(pDX, IDC_PROFILE_REQ_HANDLING_PRIORITY_EDIT, m_handlingPriorityEdit);
}


// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL ProfileUmtsReqPage::OnInitDialog()
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
    if (!m_defaultSettingsRspRCP.IsNull()) {
        DefaultInit();
    }
    if (!m_profileSettingsRspRCP.IsNull()) {
        ProfileInit();
    }

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
void ProfileUmtsReqPage::DefaultInit()
{
    if (m_defaultSettingsRspRCP->IsUmtsRequestedQos()) {
        m_initTrafficClass = m_defaultSettingsRspRCP->GetReqTrafficClass();
        m_initMaxUplinkBitrate = m_defaultSettingsRspRCP->GetReqMaxUplinkBitrate();
        m_initMaxDownlinkBitrate = m_defaultSettingsRspRCP->GetReqMaxDownlinkBitrate();
        m_initGtdUplinkBitrate = m_defaultSettingsRspRCP->GetReqGtdUplinkBitrate();
        m_initGtdDownlinkBitrate = m_defaultSettingsRspRCP->GetReqGtdDownlinkBitrate();
        m_initQosDeliveryOrder = m_defaultSettingsRspRCP->GetReqQosDeliveryOrder();
        m_initMaxSduSize = m_defaultSettingsRspRCP->GetReqMaxSduSize();
        m_initSduErrorRatio = m_defaultSettingsRspRCP->GetReqSduErrorRatio();
        m_initResidualBerRatio = m_defaultSettingsRspRCP->GetReqResidualBerRatio();
        m_initErroneousSdu = m_defaultSettingsRspRCP->GetReqDeliveryErroneousSdu();
        m_initTransferDelay = m_defaultSettingsRspRCP->GetReqTransferDelay();
        m_initHandlingPriority = m_defaultSettingsRspRCP->GetReqTrafficHandlingPriority();
    }
}

// --------------------------------------------------------------------------
// ProfileInit
//
/// Initialize the values for this page using the profile settings rsp rcp.
// --------------------------------------------------------------------------
void ProfileUmtsReqPage::ProfileInit()
{
    if (m_profileSettingsRspRCP->IsUmtsRequestedQos()) {
        m_initTrafficClass = m_profileSettingsRspRCP->GetReqTrafficClass();
        m_initMaxUplinkBitrate = m_profileSettingsRspRCP->GetReqMaxUplinkBitrate();
        m_initMaxDownlinkBitrate = m_profileSettingsRspRCP->GetReqMaxDownlinkBitrate();
        m_initGtdUplinkBitrate = m_profileSettingsRspRCP->GetReqGtdUplinkBitrate();
        m_initGtdDownlinkBitrate = m_profileSettingsRspRCP->GetReqGtdDownlinkBitrate();
        m_initQosDeliveryOrder = m_profileSettingsRspRCP->GetReqQosDeliveryOrder();
        m_initMaxSduSize = m_profileSettingsRspRCP->GetReqMaxSduSize();
        m_initSduErrorRatio = m_profileSettingsRspRCP->GetReqSduErrorRatio();
        m_initResidualBerRatio = m_profileSettingsRspRCP->GetReqResidualBerRatio();
        m_initErroneousSdu = m_profileSettingsRspRCP->GetReqDeliveryErroneousSdu();
        m_initTransferDelay = m_profileSettingsRspRCP->GetReqTransferDelay();
        m_initHandlingPriority = m_profileSettingsRspRCP->GetReqTrafficHandlingPriority();
    }
}

// --------------------------------------------------------------------------
// BuildTlvString
//
/// Build a string of the message tlv's represented on this page.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ProfileUmtsReqPage::BuildTlvString()
{
    std::stringstream stream;

    // umts requested qos tlv
    int trafficClass = m_trafficClassCombo.GetCurSel();


    CString maxUplinkBitrate;
    m_maxUplinkBitrateEdit.GetWindowText(maxUplinkBitrate);
    if (maxUplinkBitrate.Compare(_T("")) == 0) {
        maxUplinkBitrate = _T("0");
    }

    CString maxDownlinkBitrate;
    m_maxDownlinkBitrateEdit.GetWindowText(maxDownlinkBitrate);
    if (maxDownlinkBitrate.Compare(_T("")) == 0) {
        maxDownlinkBitrate = _T("0");
    }

    CString gtdUplinkBitrate;
    m_gtdUplinkBitrateEdit.GetWindowText(gtdUplinkBitrate);
    if (gtdUplinkBitrate.Compare(_T("")) == 0) {
        gtdUplinkBitrate = _T("0");
    }

    CString gtdDownlinkBitrate;
    m_gtdDownlinkBitrateEdit.GetWindowText(gtdDownlinkBitrate);
    if (gtdDownlinkBitrate.Compare(_T("")) == 0) {
        gtdDownlinkBitrate = _T("0");
    }

    int qosDeliveryOrder = m_qosDeliveryOrderCombo.GetCurSel();

    CString maxSduSize;
    m_maxSduSizeEdit.GetWindowText(maxSduSize);
    if (maxSduSize.Compare(_T("")) == 0) {
        maxSduSize = _T("0");
    }

    int sduErrorRatio = m_sduErrorRatioCombo.GetCurSel();
    int residualBerRatio = m_residualBerRatioCombo.GetCurSel();
    int erroneousSdu = m_erroneousSduCombo.GetCurSel();

    CString transferDelay;
    m_transferDelayEdit.GetWindowText(transferDelay);
    if (transferDelay.Compare(_T("")) == 0) {
        transferDelay = _T("0");
    }

    CString handlingPriority;
    m_handlingPriorityEdit.GetWindowText(handlingPriority);
    if (handlingPriority.Compare(_T("")) == 0) {
        handlingPriority = _T("0");
    }

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
            _wtoi(handlingPriority) != m_initHandlingPriority) {
        stream << "  ReqTrafficClass " << trafficClass << std::endl
               << "  ReqMaxUplinkBitrate " << maxUplinkBitrate << std::endl
               << "  ReqMaxDownlinkBitrate " << maxDownlinkBitrate << std::endl
               << "  ReqGtdUplinkBitrate " << gtdUplinkBitrate << std::endl
               << "  ReqGtdDownlinkBitrate " << gtdDownlinkBitrate << std::endl
               << "  ReqQosDeliveryOrder " << qosDeliveryOrder << std::endl
               << "  ReqMaxSduSize " << maxSduSize << std::endl
               << "  ReqSduErrorRatio " << sduErrorRatio << std::endl
               << "  ReqResidualBerRatio " << residualBerRatio << std::endl
               << "  ReqDeliveryErroneousSdu " << erroneousSdu << std::endl
               << "  ReqTransferDelay " << transferDelay << std::endl
               << "  ReqTrafficHandlingPriority " << handlingPriority << std::endl;
    }

    return stream.str();
}
