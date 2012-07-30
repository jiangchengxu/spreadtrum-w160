// --------------------------------------------------------------------------
//
// TextProgressCtrl.h
//
/// TextProgressCtrl interface.
///
/// @file
//
// Copyright (c) 2003-2004 QUALCOMM Inc.
// QUALCOMM Proprietary.
// All Rights Reserved.
//
// --------------------------------------------------------------------------

#if !defined(AFX_TEXTPROGRESSCTRL_H__E3CC5D08_FB55_429F_8558_BC66E71D98C6__INCLUDED_)
#define AFX_TEXTPROGRESSCTRL_H__E3CC5D08_FB55_429F_8558_BC66E71D98C6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// --------------------------------------------------------------------------
// TextProgressCtrl class declaration
///
/// The TextProgressCtrl is an extension of the CProgressCtrl common control
/// that allows user defined text to be placed in the progress bar that 
/// inverts in areas where the bar covers it.
// --------------------------------------------------------------------------
class TextProgressCtrl : public CProgressCtrl
{
public:
   TextProgressCtrl();
   virtual ~TextProgressCtrl();

public:
   int SetPos(int nPos);
   int OffsetPos(int nPos);
   int SetStep(int nStep);
   int StepIt();

   void SetText(CString text) { m_oldText = m_text; m_text = text; }
   COLORREF GetBarFgColour() { return m_barFg; }
   COLORREF GetBarBgColour() { return m_barBg; }
   COLORREF GetTextFgColour() { return m_textFg; }
   COLORREF GetTextBgColour() { return m_textBg; }
   void SetBarFgColour(COLORREF color) { m_barFg = color; }
   void SetBarBgColour(COLORREF color) { m_barBg = color; }
   void SetTextFgColour(COLORREF color) { m_textFg = color; }
   void SetTextBgColour(COLORREF color) { m_textBg = color; }

private:
   int m_pos;
   int m_stepSize;
   CString m_text;
   CString m_oldText;
   int m_barWidth;
   COLORREF m_barFg;
   COLORREF m_barBg;
   COLORREF m_textFg;
   COLORREF m_textBg;

   //{{AFX_MSG(TextProgressCtrl)
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnPaint();
   afx_msg void OnSize(UINT nType, int cx, int cy);
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPROGRESSCTRL_H__E3CC5D08_FB55_429F_8558_BC66E71D98C6__INCLUDED_)
