// --------------------------------------------------------------------------
//
// WDSAbortMsg.h
//
/// WDSAbortReq/WDSAbortRsp interface.
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
// Unique identifiers for WDSAbortMsg types
// --------------------------------------------------------------------------
static const uint32 WDSAbortReqUID =
    (QMUX_TYPE_WDS << 24 | 
     QMI_CTL_FLAG_TYPE_CMD << 16 | 
     QMI_WDS_ABORT_MSG);
static const uint32 WDSAbortRspUID =
    (QMUX_TYPE_WDS << 24 | 
     QMI_CTL_FLAG_TYPE_RSP << 16 | 
     QMI_WDS_ABORT_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSAbortReq;
class WDSAbortRsp;
typedef TRCPointer<WDSAbortReq> WDSAbortReqRCP;
typedef TRCPointer<WDSAbortRsp> WDSAbortRspRCP;


// --------------------------------------------------------------------------
// WDSAbortReq
//
/// This class represents a QMI_WDS_ABORT_REQ message which aborts a 
/// previously issued QMI_WDS command
// --------------------------------------------------------------------------

class WDSAbortReq : public Message
{
    // define tlv types
    static const uint8 TX_ID_TYPE;

public:
    ~WDSAbortReq();
    
    virtual void Print(std::ostream& stream);

protected:
    WDSAbortReq();
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildTxId(std::string& value);
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) { return false; }

private:
    // TX_ID, mandatory tlv 0x01
    uint8 m_txIdType;
    uint16 m_txIdLen;
    uint16 m_txIdValue;
};


// --------------------------------------------------------------------------
// WDSAbortRsp
//
/// This class represents a QMI_WDS_ABORT_RESP message which is the response
/// to the wds abort request.
// --------------------------------------------------------------------------

class WDSAbortRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;

public:
    ~WDSAbortRsp();

    virtual void Print(std::ostream& stream);

    virtual uint16 GetResult() { return m_result; }
    virtual uint16 GetError() { return m_error; }

protected:
    WDSAbortRsp();
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
