// --------------------------------------------------------------------------
//
// MsgBuf.cpp
//
/// MsgBuf implementation.
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
#include <string>
#include <assert.h>
#include "MsgBuf.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
MsgBuf::MsgBuf(void* src,uint32 size) : m_size(size), m_index(0),m_svcType(0xFF)
{
    m_buf = new uint8[m_size];
    memcpy(&m_buf[m_index],src,m_size);
}

// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
MsgBuf::MsgBuf(uint32 size) : m_size(size), m_index(0),m_svcType(0xFF)
{
    m_buf = new uint8[m_size];
}

// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
MsgBuf::MsgBuf(MsgBuf& msgBuf) : m_index(0)
{
    m_size = msgBuf.GetSize();
    m_svcType = msgBuf.GetSvcType();
    m_buf = new uint8[m_size];
    memcpy(&m_buf[m_index],msgBuf.GetBuffer(),m_size);
}

// --------------------------------------------------------------------------
// dtor
// --------------------------------------------------------------------------
MsgBuf::~MsgBuf()
{
    if (!m_buf == NULL) {
        delete [] m_buf;
    }
}

// --------------------------------------------------------------------------
// GetByte
//
/// Return the byte at the current buffer index location, increment the
/// index by a byte.
///
/// @returns uint8 - byte value at the current buffer index location.
// --------------------------------------------------------------------------
uint8 MsgBuf::GetByte()
{
    assert(m_index < m_size);
    uint8 value = m_buf[m_index];
    m_index++;
    assert(m_index <= m_size);
    return value;
}

// --------------------------------------------------------------------------
// GetWord
//
/// Return the word at the current buffer index location, increment the
/// index by 2 bytes.
///
/// @returns uint16 - word value at the current buffer index location.
// --------------------------------------------------------------------------
uint16 MsgBuf::GetWord()
{
    assert(m_index < m_size);
    uint16 value = *(uint16*)(m_buf + m_index);
    m_index += 2;
    assert(m_index <= m_size);
    return value;
}

// --------------------------------------------------------------------------
// GetDWord
//
/// Return the double word at the current buffer index location, increment the
/// index by 4 bytes.
///
/// @returns uint32 - double word value at the current buffer index location.
// --------------------------------------------------------------------------
uint32 MsgBuf::GetDWord()
{
    assert(m_index < m_size);
    uint32 value = *(uint32*)(m_buf + m_index);
    m_index += 4;
    assert(m_index <= m_size);
    return value;
}

// --------------------------------------------------------------------------
// GetCopy
//
/// Copy a given number of bytes from the buffer to a given destination,
/// increment the index by the given number of bytes.
///
/// @param dest - copy from the buffer to the dest.
///
/// @param count - the number of bytes to copy.
// --------------------------------------------------------------------------
void MsgBuf::GetCopy(void* dest, uint32 count)
{
    // if no bytes to copy, do nothing
    if (count == 0) {
        return;
    }

    assert(m_index < m_size);
    assert(m_index + count <= m_size);
    memcpy(dest, &m_buf[m_index], count);
    m_index += count;
}

// --------------------------------------------------------------------------
// GetByteAt
//
/// Return the byte at a given index location.
///
/// @param index - location of value requested.
///
/// @returns uint8 - byte value at the given index location.
// --------------------------------------------------------------------------
uint8 MsgBuf::GetByteAt(uint32 index)
{
    assert(index < m_size);
    return m_buf[index];
}

// --------------------------------------------------------------------------
// GetWordAt
//
/// Return the word at a given index location.
///
/// @param index - location of value requested.
///
/// @returns uint8 - byte value at the given index location.
// --------------------------------------------------------------------------
uint16 MsgBuf::GetWordAt(uint32 index)
{
    assert(index < m_size);
    return *(uint16*)(m_buf + index);
}

// --------------------------------------------------------------------------
// PutByte
//
/// Add a given byte to the buffer at the current index location, increment
/// the index by a byte.
///
/// @param uint8 - byte value to add to the buffer.
// --------------------------------------------------------------------------
void MsgBuf::PutByte(uint8 value)
{
    assert(m_index < m_size);
    m_buf[m_index] = value;
    m_index++;
    assert(m_index <= m_size);
}

// --------------------------------------------------------------------------
// PutWord
//
/// Add a given word to the buffer at the current index location, increment
/// the index by 2 bytes.
///
/// @param value - word value to add to the buffer.
// --------------------------------------------------------------------------
void MsgBuf::PutWord(uint16 value)
{
    assert(m_index < m_size);
    *(uint16*)(m_buf + m_index) = value;
    m_index += 2;
    assert(m_index <= m_size);
}

// --------------------------------------------------------------------------
// PutDWord
//
/// Add a given double word to the buffer at the current index location,
/// increment the index by 4 bytes.
///
/// @param value - double word value to add to the buffer.
// --------------------------------------------------------------------------
void MsgBuf::PutDWord(uint32 value)
{
    assert(m_index < m_size);
    *(uint32*)(m_buf + m_index) = value;
    m_index += 4;
    assert(m_index <= m_size);
}

// --------------------------------------------------------------------------
// PutCopy
//
/// Copy a given number of bytes from a given source to a given destination,
/// increment the index by the given number of bytes.
///
/// @param src - copy from the src to the buffer.
///
/// @param count - the number of bytes to copy.
// --------------------------------------------------------------------------
void MsgBuf::PutCopy(const void* src, uint32 count)
{
    // if no bytes to copy, do nothing
    if (count == 0) {
        return;
    }

    assert(m_index < m_size);
    assert(m_index + count <= m_size);
    memcpy(&m_buf[m_index], src, count);
    m_index += count;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this MsgBuf to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void MsgBuf::Print(std::ostream& stream)
{
    stream << _T("Buffer (") << (int)m_size << _T(" bytes): ");
    for (unsigned int i = 0; i < m_size; i++) {
        if (i % 16 == 0) {
            stream.width(4);
            stream.fill('0');
            stream << std::endl << std::dec << i << _T(": ");
        } else if (i % 8 == 0) {
            stream << "   ";
        }

        stream.width(2);
        stream.fill('0');
        stream << std::uppercase << std::hex << (int)m_buf[i] << _T(" ");
    }

    stream << std::endl << std::endl;
}

// --------------------------------------------------------------------------
// operator=
//
/// Assign one MsgBuf to another.
///
/// @param msgBuf - the ostream to compare to.
///
/// @returns MsgBuf& - this MsgBuf after assignment.
// --------------------------------------------------------------------------
MsgBuf& MsgBuf::operator=(MsgBuf& msgBuf)
{
    if (this == &msgBuf) {
        return *this;
    }

    m_size = msgBuf.GetSize();
    m_index = 0;
    m_svcType = msgBuf.GetSvcType();

    uint8* oldBuf = m_buf;
    m_buf = new uint8[m_size];
    memcpy(&m_buf[m_index],msgBuf.GetBuffer(),m_size);
    if (!m_buf == NULL) {
        delete [] m_buf;
    }

    return *this;
}

// --------------------------------------------------------------------------
// operator==
//
/// Compare two MsgBufs for equivalency.
///
/// @param msgBuf - the MsgBuf to compare to.
///
/// @returns bool - true if equivalent, false otherwise
// --------------------------------------------------------------------------
bool MsgBuf::operator==(MsgBuf& msgBuf)
{
    if (m_size != msgBuf.GetSize()) {
        return false;
    }
    if (m_svcType != msgBuf.GetSvcType()) {
        return false;
    }
    if (memcmp(m_buf,msgBuf.GetBuffer(),m_size) != 0) {
        return false;
    }

    return true;
}