// SerialBufQueue.cpp: implementation of the CSerialBufQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialBufQueue.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialBufQueue::CSerialBufQueue()
{
    front = rear = NULL;
    InitializeCriticalSection(&m_csQueue);
}

CSerialBufQueue::~CSerialBufQueue()
{
    Clear();
    DeleteCriticalSection(&m_csQueue);    
}

inline BOOL CSerialBufQueue::IsEmpty()
{
    BOOL isEmpty;

    EnterCriticalSection(&m_csQueue);
    isEmpty = (front == NULL && rear == NULL);
    LeaveCriticalSection(&m_csQueue);
    
    return isEmpty;
}

void CSerialBufQueue::AddToTail(CSerialBuffer* buf)
{    
    ASSERT(buf != NULL);

    EnterCriticalSection(&m_csQueue);
    
    buf->next = NULL;

    if(IsEmpty())
        front = rear = buf;
    else
    {
        rear->next = buf;
        rear = buf;
    }
    
    LeaveCriticalSection(&m_csQueue);
}

CSerialBuffer* CSerialBufQueue::GetFromHead()
{
    EnterCriticalSection(&m_csQueue);
    
    CSerialBuffer *buf = NULL;

    if(!IsEmpty())
    {
        buf = front;
        if(front == rear)
        {
            front = rear = NULL;
        }
        else
        {
            front = front->next;
        }
    }

    LeaveCriticalSection(&m_csQueue);
    
    return buf;
}

void CSerialBufQueue::Clear()
{
    EnterCriticalSection(&m_csQueue);

    CSerialBuffer *buf = NULL;

    while(!IsEmpty())
    {
        buf = GetFromHead();
        delete buf;
    }

    LeaveCriticalSection(&m_csQueue);
}