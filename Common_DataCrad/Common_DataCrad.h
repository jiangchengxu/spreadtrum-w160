// Common_DataCrad.h : main header file for the ONDA_ENG DLL
//

#if !defined(AFX_ONDA_ENG_H__0D4B33D8_6678_48B5_BDE5_549B05D755F2__INCLUDED_)
#define AFX_ONDA_ENG_H__0D4B33D8_6678_48B5_BDE5_549B05D755F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCommonApp
// See Common_DataCrad.cpp for the implementation of this class
//

class CCommonApp : public CWinApp
{
public:
	CCommonApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCommonApp)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONDA_ENG_H__0D4B33D8_6678_48B5_BDE5_549B05D755F2__INCLUDED_)
