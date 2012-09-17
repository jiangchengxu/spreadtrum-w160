#if !defined(AFX_PHONEBOOKDLG_H__D8DFEB98_CE2D_4DB4_B7B9_A6A768C95033__INCLUDED_)
#define AFX_PHONEBOOKDLG_H__D8DFEB98_CE2D_4DB4_B7B9_A6A768C95033__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhoneBookDlg.h : header file
//
#include "TabedDialog.h"
#include "MyListCtrl.h"
#include "CommonStructs.h"    // Added by ClassView
#include "PbData.h"
#include "SearchContactDlg.h"
#include "HeaderCtrlEx.h"
#include "ExDataExch.h"
#include "WaitDlg.h"
#include "Ds.h"
#include "Resource.h"


/////////////////////////////////////////////////////////////////////////////
// CPhoneBookDlg dialog

class CPhoneBookDlg : public CTabedDialog
{
protected:
    //void ImportFromVcf(TCHAR **pPara, DWORD nLen, LPVOID pHandle);
	void ImportFromVcf(char **pPara, DWORD nLen, LPVOID pHandle);
    //void ImportFromCsv(TCHAR **pPara, DWORD nLen, LPVOID pHandle);
	void ImportFromCsv(char **pPara, DWORD nLen, LPVOID pHandle);
// Construction
public:
    void InitListCtrl(int flag);
    CPhoneBookDlg(CWnd* pParent = NULL);   // standard constructor
    ~CPhoneBookDlg();
    virtual int DealwithResponseProc(LPARAM lParam, WPARAM wParam);
    LRESULT ReDrawListCtrl(WPARAM wParam = NULL, LPARAM lParam = NULL);
	LRESULT ReDrawTreeCtrl(WPARAM wParam = NULL, LPARAM lParam = NULL);
	LRESULT OnUpdateTreeContactNum(WPARAM wParam = NULL, LPARAM lParam = NULL);
	void ReadDataFromLoc(int flag = 1);     //flag����0����Ϊ��ȡDATACARD�е����ݣ�1����Ϊ��ȡUSIM�е�����
	static void AtRespReadState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
	LRESULT OnDelDCRecord(WPARAM wParam = NULL, LPARAM lParam = NULL);
	LRESULT SearchContactInList(WPARAM wParam=0, LPARAM lParam=0);
	LRESULT GetListTotalCount(WPARAM wParam=0, LPARAM lParam=0);
	static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	bool CheckNameUnicode(const TCHAR* ch,int* iByte);
	bool CPhoneBookDlg::CheckNameUnicode(const TCHAR* ch);
	BOOL isChineseChar(const char* ch);
	BOOL isChineseCharUnicode(const TCHAR* ch);

	int GetFirstIdleIndex(StPbRecord *pStDCPbRecord, int nTotalNum);
	void UpdateContactNumForTreeCtrl(HTREEITEM hRoot,int groupType);
	void ImportGroup();

	void ProgressOpen(int nUpper,int nSetp);  //���ý�������״̬
	void ReadDataFromSimOrUSB(int nFlag);

// Dialog Data
    //{{AFX_DATA(CPhoneBookDlg)
    enum { IDD = IDD_DIALOG_PHONEBOOK };
    CMyListCtrl    m_lstPhoneBook;
	CTreeCtrl      m_TreePhoneBook;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPhoneBookDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPhoneBookDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnButtonSendsms();
    afx_msg void OnButtonCall();
    afx_msg void OnButtonAdd();
    afx_msg void OnButtonModify();
    afx_msg void OnButtonDelete();
    afx_msg void OnRclickListPhonebook(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonTransfer();
    afx_msg void OnButtonSearch();
    afx_msg void OnButtonDeleteAll();
	afx_msg void OnButtonGroup();
    afx_msg void OnDblclkListPhonebook(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonExport();
    afx_msg void OnColumnclickListPhonebook(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonImport();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeTree(WPARAM wParam, LPARAM lParam);
	afx_msg void OnModemInOut(WPARAM wParam, LPARAM lParam);

//	void DeleteSIMRecord();
	//}}AFX_MSG
    afx_msg LRESULT OnSearchItem(WPARAM wParam=0, LPARAM lParam=0);
    afx_msg LRESULT OnAddContact(WPARAM wParam=0, LPARAM lParam=0);
	
	

	void ExportARecord(IGenerator* phbHandle,int iIndex);
	bool SelContact(int nItem, CString* strNumber);
	BOOL isChsChar(const char* ch);
	BOOL isChsCharUnicode(const TCHAR* ch);
	bool CheckPbName(const char* ch);
	bool CheckPbNameUnicode(const TCHAR* ch);	
	bool CheckPbNumber(CString strNum);
    CString GetPicPath();
	void MallocDlgMem(int nUpper = 100,int nStep =1);
    DECLARE_MESSAGE_MAP()
	bool AtWriteARecord(CString Name,CString Num, int index,int nflag);
	bool AtWriteARecord2(CString Name,CString Num, int index,int nflag);
	static void AtRespWriteRecordState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);	
	static void AtRespWriteRecordState2(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
	bool DeleteAllPCLOCItem();
	bool DeletePCCardAndUSIMLOCItem(int nDelCount,DWORD* LocAddress,int flag);
	bool CanMatch( CString src, CString substr );
	void UpdateButtonStatus(bool bEnable = true);
	bool CheckStatForTrans();  //����λ��ͨѶǰ�����SIM��״̬�Ͷ˿��Ƿ�����

	bool WriteToUSIM(StPbRecord *Record,int Flag);
		bool TransferBreak();  

	
	

private:	
    CPbData  *m_pPbData;
    int ColumnBeClicked;
    CSearchContactDlg *m_pDlg;
	//CTransferDlg    *m_transDlg;
	CString m_StrPathName;

	CString m_strCurSelGroup;
	CString m_strUSIM; //��ʾ��sim������  WCDMA: USIM, CDMA2000: R-UIM
	CString m_strPCCard; //USB Modem
	typedef enum
	{
		PCLOC = 0,
		PCCardLOC,
		USIMLOC,
		ALLLOC,
		PCGROUP
	} enuLoc;
	CWaitDlg *m_pWaitDlg;   //ʵ����CWaitDlg
	HWND m_Hwnd;
	CImageList m_ilLsts;
	HANDLE m_Event;
	BOOL m_bEditSim;
	int m_iContactType;//���������������޸ĵ绰����¼

public:
	////////////////////////////////////////////////////////////////////////// PB buttons begin
	CShadeButtonST m_btnNew;
	CShadeButtonST m_btnEdit;
	CShadeButtonST m_btnDelete;
	CShadeButtonST m_btnGroup;
	CShadeButtonST m_btnSMS;
	CShadeButtonST m_btnTransfer;
	CShadeButtonST m_btnSearch;
	CShadeButtonST m_btnDial;
	////////////////////////////////////////////////////////////////////////// 
	BOOL SetupCall();
	int* m_pCRecordIndex;  //�����ɾ����PC�������index
	DWORD* m_PCRecordAds;
	DWORD* m_PCCardRecordAds;
	DWORD* m_USIMRecordAds;
	DWORD* m_RecordTempAds;
	int m_nPCCount;
	int m_nPCCardCount;  //ѡ���˼���PCCARD��¼
	int m_nUSIMCount; //ѡ���˼���USIM��¼
	int m_nDCCount;
	int nCurSelIndex; //��ǰѡ��Ĵ洢��
	CString m_StrSource;
	CString m_StrSearchKeyofName;
	CString m_StrSearchKeyofMobile;
	int m_nStoreFlag;  // //����USIM�洢����DATACARD�洢����0ΪDATACARD,1ΪUSIM��
	int m_ReadDataFromDCState;  //���ڱ����ص������е����
	int m_nIndex;        //�ӵڶ�����ʼѭ����ȡ����
	bool m_ReadDataCardAndUSIMStatus;  //��ȡUSIM��DATACARD�洢��ʱ�Ƿ���ִ���trueΪ��ȷ��falseΪ����

	CProgressCtrl	m_Progress;

	BOOL m_bNewGroupFlagUSIM,m_bNewGroupFlagUSB;

	HTREEITEM m_ht_PC,m_ht_USIM,m_ht_PCCard;
	CString m_strGPBeforeUpdated;
	CString m_strGPAfterUpdated;
	CString m_strGPDelete;


//    CHeaderCtrlEx *m_headerCtrl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONEBOOKDLG_H__D8DFEB98_CE2D_4DB4_B7B9_A6A768C95033__INCLUDED_)
