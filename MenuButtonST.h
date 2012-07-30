// MenuButtonST.h: interface for the CMenuButtonST class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUST_H__4A26BCF6_D52F_4625_8C1F_620A397E8E54__INCLUDED_)
#define AFX_MENUST_H__4A26BCF6_D52F_4625_8C1F_620A397E8E54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BtnST.h"

class CMenuButtonST : public CButtonST  
{
public:
	void SetMenuPopup(BOOL bl);
	CMenuButtonST();
	virtual ~CMenuButtonST();

protected:
	void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};

#endif // !defined(AFX_MENUST_H__4A26BCF6_D52F_4625_8C1F_620A397E8E54__INCLUDED_)
