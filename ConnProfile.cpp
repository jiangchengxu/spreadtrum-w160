// ConnProfile.cpp: implementation of the CConnProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hsdpa.h"
#include "ConnProfile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CConnProfile, CObject, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConnProfile::CConnProfile()
{
    m_strConnProfileFileName = "ConnProfile.dat";
    m_ConnNum = 0;
    ::ZeroMemory(m_ConnProfile, sizeof(m_ConnProfile));
}

CConnProfile::CConnProfile(CString strFileName)
{
    m_strConnProfileFileName = _T("NdisConnProfile.dat");
    m_ConnNum = 0;
    ::ZeroMemory(m_ConnProfile, sizeof(m_ConnProfile));
}

CConnProfile::~CConnProfile()
{
}

BOOL CConnProfile::EditConnProfile(WORD nIndex, const StConnProfile &profile)
{
    ASSERT(nIndex < m_ConnNum);

    if(profile.bDefault)
    {
        WORD cnt;
        for(cnt = 0; cnt < m_ConnNum; cnt++)
        {
            m_ConnProfile[cnt].bDefault = FALSE;
        }
    }

    m_ConnProfile[nIndex] = profile;
    WriteDataToFile();
    return TRUE;
}

BOOL CConnProfile::AddConnProfile(const StConnProfile &profile)
{
    if(IsFull())
        return FALSE;
    else
    {
        if(profile.bDefault)
        {
            WORD cnt;
            for(cnt = 0; cnt < m_ConnNum; cnt++)
            {
                m_ConnProfile[cnt].bDefault = FALSE;
            }
        }

        m_ConnProfile[m_ConnNum] = profile;
        m_ConnNum++;
        WriteDataToFile();
        return TRUE;
    }
}

//������������
void CConnProfile::LoadConnProfile()
{
    if(!ReadDataFromFile())
        DelallConnProfile();
}

//����ָ��λ�õ�һ����¼
StConnProfile CConnProfile::ReadConnProfile(WORD nIndex)
{
    ASSERT(nIndex < m_ConnNum);
    return (m_ConnProfile[nIndex]);
}

//ɾ��һ����¼
BOOL CConnProfile::DeleteConnProfile(WORD nIndex)
{
    ASSERT(nIndex < m_ConnNum);

    int i;
    for(i = nIndex; i < m_ConnNum - 1; i++)
        m_ConnProfile[i] = m_ConnProfile[i+1];
    ::ZeroMemory(&m_ConnProfile[i], sizeof(StConnProfile));
    m_ConnNum--;

    WriteDataToFile();
    return TRUE;
}

//ɾ�����м�¼
BOOL CConnProfile::DelallConnProfile()
{
    m_ConnNum = 0;
    ::ZeroMemory(m_ConnProfile, sizeof(m_ConnProfile));
    WriteDataToFile();
    return TRUE;
}

//���ص�ǰ��¼��Ŀ
WORD CConnProfile::GetConnNum() const
{
    return (m_ConnNum);
}

//���ص�ǰNdis��¼��Ŀ
WORD CConnProfile::GetNdisConnNum() const
{
    return (m_ConnNum);
}

//���ص�ǰRas��¼��Ŀ
WORD CConnProfile::GetRasConnNum() const
{
    return (m_ConnNum);
}

//�жϵ�ǰ��¼��ĿΪ��
BOOL CConnProfile::IsEmpty() const
{
    return (m_ConnNum == 0);
}

//�жϵ�ǰ��¼��Ŀ����
BOOL CConnProfile::IsFull() const
{
    return (m_ConnNum == CONNECTION_PROFILE_MAX);
}

CString CConnProfile::DeorEncrypt(const TCHAR* strData)     //����   
{
	//CString strTemp = _T("");  
	TCHAR strTemp ;
	CString strCode = _T("");  
	int i = 0;

	for (i = 0; i <= wcslen(strData);i++)   
	{   
	  TCHAR ch = strData[i];   
	  strTemp = ch ^ 0XFFFF;
	  strCode += strTemp;
	} 
	return strCode;		 
} 
  
//���ļ��ж�ȡ����
BOOL CConnProfile::ReadDataFromFile()
{
    WORD         cnt;
    CFile        file;
    CFileStatus  filestatus;
	CString strPwd = _T("");
    
    if(CFile::GetStatus(m_strConnProfileFileName, filestatus))
    {
//        m_strConnProfileFileName=_T("NdisConnProfile.dat");
	    file.Open(m_strConnProfileFileName, CFile::modeRead);
		
        file.Read(&m_ConnNum, sizeof(m_ConnNum));
        
        for(cnt = 0; cnt < m_ConnNum; cnt++)
        {
            file.Read(&m_ConnProfile[cnt], sizeof(StConnProfile));
		    strPwd = DeorEncrypt(m_ConnProfile[cnt].szPassword);
			wcscpy(m_ConnProfile[cnt].szPassword,strPwd);
        }
        file.Close();
        return TRUE;
    }
    else
        return FALSE;
}

//������д���ļ�
BOOL CConnProfile::WriteDataToFile()
{
    WORD         cnt;
    CFile        file;
    CFileStatus  filestatus;
	CString strPwd = _T("");
	CString strPwdTemp = _T("");
    
    if(CFile::GetStatus(m_strConnProfileFileName, filestatus))
        SetFileAttributes(m_strConnProfileFileName, FILE_ATTRIBUTE_ARCHIVE);
    
    file.Open(m_strConnProfileFileName, CFile::modeCreate | CFile::modeWrite);    
    file.Write(&m_ConnNum, sizeof(m_ConnNum));
    
    for(cnt = 0; cnt < m_ConnNum; cnt++)
    {
		strPwd = DeorEncrypt(m_ConnProfile[cnt].szPassword);
		strPwdTemp.Format(m_ConnProfile[cnt].szPassword);
		wcscpy(m_ConnProfile[cnt].szPassword,strPwd);
        file.Write(&m_ConnProfile[cnt], sizeof(StConnProfile));
		wcscpy(m_ConnProfile[cnt].szPassword, strPwdTemp);
    }
    file.Close();
    return TRUE;
}

BOOL CConnProfile::IsExistent(LPCTSTR pEntryName)
{
    ASSERT(pEntryName && wcslen(pEntryName) > 0);

    WORD cnt;
    BOOL bExist = FALSE;

    for(cnt = 0; cnt < m_ConnNum; cnt++)
    {
        if(wcscmp(pEntryName, m_ConnProfile[cnt].szEntryName) == 0)
        {
            bExist = TRUE;
            break;
        }
    }
    return bExist;
}

WORD CConnProfile::GetDefaultConnProfileIndex()
{
    WORD cnt;

    for(cnt = 0; cnt < m_ConnNum; cnt++)
    {
        if(m_ConnProfile[cnt].bDefault)
            break;
    }

    return cnt;
}
