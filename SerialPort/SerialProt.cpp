// Comm.cpp: implementation of the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../HSDPA.h"
#include "../HSDPADlg.h"
#include "SerialPort.h"
#include "../resource.h"
#include "../Ds.h"
#include "Winioctl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void AfxErrorMsg(DWORD dwError)
{
    CString str;
    str.Format(_T("%d"), dwError);
    AfxMessageBox(str);
}

#ifdef _DEBUG
struct {
    char snd[512];
    char rcv[512];
} gStAtMsg;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialPort::CSerialPort()
{
    //数据成员初始化
    m_hComm = NULL;
    m_ReadThread = NULL;
    m_WriteThread = NULL;
    m_DetectThread = NULL;
    m_ReadThread = NULL;
    m_bOverlapped = FALSE;
    m_bIsConnect = FALSE;
    m_dwEvtMask = 0;
    m_SerialState = SERIAL_STATE_INI;
 
    memset(&m_ReadOvlp,0,sizeof(OVERLAPPED));
    memset(&m_WriteOvlp,0,sizeof(OVERLAPPED));
    memset(&m_WaitOvlp,0,sizeof(OVERLAPPED));
    memset(&m_IoCtrlOvlp,0,sizeof(OVERLAPPED));

#ifdef FEATURE_SERIAL_ASYNWRITE
    for(int i = 0; i < SERAIL_EVENTARRAYNUM; i++)
    {
        m_hEventArray[i] = NULL;
    }
    //创建自动重置事件
    m_hWriteEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hIoCtrlEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hCloseWriteEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    m_hEventArray[SERAIL_WRITEEVENT]  = m_hWriteEvent;
    m_hEventArray[SERAIL_IOCTRLEVENT] = m_hIoCtrlEvent;
    m_hEventArray[SERAIL_CLOSEEVENT]  = m_hCloseWriteEvent;    
#endif

#ifndef FEATURE_SERIAL_QUEUE
    InitializeCriticalSection(&m_csRxQueue);
#ifdef FEATURE_SERIAL_ASYNWRITE
    InitializeCriticalSection(&m_csTxBuff);
#endif
#endif
}

CSerialPort::~CSerialPort()
{
    StopPort();

#ifdef FEATURE_SERIAL_ASYNWRITE
    for(int i = 0; i < SERAIL_EVENTARRAYNUM; i++)
    {
        if(m_hEventArray[i] != NULL)
            ::CloseHandle(m_hEventArray[i]);            
    }
#endif

#ifndef FEATURE_SERIAL_QUEUE
    DeleteCriticalSection(&m_csRxQueue);
#ifdef FEATURE_SERIAL_ASYNWRITE
    DeleteCriticalSection(&m_csTxBuff);
#endif
#endif
}

BOOL CSerialPort::Open(LPCTSTR lpszCommName, BOOL bOverlapped)
{    
    // Check if the port isn't already opened
    ASSERT(!IsOpen());

    m_bOverlapped = bOverlapped;

    m_hComm = ::CreateFile(lpszCommName,
                           GENERIC_WRITE | GENERIC_READ,
                           0,
                           NULL,
                           OPEN_EXISTING,
                           bOverlapped ? FILE_FLAG_OVERLAPPED : 0,
                           NULL);

    if(m_hComm == INVALID_HANDLE_VALUE)
    {
        m_hComm = NULL;
        m_bOverlapped = FALSE;
        return FALSE;
    }
    return TRUE;
}

void CSerialPort::Close()
{    
    if(IsOpen())
    {
		m_bIsConnect = FALSE;
		SetCommMask(m_hComm,0);
		EscapeCommFunction(m_hComm,CLRDTR);
        Purge(PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
        ::CloseHandle(m_hComm);
        m_hComm = NULL;
        m_bOverlapped = FALSE;
    }
	Sleep(100);
}

inline BOOL CSerialPort::IsOpen() const
{
    return (m_hComm != NULL);
}

inline HANDLE CSerialPort::GetCommHandle() const
{
    return m_hComm;
}

DWORD CSerialPort::GetLastError() const
{
    return ::GetLastError();
}

//直接从串口读数据
DWORD CSerialPort::Read(LPVOID lpBuffer, DWORD dwToRead, DWORD dwTimeout)
{
    ASSERT(IsOpen());

    DWORD dwRead = 0;

    //同步读
    if(!m_bOverlapped) 
    {
        // Read the data
        if(!::ReadFile(m_hComm, lpBuffer, dwToRead, &dwRead, 0))
            dwRead = 0;
    }
    else //异步读
    {    
        // Wait for the event to happen
        memset(&m_ReadOvlp,0,sizeof(OVERLAPPED));
        m_ReadOvlp.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        
        // Write the data
        if(!::ReadFile(m_hComm, lpBuffer, dwToRead, &dwRead, &m_ReadOvlp))
        {
            m_dwLastError = ::GetLastError();
            // Overlapped operation in progress is not an actual error
            if(m_dwLastError == ERROR_HANDLE_EOF)
                NULL;
            else if(m_dwLastError != ERROR_IO_PENDING)
                dwRead = 0;
            else
            {
                // Wait for the overlapped operation to complete
                switch(::WaitForSingleObject(m_ReadOvlp.hEvent, dwTimeout))
                {
                case WAIT_OBJECT_0:
                    // The overlapped operation has completed
                    if(!::GetOverlappedResult(m_hComm, &m_ReadOvlp, &dwRead, FALSE))
                    {
                        if(::GetLastError() != ERROR_HANDLE_EOF)
                            dwRead = 0;
                    }
                    break;
                case WAIT_TIMEOUT:
                    // Cancel the I/O operation
                    ::CancelIo(m_hComm);
                default:
                    dwRead = 0;
                }
            }
        }
        ::CloseHandle(m_ReadOvlp.hEvent);
    }

    return dwRead;
}

//直接向串口写数据
DWORD CSerialPort::Write(LPCVOID lpBuffer, DWORD dwToWrite, DWORD dwTimeout)
{
    ASSERT(IsOpen());

    DWORD dwWrite = 0;

    //同步写
    if(!m_bOverlapped) 
    {
        // Write the data
        if(!::WriteFile(m_hComm, lpBuffer, dwToWrite, &dwWrite, 0))
            dwWrite = 0;
    }
    else //异步写
    {    
        // Wait for the event to happen
        memset(&m_WriteOvlp,0,sizeof(OVERLAPPED));
        m_WriteOvlp.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        
        // Write the data
        if(!::WriteFile(m_hComm, lpBuffer, dwToWrite, &dwWrite, &m_WriteOvlp))
        {    
            // Overlapped operation in progress is not an actual error
            if(::GetLastError() != ERROR_IO_PENDING)
                dwWrite = 0;
            else
            {
                // Wait for the overlapped operation to complete
                switch(::WaitForSingleObject(m_WriteOvlp.hEvent, dwTimeout))
                {
                case WAIT_OBJECT_0:
                    // The overlapped operation has completed
                    if(!::GetOverlappedResult(m_hComm, &m_WriteOvlp, &dwWrite, FALSE))
                        dwWrite = 0;
                    break;
                case WAIT_TIMEOUT:
                    // Cancel the I/O operation
                    ::CancelIo(m_hComm);
                default:
                    dwWrite = 0;
                }
            }
        }
        ::CloseHandle(m_WriteOvlp.hEvent);
    }

    return dwWrite;
}

BOOL CSerialPort::WaitEvent(DWORD dwTimeout)
{
    ASSERT(IsOpen());

    m_dwEvtMask = 0;
    BOOL bRet = FALSE;
    DWORD dwTrans;

    // Wait for the COM event
    if(!m_bOverlapped) //同步
    {
        if(!::WaitCommEvent(m_hComm, LPDWORD(&m_dwEvtMask), 0))
            bRet = FALSE;
    }
    else //异步
    {
        // Wait for the event to happen
        memset(&m_WaitOvlp,0,sizeof(OVERLAPPED));
        m_WaitOvlp.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        
        // Wait for the COM event
        if(!::WaitCommEvent(m_hComm, LPDWORD(&m_dwEvtMask), &m_WaitOvlp))
        {    
            // Overlapped operation in progress is not an actual error
            if(::GetLastError() != ERROR_IO_PENDING)
                bRet = FALSE;
            else
            {
                // Wait for the overlapped operation to complete
                switch (::WaitForSingleObject(m_WaitOvlp.hEvent,dwTimeout))
                {
                case WAIT_OBJECT_0:
                    if(!::GetOverlappedResult(m_hComm, &m_WaitOvlp, &dwTrans, FALSE))
                        bRet = FALSE;
                    else
                        bRet = TRUE;
                    break;
        
                case WAIT_TIMEOUT:
                    // Cancel the I/O operation
                    ::CancelIo(m_hComm);                
                default:
                    bRet = FALSE;
                }
            }
        }
        ::CloseHandle(m_WaitOvlp.hEvent);
    }

    return bRet;
}

BOOL CSerialPort::Purge(DWORD dwFlags)
{
    ASSERT(IsOpen());
    return ::PurgeComm(m_hComm, dwFlags);
}

BOOL CSerialPort::IoControl(DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, DWORD dwTimeout)
{
    ASSERT(IsOpen());

    BOOL bRet = FALSE;

    //同步控制
    if(!m_bOverlapped) 
    {
        if(!::DeviceIoControl(m_hComm,
                              dwIoControlCode,
                              lpInBuffer,
                              nInBufferSize,
                              lpOutBuffer,
                              nOutBufferSize,
                              lpBytesReturned,
                              0))
        bRet = FALSE;
    }
    else //异步控制
    {
        // Wait for the event to happen
        memset(&m_IoCtrlOvlp,0,sizeof(OVERLAPPED));
        m_IoCtrlOvlp.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        
        if(!::DeviceIoControl(m_hComm,
                              dwIoControlCode,
                              lpInBuffer,
                              nInBufferSize,
                              lpOutBuffer,
                              nOutBufferSize,
                              lpBytesReturned,
                              &m_IoCtrlOvlp))    
        {
            // Overlapped operation in progress is not an actual error
            if(::GetLastError() != ERROR_IO_PENDING)
                bRet = FALSE;
            else
            {
                // Wait for the overlapped operation to complete
                switch(::WaitForSingleObject(m_IoCtrlOvlp.hEvent, dwTimeout))
                {
                case WAIT_OBJECT_0:
                    // The overlapped operation has completed
                    if(!::GetOverlappedResult(m_hComm, &m_IoCtrlOvlp, lpBytesReturned, FALSE))  
                        bRet = FALSE;
                    else
                        bRet = TRUE;
                    break; 
                case WAIT_TIMEOUT:
                    // Cancel the I/O operation
                    ::CancelIo(m_hComm);
                default:
                    bRet = FALSE;
                }
            }
        }
        ::CloseHandle(m_IoCtrlOvlp.hEvent);
    }
    
    // Return successfully
    return bRet;
}

#ifdef FEATURE_SERIAL_ASYNWRITE
UINT CSerialPort::CommWriteThreadProc(LPVOID pParam)
{
    DWORD dwWrite;
    DWORD dwEvent;
    CSerialPort *pComm = (CSerialPort*)pParam;

//  AfxMessageBox("Serial Write Thread Start!");
    for(;;)
    {
        dwEvent = ::WaitForMultipleObjects(SERAIL_EVENTARRAYNUM, pComm->m_hEventArray, FALSE, INFINITE);
        dwEvent -= WAIT_OBJECT_0;

        switch(dwEvent) {
        case SERAIL_WRITEEVENT:
            pComm->Purge(PURGE_TXCLEAR);
            dwWrite = 0;
#ifdef FEATURE_SERIAL_QUEUE    
            CSerialBuffer *buf;
            while(buf = pComm->m_WriteBufQueue.GetFromHead())
            {
                dwWrite = 0;                
                dwWrite = pComm->Write(buf->m_szBuffer, buf->m_dwBytes);
                delete buf;
            }
#else
            EnterCriticalSection(&pComm->m_csTxBuff);
            dwWrite = pComm->Write(pComm->m_TxBuff, pComm->m_wTxCount);
            LeaveCriticalSection(&pComm->m_csTxBuff);
#endif
            break;

        case SERAIL_IOCTRLEVENT:
//            pComm->IoControl();
            break;
        case SERAIL_CLOSEEVENT:
            pComm->m_WriteThread = NULL;
            AfxEndThread(0);
            break;
        }
    }
    return 0;
}
#endif

//正如在《Serial communications in Microsoft Win32》等文章中提到的，同步（NonOverLapped）方式是
//比较简单的一种方式,，编写起来代码的长度要明显少于异步（OverLapped）方式，我开始用同步方式编写
//了整个子程序，在 Windows98 下工作正常，但后来在 Windows2000下测试，发现接收正常，但一发送数
//据，程序就会停在那里，原因应该在于同步方式下如果有一个通讯 Api 在操作中，另一个会阻塞直到上一
//个操作完成，所以当读数据的线程停留在 WaitCommEvent 的时候，WriteFile 就停在那里。我又测试了我
//手上所有有关串行通讯的例子程序，发现所有使用同步方式的程序在 Windows 2000 下全部工作不正常，
//对这个问题我一直找不到解决的办法，后来在 Iczelion 站点上发现一篇文章提到 NT 下对串行通讯的处
//理和 9x 有些不同，根本不要指望在 NT 或 Windows 2000 下用同步方式同时收发数据，我只好又用异步
//方式把整个通讯子程序重新写了一遍。
BOOL CSerialPort::StartPort(LPCTSTR lpszCommName, BOOL bCreateThread, BOOL bOverlapped)
{
    //注意: 强制设置为异步(重叠)I/O方式
    bOverlapped = TRUE;

#ifndef FEATURE_SERIAL_QUEUE
    EnterCriticalSection(&m_csRxQueue);    
    memset(m_RxQueueCtrl.RxBuffer, 0, SERIAL_RXBUFFERSIZE);
    m_RxQueueCtrl.pRead = m_RxQueueCtrl.pWrite = &m_RxQueueCtrl.RxBuffer[0];
    m_RxQueueCtrl.wRxCount = 0;
    LeaveCriticalSection(&m_csRxQueue);

#ifdef FEATURE_SERIAL_ASYNWRITE
    EnterCriticalSection(&m_csTxBuff);    
    memset(m_TxBuff, 0, SERIAL_TXBUFFERSIZE);
    m_wTxCount = 0;
    LeaveCriticalSection(&m_csTxBuff);
#endif
#endif
    
    if(Open(lpszCommName, bOverlapped) && Config())
    {
        m_bIsConnect = TRUE;      

        if(bCreateThread)
        {
            m_ReadThread = AfxBeginThread(CommReadThreadProc, this);
            m_DetectThread = AfxBeginThread(CommDetectThreadProc, this);
#ifdef FEATURE_SERIAL_ASYNWRITE
            m_WriteThread = AfxBeginThread(CommWriteThreadProc, this);
#endif
        }

        m_SerialState = SERIAL_STATE_CMD;        
        return TRUE;
    }
    else
    {
        m_SerialState = SERIAL_STATE_EXCEPTION;        
        return FALSE;        
    }
}

BOOL CSerialPort::StopPort()
{
#ifdef FEATURE_SERIAL_ASYNWRITE
    if(m_WriteThread)
    {
        if(m_hCloseWriteEvent)
            ::SetEvent(m_hCloseWriteEvent);
        m_WriteThread = NULL;
    }
#endif

#if 0
    if(m_ReadThread && m_ReadThread->m_hThread)
    {
        ::TerminateThread(m_ReadThread->m_hThread, 0);
        m_ReadThread = NULL;
    }
#endif

#ifdef FEATURE_SERIAL_QUEUE        
    m_ReadBufQueue.Clear();
    m_WriteBufQueue.Clear();
#endif

    Close();
    m_SerialState = SERIAL_STATE_INI;    
    return TRUE;
}

CHSDPADlg *pSpMainDlg = NULL;

BOOL CSerialPort::WriteToPort(const char *pBuf, WORD wLen, BOOL bReportError)
{
    ASSERT(pBuf != NULL && wLen < SERIAL_TXBUFFERSIZE);

    if(m_SerialState != SERIAL_STATE_CMD)
    {
        if(pSpMainDlg)
        {
            BOOL bSyncInitMask;
            EnterCriticalSection(&pSpMainDlg->m_csSyncInitMask);
            bSyncInitMask = pSpMainDlg->m_bSyncInitMask; //added by wk
            LeaveCriticalSection(&pSpMainDlg->m_csSyncInitMask);
            if(!bSyncInitMask && bReportError)
            {
                if(m_SerialState == SERIAL_STATE_INI)
                    AfxMessageBox(IDS_USB_PORT_INI);
                else if(m_SerialState == SERIAL_STATE_EXCEPTION)
                    AfxMessageBox(IDS_USB_PORT_EXCEPTION);
                else if(m_SerialState == SERIAL_STATE_CMD_WAIT)
                    AfxMessageBox(IDS_USB_PORT_WAIT);
                else if(m_SerialState == SERIAL_STATE_DATA)
                    AfxMessageBox(IDS_USB_PORT_DATA);
                else
                    AfxMessageBox(IDS_USB_PORT_WAIT);
            }  
        }
        return FALSE;
    }

#ifdef FEATURE_SERIAL_ASYNWRITE
#ifdef FEATURE_SERIAL_QUEUE    
    CSerialBuffer *pWriteBuf = new CSerialBuffer;
    pWriteBuf->m_dwBytes = wLen;
    memcpy(pWriteBuf->m_szBuffer, pBuf, wLen);
    m_WriteBufQueue.AddToTail(pWriteBuf);
#else
    EnterCriticalSection(&m_csTxBuff);
    m_wTxCount = wLen;
    memcpy(m_TxBuff, pBuf, wLen);
    LeaveCriticalSection(&m_csTxBuff);
#endif
    ASSERT(m_WriteThread != NULL);
    ::SetEvent(m_hWriteEvent);
    m_SerialState = SERIAL_STATE_CMD_WAIT;
#else
    DWORD dwWrite = 0;
    Purge(PURGE_TXCLEAR);
    m_SerialState = SERIAL_STATE_CMD_WAIT;
    dwWrite = Write(pBuf, wLen);
    if(dwWrite != wLen)
    {
        m_SerialState = SERIAL_STATE_CMD;
        return FALSE;
    }
#endif

#ifdef _DEBUG
    memset(&gStAtMsg, 0, sizeof(gStAtMsg));
    memcpy(gStAtMsg.snd, pBuf, wLen);
#endif

#ifdef FEATURE_ATTEST_SUPPORT
	/*added by taolinling start on 2008.3.19*/
	//输入，out
	CStdioFile file;
	if(file.Open("AtDebug.log", CFile::modeReadWrite))
	{
		CString str;
		str.Format("%s", &pBuf[0]);
		str.Insert(str.GetLength(),"\n");
		DWORD dwActual = file.SeekToEnd();
		file.WriteString(str);
		file.Close();		
	}
	/*added by taolinling end on 2008.3.19*/
#endif
    return TRUE;
}

UINT CSerialPort::CommReadThreadProc(LPVOID pParam)
{
	CSerialPort *pComm = (CSerialPort*)pParam;
    DWORD   dwErrorFlags;
    COMSTAT comStat;
    pComm->Purge(PURGE_RXCLEAR);
    //注册监听的事件
    ::SetCommMask(pComm->m_hComm, EV_RXCHAR | EV_ERR);
    
    while(pComm->IsConnect()) 
    {
        if(pComm->WaitEvent())
        {    
            if(pComm->m_dwEvtMask & EV_RXCHAR)
            {                
				DWORD dwReadBytes = 0;			
                do    //读缓冲区有数据
                {                    
                    BYTE WorkBuff[SERIAL_RXBUFFERSIZE];
                    memset(WorkBuff, 0, SERIAL_RXBUFFERSIZE);
                    if((dwReadBytes = pComm->Read(WorkBuff, 1)) > 0)
                    {
                        DWORD wCopyBytes = 0;
                        BYTE *ptr = WorkBuff;                   
                        EnterCriticalSection(&pComm->m_csRxQueue);
                        WORD wRxCount = pComm->m_RxQueueCtrl.wRxCount;
                        wCopyBytes = min(dwReadBytes, (WORD)(SERIAL_RXBUFFERSIZE - pComm->m_RxQueueCtrl.wRxCount));
                        while(wCopyBytes-- > 0)
                        {
                            *pComm->m_RxQueueCtrl.pWrite++ = *ptr++;
                            if(pComm->m_RxQueueCtrl.pWrite >= &pComm->m_RxQueueCtrl.RxBuffer[SERIAL_RXBUFFERSIZE])
                                pComm->m_RxQueueCtrl.pWrite = &pComm->m_RxQueueCtrl.RxBuffer[0];
                            pComm->m_RxQueueCtrl.wRxCount++;
                        }  
                        LeaveCriticalSection(&pComm->m_csRxQueue);
                        ::SetEvent(g_AtRespEvent);
                    }  
                }while(dwReadBytes > 0);
//Debug
#ifdef _DEBUG
#if 0
                CStdioFile file;
                file.Open("CommTest.txt", CFile::modeCreate | CFile::modeWrite);
                file.WriteString((LPCTSTR)pComm->m_RxQueueCtrl.RxBuffer);
                file.Close();
#endif
#endif
            }
            else if(pComm->m_dwEvtMask & EV_ERR)
            {
                pComm->Purge(PURGE_RXCLEAR);
                ::ClearCommError(pComm->m_hComm, &dwErrorFlags, &comStat);
            }
        }
    }
    pComm->m_ReadThread = NULL;
    return 0;
}

UINT CSerialPort::CommDetectThreadProc(LPVOID pParam)
{
    CSerialPort *pComm = (CSerialPort*)pParam;
    ULONG        event = 0, junk = 0;
    
 //   AfxMessageBox("DECT");
    
    if(!pComm->IsConnect())
        return 0;
    if(pComm->IoControl(IOCTL_QCOMSER_WAIT_NOTIFY_CODE,
                       NULL,
                       0,
                       &event,
                       sizeof(event),
                       &junk))
    {  
//    AfxMessageBox("disc");
        if(!pComm->IsConnect())
            return 0;
        
        if(event & 0x01)
        {
            CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
            ASSERT(pMainDlg);
         
#ifdef FEATURE_AUTOQUIT
//			AfxMessageBox(IDS_CARD_DISCONN);
			CHSDPADlg* pMainDlgTemp = (CHSDPADlg*)(theApp.GetMainWnd());
			PreMsg *PreMsgDlgTemp = NULL;
			
			if (!pMainDlgTemp->m_bPortStatus)
			{
				
				if (NULL == PreMsgDlgTemp)
				{
					CString strDspInfo;
					PreMsg *PreMsgDlg = new PreMsg;
					ASSERT(PreMsgDlg);
					PreMsgDlg->Create(PreMsgDlg->IDD);
					//PreMsgDlg->SetBitmap(IDB_SPLASH,255,0,255);
					PreMsgDlg->SetBitmap(g_SetData.Main_szSplashBmp, 255 ,0 ,255);
					PreMsgDlg->ShowWindow(SW_SHOW);
					strDspInfo.LoadString(IDS_CARD_DISCONN);
					PreMsgDlg->SetText(strDspInfo);
					Sleep(2000);
					PreMsgDlg->DestroyWindow();
					delete PreMsgDlg;
					pMainDlgTemp->PostMessage(WM_QUIT);
				}
				else
				{
					/*
					CString strDspInfo;
					strDspInfo.LoadString(IDS_CARD_DISCONN);
					PreMsgDlgTemp->SetText(strDspInfo);
					Sleep(2000);
					*/
					exit(0);
				}				
			}
			else
			{
				pMainDlgTemp->m_bPortStatus = false;
			}
#else
            pComm->StopPort();
            pComm->m_DetectThread = NULL;
            pMainDlg->m_bInComSms = FALSE;
            pMainDlg->m_bInComCall = FALSE;
            if(g_BGPassEvt)
                ::SetEvent(g_BGPassEvt);
            pMainDlg->ResetSyncData();
            pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_RSSI, 6);
            pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NETWORK, NW_TYPE_NOSRV);
            pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_ROAM, ROAM_OFF);
            pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 0);
		    (theApp.GetMainWnd())->SetTimer(IDT_USB_CONN_DETECT, 1000, NULL);
#endif
        }
    }
    return 0;
}

//打印缓冲区字节数
DWORD CSerialPort::PrintBufBytes(int type) //0: 读缓冲，1: 写缓冲
{
    DWORD   dwErrorFlags;
    COMSTAT comStat;    

    CString str;
    DWORD   dwBytes;

    ::ClearCommError(m_hComm, &dwErrorFlags, &comStat);    

    if(type == 0)
    {
        str.Format(_T("ReadBuf: %d"), comStat.cbInQue);
        dwBytes = comStat.cbInQue;
    }
    else
    {
        str.Format(_T("WriteBuf: %d"), comStat.cbOutQue);
        dwBytes = comStat.cbOutQue;
    }
    AfxMessageBox(str);
    return dwBytes;
}

BOOL CSerialPort::Config()
{
    ASSERT(IsOpen());

    COMMTIMEOUTS TimeOuts;

    ::SetupComm(m_hComm, 1200, 1200);
    
    TimeOuts.ReadIntervalTimeout = MAXDWORD;
    TimeOuts.ReadTotalTimeoutMultiplier = 0;
    TimeOuts.ReadTotalTimeoutConstant = 1000;
    TimeOuts.WriteTotalTimeoutMultiplier = 0;
    TimeOuts.WriteTotalTimeoutConstant = 1000;
    ::SetCommTimeouts(m_hComm, &TimeOuts);

    DCB dcb;    
    dcb.DCBlength = sizeof(DCB);
    
    if(!::GetCommState(m_hComm, &dcb))
        return FALSE;

    dcb.BaudRate = CBR_19200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    
    return ::SetCommState(m_hComm, &dcb);
}

BOOL CSerialPort::IsConnect() const
{
    return m_bIsConnect;
}
EnSerialState CSerialPort::GetSerialState() const
{
    return m_SerialState;
}

EnSerialState CSerialPort::SetSerialState(const EnSerialState state)
{
    ASSERT(state >= SERIAL_STATE_INI && state < SERIAL_STATE_MAX);

    EnSerialState oldState = m_SerialState;
    m_SerialState = state;
    return oldState;
}

BOOL CSerialPort::CommIsReady()
{   
    EnSerialState state;

    if((state = GetSerialState()) != SERIAL_STATE_CMD)
    {
        if(state == SERIAL_STATE_INI)
            AfxMessageBox(IDS_USB_PORT_INI);
        else if(state == SERIAL_STATE_EXCEPTION)
            AfxMessageBox(IDS_USB_PORT_EXCEPTION);
//         else if(state == SERIAL_STATE_CMD_WAIT)
//             AfxMessageBox(IDS_USB_PORT_WAIT);
        else if(state == SERIAL_STATE_DATA)
            AfxMessageBox(IDS_USB_PORT_DATA);
//         else
//             AfxMessageBox(IDS_USB_PORT_WAIT);
        return FALSE;
    }
    else
        return TRUE;
}

/***********************wj add for voice com part**************************/

CSerialPort_Voice::CSerialPort_Voice()
{
    //数据成员初始化
    m_hComm_Voice = NULL;
    m_ReadThread_Voice = NULL;
    m_WriteThread_Voice = NULL;
    m_DetectThread_Voice = NULL;
    m_ReadThread_Voice = NULL;
    m_bOverlapped_Voice = FALSE;
    m_bIsConnect_Voice = FALSE;
    m_dwEvtMask_Voice = 0;
    m_SerialState_Voice = SERIAL_STATE_INI;
 
    memset(&m_ReadOvlp_Voice,0,sizeof(OVERLAPPED));
    memset(&m_WriteOvlp_Voice,0,sizeof(OVERLAPPED));
    memset(&m_WaitOvlp_Voice,0,sizeof(OVERLAPPED));
    memset(&m_IoCtrlOvlp_Voice,0,sizeof(OVERLAPPED));

#ifdef FEATURE_SERIAL_ASYNWRITE
    for(int i = 0; i < SERAIL_EVENTARRAYNUM; i++)
    {
        m_hEventArray_Voice[i] = NULL;
    }
    //创建自动重置事件
    m_hWriteEvent_Voice = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hIoCtrlEvent_Voice = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hCloseWriteEvent_Voice = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    m_hEventArray_Voice[SERAIL_WRITEEVENT]  = m_hWriteEvent_Voice;
    m_hEventArray_Voice[SERAIL_IOCTRLEVENT] = m_hIoCtrlEvent_Voice;
    m_hEventArray_Voice[SERAIL_CLOSEEVENT]  = m_hCloseWriteEvent_Voice;    
#endif

#ifndef FEATURE_SERIAL_QUEUE
    InitializeCriticalSection(&m_csRxQueue_Voice);
#ifdef FEATURE_SERIAL_ASYNWRITE
    InitializeCriticalSection(&m_csTxBuff_Voice);
#endif
#endif
}

CSerialPort_Voice::~CSerialPort_Voice()
{
    StopPort_Voice();

#ifdef FEATURE_SERIAL_ASYNWRITE
    for(int i = 0; i < SERAIL_EVENTARRAYNUM; i++)
    {
        if(m_hEventArray_Voice[i] != NULL)
            ::CloseHandle(m_hEventArray_Voice[i]);            
    }
#endif

#ifndef FEATURE_SERIAL_QUEUE
    DeleteCriticalSection(&m_csRxQueue_Voice);
#ifdef FEATURE_SERIAL_ASYNWRITE
    DeleteCriticalSection(&m_csTxBuff_Voice);
#endif
#endif
}

BOOL CSerialPort_Voice::Open_Voice(LPCTSTR lpszCommName, BOOL bOverlapped)
{    
    // Check if the port isn't alRead_Voicey Open_Voiceed
//    ASSERT(!IsOpen_Voice()); zhjj test

    m_bOverlapped_Voice = bOverlapped;

    m_hComm_Voice = ::CreateFile(lpszCommName,
                           GENERIC_WRITE | GENERIC_READ,
                           0,
                           NULL,
                           OPEN_EXISTING,
                           bOverlapped ? FILE_FLAG_OVERLAPPED : 0,
                           NULL);

    if(m_hComm_Voice == INVALID_HANDLE_VALUE)
    {
        m_hComm_Voice = NULL;
        m_bOverlapped_Voice = FALSE;
        return FALSE;
    }
    return TRUE;
}

void CSerialPort_Voice::Close_Voice()
{    
    if(IsOpen_Voice())
    {
		m_bIsConnect_Voice = FALSE;
		SetCommMask(m_hComm_Voice,0);
		EscapeCommFunction(m_hComm_Voice,CLRDTR);
        Purge_Voice(PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
        ::CloseHandle(m_hComm_Voice);
        m_hComm_Voice = NULL;
        m_bOverlapped_Voice = FALSE;
    }
	Sleep(100);
}

inline BOOL CSerialPort_Voice::IsOpen_Voice() const
{
    return (m_hComm_Voice != NULL);
}

inline HANDLE CSerialPort_Voice::GetCommHandle_Voice() const
{
    return m_hComm_Voice;
}

DWORD CSerialPort_Voice::GetLastError_Voice() const
{
    return ::GetLastError();
}

//直接从串口读数据
DWORD CSerialPort_Voice::Read_Voice(LPVOID lpBuffer, DWORD dwToRead, DWORD dwTimeout)
{
    ASSERT(IsOpen_Voice());

    DWORD dwRead_Voice = 0;

    //同步读
    if(!m_bOverlapped_Voice) 
    {
        // Read_Voice the data
        if(!::ReadFile(m_hComm_Voice, lpBuffer, dwToRead, &dwRead_Voice, 0))
            dwRead_Voice = 0;
    }
    else //异步读
    {    
        // Wait for the event to happen
        memset(&m_ReadOvlp_Voice,0,sizeof(OVERLAPPED));
        m_ReadOvlp_Voice.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        
        // Write_Voice the data
        if(!::ReadFile(m_hComm_Voice, lpBuffer, dwToRead, &dwRead_Voice, &m_ReadOvlp_Voice))
        {
            m_dwLastError_Voice = ::GetLastError();
            // Overlapped operation in progress is not an actual error
            if(m_dwLastError_Voice == ERROR_HANDLE_EOF)
                NULL;
            else if(m_dwLastError_Voice != ERROR_IO_PENDING)
                dwRead_Voice = 0;
            else
            {
                // Wait for the overlapped operation to complete
                switch(::WaitForSingleObject(m_ReadOvlp_Voice.hEvent, dwTimeout))
                {
                case WAIT_OBJECT_0:
                    // The overlapped operation has completed
                    if(!::GetOverlappedResult(m_hComm_Voice, &m_ReadOvlp_Voice, &dwRead_Voice, FALSE))
                    {
                        if(::GetLastError() != ERROR_HANDLE_EOF)
                            dwRead_Voice = 0;
                    }
                    break;
                case WAIT_TIMEOUT:
                    // Cancel the I/O operation
                    ::CancelIo(m_hComm_Voice);
                default:
                    dwRead_Voice = 0;
                }
            }
        }
        ::CloseHandle(m_ReadOvlp_Voice.hEvent);
    }

    return dwRead_Voice;
}

//直接向串口写数据
DWORD CSerialPort_Voice::Write_Voice(LPCVOID lpBuffer, DWORD dwToWrite, DWORD dwTimeout)
{
    ASSERT(IsOpen_Voice());

    DWORD dwWrite_Voice = 0;

    //同步写
    if(!m_bOverlapped_Voice) 
    {
        // Write_Voice the data
        if(!::WriteFile(m_hComm_Voice, lpBuffer, dwToWrite, &dwWrite_Voice, 0))
            dwWrite_Voice = 0;
    }
    else //异步写
    {    
        // Wait for the event to happen
        memset(&m_WriteOvlp_Voice,0,sizeof(OVERLAPPED));
        m_WriteOvlp_Voice.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        
        // Write_Voice the data
        if(!::WriteFile(m_hComm_Voice, lpBuffer, dwToWrite, &dwWrite_Voice, &m_WriteOvlp_Voice))
        {    
            // Overlapped operation in progress is not an actual error
            if(::GetLastError() != ERROR_IO_PENDING)
                dwWrite_Voice = 0;
            else
            {
                // Wait for the overlapped operation to complete
                switch(::WaitForSingleObject(m_WriteOvlp_Voice.hEvent, dwTimeout))
                {
                case WAIT_OBJECT_0:
                    // The overlapped operation has completed
                    if(!::GetOverlappedResult(m_hComm_Voice, &m_WriteOvlp_Voice, &dwWrite_Voice, FALSE))
                        dwWrite_Voice = 0;
                    break;
                case WAIT_TIMEOUT:
                    // Cancel the I/O operation
                    ::CancelIo(m_hComm_Voice);
                default:
                    dwWrite_Voice = 0;
                }
            }
        }
        ::CloseHandle(m_WriteOvlp_Voice.hEvent);
    }

    return dwWrite_Voice;
}

BOOL CSerialPort_Voice::WaitEvent_Voice(DWORD dwTimeout)
{
    ASSERT(IsOpen_Voice());

    m_dwEvtMask_Voice = 0;
    BOOL bRet = FALSE;
    DWORD dwTrans;

    // Wait for the COM event
    if(!m_bOverlapped_Voice) //同步
    {
        if(!::WaitCommEvent(m_hComm_Voice, LPDWORD(&m_dwEvtMask_Voice), 0))
            bRet = FALSE;
    }
    else //异步
    {
        // Wait for the event to happen
        memset(&m_WaitOvlp_Voice,0,sizeof(OVERLAPPED));
        m_WaitOvlp_Voice.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        
        // Wait for the COM event
        if(!::WaitCommEvent(m_hComm_Voice, LPDWORD(&m_dwEvtMask_Voice), &m_WaitOvlp_Voice))
        {    
            // Overlapped operation in progress is not an actual error
            if(::GetLastError() != ERROR_IO_PENDING)
                bRet = FALSE;
            else
            {
                // Wait for the overlapped operation to complete
                switch (::WaitForSingleObject(m_WaitOvlp_Voice.hEvent,dwTimeout))
                {
                case WAIT_OBJECT_0:
                    if(!::GetOverlappedResult(m_hComm_Voice, &m_WaitOvlp_Voice, &dwTrans, FALSE))
                        bRet = FALSE;
                    else
                        bRet = TRUE;
                    break;
        
                case WAIT_TIMEOUT:
                    // Cancel the I/O operation
                    ::CancelIo(m_hComm_Voice);                
                default:
                    bRet = FALSE;
                }
            }
        }
        ::CloseHandle(m_WaitOvlp_Voice.hEvent);
    }

    return bRet;
}

BOOL CSerialPort_Voice::Purge_Voice(DWORD dwFlags)
{
    ASSERT(IsOpen_Voice());
    return ::PurgeComm(m_hComm_Voice, dwFlags);
}

BOOL CSerialPort_Voice::IoControl_Voice(DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, DWORD dwTimeout)
{
    ASSERT(IsOpen_Voice());

    BOOL bRet = FALSE;

    //同步控制
    if(!m_bOverlapped_Voice) 
    {
        if(!::DeviceIoControl(m_hComm_Voice,
                              dwIoControlCode,
                              lpInBuffer,
                              nInBufferSize,
                              lpOutBuffer,
                              nOutBufferSize,
                              lpBytesReturned,
                              0))
        bRet = FALSE;
    }
    else //异步控制
    {
        // Wait for the event to happen
        memset(&m_IoCtrlOvlp_Voice,0,sizeof(OVERLAPPED));
        m_IoCtrlOvlp_Voice.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        
        if(!::DeviceIoControl(m_hComm_Voice,
                              dwIoControlCode,
                              lpInBuffer,
                              nInBufferSize,
                              lpOutBuffer,
                              nOutBufferSize,
                              lpBytesReturned,
                              &m_IoCtrlOvlp_Voice))    
        {
            // Overlapped operation in progress is not an actual error
            if(::GetLastError() != ERROR_IO_PENDING)
                bRet = FALSE;
            else
            {
                // Wait for the overlapped operation to complete
                switch(::WaitForSingleObject(m_IoCtrlOvlp_Voice.hEvent, dwTimeout))
                {
                case WAIT_OBJECT_0:
                    // The overlapped operation has completed
                    if(!::GetOverlappedResult(m_hComm_Voice, &m_IoCtrlOvlp_Voice, lpBytesReturned, FALSE))  
                        bRet = FALSE;
                    else
                        bRet = TRUE;
                    break; 
                case WAIT_TIMEOUT:
                    // Cancel the I/O operation
                    ::CancelIo(m_hComm_Voice);
                default:
                    bRet = FALSE;
                }
            }
        }
        ::CloseHandle(m_IoCtrlOvlp_Voice.hEvent);
    }
    
    // Return successfully
    return bRet;
}

#ifdef FEATURE_SERIAL_ASYNWRITE
UINT CSerialPort_Voice::CommWriteThreadProc_Voice(LPVOID pParam)
{
    DWORD dwWrite_Voice;
    DWORD dwEvent;
    CSerialPort_Voice *pComm = (CSerialPort_Voice*)pParam;

//  AfxMessageBox("Serial Write_Voice ThRead_Voice Start!");
    for(;;)
    {
        dwEvent = ::WaitForMultipleObjects(SERAIL_EVENTARRAYNUM, pComm->m_hEventArray_Voice, FALSE, INFINITE);
        dwEvent -= WAIT_OBJECT_0;

        switch(dwEvent) {
        case SERAIL_WRITEEVENT:
            pComm->Purge_Voice(PURGE_TXCLEAR);
            dwWrite_Voice = 0;
#ifdef FEATURE_SERIAL_QUEUE    
            CSerialBuffer *buf;
            while(buf = pComm->m_WriteBufQueue_Voice.GetFromHead())
            {
                dwWrite_Voice = 0;                
                dwWrite_Voice = pComm->Write_Voice(buf->m_szBuffer, buf->m_dwBytes);
                delete buf;
            }
#else
            EnterCriticalSection(&pComm->m_csTxBuff_Voice);
            dwWrite_Voice = pComm->Write_Voice(pComm->m_TxBuff_Voice, pComm->m_wTxCount_Voice);
            LeaveCriticalSection(&pComm->m_csTxBuff_Voice);
#endif
            break;

        case SERAIL_IOCTRLEVENT:
//            pComm->IoControl_Voice();
            break;
        case SERAIL_CLOSEEVENT:
            pComm->m_WriteThread_Voice = NULL;
            AfxEndThread(0);
            break;
        }
    }
    return 0;
}
#endif

//正如在《Serial communications in Microsoft Win32》等文章中提到的，同步（NonOverLapped）方式是
//比较简单的一种方式,，编写起来代码的长度要明显少于异步（OverLapped）方式，我开始用同步方式编写
//了整个子程序，在 Windows98 下工作正常，但后来在 Windows2000下测试，发现接收正常，但一发送数
//据，程序就会停在那里，原因应该在于同步方式下如果有一个通讯 Api 在操作中，另一个会阻塞直到上一
//个操作完成，所以当读数据的线程停留在 WaitCommEvent 的时候，WriteFile 就停在那里。我又测试了我
//手上所有有关串行通讯的例子程序，发现所有使用同步方式的程序在 Windows 2000 下全部工作不正常，
//对这个问题我一直找不到解决的办法，后来在 Iczelion 站点上发现一篇文章提到 NT 下对串行通讯的处
//理和 9x 有些不同，根本不要指望在 NT 或 Windows 2000 下用同步方式同时收发数据，我只好又用异步
//方式把整个通讯子程序重新写了一遍。
BOOL CSerialPort_Voice::StartPort_Voice(LPCTSTR lpszCommName, BOOL bCreateThread, BOOL bOverlapped)
{
    //注意: 强制设置为异步(重叠)I/O方式
    bOverlapped = TRUE;

#ifndef FEATURE_SERIAL_QUEUE
    EnterCriticalSection(&m_csRxQueue_Voice);    
    memset(m_RxQueueCtrl_Voice.RxBuffer, 0, SERIAL_RXBUFFERSIZE);
    m_RxQueueCtrl_Voice.pRead = m_RxQueueCtrl_Voice.pWrite = &m_RxQueueCtrl_Voice.RxBuffer[0];
    m_RxQueueCtrl_Voice.wRxCount = 0;
    LeaveCriticalSection(&m_csRxQueue_Voice);

#ifdef FEATURE_SERIAL_ASYNWRITE
    EnterCriticalSection(&m_csTxBuff_Voice);    
    memset(m_TxBuff_Voice, 0, SERIAL_TXBUFFERSIZE);
    m_wTxCount_Voice = 0;
    LeaveCriticalSection(&m_csTxBuff_Voice);
#endif
#endif
    
    if(Open_Voice(lpszCommName, bOverlapped) && Config_Voice())
    {
        m_bIsConnect_Voice = TRUE;      

        if(bCreateThread)
        {
            m_ReadThread_Voice = AfxBeginThread(CommReadThreadProc_Voice, this);
            m_DetectThread_Voice = AfxBeginThread(CommDetectThreadProc_Voice, this);
#ifdef FEATURE_SERIAL_ASYNWRITE
            m_WriteThread_Voice = AfxBeginThread(CommWriteThreadProc_Voice, this);
#endif
        }

        m_SerialState_Voice = SERIAL_STATE_CMD;        
        return TRUE;
    }
    else
    {
        m_SerialState_Voice = SERIAL_STATE_EXCEPTION;        
        return FALSE;        
    }
}

BOOL CSerialPort_Voice::StopPort_Voice()
{
#ifdef FEATURE_SERIAL_ASYNWRITE
    if(m_WriteThread_Voice)
    {
        if(m_hCloseWriteEvent_Voice)
            ::SetEvent(m_hCloseWriteEvent_Voice);
        m_WriteThread_Voice = NULL;
    }
#endif

#if 0
    if(m_ReadThread_Voice && m_readThread_Voice->m_hThread_Voice)
    {
        ::TerminateThread_Voice(m_ReadThread_Voice->m_hThread_Voice, 0);
        m_ReadThread_Voice = NULL;
    }
#endif

#ifdef FEATURE_SERIAL_QUEUE        
    m_ReadBufQueue_Voice.Clear();
    m_WriteBufQueue_Voice.Clear();
#endif

    Close_Voice();
    m_SerialState_Voice = SERIAL_STATE_INI;    
    return TRUE;
}

CHSDPADlg *pSpMainDlg_Voice = NULL;

BOOL CSerialPort_Voice::WriteToPort_Voice(const char *pBuf, WORD wLen, BOOL bReportError)
{
    ASSERT(pBuf != NULL && wLen < SERIAL_TXBUFFERSIZE);

    if(m_SerialState_Voice != SERIAL_STATE_CMD)
    {
        if(pSpMainDlg_Voice)
        {
            BOOL bSyncInitMask;
            EnterCriticalSection(&pSpMainDlg_Voice->m_csSyncInitMask);
            bSyncInitMask = pSpMainDlg_Voice->m_bSyncInitMask; //added by wk
            LeaveCriticalSection(&pSpMainDlg_Voice->m_csSyncInitMask);
            if(!bSyncInitMask && bReportError)
            {
                if(m_SerialState_Voice == SERIAL_STATE_INI)
                    AfxMessageBox(IDS_USB_PORT_INI);
                else if(m_SerialState_Voice == SERIAL_STATE_EXCEPTION)
                    AfxMessageBox(IDS_USB_PORT_EXCEPTION);
                else if(m_SerialState_Voice == SERIAL_STATE_CMD_WAIT)
                    AfxMessageBox(IDS_USB_PORT_WAIT);
                else if(m_SerialState_Voice == SERIAL_STATE_DATA)
                    AfxMessageBox(IDS_USB_PORT_DATA);
                else
                    AfxMessageBox(IDS_USB_PORT_WAIT);
            }  
        }
        return FALSE;
    }

#ifdef FEATURE_SERIAL_ASYNWRITE
#ifdef FEATURE_SERIAL_QUEUE    
    CSerialBuffer *pWriteBuf_Voice = new CSerialBuffer;
    pWriteBuf_Voice->m_dwBytes = wLen;
    memcpy(pWriteBuf_Voice->m_szBuffer, pBuf, wLen);
    m_WriteBufQueue_Voice.AddToTail(pWriteBuf_Voice);
#else
    EnterCriticalSection(&m_csTxBuff_Voice);
    m_wTxCount_Voice = wLen;
    memcpy(m_TxBuff_Voice, pBuf, wLen);
    LeaveCriticalSection(&m_csTxBuff_Voice);
#endif
    ASSERT(m_WriteThread_Voice != NULL);
    ::SetEvent(m_hWriteEvent_Voice);
    m_SerialState_Voice = SERIAL_STATE_CMD_WAIT;
#else
    DWORD dwWrite_Voice = 0;
    Purge_Voice(PURGE_TXCLEAR);
    m_SerialState_Voice = SERIAL_STATE_CMD_WAIT;
    dwWrite_Voice = Write_Voice(pBuf, wLen);
    if(dwWrite_Voice != wLen)
    {
        m_SerialState_Voice = SERIAL_STATE_CMD;
        return FALSE;
    }
#endif

#ifdef _DEBUG
    memset(&gStAtMsg, 0, sizeof(gStAtMsg));
    memcpy(gStAtMsg.snd, pBuf, wLen);
#endif

#ifdef FEATURE_ATTEST_SUPPORT
	/*added by taolinling start on 2008.3.19*/
	//输入，out
	CStdioFile file;
	if(file.Open("AtDebug.log", CFile::modeReadWrite))
	{
		CString str;
		str.Format("%s", &pBuf[0]);
		str.Insert(str.GetLength(),"\n");
		DWORD dwActual = file.SeekToEnd();
		file.WriteString(str);
		file.Close();		
	}
	/*added by taolinling end on 2008.3.19*/
#endif
    return TRUE;
}

UINT CSerialPort_Voice::CommReadThreadProc_Voice(LPVOID pParam)
{
	CSerialPort_Voice *pComm = (CSerialPort_Voice*)pParam;
    DWORD   dwErrorFlags;
    COMSTAT comStat;
    pComm->Purge_Voice(PURGE_RXCLEAR);
    //注册监听的事件
    ::SetCommMask(pComm->m_hComm_Voice, EV_RXCHAR | EV_ERR);
    
    while(pComm->IsConnect_Voice()) 
    {
        if(pComm->WaitEvent_Voice())
        {    
            if(pComm->m_dwEvtMask_Voice & EV_RXCHAR)
            {                
				DWORD dwReadBytes_Voice = 0;			
                do    //读缓冲区有数据
                {                    
                    BYTE WorkBuff[SERIAL_RXBUFFERSIZE];
                    memset(WorkBuff, 0, SERIAL_RXBUFFERSIZE);
                    if((dwReadBytes_Voice = pComm->Read_Voice(WorkBuff, 1)) > 0)
                    {
                        DWORD wCopyBytes = 0;
                        BYTE *ptr = WorkBuff;                   
                        EnterCriticalSection(&pComm->m_csRxQueue_Voice);
                        WORD wRxCount = pComm->m_RxQueueCtrl_Voice.wRxCount;
                        wCopyBytes = min(dwReadBytes_Voice, (WORD)(SERIAL_RXBUFFERSIZE - pComm->m_RxQueueCtrl_Voice.wRxCount));
                        while(wCopyBytes-- > 0)
                        {
                            *pComm->m_RxQueueCtrl_Voice.pWrite++ = *ptr++;
                            if(pComm->m_RxQueueCtrl_Voice.pWrite >= &pComm->m_RxQueueCtrl_Voice.RxBuffer[SERIAL_RXBUFFERSIZE])
                                pComm->m_RxQueueCtrl_Voice.pWrite = &pComm->m_RxQueueCtrl_Voice.RxBuffer[0];
                            pComm->m_RxQueueCtrl_Voice.wRxCount++;
                        }  
                        LeaveCriticalSection(&pComm->m_csRxQueue_Voice);
                        ::SetEvent(g_AtRespEvent);
                    }  
                }while(dwReadBytes_Voice > 0);
//Debug
#ifdef _DEBUG
#if 0
                CStdioFile file;
                file.Open("CommTest.txt", CFile::modeCreate | CFile::modeWrite_Voice);
                file.WriteString((LPCTSTR)pComm->m_RxQueueCtrl_Voice.RxBuffer);
                file.Close();
#endif
#endif
            }
            else if(pComm->m_dwEvtMask_Voice & EV_ERR)
            {
                pComm->Purge_Voice(PURGE_RXCLEAR);
                ::ClearCommError(pComm->m_hComm_Voice, &dwErrorFlags, &comStat);
            }
        }
    }
    pComm->m_ReadThread_Voice = NULL;
    return 0;
}

UINT CSerialPort_Voice::CommDetectThreadProc_Voice(LPVOID pParam)
{
    CSerialPort_Voice *pComm = (CSerialPort_Voice*)pParam;
    ULONG        event = 0, junk = 0;
    
 //   AfxMessageBox("DECT");
    
    if(!pComm->IsConnect_Voice())
        return 0;
    if(pComm->IoControl_Voice(IOCTL_QCOMSER_WAIT_NOTIFY_CODE,
                       NULL,
                       0,
                       &event,
                       sizeof(event),
                       &junk))
    {  
//    AfxMessageBox("disc");
        if(!pComm->IsConnect_Voice())
            return 0;
        
        if(event & 0x01)
        {
            CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
            ASSERT(pMainDlg);
         
#ifdef FEATURE_AUTOQUIT
//			AfxMessageBox(IDS_CARD_DISCONN);
			CHSDPADlg* pMainDlgTemp = (CHSDPADlg*)(theApp.GetMainWnd());
			PreMsg *PreMsgDlgTemp = NULL;
			
			if (!pMainDlgTemp->m_bPortStatus)
			{
				
				if (NULL == PreMsgDlgTemp)
				{
					CString strDspInfo;
					PreMsg *PreMsgDlg = new PreMsg;
					ASSERT(PreMsgDlg);
					PreMsgDlg->Create(PreMsgDlg->IDD);
					//PreMsgDlg->SetBitmap(IDB_SPLASH,255,0,255);
					PreMsgDlg->SetBitmap(g_SetData.Main_szSplashBmp, 255 ,0 ,255);
					PreMsgDlg->ShowWindow(SW_SHOW);
					strDspInfo.LoadString(IDS_CARD_DISCONN);
					PreMsgDlg->SetText(strDspInfo);
					Sleep(2000);
					PreMsgDlg->DestroyWindow();
					delete PreMsgDlg;
					pMainDlgTemp->PostMessage(WM_QUIT);
				}
				else
				{
					/*
					CString strDspInfo;
					strDspInfo.LoadString(IDS_CARD_DISCONN);
					PreMsgDlgTemp->SetText(strDspInfo);
					Sleep(2000);
					*/
					exit(0);
				}				
			}
			else
			{
				pMainDlgTemp->m_bPortStatus = false;
			}
#else
            pComm->StopPort_Voice();
            pComm->m_DetectThread_Voice = NULL;
            pMainDlg->m_bInComSms = FALSE;
            pMainDlg->m_bInComCall = FALSE;
            if(g_BGPassEvt)
                ::SetEvent(g_BGPassEvt);
            pMainDlg->ResetSyncData();
            pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_RSSI, 6);
            pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NETWORK, NW_TYPE_NOSRV);
            pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_ROAM, ROAM_OFF);
            pMainDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 0);
		    (theApp.GetMainWnd())->SetTimer(IDT_USB_CONN_DETECT, 1000, NULL);
#endif
        }
    }
    return 0;
}

//打印缓冲区字节数
DWORD CSerialPort_Voice::PrintBufBytes_Voice(int type) //0: 读缓冲，1: 写缓冲
{
    DWORD   dwErrorFlags;
    COMSTAT comStat;    

    CString str;
    DWORD   dwBytes;

    ::ClearCommError(m_hComm_Voice, &dwErrorFlags, &comStat);    

    if(type == 0)
    {
        str.Format(_T("Read_VoiceBuf: %d"), comStat.cbInQue);
        dwBytes = comStat.cbInQue;
    }
    else
    {
        str.Format(_T("Write_VoiceBuf: %d"), comStat.cbOutQue);
        dwBytes = comStat.cbOutQue;
    }
    AfxMessageBox(str);
    return dwBytes;
}

BOOL CSerialPort_Voice::Config_Voice()
{
    ASSERT(IsOpen_Voice());

    COMMTIMEOUTS TimeOuts;

    ::SetupComm(m_hComm_Voice, 1200, 1200);
    
    TimeOuts.ReadIntervalTimeout = MAXDWORD;
    TimeOuts.ReadTotalTimeoutMultiplier = 0;
    TimeOuts.ReadTotalTimeoutConstant = 1000;
    TimeOuts.WriteTotalTimeoutMultiplier = 0;
    TimeOuts.WriteTotalTimeoutConstant = 1000;
    ::SetCommTimeouts(m_hComm_Voice, &TimeOuts);

    DCB dcb;    
    dcb.DCBlength = sizeof(DCB);
    
    if(!::GetCommState(m_hComm_Voice, &dcb))
        return FALSE;

    dcb.BaudRate = CBR_19200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    
    return ::SetCommState(m_hComm_Voice, &dcb);
}

BOOL CSerialPort_Voice::IsConnect_Voice() const
{
    return m_bIsConnect_Voice;
}
EnSerialState CSerialPort_Voice::GetSerialState_Voice() const
{
    return m_SerialState_Voice;
}

EnSerialState CSerialPort_Voice::SetSerialState_Voice(const EnSerialState state)
{
    ASSERT(state >= SERIAL_STATE_INI && state < SERIAL_STATE_MAX);

    EnSerialState oldState = m_SerialState_Voice;
    m_SerialState_Voice = state;
    return oldState;
}

BOOL CSerialPort_Voice::CommIsReady_Voice()
{   
    EnSerialState state;

    if((state = GetSerialState_Voice()) != SERIAL_STATE_CMD)
    {
        if(state == SERIAL_STATE_INI)
            AfxMessageBox(IDS_USB_PORT_INI);
        else if(state == SERIAL_STATE_EXCEPTION)
            AfxMessageBox(IDS_USB_PORT_EXCEPTION);
//         else if(state == SERIAL_STATE_CMD_WAIT)
//             AfxMessageBox(IDS_USB_PORT_WAIT);
        else if(state == SERIAL_STATE_DATA)
            AfxMessageBox(IDS_USB_PORT_DATA);
//         else
//             AfxMessageBox(IDS_USB_PORT_WAIT);
        return FALSE;
    }
    else
        return TRUE;
}






















