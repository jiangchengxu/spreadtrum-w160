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

#define SERIAL_TIMEOUT       1000        //串口读写超时时间

//串口状态
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
//串口事件枚举
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
//串口类
class CSerialPort
{
public:
    //关闭串口，结束串口线程
    BOOL StopPort();

    //打开串口，启动串口线程
    BOOL StartPort(LPCTSTR lpszCommName, BOOL bCreateThread = TRUE, BOOL bOverlapped = TRUE);

#ifdef FEATURE_SERIAL_ASYNWRITE
    //串口写线程函数
    static UINT CommWriteThreadProc(LPVOID pParam);
#endif

    //串口读线程函数
    static UINT CommReadThreadProc(LPVOID pParam);

    //串口断线检测函数
    static UINT CommDetectThreadProc(LPVOID pParam);

    //直接向串口写数据
    DWORD Write(LPCVOID lpBuffer, DWORD dwToWrite, DWORD dwTimeout = INFINITE);

    //直接从串口读数据
    DWORD Read(LPVOID lpBuffer, DWORD dwToRead, DWORD dwTimeout = INFINITE);

    //直接向串口发送控制命令
    BOOL IoControl(DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, DWORD dwTimeout = INFINITE);

    //等待串口读事件
    BOOL WaitEvent(DWORD dwTimeout = INFINITE);

    //填充写缓冲，通知写线程向串口写数据
    BOOL WriteToPort(const char *pBuf, WORD wLen, BOOL bReportError = TRUE);

    //清除串口缓冲区
    BOOL Purge(DWORD dwFlags);

    //是否连接到串口
    BOOL IsConnect() const;

    //打印缓冲区字节数(DEBUG)
    DWORD PrintBufBytes(int type);

    //构造函数，主要完成各个成员的初始化，将状态变量赋值，创建事件。
    CSerialPort();

    //析构函数，关闭串口，关闭句柄，释放空间
    virtual ~CSerialPort();

protected:
    //关闭串口
    void Close();

    //打开串口
    BOOL Open(LPCTSTR lpCommName, BOOL bOverlapped = FALSE);

    //配置串口
    BOOL Config();

    //获得上次错误码
    DWORD GetLastError() const;

    //获得串口句柄
    HANDLE GetCommHandle() const;

    //串口是否打开
    BOOL IsOpen() const;

public:
    BOOL CommIsReady();
    EnSerialState SetSerialState(const EnSerialState state);
    EnSerialState GetSerialState() const;

#ifdef FEATURE_SERIAL_ASYNWRITE
    HANDLE m_hCloseWriteEvent;    //串口关闭事件
    HANDLE m_hIoCtrlEvent;        //串口控制事件
    HANDLE m_hWriteEvent;        //串口写事件
    HANDLE m_hEventArray[SERAIL_EVENTARRAYNUM]; //事件数组
#endif

    CWinThread* m_WriteThread;            //写线程指针
    CWinThread* m_ReadThread;             //读线程指针
    CWinThread* m_DetectThread;           //掉线检测线程指针

#ifdef FEATURE_SERIAL_QUEUE
    CSerialBufQueue m_ReadBufQueue;        //读缓冲队列
    CSerialBufQueue m_WriteBufQueue;    //写缓冲队列
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
    volatile BOOL m_bIsConnect;        //串口是否打开标志
    HANDLE m_hComm;            /* 串行端口句柄 */
    BOOL   m_bOverlapped;    /* I/0 操作方式 (FALSE:同步 TRUE:异步) */
    DWORD  m_dwEvtMask;        /* 串口事件掩码 */
    DWORD  m_dwLastError;    /* 上次错误值 */
    volatile EnSerialState m_SerialState;    //串口状态
};

#endif // !defined(AFX_COMM_H__ADB4E48C_B7D8_4F13_A96A_FAC528226512__INCLUDED_)





















