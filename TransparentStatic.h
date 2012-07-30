// TransparentStatic.h : header file
//
#ifndef _TRANSPARENT_H
#define _TRANSPARENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////
// CTransparentStatic
class CTransparentStatic : public CStatic
{
    DECLARE_DYNAMIC(CTransparentStatic)

public:
    CTransparentStatic();
    virtual ~CTransparentStatic();

protected:
    DECLARE_MESSAGE_MAP()
public:
    int StaticControlStyle;
    //afx_msg void OnPaint();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);    
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

};

#endif
