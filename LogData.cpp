// LogData.cpp : implementation file
//

#include "stdafx.h"
#include "LogData.h"

IMPLEMENT_SERIAL( CLogData, CObject, 0 )
/////////////////////////////////////////////////////////////////////////////
// CLogData

//���캯����mLogData���㣬m_bModifed����ΪFALSE
CLogData::CLogData()
{
    memset(&mLogData,0,sizeof(stLogList));
    m_bModifed = FALSE;
}

//������������
CLogData::~CLogData()
{
}

//���Һ�ָ��������ͬ�ļ�¼����������ֵ��δʵ�֣�
int    CLogData::SearchList(stLogItem *pItem)
{

    int    MemNum = -1;
#if 1   
    int i=0;

    if( mLogData.count >= 0)
    {                                
        for(i;i<mLogData.count;i++)
        {
            if(mLogData.mItemData[i].cTimeStamp == pItem->cTimeStamp)
                break;                
        }
        if(i==mLogData.count)
        {/*Not Found*/
            MemNum = SEARCH_NODATA;
        }
        else
        {
            MemNum = i;
        }
    }
    else
    {                        
        return SEARCH_NODATA;    
    }        
#endif
    return MemNum;
}

//�������ݣ���������¼������ɾ�����һ���������ļ�
BOOL CLogData::SaveItem(stLogItem *pItem)
{
    int index;
    int i;

    /*'.'New Entry'.'*/
    index = mLogData.count;
    if(mLogData.count<LOG_ITEM_MAX)
    {
        memcpy(&(mLogData.mItemData[index]),pItem,sizeof(stLogItem));
        mLogData.count++;
    }
    else
    {
        for(i=0; i<LOG_ITEM_MAX-1; i++)
        {
            mLogData.mItemData[i]=mLogData.mItemData[i+1];
        }
        memcpy(&(mLogData.mItemData[LOG_ITEM_MAX-1]),pItem,sizeof(stLogItem));
        mLogData.count=LOG_ITEM_MAX;
    }
    
    //Write to the local file...
    m_bModifed = TRUE;
    SaveFile();
    return TRUE;
}

//����������
BOOL CLogData::ClearAll()
{
    CFileStatus rStatus;
    CString strPath;
    if(strPath.IsEmpty())
    {
        strPath = szLogDataPath;    
    }
    // If file *.dat does not exist create it.
    if (CFile::GetStatus( strPath, rStatus ) == TRUE) 
    {
        CFile::Remove(strPath);
    }
    mLogData.count=0;

    return TRUE;
}

//ɾ��ָ������λ�õļ�¼�������ļ�
BOOL CLogData::RemoveItem(int npos)
{
    int i;
    for(i=npos;i<mLogData.count-1;i++)
    {
        mLogData.mItemData[i]=mLogData.mItemData[i+1];
    }
    mLogData.count--;
    m_bModifed = TRUE;
    SaveFile();
    return TRUE;
}

//����LogData��
void CLogData::LoadLogData()
{
    if(!LoadFile())
    {
        memset(&mLogData,0,sizeof(stLogList));
        m_bModifed = TRUE;
        SaveFile();
    }
}

//�ӱ����ļ���������
BOOL CLogData::LoadFile(LPCTSTR szFileName)
{
    CFileStatus rStatus;
    CString strPath;
    strPath = szFileName;
    if(strPath.IsEmpty())
    {
        strPath = szLogDataPath;    
    }
    // If file *.dat does not exist create it.
    if (CFile::GetStatus( strPath, rStatus ) == FALSE) 
    {
        CFile LogFile;
        LogFile.Open(strPath, CFile::modeCreate | CFile::modeWrite);
        LogFile.Close();
        return FALSE;
    }
    else
    {
        CFile LogFile;
        LogFile.Open(strPath, CFile::modeRead);
        CArchive arLoad(&LogFile,CArchive::load);
        Serialize(arLoad);
        arLoad.Close();
        LogFile.Close();
    }
    return TRUE;
}

//�����ļ�
BOOL CLogData::SaveFile(LPCTSTR szFileName)
{
    CString strPath;
    CFile LogFile;
    CFileStatus  filestatus;
    strPath = szFileName;
    if(strPath.IsEmpty())
    {
        strPath = szLogDataPath;    
    }
    if (m_bModifed) 
    {
        if(CFile::GetStatus(strPath, filestatus))
            SetFileAttributes(strPath, FILE_ATTRIBUTE_ARCHIVE);
        LogFile.Open(strPath, CFile::modeCreate | CFile::modeWrite);
        CArchive arStore(&LogFile,CArchive::store);
        Serialize(arStore);
        arStore.Close();
        LogFile.Close();
    }
    m_bModifed = FALSE;
    return TRUE;
}

//���л����������������룬��������
void CLogData::Serialize(CArchive& ar)
{
    stLogList* pLogData;
    CLogData tempLog;
    int j;

    CObject::Serialize(ar);
    pLogData = (stLogList*)(&mLogData);
    
    if(ar.IsStoring())
    {
        ar<<pLogData->count;
        
        for(j = 0; j < pLogData->count; j++)
        {
            ar<<pLogData->mItemData[j].nLogType
              <<pLogData->mItemData[j].cTimeStamp
              <<pLogData->mItemData[j].cDuration
              <<pLogData->mItemData[j].dwByteIn
              <<pLogData->mItemData[j].dwByteOut;
        }
    }
    else
    {
        ar>>pLogData->count;
        
        for(j = 0; j < pLogData->count; j++)
        {
            ar>>pLogData->mItemData[j].nLogType
              >>pLogData->mItemData[j].cTimeStamp
              >>pLogData->mItemData[j].cDuration
              >>pLogData->mItemData[j].dwByteIn
              >>pLogData->mItemData[j].dwByteOut;
        }
    }
}

//����Ӹ������ڵ���ǰ��������������(��λ��Byte)
DWORD CLogData::CalculateTotalFlux(const COleDateTime &datetime)
{
    WORD   cnt;
    double dwMByte = 0.0;
    double dwByte = 0.0;
    double dwM = 0.0;

    COleDateTime currtime = COleDateTime::GetCurrentTime();

    for(cnt = 0; cnt < mLogData.count; cnt++)
    {
        if(mLogData.mItemData[cnt].cTimeStamp >= datetime
            && mLogData.mItemData[cnt].cTimeStamp <= currtime)
        {
            dwByte += (mLogData.mItemData[cnt].dwByteIn
                + mLogData.mItemData[cnt].dwByteOut);

			/*
            dwM = dwByte / (1024 * 1024);

            if(dwM >= (1000))  //1GB
            {
                dwMByte += dwM;
                dwByte -= (dwM * 1024 * 1024);
            }
			*/
        }
    }
/*
    dwM = dwByte / (1024 * 1024);
    dwMByte += dwM;

    return (DWORD)dwMByte;
	*/
	return dwByte;
}

//����Ӹ������ڵ���ǰ����������ʱ��(��λ��Sec)
DWORD CLogData::CalculateTotalDuration(const COleDateTime &datetime)
{
    WORD  cnt;
    DWORD dwSec = 0;
    CTimeSpan time(0,0,0,0);

    COleDateTime currtime = COleDateTime::GetCurrentTime();

    for(cnt = 0; cnt < mLogData.count; cnt++)
    {
        if(mLogData.mItemData[cnt].cTimeStamp >= datetime
            && mLogData.mItemData[cnt].cTimeStamp <= currtime)
        {
            time += mLogData.mItemData[cnt].cDuration;
        }
    }
    dwSec = (DWORD)time.GetTotalSeconds();
    return dwSec;
}


