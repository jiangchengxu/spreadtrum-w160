
#if !defined _PBDATAS_H_
#define _PBDATAS_H_

#include "CommonStructs.h"

class CPbData
{
public:
    CPbData();   // standard constructor
    virtual	~CPbData();
//�ļ�������
    StPbRecord PbDatas[PB_RECORD_MAX];
    StPbGroup PbGroups[PB_GROUP_MAX];
    bool ReadDatas( CString file_name );
    bool GetGroups(CString file_name);
    bool AddTailSingle( StPbRecord PbData,DWORD* ArrItemAds);
    bool AddGroup( StPbGroup PbGroup,DWORD* ArrItemAds );
    bool UpdateMidLine( StPbRecord PbData, int index );
    bool UpdateFile();
    bool UpdateGroupFile();
    bool DeleteAllItems( );
    bool DelSigle( StPbRecord * pPCDelItem );
    bool DelSigleGroup( CString dellItem);
    bool CheckFull( );
    bool CheckGroupFull();
    bool CheckNull( );
    void SetFileAtt(CString strFilename);
    void initPbDatas();
    void initPbGroups();
//�����ӿ���
    bool SearchItemByNumber( CString strNumber, StPbRecord *PbData );
    bool SearchNameByNumber( CString strNumber, TCHAR * Name );


private:
//�ļ�������
    CString CompareAtr( const TCHAR * str );
    void Del( StPbRecord * pPCDelItem );
    void DelGroup(StPbGroup *pGroup);

//�����ӿ���
    CString filename;
    CString groupFileName;
    CFile myFile;
    CFile groupFile;
};

#endif