// SerialBufQueue.h: interface for the CSerialBufQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALBUFQUEUE_H__6BA2935C_681F_4D8E_86D3_2B072F4CCB33__INCLUDED_)
#define AFX_SERIALBUFQUEUE_H__6BA2935C_681F_4D8E_86D3_2B072F4CCB33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialBuffer.h"

class CSerialBufQueue  
{
public:
    CSerialBufQueue();
    virtual ~CSerialBufQueue();
    BOOL IsEmpty();
    void AddToTail(CSerialBuffer* buf);
    CSerialBuffer* GetFromHead();
    void Clear();
private:
    CSerialBuffer *front;
    CSerialBuffer *rear;
    CRITICAL_SECTION m_csQueue;    //同步队列操作    
};

#endif // !defined(AFX_SERIALBUFQUEUE_H__6BA2935C_681F_4D8E_86D3_2B072F4CCB33__INCLUDED_)
