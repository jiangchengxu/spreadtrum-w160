#if !defined(AFX_NEWSMSDLG_H__0B65C167_4912_4A12_ADB0_B1262D8A8544__INCLUDED_)
#define AFX_NEWSMSDLG_H__0B65C167_4912_4A12_ADB0_B1262D8A8544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsWriteDlg.h : header file
//
#include "SmsEdit.h"
#include "NumEdit.h"
#include "TransparentStatic.h"
#include "Ds.h"
#include "CommonStructs.h"    // Added by ClassView
#include "ProgressExDlg.h"
#include "HSDPADlg.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CSmsWriteDlg dialog

class CSmsWriteDlg : public CBaseDialog
{
// Construction
public:

    BOOL thelastone;//标记马上发送的是否是长短信的最后一条
    BOOL bSaveSendSms;//标记发送出去的SMS是否需要保存，主要是用于区别flexi长短信中发送最后一条前会将倒数第二条保存两次的问题
    BOOL theLMSSavenow;//标记LMS发送完成，可以保存到发件箱
    char MinMaxChar;
    void OnTemplateSel(WPARAM wp, LPARAM lp);
    //进度条位置
    BOOL ProgressPos(int nPos);
    //打开进度框
    BOOL ProgressOpen(int nUpper = 0, int nStep = 0);
    //关闭进度框
    BOOL ProgressClose();
    //进度条步进
    BOOL ProgressStep();
    //设置进度框
    BOOL ProgressSet(int nLower, int nUpper, int nStep);
    //发送AT$QCMGS
    BOOL SndAtSmsQCMGS(int nStep);
    //发送AT$QCSMP
    BOOL SndAtSmsQCSMP();
    //发送At$HMSGL(CDMA2000 add by liub)
    BOOL SndAtSmsQHMSGL();
    //发送AT$QCSCA
    BOOL SndAtSmsQCSCA();
    BOOL SndAtSmsQCMMS(int param);

    //发送AT$QHMSGP(设置短信其他参数<发送报告>,<短消息类型>,<消息头>,<优先级>)
    BOOL SndAtSmsQHMSGP();
    //响应AT$QHMSGP
    static void RspAtSmsQHMSGP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //响应AT$QCSCA
    static void RspAtSmsQCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //响应AT$QCSMP
    static void RspAtSmsQCSMP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //响应AT$QHMSGL(CDMA2000) //add by liub
    static void RspAtSmsQHMSGL(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //响应AT$QCMGS
    static void RspAtSmsQCMGS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQCMMS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);

    CSmsWriteDlg(CWnd* pParent = NULL,
                 LPCTSTR lpszNumber = NULL,
                 LPCTSTR lpszContent = NULL,
                 EnSmsWriteType writeType = SMS_WRITE_NONE,
                 LPCTSTR lpszSCNum = NULL);

    void SaveSMS(WPARAM wParam, LPARAM lParam);

    void SaveSMStoDraft();
    void SaveSMStoOutbox();

// Dialog Data
    //{{AFX_DATA(CSmsWriteDlg)
    enum { IDD = IDD_DIALOG_WRITESMS };
    CSmsEdit    m_editSmsDetails;
    CNumEdit    m_editNumber;

    CString     m_strNumber;
    CString     m_strSmsDetails;
    CString     m_strStatistic;
    CString     m_strSegNum;

    //}}AFX_DATA
    /*
    CTransparentStatic m_static_number;
    CTransparentStatic m_static_content;
    CTransparentStatic m_static_statistic;
    */
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSmsWriteDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    CString m_strLastGroupNumber; //号码
    BOOL IsNumbersOver(); //判断号码数超长

    // Generated message map functions
    //{{AFX_MSG(CSmsWriteDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnChangeEditSmsdetails();
    afx_msg void OnButtonSmsPb();
    afx_msg void OnButtonSmsPb2();
    afx_msg void OnButtonSmsSend();
    afx_msg void OnButtonSmsSave();
    afx_msg void OnButtonSmsClear();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnUpdateEditNumber();
    afx_msg void OnUpdateEditSmsdetails();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnTemplates();
    //}}AFX_MSG
    afx_msg LRESULT OnReceiveNum(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCreateProgress(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT OnSmsSendPorc(WPARAM wParam, LPARAM lParam = 0);
    //flexi发长短信刷新进度条
    afx_msg LRESULT UpdateProgressStep(WPARAM wParam , LPARAM lParam = 0);
    DECLARE_MESSAGE_MAP()
private:
    void UpdateStatistic(); //更新状态
    CHSDPADlg *m_pMainWnd; //主窗口
    void SaveSendSmsToOutbox(BOOL bSndSucc); //保存SMS到发件箱
    CProgressExDlg* pDlg;    //进度框指针
    BOOL RetrieveGroupNum(); //获得群发号码
    CSmsData  *m_pSmsData; //SMS数据指针
    int  m_nNumCount; //号码数
    int  m_nCurNum; //当前发送号
    char m_szGroupNum[SMS_GROUPSEND_MAX][PB_NUM_MAX + 2];//号码缓存
    char m_szGroupNumSendNum[PB_NUM_MAX + 12];//发送时的真正号码 //flexi 针对+62开头的号码发送短信需要改成0打头的需求
    int m_nStep;
    int m_nUpper;
    int m_nLower;
    //Modified by Zhou Bin 2008.12.30
    char m_szSCNumber[SMS_SC_NUM_MAX*2 +2]; //消息中心号码
    EnSmsWriteType m_WriteType; //发送类别
};

#endif // !defined(AFX_NEWSMSDLG_H__0B65C167_4912_4A12_ADB0_B1262D8A8544__INCLUDED_)
