// --------------------------------------------------------------------------
//
// WDSResetMsg.h
//
/// WDSResetReq/WDSResetRsp interface.
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
static const uint32 WDSResetReqUID =
    (QMUX_TYPE_WDS << 24 | QMI_CTL_FLAG_TYPE_CMD << 16 | QMI_WDS_RESET_MSG);
static const uint32 WDSResetRspUID =
    (QMUX_TYPE_WDS << 24 | QMI_CTL_FLAG_TYPE_RSP << 16 | QMI_WDS_RESET_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSResetReq;
class WDSResetRsp;
typedef TRCPointer<WDSResetReq> WDSResetReqRCP;
typedef TRCPointer<WDSResetRsp> WDSResetRspRCP;


// --------------------------------------------------------------------------
// WDSResetReq
//
/// This class represents a QMI_WDS_RESET_REQ message which requests that the
/// service reset its state corresponding to the requesting control point.
// --------------------------------------------------------------------------
class WDSResetReq : public Message
{
public:
    ~WDSResetReq();

    virtual void Print(std::ostream& stream);

protected:
    WDSResetReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }
};


// --------------------------------------------------------------------------
// WDSResetRsp
//
/// This class represents a QMI_WDS_RESET_RESP message which is the response
/// to a request that the service reset its state corresponding to the
/// control point.
// --------------------------------------------------------------------------
class WDSResetRsp : public Message
{
    // define tlv types
    const uint8 RESULT_CODE_TYPE;

public:
    ~WDSResetRsp();

    virtual void Print(std::ostream& stream);

protected:
    WDSResetRsp();
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
