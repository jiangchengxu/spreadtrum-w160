// --------------------------------------------------------------------------
//
// NASSetEventReportMsg.h
//
/// NASSetEventReportReq/NASSetEventReportRsp/NASEventReportInd interface.
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
#include <vector>


// --------------------------------------------------------------------------
// Unique identifiers for NASSetEventReportMsg types
// --------------------------------------------------------------------------
static const uint32 NASSetEventReportReqUID =
    (QMUX_TYPE_NAS << 24 |
     QMI_CTL_FLAG_TYPE_CMD << 16 |
     QMI_NAS_SET_EVENT_REPORT_MSG);
static const uint32 NASSetEventReportRspUID =
    (QMUX_TYPE_NAS << 24 |
     QMI_CTL_FLAG_TYPE_RSP << 16 |
     QMI_NAS_SET_EVENT_REPORT_MSG);
static const uint32 NASEventReportIndUID =
    (QMUX_TYPE_NAS << 24 |
     QMI_CTL_FLAG_TYPE_IND << 16 |
     QMI_NAS_SET_EVENT_REPORT_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class NASSetEventReportReq;
class NASSetEventReportRsp;
class NASEventReportInd;
typedef TRCPointer<NASSetEventReportReq> NASSetEventReportReqRCP;
typedef TRCPointer<NASSetEventReportRsp> NASSetEventReportRspRCP;
typedef TRCPointer<NASEventReportInd> NASEventReportIndRCP;


// --------------------------------------------------------------------------
// NASSetEventReportReq
//
/// This class represents a QMI_NAS_SET_EVENT_REPORT_REQ message which
/// sets state for reporting conditions for specific parameters. A periodic
/// indication will be sent as per the conditions set in this request.
// --------------------------------------------------------------------------

class NASSetEventReportReq : public Message
{
    // define tlv types
    static const uint8 SIGNAL_STRENGTH_INDICATOR_TYPE ;

public:
    ~NASSetEventReportReq();

    virtual void Print(std::ostream& stream);

protected:
    NASSetEventReportReq();
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildReportSigStrength(std::string& value);
    virtual bool BuildRangeLimit(std::string& value);
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }

private:
    // Signal Strength Indicator, optional tlv 0x10
    uint8 m_signalStrengthIndicatorType;
    uint16 m_signalStrengthIndicatorLen;
    uint8 m_reportSigStrength;
    uint8 m_numRangeLimits;
    std::vector<int8> m_rangeLimits;
};


// --------------------------------------------------------------------------
// NASSetEventReportRsp
//
/// This class represents a QMI_NAS_SET_EVENT_REPORT_RESP message
/// which is the response to the set event report request.
// --------------------------------------------------------------------------

class NASSetEventReportRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;

public:
    ~NASSetEventReportRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() {
        return m_result;
    }
    uint16 GetError() {
        return m_error;
    }

protected:
    NASSetEventReportRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
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
// NASEventReportInd
//
/// This class represents a QMI_NAS_EVENT_REPORT_IND message.
/// Indication is sent when any of the conditions for reporting set in the
/// set event report request become true.
// --------------------------------------------------------------------------

class NASEventReportInd : public Message
{
    // define tlv types
    static const uint8 SIGNAL_STRENGTH_TYPE;

public:
    ~NASEventReportInd();

    virtual void Print(std::ostream& stream);

    int8 GetSignalStrength() {
        return m_signalStrength;
    }
    uint8 GetRadioIf() {
        return m_radioIf;
    }

protected:
    NASEventReportInd();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackSignalStrength(MsgBuf& msgBuf);

private:
    // Signal Strength, optional tlv 0x10
    uint8 m_signalStrengthType;
    uint16 m_signalStrengthLen;
    int8 m_signalStrength;
    uint8 m_radioIf;
};
