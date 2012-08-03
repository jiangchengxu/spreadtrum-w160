// SetupData.cpp: implementation of the CSetupData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hsdpa.h"
#include "SetupData.h"
#include "Ds.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

//配置文件名
const TCHAR szInifilename[]=_T(".\\HSDPA.ini");
//TCHAR szInifilename[50]={0};

//全局变量，存储设置信息
stSetData g_SetData;

//全局变量中字符部分
stStringType IniStringTable[]=
{
    _T("Internet"), _T("Internet_szDefaultConnName"), _T("Default Connection"), RAS_MaxEntryName,
    _T("Internet"), _T("Internet_szLocalPhoneNumber"), _T("*98#"), RAS_MaxPhoneNumber,
    _T("Internet"), _T("Internet_szDeviceName"), _T("USB Modem"), RAS_MaxDeviceName,
    _T("Internet"), _T("Internet_szDeviceType"), _T("modem"), RAS_MaxDeviceType,
    _T("Internet"), _T("Internet_szUserName"), _T(""), UNLEN,
    _T("Internet"), _T("Internet_szPassword"), _T(""), PWLEN,
    _T("Main"), _T("General_szSkinFile"), _T("skin\\neostyle\\theme.ini"), 256,
    _T("Internet"), _T("Internet_szDefaultConnection"), _T("Default Connection"), RAS_MaxEntryName,
//wj test
    _T("Internet"), _T("Internet_szPCUIName"), _T("PCUI Device Interface"), 256,
    _T("Internet"), _T("Internet_szPCVOICEName"), _T("PCUI Voice Interface"), 256,
	//程序前缀名称
	_T("Main"), _T("Main_szPrefixName"), _T(""), 32,
	//URL
	_T("Main"), _T("Main_szURL"), _T(""), 256,
	//Email
	_T("Main"), _T("Main_szEMail"), _T(""), 256,
	//Splash map name
	_T("Main"), _T("Main_szSplashBmp"), _T(""), 256,
	//Logo map name
	_T("Main"), _T("Main_szLogoBmp"), _T(""), 256,
#ifdef FEATURE_VERSION_ITELCO
	_T("Main"), _T("Main_szTitleLogoIcon"), _T(""), 256,  //added by ljl 2007.09.19
#endif
	//Company
	_T("Main"), _T("Main_szCompany"), _T(""), 64,
	//MainIcon
	_T("Main"), _T("Main_szMainIcon"), _T("skin\\MAINFRAME.ico"), 64,
	//Help file name
	_T("Main"), _T("Main_szHelpFile"), _T(""), 256,
	//Default Profile name
	_T("Internet"), _T("Internet_szProfileName"), _T("Default"), RAS_MaxEntryName,
	//Default APN
	_T("Internet"), _T("Internet_szAPN"), _T(""), 256,
	//UsbDiskFriendlyName
	_T("Main"), _T("Main_szUsbDiskFriendlyName"), _T(""), 256,
	//Application name
	_T("Main"), _T("Main_szAppName"), _T(""), 64, 
    //SMS Alert sound file path
    _T("Messages"), _T("Messages_szSoundFile"), _T(""), MAX_PATH, 
    _T("Main"), _T("Main_szVID"), _T("1614"), 20, 
    _T("Main"), _T("Main_szPID"), _T("0800"), 20, 
	_T("Internet"), _T("Internet_szCurrentPwd"),_T(""), 40,
	//3G type setting 
	_T("Setup"), _T("Setup_sz3GType"),_T("WCDMA"), 10,
	//Last time reset 
	_T("Internet"), _T("Internet_szLastTimeReset"),_T(""), 40,

	// Call Emergency Numbers
	_T("Messages"), _T("Messages_szVoiceMailNumber"), _T(""),40,//add by liub for voicemail
	_T("Call"), _T("Call_szEmergencyNumbers"), _T(""), MAX_PATH, 
};

//全局变量中整型部分
stIntType IniIntTable[]=
{
    _T("Internet"), _T("Internet_dwFramingProtocol"), 1,
    _T("Internet"), _T("Internet_dwfOptions"), 1027408664,
    _T("Internet"), _T("Internet_dwType"), 1,
    _T("Internet"), _T("Internet_dwCountryID"), 86,
    _T("Internet"), _T("Internet_dwCountryCode"), 86,
    _T("Internet"), _T("Internet_dwfNetProtocols"), 4,
    _T("Internet"), _T("Internet_dwSubEntries"), 1,
    _T("Internet"), _T("Internet_dwEncryptionType"), 3,
    _T("Internet"), _T("Internet_dwSubEntry"), 1,
    _T("Internet"), _T("Internet_nConnections"), 0,
    _T("Setup"), _T("Setup_nSkinStyle"), 0,
    _T("Setup"), _T("Setup_nPowerManagement"), 1,
	_T("Main"), _T("Main_nCall"), 1,
	_T("Main"), _T("Main_nLanguage"), 1,
	_T("Main"), _T("Main_nCompany"), 1,
	//Prefered Storage. 0: ME, 1: USIM, 2:PC
	_T("Messages"), _T("Messages_nPreStor"), 2,
	//Validity Period.	0: 1 hour, 1: 6 hours, 2: 1 day,
	//					3: 3 days, 4: 1 week,  5: 1 year
	_T("Messages"), _T("Messages_nValPeriod"), 2,
	//Delivery report. 0: Off, 1: On
	_T("Messages"), _T("Messages_nDeliReport"), 0,
	//have help menu? 1:yes, 0:no
	_T("Main"), _T("Main_nHelp"), 1,
	//PC Card Auto run. 0:Manual run; 1:Auto run when PC card inserted
	_T("Main"), _T("Main_nAutoRun"), 0, 
    
    //Launch application when Windows start up. 1:yes, 0:no
    _T("Setup"), _T("Setup_nAutoLaunchApp"), 0, 
    //Connect internet while starting. 1:yes, 0:no
    _T("Setup"), _T("Setup_nAutoConnect"), 0, 
    //Start main window minimized. 1:yes, 0:no
    _T("Setup"), _T("Setup_nStartMini"), 0, 
    //Standby/Hibernate Setting. 1:Allow, 0:Prevent
    _T("Setup"), _T("Setup_nAllowSleep"), 1,
	//NDIS availability  1:yes, 0:no
	_T("Setup"),_T("Setup_nNDIS"), 0,
    //Alert window when receiving new SMS. 1:yes, 0:no
    _T("Messages"), _T("Messages_nAlertWindow"), 1, 
    //Alert Tone when receiving new SMS. 1:yes, 0:no
    _T("Messages"), _T("Messages_nAlertTone"), 1, 
	//CDMA2000 SMS 优先级设置,0:Urgent, 1:Emergency, 2:General, 3:Interactive
	_T("Messages"),_T("Messages_nPriority"),2,
	//CDMA2000 SMS 有效期设置, 0:    30分钟     有效期值=5
//                             1:    1小时      有效期值=11
// 	                           2:    2小时	     有效期值=23
// 	                           3:    6小时      有效期值=71
// 				               4:    12小时     有效期值=144
// 				               5:    1天        有效期值=167
// 				               6:    1星期      有效期值=173
// 				               7:    无限制     有效期值=245
// 							   8:    默认       有效期值=246
    _T("Messages"),_T("Messages_nDefDelivery"),8,

//added by wk end on 2007-2-5


    _T("Internet"), _T("Internet_nPromptEnable"), 0,  

    _T("Internet"), _T("Internet_nPromptYear"), 0,  
    _T("Internet"), _T("Internet_nPromptMonth"), 0, 
    _T("Internet"), _T("Internet_nPromptDay"), 0, 
    _T("Internet"), _T("Internet_nPromptFluxOrDuration"), 0, 
    _T("Internet"), _T("Internet_nPromptFluxVal"), 50, 
    _T("Internet"), _T("Internet_nPromptDurationVal"), 600, 

    //Auth type. 0: CHAP 1: PAP
    _T("Internet"), _T("Internet_nAuthType"), 0, 

	_T("Internet"), _T("Internet_nPromptDisconnEnable"), 0,  //added by ljl 2007.8.30
	_T("Internet"), _T("Internet_nPromptPwdConfirm"), 0,      //added by ljl 2007.8.30
	_T("Internet"), _T("Internet_nReachUsageLimit"), 0,      //added by ljl 2007.9.17

	_T("Main"), _T("Main_nSimLock"), 1,
};

//设置部分字符型变量
TCHAR* ValueStringTable[] = 
{
    g_SetData.Internet_szDefaultConnName,
    g_SetData.Internet_szLocalPhoneNumber,
    g_SetData.Internet_szDeviceName,
    g_SetData.Internet_szDeviceType,
    g_SetData.Internet_szUserName,
    g_SetData.Internet_szPassword, 
    g_SetData.General_szSkinFile,
    g_SetData.Internet_szDefaultConnection,    //默认的连接名称
    //Added by lgz 2006-06-22
    g_SetData.Internet_szPCUIName,  //PCUI口名称
    g_SetData.Internet_szPCVOICEName,  //PCUI口名称wj test
	g_SetData.Main_szPrefixName,	//程序前缀名称
	g_SetData.Main_szURL,			//URL
	g_SetData.Main_szEMail,			//Email
	g_SetData.Main_szSplashBmp,		//Splash BMP
	g_SetData.Main_szLogoBmp,		//Logo BMP
#ifdef FEATURE_VERSION_ITELCO
	g_SetData.Main_szTitleLogoIcon,   //Title Logo added by ljl 2007.09.19
#endif
	g_SetData.Main_szCompany,		//Company
	g_SetData.Main_szMainIcon,		//MainIcon
	g_SetData.Main_szHelpFile,		//help name
	g_SetData.Internet_szProfileName,//default profile name
	g_SetData.Internet_szAPN,		//default APN
	g_SetData.Main_szUsbDiskFriendlyName, //UsbDiskFriendlyName		
	g_SetData.Main_szAppName,	//Application name
    g_SetData.Messages_szSoundFile,  //SMS Alert sound file path
    g_SetData.Main_szVID, //VID
    g_SetData.Main_szPID, //PID
	g_SetData.Internet_szCurrentPwd,   
	g_SetData.Setup_sz3GType,
	g_SetData.Internet_szLastTimeReset,
	g_SetData.Messages_szVoiceMailNumber,//add by liub for voicemail
	g_SetData.Call_szEmergencyNumbers,
};

//设置部分数值型变量
int* ValueIntTable[] = 
{
    &g_SetData.Internet_dwFramingProtocol,
    &g_SetData.Internet_dwfOptions,
    &g_SetData.Internet_dwType,
    &g_SetData.Internet_dwCountryID,
    &g_SetData.Internet_dwCountryCode,
    &g_SetData.Internet_dwfNetProtocols,
    &g_SetData.Internet_dwSubEntries,
    &g_SetData.Internet_dwEncryptionType,
    &g_SetData.Internet_dwSubEntry,
    &g_SetData.Internet_nConnections,
    &g_SetData.Setup_nSkinStyle,
    &g_SetData.Setup_nPowerManagement,
	//have call function? 1:yes, 0:no
	&g_SetData.Main_nCall,
	//language
	&g_SetData.Main_nLanguage,
	//company
	&g_SetData.Main_nCompany,
	//Prefered Storage. 0:ME, 1: USIM, 2:PC
	&g_SetData.Messages_nPreStor,
	//Validity Period.	0: 1 hour, 1: 6 hours, 2: 1 day,
	//					3: 3 days, 4: 1 week,  5: 1 year
	&g_SetData.Messages_nValPeriod,
	//Delivery report. 0: Off, 1: On
	&g_SetData.Messages_nDeliReport,
	//have help menu? 1:yes, 0:no
	&g_SetData.Main_nHelp,
	//PC Card Auto run. 0:Manual run; 1:Auto run when PC card inserted
	&g_SetData.Main_nAutoRun, 

    //Launch application when Windows start up. 1:yes, 0:no
    &g_SetData.Setup_nAutoLaunchApp,
    //Connect internet while starting. 1:yes, 0:no
    &g_SetData.Setup_nAutoConnect,
    //Start main window minimized. 1:yes, 0:no
    &g_SetData.Setup_nStartMini,
    //Standby/Hibernate Setting. 1:Allow, 0:Prevent
    &g_SetData.Setup_nAllowSleep,
    //NDIS feature supported 1: yes 0:no
	&g_SetData.Setup_nNDIS,
    //Alert window when receiving new SMS. 1:yes, 0:no
    &g_SetData.Messages_nAlertWindow,
    //Alert Tone when receiving new SMS. 1:yes, 0:no
    &g_SetData.Messages_nAlertTone,
    //CDMA2000 SMS 优先级设置,0:Urgent, 1:Emergency, 2:General, 3:Interactive
	&g_SetData.Messages_nPriority,
	//CDMA2000 SMS 有效期设置
	&g_SetData.Messages_nDefDelivery,


    &g_SetData.Internet_nPromptEnable,  
    &g_SetData.Internet_nPromptYear,  
    &g_SetData.Internet_nPromptMonth, 
    &g_SetData.Internet_nPromptDay, 
    &g_SetData.Internet_nPromptFluxOrDuration, 
    &g_SetData.Internet_nPromptFluxVal, 
    &g_SetData.Internet_nPromptDurationVal, 


    //Auth type. 0: CHAP 1: PAP
    &g_SetData.Internet_nAuthType,
	&g_SetData.Internet_nPromptDisconnEnable,  
	&g_SetData.Internet_nPromptPwdConfirm,      
	&g_SetData.Internet_nReachUsageLimit, 
	
	&g_SetData.Main_nSimLock,
};


//去掉字符串末尾指定的字符
TCHAR *strrrc(TCHAR *str, TCHAR c)
{
    int i;
    for(i=wcslen(str);*(str+i-1)==c;i=wcslen(str))
    {
        *(str+i-1)='\0';
    }
    return str;
}
char *strrrcA(char *str, char c)
{
    int i;
    for(i=strlen(str);*(str+i-1)==c;i=strlen(str))
    {
        *(str+i-1)='\0';
    }
    return str;
}

#if 0

void GetIniFileName()
{
	const TCHAR strRegClass[] = {_T("Software\\HSDPA\\LANGINI")};
	HKEY hKeyDev	= NULL;
	char cLang[10] = {0};
	LONG length = sizeof(cLang);
	//DWORD dwIndex	= 0;
	//CHAR chPrevKey[MAX_PATH+1]	= {0};
	LONG lOpenRet	= ERROR_SUCCESS;

	lOpenRet = RegOpenKeyEx(HKEY_CURRENT_USER, strRegClass, 0, KEY_ALL_ACCESS, &hKeyDev);
	if(ERROR_SUCCESS == lOpenRet)
	{
		LONG lQueryRes = ERROR_SUCCESS;
		lQueryRes = RegQueryValueEx(hKeyDev,_T("LANG") , NULL, NULL,
							(unsigned char *)cLang, (LPDWORD)&length);
		if(ERROR_SUCCESS == lQueryRes)
		{
			if(!strcmp(cLang,"e"))
			{
				wcscpy(szInifilename,_T(".\\HSDPAE.ini"));				
			}
			else if(!strcmp(cLang,"a"))
			{
				wcscpy(szInifilename,_T(".\\HSDPAA.ini"));
			}
			else
			{
				wcscpy(szInifilename,_T(".\\HSDPA.ini"));
			}			
		}
	}
	else
	{
		wcscpy(szInifilename,_T(".\\HSDPA.ini"));
	}
	return;
}
#endif

BOOL LoadIniFile()
{
    int count;
    int i;
	USES_CONVERSION;
	//GetIniFileName();
    count=sizeof(IniStringTable)/sizeof(struct stStringType);
    for(i=0; i<count; i++)
    {
        GetPrivateProfileString(IniStringTable[i].lpSectionName,
                            IniStringTable[i].lpKeyName,
                            IniStringTable[i].lpDefault,
                            (TCHAR*)ValueStringTable[i],
                            IniStringTable[i].nSize,
                            szInifilename);
    }

    count=sizeof(IniIntTable)/sizeof(struct stIntType);
    for(i=0; i<count; i++)
    {
        *ValueIntTable[i]=GetPrivateProfileInt(IniIntTable[i].lpSectionName,
                        IniIntTable[i].lpKeyName,
                        IniIntTable[i].nDefault,
                        szInifilename);
    }

	CString strPwdXor = _T("");

// 	if(NULL !=wcslen(g_SetData.Internet_szCurrentPwd))
// 	{
// 		char *strPwd=W2A(g_SetData.Internet_szCurrentPwd);
// 	//	strPwdXor = PWDXOR(g_SetData.Internet_szCurrentPwd);
// 		strPwdXor = PWDXOR(strPwd);
// 		wcscpy(g_SetData.Internet_szCurrentPwd,strPwdXor);
// 	}

    return 0;
}

BOOL SaveIniFile()
{
    int count;
    int i;


 	//USES_CONVERSION;

// 	CString strPwdXor = _T("");
// 	CString strPwdBack = _T("");
// 	if(NULL !=wcslen(g_SetData.Internet_szCurrentPwd))
// 	{
// 			strPwdBack.Format(g_SetData.Internet_szCurrentPwd);
// 			
// 			char *strPwd=W2A(g_SetData.Internet_szCurrentPwd);		
// 
// 		//strPwdXor = PWDXOR(g_SetData.Internet_szCurrentPwd);
// 		strPwdXor = PWDXOR(strPwd);
// 		wcscpy(g_SetData.Internet_szCurrentPwd,strPwdXor);
// 	}


    CFileStatus  filestatus;
    if(CFile::GetStatus(szInifilename, filestatus))
        SetFileAttributes(szInifilename, FILE_ATTRIBUTE_ARCHIVE);

    count=sizeof(IniStringTable)/sizeof(struct stStringType);
    for(i=0; i<count; i++)
    {
        WritePrivateProfileString(IniStringTable[i].lpSectionName,
                            IniStringTable[i].lpKeyName,
                            (TCHAR*)ValueStringTable[i],
                            szInifilename);
    }

    count=sizeof(IniIntTable)/sizeof(struct stIntType);
    for(i=0; i<count; i++)
    {
        CString strTemp;
        strTemp.Format(_T("%d"),*ValueIntTable[i]);
        WritePrivateProfileString(IniIntTable[i].lpSectionName,
                                IniIntTable[i].lpKeyName,
                                strTemp,
                                szInifilename);
    }
	//wcscpy(g_SetData.Internet_szCurrentPwd,strPwdBack);  /* added by ljl 2007.9.3 */
    
	return 0;
}


BOOL SaveIniFilePwd()
{
    int count;
    int i;
	USES_CONVERSION;

	CString strPwdXor = _T("");
	CString strPwdBack = _T("");
	if(NULL !=wcslen(g_SetData.Internet_szCurrentPwd))
	{
		strPwdBack.Format(g_SetData.Internet_szCurrentPwd);
		
		char *strPwd=W2A(g_SetData.Internet_szCurrentPwd);		
		
		//strPwdXor = PWDXOR(g_SetData.Internet_szCurrentPwd);
		strPwdXor = PWDXOR(strPwd);
		wcscpy(g_SetData.Internet_szCurrentPwd,strPwdXor);
	}

	
    CFileStatus  filestatus;
    if(CFile::GetStatus(szInifilename, filestatus))
        SetFileAttributes(szInifilename, FILE_ATTRIBUTE_ARCHIVE);
	
    count=sizeof(IniStringTable)/sizeof(struct stStringType);
    for(i=0; i<count; i++)
    {
        WritePrivateProfileString(IniStringTable[i].lpSectionName,
			IniStringTable[i].lpKeyName,
			(TCHAR*)ValueStringTable[i],
			szInifilename);
    }
	
    count=sizeof(IniIntTable)/sizeof(struct stIntType);
    for(i=0; i<count; i++)
    {
        CString strTemp;
        strTemp.Format(_T("%d"),*ValueIntTable[i]);
        WritePrivateProfileString(IniIntTable[i].lpSectionName,
			IniIntTable[i].lpKeyName,
			strTemp,
			szInifilename);
    }
	//wcscpy(g_SetData.Internet_szCurrentPwd,strPwdBack);  /* added by ljl 2007.9.3 */
    
	return 0;
}

//添加前缀
BOOL AddPrefixName(CString& str)
{
	CString temp;
	temp=str;
    str=g_SetData.Main_szPrefixName;
	str+=_T(" ");
    str+= temp;
	return 0;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSetupData::CSetupData()
{

}

CSetupData::~CSetupData()
{

}

CString PWDXOR(const char* strData)     //加密解密   
{
	CString strTemp = _T("");  
    //TCHAR strTemp ;
	CString strCode = _T("");  
	int i = 0;

	for (i = 0; i <= strlen(strData);i++)   
	{   
	  char ch = strData[i];   
	  strTemp.Format(_T("%d"),ch ^ 0XFF);
	  strCode += strTemp;
	} 
	return strCode;		 
} 