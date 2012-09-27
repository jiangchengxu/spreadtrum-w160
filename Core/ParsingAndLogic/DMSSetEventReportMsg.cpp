// --------------------------------------------------------------------------
//
// DMSSetEventReportMsg.cpp
//
/// DMSSetEventReportReq/DMSSetEventReportRsp/DMSEventReportInd
/// implementation.
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
#include "DMSSetEventReportMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<DMSSetEventReportReq> ReqStringCreator("QMI_DMS_SET_EVENT_REPORT_REQ");
static MessageCreator<DMSSetEventReportReq> ReqUint32Creator(DMSSetEventReportReqUID);

static MessageCreator<DMSSetEventReportRsp> RspStringCreator("QMI_DMS_SET_EVENT_REPORT_RSP");
static MessageCreator<DMSSetEventReportRsp> RspUint32Creator(DMSSetEventReportRspUID);

static MessageCreator<DMSEventReportInd> IndStringCreator("QMI_DMS_EVENT_REPORT_IND");
static MessageCreator<DMSEventReportInd> IndUint32Creator(DMSEventReportIndUID);


// --------------------------------------------------------------------------
// DMSSetEventReportReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSSetEventReportReq.
// --------------------------------------------------------------------------
const uint8 DMSSetEventReportReq::POWER_STATE_REPORTING_MODE_TYPE = 0x10;
const uint8 DMSSetEventReportReq::BATTERY_LVL_LIMITS_TYPE = 0x11;
DMSSetEventReportReq::DMSSetEventReportReq() :
    Message(QMUX_TYPE_DMS,QMI_DMS_SET_EVENT_REPORT_MSG,QMI_CTL_FLAG_TYPE_CMD),
    m_powerStateReportModeType(TLV_TYPE_INVALID),
    m_powerStateReportModeLen(0),
    m_powerStateReportMode(-1),
    m_batteryLvlLimitsType(TLV_TYPE_INVALID),
    m_batteryLvlLimitsLen(0),
    m_batteryLvlLowerLimit(-1),
    m_batteryLvlUpperLimit(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSSetEventReportReq.
// --------------------------------------------------------------------------
DMSSetEventReportReq::~DMSSetEventReportReq()
{}

// --------------------------------------------------------------------------
// Build
//
/// Parse a string into this Message.
///
/// @param nameValue - string of name value pairs.
///
/// @returns bool - true if build successful
// --------------------------------------------------------------------------
bool DMSSetEventReportReq::Build(std::string& nameValue)
{
    // call the base build function
    if (Message::Build(nameValue)) {
        // at least one optional tlv must be present
        if (m_powerStateReportModeType != POWER_STATE_REPORTING_MODE_TYPE &&
                m_batteryLvlLimitsType != BATTERY_LVL_LIMITS_TYPE) {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                   << _T("At least one of the following optional tlv's must be present:") << std::endl
                   << _T("  Power State Reporting Mode") << std::endl
                   << _T("  Battery Level Report Limits") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }
    }
    return true;
}

// --------------------------------------------------------------------------
// GetBuilderMap
//
/// Return a reference to the string to builder map for this Message.
///
/// @returns StringBuilderMap& - reference to the string to builder map for
///     this Message type.
// --------------------------------------------------------------------------
Message::StringBuilderMap& DMSSetEventReportReq::GetBuilderMap()
{
    static StringBuilderMap SBMap;
    if (SBMap.empty()) {
        bool bSuccess = SBMap.insert(SBPair("PowerStateReportMode",(Builder)BuildPowerStateReportMode)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("BatteryLvlLowerLimit",(Builder)BuildBatteryLvlLowerLimit)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("BatteryLvlUpperLimit",(Builder)BuildBatteryLvlUpperLimit)).second;
        assert(bSuccess);
    }
    return SBMap;
}

// --------------------------------------------------------------------------
// BuildPowerStateReportMode
//
/// Build the power state report mode attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool DMSSetEventReportReq::BuildPowerStateReportMode(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num != 0 && num != 1) {
        // report invalid profile type
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Power State Reporting Mode '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid mode values are 0 and 1.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_powerStateReportModeType = POWER_STATE_REPORTING_MODE_TYPE;
    m_powerStateReportModeLen = 1;
    m_powerStateReportMode = (uint8)num;
    m_length += (m_powerStateReportModeLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildBatteryLvlLowerLimit
//
/// Build the battery level lower limit attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool DMSSetEventReportReq::BuildBatteryLvlLowerLimit(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num < 1 || num > 100) {
        // report invalid profile type
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Battery Level Lower Limit '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid limit values are 1 - 100.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_batteryLvlLimitsType = BATTERY_LVL_LIMITS_TYPE;
    m_batteryLvlLimitsLen = 2;
    m_batteryLvlLowerLimit = (uint8)num;
    m_length += (m_batteryLvlLimitsLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "BatteryLvlUpperLimit";
    return true;
}

// --------------------------------------------------------------------------
// BuildBatteryLvlUpperLimit
//
/// Build the battery level upper limit attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool DMSSetEventReportReq::BuildBatteryLvlUpperLimit(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("BatteryLvlLowerLimit") != 0) {
        ReportInvalidSequence("BatteryLvlLowerLimit");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num < 1 || num > 100) {
        // report invalid profile type
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Battery Level Upper Limit '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid limit values are 1 - 100.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    // no type or length, part of m_batteryLvlLimitsType tlv
    m_batteryLvlUpperLimit = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool DMSSetEventReportReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

    // optional tlv, power state reporting mode
    if (m_powerStateReportModeType == POWER_STATE_REPORTING_MODE_TYPE) {
        m_pMsgBuf->PutByte(m_powerStateReportModeType);
        m_pMsgBuf->PutWord(m_powerStateReportModeLen);
        m_pMsgBuf->PutByte(m_powerStateReportMode);
    }

    // optional tlv, battery level report limits
    if (m_batteryLvlLimitsType == BATTERY_LVL_LIMITS_TYPE) {
        m_pMsgBuf->PutByte(m_batteryLvlLimitsType);
        m_pMsgBuf->PutWord(m_batteryLvlLimitsLen);
        m_pMsgBuf->PutByte(m_batteryLvlLowerLimit);
        m_pMsgBuf->PutByte(m_batteryLvlUpperLimit);
    }

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
void DMSSetEventReportReq::Print(std::ostream& stream)
{
    stream << "QMI_DMS_SET_EVENT_REPORT_REQ" << std::endl
           << _T("{") << std::endl;

    if (m_powerStateReportModeType == POWER_STATE_REPORTING_MODE_TYPE) {
        stream << _T("  PowerStateReportMode ") << (int)m_powerStateReportMode << std::endl;
    }

    if (m_batteryLvlLimitsType == BATTERY_LVL_LIMITS_TYPE) {
        stream << _T("  BatteryLvlLowerLimit ") << (int)m_batteryLvlLowerLimit << std::endl
               << _T("  BatteryLvlUpperLimit ") << (int)m_batteryLvlUpperLimit << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// DMSSetEventReportRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSSetEventReportRsp.
// --------------------------------------------------------------------------
const uint8 DMSSetEventReportRsp::RESULT_CODE_TYPE = 0x02;
DMSSetEventReportRsp::DMSSetEventReportRsp() :
    Message(QMUX_TYPE_DMS,QMI_DMS_SET_EVENT_REPORT_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSSetEventReportRsp.
// --------------------------------------------------------------------------
DMSSetEventReportRsp::~DMSSetEventReportRsp()
{}

// --------------------------------------------------------------------------
// Unpack
//
/// Unpack a MsgBuf into this Message.
///
/// @param msgBuf - the MsgBuf to unpack.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool DMSSetEventReportRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // validate message length
    if (m_length != 7) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected message length is 7 bytes, unpacked length is ")
               << m_length << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
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
Message::Uint8UnpackerMap& DMSSetEventReportRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
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
bool DMSSetEventReportRsp::UnpackResultCode(MsgBuf& msgBuf)
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
void DMSSetEventReportRsp::Print(std::ostream& stream)
{
    stream << "QMI_DMS_SET_EVENT_REPORT_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl
           << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// DMSEventReportInd
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSEventReportInd.
// --------------------------------------------------------------------------
const uint8 DMSEventReportInd ::POWER_STATE_TYPE = 0x10;
DMSEventReportInd::DMSEventReportInd() :
    Message(QMUX_TYPE_DMS,QMI_DMS_SET_EVENT_REPORT_MSG,QMI_CTL_FLAG_TYPE_IND),
    m_powerStateType(TLV_TYPE_INVALID),
    m_powerStateLen(0),
    m_powerStatus(-1),
    m_batteryLvl(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSEventReportInd.
// --------------------------------------------------------------------------
DMSEventReportInd::~DMSEventReportInd()
{}

// --------------------------------------------------------------------------
// Unpack
//
/// Unpack a MsgBuf into this Message.
///
/// @param msgBuf - the MsgBuf to unpack.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool DMSEventReportInd::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // at least one optional tlv must be present
    if (m_powerStateType != POWER_STATE_TYPE) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("At least one of the following optional tlv's must be present:") << std::endl
               << _T("  Power State") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // validate message length
    if (m_length != 5) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected message length is 5 bytes, unpacked length is ")
               << m_length << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
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
Message::Uint8UnpackerMap& DMSEventReportInd::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
        bool bSuccess = UUMap.insert(UUPair(POWER_STATE_TYPE,(Unpacker)UnpackPowerState)).second;
        assert(bSuccess);
    }
    return UUMap;
}

// --------------------------------------------------------------------------
// UnpackPowerState
//
/// Unpack the power state tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool DMSEventReportInd::UnpackPowerState(MsgBuf& msgBuf)
{
    m_powerStateType = POWER_STATE_TYPE;

    m_powerStateLen = msgBuf.GetWord();
    if (m_powerStateLen != 2) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Power State length is 2 bytes, unpacked length is ")
               << m_powerStateLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_powerStatus = msgBuf.GetByte();
    if (m_powerStatus > 15) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Unexpected use of 4 most significant bits in Power Status byte.")
               << std::endl << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_batteryLvl = msgBuf.GetByte();
    if (m_batteryLvl > 100) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Battery Level values are 0 - 100 , unpacked value is ")
               << m_batteryLvl << _T(" .") << std::endl
               << std::endl;
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
void DMSEventReportInd::Print(std::ostream& stream)
{
    stream << "QMI_DMS_EVENT_REPORT_IND" << std::endl
           << _T("{") << std::endl;

    if (m_powerStateType != TLV_TYPE_INVALID) {
        stream << _T("  PowerStatus ") << (int)m_powerStatus << std::endl
               << _T("  BatteryLvl ") << (int)m_batteryLvl << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
