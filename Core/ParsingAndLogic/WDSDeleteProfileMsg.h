// --------------------------------------------------------------------------
//
// WDSDeleteProfileMsg.h
//
/// WDSDeleteProfileReq/WDSDeleteProfileRsp interface.
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
// Unique identifiers for WDSDeleteProfileMsg types
// --------------------------------------------------------------------------
static const uint32 WDSDeleteProfileReqUID =
    (QMUX_TYPE_WDS << 24 | 
     QMI_CTL_FLAG_TYPE_CMD << 16 | 
     QMI_WDS_DELETE_PROFILE_MSG);
static const uint32 WDSDeleteProfileRspUID =
    (QMUX_TYPE_WDS << 24 | 
     QMI_CTL_FLAG_TYPE_RSP << 16 | 
     QMI_WDS_DELETE_PROFILE_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSDeleteProfileReq;
class WDSDeleteProfileRsp;
typedef TRCPointer<WDSDeleteProfileReq> WDSDeleteProfileReqRCP;
typedef TRCPointer<WDSDeleteProfileRsp> WDSDeleteProfileRspRCP;


// --------------------------------------------------------------------------
// WDSDeleteProfileReq
//
/// This class represents a QMI_WDS_DELETE_PROFILE_REQ message which deletes
/// a configured profile.
// --------------------------------------------------------------------------

class WDSDeleteProfileReq : public Message
{
    // define tlv types
    static const uint8 PROFILE_IDENTIFIER_TYPE;

public:
    ~WDSDeleteProfileReq();
    
    virtual void Print(std::ostream& stream);

    virtual uint8 GetProfileType() { return m_profileType; }
    virtual uint8 GetProfileIndex() { return m_profileIndex; }

protected:
    WDSDeleteProfileReq();
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildProfileType(std::string& value);
    virtual bool BuildProfileIndex(std::string& value);

    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) { return false; }

private:
    // Profile Identifier, mandatory tlv 0x01
    uint8 m_profileIdentifierType;
    uint16 m_profileIdentifierLen;
    uint8 m_profileType;
    uint8 m_profileIndex;
};


// --------------------------------------------------------------------------
// WDSDeleteProfileRsp
//
/// This class represents a QMI_WDS_DELETE_PROFILE_RESP message which is the
/// response to the wds delete profile request.
// --------------------------------------------------------------------------

class WDSDeleteProfileRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;

public:
    ~WDSDeleteProfileRsp();

    virtual void Print(std::ostream& stream);

    virtual uint16 GetResult() { return m_result; }
    virtual uint16 GetError() { return m_error; }

protected:
    WDSDeleteProfileRsp();
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
