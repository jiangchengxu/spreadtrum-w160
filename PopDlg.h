#if !defined(AFX_POPDLG_H__0E845D0A_E52A_4F12_B62D_9367BA9481D5__INCLUDED_)
#define AFX_POPDLG_H__0E845D0A_E52A_4F12_B62D_9367BA9481D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopDlg.h : header file
//
#define IDT_POPDLG_TIMER    (WM_USER+120)
#define IDT_DEPOPDLG_TIMER    (WM_USER+122)

#include "HSDPA.h"

/////////////////////////////////////////////////////////////////////////////
// CPopDlg dialog
class CPopDlg : public CBaseDialog//,CDlgHelper
{
public:
    DECLARE_DYNAMIC(CPopDlg)
protected:
    int m_nWidth;            //  window width
    int m_nHeight;           //  window height 
    int m_nDelay;
    int m_dx; 
    int m_dy;
public:
	int m_nPos;
    DWORD m_eState;
// Construction
public:
	BOOL IsPopUp();
	void ShowButton(BOOL bInComCallOrSms = TRUE);
    CPopDlg(UINT nWaveID);
    virtual ~CPopDlg();

    BOOL Create(UINT nID, CWnd* pWnd, int nPos)
    {
        if(GetSafeHwnd() != NULL)
            return 0;

        if(m_nWaveID == IDR_WAVE_SMS)
            NULL;
            //PlaySound((LPCTSTR)"wav\\sms.wav", NULL, SND_ASYNC | SND_NOSTOP);
            //PlaySound((LPCTSTR)IDR_WAVE_SMS, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
        else if(m_nWaveID == IDR_WAVE_CALL)
            PlaySound(_T("wav\\call.wav"), NULL, SND_ASYNC | SND_LOOP);
            //PlaySound((LPCTSTR)IDR_WAVE_CALL, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC | SND_LOOP);

        m_bIsPopUp = CBaseDialog::Create(nID, pWnd);
		m_nPos = nPos;
        return m_bIsPopUp;
    }

    BOOL DestroyWindow()
    {
        if(GetSafeHwnd() == NULL)
            return 0;

        m_bIsPopUp = !CBaseDialog::DestroyWindow();
		m_nPos = 0;

        if(m_nWaveID == IDR_WAVE_CALL)
            PlaySound((LPCTSTR)NULL, NULL, SND_PURGE);

        return !m_bIsPopUp;
    }

    void PostNcDestroy()
    {
//        delete this;
    }

    enum { IDD = IDD_DIALOG_POP_TIP };
	CString m_strTipInfo;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPopDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPopDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnButtonAnswer();
    afx_msg void OnButtonReject();
	afx_msg void OnButtonPview();
	afx_msg void OnButtonPcancel();
	afx_msg void OnDestroy();
	//}}AFX_MSG
    afx_msg LRESULT OnDestroyWnd(WPARAM wParam, LPARAM lParam = 0);    
    DECLARE_MESSAGE_MAP()
private:
	BOOL m_bIsPopUp;
    UINT m_nWaveID;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_POPDLG_H__0E845D0A_E52A_4F12_B62D_9367BA9481D5__INCLUDED_)
