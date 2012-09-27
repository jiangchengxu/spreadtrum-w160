#ifndef _AFX_HISTITEM_H_
#define _AFX_HISTITEM_H_

/////////////////////////////////////////////////////////////////////////////
//CHistData
class CHistData: public CObject
{
public:
    virtual ~CHistData();
    CHistData();
    BOOL IsEmpty(EnHistKind kind);
    BOOL IsFull(EnHistKind kind);
    USHORT GetCount(EnHistKind kind);
    stHistItem GetItem(EnHistKind kind, USHORT npos);
    void SaveItem(EnHistKind kind, stHistItem &Item);
    void RemoveItem(EnHistKind kind, USHORT npos);
    void RemoveAllItem(EnHistKind kind);
    const TCHAR* GetRawNum(const TCHAR *pNum);
    int  SearchList(EnHistKind kind, LPCTSTR pNumber);
    void LoadHistData();
    BOOL LoadFile();
    void SaveFile();
private:
    stHistList m_HistData[HISTKIND_MAX];
    CString	   m_strHistFileName;
#ifdef FEATURE_OBJECT_SERIAL
    virtual void Serialize(CArchive& ar);
#endif
    DECLARE_SERIAL(CHistData)
};

#endif //#ifndef _AFX_HISTITEM_H_
