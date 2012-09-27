// --------------------------------------------------------------------------
//
// MsgBuf.h
//
/// MsgBuf interface.
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

#include "CommonDefs.h"

// --------------------------------------------------------------------------
// MsgBuf class declaration
//
/// The MsgBuf class is a wrapper for a void* array. It provides services for
/// bounds checked access to data.
// --------------------------------------------------------------------------
class MsgBuf
{
public:
    MsgBuf() : m_buf(0), m_size(0), m_index(0),m_svcType(0xFF) {};
    MsgBuf(void* src,uint32 size);
    MsgBuf(uint32 size);
    MsgBuf(MsgBuf& msgBuf);
    ~MsgBuf();

    uint8 GetByte();
    uint16 GetWord();
    uint32 GetDWord();
    void GetCopy(void* dest, uint32 count);
    uint8 GetByteAt(uint32 index);
    uint16 GetWordAt(uint32 index);

    void PutByte(uint8 value);
    void PutWord(uint16 value);
    void PutDWord(uint32 value);
    void PutCopy(const void* src, uint32 count);

    uint32 GetSize() {
        return m_size;
    }
    uint32 GetIndex() {
        return m_index;
    }
    const uint8* GetBuffer() {
        return m_buf;
    }
    void Rewind() {
        m_index = 0;
    }
    bool EOB() {
        return m_index == m_size;
    }
    void Print(std::ostream& stream);

    void SetSvcType(uint8 svcType) {
        m_svcType = svcType;
    }
    uint8 GetSvcType() {
        return m_svcType;
    }

    MsgBuf& operator=(MsgBuf& msgBuf);
    bool operator==(MsgBuf& msgBuf);
    bool operator!=(MsgBuf& msgBuf) {
        return !operator==(msgBuf);
    }

private:
    uint8* m_buf;
    uint32 m_size;
    uint32 m_index;
    uint8 m_svcType;
};
