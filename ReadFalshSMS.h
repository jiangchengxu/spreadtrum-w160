#if !defined(AFX_READFALSHSMS_H__FCF95DD5_5831_4646_9FFA_B3E3EEBF53F7__INCLUDED_)
#define AFX_READFALSHSMS_H__FCF95DD5_5831_4646_9FFA_B3E3EEBF53F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReadFalshSMS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReadFalshSMS dialog

class CReadFalshSMS : public CBaseDialog
{
// Construction
public:
	CReadFalshSMS(CWnd* pParent = NULL);   // standard constructor

	int DlgDestoryTime;
// Dialog Data
	//{{AFX_DATA(CReadFalshSMS)
	enum { IDD = IDD_DIALOG_READ_CFTSMS };
	CString    m_strCFTContent;
    CString    m_strCFTDatetime;
    CString    m_strCFTName;
    CString    m_strCFTNumber;
    CString    m_strCFTByteCount;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void UpdataSMS();//add by liub
	void OnCancel();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReadFalshSMS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReadFalshSMS)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
   // virtual void OnCancel();
//	afx_msg void OnTimer(UINT nIDEvent);
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CSmsData  *m_pSmsData; //SMS数据指针
    CPbData      *m_pPbData; //PBM数据指针
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READFALSHSMS_H__FCF95DD5_5831_4646_9FFA_B3E3EEBF53F7__INCLUDED_)
