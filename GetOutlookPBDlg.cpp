// GetOutlookPBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "GetOutlookPBDlg.h"
#include "ExDataExch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetOutlookPBDlg dialog


CGetOutlookPBDlg::CGetOutlookPBDlg(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CGetOutlookPBDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CGetOutlookPBDlg)
    // NOTE: the ClassWizard will add member initialization here
    m_nBkTag = 1;
    //}}AFX_DATA_INIT
    pContactsPtr = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    m_pApp = NULL;
    m_pNS = NULL;
    m_pFolder = NULL;
    m_OulContact.SetSize(EnItemCount); //设置CStringArray的大小

    InitRecordMapping();

}

void CGetOutlookPBDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGetOutlookPBDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetOutlookPBDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CGetOutlookPBDlg)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetOutlookPBDlg message handlers

BOOL CGetOutlookPBDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();
    m_bFirst = TRUE;
    //SetSkin(g_SetData.Setup_nSkinStyle);
    // TODO: Add extra initialization here
    GetDlgItem(IDOK)->EnableWindow(FALSE);
    if (!InitOutlookContactApp()) {
        CDialog::OnCancel();
        return FALSE;
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CGetOutlookPBDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
    if (m_pNS.m_lpDispatch != NULL) {
        m_pNS.Logoff();
    }

    m_pApp.ReleaseDispatch();
    m_OulContact.RemoveAll();
}

void CGetOutlookPBDlg::OnOK()
{
    // TODO: Add extra validation here
    ::AfxGetApp()->DoWaitCursor(1);//显示等待指示的鼠标模式
    ReadMAPIFolder(m_pFolder);
    ::AfxGetApp()->DoWaitCursor(-1);

    CBaseDialog::OnOK();
}

BOOL CGetOutlookPBDlg::InitOutlookContactApp()
{
    COleException e;

    if (!m_pApp.CreateDispatch(_T("Outlook.Application"), &e)) {

        CString str;
        CString strAppName;
        strAppName.LoadString(IDS_APPNAME);
        AddPrefixName(strAppName);
        //str.Format("CreateDispatch() failed w/error 0x%08lx", e.m_sc);
        str.Format(IDS_NOTOUTLOOK);
        this->MessageBox(str, strAppName,MB_SETFOREGROUND);

        return FALSE;
    }
    m_pNS = m_pApp.GetNamespace(_T("MAPI"));  //获得 NameSpace 类型
    if (m_pNS.m_lpDispatch==NULL) {
        return FALSE;
    }

    COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    m_pNS.Logon(covOptional, covOptional, covOptional, covOptional);

    return TRUE;
}

//将m_RM与outlook中的值匹配起来
void CGetOutlookPBDlg::InitRecordMapping()
{
    m_RM.nNameMap = EFullName;

    m_RM.nTel1Map = EMobileTelephoneNumber;
    m_RM.nTel2Map = EHomeTelephoneNumber;
    m_RM.nTel3Map = EBusinessTelephoneNumber;
    m_RM.nFaxMap = EBusinessFaxNumber;

    m_RM.nEmail1Map = EEmail1Address;
    m_RM.nEmail2Map = EEmail2Address;

    m_RM.nURLMap = EWebPage;
    m_RM.nICQMap = EIMAddress;
    m_RM.nMemoMap = EBody;
}

//获取单个联系人的值，并取需要的值放入pbdata中
BOOL CGetOutlookPBDlg::GetContactItem(_ContactItem contact)
{
    BOOL bRes = FALSE;
    SetOulContactValue(contact);

    DWORD PbArrItemAds = NULL;
    StPbRecord *pRecord = new StPbRecord();
    if (pRecord == NULL) {
        return FALSE;
    }
    memset(pRecord,0,sizeof(StPbRecord));

    wcscpy(pRecord->strName,m_OulContact[m_RM.nNameMap]);
    wcscpy(pRecord->strMobile,m_OulContact[m_RM.nTel1Map]);
    wcscpy(pRecord->strHome,m_OulContact[m_RM.nTel2Map]);
    wcscpy(pRecord->strCompany,m_OulContact[m_RM.nTel3Map]);
    wcscpy(pRecord->strEmail,m_OulContact[m_RM.nEmail1Map]);

    if (0 == wcslen(pRecord->strName)
            &&(0 ==wcslen(pRecord->strMobile))
            &&(0 == wcslen(pRecord->strHome))
            &&(0 == wcslen(pRecord->strCompany))
       ) {
        if(pRecord != NULL) {
            delete pRecord;
        }

        return FALSE;
    }
    //检查电话本记录是否符合规范
    if(CheckContactRule(pRecord)) {
        bool bResult = pContactsPtr->AddTailSingle(*pRecord,&PbArrItemAds);
        if (!bResult) {
            CString strPbSaveIfo;
            strPbSaveIfo.LoadString(IDS_PB_SAVEFAILED);
            AfxMessageBox(strPbSaveIfo);
            return FALSE;
        }
        bRes = TRUE;
    } else {
        bRes = FALSE;
    }
    if(pRecord != NULL) {
        delete pRecord;
    }
    return bRes;
}

void CGetOutlookPBDlg::OnButtonBrowse()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_EDIT_FOLDER)->SetWindowText(_T(""));
    GetDlgItem(IDOK)->EnableWindow(FALSE);

    m_pFolder = m_pNS.PickFolder();//显示Outlook文件夹采摘工具
    if (m_pFolder != NULL) {
        CString szVersion = m_pApp.GetVersion();
        if (szVersion.Left(2).Compare(_T("11"))==0) {
            GetDlgItem(IDC_EDIT_FOLDER)->SetWindowText(m_pFolder.GetFolderPath());
        } else {
            GetDlgItem(IDC_EDIT_FOLDER)->SetWindowText(m_pFolder.GetName());
        }
        GetDlgItem(IDOK)->EnableWindow(TRUE);
    }
}

//清除m_OulContact中的值并释放内存
void CGetOutlookPBDlg::ClearOulContactValue()
{
    for (int i = 0; i < EnItemCount; i++) {
        m_OulContact[i].Empty();
    }
}

//从_ContactItem中获取outlook中的联系人内容，并将值保存在m_OulContact中
void CGetOutlookPBDlg::SetOulContactValue(_ContactItem contact)
{
    ClearOulContactValue();

    m_OulContact[EFirstName] = contact.GetFirstName();
    m_OulContact[ELastName] = contact.GetLastName();
    m_OulContact[EFullName] = contact.GetFullName();
    m_OulContact[ENickName] = contact.GetNickName();
    m_OulContact[EBusinessTelephoneNumber] = contact.GetBusinessTelephoneNumber();
    m_OulContact[EBusiness2TelephoneNumber] = contact.GetBusiness2TelephoneNumber();
    m_OulContact[EHomeTelephoneNumber] = contact.GetHomeTelephoneNumber();
    m_OulContact[EHome2TelephoneNumber] = contact.GetHome2TelephoneNumber();
    m_OulContact[EMobileTelephoneNumber] = contact.GetMobileTelephoneNumber();
    m_OulContact[EPrimaryTelephoneNumber] = contact.GetPrimaryTelephoneNumber();
    m_OulContact[EOtherTelephoneNumber] = contact.GetOtherTelephoneNumber();
    m_OulContact[EAssistantTelephoneNumber] = contact.GetAssistantTelephoneNumber();
    m_OulContact[ECallbackTelephoneNumber] = contact.GetCallbackTelephoneNumber();
    m_OulContact[ECarTelephoneNumber] = contact.GetCarTelephoneNumber();
    m_OulContact[ECompanyMainTelephoneNumber] = contact.GetCompanyMainTelephoneNumber();
    m_OulContact[ERadioTelephoneNumber] = contact.GetRadioTelephoneNumber();
    m_OulContact[ETTYTDDTelephoneNumber] = contact.GetTTYTDDTelephoneNumber();
    m_OulContact[EBusinessFaxNumber] = contact.GetBusinessFaxNumber();
    m_OulContact[EHomeFaxNumber] = contact.GetHomeFaxNumber();
    m_OulContact[EOtherFaxNumber] = contact.GetOtherFaxNumber();
    m_OulContact[ETelexNumber] = contact.GetTelexNumber();
    m_OulContact[EWebPage] = contact.GetWebPage();
    m_OulContact[EBody] = contact.GetBody();

    if (contact.GetEmail1AddressType().Compare(_T("EX"))!=0)
        m_OulContact[EEmail1Address] = contact.GetEmail1Address();
    if (contact.GetEmail2AddressType().Compare(_T("EX"))!=0)
        m_OulContact[EEmail2Address] = contact.GetEmail2Address();
    if (contact.GetEmail3AddressType().Compare(_T("EX"))!=0)
        m_OulContact[EEmail3Address] = contact.GetEmail3Address();

    CString szVersion = contact.GetOutlookVersion();
    if (szVersion.Left(2).Compare(_T("11"))==0)
        m_OulContact[EIMAddress] = contact.GetIMAddress();
}

const long olContact_Type = 40;
int CGetOutlookPBDlg::ReadMAPIFolder(MAPIFolder pFolder)
{
    if (pFolder==NULL)
        return 0;

    int nContactCount = 0;
    _Items contactitems = pFolder.GetItems();
    if (contactitems.GetCount()) {
        _ContactItem contact = contactitems.GetFirst();  //_ContactItem 表示单个联系人
        while (contact.m_lpDispatch) {
            if (olContact_Type == contact.GetClass()) {
                if( pContactsPtr->CheckFull()) {
                    AfxMessageBox(IDS_PB_FULL);

                    return 0;
                }
                if (GetContactItem(contact))
                    nContactCount++;
            }
            contact = contactitems.GetNext();
        }
    }

    _Folders outlookfolders = pFolder.GetFolders();
    if (outlookfolders.GetCount()) {
        MAPIFolder childfolder = outlookfolders.GetFirst();
        while (childfolder.m_lpDispatch) {
            nContactCount += ReadMAPIFolder(childfolder);
            childfolder = outlookfolders.GetNext();
        }
    }
    return nContactCount;
}

//检查联系人内容是否符合规范
bool CGetOutlookPBDlg::CheckContactRule(StPbRecord *pPBRecord)
{
    bool bCheckRes = false;
    //检查姓名是否符合规范
    CString strName = _T("");
    strName.Format(pPBRecord->strName);

    if(0 != strName.GetLength()) {
        bCheckRes = CheckPbNameUnicode(strName);
        if(!bCheckRes) {
            return false;
        }
        if(VCARD_NAME_MAX < strName.GetLength()) {
            strName = strName.Left(VCARD_NAME_MAX);
        }
        memcpy(pPBRecord->strName,strName,strName.GetLength());
        //AfxMessageBox(mphb.strName);
    }
    //检查Mobile是否符合规范
    bCheckRes = false;
    CString strMobile = _T("");
    strMobile.Format(pPBRecord->strMobile);
    USES_CONVERSION;

    if (0 != strMobile.GetLength()) {
        if(!CheckPbNumber(strMobile)) {
            return false;
        }
        if(VCARD_NUM_MAX < strMobile.GetLength()) {
            strMobile = strMobile.Left(VCARD_NUM_MAX);
        }
        if (!CompareChar(strMobile)) {
            return false;
        }

        memcpy(pPBRecord->strMobile,strMobile,strMobile.GetLength());
    }
    //检查Home是否符合规范
    bCheckRes = false;
    CString strHome = _T("");
    strHome.Format(pPBRecord->strHome);

    if (0 != strHome.GetLength()) {
        if(!CheckPbNumber(strHome)) {
            return false;
        }
        if(VCARD_NUM_MAX < strHome.GetLength()) {
            strHome = strHome.Left(VCARD_NUM_MAX);
        }
        if (!CompareChar(strHome)) {
            return false;
        }

        memcpy(pPBRecord->strHome,strHome,strHome.GetLength());
    }
    //检查Company是否符合规范
    bCheckRes = false;
    CString strCompany = _T("");
    strCompany.Format(pPBRecord->strCompany);

    if (0 != strCompany.GetLength()) {
        if(!CheckPbNumber(strCompany)) {
            return false;
        }
        if(VCARD_NUM_MAX < strCompany.GetLength()) {
            strCompany = strCompany.Left(VCARD_NUM_MAX);
        }
        if (!CompareChar(strCompany)) {
            return false;
        }

        memcpy(pPBRecord->strCompany,strCompany,strCompany.GetLength());
    }
    //检查Email是否符合规范
    bCheckRes = false;
    CString strEmail = _T("");
    strEmail.Format(pPBRecord->strCompany);
    if (0 != strEmail.GetLength()) {
        if(VCARD_EMAIL_MAX < strEmail.GetLength()) {
            strEmail = strEmail.Left(VCARD_EMAIL_MAX);
        }

        memcpy(pPBRecord->strEmail,strEmail,strEmail.GetLength());
    }
    return true;
}

bool CGetOutlookPBDlg::CheckPbName(const char* ch)
{
    bool bNameUcs = false;  //用于区分姓名中是否有中文，默认为英文

    int nLen = 0;
    while(*ch) {
        BOOL bRes = isChsChar(ch);
        if(bRes) {
            ch = ch + 2;
            bNameUcs = true;
        } else {
            ch++;
        }
        nLen++;
    }
    if(bNameUcs) {
        if (PB_NAME_PC_MAX < nLen) {
            AfxMessageBox(IDS_PB_NAMEMAX);

            return false;
        }
    }
    return true;
}

bool CGetOutlookPBDlg::CheckPbNameUnicode(const TCHAR* ch)
{
    bool bNameUcs = false;  //用于区分姓名中是否有中文，默认为英文

    int nLen = 0;
    while(*ch) {
        BOOL bRes = isChsCharUnicode(ch);
        if(bRes) {
            bNameUcs = true;
        }


        ch++;

        nLen++;
    }
    if(bNameUcs) {
        if (PB_NAME_PC_MAX < nLen) {
            AfxMessageBox(IDS_PB_NAMEMAX);

            return false;
        }
    }
    return true;
}
bool CGetOutlookPBDlg::CheckPbNumber(CString strNum)
{
    if((-1 != strNum.Find('+'))&&(1 == strNum.GetLength())) {
        AfxMessageBox(IDS_CONTACT_NOPLUSONLY);

        return false;
    }
    return true;
}

BOOL CGetOutlookPBDlg::isChsCharUnicode(const TCHAR* ch)
{
    if( (*ch & 0x80 )&&((*ch >> 8)&0x80)) {
        // ch 为汉字或全角字符
        return TRUE;
    }
    return FALSE;
}
BOOL CGetOutlookPBDlg::isChsChar(const char* ch)
{
    if( (*ch & 0x80 )&&(*(ch+1)&0x80)) {
        // ch 为汉字或全角字符
        return TRUE;
    }
    return FALSE;
}
