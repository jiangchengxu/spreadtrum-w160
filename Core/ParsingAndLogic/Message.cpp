// --------------------------------------------------------------------------
//
// Message.cpp
//
/// Message implementation.
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
#include "Message.h"
#include "MessageManager.h"
#include "MsgBuf.h"
#include "tchar.h"

// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
Message::Message(uint8 svcType,uint16 msgType,uint8 ctlType) :
    m_svcType(svcType),
    m_msgType(msgType),
    m_ctlType(ctlType),
    m_txId(0),
    m_length(0),
    m_pMsgBuf(0)
{}

// --------------------------------------------------------------------------
// dtor
// --------------------------------------------------------------------------
Message::~Message()
{
    if (m_pMsgBuf != NULL) {
        delete m_pMsgBuf;
    }
}

// --------------------------------------------------------------------------
// Build
//
/// Parse a string into this Message.
///
/// @param nameValue - string of name value pairs.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool Message::Build(std::string& nameValue)
{
    // get a transaction id for this message
    m_txId = QCWWAN_GenerateTransactionId();

    // check for empty name value string (no attributes)
    if (nameValue.empty()) {
        if (!m_nextName.empty()) {
            // report expected attribute not found
            std::stringstream stream;
            stream << _T("Warning: unable to build message:") << std::endl
                   << _T("Expected attribute '") << m_nextName << _T("'.")
                   << std::endl << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }
        // build an internal MsgBuf representation of this message
        if (!BuildMsgBuf()) {
            return false;
        }
        return true;
    }

    // put the name value string in a stream
    std::stringstream attribStream(nameValue);
    std::string value;

    // intialize message with name value pairs
    while (!attribStream.eof()) {
        // get the attribute name
        attribStream >> m_name;

        // check for name/value pair mismatch
        if (attribStream.eof()) {
            // report name/value pair mismatch
            std::stringstream stream;
            stream << _T("Warning: unable to build message:") << std::endl
                   << _T("The message string attribute, ") << m_name
                   << _T(", has no associated value.") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }

        // check for attribute sequence error
        if (!m_nextName.empty() && m_nextName.compare(m_name) != 0) {
            // report expected attribute not found
            std::stringstream stream;
            stream << _T("Warning: unable to build message:") << std::endl
                   << _T("Expected attribute '") << m_nextName;

            if (!m_prevName.empty()) {
                stream << _T("' to follow attribute '") << m_prevName;
            }

            stream << _T("'.") << std::endl
                   << _T("Attribute found was '") << m_name
                   << _T("'.") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }

        // get the attribute value
        attribStream >> value;

        // find the attribute name in the builder map and call its build method
        StringBuilderMap SBMap = GetBuilderMap();
        StringBuilderMap::iterator iter = SBMap.find(m_name);
        if(iter == SBMap.end()) {
            // report unknown attribute
            std::stringstream stream;
            stream << _T("Warning: unable to build message:") << std::endl
                   << _T("Unknown attribute '") << m_name
                   << _T("' found.") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        } else {
            // call attribute's build method, check for failure
            if(!(this->*iter->second)(value)) {
                return false;
            }
        }
    }

    // check for exit when expecting an attribute
    if (!m_nextName.empty()) {
        // report end of string when attribute expected
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Expected attribute '") << m_nextName
               << _T("' to follow attribute '") << m_prevName
               << _T("'.") << std::endl
               << _T("Reached end of message string.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // build an internal MsgBuf representation of this message
    if (!BuildMsgBuf()) {
        return false;
    }

    return true;
}

// --------------------------------------------------------------------------
// GetBuilderMap
//
/// Default implementation returns an empty string to builder map.
///
/// @returns StringBuilderMap& - empty string to builder map.
// --------------------------------------------------------------------------
Message::StringBuilderMap& Message::GetBuilderMap()
{
    static StringBuilderMap SBMap;
    return SBMap;
}

// --------------------------------------------------------------------------
// Unpack
//
/// Unpack a MsgBuf into a Message.
///
/// @param msgBuf - the MsgBuf to unpack.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool Message::Unpack(MsgBuf& msgBuf)
{
    msgBuf.Rewind();

    // ctl type, captured during construction
    msgBuf.GetByte();

    // get the transaction id
    m_txId = msgBuf.GetWord();

    // msg type, captured during construction
    msgBuf.GetWord();

    // get the message length
    m_length = msgBuf.GetWord();

    // unpack tlv's while data exists in buffer
    while (!msgBuf.EOB()) {
        //get the tlv type
        uint8 tlvType = msgBuf.GetByte();

        // find the tlv type in the unpacker map and call its unpack method
        Uint8UnpackerMap& UUMap = GetUnpackerMap();
        Uint8UnpackerMap::iterator iter = UUMap.find(tlvType);
        if(iter == UUMap.end()) {
            // report unknown tlv type
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                   << _T("Unknown tlv type '")
                   << std::showbase << std::hex << (int)tlvType
                   << _T("' found.") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        } else {
            // call tlv type's unpack method, check for failure
            if(!(this->*iter->second)(msgBuf)) {
                return false;
            }
        }
    }

    // set the internal MsgBuf representation of this message
    m_pMsgBuf = new MsgBuf(msgBuf);

    return true;
}

// --------------------------------------------------------------------------
// GetUnpackerMap
//
/// Default implementation returns an empty string to unpacker map.
///
/// @returns Uint8UnpackerMap& - empty string to builder map.
// --------------------------------------------------------------------------
Message::Uint8UnpackerMap& Message::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    return UUMap;
}

// --------------------------------------------------------------------------
// PrintMsgBuf
//
/// Print the MsgBuf of this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void Message::PrintMsgBuf(std::ostream& stream)
{
    m_pMsgBuf->Print(stream);
}

// --------------------------------------------------------------------------
// ExtractIpv4Addr
//
/// Extract an IPv4 Address from a dotted quad string representation.
///
/// @param value - string representation of the IP address.
///
/// @param addr - uint32 in which to store the extracted address.
///
/// @returns bool - true if extract successful.
// --------------------------------------------------------------------------
bool Message::ExtractIpv4Addr(std::string& value,uint32& addr)
{
    uint32 num[4] = { 256,256,256,256 };

    sscanf(value.c_str(),"%u.%u.%u.%u",&num[0],&num[1],&num[2],&num[3]);

    for (int i = 0; i < 4; i++) {
        // validate dotted quad
        if (num[i] > 255) {
            // report invalid IPv4 address
            std::stringstream stream;
            stream << _T("Warning: unable to extract IPv4 address:") << std::endl
                   << _T("Addresses should be formatted as 'w.x.y.z', ")
                   << _T("where the values of w, x, y and z are 0 to 255 inclusive.") << std::endl
                   << _T("The string recieved was '") << value << _T("'.") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }

        // pack the individual values into a uint32
        addr = addr << 8;
        addr = addr | num[i];
    }
    return true;
}

// --------------------------------------------------------------------------
// PrintIPv4Addr
//
/// Print an IPv4 address as a dotted quad.
///
/// @param addr - the uint32 to be printed as a dotted quad.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void Message::PrintIPv4Addr(uint32 addr,std::ostream& stream)
{
    uint8* num = (uint8*)&addr;

    // the uint32 is stored little endian, so pull out in reverse order
    stream << (uint32)num[3] << _T(".")
           << (uint32)num[2] << _T(".")
           << (uint32)num[1] << _T(".")
           << (uint32)num[0];
}

// --------------------------------------------------------------------------
// ReportInvalidSequence
//
/// When building a message, if the current attribute (m_name) is expected to
/// follow an attribute (expectedPrev) but instead follows another attribute
/// (m_prev), report an invalid attribute sequence.
///
/// @param expectedPrev - the previous attribute that was expected.
// --------------------------------------------------------------------------
void Message::ReportInvalidSequence(std::string expectedPrev)
{
    // report invalid attribute sequence
    std::stringstream stream;
    stream << _T("Warning: unable to build message:") << std::endl
           << _T("Expected attribute '") << m_name
           << _T("' to follow attribute '" << expectedPrev << "'.") << std::endl
           << _T("Previous attribute was '") << m_prevName
           << _T("'.") << std::endl
           << std::endl;
    MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
}