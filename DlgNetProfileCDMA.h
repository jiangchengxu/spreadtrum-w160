#if !defined(AFX_DLGNETPROFILECDMA_H__CCA7BC6A_E4E8_4146_A77C_C1926DB0212E__INCLUDED_)
#define AFX_DLGNETPROFILECDMA_H__CCA7BC6A_E4E8_4146_A77C_C1926DB0212E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNetProfileCDMA.h : header file
//
#include "ConnProfile.h"
#include "NumEdit.h"
#include "Ras.h"
#include "CommonStructs.h"
#include "Ds.h"



/////////////////////////////////////////////////////////////////////////////
// CDlgNetProfileCDMA dialog

class CDlgNetProfileCDMA : public CBaseDialog
{
// Construction
public:
	CDlgNetProfileCDMA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNetProfileCDMA)
	enum { IDD = IDD_DIALOG_PROFILE_CDMA2000 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNetProfileCDMA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CNumEdit m_NumEditPhoneNumber;
	
    CString  m_strConnStatus;
	CString	 m_strPassword;
	CString	 m_strUserName;
	CString	 m_strPhoneNumber;
	CString  m_strAPN;
	
	BOOL     m_bSetDefault;
    int      m_nAuthRadio;
	int nCGDCONTType;	//0:CGDCONT set, 1:CGDCONT?, 2:CGDCONT=?
	CString m_strLastAPN;
	//EnNetWorkSrv m_OldnetworkSrv;
	//EnNetWorkType m_Oldnetwork;
	EnNetCallLog m_Oldnetwork; //added by GL
	BOOL BApnSet; //是否已经设置过连接
#ifdef FEATURE_NDIS_SUPPORT
	int m_connect_type;
#endif

protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDlgNetProfile)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	afx_msg OnPaint();
	
	
	afx_msg void OnButtonSaveconn();
    afx_msg void OnButtonNewconn();
    afx_msg void OnButtonDeleteconn();
	afx_msg void OnButtonDial();
    afx_msg void OnButtonDetails();
    afx_msg void OnSelchangeComboConnectName();
    afx_msg void OnButtonCalllog();
	afx_msg LRESULT OnUpdateInternetDlg(WPARAM wParam, LPARAM lParam = 0);
	
#ifdef FEATURE_NDIS_SUPPORT
	afx_msg void OnRadioNdis();
	afx_msg void OnRadioRas();
#endif
	DECLARE_MESSAGE_MAP()

protected:
	CComboBox  m_cmbConnectionName;
	
private:
	CConnProfile *m_pConnProf;
    CConnProfile *m_pNdisConnProf;
	CStdioFile m_file;
	//连接中的Profile名称
	CString m_strActiveProfile;
	
	void UpdateButton();
	
public:
	//接入点设置
	static void RspAtCGDCONT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
	BOOL SndAtCGDCONT(CString strConnPoint);
	void SetAPN(CString strPLMN);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNETPROFILECDMA_H__CCA7BC6A_E4E8_4146_A77C_C1926DB0212E__INCLUDED_)
