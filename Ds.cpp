#include "stdafx.h"
#include "Ds.h"
#include "HSDPADlg.h"
#include "HSDPA.h"

#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

const EnDsatResMode gc_dsatmode = DSAT_MODE_STRING;

const char gc_dsatResCodeTbl[DSAT_MAX][DSAT_MODE_MAX][30] =
{
#ifndef FEATURE_ATAMOI 
    "0", "OK",
    "4", "ERROR",
    "2", "RING",
    "3", "NO CARRIER",
    "6", "NO DIALTONE",
    "+CME ERROR:", "+CME ERROR:",
    "+CMS ERROR:", "+CMS ERROR:",
	"+CMS ERROR: 500", "+CMS ERROR: 500",
    "+CMTI: ", "+CMTI: ", 
#ifdef FEATURE_HAIER_SMS
    "^HCMT:", "^HCMT:", 
#else
    "+CMT: ", "+CMT: ", 
#endif
	"+RVMFB: ","+RVMFB: ",//add by liub
	"+RVMFBUPDATE: ","+RVMFBUPDATE: ",//TATA
#ifdef FEATURE_HAIER_CM
	"^HRSSILVL:", "^HRSSILVL:",
#else
    "+RSSI: ", "+RSSI: ",
#endif
    "+CSMSS:", "+CSMSS:", 
    "+CPBSS:", "+CPBSS:", 
#ifdef FEATURE_HAIER_CM
    "+CLIP:", "+CLIP:",
	"^MODE:", "^MODE:",
#else
    "+CLIP: ", "+CLIP: ",
    "Network status: ", "Network status: ",
#endif
    "Network Service: ", "Network Service: ",
    "Roaming status: ", "Roaming status: ",
    "HANDSET_NOTIFY: ", "HANDSET_NOTIFY: ",
    "SIM_READY: ", "SIM_READY: ", 
    "Current PLMN: ", "Current PLMN: ", 
    "Srv status: ", "Srv status: ", 
    "+CDS: ", "+CDS: ", 
    "+CUSD: ", "+CUSD: ", 
	"+QCPDSI:", "+QCPDSI:", 
#else
    "0", "OK",
    "4", "ERROR",
    "+CME ERROR:", "+CME ERROR:",
    "+CMS ERROR:", "+CMS ERROR:",
    "+CMS ERROR: 500", "+CMS ERROR: 500",
    "AT_CMD_RESP_COMP_IND", "AT_CMD_RESP_COMP_IND",
    "2", "RING",
    "3", "NO CARRIER",
    "6", "NO DIALTONE",
    "+CMTI: ", "+CMTI: ", 
    "+CMT: ", "+CMT: ", 
	"+RVMFB: ","+RVMFB: ",//add by liub
	"+RVMFBUPDATE: ","+RVMFBUPDATE: ",//TATA
    "+RSSI: ", "+RSSI: ",
    "+CSMSS:", "+CSMSS:", 
    "+CPBSS:", "+CPBSS:", 
    "+CLIP: ", "+CLIP: ",
    "Network status: ", "Network status: ",
    "Network Service: ", "Network Service: ",
    "Roaming status: ", "Roaming status: ",
    "HANDSET_NOTIFY: ", "HANDSET_NOTIFY: ",
    "SIM_READY: ", "SIM_READY: ", 
    "Current PLMN: ", "Current PLMN: ", 
    "Srv status: ", "Srv status: ", 
    "+CDS: ", "+CDS: ", 
    "+CUSD: ", "+CUSD: ", 
	"+QCPDSI:", "+QCPDSI:", 
#endif
	"+HVPRIV", "+HVPRIV", 
	"+SIDLOCK: ", "+SIDLOCK: ", 
#ifdef FEATURE_HAIER_CM
	"+CEND:", "+CEND:",
	"^CEND:", "^CEND",
	"+CFNM:", "+CFNM:",
#endif
};

static StAtResp g_AtRespArr[ATRESP_MAX];
static HANDLE g_DsEventArr[DSEVENT_ARRNUM];

HANDLE g_AtRespEvent; //用于DS和串口字符传送同步
HANDLE g_EndEvent;  //结束DS任务

//用于APP与DS运行时注册普通命令响应同步
HANDLE g_AtRegEvt; 

//用于APP与DS启动时注册主动消息响应同步
HANDLE g_AppRegEvt; 

//用于APP普通命令超时处理
HANDLE g_AtTimeout; 

//解析结果字符串缓冲
static BYTE g_DsatStrArr[DSAT_STRING_ROW][DSAT_STRING_COL];
//解析结果字符串个数
static WORD g_DsatStrNum;
//解析结果指针
static BYTE *g_DsatPtr = &g_DsatStrArr[0][0];
//当前解析状态
static EnDsatState g_DsatState = STATE_START;
//解析得到的结果码
static EnDsatResCode g_DsatResCode = DSAT_MAX;


HANDLE g_BGPassEvt;
HANDLE g_BGReadNewSmsEvt;
HANDLE g_BGEvtArr[BGEVT_ARRNUM];

StAtResp BGSmsResp;
StAtResp BGCallResp;
StAtResp BGClipResp;

//解析结果字符串缓冲
static BYTE g_BGSmsStrArr[BG_STRING_ROW][BG_STRING_COL];
static BYTE g_BGCallStrArr[BG_STRING_ROW][BG_STRING_COL];
static BYTE g_BGClipStrArr[BG_STRING_ROW][BG_STRING_COL];

//解析结果字符串个数
static WORD g_BGSmsStrNum;
static WORD g_BGCallStrNum;
static WORD g_BGClipStrNum;

//短消息CMTI通知缓存队列
static StBGSmsQueue g_BGSmsQueue;

void BGEvtSms(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    ASSERT(wStrNum <= BG_STRING_ROW);

    if(wStrNum > BG_STRING_ROW)
    {

        return;
    }
  
    ::EnterCriticalSection(&g_BGSmsQueue.cs);
    if(g_BGSmsQueue.wRxCount < BG_SMS_ARRNUM)
    {
        g_BGSmsQueue.StrNum[g_BGSmsQueue.nWriteIndex] = wStrNum;
        for(WORD cnt = 0; cnt < wStrNum; cnt++)
            strncpy((char*)g_BGSmsQueue.StrArr[g_BGSmsQueue.nWriteIndex][cnt], (char*)strArr[cnt], DSAT_STRING_COL);
        g_BGSmsQueue.nWriteIndex++;
        if(g_BGSmsQueue.nWriteIndex >= BG_SMS_ARRNUM)
            g_BGSmsQueue.nWriteIndex = 0;
        g_BGSmsQueue.wRxCount++;
    }
    ::LeaveCriticalSection(&g_BGSmsQueue.cs);

    ::SetEvent(g_BGEvtArr[BGEVT_SMS]);
}

void BGEvtCall(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    ASSERT(wStrNum <= BG_STRING_ROW);

    if(wStrNum > BG_STRING_ROW)
        return;

    g_BGCallStrNum = 0;
    memset(g_BGCallStrArr, 0x00, sizeof(g_BGCallStrArr));

    g_BGCallStrNum = wStrNum;
    for(WORD cnt = 0; cnt < wStrNum; cnt++)
        strcpy((char*)g_BGCallStrArr[cnt], (char*)strArr[cnt]);

    ::SetEvent(g_BGEvtArr[BGEVT_CALL]);
}

void BGEvtClip(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    ASSERT(wStrNum <= BG_STRING_ROW);

    if(wStrNum > BG_STRING_ROW)
        return;
 
    g_BGClipStrNum = 0;
    memset(g_BGClipStrArr, 0x00, sizeof(g_BGClipStrArr));

    g_BGClipStrNum = wStrNum;
    for(WORD cnt = 0; cnt < wStrNum; cnt++)
        strcpy((char*)g_BGClipStrArr[cnt], (char*)strArr[cnt]);

    ::SetEvent(g_BGEvtArr[BGEVT_CLIP]);
}

//处理来电，来短信事件的后台线程
UINT BGThreadProc(LPVOID pParam)
{  
    CHSDPADlg *pMainDlg = (CHSDPADlg*)pParam;
    ASSERT(pMainDlg);

    memset(&g_BGSmsQueue, 0x00, sizeof(StBGSmsQueue));
    ::InitializeCriticalSection(&g_BGSmsQueue.cs);

    g_BGEvtArr[BGEVT_SMS]   = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_BGEvtArr[BGEVT_CALL]  = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_BGEvtArr[BGEVT_CLIP]  = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_BGEvtArr[BGEVT_END]   = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_BGPassEvt = ::CreateEvent(NULL, TRUE, TRUE, NULL);
    g_BGReadNewSmsEvt = ::CreateEvent(NULL, FALSE, TRUE, NULL);

    DWORD dwEvent;
    while(true)
    {
        dwEvent = ::WaitForMultipleObjects(BGEVT_ARRNUM, g_BGEvtArr, FALSE, INFINITE);
        dwEvent -= WAIT_OBJECT_0;

        switch(dwEvent) {
        case BGEVT_SMS:
            //pMainDlg->m_bInComSms = TRUE;
            ::WaitForSingleObject(g_BGPassEvt, INFINITE);
            
            while(true)
            {
                g_BGSmsStrNum = 0;
                memset(g_BGSmsStrArr, 0x00, sizeof(g_BGSmsStrArr));
                
                ::EnterCriticalSection(&g_BGSmsQueue.cs);
                if(g_BGSmsQueue.wRxCount > 0)
                {
                    g_BGSmsStrNum = g_BGSmsQueue.StrNum[g_BGSmsQueue.nReadIndex];
                    for(WORD cnt = 0; cnt < g_BGSmsStrNum; cnt++)
                        strncpy((char*)g_BGSmsStrArr[cnt], (char*)g_BGSmsQueue.StrArr[g_BGSmsQueue.nReadIndex][cnt], DSAT_STRING_COL);
                    g_BGSmsQueue.nReadIndex++;
                    if(g_BGSmsQueue.nReadIndex >= BG_SMS_ARRNUM)
                        g_BGSmsQueue.nReadIndex = 0;
                    g_BGSmsQueue.wRxCount--;
                    ::LeaveCriticalSection(&g_BGSmsQueue.cs);
                    //Sleep(100);
                    (BGSmsResp.m_AtRespFunc)(BGSmsResp.m_pWnd, g_BGSmsStrArr, g_BGSmsStrNum);
                }
                else
                {
                    ::LeaveCriticalSection(&g_BGSmsQueue.cs);
                    break;
                }
            }
            break;
        case BGEVT_CALL:
            pMainDlg->m_bInComCall = TRUE;
            if(WAIT_OBJECT_0 == ::WaitForSingleObject(g_BGPassEvt, INFINITE))
                (BGCallResp.m_AtRespFunc)(BGCallResp.m_pWnd, g_BGCallStrArr, g_BGCallStrNum);
            break;
        case BGEVT_CLIP:
            pMainDlg->m_bInComCall = TRUE;
            if(WAIT_OBJECT_0 == ::WaitForSingleObject(g_BGPassEvt, INFINITE))
                (BGClipResp.m_AtRespFunc)(BGClipResp.m_pWnd, g_BGClipStrArr, g_BGClipStrNum);
            break;
        case BGEVT_END:
            for(BYTE i=BGEVT_SMS; i<BGEVT_ARRNUM;i++)
                ::CloseHandle(g_BGEvtArr[i]);
            ::CloseHandle(g_BGPassEvt);
            ::CloseHandle(g_BGReadNewSmsEvt);
            ::DeleteCriticalSection(&g_BGSmsQueue.cs);
            AfxEndThread(0);
            break;
        }
    }

    return 0;
}

//注册AT响应处理函数
#if 0
void RegisterAtRespFunc(EnAtRespFuncType type, AtRespFunc func, CWnd* pWnd)
{
    ASSERT(type >= ATRESP_RING && type < ATRESP_MAX);
    ASSERT(func != NULL && pWnd != NULL);

    g_AtRespArr[type].m_AtRespFunc = func;
    g_AtRespArr[type].m_pWnd = pWnd;

    if(type == ATRESP_GENERAL_AT)
        ::SetEvent(g_AtRegEvt);
}
#else
void RegisterAtRespFunc(EnAtRespFuncType type, AtRespFunc func, LPVOID pWnd)
{
    ASSERT(type >= ATRESP_RING && type < ATRESP_MAX);
    ASSERT(func != NULL && pWnd != NULL);

    if(type == ATRESP_RING)
    {
        BGCallResp.m_AtRespFunc = func;
        BGCallResp.m_pWnd = pWnd;
        g_AtRespArr[type].m_AtRespFunc = BGEvtCall;
        g_AtRespArr[type].m_pWnd = NULL;
    }
    else if(type == ATRESP_CLIP)
    {
        BGClipResp.m_AtRespFunc = func;
        BGClipResp.m_pWnd = pWnd;
        g_AtRespArr[type].m_AtRespFunc = BGEvtClip;
        g_AtRespArr[type].m_pWnd = NULL;
    }
    else if(type == ATRESP_CMTI)
    {
        BGSmsResp.m_AtRespFunc = func;
        BGSmsResp.m_pWnd = pWnd;
        g_AtRespArr[type].m_AtRespFunc = BGEvtSms;
        g_AtRespArr[type].m_pWnd = NULL;
    }
    else
    {
        g_AtRespArr[type].m_AtRespFunc = func;
        g_AtRespArr[type].m_pWnd = pWnd;
    }
	
    if(type == ATRESP_GENERAL_AT)
        ::SetEvent(g_AtRegEvt);
}
#endif

//注销AT响应处理函数
void DeRegisterAtRespFunc(EnAtRespFuncType type)
{
    ASSERT(type >= ATRESP_RING && type <= ATRESP_MAX);

    if(type == ATRESP_GENERAL_AT)
        ::ResetEvent(g_AtRegEvt);

    if(type == ATRESP_MAX)
        memset(g_AtRespArr, 0, sizeof(g_AtRespArr));
    else
        memset(&g_AtRespArr[type], 0, sizeof(StAtResp));
}

//AT超时通知函数
void AtTimeoutNotify()
{
    ::SetEvent(g_AtTimeout);
    AfxMessageBox(IDS_USB_PORT_TIMEOUT);
}

CHSDPADlg *pDsMainDlg = NULL;

//执行AT响应处理函数
static void CallAtRespFunc(EnAtRespFuncType type)
{
    ASSERT(type >= ATRESP_RING && type < ATRESP_MAX); 
    
    if(type == ATRESP_GENERAL_AT)
    {
        if(WAIT_OBJECT_0 != ::WaitForSingleObject(g_AtRegEvt, 30000))
        {
            ASSERT(FALSE);
        }
    }

//Debug
#if _DEBUG
#if 0
    char msg[512];
    memset(msg, 0, 512);
    for(int i = 0; i < g_DsatStrNum; i++)
    {
        strcat(msg, (char *)g_DsatStrArr[i]);
        strcat(msg, "\n");
    }
    AfxMessageBox(msg);
#endif
#endif

//    ASSERT(g_AtRespArr[type].m_AtRespFunc != NULL && g_AtRespArr[type].m_pWnd != NULL);
    ASSERT(g_AtRespArr[type].m_AtRespFunc != NULL);

    if(g_AtRespArr[type].m_AtRespFunc == NULL)
        return;

    if(type != ATRESP_GENERAL_AT)
    {
        //下位机在上位机初始化阶段不主动发送RSSI,NETWORK,ROAM变化通知
#if 0
        if(type == ATRESP_RSSI || type == ATRESP_NWCHG || type == ATRESP_ROAMCHG
            || type == ATRESP_RING || type == ATRESP_CMTI)
        {
            if(pDsMainDlg)
            {
                BOOL bSyncInitMask;
                EnterCriticalSection(&pDsMainDlg->m_csSyncInitMask);
                bSyncInitMask = pDsMainDlg->m_bSyncInitMask;
                LeaveCriticalSection(&pDsMainDlg->m_csSyncInitMask);
                if(bSyncInitMask)
                    return;
            }
        }
#endif

        (*g_AtRespArr[type].m_AtRespFunc)(g_AtRespArr[type].m_pWnd, g_DsatStrArr, g_DsatStrNum);
    }
    else
    {
        StAtResp lastAtResp = g_AtRespArr[type];
        DeRegisterAtRespFunc(type);
        (*lastAtResp.m_AtRespFunc)(lastAtResp.m_pWnd,g_DsatStrArr, g_DsatStrNum);
    }
}

//判断是否找到了AT响应的结果码
static EnDsatResCode FindEndCode(const char *str)
{
    ASSERT(str != NULL);

    int i;
    EnDsatResCode begCode;
    EnDsatResCode divCode;

#ifndef FEATURE_ATAMOI
    begCode = DSAT_OK;
    divCode = DSAT_CME_ERROR;
#else
    begCode = DSAT_ATAMOI;
    divCode = DSAT_CMTI;
#endif

    for(i = begCode; i < divCode; i++)
    {
        if(strcmp(str, gc_dsatResCodeTbl[i][gc_dsatmode]) == 0)
            return (EnDsatResCode)(i);
    }
    for(; i < DSAT_MAX; i++)
    {
        if(memcmp(str, gc_dsatResCodeTbl[i][gc_dsatmode], strlen(gc_dsatResCodeTbl[i][gc_dsatmode])) == 0)
            return (EnDsatResCode)(i);        
    }

    return DSAT_MAX;
}

//重置解析缓冲和相关变量
static void ResetParseData()
{
    memset(g_DsatStrArr, 0, sizeof(g_DsatStrArr));
    g_DsatStrNum = 0;
    g_DsatPtr = &g_DsatStrArr[0][0];    
    g_DsatState = STATE_START;
    g_DsatResCode = DSAT_MAX;
}

//AT响应解析函数
static void AtRespParse(CSerialPort *pComm) 
{
    ASSERT(pComm != NULL);

#ifdef FEATURE_ATTEST_SUPPORT
	//输出，in
	CStdioFile file;
	if(file.Open("AtDebug.log", CFile::modeReadWrite))
	{
		CString str;
		str.Format("%s", (char *)&pComm->m_RxQueueCtrl.RxBuffer[0]);
		str.Insert(str.GetLength(),"\n");
		DWORD dwActual = file.SeekToEnd();
		file.WriteString(str);
		file.Close();		
	}
#endif

    BYTE ch = 0;
    while(true)
    {
        EnterCriticalSection(&pComm->m_csRxQueue);
        if(pComm->m_RxQueueCtrl.wRxCount > 0)
        {
            ch = *pComm->m_RxQueueCtrl.pRead++;
            if(pComm->m_RxQueueCtrl.pRead >= &pComm->m_RxQueueCtrl.RxBuffer[SERIAL_RXBUFFERSIZE])
                pComm->m_RxQueueCtrl.pRead = &pComm->m_RxQueueCtrl.RxBuffer[0];
            pComm->m_RxQueueCtrl.wRxCount--;
            LeaveCriticalSection(&pComm->m_csRxQueue);    
        }
        else 
        {
            LeaveCriticalSection(&pComm->m_csRxQueue);        
            break;
        }

        ASSERT(g_DsatStrNum <= DSAT_STRING_ROW);

        switch(g_DsatState)
        {
        case STATE_START:
            if(ch == AT_FLAG_S3)
            {
                if((g_DsatResCode == DSAT_CMT) || (g_DsatResCode == DSAT_CDS))
                    g_DsatState = STATE_RESCODE_S3;
                else
                    g_DsatState = STATE_HEAD_S3;
            }
            else if(g_DsatStrNum > 0)
            {
                *g_DsatPtr++ = ch;
                g_DsatState = STATE_FIND_CONTENT;
            }
            break;
        case STATE_HEAD_S3:
            if(ch == AT_FLAG_S4)
                g_DsatState = STATE_HEAD_S4;
            else if(ch != AT_FLAG_S3) //error
                g_DsatState = STATE_START;
            break;
        case STATE_HEAD_S4:
            if(ch == AT_FLAG_MR)
            {
                *g_DsatPtr++ = ch;
                g_DsatStrNum++;
                g_DsatState = STATE_FIND_MR;
            }
            else if(ch != AT_FLAG_S3)
            {
                *g_DsatPtr++ = ch;
                g_DsatState = STATE_FIND_CONTENT;
            }
            else
            {
                //g_DsatState = STATE_TAIL_S3; 
                g_DsatState = STATE_HEAD_S3;                 
            }
            break;
        case STATE_FIND_CONTENT:
            if(ch != AT_FLAG_S3)
            {
                *g_DsatPtr++ = ch;
            }
            else
            {
                if((g_DsatResCode != DSAT_CMT) && (g_DsatResCode != DSAT_CDS))
                {
                    g_DsatResCode = FindEndCode((char *)g_DsatStrArr[g_DsatStrNum++]);
                    if(g_DsatResCode == DSAT_MAX || g_DsatResCode == DSAT_CMT || g_DsatResCode == DSAT_CDS)
                    {
                        g_DsatState = STATE_TAIL_S3;
                    }
                    else
                        g_DsatState = STATE_RESCODE_S3;
                    g_DsatPtr = &g_DsatStrArr[g_DsatStrNum][0];  
                }
                else
                {
                    g_DsatStrNum++;
                    g_DsatState = STATE_RESCODE_S3;
                }
            }
            break;
        case STATE_TAIL_S3:
            if(ch == AT_FLAG_S4)
                g_DsatState = STATE_START;
            else if(ch != AT_FLAG_S3) //error
            {
                memset(&g_DsatStrArr[--g_DsatStrNum][0], 0, (DSAT_STRING_COL));
                g_DsatPtr = &g_DsatStrArr[g_DsatStrNum][0];
                g_DsatResCode = DSAT_MAX;                
                g_DsatState = STATE_START;
            }
            break;
        case STATE_RESCODE_S3:
            if(ch == AT_FLAG_S4)
            {
                g_DsatState = STATE_END;
            }
            else if(ch != AT_FLAG_S3) //error
            {
                memset(&g_DsatStrArr[--g_DsatStrNum][0], 0, (DSAT_STRING_COL));
                g_DsatPtr = &g_DsatStrArr[g_DsatStrNum][0];
                g_DsatResCode = DSAT_MAX;
                g_DsatState = STATE_START;
            }
            break;
        case STATE_FIND_MR:
            if(ch == AT_FLAG_SP)
            {
                *g_DsatPtr++ = ch;
                g_DsatResCode = DSAT_OK;
                g_DsatState = STATE_END;
            }
            else if(ch != AT_FLAG_MR) //error
            {
                memset(&g_DsatStrArr[--g_DsatStrNum][0], 0, (DSAT_STRING_COL));
                g_DsatPtr = &g_DsatStrArr[g_DsatStrNum][0];
                g_DsatResCode = DSAT_MAX;
                g_DsatState = STATE_START;
            }
            break;
        }

        //解析到一条完整的AT命令响应，调用AT处理回调函数
        if(g_DsatState == STATE_END && g_DsatResCode != DSAT_MAX)
        {
            if(g_DsatResCode == DSAT_RING)
            {
                if(g_SetData.Main_nCall)
                    CallAtRespFunc(ATRESP_RING);
            }
            else if(g_DsatResCode == DSAT_NO_CARRIER)
            {
                CallAtRespFunc(ATRESP_NO_CARRIER);
            }
            else if(g_DsatResCode == DSAT_DIALTONE
#ifdef FEATURE_HAIER_CM
					|| g_DsatResCode == DSAT_CEND
#endif
				)
            {
                if(g_SetData.Main_nCall)
                    CallAtRespFunc(ATRESP_DIALTONE);
            }
            else if(g_DsatResCode == DSAT_CMTI)
            {
                CallAtRespFunc(ATRESP_CMTI);
            }
            else if(g_DsatResCode == DSAT_CMT)
            {
                CallAtRespFunc(ATRESP_CMT);
            }
			else if(g_DsatResCode == DSAT_RVMFB)//TATA 需求 add by liub
            {
                CallAtRespFunc(ATRESP_RVMFB);
            }
			else if(g_DsatResCode == DSAT_RVMFBUPDATE)//TATA 需求 add by liub
            {
                CallAtRespFunc(ATRESP_RVMFBUPDATA);
            }
            else if(g_DsatResCode == DSAT_RSSI)
            {
                CallAtRespFunc(ATRESP_RSSI);
            }
            else if(g_DsatResCode == DSAT_CSMSS)
            {
                CallAtRespFunc(ATRESP_CSMSS);
            }
            else if(g_DsatResCode == DSAT_CPBSS)
            {
                CallAtRespFunc(ATRESP_CPBSS);
            }
            else if(g_DsatResCode == DSAT_CLIP)
            {
                if(g_SetData.Main_nCall)
                    CallAtRespFunc(ATRESP_CLIP);
            }
            else if(g_DsatResCode == DSAT_NWCHG)
            {
                CallAtRespFunc(ATRESP_NWCHG);
            }
            else if(g_DsatResCode == DSAT_NWSRVCHG)
            {
                CallAtRespFunc(ATRESP_NWSRVCHG);
            }
            else if(g_DsatResCode == DSAT_ROAMCHG)
            {
                CallAtRespFunc(ATRESP_ROAMCHG);
            }
            else if(g_DsatResCode == DSAT_HANDSET)
            {
                CallAtRespFunc(ATRESP_HANDSETCHG);
            }
            else if(g_DsatResCode == DSAT_SIMREADY)
            {
                CallAtRespFunc(ATRESP_SIMREADY);
            }
            else if(g_DsatResCode == DSAT_PLMNCHG)
            {
                CallAtRespFunc(ATRESP_PLMNCHG);
            }
            else if(g_DsatResCode == DSAT_SRVSTATUSCHG)
            {
                CallAtRespFunc(ATRESP_SRVSTATUSCHG);
            }
            else if(g_DsatResCode == DSAT_CDS)
            {
                CallAtRespFunc(ATRESP_CDS);
            }
			else if(g_DsatResCode == DSAT_CUSD)
            {
                CallAtRespFunc(ATRESP_CUSD);
            }
			else if(g_DsatResCode == DSAT_PS)
            {
                CallAtRespFunc(ATRESP_PS);
            }
			else if(g_DsatResCode == DSAT_HVPRIV)
            {
                CallAtRespFunc(ATRESP_HVPRIV);
            }
			else if(g_DsatResCode == DSAT_SIDLOCK)
            {
                CallAtRespFunc(ATRESP_SIDLOCK);
            }
#ifdef FEATURE_HAIER_CM
			else if(g_DsatResCode == DSAT_CFNM)
            {
                //don't process +CFNM unsolicted command now
            }else if(g_DsatResCode == DSAT_HCEND){
				//don't process ^CEND unsolicted command now
			}
#endif
#ifdef FEATURE_ATAMOI
            else if(g_DsatResCode == DSAT_ATAMOI)
            {
                if(g_DsatStrNum > 1)
                    g_DsatStrNum--;
#else
            else if(g_DsatResCode == DSAT_OK || g_DsatResCode == DSAT_ERROR
                    || g_DsatResCode == DSAT_CME_ERROR || g_DsatResCode == DSAT_CMS_ERROR)
            {
#endif
                pComm->SetSerialState(SERIAL_STATE_CMD);
                CallAtRespFunc(ATRESP_GENERAL_AT);
            }
            ResetParseData();
        }
    } //while
}

//普通AT命令超时处理
static void AtTimeoutProc(CSerialPort *pComm) 
{
    ASSERT(pComm != NULL);

    pComm->Purge(PURGE_RXCLEAR);

    ::EnterCriticalSection(&pComm->m_csRxQueue);
    memset(&pComm->m_RxQueueCtrl, 0x00, sizeof(StRxQueueCtrl));
    ::LeaveCriticalSection(&pComm->m_csRxQueue);

    ResetParseData();

    DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
    pComm->SetSerialState(SERIAL_STATE_CMD);
}

void DsEventCreate()
{
    g_AtRespEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_EndEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    

    g_AtRegEvt = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    

    g_AppRegEvt = ::CreateEvent(NULL, TRUE, FALSE, NULL);

    g_AtTimeout = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    g_DsEventArr[DSEVENT_ATRESP] = g_AtRespEvent;
    g_DsEventArr[DSEVENT_ATTIMEOUT] = g_AtTimeout;
    g_DsEventArr[DSEVENT_END] = g_EndEvent;
}

void DsEventDestroy()
{
    for(BYTE i=DSEVENT_ATRESP; i<DSEVENT_ARRNUM;i++)
        ::CloseHandle(g_DsEventArr[i]);
    ::CloseHandle(g_AtRegEvt);
    ::CloseHandle(g_AppRegEvt);
}

//Ds任务
UINT DsThreadProc(LPVOID pParam)
{
    CSerialPort *pComm = (CSerialPort*)pParam;

    ResetParseData();
    DeRegisterAtRespFunc(ATRESP_MAX);

    ResetSmsRcvConcBuf(SMS_RCVCONCBUF_MAX);
    
    ::WaitForSingleObject(g_AppRegEvt, INFINITE);

    DWORD dwEvent = 0;
    while(true)
    {
        dwEvent = ::WaitForMultipleObjects(DSEVENT_ARRNUM, g_DsEventArr, FALSE, INFINITE);
        dwEvent -= WAIT_OBJECT_0;

        switch(dwEvent) {
        case DSEVENT_ATRESP:
            AtRespParse(pComm);
            break;
        case DSEVENT_ATTIMEOUT:
            AtTimeoutProc(pComm);
            break;
        case DSEVENT_END:
            DsEventDestroy();
            AfxEndThread(0);
            break;
        }
    }

    return 0;
}

//Sms At related variables and functions
EnSmsState GetSmsStateFromStr(LPCSTR str)
{
    BYTE cnt;
    for(cnt = 0; cnt < SMS_STATE_MAX; cnt++)
    {
        if(strcmp(str, gcstrSmsState[cnt][gSmsMode]) == 0)
            break;
    }
    return (EnSmsState)cnt;
}

LPCSTR GetSmsStrFromState(EnSmsState state)
{
    ASSERT(state >= SMS_STATE_MT_NOT_READ && state < SMS_STATE_MAX);
    return (gcstrSmsState[state][gSmsMode]);
}

//for gsm/wcdma format
//"年/月/日,时:分:秒" //"6/3/20,13:22:34" -cdma
//"06/05/23,09:19:55+00" -gsm/wcdma
BOOL GetTimeFromStr(LPCSTR str, COleDateTime &time)
{
    int Date[3];
    int Time[3];
    int Zone = 0;
    char *pDate, *pTime, *pZone = 0;
    char *p, *p1, *p2;
    char WorkStr[30] = {0};

    strncpy(WorkStr, str, sizeof(WorkStr));

    pDate = WorkStr; pTime = NULL;
    p = WorkStr;

    while(*p)
    {
        if(*p == ',')
        {
            *p++ = 0;
            pTime = p;
            break;
        }
        else if(*p == '+')
        {
            *p++ = 0;
            pZone = p;
            Zone = atoi(pZone);
            break;
        }
        p++;
    }

    if(pTime == NULL || *pTime == 0)
        return FALSE;

    int cnt = 0;
    p1 = p2 = pDate;
    while(cnt < 3)
    {
        if(*p2 == '/')
        {
            *p2++ = 0;
            Date[cnt++] = atoi(p1);
            p1 = p2;
        }
        else if(*p2 == 0)
        {
            Date[cnt++] = atoi(p1);
            break;        
        }
        else
            p2++;
    }

    if(cnt != 3)
        return FALSE;

    Date[0] += 2000;

    cnt = 0;
    p1 = p2 = pTime;
    while(cnt < 3)
    {
        if(*p2 == ':')
        {
            *p2++ = 0;
            Time[cnt++] = atoi(p1);
            p1 = p2;
        }
        else if(*p2 == 0)
        {
            Time[cnt++] = atoi(p1);
            break;        
        }
        else
            p2++;
    }
    
    if(cnt != 3)
        return FALSE;

    if((Date[1] >= 1 && Date[1] <= 12) && (Date[2] >= 1 && Date[2] <= 31) 
        && (Time[0] >= 0 && Time[0] <= 23) && (Time[1] >=0 && Time[1] <= 59) && (Time[2] >=0 && Time[2] <= 59))
    {
        time = COleDateTime(Date[0], Date[1], Date[2], Time[0], Time[1], Time[2]);
        return TRUE;
    }
    else
        return FALSE;
}

BOOL IsValidCallNumChar(UINT nChar)
{
    if((nChar >= '0' && nChar <= '9')
        || nChar == 8 || nChar == '*' 
        || nChar == '#' || nChar == '+')
        return TRUE;
    else
        return FALSE;
}

BOOL IsValidSmsNumChar(UINT nChar)
{
    if((nChar >= '0' && nChar <= '9')
        || nChar == 8 || nChar == 22 || nChar == 3 || nChar == ';' || nChar == '+')
        return TRUE;
    else 
        return FALSE;
}

//GB转UCS2处理函数
CString GBToUCS2(const char *strGb)
{
     ASSERT(strGb);

     CString strUcs2;
     CString strTmp;
     wchar_t* wszUnicode = NULL;    //Unicode编码的字符
     int iLen = 0;                  //需要转换的字符数
 
     if(strlen(strGb) == 0)
         return strUcs2;

     //计算转换的字符数
     iLen = MultiByteToWideChar(CP_ACP, 0, strGb, -1, NULL, 0);
 
     //给wszUnicode分配内存
     wszUnicode = new wchar_t[iLen+1];

     //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
     MultiByteToWideChar(CP_ACP, 0, strGb, -1, wszUnicode, iLen);

	 for(int i=0;i<iLen-1;i++)
	 {
        strTmp.Format(_T("%04X"), wszUnicode[i]);
        strUcs2 += strTmp;
	 }

	 //释放内存
     delete []wszUnicode;
     return strUcs2;
}

//GB转UCS2处理函数
CString BTToUCS2(const TCHAR *strGb)
{
     ASSERT(strGb);

     CString strUcs2;
    CString strTmp;
    // wchar_t* wszUnicode = NULL;    //Unicode编码的字符
     int iLen = 0;                  //需要转换的字符数
 
     if(wcslen(strGb) == 0)
         return strUcs2;

     //计算转换的字符数
   //  iLen = MultiByteToWideChar(CP_ACP, 0, strGb, -1, NULL, 0);
    iLen =wcslen(strGb);
     //给wszUnicode分配内存
   //  wszUnicode = new wchar_t[iLen+1];

	

     //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
    // MultiByteToWideChar(CP_ACP, 0, strGb, -1, wszUnicode, iLen);

	 for(int i=0;i<iLen;i++)
	 {
        strTmp.Format(_T("%04X"), strGb[i]);
        strUcs2 += strTmp;
	 }

	 //释放内存
    // delete []wszUnicode;
     return strUcs2;
}

//UCS2转UCS2处理函数
int  UCS2ToUCS2(CString strGb, CString strUcs2)
{
     ASSERT(strGb);
     
    CString strTmp;
    // wchar_t* wszUnicode = NULL;    //Unicode编码的字符
     int iLen = 0;                  //需要转换的字符数
 
     if(wcslen(strGb) == 0)
          return 0;

    iLen =wcslen(strGb);

     wchar_t* wszUnicode = new wchar_t[iLen*2+1];
     memset(wszUnicode, 0, iLen*2+1);
	

     //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
    // MultiByteToWideChar(CP_ACP, 0, strGb, -1, wszUnicode, iLen);
       int j = 0;
	for(int i = 0; i < iLen ; )
	{
		wszUnicode[j] =  (strGb[i] >> 8)& 0x00FF;
		wszUnicode[j + 1] = strGb[i] & 0x00FF;
		j +=2;
		i +=1;
			
	} 
       
	 for(int num=0 ;num<j ;num++)
	 {
        
		 strTmp.Format(_T("%x"), wszUnicode[num]);
        strUcs2 += strTmp;
	 }

	 //释放内存
     delete []wszUnicode;
     return j;
}
//UCS2转GB处理函数
CString UCS2ToGB(const CString &strUcs2)
{
     CString strGb;
     wchar_t* wszUnicode=NULL; //Unicode编码的字符
     char* pszGb=NULL; //Gb编码的繁体字符  
     int iLen=0; //需要转换的字符数     
     
	 iLen = strUcs2.GetLength()/4;
	 wszUnicode=new wchar_t[iLen+1];	 
	 CString str;
	 wchar_t wData;	 
     
	 for(int i=0;i<iLen;i++)
	 {
		str=strUcs2.Mid(i*4,4);
		swscanf(str,_T("%x"),&wData);	
		wszUnicode[i]=wData;		
	 }
	 
     wszUnicode[i]=0;
     
     //计算转换的字符数
     iLen=WideCharToMultiByte (CP_ACP,0,(PWSTR) wszUnicode, -1, NULL,0, NULL, NULL);
     
     //给pszGbt分配内存
     pszGb=new char[iLen+1];   
     
     //转换Unicode码到Gb码繁体，使用API函数WideCharToMultiByte
     WideCharToMultiByte (CP_ACP,0,(PWSTR) wszUnicode, -1, pszGb,iLen, NULL, NULL);    
	 
     strGb=pszGb;
     
     //释放内存
     delete [] wszUnicode;
     delete [] pszGb;

     return strGb;
}

#ifdef FEATURE_HAIER_SMS
char * WCharToGB(const wchar_t *wchar)
{
     char* pszGb=NULL;  
     int iLen=0; //需要转换的字符数
	 int strlen = wcslen(wchar);
	 wchar_t *wszUnicode = new wchar_t[strlen + 1];
	 memset(wszUnicode, 0, sizeof(wchar_t)*(strlen + 1));

	 WCharToUnicode(wchar, (char *)wszUnicode);

     //计算转换的字符数
     iLen=WideCharToMultiByte (CP_ACP,0, wszUnicode, -1, NULL,0, NULL, NULL);
     
     //给pszGbt分配内存
     pszGb=new char[iLen+1];   
     
     //转换Unicode码到Gb码繁体，使用API函数WideCharToMultiByte
     WideCharToMultiByte (CP_ACP,0, wszUnicode, -1, pszGb,iLen, NULL, NULL);    

     pszGb[iLen] = 0;
     
     //释放内存
     return pszGb;
}
#endif

void ASCToUCS2(const char* unicode, TCHAR* WChar)
{

	int len = strlen(unicode);
	int j = 0;
	for(int i = 0; i < len;){
		WChar[j] = unicode[i];
		WChar[j] = WChar[j]<<8;
		WChar[j] = WChar[j]|(unicode[i + 1] &0x00FF);
		i += 2;
		j += 1;
		if(unicode[i] == 0x00 && unicode[i + 1 ] == 0x00)
			break;
	}
	WChar[j] = 0;
}

BOOL ASCHEXToWchar(const char* ascii, TCHAR* WChar)
{	
	int len = strlen(ascii);
	int j = 0;
	for(int i = 0; i < len; i++)
	{
		if (i%2 == 0)
		{
			WChar[j++] = '0';
			WChar[j++] = '0';
		}
		WChar[j++] = ascii[i];
	}
	
	return TRUE;
}

int WCharToUnicode(const TCHAR *WChar, char* unicode)
{
	int j = 0;
	if(WChar == NULL){
		unicode[0] = '\0';
		return j;
	}
	int len = _tcslen(WChar);
	for(int i = 0; i < len; i ++)
	{
		unicode[j] = (char)(WChar[i] >> 8);
		unicode[j + 1] = (char)(WChar[i] & 0x00FF);
		if(unicode[j] == 0x00 && unicode[j + 1] == 0x00)
			break;
		j += 2;
	}
	return j;
}

int WCharToChar(const TCHAR *wchar, char* cstr)
{
	char unicode[2];
	if(wchar == NULL){
		memset(cstr, 0, strlen(cstr));
		return 0;
	}
	int len = _tcslen(wchar);
	for(int i = 0; i < len; i ++)
	{
		unicode[0] = (char)(wchar[i] & 0x00FF);
		unicode[1] = (char)(wchar[i] >> 8);
		if(unicode[0] == 0x00 && unicode[1] == 0x00){
			cstr[i] = 0x00;
			break;
		}
		if(unicode[1] == 0x00)
			cstr[i] = unicode[0];
	}
	return len;
}
BOOL IsGsm7bitChar(const wchar_t uc)
{
    int i, j;

    for(i = 0; i < GSM0338_EXTENSIONS; i++)
        for(j = 0; j < GSM0338_ALPHABET_SIZE; j++)
        {
            if(uc == GSM0338ToUCS2Array[i][j])
                return TRUE;
        }
    return FALSE;
}
BOOL UnicodeIsHighStr(TCHAR *wchar)
{
	BOOL result = FALSE;
	TCHAR wstr;
	char str;
	INT len = _tcslen(wchar);
	for(int i = 0; i < len; i ++){
		wstr = wchar[i];
		str = (char)(wstr >> 8);
		if(str != 0x00){
			result = TRUE;
			break;
		}
	}
	return result;
}
BOOL CStringisHighStr(CString str)
{
	BOOL result;
	LPTSTR lptstr = str.GetBuffer(str.GetLength());
	result = UnicodeIsHighStr(lptstr);
	str.ReleaseBuffer();
	return result;
}

//判断文本中的字符是否全部在GSM-7bit编码表中
BOOL IsAlphabetUnicode(const TCHAR *str)
{
     ASSERT(str);

      wchar_t* wszUnicode = NULL;
     int iLen = 0;
     int i = 0;
 
     if(wcslen(str) == 0)
         return TRUE;

     //计算转换的字符数
    //iLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
     iLen = wcslen(str);
 
     //给wszUnicode分配内存
     wszUnicode = new wchar_t[iLen+1];
    wcscpy(wszUnicode,str);
		
     //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
     //MultiByteToWideChar(CP_ACP, 0, str, -1, wszUnicode, iLen);

     for(i = 0; i < iLen; i++)
     {
         if(!IsGsm7bitChar(wszUnicode[i]))
		 {
			 delete []wszUnicode;				//wyw_0326 add

             return FALSE;
		 }
     }

	 //释放内存
     delete []wszUnicode;
	 return TRUE;
}
BOOL IsAlphabet(const char *str)
{
     ASSERT(str);

     wchar_t* wszUnicode = NULL;
     int iLen = 0;
     int i = 0;
 
     if(strlen(str) == 0)
         return TRUE;

     //计算转换的字符数
     iLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
 
     //给wszUnicode分配内存
     wszUnicode = new wchar_t[iLen+1];

     //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
     MultiByteToWideChar(CP_ACP, 0, str, -1, wszUnicode, iLen);

     for(i = 0; i < iLen-1; i++)
     {
         if(!IsGsm7bitChar(wszUnicode[i]))
             return FALSE;
     }

	 //释放内存
     delete []wszUnicode;
	 return TRUE;
}

//获得双字节字符集文本串的字符个数
USHORT GetUnicodeCharNum(const TCHAR *str)
{
    USHORT len;
    len = wcslen(str);
  /*  num = 0;
    char * strTemp;
	strTemp=new char[len*2];
	WCharToUnicode(str, strTemp);
    for(i=0;i<len*2;i++)
    {
        num++;

		
        if(IsDBCSLeadByte((BYTE)strTemp[i]))
            i++;
    }
	delete []strTemp;*/
    return len;
}


USHORT GetACSIICharNum(const TCHAR *str)
{
    USHORT len, num, i;
    len = wcslen(str);
    num = 0;
	char * strTemp;
	strTemp=new char[len];
	WCharToChar(str, strTemp);
    for(i=0;i<len;i++)
    {
        num++;
        if(IsDBCSLeadByte((BYTE)strTemp[i]))
            i++;
    }
	delete []strTemp;
    return num;
}

void AtRespDummy(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    return;
}

#ifdef FEATURE_SMS_PDUMODE
//长度，类型，号码
/*0D91683129413485F7*/
void DecodeNumFormSmsPDU(const char *pdunum, char *pOutNum)
{
    ASSERT(pdunum && pOutNum);

    const char *p = pdunum;
    const char *q = p;
    USHORT numtype = 0;
    char buf[10];

    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    numtype = strtol(buf, NULL, 16);

    switch(numtype) {
    case 0x91:
        pOutNum[0] = '+';
    	break;
    }

    p+=2;
    while(*p)
    {
        q = p+1;
        if(*q)
        {
            pOutNum[strlen(pOutNum)] = *q;
        }
        
        if(UPCASE(*p) != 'F')
        {
            pOutNum[strlen(pOutNum)] = *p;
            p+=2;
        }
        else
        {
            pOutNum[strlen(pOutNum)] = '\0';
            break;
        }
    }
}

//年，月，日，时，分，秒，时区
/*60804041235100*/
void DecodeTimeFormSmsPDU(const char *pdutime, COleDateTime *pOutTime)
{
    ASSERT(pdutime && pOutTime);

    int time[7];
    int cnt;
    const char *p, *q;

    for(cnt = 0; cnt < 7; cnt++)
        time[cnt] = 0;

    p = q = pdutime;
    cnt = 0;

    while(*p && cnt < 7)
    {
        q = p+1;
        time[cnt] = (*q-48) * 10 + (*p-48);
        cnt++;p+=2;
    }

    *pOutTime = COleDateTime(time[0]+2000, time[1], time[2], time[3], time[4], time[5]);
}

//内容
/*963F62C9*/
void DecodeContentFromSmsPDU(const char *pduContent, const BYTE codeType, char *pContent)
{
    ASSERT(pduContent && pContent);

    //ASCII
    if(codeType == 0x00)
    {
        const char *p = pduContent;
        char buf[10];
        char c;

        while(*p)
        {
            memset(buf, 0x00, sizeof(buf));
            strncpy(buf, p, 2);
            c = strtol(buf, NULL, 16); 
            pContent[strlen(pContent)] = c;
            p+=2;
        }
    }
    //UCS2
    else if(codeType == 0x08)
    {
        CString strGb = UCS2ToGB((CString)pduContent);
        strncpy(pContent, strGb, SMS_CHAR_MAX);
    }
    else
        ASSERT(false);
}

/* +CMGR: 1,,24
0891683108200505F0240D91683129413485F700086080404123510004963F62C9
*/
void DecodeSmsPDU(const char *pdu, const USHORT len, StSmsRecord *pRecord)
{
    ASSERT(pdu != NULL && strlen(pdu) > 0);
    ASSERT(len > 0);
    ASSERT(pRecord != NULL);

    const char *p = pdu;
    char buf[10];
    USHORT numlen = 0;
    USHORT contentlen = 0;
    BYTE fo = 0;
    BYTE pid = 0;
    BYTE code = 0;
    char pdunum[50];
    char pdutime[30];
    char pduContent[500];

    memset(pdunum, 0x00, sizeof(pdunum));
    memset(pdutime, 0x00, sizeof(pdutime));
    memset(pduContent, 0x00, sizeof(pduContent));
    
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    numlen = strtol(buf, NULL, 16);

    //skip SC Number
    p = p + 2 * (numlen+1);

    if(*p)
    {
        memset(buf, 0x00, sizeof(buf));
        strncpy(buf, p, 2);
        fo = strtol(buf, NULL, 16); 
       
        p+=2;
        memset(buf, 0x00, sizeof(buf));
        strncpy(buf, p, 2);
        numlen = strtol(buf, NULL, 16); 
        
        p+=2;
        strncpy(pdunum, p, numlen+3);
        DecodeNumFormSmsPDU(pdunum, (char*)pRecord->szNumber);

        p+=(numlen+3);
        memset(buf, 0x00, sizeof(buf));
        strncpy(buf, p, 2);
        pid = strtol(buf, NULL, 16);

        p+=2;
        memset(buf, 0x00, sizeof(buf));
        strncpy(buf, p, 2);
        code = strtol(buf, NULL, 16);

        p+=2;
        strncpy(pdutime, p, 14);
        DecodeTimeFormSmsPDU(pdutime, &pRecord->timestamp);

        p+=14;
        memset(buf, 0x00, sizeof(buf));
        strncpy(buf, p, 2);
        contentlen = strtol(buf, NULL, 16);

        p+=2;
        strncpy(pduContent, p, contentlen*2);
        DecodeContentFromSmsPDU(pduContent, code, (char*)pRecord->szContent);
    }
}

void EncodeSmsPDU()
{
}
#endif

//提取长短信的参数
//输入："nRefCnt/nSeqCnt/nTotalCnt","1/1/2"
//输出：长短信标识号,长短信当前分段号,长短信总分段号
BOOL ExtractConcatenateSmsPara(char *Para, USHORT *nRefCnt, BYTE *nSeqCnt, BYTE *nTotalCnt)
{
    ASSERT(Para && strlen(Para) > 0);

    BOOL ret = TRUE;
    char *p = Para;
    char *ptr[3];
    int cnt = 1;
    ptr[0] = p; ptr[1] = ptr[2] = 0;

    while(*p && cnt < 3)
    {
        if(*p == '/')
        {
            ptr[cnt++] = p+1;
            *p = 0;
        }
        p++;
    }

    if(cnt < 3 || ptr[1] == 0 || ptr[2] == 0)
        ret = FALSE;
    else
    {
        *nRefCnt = atoi(ptr[0]);
        *nSeqCnt = atoi(ptr[1]);
        *nTotalCnt = atoi(ptr[2]);

        if(*nSeqCnt == 0 || *nTotalCnt == 0 || *nSeqCnt > *nTotalCnt)
            ret = FALSE;
    }
    
    if(!ret)
        *nRefCnt = *nSeqCnt = *nTotalCnt = 0;
    return ret;
}

//flexi 版本，第一个参数修改为短信内容
//提取长短信的参数
//输入："nRefCnt/nSeqCnt/nTotalCnt","1/1/2"
//输出：长短信标识号,长短信当前分段号,长短信总分段号
BOOL ExtractConcatenateSmsPara_Flexi(CString Para, TCHAR *nRefCnt, BYTE *nSeqCnt, BYTE *nTotalCnt)
{
    ASSERT(Para && wcslen(Para) > 0);

	BOOL ret = TRUE;

	TCHAR* szMsg = new TCHAR[100];
	//其参数为CString字符串的长度
	szMsg = Para.GetBuffer(Para.GetLength());
	Para.ReleaseBuffer();
// 	delete []szMsg;
// 	szMsg = NULL;

//	TCHAR *p = szMsg;
//	p++;
//	int cnt;

// 	TCHAR *ptr[5];
//     for (cnt =0; cnt < 5; cnt++)
//     {
// 		*ptr[cnt] = szMsg[cnt+1];
//     }
// 	delete []szMsg;
//  	szMsg = NULL;

//      if(cnt < 5 || ptr[1] == 0 || ptr[2] == 0)
//          ret = FALSE;
	
//	TCHAR nRefCnt_flexi;


	TCHAR STR_SeqCnt1[2] = {0};
	STR_SeqCnt1[0] = szMsg[1];

	TCHAR STR_SeqCnt2[2] = {0};
	STR_SeqCnt2[0] = szMsg[2];

	TCHAR STR_nTotalCnt1[2] = {0};
	STR_nTotalCnt1[0] = szMsg[4];

	TCHAR STR_nTotalCnt2[2] = {0};
	STR_nTotalCnt2[0] = szMsg[5];


	
	*nRefCnt = szMsg[3];
	*nSeqCnt = (BYTE)((_wtoi(STR_SeqCnt1) *10) + (_wtoi(STR_SeqCnt2)));
    *nTotalCnt = (BYTE)((_wtoi(STR_nTotalCnt1) *10) + (_wtoi(STR_nTotalCnt2)));

    if(*nSeqCnt == 0 || *nTotalCnt == 0 || *nSeqCnt > *nTotalCnt)
             ret = FALSE;

	if(!ret)
	   *nRefCnt = *nSeqCnt = *nTotalCnt = 0;
    return ret;

//     BOOL ret = TRUE;
//     char *p = Para;
//     char *ptr[3];
//     int cnt = 1;
//     ptr[0] = p; ptr[1] = ptr[2] = 0;
// 	                                   
//     while(*p && cnt < 3)
//     {
//         if(*p == '/')
//         {
//             ptr[cnt++] = p+1;
//             *p = 0;
//         }
//         p++;
//     }
// 	
//     if(cnt < 3 || ptr[1] == 0 || ptr[2] == 0)
//         ret = FALSE;
//     else
//     {
//         *nRefCnt = atoi(ptr[0]);
//         *nSeqCnt = atoi(ptr[1]);
//         *nTotalCnt = atoi(ptr[2]);
// 		
//         if(*nSeqCnt == 0 || *nTotalCnt == 0 || *nSeqCnt > *nTotalCnt)
//             ret = FALSE;
//     }
//     
//     if(!ret)
//         *nRefCnt = *nSeqCnt = *nTotalCnt = 0;
//     return ret;
}

//显示长短信参数
//输入：长短信标识号,长短信当前分段号,长短信总分段号
//输出："(nRefCnt/nSeqCnt/nTotalCnt)","(1/1/2)"
//Modified by Zhou Bin 2008.12.30
//BOOL DspConcatenateSmsPara(TCHAR *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt)
BOOL DspConcatenateSmsPara(char *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt)
{
    ASSERT(DspBuf);

    if(nSeqCnt == 0 || nTotalCnt == 0 || nSeqCnt > nTotalCnt)
        return FALSE;
    else
    {
#if 0
		//Modified by Zhou Bin 2008.12.30
        //swprintf(DspBuf, _T("(%d/%d/%d)"), nRefCnt, nSeqCnt, nTotalCnt);
		sprintf(DspBuf, "(%d/%d/%d)", nRefCnt, nSeqCnt, nTotalCnt);
#else
        //swprintf(DspBuf,_T("(%d/%d)"), nSeqCnt, nTotalCnt);
		sprintf(DspBuf,"(%d/%d)", nSeqCnt, nTotalCnt);
		
#endif
        return TRUE;
    }
}

//设置写发长短信参数
//输入：长短信标识号,长短信总分段号,长短信当前分段号
//输出：CONCAT_8: "050003050201", CONCAT_16: "06080400030201"
BOOL SetConcatenateSmsPara(TCHAR *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt)
{
    ASSERT(DspBuf);

    if(nSeqCnt == 0 || nTotalCnt == 0 || nSeqCnt > nTotalCnt)
        return FALSE;
    else
    {
        if(nRefCnt <= 0x00FF)
            swprintf(DspBuf, _T("050003%02X%02X%02X"), nRefCnt, nTotalCnt, nSeqCnt);
        else
            swprintf(DspBuf, _T("060804%04X%02X%02X"), nRefCnt, nTotalCnt, nSeqCnt);
        return TRUE;
    }
}

BOOL SetConcatenateSmsParaA(char *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt, char MinMaxChar)
{
    ASSERT(DspBuf);

    if(nSeqCnt == 0 || nTotalCnt == 0 || nSeqCnt > nTotalCnt)
        return FALSE;
    else
    {
        if(nRefCnt <= 0x00FF)
		{
			if (nSeqCnt<10 && nTotalCnt <10)
			{
			    //sprintf(DspBuf, "050003%02X%02X%02X", nRefCnt, nTotalCnt, nSeqCnt);
		    	sprintf(DspBuf,"(0%d%c0%d)",nSeqCnt,MinMaxChar,nTotalCnt);//FLEXI modify by liub
			}
			else if (nSeqCnt<10)
			{
				sprintf(DspBuf,"(0%d%c%d)",nSeqCnt,MinMaxChar,nTotalCnt);//FLEXI modify by liub
			}
			else if (nTotalCnt<10)
			{
				sprintf(DspBuf,"(%d%c0%d)",nSeqCnt,MinMaxChar,nTotalCnt);//FLEXI modify by liub
			}
			else
			{
				sprintf(DspBuf,"(%d%c%d)",nSeqCnt,MinMaxChar,nTotalCnt);//FLEXI modify by liub
			}
		}

        else
		{
			if (nSeqCnt<10 && nTotalCnt <10)
			{
				//sprintf(DspBuf, "050003%02X%02X%02X", nRefCnt, nTotalCnt, nSeqCnt);
				sprintf(DspBuf,"(0%d%c0%d)",nSeqCnt,MinMaxChar,nTotalCnt);//FLEXI modify by liub
			}
			else if (nSeqCnt<10)
			{
				sprintf(DspBuf,"(0%d%c%d)",nSeqCnt,MinMaxChar,nTotalCnt);//FLEXI modify by liub
			}
			else if (nTotalCnt<10)
			{
				sprintf(DspBuf,"(%d%c0%d)",nSeqCnt,MinMaxChar,nTotalCnt);//FLEXI modify by liub
			}
			else
			{
				sprintf(DspBuf,"(%d%c%d)",nSeqCnt,MinMaxChar,nTotalCnt);//FLEXI modify by liub
			}
		}
            //sprintf(DspBuf, "060804%04X%02X%02X", nRefCnt, nTotalCnt, nSeqCnt);
		    //sprintf(DspBuf,"(%d/%d)",nSeqCnt,nTotalCnt);//FLEXI modify by liub
        return TRUE;
    }
}

//判断当前记录是否是合法的长短信
BOOL IsConcatenateSms(StSmsRecord *pSmsRecord)
{
    ASSERT(pSmsRecord);

    BOOL ret = FALSE;

    if(pSmsRecord->flag & SMS_RECORD_FLAG_CONCATENATE_SEGE)
    {
        if(pSmsRecord->nSeqCnt == 0 || pSmsRecord->nTotalCnt == 0
            || pSmsRecord->nSeqCnt > pSmsRecord->nTotalCnt)
            ret = FALSE;
        else
            ret = TRUE;
    }

    if(!ret)
    {
        pSmsRecord->flag &= (BYTE)~SMS_RECORD_FLAG_CONCATENATE_SEGE;
        pSmsRecord->nRefCnt = pSmsRecord->nSeqCnt = pSmsRecord->nTotalCnt = 0;
    }
    
    return ret;
}

#ifdef FEATURE_SMS_CONCATENATE
BYTE gSmsRefCnt = -1;
BOOL gSmsIsConcatenate = FALSE;
BOOL gSmsIsAsciiCode = TRUE;
BYTE gSmsTotalSege;
BYTE gSmsCurSege;
//Modified by Zhou Bin 2008.12.30
TCHAR gszSmsSege[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX + 1];
//TCHAR gszSmsSege[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX + 1];

void ResetSmsConcatenateData()
{
    gSmsIsConcatenate = FALSE;
    gSmsIsAsciiCode = TRUE;
    gSmsCurSege = 0;
    gSmsTotalSege = 0;
    memset(gszSmsSege, 0x00, sizeof(gszSmsSege));
}

BOOL DivideSmsConcatenate(const TCHAR *szRawContent)
{
    ASSERT(szRawContent);
    
    ResetSmsConcatenateData();

    int l = GetUnicodeCharNum(szRawContent);

    if(l == 0 || l > (SMS_CONCAT_SEGMENT_MAX * SMS_CONCAT_ASCII_MAX))
        return FALSE;

    gSmsIsAsciiCode = IsAlphabetUnicode(szRawContent);

    if(gSmsIsAsciiCode)
    {
        if(GetACSIICharNum(szRawContent) > SMS_CHAR_MAX)
            gSmsIsConcatenate = TRUE;
    }
    else
    {
        if(GetUnicodeCharNum(szRawContent) > SMS_CHINESE_CHAR_MAX)
            gSmsIsConcatenate = TRUE;
    }

    if(gSmsIsConcatenate)
    {
        gSmsRefCnt++;

        if(gSmsIsAsciiCode)
          {
            USHORT i, j;
            USHORT GbNum = 0;
            USHORT len = wcslen(szRawContent);
            
            gSmsTotalSege = 0;

            for(i=0, j=0; i < SMS_CONCAT_ASCII_MAX&& j < len;)
            {
               // gszSmsSege[gSmsTotalSege][i++] = szRawContent[j];
               
           //     if((IsDBCSLeadByte((BYTE)(char)(szRawContent[j]  & 0x00FF))) && (j < len))
             //   {
                    gszSmsSege[gSmsTotalSege][i] = szRawContent[j];
              //  }
                i++;
		  j++;
                GbNum++;
                            
                if(GbNum == SMS_CONCAT_ASCII_MAX && j < len && gSmsTotalSege < SMS_CONCAT_SEGMENT_MAX-1)
                {
                    i = 0;
                    GbNum = 0;
                    gSmsTotalSege++;
                }
            }
            gSmsTotalSege++;
        }
        else
        {
            USHORT i, j;
            USHORT GbNum = 0;
            USHORT len = wcslen(szRawContent);
            
            gSmsTotalSege = 0;
            for(i=0, j=0; i < SMS_CONCAT_GB_MAX && j < len;)
            {
              //  gszSmsSege[gSmsTotalSege][i++] = szRawContent[j];
				
              //  if((IsDBCSLeadByte((BYTE)(char)(szRawContent[j] >> 8))) && (j < len))
              //  {
                    gszSmsSege[gSmsTotalSege][i] = szRawContent[j];
               // }
                i++;
		  j++;
                GbNum++;
                            
                if(GbNum == SMS_CONCAT_GB_MAX && j < len && gSmsTotalSege < SMS_CONCAT_SEGMENT_MAX-1)
                {
                    i = 0;
                    GbNum = 0;
                    gSmsTotalSege++;
                }
            }
            gSmsTotalSege++;
        }
    }

    return TRUE;
}
#endif //FEATURE_SMS_CONCATENATE

//国家码列表
const char szCountryCodeArr[][10] = {
		"+65",
        "+852",
        "+86",
        "+886",
        "+1",
        "+27",
        "+30",
        "+31",
        "+351",
        "+359",
        "+385",
        "+39",
        "+40",
        "+44",
        "+46",
        "+49",
        "+504",
        "+53",
        "+55",
        "+61",
        "+62",
        "+64",
        "+90",
        "+91",
        "+94",
        "+961",
        "", //end flag
};

BOOL SmsAtCMGRRspProc(BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum, StSmsRecord &record, const EnSmsKind kind)
{
    int cnt = 0;
    char *ptr[7], *p;
    p = (char *)&strArr[0][strlen(gcstrResSms[AT_SMS_QCMGR])];
#ifdef FEATURE_ATTEST_SUPPORT
	CStdioFile file;
	if(file.Open("Sms.log", CFile::modeReadWrite))
	{
		CString str;
		str.Format("%s", p);
		str.Insert(str.GetLength(),"\n");
		DWORD dwActual = file.SeekToEnd();
		file.WriteString(str);
		file.Close();		
	}
#endif
    ptr[0] = ptr[1] = ptr[2] = ptr[3] = ptr[4] = ptr[5] = ptr[6] =0;

    BOOL bOutQuot = TRUE;//判断是否在双引号内
	BOOL bNo_See_Comma = TRUE;//判断是否遇到逗号
// 
//     while(*p != '\0')
//     {
//         if(*p == '\"')
//         {   
//             if(bOutQuot && cnt < 5)
//                 ptr[cnt++] = p;
// 
//             bOutQuot = !bOutQuot;
//             p++;
//         }
//         else if(*p == ',' && bOutQuot)
//         {
//             *p++ = '\0';
//         }
//         else
//         {
//             p++;
//         }
//     }
  	while(*p != '\0')
    {
		if (bNo_See_Comma)
		{   
	    	ptr[cnt++] = p;
			bNo_See_Comma = FALSE;

		//	p++;
		}

		
		if(*p == ',' && bOutQuot == TRUE && cnt < 7)
        {
            
            *p++ = '\0';
			bNo_See_Comma = TRUE;
        }
        else if(*p == '\"')
        {
			bOutQuot = !bOutQuot;
      		p++;
        }
		else
			p++;
    }


    for(int i = 0; i < 5; i++)
    {
        if(ptr[i] != 0)
        {
            if(*(ptr[i] + strlen(ptr[i]) - 1) == '\"')
                *(ptr[i] + strlen(ptr[i]) - 1) = '\0';
            if(*ptr[i] == '\"')
                ptr[i]++;
        }
    }

    if((record.state = GetSmsStateFromStr(ptr[0])) == SMS_STATE_MAX)
    {
        return FALSE;
    }

    //state test begin
    if(kind == SMS_KIND_MT)
    {
        if(record.state != SMS_STATE_MT_NOT_READ
            && record.state != SMS_STATE_MT_READ)
        {
            return FALSE;
        }
    }
    else if(kind == SMS_KIND_MO)
    {
        if(record.state != SMS_STATE_MO_NOT_SENT
            && record.state != SMS_STATE_MO_SENT)
        {
            return FALSE;
        }
    }
    //state test end
	CString szNumTemp=(char*)ptr[1];
    wcscpy((TCHAR *)record.szNumber, szNumTemp);

    int time, scnum, concatenate,ascii_or_unicode;
    time = scnum = concatenate = ascii_or_unicode = -1;

    if(record.state == SMS_STATE_MT_NOT_READ || record.state == SMS_STATE_MT_READ)
    {
		if (!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000")))
		{
			//time = 2; scnum = 4; concatenate = 3;
			time =3; scnum = 2; concatenate = -1; ascii_or_unicode =5;
		}
		else
		{
            time = 3; scnum = 2; concatenate = -1;ascii_or_unicode =5;
		}
    }
    else if(record.state == SMS_STATE_MO_NOT_SENT || record.state == SMS_STATE_MO_SENT)
    {
		if (!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000")))
		{
             time = 3; scnum = 2; concatenate = -1;ascii_or_unicode =4;
		}
		else
		{
			time = 3; scnum = 2; concatenate = -1;ascii_or_unicode =4;
		}
		record.state = SMS_STATE_MT_READ;
    }

    if(time == -1 || !(ptr[time] && *ptr[time]))
        record.timestamp = COleDateTime::GetCurrentTime();
    else
    {
        if(!GetTimeFromStr((LPCSTR)ptr[time], record.timestamp))
            record.timestamp = COleDateTime::GetCurrentTime();
    }
		CString szSCNumTemp=(char*)ptr[scnum];
    if(scnum == -1 || !(ptr[scnum] && *ptr[scnum]))
        memset(record.szSCNumber, 0x00, SMS_SC_NUM_MAX);
    else
        wcsncpy((TCHAR*)record.szSCNumber, szSCNumTemp, SMS_SC_NUM_MAX);

    if(concatenate != -1 && ptr[concatenate] && *ptr[concatenate])
    {
        if(ExtractConcatenateSmsPara(ptr[concatenate], &record.nRefCnt, &record.nSeqCnt, &record.nTotalCnt))
            record.flag |= SMS_RECORD_FLAG_CONCATENATE_SEGE;
    }
    //modified by wk end on 2006-8-22

	USES_CONVERSION;
    if(wStrNum == 3)
    {
		//modify by lijl 2009.4.13 以ascii码形式存储短信内容
		if (*ptr[ascii_or_unicode] == '4')
		{
		CString strGb = UCS2ToGB(A2W((char*)strArr[1]));
		strncpy(record.szContent, W2A(strGb), SMS_CHAR_MAX);

		}
		else
		{
			CString strGb = UCS2ToGB((CString)((char*)strArr[1]));
			strncpy(record.szContent, W2A((LPCTSTR)strGb), SMS_CHAR_MAX*2);
            //wcsncpy((TCHAR *)record.szContent, strGb, SMS_CHAR_MAX*2);
		}





//		CString strGb = UCS2ToGB(A2W((char*)strArr[1]));
//		strncpy(record.szContent, W2A(strGb), SMS_CHAR_MAX);


    }
    else
        memset((TCHAR *)record.szContent, 0, SMS_CHAR_MAX*2); 
#ifdef FEATURE_ATTEST_SUPPORT	
	if(file.Open("Sms.log", CFile::modeReadWrite))
	{
		CString str;
		str.Format("%s", (CString)((char*)strArr[1]));
		str.Insert(str.GetLength(),"\n");
		DWORD dwActual = file.SeekToEnd();
		file.WriteString(str);
		file.Close();		
	}
#endif
    
    return TRUE;
}

const wchar_t GSM0338ToUCS2Array[GSM0338_EXTENSIONS][GSM0338_ALPHABET_SIZE] = 
{
	/* GSM 7 bit default alphabet */
	{
		0x0040,		/* 00 */
		0x00A3,
		0x0024,
		0x00A5,
		0x00E8,
		0x00E9,
		0x00F9,
		0x00EC,
		0x00F2,
		0x00C7,
		0x000A,
		0x00D8,
		0x00F8,
		0x000D,
		0x00C5,
		0x00E5,		/* 0F */

		0x0394,		/* 10 */
		0x005F,
		0x03A6,
		0x0393,
		0x039B,
		0x03A9,
		0x03A0,
		0x03A8,
		0x03A3,
		0x0398,
		0x039E,
		0x0020,
		0x00C6,
		0x00E6,
		0x00DF,
		0x00C9,		/* 1F */

		0x0020,		/* 20 */
		0x0021,
		0x0022,
		0x0023,
		0x00A4,
		0x0025,
		0x0026,
		0x0027,
		0x0028,
		0x0029,
		0x002A,
		0x002B,
		0x002C,
		0x002D,
		0x002E,
		0x002F,		/* 2F */

		0x0030,		/* 30 */
		0x0031,
		0x0032,
		0x0033,
		0x0034,
		0x0035,
		0x0036,
		0x0037,
		0x0038,
		0x0039,
		0x003A,
		0x003B,
		0x003C,
		0x003D,
		0x003E,
		0x003F,		/* 3F */

		0x00A1,		/* 40 */
		0x0041,
		0x0042,
		0x0043,
		0x0044,
		0x0045,
		0x0046,
		0x0047,
		0x0048,
		0x0049,
		0x004A,
		0x004B,
		0x004C,
		0x004D,
		0x004E,
		0x004F,		/* 4F */

		0x0050,		/* 50 */
		0x0051,
		0x0052,
		0x0053,
		0x0054,
		0x0055,
		0x0056,
		0x0057,
		0x0058,
		0x0059,
		0x005A,
		0x00C4,
		0x00D6,
		0x00D1,
		0x00DC,
		0x00A7,		/* 5F */

		0x00BF,		/* 60 */
		0x0061,
		0x0062,
		0x0063,
		0x0064,
		0x0065,
		0x0066,
		0x0067,
		0x0068,
		0x0069,
		0x006A,
		0x006B,
		0x006C,
		0x006D,
		0x006E,
		0x006F,		/* 6F */

		0x0070,		/* 70 */
		0x0071,
		0x0072,
		0x0073,
		0x0074,
		0x0075,
		0x0076,
		0x0077,
		0x0078,
		0x0079,
		0x007A,
		0x00E4,
		0x00F6,
		0x00F1,
		0x00FC,
		0x00E0,		/* 7F */
		
#if 0
		0x0040		/* @ sign exception */
#endif
	},

	/* GSM 7 bit default alphabet extension table */
	{
		0x0040,				/* 00 */
		0x00A3,
		0x0024,
		0x00A5,
		0x00E8,
		0x00E9,
		0x00F9,
		0x00EC,
		0x00F2,
		0x00C7,
		0x000A, 	/* this should be a Page Break, otherwise Line Feed if not handled */
		0x00D8,
		0x00F8,
		0x000D,
		0x00C5,
		0x00E5,				/* 0F */

		0x0394,				/* 10 */
		0x005F,
		0x03A6,
		0x0393,
		0x005E,		/* ^ */
		0x03A9,
		0x03A0,
		0x03A8,
		0x03A3,
		0x0398,
		0x039E,
		0x0020,
		0x00C6,
		0x00E6,
		0x00DF,
		0x00C9,				/* 1F */

		0x0020,				/* 20 */
		0x0021,
		0x0022,
		0x0023,
		0x00A4,
		0x0025,
		0x0026,
		0x0027,
		0x007B,		/* { */
		0x007D,		/* } */
		0x002A,
		0x002B,
		0x002C,
		0x002D,
		0x002E,
		0x005C,		/* \ */	/* 2F */

		0x0030,				/* 30 */
		0x0031,
		0x0032,
		0x0033,
		0x0034,
		0x0035,
		0x0036,
		0x0037,
		0x0038,
		0x0039,
		0x003A,
		0x003B,
		0x005B,		/* [ */
		0x007E,		/* ~ */
		0x005D,		/* ] */
		0x003F,				/* 3F */

		0x007C,		/* | */	/* 40 */
		0x0041,
		0x0042,
		0x0043,
		0x0044,
		0x0045,
		0x0046,
		0x0047,
		0x0048,
		0x0049,
		0x004A,
		0x004B,
		0x004C,
		0x004D,
		0x004E,
		0x004F,				/* 4F */

		0x0050,				/* 50 */
		0x0051,
		0x0052,
		0x0053,
		0x0054,
		0x0055,
		0x0056,
		0x0057,
		0x0058,
		0x0059,
		0x005A,
		0x00C4,
		0x00D6,
		0x00D1,
		0x00DC,
		0x00A7,				/* 5F */

		0x00BF,				/* 60 */
		0x0061,
		0x0062,
		0x0063,
		0x0064,
		0x20AC,		/* Euro currency symbol */
		0x0066,
		0x0067,
		0x0068,
		0x0069,
		0x006A,
		0x006B,
		0x006C,
		0x006D,
		0x006E,
		0x006F,				/* 6F */

		0x0070,				/* 70 */
		0x0071,
		0x0072,
		0x0073,
		0x0074,
		0x0075,
		0x0076,
		0x0077,
		0x0078,
		0x0079,
		0x007A,
		0x00E4,
		0x00F6,
		0x00F1,
		0x00FC,
		0x00E0,				/* 7F */

#if 0
		0x0040		/* @ sign exception */
#endif
	}
};

StRcvConcSmsRecord gSmsRcvConcBUf[SMS_RCVCONCBUF_MAX];

void ResetSmsRcvConcBuf(BYTE nIndex)
{
	ASSERT(nIndex <= SMS_RCVCONCBUF_MAX);

	BYTE cnt;
	if(nIndex == SMS_RCVCONCBUF_MAX)
	{
		for(cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++)
		{
			memset(&gSmsRcvConcBUf[cnt], 0x00, sizeof(StRcvConcSmsRecord));
			gSmsRcvConcBUf[cnt].nTimerID = IDT_RCVCONCSMS_TIMEOUT_BEGIN + cnt;
		}
	}
	else
	{
		memset(&gSmsRcvConcBUf[nIndex], 0x00, sizeof(StRcvConcSmsRecord));
		gSmsRcvConcBUf[nIndex].nTimerID = IDT_RCVCONCSMS_TIMEOUT_BEGIN + nIndex;
	}
}

static void SaveSmsSegeToRcvConcBuf(const StSmsRecord &sege, const BYTE cnt)
{	
	gSmsRcvConcBUf[cnt].bInuse = TRUE;
	gSmsRcvConcBUf[cnt].nSeqFlag[sege.nSeqCnt-1] = TRUE;
	gSmsRcvConcBUf[cnt].nRefCnt = sege.nRefCnt;
	gSmsRcvConcBUf[cnt].nTotalCnt = sege.nTotalCnt;
	gSmsRcvConcBUf[cnt].timestamp = sege.timestamp;
	wcsncpy((TCHAR*)gSmsRcvConcBUf[cnt].szNumber, (const TCHAR*)sege.szNumber, PB_NUM_MAX);
	wcsncpy((TCHAR*)gSmsRcvConcBUf[cnt].szSCNumber, (const TCHAR*)sege.szSCNumber, SMS_SC_NUM_MAX);
	wcsncpy((TCHAR*)gSmsRcvConcBUf[cnt].szContent[sege.nSeqCnt-1], (const TCHAR*)sege.szContent, SMS_CONCAT_ASCII_MAX*2);
}

int SaveSmsSegeToRcvConcBuf(const StSmsRecord &sege)
{
	BYTE cnt;
 
	if(sege.nSeqCnt == 0 || sege.nTotalCnt == 0 || sege.nSeqCnt > sege.nTotalCnt 
		|| sege.nSeqCnt > SMS_CONCAT_SEGMENT_MAX || sege.nTotalCnt > SMS_CONCAT_SEGMENT_MAX)
		return -1;

	for(cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++)
	{
		if(gSmsRcvConcBUf[cnt].bInuse && gSmsRcvConcBUf[cnt].nRefCnt == sege.nRefCnt
			&& gSmsRcvConcBUf[cnt].nTotalCnt == sege.nTotalCnt 
			&& !gSmsRcvConcBUf[cnt].nSeqFlag[sege.nSeqCnt-1])
		{
            if(wcscmp((const TCHAR*)gSmsRcvConcBUf[cnt].szNumber, (const TCHAR*)sege.szNumber) == 0)
            {
			    SaveSmsSegeToRcvConcBuf(sege, cnt);
			    return cnt;
            }
		}
	}

	for(cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++)
	{
		if(!gSmsRcvConcBUf[cnt].bInuse)
		{
			SaveSmsSegeToRcvConcBuf(sege, cnt);
			return cnt;
		}
	}

	return -1;
}

int MergeConcSmsSegeTogether(StSmsRecord &record)
{
	BYTE i;
	BYTE cnt;

	memset(&record, 0x00, sizeof(StSmsRecord));

	for(cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++)
	{
		if(gSmsRcvConcBUf[cnt].bInuse && gSmsRcvConcBUf[cnt].nTotalCnt > 0)
		{
			for(i = 0; i < gSmsRcvConcBUf[cnt].nTotalCnt; i++)
			{
				if(!gSmsRcvConcBUf[cnt].nSeqFlag[i])
					break;
			}

			//Ready to merge
			if(i == gSmsRcvConcBUf[cnt].nTotalCnt)
			{
				record.flag = SMS_RECORD_FLAG_NEW | SMS_RECORD_FLAG_CONCATENATE_FULL;
				record.state = SMS_STATE_MT_NOT_READ;
				record.nRefCnt = gSmsRcvConcBUf[cnt].nRefCnt;
				record.timestamp = gSmsRcvConcBUf[cnt].timestamp;
				strncpy((char*)record.szNumber, (const char*)gSmsRcvConcBUf[cnt].szNumber, PB_NUM_MAX);
				record.m_NoATRspCDS = TRUE;
			//	wcsncpy((TCHAR*)record.szSCNumber, (const TCHAR*)gSmsRcvConcBUf[cnt].szSCNumber, SMS_SC_NUM_MAX);//(WCDMA使用)

				for(i = 0; i < gSmsRcvConcBUf[cnt].nTotalCnt; i++)
					strncat((char*)record.szContent, (const char*)gSmsRcvConcBUf[cnt].szContent[i], SMS_CONCAT_ASCII_MAX*2);

				ResetSmsRcvConcBuf(cnt);
				return cnt;
			}
		}
	}
	return -1;
}

BYTE gSmsTranRefCnt = -1;
BOOL gSmsTranIsConcatenate = FALSE;
BOOL gSmsTranIsAsciiCode = TRUE;
BYTE gSmsTranTotalSege;
BYTE gSmsTranCurSege;
TCHAR gszSmsTranSege[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX + 1];

void ResetSmsTranConcatenateData()
{
    gSmsTranIsConcatenate = FALSE;
    gSmsTranIsAsciiCode = TRUE;
    gSmsTranCurSege = 0;
    gSmsTranTotalSege = 0;
    memset(gszSmsTranSege, 0x00, sizeof(gszSmsTranSege));
}

BOOL DivideSmsTranConcatenate(const TCHAR *szRawContent)
{
    ASSERT(szRawContent);
    
    ResetSmsTranConcatenateData();

    int l = GetUnicodeCharNum(szRawContent);

    if(l == 0 || l > (SMS_CONCAT_SEGMENT_MAX * SMS_CONCAT_ASCII_MAX))
        return FALSE;

    gSmsTranIsAsciiCode = IsAlphabetUnicode(szRawContent);

    if(gSmsTranIsAsciiCode)
    {
        if(GetACSIICharNum(szRawContent) > SMS_CHAR_MAX)
            gSmsTranIsConcatenate = TRUE;
    }
    else
    {
        if(GetUnicodeCharNum(szRawContent) > SMS_CHINESE_CHAR_MAX)
            gSmsTranIsConcatenate = TRUE;
    }

    if(gSmsTranIsConcatenate)
    {
        gSmsTranRefCnt++;

        if(gSmsTranIsAsciiCode)
          {
            USHORT i, j;
            USHORT GbNum = 0;
            USHORT len = wcslen(szRawContent);
            
            gSmsTranTotalSege = 0;

            for(i=0, j=0; i < SMS_CONCAT_ASCII_MAX && j < len;)
            {
                //gszSmsTranSege[gSmsTranTotalSege][i++] = szRawContent[j];
				
                //if((IsDBCSLeadByte((BYTE)szRawContent[j++])) && (j < len))
                {
                    gszSmsTranSege[gSmsTranTotalSege][i++] = szRawContent[j++];
                }
                
                GbNum++;
                            
                if(GbNum == SMS_CONCAT_ASCII_MAX && j < len && gSmsTranTotalSege < SMS_CONCAT_SEGMENT_MAX-1)
                {
                    i = 0;
                    GbNum = 0;
                    gSmsTranTotalSege++;
                }
            }
            gSmsTranTotalSege++;
        }
        else
        {
            USHORT i, j;
            USHORT GbNum = 0;
            USHORT len = wcslen(szRawContent);
            
            gSmsTranTotalSege = 0;
            for(i=0, j=0; i < SMS_CONCAT_GB_MAX && j < len;)
            {
               // gszSmsTranSege[gSmsTranTotalSege][i++] = szRawContent[j];
				
			//	if((IsDBCSLeadByte((BYTE)szRawContent[j++])) && (j < len))
                {
                    gszSmsTranSege[gSmsTranTotalSege][i++] = szRawContent[j++];
                }
                
                GbNum++;
                            
                if(GbNum == SMS_CONCAT_GB_MAX && j < len && gSmsTranTotalSege < SMS_CONCAT_SEGMENT_MAX-1)
                {
                    i = 0;
                    GbNum = 0;
                    gSmsTranTotalSege++;
                }
            }
            gSmsTranTotalSege++;
        }
    }

    return TRUE;
}

const BYTE gc_sms_validity_period[] = {
	11, 71, 167, 169, 173, 244
};

TCHAR gSmsCentreNum[SMS_SC_NUM_MAX+2];

BYTE gSmsExtractNumCnt;
//TCHAR gSmsExtractNumBuf[SMS_EXTRACTNUM_MAX][PB_NUM_MAX+2];
char gSmsExtractNumBuf[SMS_EXTRACTNUM_MAX][PB_NUM_MAX+2];

BYTE ExtractNumFromSmsContent(const char *szRawContent)
{
#define SMS_EXTRACTNUM_FROM 5
#define SMS_EXTRACTNUM_TO PB_NUM_MAX
	
    gSmsExtractNumCnt = 0;
    memset(gSmsExtractNumBuf, 0x00, sizeof(gSmsExtractNumBuf));
	
    if(szRawContent != NULL && strlen(szRawContent) > 0)
    {
        BOOL bFound = FALSE;
        USHORT i, j, k, l, m;
        USHORT len = strlen(szRawContent);
		
        for(i=0,j=0,k=0; k < len && gSmsExtractNumCnt < SMS_EXTRACTNUM_MAX; k++)
        {
            if(IsDBCSLeadByte((BYTE)szRawContent[k]))
            {
                if(bFound)
                {
                    j = k;
                    
                    if((j-i) >= SMS_EXTRACTNUM_FROM && (j-i) <= SMS_EXTRACTNUM_TO)
                    {
                        for(l=0,m=i; m<j; l++,m++)
                            gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
                        gSmsExtractNumCnt++;
                    }
					
                    bFound = FALSE;
                    i = j = 0;
                }
                k++;
            }
            else if(szRawContent[k] >= '0' && szRawContent[k] <= '9')
            {
                if(!bFound)
                {
                    i = k;
                    bFound = TRUE;
                }
                else
                {
                    if(k == len - 1)
                    {
                        j = k + 1;
						
                        if((j-i) >= SMS_EXTRACTNUM_FROM && (j-i) <= SMS_EXTRACTNUM_TO)
                        {
                            for(l=0,m=i; m<j; l++,m++)
                                gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
                            gSmsExtractNumCnt++;
                        }
                    }
                }
            }
            else
            {
                if(bFound)
                {
                    j = k;
                    
                    if((j-i) >= SMS_EXTRACTNUM_FROM && (j-i) <= SMS_EXTRACTNUM_TO)
                    {
                        for(l=0,m=i; m<j; l++,m++)
                            gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
                        gSmsExtractNumCnt++;
                    }
					
                    bFound = FALSE;
                    i = j = 0;
                }
            }
        }
    }
    
    return gSmsExtractNumCnt;
}
// BYTE ExtractNumFromSmsContent(const TCHAR *szRawContent)
// {
// #define SMS_EXTRACTNUM_FROM 5
// #define SMS_EXTRACTNUM_TO PB_NUM_MAX
// 
//     gSmsExtractNumCnt = 0;
//     memset(gSmsExtractNumBuf, 0x0000, sizeof(gSmsExtractNumBuf));
// 
//     if(szRawContent != NULL && wcslen(szRawContent) > 0)
//     {
//         BOOL bFound = FALSE;
//         USHORT i, j, k, l, m;
//         USHORT len = wcslen(szRawContent);
// 
//         for(i=0,j=0,k=0; k < len && gSmsExtractNumCnt < SMS_EXTRACTNUM_MAX; k++)
//         {
//             if(IsDBCSLeadByte((BYTE)szRawContent[k]))
//             {
//                 if(bFound)
//                 {
//                     j = k;
//                     
//                     if((j-i) >= SMS_EXTRACTNUM_FROM && (j-i) <= SMS_EXTRACTNUM_TO)
//                     {
//                         for(l=0,m=i; m<j; l++,m++)
//                             gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
//                         gSmsExtractNumCnt++;
//                     }
// 
//                     bFound = FALSE;
//                     i = j = 0;
//                 }
//                 k++;
//             }
//             else if(szRawContent[k] >= '0' && szRawContent[k] <= '9')
//             {
//                 if(!bFound)
//                 {
//                     i = k;
//                     bFound = TRUE;
//                 }
//                 else
//                 {
//                     if(k == len - 1)
//                     {
//                         j = k + 1;
// 
//                         if((j-i) >= SMS_EXTRACTNUM_FROM && (j-i) <= SMS_EXTRACTNUM_TO)
//                         {
//                             for(l=0,m=i; m<j; l++,m++)
//                                 gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
//                             gSmsExtractNumCnt++;
//                         }
//                     }
//                 }
//             }
//             else
//             {
//                 if(bFound)
//                 {
//                     j = k;
//                     
//                     if((j-i) >= SMS_EXTRACTNUM_FROM && (j-i) <= SMS_EXTRACTNUM_TO)
//                     {
//                         for(l=0,m=i; m<j; l++,m++)
//                             gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
//                         gSmsExtractNumCnt++;
//                     }
// 
//                     bFound = FALSE;
//                     i = j = 0;
//                 }
//             }
//         }
//     }
//     
//     return gSmsExtractNumCnt;
// }

BOOL g_bSmsSyncComm;
USHORT g_ME_SmsCnt;
USHORT g_ME_SmsNum;
USHORT g_ME_SmsMax;
USHORT g_SM_SmsCnt;
USHORT g_SM_SmsNum;
USHORT g_SM_SmsMax;
StSmsCardRecord *g_ME_SmsRecord;
StSmsCardRecord *g_SM_SmsRecord;
StSmsRecord g_RcvSmsRecord;

void UE_InitSmsBuf()
{
    g_bSmsSyncComm = FALSE;
	g_ME_SmsCnt = 0;
    g_ME_SmsNum = 0;
    g_ME_SmsMax = 0;
	g_SM_SmsCnt = 0;
    g_SM_SmsNum = 0;
    g_SM_SmsMax = 0;
    g_ME_SmsRecord = NULL;
    g_SM_SmsRecord = NULL;

	g_ME_SmsRecord = new StSmsCardRecord[SMS_ME_NUM_MAX];
	g_SM_SmsRecord = new StSmsCardRecord[SMS_SM_NUM_MAX];

	ASSERT(g_ME_SmsRecord);
	ASSERT(g_SM_SmsRecord);

    memset(g_ME_SmsRecord, 0x00, sizeof(StSmsCardRecord)*SMS_ME_NUM_MAX);
    memset(g_SM_SmsRecord, 0x00, sizeof(StSmsCardRecord)*SMS_SM_NUM_MAX);
}

void UE_ReleaseSmsBuf()
{
    delete []g_ME_SmsRecord;
	delete []g_SM_SmsRecord;
}

StSmsCardRecord UE_ReadSmsCardRecord(EnLocType type, WORD nIndex)
{
    ASSERT(type == LOC_ME || type == LOC_UIM);

    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;

    if(type == LOC_ME)
    {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
    }
    else
    {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
    }
    
    ASSERT(pSmsRecord && pSmsNum && nIndex < *pSmsNum);

    return pSmsRecord[nIndex];
}

void UE_EditSmsCardRecord(EnLocType type, WORD nIndex, const StSmsRecord &record)
{
    ASSERT(type == LOC_ME || type == LOC_UIM);

    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;

    if(type == LOC_ME)
    {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
    }
    else
    {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
    }
    
    ASSERT(pSmsRecord && pSmsNum && nIndex < *pSmsNum);

    pSmsRecord[nIndex].record = record;
}

BOOL UE_DeleteSmsCardRecord(EnLocType type, WORD nIndex)
{
    ASSERT(type == LOC_ME || type == LOC_UIM);

    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;

    if(type == LOC_ME)
    {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
    }
    else
    {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
    }
    
    ASSERT(pSmsRecord && pSmsNum);

    if(nIndex < *pSmsNum)
    {
        for(WORD i = nIndex; i < *pSmsNum-1; i++)
            pSmsRecord[i] = pSmsRecord[i+1];

        memset(&pSmsRecord[i], 0x00, sizeof(StSmsCardRecord));
        (*pSmsNum)--;  
        return TRUE;
    }
    else
        return FALSE;
}

BOOL UE_AddSmsCardRecord(EnLocType type, const StSmsCardRecord &record)
{
    ASSERT(type == LOC_ME || type == LOC_UIM);

    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;
    USHORT *pSmsMax = NULL;

    if(type == LOC_ME)
    {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
        pSmsMax = &g_ME_SmsMax;
    }
    else
    {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
        pSmsMax = &g_SM_SmsMax;
    }
    
    ASSERT(pSmsRecord && pSmsNum && pSmsMax);

    if(*pSmsNum < *pSmsMax)
    {
        pSmsRecord[*pSmsNum] = record;
        (*pSmsNum)++;
        return TRUE;
    }
    else
        return FALSE;
}

BOOL UE_AddSmsCardRecord(EnLocType type, const StSmsRecord &record, const WORD &nIndex)
{
    StSmsCardRecord CardRecord;
    memset(&CardRecord, 0x00, sizeof(StSmsCardRecord));

    CardRecord.index = nIndex;
    CardRecord.record = record;

    return UE_AddSmsCardRecord(type, CardRecord);
}

WORD UE_GetUnreadSmsNum(EnLocType loctype)
{
    ASSERT(loctype == LOC_ME || loctype == LOC_UIM || loctype == LOC_MAX);

    if(!g_bSmsSyncComm)
        return 0;

    WORD i;
    WORD nUnreadNum = 0;

    if(loctype == LOC_ME || loctype == LOC_MAX)
    {
        for(i = 0; i < g_ME_SmsNum; i++)
        {
            if(g_ME_SmsRecord[i].record.state == SMS_STATE_MT_NOT_READ)
                nUnreadNum++;
        }
    }

    if(loctype == LOC_UIM || loctype == LOC_MAX)
    {
        for(i = 0; i < g_SM_SmsNum; i++)
        {
            if(g_SM_SmsRecord[i].record.state == SMS_STATE_MT_NOT_READ)
                nUnreadNum++;
        }
    }

    return nUnreadNum;
}

BOOL UE_ClearSmsFlag(EnLocType loctype, BYTE Clearflag, BOOL bAll)
{
    ASSERT(loctype == LOC_ME || loctype == LOC_UIM || loctype == LOC_MAX);

    if(!g_bSmsSyncComm)
        return FALSE;

    BOOL ret = FALSE; 
    WORD i;
    
    if(loctype == LOC_ME || loctype == LOC_MAX)
    {    
        for(i = 0; i < g_ME_SmsNum; i++)
        {
            if(g_ME_SmsRecord[i].record.flag & Clearflag)
            {
                ret = TRUE;
                g_ME_SmsRecord[i].record.flag &= (BYTE)~Clearflag;
                if(!bAll)
                    break;
            }
        }
    }

    if(loctype == LOC_UIM || loctype == LOC_MAX)
    {    
        for(i = 0; i < g_SM_SmsNum; i++)
        {
            if(g_SM_SmsRecord[i].record.flag & Clearflag)
            {
                ret = TRUE;
                g_SM_SmsRecord[i].record.flag &= (BYTE)~Clearflag;
                if(!bAll)
                    break;
            }
        }
    }
    
    return ret;
}

BOOL UE_SmsIsFull(EnLocType loctype)
{
    ASSERT(loctype == LOC_ME || loctype == LOC_UIM || loctype == LOC_MAX);

    if(loctype == LOC_ME)
        return (g_ME_SmsNum == g_ME_SmsMax);
    else if(loctype == LOC_UIM)
        return (g_SM_SmsNum == g_SM_SmsMax);
    else
        return ((g_ME_SmsNum == g_ME_SmsMax) && (g_SM_SmsNum == g_SM_SmsMax));
}

int UE_SmsFindCardRecord(EnLocType loctype, WORD nIndex)
{
    ASSERT(loctype == LOC_ME || loctype == LOC_UIM);

    if(!g_bSmsSyncComm)
        return -1;
    
    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;
    USHORT *pSmsMax = NULL;
    WORD cnt = 0;

    if(loctype == LOC_ME)
    {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
        pSmsMax = &g_ME_SmsMax;
    }
    else
    {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
        pSmsMax = &g_SM_SmsMax;
    }
    
    ASSERT(pSmsRecord && pSmsNum && pSmsMax && nIndex < *pSmsMax);

    for(cnt=0; cnt<*pSmsNum; cnt++)
    {
        if(pSmsRecord[cnt].index == nIndex)
            return cnt;
    }

    return -1;
}

int g_DataCardTotalNum = 0; //DATACARD存储器支持的总条数
int g_USimTotalNum = 0;     //USIM存储器支持的总条数
int g_DataCardUsedNum = 0; //DATACARD存储器的已用条数
int g_USimUsedNum = 0;     //USIM存储器的已用条数
StPbRecord* pUSIMPbRecord = NULL; //存储从USIM存储器中读取出来的电话薄记录
StPbRecord* pDataCardPbRecord = NULL;//存储从DATACARD存储器中读取出来的电话薄记录
bool gbPcCardInit = false;  /*PCCARD是否已读取数据至内存中，真为已读取*/
bool gbSimInit = false;     /*SIM是否已读取数据至内存中，真为已读取*/

BOOL g_bDataService = FALSE; //数据连接标志
BYTE g_nDataConnType = 0; //数据连接类型
BOOL g_bConnLimitTip = TRUE; //数据连接限制提示标记
CString    m_str_sms_Prompt = "Reading RUIM SMS,Waiting";
int gCurAutoPref = 0;
