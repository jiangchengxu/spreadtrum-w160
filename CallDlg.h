#if !defined(AFX_CALLDLG1_H__80A2C584_F371_4BA5_8B72_59C2845871CD__INCLUDED_)
#define AFX_CALLDLG1_H__80A2C584_F371_4BA5_8B72_59C2845871CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallDlg.h : header file
//
#include "TabedDialog.h"
#include "PbData.h"
#include "CallHistoryDlg.h"
#include "HistData.h"
#include "SliderCtrlEx.h"
#include "ScreenStatic.h"
#include "NumEdit.h"
#include "Ds.h"
#include "HistListCtrl.h"
#include "SelPosition.h"

/////////////////////////////////////////////////////////////////////////////
// CCallDlg dialog

#define     WM_ATCDVRESULT        (WM_USER+31)
#define     IDT_DIAL_TIME         (WM_USER+32)
#define     WM_CALLING_FAILED      (WM_USER+33)

class CCallDlg : public CTabedDialog
{
private:
	void OnKey0();
	void OnButtonAdd();
	CCallHistoryDlg* m_pHistDlg;

	BOOL OutCallorInCall;//表示：0-主叫 ; 1-被叫,暂时用于界面现实号码问题
    CPbData   *m_pPbData;
	BOOL       m_bSortAscendArr[HISTKIND_MAX];               // Is sort ascending?
    EnHistCol  m_SortColArr[HISTKIND_MAX];                   // current sorted column
	WORD       m_DspIndexArr[HISTKIND_MAX][HIST_ITEM_MAX];  // 数据索引 add by liub 1030 for call history
    CHistData *m_pHistData;
    COleDateTime m_CurrentTime;
	EnHistKind m_DispFilter;

	//add by liub 1111 for callhistory begin
	void ResetDspIndexArr(EnHistKind type);
	void UpdateHistSortList(EnHistKind type);
	void BubbleSort(EnHistKind type, EnHistCol col);
	void ReverseSortIndexArr(EnHistKind type);
	void SortHistList(EnHistKind type, EnHistCol col);
	void InitDspIndexArr();
	//add by liub 1111 end
public:
    ////////////////////////////////////////////////////////////////////////// add by liub for 模拟键盘按键
	CSkinButton m_key1;
	CSkinButton m_key2;
	CSkinButton m_key3;
	CSkinButton m_key4;
	CSkinButton m_key5;
	CSkinButton m_key6;
	CSkinButton m_key7;
	CSkinButton m_key8;
	CSkinButton m_key9;
	CSkinButton m_key0;
	CSkinButton m_keyStar;
	CSkinButton m_keyHash;
	CSkinButton m_keyContacts;
	CSkinButton m_keydial;
	CSkinButton m_keyend;
    CSkinButton m_keyclear;
	
	CSkinButtonResource m_btnkeyres;

	CFont m_fontIdle; //添加字体用于模拟屏幕上使用（待机时） add by liub
    BOOL IdleFont;//判断待机时字体是否设置过 add by liub

	////////////////////////////////////////////////////////////////////////// liub add end
	CHAR m_cKeyDown;

    BOOL       m_blIsInCalling;
    COleDateTime m_CallHistTimeStamp;
    CTimeSpan m_CallHistTimeSpan;
    EnHistKind m_CallHistKind;
    CString	   m_CallHistNumber;    //用于通话记录存储号码使用

	BOOL m_bWaitingCall;

public:
    DECLARE_DYNAMIC(CCallDlg)
	void SendVTS(TCHAR* cvts);
    void SetVolume();
	void InitListCtrl();//add by liub 1030 for call history
	void InitTabCtrl();//add by liub 1030 for call history
	static void AtRespVts(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);    
    static void AtRespDial(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespHangup(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespAnswer(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespReject(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    void DispStatus(CString& sDur);
    void ResetCallHistData();
    void SaveRecordToCallHist();
    CCallDlg(CWnd* pParent = NULL);   // standard constructor
    virtual BOOL CanSwitchMe();
    virtual int DealwithResponseProc(LPARAM lParam, WPARAM wParam);
	bool SelContact(int nItem, CString* strNumber);//add by liub
	void DispCallHistory();//add by liub 1030 for call history
	void EndCallbyHANDSET();
public:
	BOOL EnableButton(BOOL bEnable, BOOL bSimCard = TRUE);
    //{{AFX_DATA(CCallDlg)
	enum { IDD = IDD_DIALOG_CALL };
    CSliderCtrlEx m_sliderVol;
    CNumEdit      m_EditNum;
    CString       m_strEditNum;//此处为拨打的真实号码
    CString       m_strStatus;
	CString       m_Numtemp;//add  by liub （由于屏幕需要换行，所以另定义一个变量为显示时使用）
  //  CScreenStatic m_screenInfo;//liub test
	CEnTabCtrl    m_tabHistCall;//add by liub 1030 for call history
	CHistListCtrl m_lstHistCall;//add by liub 1030 for call history
	//}}AFX_DATA
protected:
	CImageList m_ilTabs;//add by liub 1030 for call history
    CImageList m_ilLsts;//add by liub 1030 for call history
	
// Overrides
    //{{AFX_VIRTUAL(CCallDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    // Generated message map functions
    //{{AFX_MSG(CCallDlg)
    afx_msg void OnButtonCallhistory();
    afx_msg void OnButtonNum1();
    afx_msg void OnButtonNum2();
    afx_msg void OnButtonNum3();
    afx_msg void OnButtonNum4();
    afx_msg void OnButtonNum5();
    afx_msg void OnButtonNum6();
    afx_msg void OnButtonNum7();
    afx_msg void OnButtonNum8();
    afx_msg void OnButtonNum9();
    afx_msg void OnButtonNumStar();
    afx_msg void OnButtonNum0();
    afx_msg void OnButtonNumHash();
    afx_msg void OnButtonCall();
	afx_msg void OnButtonCall2();//add by liub
    afx_msg void OnButtonHungup();
    afx_msg void OnButtonClear();
    afx_msg void OnButtonSms();
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnButtonGetphbitem();
	afx_msg void OnChangeEditNumber();
	afx_msg void OnUpdateEditNumber();
	afx_msg void OnSelchangeTabHistCall(NMHDR* pNMHDR, LRESULT* pResult);//add by liub 1030 for call history
	afx_msg void OnColumnclickListHistCall(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSave();//add by liub for SaveButton
	afx_msg void OnButtonDelete();//add by liub for delete
	afx_msg void AnswerIncomingCall(WPARAM wParam, LPARAM lParam);//add by liub
	//}}AFX_MSG
    afx_msg LRESULT OnDial(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT SetCallState(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT OnGetNumFromPhb(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMenuItemRightClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveHist(WPARAM wParam, LPARAM lParam = 0);//add by liub
	afx_msg LRESULT OnCallingFailed(WPARAM wParam, LPARAM lParam = 0);//add by zhou bin
	
	
    DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_CALLDLG1_H__80A2C584_F371_4BA5_8B72_59C2845871CD__INCLUDED_)
