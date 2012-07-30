// SmsData.cpp: implementation of the CSmsData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSDPA.h"
#include "CommonStructs.h"
#include "SmsData.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CSmsData, CObject, 1)
CSmsData::CSmsData()
{
	
}

CSmsData::CSmsData(const TCHAR *fileName)
{
    m_strSmsFileName = fileName;
}

CSmsData::~CSmsData()
{

}

//���ܣ������������
//���룺��
//�������
void CSmsData::ResetSmsData()
{
    DelallSmsRecord(SMS_TYPE_ALL);
}


//���ܣ������ļ�������
//��������
//���: ��
void CSmsData::LoadSmsData()
{
    if(!ReadDataFromFile())
        ResetSmsData();
}

//���ܣ���ȡ��¼
//������type: ���ͣ�nIndex: ����
//���: ��¼
StSmsRecord CSmsData::ReadSmsRecord(EnSmsType type, WORD nIndex)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(nIndex < m_SmsNum[type]);

    return m_SmsRecord[type][nIndex];
}

//���ܣ����Ӽ�¼
//������type:���ͣ�nState:״̬��pNumber:���룬time:ʱ�䣬pContent:����
//      pSCNum:��Ϣ���ĺ��룬flag:��ǣ�nRefCnt:��SMS��ţ�nSeqCnt:��ǰ�κţ�
//      nTotalCnt:�ܶκ�, bAutoReplace:�Ƿ��Զ��滻
//���: TRUE/FALSE
BOOL CSmsData::AddSmsRecord(EnSmsType type, EnSmsState nState, LPCTSTR pNumber,
                            const COleDateTime &time, LPCTSTR pContent, LPCTSTR pSCNum,
                            BYTE flag, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt,
                            BOOL bAutoReplace)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    
    ASSERT(m_SmsNum[type] <= SMS_RECORD_MAX);
    
    if(m_SmsNum[type] == SMS_RECORD_MAX)
    {
        if(bAutoReplace)
            DeleteWhenFull(type);
        else
            return FALSE;
    }

    int i, j;
        
    USES_CONVERSION;
    for(i = 0; i < m_SmsNum[type]; i++)
        if(time >= m_SmsRecord[type][i].timestamp)
            break;
    for(j = m_SmsNum[type]; j > i; j--)
	  {  
        m_SmsRecord[type][j] = m_SmsRecord[type][j-1];
    	}       
    m_SmsRecord[type][i].state = nState;
    m_SmsRecord[type][i].timestamp = time;
	//Modified by Zhou Bin 2008.12.30
//     wcsncpy(m_SmsRecord[type][i].szNumber, pNumber, PB_NUM_MAX);
//     wcsncpy(m_SmsRecord[type][i].szContent, pContent, sizeof(m_SmsRecord[type][i].szContent));
	char *pChNumber=W2A(pNumber);
	char *WCharContent=W2A(pContent);
	strncpy(m_SmsRecord[type][i].szNumber, pChNumber, PB_NUM_MAX);
    strncpy(m_SmsRecord[type][i].szContent, WCharContent, sizeof(m_SmsRecord[type][i].szContent));
    m_SmsRecord[type][i].flag = flag;
    m_SmsRecord[type][i].nRefCnt = nRefCnt;
    m_SmsRecord[type][i].nSeqCnt = nSeqCnt;
    m_SmsRecord[type][i].nTotalCnt = nTotalCnt;
	m_SmsRecord[type][i].m_NoATRspCDS = TRUE;
	char *pChSCNumber=W2A(pSCNum);
   // wcsncpy(m_SmsRecord[type][i].szSCNumber, pSCNum, SMS_SC_NUM_MAX);
	strncpy(m_SmsRecord[type][i].szSCNumber, pChSCNumber, SMS_SC_NUM_MAX);
    m_SmsNum[type]++;
    WriteDataToFile();
    return TRUE;
}

//���ܣ�ɾ����¼
//������type:����,nIndex:����
//���: TRUE/FALSE
BOOL CSmsData::DeleteSmsRecord(EnSmsType type, WORD nIndex)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(nIndex < m_SmsNum[type]);

    int i;
    for(i = nIndex; i < m_SmsNum[type] - 1; i++)
        m_SmsRecord[type][i] = m_SmsRecord[type][i+1];
    ::ZeroMemory(&m_SmsRecord[type][i], sizeof(StSmsRecord));
    m_SmsNum[type]--;

    WriteDataToFile();
    return TRUE;
}

//���ܣ�ɾ��ȫ����¼
//������type:����
//���: TRUE/FALSE
BOOL CSmsData::DelallSmsRecord(EnSmsType type)
{
    ASSERT(type >= SMS_TYPE_INBOX && type <= SMS_TYPE_ALL);

    int i, j;

    if(type == SMS_TYPE_ALL)
    {
        for(i = SMS_TYPE_INBOX; i < SMS_TYPE_ALL; i++)
        {
            m_SmsNum[i] = 0;
            for(j = 0; j < SMS_RECORD_MAX; j++)
                ::ZeroMemory(&m_SmsRecord[i][j], sizeof(StSmsRecord));
        }
    }
    else
    {
        m_SmsNum[type] = 0;
        for(j = 0; j < SMS_RECORD_MAX; j++)
            ::ZeroMemory(&m_SmsRecord[type][j], sizeof(StSmsRecord));
    }

    WriteDataToFile();
    return TRUE;
}

//���ܣ����SMS����
//������type:����
//���: ��¼����
WORD CSmsData::GetSmsNum(EnSmsType type) const
{
    ASSERT(type >= SMS_TYPE_INBOX && type <= SMS_TYPE_ALL);

    if(type == SMS_TYPE_ALL)
        return (m_SmsNum[SMS_TYPE_INBOX] + m_SmsNum[SMS_TYPE_OUTBOX] + m_SmsNum[SMS_TYPE_DRAFT]);
    else
        return m_SmsNum[type];
}

//���ܣ��жϵ�ǰSMS�Ƿ�Ϊ��
//������type:����
//���: TRUE/FALSE
BOOL CSmsData::IsEmpty(EnSmsType type) const
{
    ASSERT(type >= SMS_TYPE_INBOX && type <= SMS_TYPE_ALL);

    if(type == SMS_TYPE_ALL)
        return ((m_SmsNum[SMS_TYPE_INBOX]+m_SmsNum[SMS_TYPE_OUTBOX]+m_SmsNum[SMS_TYPE_DRAFT]) == 0);
    else
        return (m_SmsNum[type] == 0);
}

//���ܣ��жϵ�ǰSMS�Ƿ�����
//������type:����
//���: TRUE/FALSE
BOOL CSmsData::IsFull(EnSmsType type) const
{
    ASSERT(type >= SMS_TYPE_INBOX && type <= SMS_TYPE_ALL);

    if(type == SMS_TYPE_ALL)
        return ((m_SmsNum[SMS_TYPE_INBOX] == SMS_RECORD_MAX)
                && (m_SmsNum[SMS_TYPE_OUTBOX] == SMS_RECORD_MAX)
                && (m_SmsNum[SMS_TYPE_DRAFT] == SMS_RECORD_MAX));
    else
        return (m_SmsNum[type] == SMS_RECORD_MAX);
}

#ifdef FEATURE_OBJECT_SERIAL
void CSmsData::Serialize(CArchive &ar)
{
    int  i;
    WORD type;
    WORD cnt;

    CObject::Serialize(ar);

    if(ar.IsStoring())
    {
        for(type = SMS_TYPE_INBOX; type < SMS_TYPE_ALL; type++)
            ar << m_SmsNum[type];

        for(type = SMS_TYPE_INBOX; type < SMS_TYPE_ALL; type++)
            for(cnt = 0; cnt < m_SmsNum[type]; cnt++)
            {
                ar << m_SmsRecord[type][cnt].nState;
                
                for(i = 0; i < (PB_NUM_MAX+2); i++)
                    ar << m_SmsRecord[type][cnt].szNumber[i];
                
                ar << m_SmsRecord[type][cnt].timestamp;
                
                for(i = 0; i < (SMS_CHAR_MAX+2); i++)
                    ar << m_SmsRecord[type][cnt].szContent[i];
            }    
    }
    else
    {
        for(type = SMS_TYPE_INBOX; type < SMS_TYPE_ALL; type++)
            ar >> m_SmsNum[type];
        
        for(type = SMS_TYPE_INBOX; type < SMS_TYPE_ALL; type++)
            for(cnt = 0; cnt < m_SmsNum[type]; cnt++)
            {
                ar >> m_SmsRecord[type][cnt].nState;
                
                for(i = 0; i < (PB_NUM_MAX+2); i++)
                    ar >> m_SmsRecord[type][cnt].szNumber[i];
                
                ar >> m_SmsRecord[type][cnt].timestamp;
                
                for(i = 0; i < (SMS_CHAR_MAX+2); i++)
                    ar >> m_SmsRecord[type][cnt].szContent[i];
            }
    }
}
#endif

//���ܣ����ļ��ж�ȡSMS
//��������
//���: TRUE/FALSE
BOOL CSmsData::ReadDataFromFile()
{
    CFile        file;
    CFileStatus filestatus;
    
    if(CFile::GetStatus(m_strSmsFileName, filestatus))
    {
        file.Open(m_strSmsFileName, CFile::modeRead|CFile::typeBinary );
        
#ifdef FEATURE_OBJECT_SERIAL
        CArchive ar(&file, CArchive::load);
        Serialize(ar);
        ar.Close();
#else
        WORD type;
        WORD cnt;

        file.Read(m_SmsNum, sizeof(m_SmsNum));
        
        for(type = SMS_TYPE_INBOX; type < SMS_TYPE_ALL; type++)
            for(cnt = 0; cnt < m_SmsNum[type]; cnt++)
            {
                file.Read(&m_SmsRecord[type][cnt], sizeof(StSmsRecord));
            }
#endif
        file.Close();
        return TRUE;
    }
    else
        return FALSE;
}

//���ܣ���SMSд���ļ���
//��������
//���: TRUE/FALSE
BOOL CSmsData::WriteDataToFile()
{
    CFile        file;
    CFileStatus  filestatus;
    
    if(CFile::GetStatus(m_strSmsFileName, filestatus))
        SetFileAttributes(m_strSmsFileName, FILE_ATTRIBUTE_ARCHIVE);
     
    file.Open(m_strSmsFileName, CFile::modeCreate | CFile::modeWrite);

#ifdef FEATURE_OBJECT_SERIAL
    CArchive ar(&file, CArchive::store);
    Serialize(ar);
    ar.Close();
#else
    WORD type;
    WORD cnt;
    
    file.Write(m_SmsNum, sizeof(m_SmsNum));
    
    for(type = SMS_TYPE_INBOX; type < SMS_TYPE_ALL; type++)
        for(cnt = 0; cnt < m_SmsNum[type]; cnt++)
        {
            file.Write(&m_SmsRecord[type][cnt], sizeof(StSmsRecord));
        }
#endif
    file.Close();
    return TRUE;
}

#ifdef FEATURE_TEXT_MODE
void CSmsData::TranslateRecordToString(const StSmsRecord &record, TCHAR *str)
{
    memset(str, 0, SMS_RECORDLINE_LENGTH);

    swprintf(str, _T("%d|%s|%ld|%s\r\n"), record.nState, record.szNumber,
                                     record.timestamp.GetTime(), record.szContent);
}

void CSmsData::TranslateStringToRecord(const TCHAR *str, StSmsRecord *record)
{
    ASSERT(str != NULL);

    TCHAR *ptr1, *ptr2;
    TCHAR szWork[SMS_RECORDLINE_LENGTH] = {0};

    wcsncpy(szWork, str, SMS_RECORDLINE_LENGTH);

    ptr1 = ptr2 = szWork;

    while(*ptr2 != '|') ptr2++;
    *ptr2 = 0;
    record->nState = _ttoi(ptr1);
    ptr1 = ++ptr2;
    
    while(*ptr2 != '|') ptr2++;
    *ptr2 = 0;
    wcsncpy((TCHAR*)record->szNumber, ptr1, PB_NUM_MAX);
    ptr1 = ++ptr2;

    while(*ptr2 != '|') ptr2++;
    *ptr2 = 0;
    record->timestamp = (time_t)ttoi(ptr1);
    ptr1 = ++ptr2;

    wcsncpy((TCHAR*)record->szContent, ptr1, SMS_CHAR_MAX);
}
#endif

//���ܣ�����SMS��¼
//������type:���ͣ�record:��¼��AutoReplace:�Ƿ��Զ��滻
//���: TRUE/FALSE
BOOL CSmsData::AddSmsRecord(EnSmsType type, StSmsRecord record, BOOL bAutoReplace)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(m_SmsNum[type] <= SMS_RECORD_MAX);
    
    if(m_SmsNum[type] == SMS_RECORD_MAX)
    {
        if(bAutoReplace)
            DeleteWhenFull(type);
        else
            return FALSE;
    }

    int i, j;
    
    for(i = 0; i < m_SmsNum[type]; i++)
        if(record.timestamp >= m_SmsRecord[type][i].timestamp)
            break;
    for(j = m_SmsNum[type]; j > i; j--)
        m_SmsRecord[type][j] = m_SmsRecord[type][j-1];
        
    m_SmsRecord[type][i] = record;    
    m_SmsNum[type]++;
	if (record.m_NoATRspCDS == TRUE)
	{
	    WriteDataToFile();
	}

    return TRUE;
}


//���ܣ��༭SMS��¼
//������type:���ͣ�nIndex:������record:��¼
//���: TRUE/FALSE
BOOL CSmsData::EditSmsRecord(EnSmsType type, WORD nIndex, const StSmsRecord &record)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(nIndex < m_SmsNum[type]);
    m_SmsRecord[type][nIndex] = record;
    WriteDataToFile();
    return TRUE;
}

//���ܣ���¼��ʱɾ�������һ����¼
//������type:����
//���: ��
void CSmsData::DeleteWhenFull(EnSmsType type)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
    ASSERT(m_SmsNum[type] == SMS_RECORD_MAX);

    int cnt = -1;

    if(type == SMS_TYPE_INBOX)
    {
        for(cnt = SMS_RECORD_MAX-1; cnt >= 0; cnt--)
            if(m_SmsRecord[type][cnt].state == SMS_STATE_MT_READ)
                break;
    }

    if(cnt < 0)
        DeleteSmsRecord(type, SMS_RECORD_MAX-1);
    else
        DeleteSmsRecord(type, cnt);
}


//���ܣ����δ��SMS����
//��������
//���: ����
WORD CSmsData::GetUnreadSmsNum()
{
    WORD nUnreadNum = 0;

    for(WORD i = 0; i < m_SmsNum[SMS_TYPE_INBOX]; i++)
    {
        if(m_SmsRecord[SMS_TYPE_INBOX][i].state == SMS_STATE_MT_NOT_READ)
            nUnreadNum++;
    }

    return nUnreadNum;
}


//���ܣ����SMS���
//������type:���ͣ�Clearflag:��ǣ�bAll:�Ƿ�������м�¼�ı��
//���: TRUE/FALSE
BOOL CSmsData::ClearSmsFlag(EnSmsType type, BYTE Clearflag, BOOL bAll)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);

    BOOL ret = FALSE;

    for(WORD i = 0; i < m_SmsNum[type]; i++)
    {
        if(m_SmsRecord[type][i].flag & Clearflag)
        {
            ret = TRUE;
            m_SmsRecord[type][i].flag &= (BYTE)~Clearflag;
            if(!bAll)
                break;
        }
    }

    if(ret)
        WriteDataToFile();
    
    return ret;
}

//���ܣ�ɾ��ָ����ǵ����м�¼
//������type:���ͣ�flag:���
//���: TRUE/FALSE
BOOL CSmsData::DelallRecordbyFlag(EnSmsType type, BYTE flag)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);

    BOOL ret = FALSE;

    for(WORD i = 0; i < m_SmsNum[type]; i++)
    {
        if(m_SmsRecord[type][i].flag & flag)
        {
            ret = TRUE;
            DeleteSmsRecord(type, i);
            i--;
        }
    }
    
    return ret;
}
