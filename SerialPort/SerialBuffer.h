// SerialBuffer.h: interface for the CSerialBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALBUFFER_H__A84C99F3_21AA_4E26_B8E9_483595CCC6BF__INCLUDED_)
#define AFX_SERIALBUFFER_H__A84C99F3_21AA_4E26_B8E9_483595CCC6BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SERIAL_BUFFERSIZE  1024        //���ڶ�д��������С

class CSerialBuffer  
{
public:
    CSerialBuffer();
    virtual ~CSerialBuffer();
public:
    CSerialBuffer *next;                    //ָ�򻺳������е���һ�����
    DWORD   m_dwBytes;                        //�Ӵ��ڶ�ȡ���ֽ�����׼���򴮿ڷ��͵��ֽ���    
    BYTE    m_szBuffer[SERIAL_BUFFERSIZE];    //���ڶ�д����
};

#endif // !defined(AFX_SERIALBUFFER_H__A84C99F3_21AA_4E26_B8E9_483595CCC6BF__INCLUDED_)
