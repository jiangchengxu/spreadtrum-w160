// LogData.cpp : implementation file
//

#include "stdafx.h"
#include "LogData.h"

IMPLEMENT_SERIAL( CLogData, CObject, 0 )
/////////////////////////////////////////////////////////////////////////////
// CLogData

//构造函数，mLogData清零，m_bModifed设置为FALSE
CLogData::CLogData()
{
    memset(&mLogData,0,sizeof(stLogList));
    m_bModifed = FALSE;
}

//析构函数，空
CLogData::~CLogData()
{
}

//查找和指定内容相同的记录，返回索引值（未实现）
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

//保存内容，超出最大记录数，则删除最初一条。保存文件
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

//将计数清零
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

//删除指定索引位置的记录。保存文件
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

//载入LogData，
void CLogData::LoadLogData()
{
    if(!LoadFile())
    {
        memset(&mLogData,0,sizeof(stLogList));
        m_bModifed = TRUE;
        SaveFile();
    }
}

//从本地文件载入数据
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

//保存文件
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

//序列化函数，重载以载入，保存数据
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

//计算从给定日期到当前日期已上网流量(单位：Byte)
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

//计算从给定日期到当前日期已上网时间(单位：Sec)
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


