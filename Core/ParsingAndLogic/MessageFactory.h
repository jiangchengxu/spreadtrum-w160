// --------------------------------------------------------------------------
//
// MessageFactory.h
//
/// MessageFactory interface.
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

#include <map>
#include "MsgBuf.h"
#include "StringUtils.h"
#include "CommonDefs.h"
#include "Message.h"

//class Message;
class IMessageCreator;


// --------------------------------------------------------------------------
// MessageFactory
//
/// MessageFactory is a globally accessable singleton. The MessageFactory has
/// two CreateMessage methods that are responsible for determining what
/// message needs to be built, calls the appropriate Create method and 
/// return a pointer to the new Message.
// --------------------------------------------------------------------------
class MessageFactory
{
public:
    typedef std::map<std::string,IMessageCreator*> StringCreatorMap;
    typedef StringCreatorMap::value_type SCPair;

    typedef std::map<uint32,IMessageCreator*> Uint32CreatorMap;
    typedef Uint32CreatorMap::value_type UCPair;

public:
    static MessageFactory& GetInstance(); 
    ~MessageFactory() {}

    Message* CreateMessage(std::string& msgStr);
    Message* CreateMessage(MsgBuf& msgBuf);

    void RegStringCreator(std::string msgType,IMessageCreator* pMsgCreator);
    void UnregStringCreator(std::string msgType,IMessageCreator* pMsgCreator);

    void RegUint32Creator(uint32 msgUID,IMessageCreator* pMsgCreator);
    void UnregUint32Creator(uint32 msgUID,IMessageCreator* pMsgCreator);

private:
    MessageFactory() {}

private:
    StringCreatorMap m_SCMap;
    Uint32CreatorMap m_UCMap;
};


// --------------------------------------------------------------------------
// IMessageCreator    
//
/// A class derived from IMessageCreator knows how to create a specific type
/// of log Message object.  The derived class may be registered with the
/// MessageFactory class to allow instantiation based on message type.
// --------------------------------------------------------------------------
class IMessageCreator
{
public:
    virtual Message* Create(std::string& nameValue) = 0;
    virtual Message* Create(MsgBuf& msgBuf) = 0;
};


// --------------------------------------------------------------------------
// MessageCreator    
//
/// A standard template for implementing the IMessageCreator interface. The
/// template parameter T is the class of Message to be created by the 
/// MessageCreator. The constructor automatically registers the creator with
/// the factory. The create methods are invoked to instantiate a specialized
/// Message object.
// --------------------------------------------------------------------------
template<typename T>
class MessageCreator : public IMessageCreator
{
public:
    // Self-registering Constructor
    MessageCreator(std::string msgType) : m_msgType(msgType),m_msgUID(0)
    {
        MessageFactory::GetInstance().RegStringCreator(m_msgType, this);
    }

    // Self-registering Constructor
    MessageCreator(uint32 msgUID) : m_msgUID(msgUID)
    {
        MessageFactory::GetInstance().RegUint32Creator(m_msgUID, this);
    }
       
    virtual ~MessageCreator() 
    {
        if (m_msgUID == 0)
        {
            MessageFactory::GetInstance().UnregStringCreator(m_msgType, this);
        }
        else
        {
            MessageFactory::GetInstance().UnregUint32Creator(m_msgUID, this);
        }
    }
          
    virtual Message* Create(std::string& nameValue)
    {
		T *dummy = NULL;
        return Message::Create(nameValue, dummy);
    }

    virtual Message* Create(MsgBuf& msgBuf)
    {
		T *dummy = NULL;
        return Message::Create(msgBuf, dummy);
    }

private:
    std::string m_msgType;
    uint32 m_msgUID;
};
