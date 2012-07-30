// --------------------------------------------------------------------------
//
// WDSGetProfileListMsg.h
//
/// WDSGetProfileListReq/WDSGetProfileListRsp interface.
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

#include <vector>
#include "Message.h"


// --------------------------------------------------------------------------
// Unique identifiers for WDSGetProfileListMsg types
// --------------------------------------------------------------------------
static const uint32 WDSGetProfileListReqUID = 
    (QMUX_TYPE_WDS << 24 | QMI_CTL_FLAG_TYPE_CMD << 16 | QMI_WDS_GET_PROFILE_LIST_MSG);
static const uint32 WDSGetProfileListRspUID = 
    (QMUX_TYPE_WDS << 24 | QMI_CTL_FLAG_TYPE_RSP << 16 | QMI_WDS_GET_PROFILE_LIST_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSGetProfileListReq;
class WDSGetProfileListRsp;
typedef TRCPointer<WDSGetProfileListReq> WDSGetProfileListReqRCP;
typedef TRCPointer<WDSGetProfileListRsp> WDSGetProfileListRspRCP;

// --------------------------------------------------------------------------
// Define profile list instance class
// --------------------------------------------------------------------------
class ProfileListInstance
{
public:
    ProfileListInstance
    (
        uint8 profileType,
        uint8 profileIndex,
        uint8 profileNameLen,
        std::string profileName
    ) : 
        m_profileType(profileType), 
        m_profileIndex(profileIndex), 
        m_profileNameLen(profileNameLen),
        m_profileName(profileName)
    {}

    ~ProfileListInstance() {}

    uint8 GetProfileType() { return m_profileType; }
    uint8 GetProfileIndex() { return m_profileIndex; }
    uint8 GetProfileNameLen() { return m_profileNameLen; }
    std::string GetProfileName() { return m_profileName; }

private:
    uint8 m_profileType;
    uint8 m_profileIndex;
    uint8 m_profileNameLen;
    std::string m_profileName;
};

// --------------------------------------------------------------------------
// WDSGetProfileListReq
//
/// This class represents a QMI_WDS_GET_PROFILE_LIST_REQ message which 
/// retrieves a list of configured profiles present on the wireless device.
// --------------------------------------------------------------------------
class WDSGetProfileListReq : public Message
{
public:
    ~WDSGetProfileListReq();

    virtual void Print(std::ostream& stream);

protected:
    WDSGetProfileListReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) { return false; }
};


// --------------------------------------------------------------------------
// WDSGetProfileListRsp
//
/// This class represents a QMI_WDS_GET_PROFILE_LIST_RESP message which is the
/// response to a request for a list of configured profiles present on the 
/// wireless device. 
// --------------------------------------------------------------------------

class WDSGetProfileListRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 PROFILE_LIST_TYPE;

public:
    ~WDSGetProfileListRsp();

    virtual void Print(std::ostream& stream);

    virtual uint16 GetResult() { return m_result; }
    virtual uint16 GetError() { return m_error; }

    virtual bool IsProfileList() { return m_profileListType == PROFILE_LIST_TYPE; }

    virtual const uint8 GetNumInstances() { return m_numInstances; }
    virtual const std::vector<ProfileListInstance*> GetProfileListInstances() { return m_profileListInstances; }

protected:
    WDSGetProfileListRsp();
    virtual bool Build(std::string& nameValue) { return false; }
    virtual bool BuildMsgBuf() { return false; }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackProfileList(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Profile List, mandatory tlv 0x01
    uint8 m_profileListType;
    uint16 m_profileListLen;
    uint8 m_numInstances;
    std::vector<ProfileListInstance*> m_profileListInstances;
};
