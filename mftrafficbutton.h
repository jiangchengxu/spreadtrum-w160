
#if !defined(AFX_MFTRAFFICBUTTON_H__9F2EABAA_E8C7_48B4_94A5_EC279905F50C__INCLUDED_)
#define AFX_MFTRAFFICBUTTON_H__9F2EABAA_E8C7_48B4_94A5_EC279905F50C__INCLUDED_

#include "MFNetTraffic.h"	// Added by ClassView
#include "dlgconnect.h"



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MFTrafficButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MFTrafficButton window

//VOID CALLBACK InterfaceChanged(int newInterface);

//typedef FARPROC INTERFACECHANCEDPROC;
typedef struct _TRAFFIC_ENTRY_
{
	double value;
	BOOL connected;
} TRAFFICENTRY;

#define PLOTGRANULATRITY 5		// Defines the width of the rectangle representing a bar in the diagram

#define NETUPDATESPEED	1000	// Value for the statistics timer
#define GRIDUPDATESPEED 50		// Value vor the grid timer
#define GRIDTIMER		1		// ID for the grid timer
#define NETTIMER		2		// ID for the statistics timer

#define GRIDXRESOLUTION	10		// Distance for grid in x direction
#define GRIDYRESOLUTION	10		// Distance for grid in y direction
#define GRIDSCROLLXSPEED -1		// How fast grid scrolls in x direction
#define GRIDSCROLLYSPEED 0		// How fast grid scrolls in y direction





typedef VOID (CALLBACK* INTERFACECHANCEDPROC)(int);

class MFTrafficButton : public CButton
{
// Construction
public:
	MFTrafficButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MFTrafficButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetAdaptiveScaling(BOOL adaptive);
	void SetInterfaceNumber(int interfacenumber);
	enum traffictype
	{
		Traffic_Total		= 0,
		Traffic_Incoming	= 1,
		Traffic_Outgoing	= 2
	};

	void SelectTrafficType(int trafficType);
	void SetInterfaceNumberNotificationFunction(INTERFACECHANCEDPROC callfunct);
	void SetUpdateSpeed(UINT netspeed, UINT gridspeed);
	void ReInit(int newInterface);
	void ReInit(RECT newSize);
	void setDrawFlag(BOOL);
	virtual ~MFTrafficButton();

	BOOL isVista();

	// Generated message map functions
protected:
	//{{AFX_MSG(MFTrafficButton)
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void InterfaceHasChanged();

	INTERFACECHANCEDPROC interfaceCallBack;

	MFNetTraffic m_cTrafficClass;

	CFont	smallFont;
	CBrush	colorbrush;

	COLORREF red, green, back, cyan, darkblue, darkgray, white, black, lightgreen, darkgreen;
	CBitmap	colorbrushbmp;
	CPen	GridPen;
	CSize	TGSize;

	CBrush	greenbrush;
	CBrush	redbrush;
	CPoint	orgBrushOrigin; 

	RECT	TrafficDrawRectangle;
	RECT	TrafficDrawUpdateRectangle;

	

	DWORD	TrafficEntries;

	BOOL	initalized;
	BOOL	isOnline;
	BOOL	brushInitalized;

	CRgn	ShapeWNDRegion;
	CRgn	ShapeDCRegion;

	double	MaxTrafficAmount;
	CString	ToggleStatusText;

	int SelectedInterface;

	BOOL useAdaptiveScale;

	TRAFFICENTRY* TrafficStats;

	int gridxstartpos;		
	int gridystartpos;
	int plotgranularity;
	BOOL drawFlag;

	// Public modification variables
public:
	int gridxresolution;		// The size of grid raster
	int gridyresolution;
	int gridscrollxspeed;		// Scroll speed of the grid
	int gridscrollyspeed; 
	int netupdatespeed;			// Should be set via SetUpdateSpeet method
	int gridupdatespeed;
	CString CurrentTraffic;
	double m_dCurrTraffic;
	CString MaximalTraffic;
	CString AllTraffic;

	CString TotalTraffic;

	CString CurrentTotalTraffic; //本次连接的总数据流量
	CDlgConnect* m_pDlgConnect;
	double currentTotalTraffic;


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFTRAFFICBUTTON_H__9F2EABAA_E8C7_48B4_94A5_EC279905F50C__INCLUDED_)
