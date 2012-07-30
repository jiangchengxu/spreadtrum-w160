// --------------------------------------------------------------------------
//
// MessageFactory.cpp
//
/// MessageFactory implementation.
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
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"

// --------------------------------------------------------------------------
// GetInstance
//
/// Get the one and only MessageFactory.
///
/// @returns MessageFactory& - reference to the MessageFactory singleton.
// --------------------------------------------------------------------------
MessageFactory& MessageFactory::GetInstance()
{
   static MessageFactory msgFactory;
   return msgFactory;
}

// --------------------------------------------------------------------------
// CreateMessage
//
/// Given a string representation of a Message, determine the specific 
/// message type to construct and call that type's Create method.
///  
/// @param msgStr - the message in string format.
///
/// @returns Message* - pointer to the Message created if successful, NULL 
///     otherwise.
// --------------------------------------------------------------------------
Message* MessageFactory::CreateMessage(std::string& msgStr)
{
    // find parens separating message type and body
    size_t lParenIdx = msgStr.find_first_of('{');
    size_t rParenIdx = msgStr.find_first_of('}');

    // verify basic format
    if (lParenIdx == std::string::npos || rParenIdx == std::string::npos)
    {
        // missing curly braces
        std::stringstream stream;
        stream << _T("Warning: unable to create message from string:") << std::endl << std::endl 
               << msgStr << std::endl
               << _T("Braces ('{}') around the message attributes are missing.") << std::endl
               << _T("Message string disregarded.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return NULL;
        return NULL;
    }

    // get messge type and body, remove whitespace 
    std::string msgType = msgStr.substr(0,lParenIdx);
    Trim(msgType);
    std::string nameValue = msgStr.substr(lParenIdx + 1,rParenIdx - (lParenIdx + 1));
    Trim(nameValue);

    // find the message type in the creator map and call the creator method
    StringCreatorMap::iterator iter = m_SCMap.find(msgType);
    if(iter == m_SCMap.end())
    {
        // msgType not found
        std::stringstream stream;
        stream << _T("Warning: unable to create message from string:") << std::endl << std::endl 
               << msgStr << std::endl << std::endl
               << _T("The message type, ") << msgType 
               << _T(", cannot be matched to a known message.") << std::endl
               << _T("Message string disregarded.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return NULL;
    }
    else
    {
        Message* pMsg = iter->second->Create(nameValue);
        if (pMsg == NULL)
        {
            // attribute parsing failed
            std::stringstream stream;
            stream << _T("Unable to create message from string:") << std::endl << std::endl 
                   << msgStr << std::endl
                   << _T("Message string disregarded.") << std::endl 
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        }
        return pMsg;
    }
}

// --------------------------------------------------------------------------
// CreateMessage
//
/// Given a MsgBuf representation of a Message, determine the specific 
/// message type to construct and call that type's Create method.
///  
/// @param msgBuf - the message in MsgBuf format.
///
/// @returns Message* - pointer to the Message created if successful, NULL 
///     otherwise.
// --------------------------------------------------------------------------
Message* MessageFactory::CreateMessage(MsgBuf& msgBuf)
{
    // pull svcType information off the buffer
    msgBuf.Rewind();
    uint8 svcType = msgBuf.GetSvcType();
    uint8 ctlType = msgBuf.GetByte();
    uint16 txId = msgBuf.GetWord();
    uint16 msgType = msgBuf.GetWord();

    // compose UID
    uint32 msgUID = (svcType << 24 | ctlType << 16 | msgType);

    // find the message type in the creator map and call the creator method
    Uint32CreatorMap::iterator iter = m_UCMap.find(msgUID);
    if(iter == m_UCMap.end())
    {
        // msgType not found 
        std::stringstream stream;
        stream << _T("Warning: unable to create message from MsgBuf:") << std::endl;
        msgBuf.Print(stream);
        stream << _T("Message buffer cannot be matched to a known message.") << std::endl
               << _T("  Service Type: ") << SERVICE_TYPE_STRINGS[svcType] << std::endl
               << _T("  Control Type: ") << (int)ctlType << std::endl
               << _T("  Transaction Id: ") << (int)txId << std::endl
               << _T("  Message Type: ") << (int)msgType << std::endl
               << _T("Message buffer disregarded.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return NULL;
    }
    else
    {
        Message* pMsg = iter->second->Create(msgBuf);
        if (pMsg == NULL)
        {
            std::stringstream stream;
            stream << _T("Unable to create message from MsgBuf:") << std::endl;
            msgBuf.Print(stream);
            stream << _T("Message buffer disregarded.") << std::endl 
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        }
        return pMsg;
    }
}

// --------------------------------------------------------------------------
// RegStringCreator
//
/// Adds a message creator to the factory's std::string to MessageCreator 
/// map.
///  
/// @param msgType - the message type in string format.
///
/// @param IMessageCreator - pointer to the message creator.
// --------------------------------------------------------------------------
void MessageFactory::RegStringCreator
(
    std::string msgType,
    IMessageCreator* pCreator
)
{
   bool bSuccess = m_SCMap.insert(SCPair(msgType, pCreator)).second;
   assert(bSuccess);
}


// --------------------------------------------------------------------------
// UnregStringCreator
//
/// Removes a message creator to the factory's std::string to MessageCreator 
/// map.
///  
/// @param msgType - the message type in string format.
///
/// @param IMessageCreator - pointer to the message creator.
// --------------------------------------------------------------------------
void MessageFactory::UnregStringCreator
(
    std::string msgType,
    IMessageCreator* pCreator
)
{
   StringCreatorMap::iterator iter = m_SCMap.find(msgType);
   if(iter != m_SCMap.end())
   {
      assert(iter->second == pCreator);
      m_SCMap.erase(iter);
   }
}

// --------------------------------------------------------------------------
// RegUint32Creator
//
/// Adds a message creator to the factory's uint32 to MessageCreator map.
///  
/// @param msgUID - the message type in uint32 format.
///
/// @param IMessageCreator - pointer to the message creator.
// --------------------------------------------------------------------------
void MessageFactory::RegUint32Creator
(
    uint32 msgUID,
    IMessageCreator* pCreator
)
{
   bool bSuccess = m_UCMap.insert(UCPair(msgUID, pCreator)).second;
   assert(bSuccess);
}


// --------------------------------------------------------------------------
// UnregUint32Creator
//
/// Removes a message creator from the factory's uint32 to MessageCreator 
/// map.
///  
/// @param msgUID - the message type in uint32 format.
///
/// @param IMessageCreator - pointer to the message creator.
// --------------------------------------------------------------------------
void MessageFactory::UnregUint32Creator
(
    uint32 msgUID,
    IMessageCreator* pCreator
)
{
   Uint32CreatorMap::iterator iter = m_UCMap.find(msgUID);
   if(iter != m_UCMap.end())
   {
      assert(iter->second == pCreator);
      m_UCMap.erase(iter);
   }
}
