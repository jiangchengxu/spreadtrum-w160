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
    //ѡ��洢��
    SEND_AT_SEL_STORE = 1,

    //��ȡUSIM�е���ʹ���������������
    SEND_STORE_CHECK_TOTAL,

    //��ȡUSIM�е绰����Ϣ
    SEND_STORE_READ_ALL,

} SendATState;

typedef enum {
    //���ô洢��
    SEND_AT_SET_STORE = 1,

    //дһ����Ϣ��ɾ��һ����Ϣ
    SEND_WRITE_RECORD,
    //��������ɾ��������Ϣ
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

    int m_nPCCardCount;  //ѡ���˼���PCCARD��¼
    int m_nUSIMCount; //ѡ���˼���USIM��¼

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTransferDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

//     void InitListCtrlPC(int flag = 1);             //flag����0����Ϊ����ListCtrl�ؼ���ʼ����1����Ϊ�����ListCtrl�ؼ���ʼ��
//     void ReDrawListCtrlPC(int flag = 1);         //flag����0����Ϊ��ȡPC�˵绰����Ϣ������ListCtrl�ؼ���1�����ȡPC�˵绰����Ϣ�������ListCtrl�ؼ���ʼ��
//     void InitListCtrlDCAndCard(int flag = 1);    //flag����0����Ϊ����ListCtrl�ؼ���ʼ����1����Ϊ�����ListCtrl�ؼ���ʼ��
    void ReadDataFromDCOrCard(int flag = 1);     //flag����0����Ϊ��ȡDATACARD�е����ݣ�1����Ϊ��ȡUSIM�е�����
    //  void ReDrawListCtrlDC(int flag = 1);         //flag����0����Ϊ��ȡDATACARD�е����ݣ�1����Ϊ��ȡUSIM�е�����
    BOOL isChineseCharUnicode(const TCHAR ch);
    bool CheckNameUnicode(const TCHAR* ch,int* iByte);
    BOOL isChineseChar(const char* ch);
    bool CheckName(const char* ch,int* iByte);

    void InitListCtrlUp(int flag = 0); //��ʼ�������ListCtrl, flag=0ΪPC,flag=1ΪUSB Modem, flag=2ΪUSIM
    void InitListCtrlDown(int flag = 1); //��ʼ�������ListCtrl, flag=0ΪPC,flag=1ΪUSB Modem, flag=2ΪUSIM
    void ReDrawListCtrlUp(int flag = 0); //�ػ������ListCtrl,flag=0ΪPC,flag=1ΪUSB Modem, flag=2ΪUSIM
    void ReDrawListCtrlDown(int flag = 0); //�ػ������ListCtrl,flag=0ΪPC,flag=1ΪUSB Modem, flag=2ΪUSIM
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

    //nflagΪ0����datacard,1����USIM
    bool AtWriteARecord(CString Name,CString Num, int index,int nflag);
    bool AtSetStore(int nflag);
    int GetFirstIdleIndex(StPbRecord *pStDCPbRecord, int nTotalNum);

    //case0.  flagΪ0����PCToDataCrad(DowntoUp ),flagΪ1����PcToUsim(UpToDown)
    bool PcToDataCradOrUsim(bool flag,int nItem,bool Cut,int Direction);

    // case1.0.  flagΪ0����DataCardToPc(UpToDown),flagΪ1����UsimToPc(DowntoUp)
    bool DataCradOrUsimToPc(bool flag,int nItem,bool Cut, int Direction);

    //case1.1.  flagΪ0����DataCardToUsim(UpToDown)��Ϊ1����UsimToDataCard(DownToUp)��
    bool DataCardToUsim(bool flag,int nItem,bool Cut,int Direction);

    void MallocDlgMem(int nUpper = 100,int nStep =1);
    void MallocDlgMemMove(int nUpper = 100,int nStep =1);

    void ProgressOpen(int nUpper,int nSetp);  //���ý�������״̬

    void DispatchMsg();  //����ϵͳ�����жѻ�����Ϣ

    void ProgressClose(int nSetp);

    //�������������ʱת�������ж�ʱ�ĺ�����������ֵΪ���ʾת���жϣ�Ϊ�ٱ�ʾת������
    bool TransferBreak();

    CPbData  *m_pPbData;

    CWaitDlg *m_pDlg;   //ʵ����CWaitDlg
    CWaitDlg *m_pWaitDlg;   //ʵ����CWaitDlg

    int m_ComboxCurIndexUp;
    int m_ComboxCurIndexDown;
    int m_nPbSelPosDlgFlag;  //PBSelDlg�ϵĸ�ѡ���Ƿ�ѡ��
    int m_nPbSelPosDlgSelFlag;//PBSelDlg�ϵ���������洢λ��ѡ����

    int m_nCurSelCtrl;  //�������������²�listctrl�ؼ��ĵ�����1�����ϲ���2�����²���0������ѡ��
    int m_nUp;
    int m_nStep;

    UINT m_nSelTotalItem ;  //�û�ѡ���������
    int  m_nCurItem ;       //��ǰ����ת��������
    HANDLE m_Event;
    CHSDPADlg* m_HsdpaDlg;
    BOOL m_PBSelect;  //�洢����ѡ���־,1:��ѡ��0:δѡ��


    HWND m_Hwnd;

public:
    bool m_bEndPbTransfer;   //���ڱ�ʶת���Ƿ����
    int m_nStoreFlag;  // //����USIM�洢����DATACARD�洢����0ΪDATACARD,1ΪUSIM��
    int m_ReadDataFromDCState;  //���ڱ����ص������е����
    CString m_StrSource;  //���ڴ洢AT�����ַ���
    int m_nIndex;        //�ӵڶ�����ʼѭ����ȡ����
    bool m_ReadDataCardAndUSIMStatus;  //��ȡUSIM��DATACARD�洢��ʱ�Ƿ���ִ���trueΪ��ȷ��falseΪ����
    int m_nGroupSelected; //�绰������ѡ�е�Ⱥ��
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERDLG_H__86DC596A_E115_4B81_B198_C5CACE648EA4__INCLUDED_)
