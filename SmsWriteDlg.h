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

    BOOL thelastone;//������Ϸ��͵��Ƿ��ǳ����ŵ����һ��
    BOOL bSaveSendSms;//��Ƿ��ͳ�ȥ��SMS�Ƿ���Ҫ���棬��Ҫ����������flexi�������з������һ��ǰ�Ὣ�����ڶ����������ε�����
    BOOL theLMSSavenow;//���LMS������ɣ����Ա��浽������
    char MinMaxChar;
    void OnTemplateSel(WPARAM wp, LPARAM lp);
    //������λ��
    BOOL ProgressPos(int nPos);
    //�򿪽��ȿ�
    BOOL ProgressOpen(int nUpper = 0, int nStep = 0);
    //�رս��ȿ�
    BOOL ProgressClose();
    //����������
    BOOL ProgressStep();
    //���ý��ȿ�
    BOOL ProgressSet(int nLower, int nUpper, int nStep);
    //����AT$QCMGS
    BOOL SndAtSmsQCMGS(int nStep);
    //����AT$QCSMP
    BOOL SndAtSmsQCSMP();
    //����At$HMSGL(CDMA2000 add by liub)
    BOOL SndAtSmsQHMSGL();
    //����AT$QCSCA
    BOOL SndAtSmsQCSCA();
    BOOL SndAtSmsQCMMS(int param);

    //����AT$QHMSGP(���ö�����������<���ͱ���>,<����Ϣ����>,<��Ϣͷ>,<���ȼ�>)
    BOOL SndAtSmsQHMSGP();
    //��ӦAT$QHMSGP
    static void RspAtSmsQHMSGP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //��ӦAT$QCSCA
    static void RspAtSmsQCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //��ӦAT$QCSMP
    static void RspAtSmsQCSMP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //��ӦAT$QHMSGL(CDMA2000) //add by liub
    static void RspAtSmsQHMSGL(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //��ӦAT$QCMGS
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
    CString m_strLastGroupNumber; //����
    BOOL IsNumbersOver(); //�жϺ���������

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
    //flexi��������ˢ�½�����
    afx_msg LRESULT UpdateProgressStep(WPARAM wParam , LPARAM lParam = 0);
    DECLARE_MESSAGE_MAP()
private:
    void UpdateStatistic(); //����״̬
    CHSDPADlg *m_pMainWnd; //������
    void SaveSendSmsToOutbox(BOOL bSndSucc); //����SMS��������
    CProgressExDlg* pDlg;    //���ȿ�ָ��
    BOOL RetrieveGroupNum(); //���Ⱥ������
    CSmsData  *m_pSmsData; //SMS����ָ��
    int  m_nNumCount; //������
    int  m_nCurNum; //��ǰ���ͺ�
    char m_szGroupNum[SMS_GROUPSEND_MAX][PB_NUM_MAX + 2];//���뻺��
    char m_szGroupNumSendNum[PB_NUM_MAX + 12];//����ʱ���������� //flexi ���+62��ͷ�ĺ��뷢�Ͷ�����Ҫ�ĳ�0��ͷ������
    int m_nStep;
    int m_nUpper;
    int m_nLower;
    //Modified by Zhou Bin 2008.12.30
    char m_szSCNumber[SMS_SC_NUM_MAX*2 +2]; //��Ϣ���ĺ���
    EnSmsWriteType m_WriteType; //�������
};

#endif // !defined(AFX_NEWSMSDLG_H__0B65C167_4912_4A12_ADB0_B1262D8A8544__INCLUDED_)
