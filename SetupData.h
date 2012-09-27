// SetupData.h: interface for the CSetupData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETUPDATA_H__E776FB69_6C0C_47CC_9AD5_8AA937B1C005__INCLUDED_)
#define AFX_SETUPDATA_H__E776FB69_6C0C_47CC_9AD5_8AA937B1C005__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonStructs.h"
typedef unsigned long ULONG_PTR;
#include "Ras.h"

extern stSetData g_SetData;
extern const TCHAR szInifilename[50];

extern stStringType IniStringTable[];

extern stIntType IniIntTable[];

extern TCHAR* ValueStringTable[];

extern int* ValueIntTable[];

void GetIniFileName();
BOOL LoadIniFile();
BOOL SaveIniFile();
BOOL SaveIniFilePwd();
BOOL AddPrefixName(CString &str);

CString PWDXOR(const char* strData);

TCHAR *strrrc(TCHAR *str, TCHAR c);
char *strrrcA(char *str, char c);

class CSetupData
{
public:
    CSetupData();
    virtual ~CSetupData();

};

#endif // !defined(AFX_SETUPDATA_H__E776FB69_6C0C_47CC_9AD5_8AA937B1C005__INCLUDED_)
