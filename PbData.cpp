#include "stdafx.h"
#include "PbData.h"
#include "HSDPA.h"
#include "Ds.h"


#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

/*=================================================================
FUNCTION
	CPbData::ReadDatas
DESCRIPTION
	read contacts data from file
INPUT PARAMETERS
	file_name : data file name.
OUTPUT PARAMETERS
	NULL
RETURN VALUE
	bool : read success or fail
DEPENDENCIES
	NULL
=================================================================*/

CPbData::CPbData()
{
    memset(PbDatas,0,sizeof(StPbRecord) * PB_RECORD_MAX);
}

CPbData::~CPbData()
{
}

bool CPbData::ReadDatas( CString file_name )
{
    filename = file_name;
    if( filename.IsEmpty() ) {
        filename = "PbData.dat";
    }
    SetFileAtt(filename);

    CFileException e;
    //open file success
    if( myFile.Open( filename, CFile::modeRead | CFile::typeBinary , &e ) ) {
        if(myFile.Read( PbDatas, sizeof(StPbRecord) * PB_RECORD_MAX)) {
            myFile.Close();
            return true;
        } else {
            myFile.Close( );
            return false;
        }
    }
    //open error,create new null file
    else {
        if( myFile.Open( filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary , &e ) ) {
            initPbDatas();
            myFile.Write(PbDatas, sizeof(StPbRecord) * PB_RECORD_MAX);
            myFile.Close();
            return true;
        } else {
            return false;
        }
        return false;
    }
}

bool CPbData::GetGroups( CString file_name )
{
    groupFileName = file_name;
    if( groupFileName.IsEmpty() ) {
        groupFileName = "PbGroup.dat";
    }
    SetFileAtt(groupFileName);

    CFileException e;
    //open file success
    if( groupFile.Open( groupFileName, CFile::modeRead | CFile::typeBinary , &e ) ) {
        if(groupFile.Read( PbGroups, sizeof(StPbGroup) * PB_GROUP_MAX)) {
            groupFile.Close();
            return true;
        } else {
            groupFile.Close( );
            return false;
        }
    }
    //open error,create new null file
    else {
        if( groupFile.Open( groupFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary , &e ) ) {
            initPbGroups();
            groupFile.Write(PbGroups, sizeof(StPbGroup) * PB_GROUP_MAX);
            groupFile.Close();
            return true;
        } else {
            return false;
        }
        return false;
    }
}




/*=================================================================
FUNCTION
	CPbData :: AddTailSingle
DESCRIPTION
	Add a new contact into array,search first item's index = -1
INPUT PARAMETERS
	PbData : contact need to be add.
	index : position that need to be add.
OUTPUT PARAMETERS
	NULL
RETURN VALUE
	bool : add success or fail
DEPENDENCIES
	NULL
=================================================================*/

bool CPbData :: AddTailSingle( StPbRecord PbData,DWORD* ArrItemAds )
{
    int nIdleItem = -1;
    for(int nIndex = 0; nIndex < PB_RECORD_MAX; nIndex ++) {
        if(-1 == PbDatas[nIndex].index) {
            nIdleItem = nIndex;
            break;
        }
    }

    if(-1 == nIdleItem) {
        CString strPbFull;
        strPbFull.LoadString(IDS_PB_FULL);
        AfxMessageBox(strPbFull);
        return false;
    } else {
        memcpy(&PbDatas[nIdleItem], &PbData, sizeof(StPbRecord));
        PbDatas[nIdleItem].index =  nIdleItem + 1;
    }

    if( !UpdateFile()) {
        Del( &PbDatas[nIdleItem]);
        AfxMessageBox(IDS_PB_ADDPBFAIL);
        return false;
    }

    *ArrItemAds = (DWORD)(&PbDatas[nIdleItem]);

    return true;
}

/*=================================================================
FUNCTION
	CPbData :: AddGroup
DESCRIPTION
	Add a new group into array,search first item's index = -1
INPUT PARAMETERS
	PbData : group needs to be add.
	index : position that need to be add.
OUTPUT PARAMETERS
	NULL
RETURN VALUE
	bool : add success or fail
DEPENDENCIES
	NULL
=================================================================*/
bool CPbData :: AddGroup( StPbGroup PbGroup,DWORD* ArrItemAds )
{
    int nIdleItem = -1;
    for(int nIndex = 0; nIndex < PB_GROUP_MAX; nIndex ++) {
        if(-1 == PbGroups[nIndex].index) {
            nIdleItem = nIndex;
            break;
        }
    }

    if(-1 == nIdleItem) {
        CString strGroupFull;
        strGroupFull.LoadString(IDS_GROUP_FULL);
        AfxMessageBox(strGroupFull);
        return false;
    } else {
        memcpy(&PbGroups[nIdleItem], &PbGroup, sizeof(StPbGroup));
        PbGroups[nIdleItem].index =  nIdleItem + 1;
    }

    if( !UpdateGroupFile()) {
        DelGroup( &PbGroups[nIdleItem]);
        AfxMessageBox(IDS_PB_ADDPBFAIL);
        return false;
    }

    *ArrItemAds = (DWORD)(&PbGroups[nIdleItem]);

    return true;
}


/*=================================================================
FUNCTION
	CPbData :: DelSigle
DESCRIPTION
	delete a contact from memory
INPUT PARAMETERS
	index : contact need to be delete.
OUTPUT PARAMETERS
	NULL
RETURN VALUE
	NULL
DEPENDENCIES
	NULL
=================================================================*/
bool CPbData :: DelSigle( StPbRecord * pPCDelItem )
{
    StPbRecord *PbDataTemp = new StPbRecord();
    memcpy( PbDataTemp , pPCDelItem, sizeof(StPbRecord) );

    Del(pPCDelItem);

    if( UpdateFile()) {
        delete PbDataTemp;
        PbDataTemp = NULL;
        return true;
    } else {
        memcpy( pPCDelItem, PbDataTemp, sizeof(StPbRecord) );
        delete PbDataTemp;
        PbDataTemp = NULL;
        return false;
    }
}


void CPbData :: Del( StPbRecord * pPCDelItem )
{
    memset(pPCDelItem,0,sizeof(StPbRecord));
    pPCDelItem->index = -1;

    return;
}

bool CPbData :: DelSigleGroup( CString groupName )
{
    StPbGroup *PbGroupTemp = new StPbGroup();
    for(int i=0; i<PB_GROUP_MAX; i++) {
        if(PbGroups[i].strGroupName==groupName) {
            memcpy( PbGroupTemp , &PbGroups[i], sizeof(StPbGroup) );
            DelGroup(&PbGroups[i]);
            if( UpdateGroupFile()) {
                delete PbGroupTemp;
                PbGroupTemp = NULL;
                return true;
            } else {
                memcpy( &PbGroups[i], PbGroupTemp, sizeof(StPbGroup) );
                delete PbGroupTemp;
                PbGroupTemp = NULL;
                return false;
            }

        }
    }
    return true;
}

void CPbData :: DelGroup( StPbGroup * pGroup)
{
    memset(pGroup,0,sizeof(StPbGroup));
    pGroup->index = -1;

    return;
}

/*=================================================================
FUNCTION
	CPbData :: UpdateMidLine
DESCRIPTION
	Update specify contact
INPUT PARAMETERS
	PbData : new contact
	index : position that need to be update.
OUTPUT PARAMETERS
	NULL
RETURN VALUE
	bool : update success or fail
DEPENDENCIES
	NULL
=================================================================*/

bool CPbData :: UpdateMidLine( StPbRecord PbData, int index )
{
    int nArrIndex = index - 1;
    StPbRecord *PbDataTemp = new StPbRecord();
    memcpy( PbDataTemp , &PbDatas[nArrIndex], sizeof(StPbRecord) );

    memcpy( &PbDatas[nArrIndex], &PbData, sizeof(StPbRecord) );

    if( UpdateFile() ) {
        delete PbDataTemp;
        return true;
    } else {
        memcpy( &PbDatas[nArrIndex], PbDataTemp, sizeof(StPbRecord) );
        delete PbDataTemp;
        return false;
    }
}

/*=================================================================
FUNCTION
	CPbData :: SetFileAtt
DESCRIPTION
	set file attribute to normal
INPUT PARAMETERS
	strFilename : file need to be set normal attribute
OUTPUT PARAMETERS
	NULL
RETURN VALUE
	NULL
DEPENDENCIES
	NULL
=================================================================*/

void CPbData :: SetFileAtt(CString strFilename)
{
    DWORD dwFileAtt = GetFileAttributes(strFilename);
    if (FILE_ATTRIBUTE_ARCHIVE != dwFileAtt) {
        SetFileAttributes(strFilename,FILE_ATTRIBUTE_ARCHIVE);
    }
}

/*=================================================================
FUNCTION
	CPbData :: UpdateFile
DESCRIPTION
	write memory data to file
INPUT PARAMETERS
	TotalNum : contacts' amount
OUTPUT PARAMETERS
	NULL
RETURN VALUE
	bool : update file success or fail
DEPENDENCIES
	NULL
=================================================================*/

bool CPbData :: UpdateFile()
{
    SetFileAtt(filename);

    CFileException e;

    if( myFile.Open( filename, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate, &e ) ) {
        myFile.Write(PbDatas, sizeof(StPbRecord) * PB_RECORD_MAX);
        myFile.Close( );
        return true;
    } else {
        return false;
    }
    return false;
}

/*=================================================================
FUNCTION
	CPbData :: UpdateGroupFile
DESCRIPTION
	write memory data to phone book group file
INPUT PARAMETERS
	TotalNum : group's amount
OUTPUT PARAMETERS
	NULL
RETURN VALUE
	bool : update file success or fail
DEPENDENCIES
	NULL
=================================================================*/

bool CPbData :: UpdateGroupFile()
{
    SetFileAtt(groupFileName);

    CFileException e;

    if( groupFile.Open( groupFileName, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate, &e ) ) {
        groupFile.Write(PbGroups, sizeof(StPbGroup) * PB_GROUP_MAX);
        groupFile.Close( );
        return true;
    } else {
        return false;
    }
    return false;
}

/*=================================================================
FUNCTION
	CPbData :: DeleteAllItems
DESCRIPTION
	delete all contacts from memory and creat a new empty file
INPUT PARAMETERS
	NULL
OUTPUT PARAMETERS
	NULL
RETURN VALUE
	bool : delete all contacts success or fail
DEPENDENCIES
	NULL
=================================================================*/

bool CPbData :: DeleteAllItems( )
{
    memset( PbDatas , 0, sizeof(StPbRecord)*PB_RECORD_MAX );
    if( filename.IsEmpty( ) ) {
        filename = "PbData.dat";
    }
    SetFileAtt(filename);

    CFileException e;
    if( myFile.Open( filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary , &e ) ) {
        initPbDatas();

        myFile.Write(PbDatas,sizeof(StPbRecord) * PB_RECORD_MAX);
        myFile.Close();
        return true;
    }
    return false;
}

//return true when full
bool CPbData :: CheckFull( )
{
    bool bRes = true;
    for(int nInedx = 0; nInedx < PB_RECORD_MAX; nInedx ++) {
        if (-1 == PbDatas[nInedx].index) {
            bRes = false;

            break;
        }
    }
    return bRes;
}


bool CPbData :: CheckGroupFull( )
{
    bool bRes = true;
    for(int nInedx = 0; nInedx < PB_GROUP_MAX; nInedx ++) {
        if (-1 == PbGroups[nInedx].index) {
            bRes = false;

            break;
        }
    }
    return bRes;
}

//return true when Null
bool CPbData :: CheckNull( )
{
    bool bRes = true;
    for(int nInedx = 0; nInedx < PB_RECORD_MAX; nInedx ++) {
        if (-1 != PbDatas[nInedx].index) {
            bRes = false;

            break;
        }
    }
    return bRes;
}

void CPbData :: initPbDatas()
{
    memset(PbDatas,0,sizeof(StPbRecord) * PB_RECORD_MAX);

    for(int i = 0; i < PB_RECORD_MAX; i++) {
        PbDatas[i].index = -1;
        wcscpy(PbDatas[i].strGroup,_T(""));
    }
    return;
}

void CPbData :: initPbGroups()
{
    memset(PbGroups,0,sizeof(StPbGroup) * PB_GROUP_MAX);

    for(int i = 0; i < PB_GROUP_MAX; i++) {
        PbGroups[i].index = -1;
    }
    return;
}

CString CPbData :: CompareAtr( const TCHAR * str )
{
    int len = 0;
    int match = 0;

    USES_CONVERSION;
    char *chNum=W2A(str);

    len = strlen(szCountryCodeArr[match]);
    while(len) {
        if(!memcmp(chNum, szCountryCodeArr[match], len))
            return (CString)&chNum[len];
        else {
            len = strlen(szCountryCodeArr[++match]);
        }
    }

    return (CString)&chNum[0];
}

//search contact by number
bool CPbData :: SearchItemByNumber( CString strNumber, StPbRecord *PbData )
{
    if( strNumber.GetLength() == 0 )
        return false;

    CString strExNum;
    strExNum = CompareAtr( (LPCTSTR)strNumber );

    int i;
    for( i = 0; i < PB_RECORD_MAX; i++ ) {
        if(-1 != PbDatas[i].index) {
            if( !strExNum.Compare( CompareAtr( PbDatas[i].strMobile ) )
                    || !strExNum.Compare( CompareAtr( PbDatas[i].strHome ) )
                    || !strExNum.Compare( CompareAtr( PbDatas[i].strCompany ) ) ) {
                memcpy( PbData, &PbDatas[i], sizeof(StPbRecord) );
                return true;
            }
        }
    }

    //added by wk begin on 2007-4-26
    if(gbSimInit && pUSIMPbRecord != NULL) {
        for(i = 0; i < g_USimTotalNum; i++) {
            if(-1 != pUSIMPbRecord[i].index) {
                if( !strExNum.Compare( CompareAtr( pUSIMPbRecord[i].strMobile ) )
                        || !strExNum.Compare( CompareAtr( pUSIMPbRecord[i].strHome ) )
                        || !strExNum.Compare( CompareAtr( pUSIMPbRecord[i].strCompany ) ) ) {
                    memcpy( PbData, &pUSIMPbRecord[i], sizeof(StPbRecord) );
                    return true;
                }
            }
        }
    }

    if(gbPcCardInit && pDataCardPbRecord != NULL) {
        for(i = 0; i < g_DataCardTotalNum; i++) {
            if(-1 != pDataCardPbRecord[i].index) {
                if( !strExNum.Compare( CompareAtr( pDataCardPbRecord[i].strMobile ) )
                        || !strExNum.Compare( CompareAtr( pDataCardPbRecord[i].strHome ) )
                        || !strExNum.Compare( CompareAtr( pDataCardPbRecord[i].strCompany ) ) ) {
                    memcpy( PbData, &pDataCardPbRecord[i], sizeof(StPbRecord) );
                    return true;
                }
            }
        }
    }
    //added by wk end on 2007-4-26

    return false;
}

//search contact's name by number
bool CPbData :: SearchNameByNumber( CString strNumber, TCHAR * Name )
{
    if(Name == NULL || strNumber.GetLength() == 0)
        return false;

    CString strExNum;
    strExNum = CompareAtr( (LPCTSTR)strNumber );

    int i;
    for( i = 0; i < PB_RECORD_MAX; i++ ) {
        if(-1 != PbDatas[i].index) {
            if( !strExNum.Compare( CompareAtr( PbDatas[i].strMobile ) )
                    || !strExNum.Compare( CompareAtr( PbDatas[i].strHome ) )
                    || !strExNum.Compare( CompareAtr( PbDatas[i].strCompany ) ) ) {
                wcsncpy( Name, PbDatas[i].strName, PB_NAME_MAX );
                if(wcslen(Name) > 0) //added by wk
                    return true;
            }
        }
    }

    //added by wk begin on 2007-4-26
    if(gbSimInit && pUSIMPbRecord != NULL) {
        for(i = 0; i < g_USimTotalNum; i++) {
            if(-1 != pUSIMPbRecord[i].index) {
                if( !strExNum.Compare( CompareAtr( pUSIMPbRecord[i].strMobile ) )
                        || !strExNum.Compare( CompareAtr( pUSIMPbRecord[i].strHome ) )
                        || !strExNum.Compare( CompareAtr( pUSIMPbRecord[i].strCompany ) ) ) {
                    wcsncpy( Name, pUSIMPbRecord[i].strName, PB_NAME_MAX );
                    if(wcslen(Name) > 0) //added by wk
                        return true;
                }
            }
        }
    }

    if(gbPcCardInit && pDataCardPbRecord != NULL) {
        for(i = 0; i < g_DataCardTotalNum; i++) {
            if(-1 != pDataCardPbRecord[i].index) {
                if( !strExNum.Compare( CompareAtr( pDataCardPbRecord[i].strMobile ) )
                        || !strExNum.Compare( CompareAtr( pDataCardPbRecord[i].strHome ) )
                        || !strExNum.Compare( CompareAtr( pDataCardPbRecord[i].strCompany ) ) ) {
                    wcsncpy( Name, pDataCardPbRecord[i].strName, PB_NAME_MAX );
                    if(wcslen(Name) > 0) //added by wk
                        return true;
                }
            }
        }
    }
    //added by wk end on 2007-4-26

    return false;
}
