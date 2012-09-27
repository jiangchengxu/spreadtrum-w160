// HistData.cpp : implementation file
//
#include "stdafx.h"
#include "HistData.h"

IMPLEMENT_SERIAL(CHistData, CObject, 0)
/////////////////////////////////////////////////////////////////////////////
// CHistData
CHistData::CHistData()
{
    m_strHistFileName = "HistData.dat";
    memset(m_HistData, 0x00, sizeof(m_HistData));
}

CHistData::~CHistData()
{
}

const TCHAR* CHistData::GetRawNum(const TCHAR *pNum)
{
    ASSERT(pNum && wcslen(pNum) > 0);

    if(!memcmp(pNum, _T("+86"), 3))
        return &pNum[3];
    else
        return pNum;
}

int CHistData::SearchList(EnHistKind kind, LPCTSTR pNumber)
{
    ASSERT(kind >= HISTKIND_MIS && kind < HISTKIND_MAX);
    ASSERT(pNumber != NULL && wcslen(pNumber) > 0);

    int nIndex = SEARCH_NODATA;

    for(int i = 0; i < m_HistData[kind].nCount; i++)
        if(wcscmp(GetRawNum(m_HistData[kind].ItemData[i].szNumber),
                  GetRawNum(pNumber)) == 0)
            break;

    if(i < m_HistData[kind].nCount)
        nIndex = i;
    return nIndex;
}

USHORT CHistData::GetCount(EnHistKind kind)
{
    ASSERT(kind >= HISTKIND_MIS && kind < HISTKIND_MAX);
    return (m_HistData[kind].nCount);
}

stHistItem CHistData::GetItem(EnHistKind kind, USHORT npos)
{
    ASSERT(kind >= HISTKIND_MIS && kind < HISTKIND_MAX);
    ASSERT(npos < m_HistData[kind].nCount);

    return (m_HistData[kind].ItemData[npos]);
}

BOOL CHistData::IsEmpty(EnHistKind kind)
{
    ASSERT(kind >= HISTKIND_MIS && kind <= HISTKIND_MAX);

    if(kind == HISTKIND_MAX)
        return ((m_HistData[HISTKIND_MIS].nCount + m_HistData[HISTKIND_RCV].nCount
                 + m_HistData[HISTKIND_SND].nCount) == 0);
    else
        return (m_HistData[kind].nCount == 0);
}

BOOL CHistData::IsFull(EnHistKind kind)
{
    ASSERT(kind >= HISTKIND_MIS && kind <= HISTKIND_MAX);

    if(kind == HISTKIND_MAX)
        return ((m_HistData[HISTKIND_MIS].nCount == HIST_ITEM_MAX)
                && (m_HistData[HISTKIND_RCV].nCount == HIST_ITEM_MAX)
                && (m_HistData[HISTKIND_SND].nCount == HIST_ITEM_MAX));
    else
        return (m_HistData[kind].nCount == HIST_ITEM_MAX);
}

void CHistData::SaveItem(EnHistKind kind, stHistItem &item)
{
    ASSERT(kind >= HISTKIND_MIS && kind < HISTKIND_MAX);

    BYTE   nSeq = 1;
    int    nIndex = SEARCH_NODATA;

    if((nIndex = SearchList(kind, item.szNumber)) != SEARCH_NODATA) {
        nSeq = GetItem(kind, nIndex).nSeqNum + 1;
        RemoveItem(kind, nIndex);
    }

    if(IsFull(kind))
        RemoveItem(kind, HIST_ITEM_MAX - 1);

    item.nSeqNum = nSeq;

    int i, j;
    for(i = 0; i < m_HistData[kind].nCount; i++)
        if(item.cTimeStamp >= m_HistData[kind].ItemData[i].cTimeStamp)
            break;
    for(j = m_HistData[kind].nCount; j > i; j--)
        m_HistData[kind].ItemData[j] = m_HistData[kind].ItemData[j-1];

    m_HistData[kind].ItemData[i] = item;
    m_HistData[kind].nCount++;
    SaveFile();
}

void CHistData::RemoveAllItem(EnHistKind kind)
{
    ASSERT(kind >= HISTKIND_MIS && kind <= HISTKIND_MAX);

    if(kind == HISTKIND_MAX)
        memset(m_HistData, 0x00, sizeof(m_HistData));
    else
        memset(&m_HistData[kind], 0x00, sizeof(stHistList));
    SaveFile();
}

void CHistData::RemoveItem(EnHistKind kind, USHORT npos)
{
    ASSERT(kind >= HISTKIND_MIS && kind < HISTKIND_MAX);
    ASSERT(npos < m_HistData[kind].nCount);

    USHORT i;
    for(i = npos; i < m_HistData[kind].nCount - 1; i++)
        m_HistData[kind].ItemData[i] = m_HistData[kind].ItemData[i+1];
    memset(&m_HistData[kind].ItemData[i], 0x00, sizeof(stHistItem));
    m_HistData[kind].nCount--;
    SaveFile();
}

void CHistData::LoadHistData()
{
    if(!LoadFile())
        RemoveAllItem(HISTKIND_MAX);
}

BOOL CHistData::LoadFile()
{
    CFile        file;
    CFileStatus  filestatus;

    if(!CFile::GetStatus(m_strHistFileName, filestatus)) {
        file.Open(m_strHistFileName, CFile::modeCreate | CFile::modeWrite);
        file.Close();
        return FALSE;
    }

    file.Open(m_strHistFileName, CFile::modeRead);

#ifdef FEATURE_OBJECT_SERIAL
    CArchive arLoad(&file,CArchive::load);
    Serialize(arLoad);
    arLoad.Close();
#else
    USHORT i, j;
    for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++)
        file.Read(&m_HistData[i].nCount, sizeof(USHORT));

    for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++)
        for(j = 0; j < m_HistData[i].nCount; j++)
            file.Read(&m_HistData[i].ItemData[j], sizeof(stHistItem));
#endif
    file.Close();
    return TRUE;
}

void CHistData::SaveFile()
{
    CFile        file;
    CFileStatus  filestatus;

    if(CFile::GetStatus(m_strHistFileName, filestatus))
        SetFileAttributes(m_strHistFileName, FILE_ATTRIBUTE_ARCHIVE);

    file.Open(m_strHistFileName, CFile::modeCreate | CFile::modeWrite);

#ifdef FEATURE_OBJECT_SERIAL
    CArchive arStore(&file,CArchive::store);
    Serialize(arStore);
    arStore.Close();
#else
    USHORT i, j;
    for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++)
        file.Write(&m_HistData[i].nCount, sizeof(USHORT));

    for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++)
        for(j = 0; j < m_HistData[i].nCount; j++)
            file.Write(&m_HistData[i].ItemData[j], sizeof(stHistItem));
#endif
    file.Close();
}

#ifdef FEATURE_OBJECT_SERIAL
void CHistData::Serialize(CArchive& ar)
{
    stHistList* pHistData;
    CHistData tempHist;
    int i,j,k;

    CObject::Serialize(ar);
    pHistData = (stHistList*)m_HistData;

    if(ar.IsStoring()) {
        for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++) {
            ar<<pHistData[i].nCount;
        }
        for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++) {
            for(j = 0; j < pHistData[i].nCount; j++) {
                ar<<pHistData[i].ItemData[j].nSeqNum;
                for(k = 0; k < PB_NUM_MAX; k++)
                    ar<<pHistData[i].ItemData[j].strNumber[k];
                ar<<pHistData[i].ItemData[j].cTimeStamp
                  <<pHistData[i].ItemData[j].cDuration;
            }
        }
    } else {
        for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++) {
            ar>>pHistData[i].nCount;
        }
        for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++) {
            for(j = 0; j < pHistData[i].nCount; j++) {
                ar>>pHistData[i].ItemData[j].nSeqNum;
                for(k = 0; k < PB_NUM_MAX; k++)
                    ar>>pHistData[i].ItemData[j].strNumber[k];
                ar>>pHistData[i].ItemData[j].cTimeStamp
                  >>pHistData[i].ItemData[j].cDuration;
            }
        }
    }
}
#endif