// SmsEdit.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "SmsEdit.h"
#include "Ds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmsEdit

CSmsEdit::CSmsEdit()
{
}

CSmsEdit::~CSmsEdit()
{
}


BEGIN_MESSAGE_MAP(CSmsEdit, CEdit)
	//{{AFX_MSG_MAP(CSmsEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsEdit message handlers

void CSmsEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default


	USHORT nAsciiLimit;
	USHORT nGbLimit;
#ifdef FEATURE_SMS_CONCATENATE
	nAsciiLimit = SMS_CONCAT_ASCII_MAX * SMS_CONCAT_SEGMENT_MAX;
	nGbLimit = SMS_CONCAT_GB_MAX * SMS_CONCAT_SEGMENT_MAX;
#else
	nAsciiLimit = SMS_CHAR_MAX;
	nGbLimit = SMS_CHINESE_CHAR_MAX;
#endif

			if(nChar < TCHAR(' ')){
				CEdit::OnChar(nChar, nRepCnt, nFlags);
				return;
			}
			CString text;
			GetWindowText(text);
			if(UnicodeIsHighStr(text.GetBuffer(text.GetLength())))
			{
				if(text.GetLength() >= nGbLimit){
					text.ReleaseBuffer();
					return;
				}						
			}
			else
			{
				if(text.GetLength() >= nAsciiLimit)
				{
					text.ReleaseBuffer();
					return;
				}
			}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
