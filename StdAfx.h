// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CAEB7654_F204_4190_86E6_6C20A472146A__INCLUDED_)
#define AFX_STDAFX_H__CAEB7654_F204_4190_86E6_6C20A472146A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER 0x0500
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers		

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxadv.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>        // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>            // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>        // MFC socket extensions
#include <afxtempl.h>

#include "UserMsgIdDef.h"
#include "Constants.h"
#include "BaseDialog.h"
#include "TabDialog.h"
#include "CommonStructs.h"
#include "PopDlg.h"
#pragma warning(disable:4244 4018)
#pragma warning(disable:4768)
#pragma warning(disable:4786)

#ifndef DELETEIF
#define DELETEIF(p) 	\
if (p) {				\
	delete p;			\
	p = NULL;			\
};
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CAEB7654_F204_4190_86E6_6C20A472146A__INCLUDED_)
