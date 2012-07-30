// SerialBuffer.h: interface for the CSerialBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALBUFFER_H__A84C99F3_21AA_4E26_B8E9_483595CCC6BF__INCLUDED_)
#define AFX_SERIALBUFFER_H__A84C99F3_21AA_4E26_B8E9_483595CCC6BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SERIAL_BUFFERSIZE  1024        //串口读写缓冲区大小

class CSerialBuffer  
{
public:
    CSerialBuffer();
    virtual ~CSerialBuffer();
public:
    CSerialBuffer *next;                    //指向缓冲区队列的下一个结点
    DWORD   m_dwBytes;                        //从串口读取的字节数或准备向串口发送的字节数    
    BYTE    m_szBuffer[SERIAL_BUFFERSIZE];    //串口读写缓存
};

#endif // !defined(AFX_SERIALBUFFER_H__A84C99F3_21AA_4E26_B8E9_483595CCC6BF__INCLUDED_)
