// --------------------------------------------------------------------------
//
// WDSGetCurrentChannelRateMsg.h
//
/// WDSGetCurrentChannelRateReq/WDSGetCurrentChannelRateRsp interface.
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
// Unique identifiers for WDSGetCurrentChannelRateMsg types
// --------------------------------------------------------------------------
static const uint32 WDSGetCurrentChannelRateReqUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_CMD << 16 |
     QMI_WDS_GET_CURRENT_CHANNEL_RATE_MSG);
static const uint32 WDSGetCurrentChannelRateRspUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_RSP << 16 |
     QMI_WDS_GET_CURRENT_CHANNEL_RATE_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSGetCurrentChannelRateReq;
class WDSGetCurrentChannelRateRsp;
typedef TRCPointer<WDSGetCurrentChannelRateReq> WDSGetCurrentChannelRateReqRCP;
typedef TRCPointer<WDSGetCurrentChannelRateRsp> WDSGetCurrentChannelRateRspRCP;


// --------------------------------------------------------------------------
// WDSGetCurrentChannelRateReq
//
/// This class represents a QMI_WDS_GET_CURRENT_CHANNEL_RATE_REQ message
/// which is used to obtain the current and maximum possible (for the current
/// serving radio interface) Tx and Rx channel rates. If this request is
/// issued when a network connection is not yet started, only the max channel
/// rates are returned and the current channel rates are set to zero.
// --------------------------------------------------------------------------
class WDSGetCurrentChannelRateReq : public Message
{
public:
    ~WDSGetCurrentChannelRateReq();

    virtual void Print(std::ostream& stream);

protected:
    WDSGetCurrentChannelRateReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }
};


// --------------------------------------------------------------------------
// WDSGetCurrentChannelRateRsp
//
/// This class represents a QMI_WDS_GET_CURRENT_CHANNEL_RATE_RESP message
/// which is the response to the wds get current channel rate request.
// --------------------------------------------------------------------------

class WDSGetCurrentChannelRateRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 CHANNEL_RATE_TYPE;

public:
    ~WDSGetCurrentChannelRateRsp();

    virtual void Print(std::ostream& stream);

    virtual uint16 GetResult() {
        return m_result;
    }
    virtual uint16 GetError() {
        return m_error;
    }
    virtual uint32 GetCurrentChannelTxRate() {
        return m_currentChannelTxRate;
    }
    virtual uint32 GetCurrentChannelRxRate() {
        return m_currentChannelRxRate;
    }
    virtual uint32 GetMaxChannelTxRate() {
        return m_maxChannelTxRate;
    }
    virtual uint32 GetMaxChannelRxRate() {
        return m_maxChannelRxRate;
    }


protected:
    WDSGetCurrentChannelRateRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackChannelRate(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Channel rate, mandatory tly 0x01
    uint8 m_channelRateType;
    uint16 m_channelRateLen;
    uint32 m_currentChannelTxRate;
    uint32 m_currentChannelRxRate;
    uint32 m_maxChannelTxRate;
    uint32 m_maxChannelRxRate;
};
