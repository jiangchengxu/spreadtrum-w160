////////////////////////////
#ifndef _DATA_EXCH_H_
#define _DATA_EXCH_H_

//#include "CommonStructs.h"
#include "stdafx.h"
#define EXP_PHB    0
#define EXP_SMS    1
#define EXP_NETLOG  2
#define EXP_CALLOG  3
#define EXP_NOTCSV  4


#define VCARD_NAME_MAX    32
#define VCARD_NUM_MAX     32
#define VCARD_GROUP_MAX    20
#define VCARD_EMAIL_MAX   64
#define VCARD_NOTE_MAX    128
#define VCARD_OBJ_MAX     500


typedef struct vCardContact_t {
    char   *pResultData;  // last returned/constructed data
    char   *pName;           // FN tag
    char   *pNumberCell;  // TEL;CELL;VOICE tag
    char   *pNumberHome;  // TEL;HOME;VOICE tag
    char   *pGroup;
    char   *pNumberWork;  // TEL;WORK;VOICE tag
    char   *pEmail;       // EMAIL tag
    char   *pNote;          // NOTE tag
} vCardContact;

struct vCardItem {
    char   strName[VCARD_NAME_MAX*2+2];           // FN tag
    char   strNumberCell[VCARD_NUM_MAX*2+2];  // TEL;CELL;VOICE tag
    char   strNumberHome[VCARD_NUM_MAX*2+2];  // TEL;HOME;VOICE tag
    char   strGroup[VCARD_GROUP_MAX*2+2];
    char   strNumberWork[VCARD_NUM_MAX*2+2];  // TEL;WORK;VOICE tag
    char   strEmail[VCARD_EMAIL_MAX*2+2];       // EMAIL tag
    char   strNote[VCARD_NOTE_MAX*2+2];          // NOTE tag
};

typedef struct _vCardList {
    int nCount;
    vCardItem vCardData[VCARD_OBJ_MAX];
} vCardList;


typedef enum _vCardTag {
    VCARD_BEGIN,
    VCARD_VERSION,
    VCARD_FN,
    VCARD_TELCELL,
    VCARD_TELWORK,
    VCARD_TELHOME,
    VCARD_GROUP,
    VCARD_EMAIL,
    VCARD_NOTE,
    VCARD_IGNORE,
    VCARD_END,
} vCardTag;


namespace DataExchCommFunc
{
CString GetWorkFolder();
}

/*将数据导出生成具体外部文件的接口函数集*/
class IGenerator
{
//Construction&DeConstruction
public:
    IGenerator(CWnd* pParentWnd);

    ~IGenerator();
public:
    CString DefaultDir;
    void RecoverPath();
    bool SetPath(int kind);
    int GetFormat();
    void FillHead(int kind);
    void FillTrail();


    void FillCell(CString& strCell);

    void FillvCardObj(StPbRecord& pbItem);

    void MakeFile();

private:

//用于CSV格式

    //CSV记录的某个属性值是否是复杂的引用型
    bool IsQuotedCell(CString& strCell);

    int  nFileFormat;/*0:CSV    1:VCF*/
    CString m_rsPath;
    CString m_rsData;
    CWnd* pParent;

    //用于VCF格式
    //TAG前缀
    static const char StartStr[];
    static const char NameStrFN[];
    static const char TelNubStrPref[];
    static const char CellNubStr[];
    static const char HomeNubStr[];
    static const char GroupNubStr[];
    static const char WorkNubStr[];
    static const char EmailStrPref[];
    static const char NoteStr[];
    static const char EndStr[];
};


/*从外部文件导入数据的接口函数集*/
class IParser
{
//Construction&DeConstruction
public:
    CStdioFile m_FileImport;
    DWORD m_nFileLength;
    IParser(CWnd* pParentWnd);
    ~IParser();


protected:
    BOOL FindPatInText(const char *pucPat, WORD wPatLen, char *pucText, DWORD dwTextLen, int *pdwPos);
    VOID PreBc(const char *pucPat, WORD wPatLen, WORD awBc[]);
    bool SearchKey(char *pcontact,int nLine,vCardTag flag);
private:
    int         nFileFormat;/*0:CSV    1:VCF*/
    CString     m_rsPath;
    CString        m_rsData;
    CWnd*         pParent;

    static const char StartStr[];
    static const char NameStrFN[];
    static const char TelNubStrPref[];
    static const char CellNubStr[];
    static const char HomeNubStr[];
    static const char GroupStr[];
    static const char WorkNubStr[];
    static const char EmailStrPref[];
    static const char NoteStr[];
    static const char EndStr[];

//{{VCF/////////////////////
public:
    CString GetPath();
    int GetFType();
    DWORD GetFileLen();
    //UINT ReadData(TCHAR* psrcBuf);
    UINT ReadData(char* psrcBuf);
    CString DefaultDir;
    void RecoverPath();
    bool SetPath(int kind);
    //bool ImportFromVcf(TCHAR*psBuffer, DWORD dwSize);
    bool ImportFromVcf(char* psBuffer, DWORD dwSize);
    //bool IsAllPropertyDone(vCardContact *pCurContact);
    //是否为所需要的Property
    //bool IsPropertyNeed(char *pInStr, int eTag);
    //获得Property的Tag
    //int GetPropertyTag(char * pInstr);
    /*int GetPropertyValue(int eTag,
        char * pPropertyBegin,
        char * pValue,
        vCardContact *pCurContact,
        char ** ppPropertyEnd);
    	*/
    bool ParseVcard(vCardContact *pCurContact);
    //int SetValueToContVcard(int eTag, char *pValue, vCardContact *pCurContact);

    int GetvCardItemCount();
    void GetvCardItem(int index,vCardItem *pItem);
    bool ParseARecord(char *Contact,int nIndex);  //用于解析一条记录

private:
    vCardList m_vCardList;  //用于存储解析出来的数据和下标
//}}VCF/////////////////////

};

#endif


