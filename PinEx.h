#ifndef _PINEX_H
#define _PINEX_H

#include "ds.h"
class CHSDPADlg;
typedef enum 
{
	CME_SIM_NOT_INSERTED      =  10,  /* SIM not inserted */
	CME_SIM_PIN_REQUIRED      =  11,  /* SIM PIN required */
	CME_SIM_PUK_REQUIRED      =  12,  /* SIM PUK required */
	CME_SIM_FAILURE           =  13,  /* SIM failure */
	CME_SIM_BUSY              =  14,  /* SIM busy */
	CME_SIM_WRONG             =  15,  /* SIM wrong */
	CME_INCORRECT_PASSWORD    =  16,  /* incorrect password */
	CME_SIM_NONE_REQUIRED	  =	 17,  /* SIM PIN not required */
	CME_SIM_NONE_INVALID	  =	 18,  /* SIM Lock*/
	CME_SIM_MAX
	
} eCMECode;
typedef enum 
{
	CPIN_SIM_PIN_REQUIRED     =  20,  /* SIM PIN*/
	CPIN_SIM_PUK_REQUIRED     =  21,  /* SIM PUK*/
	CPIN_SIM_NONE_REQUIRED    =  22,  /* READY*/
	CPIN_SIM_DESTROYED        =  23,  /* SIM DESTROYED*/
	CPIN_SIM_MAX
	
} eCPinCode;
typedef struct
{
    USHORT code;
    char * text;
} stTXTCode;
#define PIN_RMTIMES_MAX 3
#define PUK_RMTIMES_MAX 10

class CPinEx
{
public:
	CPinEx();
	~CPinEx();	
//{{AT CallBack Func
	int m_nRespIndex;
	static void AtRespPinHandler(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
	static void AtRespPinRemainTimes(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
//}}AT CallBack Func
public:
	CSerialPort* m_pComm;
	CHSDPADlg*   m_pRetWnd;
	static const stTXTCode cme_table[];
	static const stTXTCode cpin_table[];
	USHORT m_nPinStat;  //为1代表PIN码打开，为0代表PIN码关闭
	USHORT m_nSimStat;
	USHORT m_nRemainTimes;  //PIN码的剩余次数
	USHORT m_nRemainTimes_puk;  //PUK的剩余次数
public:
	BOOL GetSimStat();
	BOOL GetPinStat();
	BOOL GetPinRemainTimes();

protected:
	HANDLE m_GetPinStatEvent;

};

#endif