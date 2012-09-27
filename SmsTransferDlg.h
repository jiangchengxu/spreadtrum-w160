#if !defined(AFX_SMSTRANSFERDLG_H__0833F665_9F6C_4B1C_A055_AAF5B250A209__INCLUDED_)
#define AFX_SMSTRANSFERDLG_H__0833F665_9F6C_4B1C_A055_AAF5B250A209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsTransferDlg.h : header file
//
#include "CommonStructs.h"
#include "TransparentStatic.h"
#include "Ds.h"
#include "ProgressDlg.h"
#include "Smslistctrl.h"
/////////////////////////////////////////////////////////////////////////////
// CSmsTransferDlg dialog

class CSmsTransferDlg : public CBaseDialog
{
// Construction
public:
    virtual ~CSmsTransferDlg();

    UCHAR chr;
    char MinMaxChar;
    void UpTreeSMSCount();
    void UpdateOwner();
    BOOL ProgressPos(int nPos);
    BOOL ProgressOpen(int nUpper = 0, int nStep = 0);
    BOOL ProgressClose();
    BOOL ProgressStep();
    BOOL ProgressSet(int nLower, int nUpper, int nStep);
    EnLocType GetLocTypeByStr(LPCTSTR str);
    BOOL StartTransfer(BOOL bFromLToR = TRUE);

    static void RspAtSmsQCMGD(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQCMGW(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQCMGR(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQCPMS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQCSMP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQHMSGL(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQHMSGP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //·¢ËÍAt$HMSGL(CDMA2000 add by liub)
    BOOL SndAtSmsQHMSGL(BOOL bAscii = TRUE, BOOL bConcatenate = FALSE);

    BOOL SndAtSmsQCSCA(const StSmsRecord *pRecord);
    BOOL SndAtSmsQCMGD(USHORT index);
    BOOL SndAtSmsQCMGW(StSmsRecord *pRecord = NULL);
    BOOL SndAtSmsQCMGR(USHORT index);
    BOOL SndAtSmsQCPMS(EnLocType locType1, EnLocType locType2 = LOC_MAX);
    BOOL SndAtSmsQCSMP(EnSmsQcsmpType type, BOOL bAscii = TRUE, BOOL bConcatenate = FALSE);
    BOOL SndAtSmsQCSMP(StSmsRecord *pRecord);
    BOOL SndAtSmsQHMSGP();
    CSmsTransferDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    enum EnCtrlDirection {
        CTRL_DIRECTION_LEFT = 0,
        CTRL_DIRECTION_RIGHT,
        CTRL_DIRECTION_MAX
    };

    enum EnProcType {
        PROC_TYPE_INIT = 0,
        PROC_TYPE_READ,
        PROC_TYPE_COPY,
        PROC_TYPE_MOVE,
        PROC_TYPE_MAX
    };

    CShadeButtonST TransUpToDownBtn;
    CShadeButtonST TransDownToUpBtn;
    CComboBox    m_LocCombo[CTRL_DIRECTION_MAX];
    CComboBox    m_TypeCombo[CTRL_DIRECTION_MAX];
    CSmsListCtrl    m_SmsList[CTRL_DIRECTION_MAX];
    //{{AFX_DATA(CSmsTransferDlg)
    enum { IDD = IDD_DIALOG_TRANSMS };
    int            m_nRadio;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSmsTransferDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CSmsTransferDlg)
    afx_msg void OnButtonLr();
    afx_msg void OnButtonRl();
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnSelchangeComboLeftLoc();
    afx_msg void OnSelchangeComboRightLoc();
    afx_msg void OnSelchangeComboLeftType();
    afx_msg void OnSelchangeComboRightType();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    //}}AFX_MSG
    afx_msg LRESULT OnSmsTransferProc(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT OnCreateProgress(WPARAM wParam, LPARAM lParam = 0);
    DECLARE_MESSAGE_MAP()
private:
    CHSDPADlg* m_pMainWnd;
    USHORT m_ME_SmsNum;
    USHORT m_ME_SmsMax;
    USHORT m_SM_SmsNum;
    USHORT m_SM_SmsMax;
    USHORT m_ME_SmsCnt;
    USHORT m_SM_SmsCnt;
    int m_nSelCount;
    USHORT nReadIndex;
    int m_TransItem;
    USHORT m_nDeleteIndex;
    StSmsRecord m_WriteRecord;
    USHORT m_nReadIndex;
    TCHAR m_szLoc[LOC_MAX][10];
    CImageList m_ilLsts;
    CSmsData  *m_pSmsData;
    CPbData      *m_pPbData;
    EnLocType m_locType1;
    EnLocType m_locType2;
    EnLocType m_locTypeTmp1;
    EnLocType m_locTypeTmp2;
    int m_nStep;
    int m_nUpper;
    int m_nLower;;;
    CProgressDlg* pDlg;
    EnSmsType m_TransType;
    EnLocType m_SrcLoc, m_DesLoc;
    EnCtrlDirection m_SrcDirection, m_DesDirection;
    EnProcType m_ProcType;
    void ClearAllSelectMask(EnCtrlDirection direction);
    void UpdateListCtrl(EnCtrlDirection direction);
    void UpdateComboState(EnCtrlDirection direction);
    void UpdateComboType(EnCtrlDirection direction);
    void UpdateButtonState();

    bool bSetHmsgp;
    HANDLE m_Event;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSTRANSFERDLG_H__0833F665_9F6C_4B1C_A055_AAF5B250A209__INCLUDED_)
