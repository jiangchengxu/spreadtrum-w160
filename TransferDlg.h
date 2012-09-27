#if !defined(AFX_TRANSFERDLG_H__86DC596A_E115_4B81_B198_C5CACE648EA4__INCLUDED_)
#define AFX_TRANSFERDLG_H__86DC596A_E115_4B81_B198_C5CACE648EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransferDlg.h : header file
//
#include "MyListCtrl.h"
#include "Ds.h"
#include "NumEdit.h"
#include "HSDPADlg.h"
#include "WaitDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CTransferDlg dialog

typedef enum {
    //选择存储器
    SEND_AT_SEL_STORE = 1,

    //获取USIM中的已使用条数和最大条数
    SEND_STORE_CHECK_TOTAL,

    //获取USIM中电话本信息
    SEND_STORE_READ_ALL,

} SendATState;

typedef enum {
    //设置存储器
    SEND_AT_SET_STORE = 1,

    //写一条信息或删除一条信息
    SEND_WRITE_RECORD,
    //用于连续删除多条信息
    SEND_DELETE_RECORD,

} ATState;

class CWaitDlg;
class CTransferDlg : public CDialog
{
// Construction
public:
    CTransferDlg(CWnd* pParent = NULL);   // standard constructor
    ~CTransferDlg();
#if 0
    static void AtRespReadState(CWnd* pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespWriteRecordState(CWnd *pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
#else
    static void AtRespReadState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespWriteRecordState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
#endif

    static void AtRespReadState(CWnd* pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespWriteRecordState(CWnd *pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);

//    void ReDrawListCtrl();
// Dialog Data
    //{{AFX_DATA(CTransferDlg)
    enum { IDD = IDD_DIALOG_TRANSFER };
    CProgressCtrl	m_Progress;
    CStatic    m_StaUp;
    CStatic    m_StaDown;
    CMyListCtrl    m_lstPhoneBookUP;
    CMyListCtrl    m_lstPhoneBookDOWN;
    CComboBox m_ComboSelUp;
    CComboBox m_ComboSelDown;
    //}}AFX_DATA

    int m_nPCCardCount;  //选择了几条PCCARD记录
    int m_nUSIMCount; //选择了几条USIM记录

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTransferDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

//     void InitListCtrlPC(int flag = 1);             //flag等于0代表为上面ListCtrl控件初始化，1代表为下面的ListCtrl控件初始化
//     void ReDrawListCtrlPC(int flag = 1);         //flag等于0代表为读取PC端电话本信息至上面ListCtrl控件，1代表读取PC端电话本信息至下面的ListCtrl控件初始化
//     void InitListCtrlDCAndCard(int flag = 1);    //flag等于0代表为上面ListCtrl控件初始化，1代表为下面的ListCtrl控件初始化
    void ReadDataFromDCOrCard(int flag = 1);     //flag等于0代表为读取DATACARD中的数据，1代表为读取USIM中的数据
    //  void ReDrawListCtrlDC(int flag = 1);         //flag等于0代表为读取DATACARD中的数据，1代表为读取USIM中的数据
    BOOL isChineseCharUnicode(const TCHAR ch);
    bool CheckNameUnicode(const TCHAR* ch,int* iByte);
    BOOL isChineseChar(const char* ch);
    bool CheckName(const char* ch,int* iByte);

    void InitListCtrlUp(int flag = 0); //初始化上面的ListCtrl, flag=0为PC,flag=1为USB Modem, flag=2为USIM
    void InitListCtrlDown(int flag = 1); //初始化下面的ListCtrl, flag=0为PC,flag=1为USB Modem, flag=2为USIM
    void ReDrawListCtrlUp(int flag = 0); //重画上面的ListCtrl,flag=0为PC,flag=1为USB Modem, flag=2为USIM
    void ReDrawListCtrlDown(int flag = 0); //重画下面的ListCtrl,flag=0为PC,flag=1为USB Modem, flag=2为USIM
    // Generated message map functions
    //{{AFX_MSG(CTransferDlg)
    virtual BOOL OnInitDialog();

    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnButtonCopySome();
    afx_msg void OnButtonCopySomeUp();
    afx_msg void OnButtonCutSome();
    afx_msg void OnButtonCutSomeUp();
    afx_msg void OnSelchangeComboSeldown();
    afx_msg void OnSelchangeComboSelup();
    virtual void OnCancel();
    afx_msg void OnClickListUp(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickListDown(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonCopyAll();
    afx_msg void OnButtonCopyAllUp();
    afx_msg void OnButtonCutAll();
    afx_msg void OnButtonCutAllUp();
    afx_msg void OnDblclkListUp(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkListDown(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

private:
    void UpdateButtonStatus();

    //nflag为0代表datacard,1代表USIM
    bool AtWriteARecord(CString Name,CString Num, int index,int nflag);
    bool AtSetStore(int nflag);
    int GetFirstIdleIndex(StPbRecord *pStDCPbRecord, int nTotalNum);

    //case0.  flag为0代表PCToDataCrad(DowntoUp ),flag为1代表PcToUsim(UpToDown)
    bool PcToDataCradOrUsim(bool flag,int nItem,bool Cut,int Direction);

    // case1.0.  flag为0代表DataCardToPc(UpToDown),flag为1代表UsimToPc(DowntoUp)
    bool DataCradOrUsimToPc(bool flag,int nItem,bool Cut, int Direction);

    //case1.1.  flag为0代表DataCardToUsim(UpToDown)，为1代表UsimToDataCard(DownToUp)。
    bool DataCardToUsim(bool flag,int nItem,bool Cut,int Direction);

    void MallocDlgMem(int nUpper = 100,int nStep =1);
    void MallocDlgMemMove(int nUpper = 100,int nStep =1);

    void ProgressOpen(int nUpper,int nSetp);  //设置进度条的状态

    void DispatchMsg();  //处理系统队列中堆积的消息

    void ProgressClose(int nSetp);

    //当来电或来短信时转储被迫中断时的后续处理，返回值为真表示转储中断，为假表示转储继续
    bool TransferBreak();

    CPbData  *m_pPbData;

    CWaitDlg *m_pDlg;   //实例化CWaitDlg
    CWaitDlg *m_pWaitDlg;   //实例化CWaitDlg

    int m_ComboxCurIndexUp;
    int m_ComboxCurIndexDown;
    int m_nPbSelPosDlgFlag;  //PBSelDlg上的复选框是否被选中
    int m_nPbSelPosDlgSelFlag;//PBSelDlg上的三个号码存储位置选择标记

    int m_nCurSelCtrl;  //用于区分是上下部listctrl控件的单击。1代表上部，2代表下部。0代表无选择。
    int m_nUp;
    int m_nStep;

    UINT m_nSelTotalItem ;  //用户选择的总条数
    int  m_nCurItem ;       //当前正在转储的条数
    HANDLE m_Event;
    CHSDPADlg* m_HsdpaDlg;
    BOOL m_PBSelect;  //存储介质选择标志,1:已选择0:未选择


    HWND m_Hwnd;

public:
    bool m_bEndPbTransfer;   //用于标识转储是否结束
    int m_nStoreFlag;  // //区分USIM存储器和DATACARD存储器，0为DATACARD,1为USIM。
    int m_ReadDataFromDCState;  //用于标明回调函数中的入口
    CString m_StrSource;  //用于存储AT命令字符串
    int m_nIndex;        //从第二条开始循环读取数据
    bool m_ReadDataCardAndUSIMStatus;  //读取USIM或DATACARD存储器时是否出现错误。true为正确，false为错误
    int m_nGroupSelected; //电话本界面选中的群组
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERDLG_H__86DC596A_E115_4B81_B198_C5CACE648EA4__INCLUDED_)
