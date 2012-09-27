#ifndef _AFX_CALLLOGITEM_H_
#define _AFX_CALLLOGITEM_H_

// LogData.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CLogData
class CLogData: public CObject
{
public:
    DWORD CalculateTotalDuration(const COleDateTime &datetime);
    DWORD CalculateTotalFlux(const COleDateTime &datetime);
    /*{{The interface--------------------------*/
    ////���Һ�ָ��������ͬ�ļ�¼����������ֵ��δʵ�֣�
    int SearchList(stLogItem *pItem);
    BOOL SaveItem(stLogItem *pItem);
    int  GetCount() {
        return mLogData.count;
    }
    stLogItem GetItem(int npos) {
        return mLogData.mItemData[npos];
    }
    BOOL IsEmpty() {
        return (mLogData.count == 0);
    }
    /*---------------------------------------}}*/
    void LoadLogData(void);

    BOOL LoadFile(LPCTSTR szFileName = NULL);
    BOOL SaveFile(LPCTSTR szFileName = NULL);

    BOOL RemoveItem(int npos);
    BOOL ClearAll();

    BOOL m_bModifed;
    stLogList mLogData;

    /*The destructor takes care of destroying all items*/
    virtual ~CLogData();
    DECLARE_SERIAL( CLogData )
    /*The constructor*/
    CLogData();

    //���л����������������룬��������
    virtual void Serialize(CArchive& ar);
};

const char szLogDataPath[] = "LogData.dat";

#endif //#ifndef _AFX_CALLLOGITEM_H_
