/*******************************************

	MFTrafficButton

********************************************/

// MFNetTraffic.h: interface for the MFNetTraffic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFNETTRAFFIC_H__9CA9C41F_F929_4F26_BD1F_2B5827090494__INCLUDED_)
#define AFX_MFNETTRAFFIC_H__9CA9C41F_F929_4F26_BD1F_2B5827090494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Ds.h"
#include "dlgconnect.h"


class MFNetTraffic  
{
public:
	enum TrafficType 
	{
		AllTraffic		= 388,
 		IncomingTraffic	= 264,
 		OutGoingTraffic	= 506
//  		AllTraffic		= 4462,
//  		IncomingTraffic	= 4472,
//  		OutGoingTraffic	= 4482
	};

	CDlgConnect* m_pDlgConnect;

	void SetTrafficType(int trafficType);
	DWORD	GetInterfaceTotalTraffic(int index);
	BOOL	GetNetworkInterfaceName(CString *InterfaceName, int index);
	int		GetNetworkInterfacesCount();
	double	GetTraffic(int interfaceNumber);
	double  GetTrafficVista(int interfaceNumber);

	DWORD	GetInterfaceBandwidth(int index);
	MFNetTraffic();
	virtual ~MFNetTraffic();
	BOOL isVista();

		double CurrentTotalTraffic;//�������ӵ�����������
private:
	BOOL		GetInterfaces();
	double		lasttraffic;
	CStringList Interfaces;
	CList < DWORD, DWORD &>		Bandwidths;
	CList < DWORD, DWORD &>		TotalTraffics;

	BOOL InitTrafficFlag; //�������ӱ�־;
	
	int CurrentInterface;
	int CurrentTrafficType;
	double lastTotalTraffic;


#if WINVER>=0x500
	//#if WINVER>=0x0401
    RAS_STATS rsOldStatistics;    //���ڼ�¼����״̬
    RAS_STATS rsNewStatistics;    //���ڼ�¼�µ�����״̬
	
	RAS_STATS rsBeforeChangestatistics;
	COleDateTime m_BeforeChageTime;
	//RAS_STATS rsUMTSstatistics;
#endif

protected:
	HRASCONN m_hRasConn;

};

#endif // !defined(AFX_MFNETTRAFFIC_H__9CA9C41F_F929_4F26_BD1F_2B5827090494__INCLUDED_)
