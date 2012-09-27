#if !defined(AFX_SMSREADDLG_H__45B22B4F_1D28_47C3_9D15_7A98C603755A__INCLUDED_)
#define AFX_SMSREADDLG_H__45B22B4F_1D28_47C3_9D15_7A98C603755A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsReadDlg.h : header file
//
#include "TransparentStatic.h"
#include "CommonStructs.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CSmsReadDlg dialog

class CSmsReadDlg : public CBaseDialog
{
// Construction
public:
    CSmsReadDlg(EnSmsType type, EnLocFilter locFilter, WORD index, StDspEntityType (*pDspIndex)[LOCFILTER_MAX][SMS_TOTAL_RECORD_MAX], CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSmsReadDlg)
    enum { IDD = IDD_DIALOG_READSMS };
    CString    m_strContent;
    CString    m_strDatetime;
    CString    m_strName;
    CString    m_strNumber;
    CString    m_strByteCount;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSmsReadDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CSmsReadDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnPaint();
    afx_msg void OnCancelMode();
    afx_msg void OnButtonFirst();
    afx_msg void OnButtonPrev();
    afx_msg void OnButtonNext();
    afx_msg void OnButtonLast();
    afx_msg void OnUpdateEditSmsContent();
    //}}AFX_MSG
    afx_msg LRESULT OnUpdateReadSms(WPARAM wParam, LPARAM lParam = 0);
    DECLARE_MESSAGE_MAP()
private:
    //获得SMS总条数
    WORD GetTotalSmsNum(EnSmsType type, EnLocFilter locFilter);
    //从索引获得SMS记录
    StSmsRecord GetTotalSmsRecordFromDspIndex(EnSmsType type, EnLocFilter locFilter, WORD dspIndex);
    //从索引编辑SMS记录
    void EditTotalSmsRecordFromDspIndex(EnSmsType type, EnLocFilter locFilter, WORD dspIndex, const StSmsRecord &record);
    //更新SMS显示
    void UpdateSmsDsp();
    //设置标题
    void SetGroupBoxTitle();
    //更新按钮状态
    void UpdateButtonStatus();
    //显示SMS记录
    void DspSmsRecord();
    CSmsData  *m_pSmsData; //SMS数据指针
    CPbData      *m_pPbData; //PBM数据指针
    EnSmsType m_DispFilter; //SMS类型
    EnLocFilter m_LocFilter; //SMS位置
    WORD      m_nCurIndex; //当前索引
    StDspEntityType (*m_pDspEntity)[LOCFILTER_MAX][SMS_TOTAL_RECORD_MAX]; //显示索引表
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSREADDLG_H__45B22B4F_1D28_47C3_9D15_7A98C603755A__INCLUDED_)
