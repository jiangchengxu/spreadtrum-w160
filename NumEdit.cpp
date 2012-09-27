// NumEdit.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "NumEdit.h"
#include "Ds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumEdit

CNumEdit::CNumEdit()
{
    m_bFlag = false;
}

CNumEdit::~CNumEdit()
{
}


BEGIN_MESSAGE_MAP(CNumEdit, CEdit)
    //{{AFX_MSG_MAP(CNumEdit)
    ON_WM_CHAR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumEdit message handlers

void CNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    BOOL bIsValid;

    /* m_bFlag为true表示打电话模块的调用, false表示发短信模块的调用 */
    if(m_bFlag) {
        bIsValid = IsValidCallNumChar(nChar);
    } else {
        bIsValid = IsValidSmsNumChar(nChar);
    }

    if(bIsValid)
        CEdit::OnChar(nChar, nRepCnt, nFlags);
}