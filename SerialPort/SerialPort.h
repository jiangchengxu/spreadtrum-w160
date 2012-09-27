// Comm.h: interface for the CComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMM_H__ADB4E48C_B7D8_4F13_A96A_FAC528226512__INCLUDED_)
#define AFX_COMM_H__ADB4E48C_B7D8_4F13_A96A_FAC528226512__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef FEATURE_SERIAL_QUEUE
#include "SerialBufQueue.h"
#else
#define SERIAL_RXBUFFERSIZE (4096 * 4)
#define SERIAL_TXBUFFERSIZE 4096

typedef struct {
    BYTE RxBuffer[SERIAL_RXBUFFERSIZE];
    BYTE *pRead;
    BYTE *pWrite;
    WORD wRxCount;
} StRxQueueCtrl;
#endif

#define SERIAL_TIMEOUT       1000        //���ڶ�д��ʱʱ��

//����״̬
typedef enum {
    SERIAL_STATE_INI,
    SERIAL_STATE_EXCEPTION,
    SERIAL_STATE_CMD,
    SERIAL_STATE_CMD_WAIT,
    SERIAL_STATE_DATA,
    SERIAL_STATE_CALL,
    SERIAL_STATE_MAX
} EnSerialState;

#ifdef FEATURE_SERIAL_ASYNWRITE
//�����¼�ö��
typedef enum {
    SERAIL_WRITEEVENT,
    SERAIL_IOCTRLEVENT,
    SERAIL_CLOSEEVENT,
    SERAIL_EVENTARRAYNUM
} EnSerialEvent;
#endif

#define QCOMSER_IOCTL_INDEX 0x800
#define IOCTL_QCOMSER_WAIT_NOTIFY_CODE CTL_CODE (FILE_DEVICE_UNKNOWN,\
                                                 QCOMSER_IOCTL_INDEX+1,\
                                                 METHOD_BUFFERED,\
                                                 FILE_ANY_ACCESS)
//������
class CSerialPort
{
public:
    //�رմ��ڣ����������߳�
    BOOL StopPort();

    //�򿪴��ڣ����������߳�
    BOOL StartPort(LPCTSTR lpszCommName, BOOL bCreateThread = TRUE, BOOL bOverlapped = TRUE);

#ifdef FEATURE_SERIAL_ASYNWRITE
    //����д�̺߳���
    static UINT CommWriteThreadProc(LPVOID pParam);
#endif

    //���ڶ��̺߳���
    static UINT CommReadThreadProc(LPVOID pParam);

    //���ڶ��߼�⺯��
    static UINT CommDetectThreadProc(LPVOID pParam);

    //ֱ���򴮿�д����
    DWORD Write(LPCVOID lpBuffer, DWORD dwToWrite, DWORD dwTimeout = INFINITE);

    //ֱ�ӴӴ��ڶ�����
    DWORD Read(LPVOID lpBuffer, DWORD dwToRead, DWORD dwTimeout = INFINITE);

    //ֱ���򴮿ڷ��Ϳ�������
    BOOL IoControl(DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, DWORD dwTimeout = INFINITE);

    //�ȴ����ڶ��¼�
    BOOL WaitEvent(DWORD dwTimeout = INFINITE);

    //���д���壬֪ͨд�߳��򴮿�д����
    BOOL WriteToPort(const char *pBuf, WORD wLen, BOOL bReportError = TRUE);

    //������ڻ�����
    BOOL Purge(DWORD dwFlags);

    //�Ƿ����ӵ�����
    BOOL IsConnect() const;

    //��ӡ�������ֽ���(DEBUG)
    DWORD PrintBufBytes(int type);

    //���캯������Ҫ��ɸ�����Ա�ĳ�ʼ������״̬������ֵ�������¼���
    CSerialPort();

    //�����������رմ��ڣ��رվ�����ͷſռ�
    virtual ~CSerialPort();

protected:
    //�رմ���
    void Close();

    //�򿪴���
    BOOL Open(LPCTSTR lpCommName, BOOL bOverlapped = FALSE);

    //���ô���
    BOOL Config();

    //����ϴδ�����
    DWORD GetLastError() const;

    //��ô��ھ��
    HANDLE GetCommHandle() const;

    //�����Ƿ��
    BOOL IsOpen() const;

public:
    BOOL CommIsReady();
    EnSerialState SetSerialState(const EnSerialState state);
    EnSerialState GetSerialState() const;

#ifdef FEATURE_SERIAL_ASYNWRITE
    HANDLE m_hCloseWriteEvent;    //���ڹر��¼�
    HANDLE m_hIoCtrlEvent;        //���ڿ����¼�
    HANDLE m_hWriteEvent;        //����д�¼�
    HANDLE m_hEventArray[SERAIL_EVENTARRAYNUM]; //�¼�����
#endif

    CWinThread* m_WriteThread;            //д�߳�ָ��
    CWinThread* m_ReadThread;             //���߳�ָ��
    CWinThread* m_DetectThread;           //���߼���߳�ָ��

#ifdef FEATURE_SERIAL_QUEUE
    CSerialBufQueue m_ReadBufQueue;        //���������
    CSerialBufQueue m_WriteBufQueue;    //д�������
#else
    StRxQueueCtrl m_RxQueueCtrl;
    CRITICAL_SECTION m_csRxQueue;
#ifdef FEATURE_SERIAL_ASYNWRITE
    BYTE m_TxBuff[SERIAL_TXBUFFERSIZE];
    WORD m_wTxCount;
    CRITICAL_SECTION m_csTxBuff;
#endif
#endif

private:
    OVERLAPPED m_ReadOvlp;
    OVERLAPPED m_WriteOvlp;
    OVERLAPPED m_WaitOvlp;
    OVERLAPPED m_IoCtrlOvlp;
    volatile BOOL m_bIsConnect;        //�����Ƿ�򿪱�־
    HANDLE m_hComm;            /* ���ж˿ھ�� */
    BOOL   m_bOverlapped;    /* I/0 ������ʽ (FALSE:ͬ�� TRUE:�첽) */
    DWORD  m_dwEvtMask;        /* �����¼����� */
    DWORD  m_dwLastError;    /* �ϴδ���ֵ */
    volatile EnSerialState m_SerialState;    //����״̬
};

#endif // !defined(AFX_COMM_H__ADB4E48C_B7D8_4F13_A96A_FAC528226512__INCLUDED_)





















