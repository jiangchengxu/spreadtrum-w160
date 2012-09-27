// PinClckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "PinClckDlg.h"
#include "ds.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPinClckDlg dialog


CPinClckDlg::CPinClckDlg(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CPinClckDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPinClckDlg)
    m_valKey = _T("");
    m_cRemainTime = _T("");
    //m_cPinClckTip = _T("");
    //}}AFX_DATA_INIT
    m_nBkTag = 1;
//	m_pHandlePin = &(((CHSDPADlg*)(((CHSDPAApp*)(AfxGetApp()))->m_pMainWnd))->m_cHandlePin);
    m_pHandlePin = &(((CHSDPADlg*)AfxGetMainWnd())->m_cHandlePin);
}

/*tag为0代表Enable PIN，为1代表Disable PIN*/
CPinClckDlg::CPinClckDlg(CWnd* pParent,UINT tag)
    : CBaseDialog(CPinClckDlg::IDD, pParent)
{
    m_valKey = _T("");
    m_cRemainTime = _T("");
    //m_cPinClckTip = _T("");
    m_nBkTag = 1;
    m_nTag = tag;
    m_pHandlePin = &(((CHSDPADlg*)AfxGetMainWnd())->m_cHandlePin);
}

CPinClckDlg::~CPinClckDlg()
{
    ::CloseHandle(m_hPinEvt);
}

void CPinClckDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPinClckDlg)
    DDX_Text(pDX, IDC_EDIT_PIN_INPUT, m_valKey);
//	DDV_MaxChars(pDX, m_valKey, 8);

    DDX_Text(pDX, IDC_STATIC_PIN_REMAIN, m_cRemainTime);

    //DDX_Text(pDX, IDC_STATIC_PIN_CLCKTIP, m_cPinClckTip);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPinClckDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CPinClckDlg)
    ON_BN_CLICKED(IDC_BUTTON_SUBMIT, OnButtonSubmit)
    //}}AFX_MSG_MAP
    //ON_WM_PAINT()
END_MESSAGE_MAP()

/*
激活PIN 码保护功能

如果PIN 码正确并且当前PIN 码的状态为关闭，则操作成功----[提示]PIN码保护成功开启
如果当前PIN 码的状态为开启，则不操作--------------------[提示]PIN码保护已经开启
如果PIN 码错误，输入允许次数将减一----------------------
*/
/////////////////////////////////////////////////////////////////////////////
// CPinClckDlg message handlers

BOOL CPinClckDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();
    m_bFirst = TRUE;
    //SetSkin(g_SetData.Setup_nSkinStyle);

//根据操作，设置窗口标题
    CString strT1,strT2;
    strT1.LoadString(IDS_PIN_ENABLE_T);
    strT2.LoadString(IDS_PIN_DISABLE_T);

    switch (m_nTag) {
    case 0: {
        //SetWindowText("Enable PIN");//rc_update
        SetWindowText(strT1);
        break;
    }

    case 1: {
        //SetWindowText("Disable PIN");//rc_update
        SetWindowText(strT2);
        break;
    }

    default: {
        break;
    }
    }
    //UpdateData(TRUE);
    m_cRemainTime.Format(_T("%d"),m_pHandlePin->m_nRemainTimes);
    UpdateData(FALSE);

    m_hPinEvt = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    // TODO: Add extra initialization here
    ((CEdit*)GetDlgItem(IDC_EDIT_PIN_INPUT))->SetLimitText(PIN_LEN);

    return TRUE;
    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/*EnablePIN码和Disable PIN码的回调函数*/
void CPinClckDlg::AtRespCLCK(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CPinEx * pHandle = (CPinEx*)(((CPinClckDlg*)pWnd)->m_pHandlePin);
    CPinClckDlg *pdlgClck = (CPinClckDlg *)pWnd;
    CString strRet=strArr[0];

    //根据执行结果判断
    if(-1 != strRet.Find(_T("OK"),0)) {
        pHandle->m_nRemainTimes = PIN_RMTIMES_MAX;
    } else if(-1 != strRet.Find(_T("incorrect password"),0)) {
        if (0 < pHandle->m_nRemainTimes) {
            pHandle->m_nRemainTimes--;
        }
    } else if(-1 != strRet.Find(_T("SIM PUK required"),0)) {
        pHandle->m_nRemainTimes = 0;
        pHandle->m_nSimStat = CPIN_SIM_PUK_REQUIRED;
    }
    SetEvent(pdlgClck->m_hPinEvt);
}


void CPinClckDlg::OnOK()
{
    CBaseDialog::OnOK();
}

void CPinClckDlg::OnButtonSubmit()
{
    int bRet;
    UpdateData();
    int len = m_valKey.GetLength();
    if ((len > 8)||(len < 4)) {
        //AfxMessageBox("PIN Length Error!");//rc_update
        AfxMessageBox(IDS_PIN_ERR_LEN);
        return;
    }

    if(0 == m_nTag) { //Enable PIN
        if(1 == m_pHandlePin->m_nPinStat) {
            //AfxMessageBox("PIN Already Enable!");//rc_update
            AfxMessageBox(IDS_PIN_WR_ENABLE);
            return;
        }
        bRet = OnEnablePin();
    } else if(1 == m_nTag) { //Disable PIN
        if(0 == m_pHandlePin->m_nPinStat) {
            //AfxMessageBox("PIN Already Disable!");//rc_update
            AfxMessageBox(IDS_PIN_WR_DISABLE);
            return;
        }
        bRet = OnDisablePin();
    }
    m_cRemainTime.Format(_T("%d"),m_pHandlePin->m_nRemainTimes);
    UpdateData(FALSE);

    if(SYNCINITFUNCRET_DONE == bRet) {
        if(3 == m_pHandlePin->m_nRemainTimes) { //设置成功，则剩余的次数为3
            AfxMessageBox(IDS_SMS_SETSUCC);
            OnOK();
        } else if (0 == m_pHandlePin->m_nRemainTimes) {
            //被锁了
            AfxMessageBox(IDS_PIN_LOCK);
            OnOK();
        } else {
            //AfxMessageBox("Incorrect PIN Code!");//rc_update
            AfxMessageBox(IDS_PIN_ERR_PIN1);
        }
    }
}

/*打开PIN码验证*/
int CPinClckDlg::OnEnablePin()
{
    const TCHAR AtClck1[] = _T("AT+CLCK=\"SC\",1,\"");
    const TCHAR Suffix[] = _T("\"\x0d\x00");
    TCHAR szAtBuf[512] = {0};
    CString strSend = AtClck1;
    strSend += m_valKey;
    strSend += Suffix;
    wcscpy(szAtBuf, strSend);

    char szAtAscBuf[512] = {0};
    WCharToChar(szAtBuf, szAtAscBuf);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf))) {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCLCK, this);

        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hPinEvt, 5000)) {
            ::ResetEvent(m_hPinEvt);

            return SYNCINITFUNCRET_DONE;
        } else {
            ::ResetEvent(m_hPinEvt);

            return SYNCINITFUNCRET_RSP_TO;
        }
    } else {
        return SYNCINITFUNCRET_SND_ERR;
    }
}

/*关闭PIN码验证*/
int CPinClckDlg::OnDisablePin()
{
    const TCHAR AtClck1[]=_T("AT+CLCK=\"SC\",0,\"");
    const TCHAR Suffix[]=_T("\"\x0d\x00");
    TCHAR szAtBuf[512] = {0};
    CString strSend = AtClck1;
    strSend += m_valKey;
    strSend += Suffix;
    wcscpy(szAtBuf, strSend);
    char szAtAscBuf[512] = {0};
    WCharToChar(szAtBuf, szAtAscBuf);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf))) {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCLCK, this);

        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hPinEvt, 5000)) {
            ::ResetEvent(m_hPinEvt);
            return SYNCINITFUNCRET_DONE;
        } else {
            ::ResetEvent(m_hPinEvt);
            return SYNCINITFUNCRET_RSP_TO;
        }
    }
    return SYNCINITFUNCRET_SND_ERR;

}
