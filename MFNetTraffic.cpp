/*******************************************

	MFTrafficButton

	
********************************************/

// MFNetTraffic.cpp: implementation of the MFNetTraffic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MFNetTraffic.h"
#include "float.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "winperf.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MFNetTraffic::MFNetTraffic()
{
	lasttraffic = 0.0;
	InitTrafficFlag=true;
	CurrentInterface = -1;
	CurrentTrafficType = AllTraffic;
	lastTotalTraffic=0.0;
	CurrentTotalTraffic=0.0;
	if(!isVista())
	{
		GetInterfaces();
	}
	
}

MFNetTraffic::~MFNetTraffic()
{
	CurrentTotalTraffic=0;	
}

// Little helper functions
// Found them on CodeGuru, but do not know who has written them originally

PERF_OBJECT_TYPE *FirstObject(PERF_DATA_BLOCK *dataBlock)
{
  return (PERF_OBJECT_TYPE *) ((BYTE *)dataBlock + dataBlock->HeaderLength);
}

PERF_OBJECT_TYPE *NextObject(PERF_OBJECT_TYPE *act)
{
  return (PERF_OBJECT_TYPE *) ((BYTE *)act + act->TotalByteLength);
}

PERF_COUNTER_DEFINITION *FirstCounter(PERF_OBJECT_TYPE *perfObject)
{
  return (PERF_COUNTER_DEFINITION *) ((BYTE *) perfObject + perfObject->HeaderLength);
}

PERF_COUNTER_DEFINITION *NextCounter(PERF_COUNTER_DEFINITION *perfCounter)
{
  return (PERF_COUNTER_DEFINITION *) ((BYTE *) perfCounter + perfCounter->ByteLength);
}

PERF_COUNTER_BLOCK *GetCounterBlock(PERF_INSTANCE_DEFINITION *pInstance)
{
  return (PERF_COUNTER_BLOCK *) ((BYTE *)pInstance + pInstance->ByteLength);
}

PERF_INSTANCE_DEFINITION *FirstInstance (PERF_OBJECT_TYPE *pObject)
{
  return (PERF_INSTANCE_DEFINITION *)  ((BYTE *) pObject + pObject->DefinitionLength);
}

PERF_INSTANCE_DEFINITION *NextInstance (PERF_INSTANCE_DEFINITION *pInstance)
{
  // next instance is after
  //    this instance + this instances counter data

  PERF_COUNTER_BLOCK  *pCtrBlk = GetCounterBlock(pInstance);

  return (PERF_INSTANCE_DEFINITION *) ((BYTE *)pInstance + pInstance->ByteLength + pCtrBlk->ByteLength);
}

char *WideToMulti(wchar_t *source, char *dest, int size)
{
  WideCharToMultiByte(CP_ACP, 0, source, -1, dest, size, 0, 0);

  return dest;
}

/*
	returns the traffic of given interface
*/
double MFNetTraffic::GetTraffic(int interfaceNumber)
{
	try
	{
#define DEFAULT_BUFFER_SIZE 40960L
		
		POSITION pos;
		CString InterfaceName;
		pos = Interfaces.FindIndex(interfaceNumber);
		if(pos==NULL)
			return 0.0;
		InterfaceName = Interfaces.GetAt(pos);
		
		
		// buffer for performance data
		unsigned char *data = new unsigned char [DEFAULT_BUFFER_SIZE];
		// return value from RegQueryValueEx: ignored for this application
		DWORD type;
		// Buffer size
		DWORD size = DEFAULT_BUFFER_SIZE;
		// return value of RegQueryValueEx
		DWORD ret;
		
		// request performance data from network object (index 510) 
		while((ret = RegQueryValueExA(HKEY_PERFORMANCE_DATA, "510", 0, &type, data, &size)) != ERROR_SUCCESS) {
			if(ret == ERROR_MORE_DATA) 
			{
				// buffer size was too small, increase allocation size
				size += DEFAULT_BUFFER_SIZE;
				
				delete []data;
				data = new unsigned char [size];
			} 
			else 
			{
				// some unspecified error has occured
				//wyw_0103
				delete []data;

				return 1;
			}
		}
		
		PERF_DATA_BLOCK *dataBlockPtr = (PERF_DATA_BLOCK *)data;
		
		// enumerate first object of list
		PERF_OBJECT_TYPE *objectPtr = FirstObject(dataBlockPtr);
		
		// trespassing the list 
		for(int a=0 ; a<(int)dataBlockPtr->NumObjectTypes ; a++) 
		{
		//	char nameBuffer[255];
			
			// did we receive a network object?
			if(objectPtr->ObjectNameTitleIndex == 510) 
			{
				// Calculate the offset
				DWORD processIdOffset = ULONG_MAX;
				
				// find first counter 
				PERF_COUNTER_DEFINITION *counterPtr = FirstCounter(objectPtr);
				
				// walking trough the list of objects
				for(int b=0 ; b<(int)objectPtr->NumCounters ; b++) 
				{
					// Check if we received datatype wished
					if((int)counterPtr->CounterNameTitleIndex == CurrentTrafficType)
						processIdOffset = counterPtr->CounterOffset;
					
					// watch next counter
					counterPtr = NextCounter(counterPtr);
				}
				
				if(processIdOffset == ULONG_MAX) {
					delete []data;
					return 1;
				}
				
				
				// Find first instance
				PERF_INSTANCE_DEFINITION *instancePtr = FirstInstance(objectPtr);
				
				DWORD fullTraffic;
				DWORD traffic;
				for(b=0 ; b<objectPtr->NumInstances ; b++) 
				{
					// evaluate pointer to name
				//	char nameBuffer[255];
					wchar_t *namePtr = (wchar_t *) ((BYTE *)instancePtr + instancePtr->NameOffset);
					
					// get PERF_COUNTER_BLOCK of this instance
					PERF_COUNTER_BLOCK *counterBlockPtr = GetCounterBlock(instancePtr);
					
					// now we have the interface name
					
				//	char *pName = WideToMulti(namePtr, nameBuffer, sizeof(nameBuffer));
					CString iName;
					iName.Format(_T("%s"),namePtr);
					//iName.Format(_T("%s"),pName);
					POSITION pos = TotalTraffics.FindIndex(b);
					if(pos!=NULL)
					{
						fullTraffic = *((DWORD *) ((BYTE *)counterBlockPtr + processIdOffset));
						TotalTraffics.SetAt(pos,fullTraffic);
					}

					// If the interface the currently selected interface?
					//if(InterfaceName == iName)
					if(iName.Find(InterfaceName)>-1)
					{
						traffic = *((DWORD *) ((BYTE *)counterBlockPtr + processIdOffset));
						double acttraffic = (double)traffic;
												
						if(InitTrafficFlag)
						{
							lastTotalTraffic=acttraffic;
							CurrentTotalTraffic=0.0;
							InitTrafficFlag=false;
						}
						else
						{
						//	lastTotalTraffic=acttraffic;
							CurrentTotalTraffic=acttraffic-lastTotalTraffic;//本次连接的总数据流量
							if(CurrentTotalTraffic<=0.00001)
								CurrentTotalTraffic=0.0;
						}
						
						double trafficdelta;
						// Do we handle a new interface (e.g. due a change of the interface number
						if(CurrentInterface != interfaceNumber)
						{
							lasttraffic = acttraffic;
							trafficdelta = 0.0;
							CurrentInterface = interfaceNumber;
						}
						else
						{
							trafficdelta = acttraffic - lasttraffic;
							lasttraffic = acttraffic;
						}
						if(trafficdelta<=0.00001)
							trafficdelta=0.0;

						//wyw_0103
						delete []data;
									
						return(trafficdelta);
					}
					
					// next instance
					instancePtr = NextInstance(instancePtr);
				}
			}
			
			// next object in list
			objectPtr = NextObject(objectPtr);
		}
		
		delete []data;
		return 0;
	}

	catch(...)
	{
		return 0;
	}
}

/*获得Windows Vista 下的网络流量*/
double MFNetTraffic::GetTrafficVista(int interfaceNumber) 
{
	if(!g_bDataService)
	{
		CurrentTotalTraffic=0.0;
		return 0.0;
	}
	switch(interfaceNumber)
	{
	case 0: //NDIS
		double trafficNDIS;
		trafficNDIS=0.0;
		CurrentTotalTraffic=0.0;
		switch(CurrentTrafficType)
		{
		case AllTraffic:
			trafficNDIS=(m_pDlgConnect->m_dNDISData_Rec+m_pDlgConnect->m_dNDISRate_Send);
			CurrentTotalTraffic=(m_pDlgConnect->m_dNDISData_Rec+m_pDlgConnect->m_dNDISData_Send)*1024;
			break;
		case IncomingTraffic:
			trafficNDIS=m_pDlgConnect->m_dNDISRate_Rec;
			CurrentTotalTraffic=m_pDlgConnect->m_dNDISData_Rec*1024;
			break;
		case OutGoingTraffic:
			trafficNDIS =m_pDlgConnect->m_dNDISRate_Send;
			CurrentTotalTraffic=m_pDlgConnect->m_dNDISData_Send*1024;
			break;
		default:
			
			break;
		}
		return trafficNDIS;
		break;
 	   case 1:  //RAS

	
			double trafficdelta;
	
    
			rsNewStatistics=m_pDlgConnect->rsNewStatistics;
				DWORD traffic;
				switch(CurrentTrafficType)
				{
					case AllTraffic:
						  traffic=(rsNewStatistics.dwBytesRcved+rsNewStatistics.dwBytesXmited);
						  break;
					case IncomingTraffic:
						traffic=rsNewStatistics.dwBytesRcved;
						  break;
					case OutGoingTraffic:
					   traffic =rsNewStatistics.dwBytesXmited;
						break;
					default:

						break;
				}
				
				double acttraffic = (double)traffic;
				if(InitTrafficFlag)
				{
					lastTotalTraffic=acttraffic;
					CurrentTotalTraffic=0.0;
					InitTrafficFlag=false;
				}
				else
				{
					//	lastTotalTraffic=acttraffic;
					CurrentTotalTraffic=acttraffic-lastTotalTraffic;//本次连接的总数据流量
					if(CurrentTotalTraffic<=0.00001)
						CurrentTotalTraffic=0.0;
				}
				
			
				// Do we handle a new interface (e.g. due a change of the interface number
				if(CurrentInterface != interfaceNumber)
				{
					lasttraffic = acttraffic;
					trafficdelta = 0.0;
					CurrentInterface = interfaceNumber;
				}
				else
				{
					trafficdelta = acttraffic - lasttraffic;
					lasttraffic = acttraffic;
				}
				if(trafficdelta<=0.00001)
					trafficdelta=0.0;
				
				
				
		// 	}
			return(trafficdelta);
			
		break;
	}
	return 0.0;
	
}

/*
	Enumerate installed interfaces. 
	See comments above
*/
BOOL MFNetTraffic::GetInterfaces()
{
	
	try
	{
		
		Interfaces.RemoveAll();
		DWORD bandwith;
		DWORD tottraff = 0;

		Interfaces.AddTail(_T("Qualcomm Wireless Ethernet Adapter 7002"));
		//Interfaces.AddTail("Qualcomm Wireless Ethernet Adapter 7002 - 数据包计划程序微型端口");
		bandwith=38400;
		Bandwidths.AddTail(bandwith);
		TotalTraffics.AddTail(tottraff);  // initial 0, just for creating the list
		

		Interfaces.AddTail(_T("WAN [PPP_SLIP] Interface"));
		bandwith=236800;
		Bandwidths.AddTail(bandwith);
		TotalTraffics.AddTail(tottraff);  // initial 0, just for creating the list

		//Interfaces.AddTail(_T("Realtek RTL8139 Family PCI Fast Ethernet NIC - 数据包计划程序微型端口"));
		Interfaces.AddTail(_T("Realtek RTL8139_810x Family Fast Ethernet NIC"));
		
		bandwith=100000000;
		Bandwidths.AddTail(bandwith);
		TotalTraffics.AddTail(tottraff);  // initial 0, just for creating the list
		// 	instancePtr = NextInstance(instancePtr);
		
		
		
		
		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
}

/*
	Returns the count of installed interfaces
*/
int MFNetTraffic::GetNetworkInterfacesCount()
{
	return Interfaces.GetCount()-1;
}

/*
	Returns the name of the given interface (-number)
*/
BOOL MFNetTraffic::GetNetworkInterfaceName(CString *InterfaceName, int index)
{
	POSITION pos = Interfaces.FindIndex(index);
	if(pos==NULL)
		return FALSE;

	InterfaceName->Format(_T("%s"),Interfaces.GetAt(pos));
	return TRUE;
}

/*
	Returns bandwith of interface e.g. 100000 for 100MBit
*/
DWORD MFNetTraffic::GetInterfaceBandwidth(int index)
{
	POSITION pos = Bandwidths.FindIndex(index);
	if(pos==NULL)
		return 0;

	else 
		return Bandwidths.GetAt(pos) / 8;

}

/*
	Sometime it is nice to know, how much traffic has a specific interface sent and received
*/

DWORD MFNetTraffic::GetInterfaceTotalTraffic(int index)
{
	DWORD		totaltraffic = 0;
	POSITION	pos;
	pos= TotalTraffics.FindIndex(index);
	if(pos!=NULL)
	{
		totaltraffic = TotalTraffics.GetAt(pos); 
		if(totaltraffic == 0.0)
		{
			GetTraffic(index);
			pos= TotalTraffics.FindIndex(index);
			if(pos!=NULL)
			{
				totaltraffic = TotalTraffics.GetAt(pos); 
			}
		}
	}

	return(totaltraffic);
}

/*
	To prevent direct manipulation of member variables....
*/
void MFNetTraffic::SetTrafficType(int trafficType)
{
	CurrentTrafficType = trafficType;
}


BOOL MFNetTraffic::isVista()
{
	WORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;
	dwVersion = GetVersion();
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	if(dwWindowsMajorVersion>5) //Windows vista
	{
		return TRUE;
	}
	else    //Windows XP and others
	{
		return FALSE;
	}
}




