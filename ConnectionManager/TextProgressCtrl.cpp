// --------------------------------------------------------------------------
//
// TextProgressCtrl.cpp
//
/// TextProgressCtrl implementation.
///
/// @file
//
// Copyright (c) 2003-2004 QUALCOMM Inc.
// QUALCOMM Proprietary.
// All Rights Reserved.
//
// --------------------------------------------------------------------------

#include "stdafx.h"
#include "TextProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(TextProgressCtrl, CProgressCtrl)
    //{{AFX_MSG_MAP(TextProgressCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
TextProgressCtrl::TextProgressCtrl() :
    m_pos(0),
    m_stepSize(1),
    m_barWidth(-1),
    m_barFg(::GetSysColor(COLOR_HIGHLIGHT)),
    m_barBg(::GetSysColor(COLOR_MENU)),
    m_textFg(::GetSysColor(COLOR_HIGHLIGHT)),
    m_textBg(::GetSysColor(COLOR_WINDOW))
{}

// --------------------------------------------------------------------------
// dtor
// --------------------------------------------------------------------------
TextProgressCtrl::~TextProgressCtrl()
{}

// --------------------------------------------------------------------------
// SetPos
//
/// Sets the progress bar control's current position as specified by nPos and
/// redraws the bar to reflect the new position.
/// The position of the progress bar control is not the physical location on
/// the screen, but rather is between the upper and lower range indicated in
/// SetRange.
///
/// @param pos - New position of the progress bar control.
///
/// @returns int - The previous position of the progress bar control.
// --------------------------------------------------------------------------
int TextProgressCtrl::SetPos(int pos)
{
    if (!::IsWindow(m_hWnd)) {
        return -1;
    }

    // get the lower and upper values of the range
    int lower = 0;
    int upper = 0;
    GetRange(lower, upper);

    // save the current position
    int oldPos = m_pos;

    // set the new position
    m_pos = pos;

    // calculate the bar width
    CRect clientRect;
    GetClientRect(clientRect);
    double percent = (double)(m_pos - lower) / ((double)(upper - lower));
    int barWidth = (int) (percent * clientRect.Width());

    // redraw if bar width or text changed
    if (barWidth != m_barWidth || m_text != m_oldText) {
        m_barWidth = barWidth;
        m_oldText = m_text;
        RedrawWindow();

    }

    return oldPos;
}

// --------------------------------------------------------------------------
// OffsetPos
//
/// Advances the progress bar control's current position by the increment
/// specified by pos by calling SetPos.
///
/// @param pos - Amount to advance the position.
///
/// @returns int - The previous position of the progress bar control.
// --------------------------------------------------------------------------
int TextProgressCtrl::OffsetPos(int pos)
{
    return SetPos(m_pos + pos);
}

// --------------------------------------------------------------------------
// SetStep
//
/// Specifies the step increment for a progress bar control. The step
/// increment is the amount by which a call to StepIt increases the progress
/// bar's current position.
/// The default step increment is 1.
///
/// @param step - New step increment.
///
/// @returns int - The previous step increment.
// --------------------------------------------------------------------------
int TextProgressCtrl::SetStep(int step)
{
    int oldStep = m_stepSize;
    m_stepSize = step;
    return oldStep;
}

// --------------------------------------------------------------------------
// StepIt
//
/// Advances the current position for a progress bar control by the step
/// increment by calling SetPos.
///
/// @returns int - The previous position of the progress bar control.
// --------------------------------------------------------------------------
int TextProgressCtrl::StepIt()
{
    return SetPos(m_pos + m_stepSize);
}

// --------------------------------------------------------------------------
// OnEraseBkgnd
//
/// Action to take when the CWnd object background needs erasing (for
/// example, when resized). It is called to prepare an invalidated region for
/// painting.
// --------------------------------------------------------------------------
BOOL TextProgressCtrl::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

// --------------------------------------------------------------------------
// OnSize
//
/// Action to take when the window is resized.
// --------------------------------------------------------------------------
void TextProgressCtrl::OnSize(UINT nType, int cx, int cy)
{
    CProgressCtrl::OnSize(nType, cx, cy);

    // force redraw on SetPos call
    m_barWidth = -1;
}

// --------------------------------------------------------------------------
// OnPaint
//
/// Action to take when when Windows or an application makes a request to
/// repaint a portion of an application's window.
// --------------------------------------------------------------------------
void TextProgressCtrl::OnPaint()
{
    CPaintDC dc(this);

    // get the lower and upper values of the range
    int lower = 0;
    int upper = 0;
    GetRange(lower, upper);

    if (lower >= upper) {
        return;
    }

    // get the client rect and rects for the filled and empty portions
    CRect clientRect;
    GetClientRect(clientRect);
    CRect filledRect(clientRect);
    CRect emptyRect(clientRect);

    // calculate the percentage filled
    double percent = (double)(m_pos - lower) / ((double)(upper - lower));

    // set the size of the filled and empty portions
    filledRect.right = filledRect.left + (int)((filledRect.right - filledRect.left) * percent);
    emptyRect.left = filledRect.right;

    // fill filled and empty portions with selected color
    dc.FillSolidRect(filledRect, m_barFg);
    dc.FillSolidRect(emptyRect, m_barBg);

    // add text if any
    if (!m_text.IsEmpty()) {
        // set font to fit vertically in bar
        HFONT font;
        font = CreateFont(clientRect.Height()-8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL);
        dc.SelectObject(font);
        dc.SetBkMode(TRANSPARENT);

        // draw text over the filled portion
        CRgn rgn;
        rgn.CreateRectRgn(filledRect.left, filledRect.top, filledRect.right, filledRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_textBg);

        dc.DrawText(m_text, clientRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        rgn.DeleteObject();

        // draw text over the empty portion
        rgn.CreateRectRgn(emptyRect.left, emptyRect.top, emptyRect.right, emptyRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_textFg);

        dc.DrawText(m_text, clientRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        rgn.DeleteObject();
    }
}
