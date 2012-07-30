#include "stdafx.h"
#include "HSDPA.h"
#include "ExDataExch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

//////////////////////////////////////
IGenerator::IGenerator(CWnd* pParentWnd)
{
    m_rsData = _T("");
    m_rsPath= _T("");
    nFileFormat = -1;
    pParent = pParentWnd;
}
IGenerator::~IGenerator()
{
}

void IGenerator::RecoverPath()
{
    ::SetCurrentDirectory(DefaultDir);
}

bool IGenerator::SetPath(int kind)
{
    //CString strFileName;
    TCHAR strFilter[128];
    TCHAR extBuf[2][5];
    memset(strFilter, 0, sizeof(strFilter));
    memset(extBuf, 0, sizeof(extBuf));

	/*根据来源选择支持的文件类型，kind表示是电话本还是短信等*/
    switch(kind)
    {
		case EXP_PHB:
		{
			/*Files (*.csv)|*.vcf|*/
			//strcpy(strFilter,"Comma Separated Values (*.csv)|*.csv|vCard Files (*.vcf)|*.vcf||");
			//strcpy(extBuf[0], ".csv");
			//strcpy(extBuf[1], ".vcf");

			CString strFileName;
			strFileName.LoadString(IDS_GENERATOR_CSVADNVCF);
			CString strCsvex;
			strCsvex.LoadString(IDS_GENERATOR_CSVEX);
			CString strVcfex;
			strVcfex.LoadString(IDS_GENERATOR_VCFEX);

			wcscpy(strFilter,strFileName);
			wcscpy(extBuf[0], strCsvex);
			wcscpy(extBuf[1], strVcfex);
			

			break;
		}
		case EXP_SMS:
		case EXP_CALLOG:
		case EXP_NETLOG:
		case EXP_NOTCSV:
		{
			/*Files (*.csv)*/
			//strcpy(strFilter,"Comma Separated Values (*.csv)|*.csv|");
			//strcpy(extBuf[0], ".csv");

			CString strFileName;
			strFileName.LoadString(IDS_GENERATOR_CSV);
			CString strCsvex;
			strCsvex.LoadString(IDS_GENERATOR_CSVEX);

			wcscpy(strFilter,strFileName);
			wcscpy(extBuf[0], strCsvex);

			break;
		}
    }
    CFileDialog folderDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY,strFilter, NULL);
	
	//显示2000下的打开文件对话框-------------
	//------用来检测目前的操作系统的版本信息
	int nStructSize = 0;
	DWORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;
	dwVersion = GetVersion();
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	if(dwVersion < 0x80000000)
	{
		nStructSize = 88;   //2k,xp下显示新的版本
	}
	else
	{
		nStructSize = 76;  //98下显示老版本
	}
	folderDlg.m_ofn.lStructSize = nStructSize;  //------用来检测目前的操作系统的版本信息

	CString strTitle = _T(""); 

    switch(kind)
    {
		case EXP_PHB:
		{
			//folderDlg.m_ofn.lpstrTitle = "Save To CSV/VCF File";
			strTitle.LoadString(IDS_GENERATOR_TITLE);

			break;
		}
		case EXP_SMS:
		case EXP_CALLOG:
		case EXP_NETLOG:
		case EXP_NOTCSV:
		{
			//folderDlg.m_ofn.lpstrTitle = "Save To CSV File";
			strTitle.LoadString(IDS_GENERATOR_CSVTITLE);
			
			break;
		}
    }
	folderDlg.m_ofn.lpstrTitle = (LPCTSTR)strTitle;
	TCHAR fPathTemp[MAX_PATH];
	CString strNewPath;
	
	::GetCurrentDirectory(MAX_PATH, fPathTemp);
	strNewPath = fPathTemp;

    //folderDlg.m_ofn.lpstrInitialDir = strNewPath;
	DefaultDir = strNewPath;
    //DefaultDir = DataExchCommFunc::GetWorkFolder();
    int iRet = folderDlg.DoModal();
    if(iRet != IDOK)
    {
		//RecoverPath();

        return false;
    }
    
    m_rsPath = folderDlg.GetPathName();
    CFileStatus rStatus;
    CString Tips;
    int iiRet;
    Tips.LoadString(IDS_FILE_EXIST);
    
    int index = folderDlg.m_ofn.nFilterIndex; /*获取用户选择了当前文件类型栏中第几项*/
    nFileFormat = index - 1;
    if(-1 == m_rsPath.Find(extBuf[nFileFormat],(m_rsPath.GetLength() - 5)))
    {
        m_rsPath += extBuf[nFileFormat];
    }

    CString ErrPath= m_rsPath + Tips;
    if (CFile::GetStatus( m_rsPath, rStatus ) == TRUE)
    {
        iiRet=AfxMessageBox(ErrPath,MB_OKCANCEL);
        if(iiRet != IDOK)
        {
			//RecoverPath();

            return false;
        }
    }
 
    if (nFileFormat == 0)
	{
        FillHead(kind);
    }
	//RecoverPath();

    return true;
}

int IGenerator::GetFormat()
{
    return nFileFormat;
}
 
void IGenerator::FillHead(int kind)
{
	switch (kind)
	{
		case EXP_PHB:
		{
			CString strPBHeader;
			strPBHeader.LoadString(IDS_EXPORT_PBHEADER);

			m_rsData += strPBHeader;
			break;
		}
		case EXP_SMS:
		{
			CString strSMSHeader;
			strSMSHeader.LoadString(IDS_EXPORT_SMSHEADER);

			m_rsData += strSMSHeader;

			break;
		}
		case EXP_NETLOG:
		{
			CString strNETHeader;
			strNETHeader.LoadString(IDS_EXPORT_NETHEADER);

			m_rsData += strNETHeader;

			break;
		}
		case EXP_NOTCSV:
		{
			m_rsData += StartStr;
			break;
		}
		default:
			break;
	}
}

void IGenerator::FillTrail()
{
    if(0 == nFileFormat)
	{
        m_rsData = m_rsData.Left(m_rsData.GetLength()-1);/*覆盖最后一个逗号*/
        m_rsData += "\r\n";
    }
	else
	{
        m_rsData += EndStr;
    }
}

void IGenerator::FillCell(CString& strCell)
{
    if(0 == nFileFormat)
	{
        if(IsQuotedCell(strCell) == 0)
		{
            m_rsData += strCell;
            m_rsData += ",";
        }
		else
		{
            strCell.Replace(_T("\""),_T("\"\""));
            m_rsData+="\"";
            m_rsData+=strCell;
            m_rsData+="\",";
        }
    }
	else
	{
        return;
    }
}

void IGenerator::FillvCardObj(StPbRecord& pbItem)
{
	CString FNval;
	FNval.Format(pbItem.strName);
    CString Cellval;
	Cellval.Format(pbItem.strMobile) ;
    CString Workval;
	Workval.Format(pbItem.strCompany);
    CString Homeval;
	Homeval.Format(pbItem.strHome);
	CString Groupval;
	Groupval.Format(pbItem.strGroup);
    CString Emailval;
	Emailval.Format(pbItem.strEmail);
    CString Noteval;
	Noteval.Format(pbItem.strMark);

    //加VCF格式的标题
	FillHead(EXP_NOTCSV);
	
    m_rsData = m_rsData + NameStrFN + FNval
        +TelNubStrPref + Cellval
        +HomeNubStr + Homeval
		+GroupNubStr + Groupval
        +WorkNubStr + Workval
        +EmailStrPref + Emailval
        +NoteStr+Noteval;
    FillTrail();
    m_rsData+="\r\n";
}

void IGenerator::MakeFile()
{
    HANDLE hFile; 
    DWORD  dwBytesRead, dwBytesWritten;
    //LPCTSTR buffer;
	TCHAR *buffer;
    LPCTSTR szExportPath = (LPCTSTR)m_rsPath;
    
	USES_CONVERSION;
    
    hFile = CreateFile((LPTSTR) szExportPath,  // file name 
        GENERIC_READ | GENERIC_WRITE, // open for read/write 
        0,                            // do not share 
        NULL,                         // no security 
        CREATE_ALWAYS,                // overwrite existing file
        FILE_ATTRIBUTE_NORMAL,        // normal file 
        NULL);                        // no attr. template 
    
    if (hFile == INVALID_HANDLE_VALUE)
	{ 
        CloseHandle(hFile);
        AfxMessageBox(IDS_GENERATOR_CREATEFAIL); 

		return;
    }
   // dwBytesRead = m_rsData.GetLength();
    //buffer = (LPCTSTR)m_rsData;
	buffer = (TCHAR *)(LPCTSTR)m_rsData;
	
	//TCHAR *WCharBuffer=A2W(buffer);
	char *buffer1=W2A(buffer);
    //WriteFile(hFile, buffer, dwBytesRead, &dwBytesWritten, NULL);
	dwBytesRead = strlen(buffer1);
	WriteFile(hFile, buffer1, dwBytesRead, &dwBytesWritten, NULL);

    CloseHandle(hFile);

}

bool IGenerator::IsQuotedCell(CString& strCell)
{
  int nRet;
 // CString cQuoteSet = ",\"\r\n";
   CString cQuoteSet = _T(",\"\r\n");
  nRet = strCell.FindOneOf(cQuoteSet);
  return ((nRet == -1)? 0 : 1);
}

CString DataExchCommFunc::GetWorkFolder()
{
    CString wFolder;
    TCHAR fDefault[MAX_PATH];
    ::GetCurrentDirectory(MAX_PATH, fDefault);
    wFolder = fDefault;
    return wFolder;
}

//用于VCF格式
//TAG前缀
const char IGenerator::StartStr[]        = "BEGIN:VCARD\nVERSION:3.0";  //24
const char IGenerator::NameStrFN[]       = "\nFN:";   //4
const char IGenerator::TelNubStrPref[]   = "\nTEL;TYPE=CELL,VOICE,PREF:";   //26
const char IGenerator::CellNubStr[]      = "\nTEL;TYPE=CELL,VOICE:";    //21
const char IGenerator::HomeNubStr[]      = "\nTEL;TYPE=HOME,VOICE:";  //21
const char IGenerator::GroupNubStr[]      = "\nGroup:";  //21
const char IGenerator::WorkNubStr[]      = "\nTEL;TYPE=WORK,VOICE:";  //21
const char IGenerator::EmailStrPref[]    = "\nEMAIL;TYPE=INTERNET,PREF:";   //26
const char IGenerator::NoteStr[]         = "\nNOTE:";//6
const char IGenerator::EndStr[]          = "\nEND:VCARD";   //10


const char IParser::StartStr[]         = "BEGIN:VCARD"; 
const char IParser::NameStrFN[]        = "FN:"; 
const char IParser::TelNubStrPref[]    = "TEL;TYPE=CELL,VOICE,PREF:"; 
const char IParser::HomeNubStr[]       = "TEL;TYPE=HOME,VOICE:";
const char IParser::GroupStr[]         = "Group:";
const char IParser::WorkNubStr[]       = "TEL;TYPE=WORK,VOICE:"; 
const char IParser::EmailStrPref[]     = "EMAIL;TYPE=INTERNET,PREF:"; 
const char IParser::NoteStr[]          = "NOTE:";
const char IParser::EndStr[]           = "END:VCARD";

IParser::IParser(CWnd* pParentWnd)
{
        m_rsData = _T("");
        m_rsPath= _T("");
        nFileFormat = -1;
        pParent = pParentWnd;
        m_nFileLength= 0;
        memset(&m_vCardList,0,sizeof(vCardList));
}

IParser::~IParser()
{
}
 
CString IParser::GetPath()
{
        return m_rsPath;
}

int IParser::GetFType()
{
        return nFileFormat;
}


//打开文件，成功返回文件的长度，失败返回0  
DWORD IParser::GetFileLen() 
{
	if (!m_FileImport.Open(m_rsPath , CFile::modeRead | CFile::typeText))
	{
		return 0;
	}
	else
	{
		m_nFileLength = m_FileImport.GetLength();

		return m_nFileLength;
	}
}

//从文件中读取数据
//UINT IParser::ReadData(TCHAR* psrcBuf)
UINT IParser::ReadData(char* psrcBuf)
{
	UINT uRes = m_FileImport.Read(psrcBuf,m_nFileLength);
    return uRes;
}

//恢复默认路径为应用程序所在的路径
void IParser::RecoverPath()
{
        ::SetCurrentDirectory(DefaultDir);
}

bool IParser::SetPath(int kind)
{
        
	//	= { "Comma Separated Values (*.csv)|*.csv|vCard Files (*.vcf)|*.vcf||" };/*TXT Files (*.txt)|*.txt|*/
	CString strFileName;	
	strFileName.LoadString(IDS_GENERATOR_CSVADNVCF);
    TCHAR strFilter[128];
	memset(strFilter,0,sizeof(strFilter));
	wcscpy(strFilter,strFileName);
	//extBuf = {".csv",".vcf"};
    TCHAR extBuf[2][5];
	memset(extBuf, 0, sizeof(extBuf));
	CString strCsvex;
	strCsvex.LoadString(IDS_GENERATOR_CSVEX);
	CString strVcfex;
	strVcfex.LoadString(IDS_GENERATOR_VCFEX);
	wcscpy(extBuf[0], strCsvex);
	wcscpy(extBuf[1], strVcfex);
	
    CFileDialog folderDlg(TRUE, NULL, NULL, 
        /*OFN_ALLOWMULTISELECT |*/OFN_HIDEREADONLY, 
        strFilter, NULL);
	
	//显示2000下的打开文件对话框-------------
	//------用来检测目前的操作系统的版本信息
	int nStructSize = 0;
	DWORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;
	dwVersion = GetVersion();
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	if(dwVersion < 0x80000000)
	{
		nStructSize = 88;   //2k,xp下显示新的版本
	}
	else
	{
		nStructSize = 76;  //98下显示老版本
	}
	folderDlg.m_ofn.lStructSize = nStructSize;  //------用来检测目前的操作系统的版本信息
	
	//folderDlg.m_ofn.lpstrTitle = "Load From CSV/VCF File";
	CString LoadFileName;
	LoadFileName.LoadString(IDS_PARSER_LOADFILE);	
	folderDlg.m_ofn.lpstrTitle = LoadFileName;

    //folderDlg.m_ofn.lpstrInitialDir = DataExchCommFunc::GetWorkFolder();
    //DefaultDir = DataExchCommFunc::GetWorkFolder();
	TCHAR fPathTemp[MAX_PATH];
	CString strNewPath;
	
	::GetCurrentDirectory(MAX_PATH, fPathTemp);
	strNewPath = fPathTemp;
	folderDlg.m_ofn.lpstrInitialDir = strNewPath;
    DefaultDir = strNewPath;

    int iRet = folderDlg.DoModal();
    if(iRet != IDOK)
	{
        return false;
    }

    m_rsPath = folderDlg.GetPathName();
	CString strExtName = m_rsPath.Right(4);
    if(0 == memcmp(extBuf[0],strExtName,4))
	{
        nFileFormat = 0;
    }
	else if (0 == memcmp(extBuf[1],strExtName,4))
	{
        nFileFormat = 1;
    }
	else
	{
        AfxMessageBox(IDS_GENERATOR_FILENAMEERR);
		
		return false;
    }
    return true;        
}

//获取记录的条数
int IParser::GetvCardItemCount()
{
        return m_vCardList.nCount;
}   

//获取一条vCardItem结构数据
void IParser::GetvCardItem(int index,vCardItem *pItem)
{
        if ((index < 0)||(index >= m_vCardList.nCount))
        {
            return;
        }
        memcpy(pItem,&(m_vCardList.vCardData[index]),sizeof(vCardItem));
}
       
/*姓名、移动电话、办公电话、住宅电话、邮件地址、备注为必选项
bool IParser::IsPropertyNeed(char *pInStr, int eTag)
{
    switch (eTag)
    {
		case VCARD_FN:
		case VCARD_TELCELL:
		case VCARD_TELWORK:
		case VCARD_TELHOME:
		case VCARD_EMAIL:
		case VCARD_NOTE:
		{
	        return true;
		}
		default:
		{
	        break;
		}
    }
    return false;
}

获取记录子项的标题名
int IParser::GetPropertyTag(char * pInstr)
{
    if (!strncmp(pInstr, "BEGIN", 5))
	{
        return VCARD_BEGIN;
    }
    else if (!strncmp(pInstr, "VERSION", 7))
	{
        return VCARD_VERSION;
    }
    else if (!strncmp(pInstr, "FN", 2))
	{
        return VCARD_FN;
    }
    else if (!strncmp(pInstr, "TEL", 3))
    {
        // Contain 4 kinds
        if (strstr(pInstr, "CELL"))
		{
            return VCARD_TELCELL;
        }
		else if (strstr(pInstr, "VOICE"))
		{
            if (strstr(pInstr, "WORK"))
			{
                return VCARD_TELWORK;
            }
			else if (strstr(pInstr, "HOME"))
			{
                return VCARD_TELHOME;
            }
        }      
    }
    else if (!strncmp(pInstr, "EMAIL", 5))
    {
        return VCARD_EMAIL;
    }
    else if (!strncmp(pInstr, "NOTE", 4))
	{
        return VCARD_NOTE;
    }
    else if (!strncmp(pInstr, "END", 3))
	{
        return VCARD_END;
    }
    
    return VCARD_IGNORE;
}

int IParser::GetPropertyValue(int eTag,char * pPropertyBegin,char * pValue,
								vCardContact *pCurContact,char ** ppPropertyEnd)
{
    int  lResult = 0;
    char *pTmpTail  = NULL;
    char *pTmpBegin = NULL;
    char *pTmpEnd   = NULL;
    bool bIsQuit = FALSE;
    bool bIsNeed = IsPropertyNeed(pPropertyBegin, eTag);
    
    if (bIsNeed)
    {
        char ch;
        pTmpTail = strchr(pValue, '\n');
        if (pTmpTail)    //找到换行
        {
            int Len = 0;
            pTmpEnd = pTmpTail;
            if (*(pTmpTail - 1) == '\r')
			{
               pTmpTail--;
            }

            do
            {
                pTmpBegin = pTmpEnd + 1;
                if (*pTmpBegin == ' ' || *pTmpBegin == '\t')//fold
                { 
                    pTmpBegin++;
                    pTmpEnd = strchr(pTmpBegin, '\n');
                    if (pTmpEnd)
					{
                        Len = pTmpEnd - pTmpBegin;
                        if (*(pTmpEnd - 1) == '\r')
						{
                            Len--;
                        }
                    }
					else
					{
                        Len = strlen(pTmpBegin);
                        *ppPropertyEnd = pTmpBegin + Len; 
                        bIsQuit = TRUE;
                    }
                    // Move
                    memmove(pTmpTail, pTmpBegin, Len);
                    pTmpTail += Len;
                }    
                else//没有找到fold的标记
                {
                    char *pTmp = NULL;// 考虑到QUOTED-PRINTABLE
                    
					do 
                    {                        
                        pTmpBegin = pTmpEnd + 1;
                        pTmp = pTmpBegin;
                        pTmpEnd = strchr(pTmpBegin, '\n');
                        if (pTmpEnd)
                        {
                            *pTmpEnd = '\0';
                        }
                        pTmpBegin = strchr(pTmpBegin, ':');
                        if (pTmpBegin)
						{//如果是新的Property,恢复
                            *pTmpEnd = '\n';
                        } 
                        
                    }
					while(!pTmpBegin && pTmpEnd);
                    
                    if (pTmpEnd)
                    {
                        *pTmpEnd = '\n';
                        *ppPropertyEnd = pTmp - 1; 
                        *(*ppPropertyEnd) = '\n';
                    }
                    else
                    {
                        if (!pTmpBegin)
						{   //Not Find and in the end
                            *ppPropertyEnd = pTmp + strlen(pTmp);
                        }
						else
						{
                            *ppPropertyEnd = pTmp - 1; 
                        }
                    }
                    
                    bIsQuit = TRUE;
                }                
            }
			while(!bIsQuit); 
            
            ch = *pTmpTail;
            *pTmpTail = '\0';
        }
        else
        {
            *ppPropertyEnd = pTmpTail + strlen(pTmpTail); 
        }
        // Save Value   
        lResult = SetValueToContVcard(eTag, pValue, pCurContact);
        // comeback
        if (pTmpTail == *ppPropertyEnd)
        {
            *(*ppPropertyEnd) = ch;
        }
    }
  
    else//not need , skip
    {   
        pTmpBegin = pValue;
        do
        {
            pTmpEnd = strchr(pTmpBegin, '\n');
            if (pTmpEnd)
            {
                if (pTmpEnd[1] == ' ' || pTmpEnd[1] == '\t')
				{
                    pTmpBegin = pTmpEnd + 2;
                }
				else
				{
                    char *pTmp = NULL;
                    do 
                    {                       
                        pTmpBegin = pTmpEnd + 1;
                        pTmp = pTmpBegin;
                        pTmpEnd = strchr(pTmpBegin, '\n');
                        if (pTmpEnd)
						{
                            *pTmpEnd = '\0';
                        }
                        pTmpBegin = strchr(pTmpBegin, ':');
                        if (pTmpBegin)
						{//如果是新的Property,恢复
                            *pTmpEnd = '\n';
                        }
                        
                    }
					while(!pTmpBegin && pTmpEnd);
                    
                    if (pTmpEnd)
                    {
                        *pTmpEnd = '\n';
                        *ppPropertyEnd = pTmp - 1; 
                        *(*ppPropertyEnd) = '\n';
                    }
                    else
                    {
                        if (!pTmpBegin)
                        {
                            *ppPropertyEnd = pTmp + strlen(pTmp);
                        }
                        else
                        {
                            *ppPropertyEnd = pTmp - 1; 
                        }
                    }
                    
                    bIsQuit = TRUE;
                }
            }
            else
            {
                *ppPropertyEnd = pTmpBegin + strlen(pTmpBegin); 
                bIsQuit = TRUE;
            }
        }
		while(!bIsQuit);
    }
    return lResult;
}

找到返回位置，未找到返回-1
int IParser::FindTag(char* cFind,char* cTag)
{
	return -1;
}
*/

//查找匹配的子字符串
BOOL IParser::FindPatInText(const char *pucPat, WORD wPatLen, char *pucText, 
							   DWORD dwTextLen, int *pdwPos)
{
    DWORD i, j;
    WORD awBc[256];
    WORD  index;
    
    if (pdwPos == NULL)
    {
        return FALSE;
    }
    if ((!pucPat)||(!pucText)||(wPatLen == 0)||(dwTextLen == 0)||(dwTextLen < wPatLen))
    {
        return FALSE;
    }
    
    *pdwPos = -1;
    //预处理
    PreBc(pucPat, wPatLen, awBc);
    
    //搜索
     i=0;
    while (i <= (dwTextLen - wPatLen))
    {
        for (j = 0; (j <= wPatLen - 1 && (pucPat[j] == pucText[i+j])); j++);
        if (j >= wPatLen)
        {
            *pdwPos = i;
            return TRUE;
        }
        index = (pucText[i + wPatLen]&0xff);
        i += awBc[index];
    }
    
    return FALSE;
}


VOID IParser::PreBc(const char *pucPat, WORD wPatLen, WORD awBc[])
{
    WORD i;
    WORD index;

    for (i = 0; i < 256; i++)
    {
        awBc[i]= wPatLen + 1;
    }
    for (i = 0; i < wPatLen; i++)
    {
        index = pucPat[i]&0xff;
        awBc[index] = wPatLen - i;
    }
}

bool IParser::SearchKey(char *pcontact,int nLine,vCardTag flag)
{
	char LF[2] = {0x0A,0};
	CString strSrc = "";
	CString strSearchKey = "";

	switch (flag)
	{
		case VCARD_FN:
		{
			strSearchKey = NameStrFN;

			break;
		}
		case VCARD_TELCELL:
		{
			strSearchKey = TelNubStrPref;

			break;
		}
		case VCARD_TELWORK:
		{
			strSearchKey = WorkNubStr;

			break;
		}
		case VCARD_TELHOME:
		{
			strSearchKey = HomeNubStr;

			break;
		}
		
		case VCARD_GROUP:
			{
				strSearchKey = GroupStr;
				
				break;
		}
		case VCARD_EMAIL:
		{
			strSearchKey = EmailStrPref;

			break;
		}
		case VCARD_NOTE:
		{
			strSearchKey = NoteStr;

			break;
		}
		default:
		{
			break;
		}
	}

	USES_CONVERSION;
	strSrc = pcontact;
	int nKeyPos = strSrc.Find(strSearchKey,0); //找到FN:
	if (-1 == nKeyPos)
	{
		AfxMessageBox(IDS_FILEERR);

		return false;
	}
	int nSrcLen = strSrc.GetLength();
	if (nSrcLen < nKeyPos)
	{
		AfxMessageBox(IDS_FILEERR);

		return false;
	}
	CString strSrcTemp = strSrc.Right(nSrcLen - nKeyPos);
	strSrc = strSrcTemp;
	
	int nFrom = strSrc.Find(strSearchKey,0);
	int nTo = strSrc.Find(A2W(LF),nFrom);
	if (nTo < nFrom)
	{
		AfxMessageBox(IDS_FILEERR);

		return false;
	}
	strSrc = strSrc.Mid(nFrom,nTo - nFrom);
	int nPos = strSrc.ReverseFind(':');
	if (-1 == nPos)
	{
		AfxMessageBox(IDS_FILEERR);

		return false;
	}
	strSrc = strSrc.Mid(nPos + 1,nTo - nPos);

	if(strSrc.CompareNoCase(_T("")))
	{
		strSrc.TrimLeft();
		strSrc.TrimRight();

		switch (flag)
		{
			case VCARD_FN:
			{
				//注意：需要考虑strSrc字符串的长度是否大于strName的最大长度，可能需要把strSrc进行截断。
				
				if(VCARD_NAME_MAX < strSrc.GetLength())
				{
					strSrc = strSrc.Left(VCARD_NAME_MAX);
				}
				memcpy(m_vCardList.vCardData[nLine].strName,strSrc,strSrc.GetLength()*2);

				break;
			}
			case VCARD_TELCELL:
			{
				if(VCARD_NUM_MAX < strSrc.GetLength())
				{
					strSrc = strSrc.Left(VCARD_NUM_MAX);
				}
				memcpy(m_vCardList.vCardData[nLine].strNumberCell,W2A(strSrc),strSrc.GetLength());

				break;
			}
			case VCARD_TELWORK:
			{
				if(VCARD_NUM_MAX < strSrc.GetLength())
				{
					strSrc = strSrc.Left(VCARD_NUM_MAX);
				}
				memcpy(m_vCardList.vCardData[nLine].strNumberWork,W2A(strSrc),strSrc.GetLength());

				break;
			}
			case VCARD_TELHOME:
			{
				if(VCARD_NUM_MAX < strSrc.GetLength())
				{
					strSrc = strSrc.Left(VCARD_NUM_MAX);
				}
				memcpy(m_vCardList.vCardData[nLine].strNumberHome,W2A(strSrc),strSrc.GetLength());

				break;
			}
			case VCARD_GROUP:
				{
					if(VCARD_GROUP_MAX < strSrc.GetLength())
					{
						strSrc = strSrc.Left(VCARD_GROUP_MAX);
					}
					memcpy(m_vCardList.vCardData[nLine].strGroup,strSrc,strSrc.GetLength()*2);
					
					break;
			}
			case VCARD_EMAIL:
			{
				if(VCARD_EMAIL_MAX < strSrc.GetLength())
				{
					strSrc = strSrc.Left(VCARD_EMAIL_MAX);
				}
				memcpy(m_vCardList.vCardData[nLine].strEmail,strSrc,strSrc.GetLength()*2);

				break;
			}
			case VCARD_NOTE:
			{
				if(VCARD_NOTE_MAX < strSrc.GetLength())
				{
					strSrc = strSrc.Left(VCARD_NOTE_MAX);
				}
				memcpy(m_vCardList.vCardData[nLine].strNote,strSrc,strSrc.GetLength()*2);

				break;
			}
			default:
			{
				break;
			}
		}
	}
	
	return true;
}

bool IParser::ParseARecord(char *Contact,int nIndex)
{
	if (-1 >= nIndex)
	{
		return false;
	}

	if (!SearchKey(Contact,nIndex,VCARD_FN))
	{
		return false;
	}

	if (!SearchKey(Contact,nIndex,VCARD_TELCELL))
	{
		return false;
	}

	if (!SearchKey(Contact,nIndex,VCARD_TELWORK))
	{
		return false;
	}

	if (!SearchKey(Contact,nIndex,VCARD_TELHOME))
	{
		return false;
	}

	if (!SearchKey(Contact,nIndex,VCARD_GROUP))
	{
		return false;
	}


	if (!SearchKey(Contact,nIndex,VCARD_EMAIL))
	{
		return false;		
	}

	if (!SearchKey(Contact,nIndex,VCARD_NOTE))
	{
		return false;
	}
	m_vCardList.nCount++;

	return true;
}

//解析文件中的内容
bool IParser::ParseVcard(vCardContact *pCurContact)
{ 
    char* pPropertyBegin = NULL;
	char *vCardARecord = NULL;  //用于存放一条记录的内容
	int FindPos = 0;
	int nBegin = 0;
	int nEnd = 0;
	int nTag = VCARD_IGNORE;
	int nIndex = -1; //记录索引


	//判断文件是否为空
	if(NULL == pCurContact->pResultData)
	{
		AfxMessageBox(IDS_FILEERR);

		return false;
	}

	//判断结构不符合要求,以BEGI2N:VCARD为开始，以END:VCARD为结束标记
	
	pPropertyBegin = pCurContact->pResultData;

	do
	{
		BOOL bResBegin = FindPatInText(StartStr,strlen(StartStr),(char*)pPropertyBegin,strlen(pPropertyBegin),&FindPos);
		nBegin = FindPos;

		BOOL bResEnd = FindPatInText(EndStr,strlen(EndStr),(char*)pPropertyBegin,strlen(pPropertyBegin),&FindPos);
		nEnd = FindPos;
		if (!bResBegin || !bResEnd || (nBegin >= nEnd))
		{
			AfxMessageBox(IDS_FILEERR);

			return false; 
		}
		//文件结构正确，开始解析
		nIndex++;
		pPropertyBegin = pPropertyBegin + nBegin;
		

		if (NULL != vCardARecord)
		{
			delete []vCardARecord;
			vCardARecord = NULL;
		}
		int nLen = nEnd + strlen(EndStr) - nBegin + 1;  //一条记录的长度
		vCardARecord = new char[nLen];
		if (NULL == vCardARecord)
		{
			AfxMessageBox(IDS_GENERATOR_LESSMEM);

			return false;
		}
		memset(vCardARecord, 0, nLen);
		char *pTest = pPropertyBegin + nLen;
		memcpy(vCardARecord, pPropertyBegin, nLen - 1);
		char * test = vCardARecord;
		
		if (0 <= nIndex)
		{
			//解析一条记录
			 if(!ParseARecord(vCardARecord,nIndex))
			 {
				 return false;
			 }

			//最后一条记录
			int nProper = strlen(pPropertyBegin);
			int nRecordLen = strlen(vCardARecord);
			if(nProper == nRecordLen)
			{
				break;
			}
			//不是最后一条
			pPropertyBegin = pPropertyBegin + nLen;
		}
	}
	while(pPropertyBegin &&(VCARD_OBJ_MAX > (nIndex + 1)));
	
	return true;
}

    /*
    if (NULL == pCurContact ||
        NULL == pCurContact->pResultData ||
        strlen(pCurContact->pResultData) <= 34) //"BEGIN:VCARD\nVERSION:3.0""\nEND:VCARD"
	{
		AfxMessageBox(IDS_FILEERR);

		return false;
	}
    
    pPropertyEnd = pCurContact->pResultData - 1;  // subtract 1 just to uniformly deal
    
    do 
    {
        pPropertyBegin = pPropertyEnd + 1;
        pValue = strchr(pPropertyBegin, ':');
        if (pValue)
		{                            //如果有值，将属性名参数与值分隔
            *pValue = '\0';
            pValue++;
        }
		else
		{
			AfxMessageBox(IDS_FILEERR);

			return false;
		}
        eTag = GetPropertyTag(pPropertyBegin);    //获得属性名称
		if (VCARD_IGNORE == eTag)
		{
			AfxMessageBox(IDS_FILEERR);

			return false;
		}
        
        if (pValue)
		{
			//vCard Obj not end yet
            lResult = GetPropertyValue(eTag,pPropertyBegin,pValue,
										pCurContact,&pPropertyEnd);
        }
        
        if ((lResult != 0)||(*pPropertyEnd == '\0')||(NULL == pValue))
        {
            bIsQuit = TRUE;
        }
        
    }
	while (!bIsQuit);
    
    return true;

}

//将文件中的值放入结构中
int IParser::SetValueToContVcard(int eTag, char *pValue, vCardContact *pCurContact)
{
    int lResult = 0;
    char *pBuffer = pValue;
    bool bIsSet = FALSE;

    int i = GetvCardItemCount();
    switch (eTag)
    {
		case VCARD_FN:
        {
            //if (NULL == pCurContact->pName)
			{
                pCurContact->pName = pBuffer;
                bIsSet = TRUE;
                strcpy(m_vCardList.vCardData[i].strName,pCurContact->pName);                
            }

            break;
        }
        
		case VCARD_TELCELL:
        {
            //if (NULL == pCurContact->pNumberCell)
			{
                pCurContact->pNumberCell = pBuffer;
                bIsSet = TRUE;
                strcpy(m_vCardList.vCardData[i].strNumberCell,pCurContact->pNumberCell);
            }    

			break;
        }
        
		case VCARD_TELWORK:
        {
            //if (NULL == pCurContact->pNumberWork)
			{
                pCurContact->pNumberWork = pBuffer;
                bIsSet = TRUE;
                strcpy(m_vCardList.vCardData[i].strNumberWork,pCurContact->pNumberWork);
            }

			break;
        }
        
		case VCARD_TELHOME:
        {
            //if (NULL == pCurContact->pNumberHome)
			{
                pCurContact->pNumberHome = pBuffer;
                bIsSet = TRUE;
                strcpy(m_vCardList.vCardData[i].strNumberHome,pCurContact->pNumberHome);
            }

			break;
        }
        
		case VCARD_EMAIL:
        {
            //if (NULL == pCurContact->pEmail)
			{
                pCurContact->pEmail = pBuffer;
                bIsSet = TRUE;
                strcpy(m_vCardList.vCardData[i].strEmail,pCurContact->pEmail);
            } 
			
			break;
        }
        
		case VCARD_NOTE:
        {
           // if (NULL == pCurContact->pNote)
			{
                pCurContact->pNote = pBuffer;
                bIsSet = TRUE;
                strcpy(m_vCardList.vCardData[i].strNote,pCurContact->pNote);
            }

			break;
        }
        
		default:
		{
			break; 
		}
   
    }
    if(IsAllPropertyDone(pCurContact))
	{
        pCurContact->pName = NULL;
		pCurContact->pNumberCell = NULL;
		pCurContact->pNumberWork = NULL;
        pCurContact->pNumberHome = NULL;
		pCurContact->pEmail = NULL;
		pCurContact->pNote = NULL;
        
        m_vCardList.nCount++;
    }

    if (!bIsSet)
	{
        free(pBuffer);
    }
    return lResult;
}

当前记录的标记有一项为空则不要了
bool IParser::IsAllPropertyDone(vCardContact *pCurContact)
{
    if( (NULL == pCurContact->pName)||(NULL == pCurContact->pNumberCell)||
        (NULL == pCurContact->pNumberWork)||(NULL == pCurContact->pNumberHome)||
        (NULL == pCurContact->pEmail)||(NULL == pCurContact->pNote) )
	{
		return false;
	}
    else
	{
		return true;
	}
}
*/

/*bool IParser::ImportFromVcf(TCHAR *psBuffer, DWORD dwSize)
{
	vCardContact vCard;
	memset(&vCard,0,sizeof(vCardContact));
	TCHAR LF[2] = {0X000A,' '};
	TCHAR* pTemp = NULL;
	while((!(memcmp(psBuffer,LF,1)))||(!(memcmp(psBuffer,LF + 1,1))))
	{
		psBuffer++;
	}
	TCHAR *tes = psBuffer + wcslen(psBuffer) - 1;
	while((!(memcmp(tes,LF,1)))||(!(memcmp(tes,LF + 1,1))))
	{
		if(NULL != pTemp)
		{
			delete []pTemp;
			pTemp = NULL;
		}
		pTemp = new TCHAR[wcslen(psBuffer)];
		memset(pTemp,0,wcslen(psBuffer));
		memcpy(pTemp,psBuffer,wcslen(psBuffer) - 1);
		memset(psBuffer,0,wcslen(psBuffer));
		memcpy(psBuffer,pTemp,wcslen(pTemp));
		tes = psBuffer + wcslen(psBuffer) - 1;
	}
    vCard.pResultData = psBuffer;
    bool nRes = ParseVcard(&vCard);
	if (!nRes)
	{
		return false;
	}
    return true;
}
*/

bool IParser::ImportFromVcf(char *psBuffer, DWORD dwSize)
{
	vCardContact vCard;
	memset(&vCard,0,sizeof(vCardContact));
	char LF[2] = {0X0A,' '};
	char* pTemp = NULL;
	while((!(memcmp(psBuffer,LF,1)))||(!(memcmp(psBuffer,LF + 1,1))))
	{
		psBuffer++;
	}
	char *tes = psBuffer + strlen(psBuffer) - 1;
	while((!(memcmp(tes,LF,1)))||(!(memcmp(tes,LF + 1,1))))
	{
		if(NULL != pTemp)
		{
			delete []pTemp;
			pTemp = NULL;
		}
		pTemp = new char[strlen(psBuffer)];
		memset(pTemp,0,strlen(psBuffer));
		memcpy(pTemp,psBuffer,strlen(psBuffer) - 1);
		memset(psBuffer,0,strlen(psBuffer));
		memcpy(psBuffer,pTemp,strlen(pTemp));
		tes = psBuffer + strlen(psBuffer) - 1;
	}
    vCard.pResultData = psBuffer;
    bool nRes = ParseVcard(&vCard);
	if (!nRes)
	{
		return false;
	}
    return true;
}
