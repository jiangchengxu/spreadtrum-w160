#if !defined(AFX_SMSDLG_H__15F005D7_0AB0_4C5F_B439_D5BE88B786D9__INCLUDED_)
#define AFX_SMSDLG_H__15F005D7_0AB0_4C5F_B439_D5BE88B786D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsDlg.h : header file
//

#include "TabedDialog.h"
#include "TabCtrl\entabCtrl.h"
#include "SmsListCtrl.h"
#include "CommonStructs.h"
#include "SmsData.h"
#include "SmsReadDlg.h"
#include "ProgressDlg.h"
#include "ReadFalshSMS.h"

/////////////////////////////////////////////////////////////////////////////
// CSmsDlg dialog

class CSmsDlg : public CTabedDialog
{
// Construction
public:

	CComboBox  m_cmbLocFilter;
	CString VoiceMailCountSMSDlg;
	////////////////////////////////////////////////////////////////////////// add by liub for SMS buttons 1121 begin
	CShadeButtonST m_btnnew;
	CShadeButtonST m_btnReply;
	CShadeButtonST m_btnForward;
	CShadeButtonST m_btnDelete;
	CShadeButtonST m_btnExtract;
	CShadeButtonST m_btnDial;
	CShadeButtonST m_btnSearch;
	CShadeButtonST m_btnEdit;
	CShadeButtonST m_btnArchive;
	CShadeButtonST m_btnResume;
	CShadeButtonST m_btnTransfer;
	////////////////////////////////////////////////////////////////////////// add by liub end
	StSmsRecord m_SmsRecord[SMS_TYPE_ALL][SMS_RECORD_MAX]; //��ǰ��¼��Ŀ

//	BOOL UnKnowSms;

	int ForwardSMSpriority;
#ifdef FEATURE_HAIER_SMS
	int sms_format;
#endif
    CSmsData  *sms_full_motify;//added by lly
	CString smstree_node_name;//add  by liub
	CString voicesmscount;//add by liub for voicemail
	int MEsmscount;//add by liub
	int SMsmscount;
	BOOL SetupCall(); //�������
    CSmsDlg(CWnd* pParent = NULL);   // standard constructor
    void DispSms(); //��ʾSMS
    void UpdateSmsSortList(EnSmsType type, EnLocFilter locFilter); //��������
    void InitDspIndexArr(EnLocFilter locFilter); //��ʼ����������
    virtual int DealwithResponseProc(LPARAM lParam, WPARAM wParam);
    BOOL ProgressPos(int nPos); //������λ��
    BOOL ProgressOpen(int nUpper = 0, int nStep = 0); //�򿪽�����
    BOOL ProgressClose(); //�رս�����
    BOOL ProgressStep(); //����������
    BOOL ProgressSet(int nLower, int nUpper, int nStep);    //���ý����� 
    //AT$QCMGD��Ӧ
	static void RspAtSmsQCMGD(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);    
    //AT$QCMGR��Ӧ
    static void RspAtSmsQCMGR(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //AT$QCPMS��Ӧ
    static void RspAtSmsQCPMS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum); 
    BOOL SndAtSmsRead();
    BOOL SndAtOffSmsRead();
	BOOL SndAtCVMR();//add by liub
	BOOL SndAtCVMD();//add by liub
    static void RspAtSmsRead(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
	static void RspAtCVMR(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);//add by liub for TATA voicemail
	static void RspAtCVMD(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);//add by liub for TATA voicemail
    //����AT$QCMGD
	BOOL SndAtSmsQCMGD(USHORT index);
    //����AT$QCMGR
    BOOL SndAtSmsQCMGR(USHORT index);
    //����AT$QCPMS
    BOOL SndAtSmsQCPMS(EnLocType locType1, EnLocType locType2 = LOC_MAX); 
    void OnInitSMSRecord();
	BOOL InitSMSTree();//liub_modify
	HTREEITEM OnSmsUpdataNumforSmsTree(HTREEITEM hRoot);
	void UpdateSortListWhenAddDelete(EnSmsType type, EnLocType loc);
	void OnSelchangeCombolocfilterForOut();
	void Updatatree();

	void SmsDelete_flexi(int index_flexi);

	//����ʾ�������
	StSmsRecord GetTotalSmsRecordFromDspIndex(EnSmsType type, EnLocFilter locFilter, WORD dspIndex);
	
    enum EnProcType
    {
        PROC_TYPE_INIT = 0,
        PROC_TYPE_READ,
        PROC_TYPE_DELETE,
        PROC_TYPE_MAX
    };

// Dialog Data
    //{{AFX_DATA(CSmsDlg)
    enum { IDD = IDD_DIALOG_SMS };
	CSmsListCtrl	m_lstSms;
	CEnTabCtrl	    m_tabSms;
	CTreeCtrl	m_treeSms;
	CString  m_SmsContent;

	
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSmsDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
	
    CImageList m_ilTabs;
    CImageList m_ilLsts;
    // Generated message map functions
    //{{AFX_MSG(CSmsDlg)
    virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditSmsSearch();
    afx_msg void OnSelchangeTabSms(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonSmsRead();
    afx_msg void OnButtonSmsReply();
    afx_msg void OnButtonSmsDial();
    afx_msg void OnButtonSmsDelete();
    afx_msg void OnButtonSmsNew();
    afx_msg void OnButtonSmsForward();
    afx_msg void OnButtonSmsAbstract();
    afx_msg void OnButtonSmsDelall();
	afx_msg void OnclkListSms(NMHDR* pNMHDR, LRESULT* pResult);//liub_modify//��ӵ���SMS�б���ʾ�����ݹ���
    afx_msg void OnButtonSmsEdit();
    afx_msg void OnButtonSmsTransfer();
    afx_msg void OnButtonSmsExport();
	afx_msg void OnButtonSmsSearch();
	afx_msg void OnButtonSmsArchive();
    afx_msg void OnButtonSmsResume();
    afx_msg void OnColumnclickListSms(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnCusDrawListSms(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCombolocfilter();
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangedSMSTree(NMHDR* pNMHDR, LRESULT* pResult);//liubo_modify SMStreeCtrl��Ӧ����
	afx_msg void OnDestroy();
	afx_msg void DeleteCDSSMS();//add by liub for delete CDSSms

	//}}AFX_MSG
    afx_msg LRESULT OnSendSms(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT OnSaveSms(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT OnReadSms(WPARAM wParam, LPARAM lParam = 0);    
	afx_msg LRESULT OnReadFlashSMS(WPARAM wParam, LPARAM lParam = 0);//add by liub for CFT
    afx_msg LRESULT OnViewSms(WPARAM wParam, LPARAM lParam = 0);    
    afx_msg LRESULT OnMenuItemClick(WPARAM wParam = 0, LPARAM lParam = 0);
    afx_msg LRESULT OnSmsOperateProc(WPARAM wParam, LPARAM lParam = 0);    
    afx_msg LRESULT OnCreateProgress(WPARAM wParam, LPARAM lParam = 0); 
	afx_msg void OnVScrollContent();
	afx_msg LRESULT OnModemStausChange(WPARAM wParam =0, LPARAM lParam = 0);
    DECLARE_MESSAGE_MAP()
private:
	void UpdateListAfterDelete(); //ɾ��������б�
    //��ɾ����������б�
	
    //�����SMS����
	WORD GetTotalSmsNum(EnSmsType type, EnLocFilter locFilter);  

	//�������༭SMS��¼
    void EditTotalSmsRecordFromDspIndex(EnSmsType type, EnLocFilter locFilter, WORD dspIndex, const StSmsRecord &record);//add by liub
    void InitFont(); //��ʼ������
    void InitTabCtrl(); //��ʼ����ǩ
    void InitListCtrl(); //��ʼ���б�
    void InitCombo(); //��ʼ����Ͽ�
    void ResetDspIndexArr(EnSmsType type, EnLocFilter locFilter); //������������
    void BubbleSort(EnSmsType type, EnLocFilter locFilter, SMS_LISTCTRL_HEADERINDEX col);//ð������
    void ReverseSortIndexArr(EnSmsType type, EnLocFilter locFilter); //��������
    void SortSmsList(EnSmsType type, EnLocFilter locFilter, SMS_LISTCTRL_HEADERINDEX col);//����SMS�б�

    CFont m_BoldFont; //����
    CSmsReadDlg *m_pReadDlg; //��SMS�Ի���
    CSmsData  *m_pSmsData; //SMS����
    CPbData      *m_pPbData; //PB����
    EnSmsType m_DispFilter; //���͹���
    EnLocFilter m_LocFilter; //λ�ù���
    BOOL      m_bSortAscendArr[SMS_TYPE_ALL][LOCFILTER_MAX];                // Is sort ascending?
    SMS_LISTCTRL_HEADERINDEX  m_SortColArr[SMS_TYPE_ALL][LOCFILTER_MAX];    // current sorted column
    StDspEntityType m_DspEntityArr[SMS_TYPE_ALL][LOCFILTER_MAX][SMS_TOTAL_RECORD_MAX];  // ��������
    int m_nStep;
    int m_nUpper;
    int m_nLower;;;
    CProgressDlg* pDlg;
    EnLocType m_locType1;
    EnLocType m_locType2;
    EnLocType m_locTypeTmp1;
    EnLocType m_locTypeTmp2;
    USHORT nReadIndex; //������
    USHORT m_nDeleteIndex; //ɾ����
    USHORT m_nReadIndex; //������
    EnProcType m_ProcType; //��������
    WORD m_DelDspIndexArr[LOC_MAX][SMS_TOTAL_RECORD_MAX]; //ɾ��SMS��������
    WORD m_DelNumArr[LOC_MAX]; //ɾ��SMS��Ŀ����
	CString m_strSearch; //��������

	//wyw_0103
	CImageList *m_imagelistSMSTree;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSDLG_H__15F005D7_0AB0_4C5F_B439_D5BE88B786D9__INCLUDED_)
