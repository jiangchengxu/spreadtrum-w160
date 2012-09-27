// --------------------------------------------------------------------------
//
// WDSGetProfileListMsg.cpp
//
/// WDSGetProfileListReq/WDSGetProfileListRsp implementation.
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
#include <assert.h>
#include <sstream>
#include "WDSGetProfileListMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSGetProfileListReq> ReqStringCreator("QMI_WDS_GET_PROFILE_LIST_REQ");
static MessageCreator<WDSGetProfileListReq> ReqUint32Creator(WDSGetProfileListReqUID);

static MessageCreator<WDSGetProfileListRsp> RspStringCreator("QMI_WDS_GET_PROFILE_LIST_RSP");
static MessageCreator<WDSGetProfileListRsp> RspUint32Creator(WDSGetProfileListRspUID);


// --------------------------------------------------------------------------
// WDSGetProfileListReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSGetProfileListReq.
// --------------------------------------------------------------------------
WDSGetProfileListReq::WDSGetProfileListReq() :
    Message(QMUX_TYPE_WDS,QMI_WDS_GET_PROFILE_LIST_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSGetProfileListReq.
// --------------------------------------------------------------------------
WDSGetProfileListReq::~WDSGetProfileListReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool WDSGetProfileListReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

    assert(m_length == m_pMsgBuf->GetIndex() - QMI_SDU_HEADER_BYTES);
    return true;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void WDSGetProfileListReq::Print(std::ostream& stream)
{
    stream << "QMI_WDS_GET_PROFILE_LIST_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSGetProfileListRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSGetProfileListRsp.
// --------------------------------------------------------------------------
const uint8 WDSGetProfileListRsp::RESULT_CODE_TYPE = 0x02;
const uint8 WDSGetProfileListRsp::PROFILE_LIST_TYPE = 0x01;
WDSGetProfileListRsp::WDSGetProfileListRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_GET_PROFILE_LIST_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_profileListType(TLV_TYPE_INVALID),
    m_profileListLen(0),
    m_numInstances(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSGetProfileListRsp.
// --------------------------------------------------------------------------
WDSGetProfileListRsp::~WDSGetProfileListRsp()
{
    std::for_each(
        m_profileListInstances.begin(),
        m_profileListInstances.end(),
        DeleteObjectFunctor());
}

// --------------------------------------------------------------------------
// Unpack
//
/// Unpack a MsgBuf into this Message.
///
/// @param msgBuf - the MsgBuf to unpack.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileListRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // validate message length, mandatory tlvs
    if (m_result == QMI_RESULT_SUCCESS) {
        // mandatory tlvs, profile list type required but length is variable
        if (m_profileListType != PROFILE_LIST_TYPE) {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                   << _T("Mandatory TLV, Profile List, is not present.")
                   << std::endl << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }
    } else {
        // only result code tlv on failure
        if (m_length != 7) {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                   << _T("Expected message length is 7 bytes, unpacked length is ")
                   << m_length << _T(" bytes.") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }
    }

    return true;
}

// --------------------------------------------------------------------------
// GetUnpackerMap
//
/// Return a reference to the uint8 to unpacker map for this Message.
///
/// @returns Uint*UnpackerMap& - reference to the uint8 to unpacker map for
///     this Message type.
// --------------------------------------------------------------------------
Message::Uint8UnpackerMap& WDSGetProfileListRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(PROFILE_LIST_TYPE,(Unpacker)UnpackProfileList)).second;
        assert(bSuccess);
    }
    return UUMap;
}

// --------------------------------------------------------------------------
// UnpackResultCode
//
/// Unpack the result code tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileListRsp::UnpackResultCode(MsgBuf& msgBuf)
{
    m_resultCodeType = RESULT_CODE_TYPE;

    m_resultCodeLen = msgBuf.GetWord();
    if (m_resultCodeLen != 4) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Result Code length is 4 bytes, unpacked length is ")
               << m_resultCodeLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_result = msgBuf.GetWord();
    m_error = msgBuf.GetWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackProfileList
//
/// Unpack the profile list tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileListRsp::UnpackProfileList(MsgBuf& msgBuf)
{
    m_profileListType = PROFILE_LIST_TYPE;
    m_profileListLen = msgBuf.GetWord();

    m_numInstances = msgBuf.GetByte();

    // loop to unpack profile list instances
    for (int i = 0; i < m_numInstances; i++) {
        uint8 profileType = msgBuf.GetByte();
        uint8 profileIndex = msgBuf.GetByte();
        uint8 profileNameLen = msgBuf.GetByte();
        std::string profileName;
        profileName.resize(profileNameLen);
        msgBuf.GetCopy(&profileName[0],profileNameLen);

        m_profileListInstances.push_back(new ProfileListInstance(
                                             profileType,profileIndex,profileNameLen,profileName));
    }

    // all tlvs are mandatory, so we should be at end of buffer
    if (!msgBuf.EOB()) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Finished unpacking message but end of buffer not reached")
               << std::endl << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    return true;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void WDSGetProfileListRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_GET_PROFILE_LIST_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    if (m_profileListType == PROFILE_LIST_TYPE) {
        // loop to print profile instances
        for (int i = 0; i < m_numInstances; i++) {
            stream << "  ProfileType " << (uint32)m_profileListInstances[i]->GetProfileType() << std::endl
                   << "  ProfileIndex " << (uint32)m_profileListInstances[i]->GetProfileIndex() << std::endl
                   << "  ProfileNameLen " << (uint32)m_profileListInstances[i]->GetProfileNameLen() << std::endl
                   << "  ProfileName " << m_profileListInstances[i]->GetProfileName() << std::endl;
        }
    }

    stream << _T("}") << std::endl << std::endl;
}
