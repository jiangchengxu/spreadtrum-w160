// --------------------------------------------------------------------------
//
// DMSSetEventReportMsg.h
//
/// DMSSetEventReportReq/DMSSetEventReportRsp/DMSEventReportInd interface.
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
// Unique identifiers for DMSSetEventReportMsg types
// --------------------------------------------------------------------------
static const uint32 DMSSetEventReportReqUID = 
    (QMUX_TYPE_DMS << 24 | 
     QMI_CTL_FLAG_TYPE_CMD << 16 | 
     QMI_DMS_SET_EVENT_REPORT_MSG);
static const uint32 DMSSetEventReportRspUID = 
    (QMUX_TYPE_DMS << 24 | 
     QMI_CTL_FLAG_TYPE_RSP << 16 | 
     QMI_DMS_SET_EVENT_REPORT_MSG);
static const uint32 DMSEventReportIndUID = 
    (QMUX_TYPE_DMS << 24 | 
     QMI_CTL_FLAG_TYPE_IND << 16 | 
     QMI_DMS_SET_EVENT_REPORT_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class DMSSetEventReportReq;
class DMSSetEventReportRsp;
class DMSEventReportInd;
typedef TRCPointer<DMSSetEventReportReq> DMSSetEventReportReqRCP;
typedef TRCPointer<DMSSetEventReportRsp> DMSSetEventReportRspRCP;
typedef TRCPointer<DMSEventReportInd> DMSEventReportIndRCP;


// --------------------------------------------------------------------------
// DMSSetEventReportReq
//
/// This class represents a QMI_DMS_SET_EVENT_REPORT_REQ message which
/// sets state for reporting conditions for specific parameters. A periodic 
/// indication will be sent as per the conditions set in this request.
// --------------------------------------------------------------------------
class DMSSetEventReportReq : public Message
{
    // define tlv types
    static const uint8 POWER_STATE_REPORTING_MODE_TYPE;
    static const uint8 BATTERY_LVL_LIMITS_TYPE;

public:
    ~DMSSetEventReportReq();

    virtual void Print(std::ostream& stream);

protected:
    DMSSetEventReportReq();
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildPowerStateReportMode(std::string& value);
    virtual bool BuildBatteryLvlLowerLimit(std::string& value);
    virtual bool BuildBatteryLvlUpperLimit(std::string& value);
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) { return false; }

private:
    // Power State Reporting Mode, optional tlv 0x10
    uint8 m_powerStateReportModeType;
    uint16 m_powerStateReportModeLen;
    uint8 m_powerStateReportMode;

    // Battery Level Report Limits, optional tlv 0x11
    uint8 m_batteryLvlLimitsType;
    uint16 m_batteryLvlLimitsLen;
    uint8 m_batteryLvlLowerLimit;
    uint8 m_batteryLvlUpperLimit;
};


// --------------------------------------------------------------------------
// DMSSetEventReportRsp
//
/// This class represents a QMI_DMS_SET_EVENT_REPORT_RESP message 
/// which is the response to the set event report request.
// --------------------------------------------------------------------------
class DMSSetEventReportRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;

public:
    ~DMSSetEventReportRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() { return m_result; }
    uint16 GetError() { return m_error; }

protected:
    DMSSetEventReportRsp();
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
// DMSEventReportInd
//
/// This class represents a QMI_DMS_EVENT_REPORT_IND message.
/// Indication is sent when any of the conditions for reporting set in the 
/// set event report request become true.
// --------------------------------------------------------------------------

class DMSEventReportInd : public Message
{
    // define tlv types
    static const uint8 POWER_STATE_TYPE;

public:
    ~DMSEventReportInd();

    virtual void Print(std::ostream& stream);

    virtual uint8 GetPowerStatus() { return m_powerStatus; }
    virtual uint8 GetBatteryLvl() { return m_batteryLvl; }

protected:
    DMSEventReportInd();
    virtual bool Build(std::string& nameValue) { return false; }
    virtual bool BuildMsgBuf() { return false; }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackPowerState(MsgBuf& msgBuf);

private:
    // Power State, optional tlv 0x10
    uint8 m_powerStateType;
    uint16 m_powerStateLen;
    uint8 m_powerStatus;
    uint8 m_batteryLvl;
};
