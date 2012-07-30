// --------------------------------------------------------------------------
//
// WDSSetEventReportMsg.h
//
/// WDSSetEventReportReq/WDSSetEventReportRsp/WDSEventReportInd interface.
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

#include "Message.h"

// --------------------------------------------------------------------------
// Unique identifiers for WDSResetMsg types
// --------------------------------------------------------------------------
static const uint32 WDSSetEventReportReqUID = 
    (QMUX_TYPE_WDS << 24 | 
     QMI_CTL_FLAG_TYPE_CMD << 16 | 
     QMI_WDS_SET_EVENT_REPORT_MSG);
static const uint32 WDSSetEventReportRspUID = 
    (QMUX_TYPE_WDS << 24 | 
     QMI_CTL_FLAG_TYPE_RSP << 16 | 
     QMI_WDS_SET_EVENT_REPORT_MSG);
static const uint32 WDSEventReportIndUID = 
    (QMUX_TYPE_WDS << 24 | 
     QMI_CTL_FLAG_TYPE_IND << 16 | 
     QMI_WDS_SET_EVENT_REPORT_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSSetEventReportReq;
class WDSSetEventReportRsp;
class WDSEventReportInd;
typedef TRCPointer<WDSSetEventReportReq> WDSSetEventReportReqRCP;
typedef TRCPointer<WDSSetEventReportRsp> WDSSetEventReportRspRCP;
typedef TRCPointer<WDSEventReportInd> WDSEventReportIndRCP;

// --------------------------------------------------------------------------
// WDSSetEventReportReq
//
/// This class represents a QMI_WDS_SET_EVENT_REPORT_REQ message which
/// sets conditions for reporting change in data call status parameters via
/// the QMI_WDS_EVENT_REPORT_IND.
// --------------------------------------------------------------------------

class WDSSetEventReportReq : public Message
{
    // define tlv types
    static const uint8 CURRENT_CHANNEL_RATE_INDICATOR_TYPE;
    static const uint8 TRANSFER_STATISTICS_INDICATOR_TYPE;
    static const uint8 CURRENT_DATA_BEARER_TECH_INDICATOR_TYPE;

public:
    ~WDSSetEventReportReq();

    virtual void Print(std::ostream& stream);

protected:
    WDSSetEventReportReq();
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildReportChannelRate(std::string& value);
    virtual bool BuildStatsPeriod(std::string& value);
    virtual bool BuildStatsMask(std::string& value);
    virtual bool BuildReportDataBearerTech(std::string& value);
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) { return false; }

private:
    // Current Channel Rate Indicator, optional tlv 0x10
    uint8 m_currentChannelRateIndicatorType;
    uint16 m_currentChannelRateIndicatorLen;
    uint8 m_reportChannelRate;

    // Transfer Statistics Indicator, optional tlv 0x11
    uint8 m_transferStatisticsIndicatorType;
    uint16 m_transferStatisticsIndicatorLen;
    uint8 m_statsPeriod;
    uint32 m_statsMask;

    // Current Data Bearer Technology Indicator, optional tlv 0x11
    uint8 m_currentDataBearerTechIndicatorType;
    uint16 m_currentDataBearerTechIndicatorLen;
    uint8 m_reportDataBearerTech;
};


// --------------------------------------------------------------------------
// WDSSetEventReportRsp
//
/// This class represents a QMI_WDS_SET_EVENT_REPORT_RESP message 
/// which is the response to the set event report request.
// --------------------------------------------------------------------------

class WDSSetEventReportRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;

public:
    ~WDSSetEventReportRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() { return m_result; }
    uint16 GetError() { return m_error; }

protected:
    WDSSetEventReportRsp();
    virtual bool Build(std::string& nameValue) { return false; }
    virtual bool BuildMsgBuf() { return false; }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;
};

// --------------------------------------------------------------------------
// WDSEventReportInd
//
/// This class represents a QMI_WDS_EVENT_REPORT_IND message.
/// Indication is sent when any of the conditions for reporting set in the 
/// set event report request become true.
// --------------------------------------------------------------------------

class WDSEventReportInd : public Message
{
    // define tlv types
    static const uint8 TX_PACKETS_OK_TYPE;
    static const uint8 RX_PACKETS_OK_TYPE;
    static const uint8 TX_PACKET_ERRORS_TYPE;
    static const uint8 RX_PACKET_ERRORS_TYPE;
    static const uint8 TX_OVERFOLWS_TYPE;
    static const uint8 RX_OVERFOLWS_TYPE;
    static const uint8 CHANNEL_RATE_TYPE;
    static const uint8 DATA_BEARER_TECH_TYPE;

public:
    ~WDSEventReportInd();

    virtual void Print(std::ostream& stream);

    virtual bool IsTxOkCount() { return m_txPacketsOkType == TX_PACKETS_OK_TYPE; }
    virtual bool IsRxOkCount() { return m_rxPacketsOkType == RX_PACKETS_OK_TYPE; }
    virtual bool IsTxErrCount() { return m_txPacketErrorsType == TX_PACKET_ERRORS_TYPE; }
    virtual bool IsRxErrCount() { return m_rxPacketErrorsType == RX_PACKET_ERRORS_TYPE; }
    virtual bool IsTxOflCount() { return m_txOverflowsType == TX_OVERFOLWS_TYPE; }
    virtual bool IsRxOflCount() { return m_rxOverflowsType == RX_OVERFOLWS_TYPE; }
    virtual bool IsCurrentChannelRate() { return m_channelRateType == CHANNEL_RATE_TYPE; }
    virtual bool IsDataBearerTech() { return m_dataBearerTechType == DATA_BEARER_TECH_TYPE; }

    virtual uint32 GetTxOkCount() { return m_txOkCount; }
    virtual uint32 GetRxOkCount() { return m_rxOkCount; }
    virtual uint32 GetTxErrCount() { return m_txErrCount; }
    virtual uint32 GetRxErrCount() { return m_rxErrCount; }
    virtual uint32 GetTxOflCount() { return m_txOflCount; }
    virtual uint32 GetRxOflCount() { return m_rxOflCount; }
    virtual uint32 GetCurrentChannelTxRate() { return m_currentChannelTxRate; }
    virtual uint32 GetCurrentChannelRxRate() { return m_currentChannelRxRate; }
    virtual uint8 GetDataBearerTech() { return m_dataBearerTech; }

protected:
    WDSEventReportInd();
    virtual bool Build(std::string& nameValue) { return false; }
    virtual bool BuildMsgBuf() { return false; }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackTxPacketsOk(MsgBuf& msgBuf);
    virtual bool UnpackRxPacketsOk(MsgBuf& msgBuf);
    virtual bool UnpackTxPacketErrors(MsgBuf& msgBuf);
    virtual bool UnpackRxPacketErrors(MsgBuf& msgBuf);
    virtual bool UnpackTxOverflows(MsgBuf& msgBuf);
    virtual bool UnpackRxOverflows(MsgBuf& msgBuf);
    virtual bool UnpackChannelRate(MsgBuf& msgBuf);
    virtual bool UnpackDataBearerTech(MsgBuf& msgBuf);

private:
    // TX Packets OK, optional tlv 0x10
    uint8 m_txPacketsOkType;
    uint16 m_txPacketsOkLen;
    uint32 m_txOkCount;

    // RX Packets OK, optional tlv 0x11
    uint8 m_rxPacketsOkType;
    uint16 m_rxPacketsOkLen;
    uint32 m_rxOkCount;

    // TX Packet Errors, optional tlv 0x12
    uint8 m_txPacketErrorsType;
    uint16 m_txPacketErrorsLen;
    uint32 m_txErrCount;

    // RX Packet Errors, optional tlv 0x13
    uint8 m_rxPacketErrorsType;
    uint16 m_rxPacketErrorsLen;
    uint32 m_rxErrCount;

    // TX Overflows, optional tlv 0x14
    uint8 m_txOverflowsType;
    uint16 m_txOverflowsLen;
    uint32 m_txOflCount;

    // RX Overflows, optional tlv 0x15
    uint8 m_rxOverflowsType;
    uint16 m_rxOverflowsLen;
    uint32 m_rxOflCount;

    // Channel Rate, optional tlv 0x16
    uint8 m_channelRateType;
    uint16 m_channelRateLen;
    uint32 m_currentChannelTxRate;
    uint32 m_currentChannelRxRate;

    // Data Bearer Technology, optional tlv 0x17
    uint8 m_dataBearerTechType;
    uint16 m_dataBearerTechLen;
    uint8 m_dataBearerTech;
};
