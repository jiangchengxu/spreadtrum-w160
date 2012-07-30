// ContactDlg.cpp : implementation file
/*=================================================================
FILE��   ContactDlg.cpp                            VERSION : 


DESCRIPTION
  ʵ����Ӻ��޸ĵ绰����¼�Ĺ��ܡ�����InsertIndex����-1ʱ��ʾ��ӡ�����Ϣģ���ͨ����¼ģ�����ȡ�����ӹ��ܣ�InsertIndex����-2��ʾ�༭�绰����¼�ӹ��ܡ�
  

=================================================================*/

/*=================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
 Notice that changes are listed in reverse chronological order.

 when        who         what, where, why
 --------   -----       ---------------------------------------------
 
=================================================================*/

#include "stdafx.h"
#include "HSDPA.h"
#include "ContactDlg.h"
#include "HSDPADLG.h"

#include "Picture.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog


CContactDlg::CContactDlg(CWnd* pParent, int InsertIndex,int Pos,TCHAR *cNumber)
    : CBaseDialog(CContactDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CContactDlg)
    m_strName = _T("");
    m_strMobile = _T("");
    m_strHome = _T("");
	m_strGroup=_T("");
	m_strGroupBeforechange=_T("");
    m_strCompany = _T("");
    m_strEmail = _T("");
    m_strMark = _T("");
    iInsertIndex = InsertIndex;
    iPos = Pos;
    //}}AFX_DATA_INIT
    wcscpy(strNumber,cNumber);
	 m_nBkTag = 1;
	m_strPicPath = _T("");
	m_strPicOldPath =  _T("");
	bLoadPic = false;
	
}

void CContactDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CContactDlg)
    DDX_Control(pDX, IDC_EDIT_NAME, m_EditName);
	DDX_Control(pDX, IDC_EDIT_MOBILE, m_EditMobile);
    DDX_Control(pDX, IDC_EDIT_HOME, m_EditHome);
    DDX_Control(pDX, IDC_EDIT_COMPANY, m_EditCompany);
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_EditEMail);
	DDX_Control(pDX, IDC_EDIT_MARK, m_EditMark);
	DDX_Control(pDX,IDC_COMBO_GROUP,m_ComboGroup);

    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
    DDV_MaxChars(pDX, m_strName, 32);

    DDX_Text(pDX, IDC_EDIT_MOBILE, m_strMobile);
    DDV_MaxChars(pDX, m_strMobile, 32);

    DDX_Text(pDX, IDC_EDIT_HOME, m_strHome);
    DDV_MaxChars(pDX, m_strHome, 32);

    DDX_Text(pDX, IDC_EDIT_COMPANY, m_strCompany);
    DDV_MaxChars(pDX, m_strCompany, 32);

    DDX_Text(pDX, IDC_EDIT_EMAIL, m_strEmail);
    DDV_MaxChars(pDX, m_strEmail, 64);

    DDX_Text(pDX, IDC_EDIT_MARK, m_strMark);
    DDV_MaxChars(pDX, m_strMark, 64);

	DDX_Text(pDX,IDC_COMBO_GROUP,m_strGroup);

    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CContactDlg)
	ON_BN_CLICKED(IDC_LOADPIC, OnLoadPic)
	ON_EN_UPDATE(IDC_EDIT_COMPANY, OnUpdateEditCompany)
	ON_WM_CHAR()
	ON_EN_UPDATE(IDC_EDIT_MOBILE, OnUpdateEditMobile)
	ON_EN_UPDATE(IDC_EDIT_HOME, OnUpdateEditHome)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactDlg message handlers
BOOL CContactDlg::isChineseChar(const char* ch)
{
    if( (*ch & 0x8000 )&&(*(ch+1)&0x8000))
    {
        // ch Ϊ���ֻ�ȫ���ַ�
        return TRUE;
    }
    return FALSE;
}

BOOL CContactDlg::isChineseCharUnicode(const TCHAR* ch)
{
//     if( ((*ch>>8) & 0x0080 )&&(*ch&0x0080))
//     {
//         // ch Ϊ���ֻ�ȫ���ַ�
//         return TRUE;
//     }
	if(*ch>0x80)
    {
        // ch Ϊ���ֻ�ȫ���ַ�
        return TRUE;
    }
    return FALSE;
}
bool CContactDlg::CheckName(const char* ch)
{
	bool bNameUcs = false;  //���������������Ƿ������ģ�Ĭ��ΪӢ��

	int nLen = 0;
	while(*ch)
	{
		BOOL bRes = isChineseChar(ch);
		if(bRes)
		{
			ch = ch + 2;
			bNameUcs = true;
		}
		else
		{
			ch++;
		}
		nLen++;
	}
	if(bNameUcs)
	{
		if (PB_NAME_PC_MAX < nLen)
		{
			AfxMessageBox(IDS_CONTACT_NAMETOOLONG);

			return false;
		}
	}
	return true;
}
bool CContactDlg::CheckNameUnicode(const TCHAR* ch)
{
	bool bNameUcs = false;  //���������������Ƿ������ģ�Ĭ��ΪӢ��

	int nLen = 0;
	while(*ch)
	{
		BOOL bRes = isChineseCharUnicode(ch);
		if(bRes)
		{
			ch=ch++;
			bNameUcs = true;
		}
		else
		{
			ch++;
		}
		nLen++;
	}
	if(bNameUcs)
	{
		if (PB_NAME_PC_MAX < nLen)
		{
			AfxMessageBox(IDS_CONTACT_NAMETOOLONG);

			return false;
		}
	}
	return true;
}

bool CContactDlg::CheckNumber(CString strNum)
{
	if((-1 != strNum.Find('+'))&&(1 == strNum.GetLength()))
	{
		AfxMessageBox(IDS_CONTACT_NOPLUSONLY);

		return false;
	}
	return true;
}

void CContactDlg::OnOK() 
{
    // TODO: Add extra validation here
	CFile FileTemp;
	
    int bRes = UpdateData(true);
	if(!bRes)
	{
		return;
	}

	//unchanged
	if((iInsertIndex == CONTACTDLG_TYPE_MODIFY)
		&&(!m_EditName.GetModify())
	    &&(!m_EditMobile.GetModify())
		&&(!m_EditHome.GetModify())
		&&(!m_EditCompany.GetModify())
		&&(!m_EditEMail.GetModify())
		&&(!m_EditMark.GetModify())
		&&(m_strGroup==m_strGroupBeforechange)
		&&(!bLoadPic))
	{
		CDialog::OnCancel();
		return;	
	}
	//��������ȫ��Ϊ�գ���ֱ�ӷ���
    if ((_T("") == m_strName) && (_T("") == m_strMobile) && 
        (_T("") == m_strHome) && (_T("") == m_strCompany))
    {
        return;
    }	
	//��������Ƿ���Ϲ淶
	bool bCheckRes = CheckNameUnicode(m_strName);
	if(!bCheckRes)
	{
		return;
	}

#ifndef FEATURE_VERSION_NOSIM
	//������������Ƿ񳬹�14���������ȸ�����ʾ�ٴ洢
	if(PB_NAME_SIM_MAX < m_strName.GetLength())
	{
		CString strAppNameTemp;
		strAppNameTemp.LoadString(IDS_APPNAME);
		AddPrefixName(strAppNameTemp);
		CString strNameMax;
		strNameMax.LoadString(IDS_PB_NAMELENMAX);
		if(this->MessageBox(strNameMax,strAppNameTemp,MB_OKCANCEL) == IDCANCEL)
		{
			return;
		}
	}
#endif	
	//�������Ƿ���Ϲ淶
	if(!CheckNumber(m_strMobile))
	{
		return;
	}
	if(!CheckNumber(m_strHome))
	{
		return;
	}
	if(!CheckNumber(m_strCompany))
	{
		return;
	}	
    //���Ӻŵ�λ���Ƿ���Ϲ淶
	int nMobile = m_strMobile.Find('+',1);
	int nHome = m_strHome.Find('+',1);
	int nCompany = m_strCompany.Find('+',1);
	if ((-1 != nMobile)||(-1 != nHome)||(-1 != nCompany))
	{
		CString strAppNameTemp;
		strAppNameTemp.LoadString(IDS_APPNAME);
		AddPrefixName(strAppNameTemp);
		CString strPlus;
		strPlus.LoadString(IDS_CONTACT_PLUS);
		this->MessageBox(strPlus,strAppNameTemp,MB_OK);
		return;
	}
	/*����ʼ���ַ�Ƿ���Ϲ淶,
	 *����������ȫ�����㣺@�ַ�������ڣ����ڵ�һλ��
	 *�������һλ,�����ʼ���ַ�����ҽ���һ��@�ַ���
	 *@�ַ�ǰ��������ַ���������ΪС���㣬
	 ��@�ַ�����һ��λ�ú�����һ���ַ�'.'���Ҹ�С������Ų����������һλ��
	 **/
	if (m_strEmail != "")
	{
		int nPos = m_strEmail.Find('@',0);
		int temp = m_strEmail.Find('.',nPos + 1);
		if ((-1 != nPos))
			{
				if((0 != nPos)&&(m_strEmail.GetLength() - 1 != nPos)
					&&(m_strEmail.ReverseFind ('@') == nPos)
					&&(nPos - 1 != m_strEmail.Find('.',nPos - 1))
					&&(nPos + 1 != m_strEmail.Find('.',nPos + 1))
					&&(-1 != m_strEmail.Find('.',nPos + 2))
					&&(m_strEmail.GetLength() - 1 != m_strEmail.Find('.',0)))
				{
				}
				else
				{
					CString strAppNameTemp;
					strAppNameTemp.LoadString(IDS_APPNAME);
					AddPrefixName(strAppNameTemp);
					CString strAchar;
					strAchar.LoadString(IDS_CONTACT_ERRMAIL);
					this->MessageBox(strAchar,strAppNameTemp,MB_OK);  //IDS_CONTACT_ERRMAIL
					return;
				}
		}
		else
		{
			CString strAppNameTemp;
			strAppNameTemp.LoadString(IDS_APPNAME);
			AddPrefixName(strAppNameTemp);
			CString strAchar;
			strAchar.LoadString(IDS_CONTACT_A);
			this->MessageBox(strAchar,strAppNameTemp,MB_OK);  //IDS_CONTACT_A
			return;		
		}
	}

	if(isChineseCharUnicode(m_strMark))
	{
		if(m_strMark.GetLength()>32)
		{
			AfxMessageBox(_T("Chinese charcters cannot be more than 32!"));
			return;
		}
	}
	

	/*
	**����ͼƬ��Ϣ�������������򽫵�ǰ��ʾ��ͼƬ������PICĿ¼�У�
	**�����޸��Ҹ���ͼƬ�ģ��򿽱��ɹ��󣬽�ԭͼƬ(·����default.jpg��·��)ɾ����
	**��ɺ�m_strPicPath�д洢ƥ���ͼƬ�ļ���·��
	*/
	if((bLoadPic)||(iInsertIndex == CONTACTDLG_TYPE_ADD))
	{
		CString strNewPath = GetPicNewPath();
		if(!CopyPicFile(m_strPicPath,strNewPath))
		{
			return;
		}
		if(iInsertIndex == CONTACTDLG_TYPE_MODIFY)
		{
			//�����ɾ����ͼƬ�Ƿ�ΪĬ��ͼƬ���粻����ɾ��
			CString strComPath = GetPicDefPath();
			if (strComPath.CompareNoCase(m_strPicOldPath))
			{
				if (m_strPicOldPath != _T(""))
				{
					TRY
					{
						FileTemp.Remove( m_strPicOldPath );
					}
					CATCH( CFileException, e )
					END_CATCH
				}
			}
		}
		m_strPicPath = strNewPath;     //save new pic file path
	}

    StPbRecord *PbBookTemp = new StPbRecord();
	memset(PbBookTemp,0,sizeof(StPbRecord));
	if(m_strName.Compare(_T(""))==0)
	{
		m_strName.LoadString(IDS_UNKNOWN_NAME);
		UpdateData(FALSE);
	}


	
    wcscpy(PbBookTemp->strName,(LPCTSTR)m_strName);
    wcscpy(PbBookTemp->strMobile,(LPCTSTR)m_strMobile);
    wcscpy(PbBookTemp->strHome,(LPCTSTR)m_strHome);
	wcscpy(PbBookTemp->strGroup,(LPCTSTR)m_strGroup);
    wcscpy(PbBookTemp->strCompany,(LPCTSTR)m_strCompany);
    wcscpy(PbBookTemp->strEmail,(LPCTSTR)m_strEmail);
    wcscpy(PbBookTemp->strMark,(LPCTSTR)m_strMark);
	wcscpy(PbBookTemp->strPicPath,(LPCTSTR)m_strPicPath);
    
    //����modify���ͷ�����Ϣ
    if(iInsertIndex != CONTACTDLG_TYPE_MODIFY)
    {
        //����Ϣ��������
        WPARAM wParam = iInsertIndex;
        LPARAM lParam = (long)PbBookTemp;

        CWnd *pWnd = NULL;

        if(iInsertIndex == CONTACTDLG_TYPE_EXTRACT)
        {
            pWnd = GetParent()->GetParent();
            if(pWnd)
			{
                GetParent()->PostMessage(WM_ABSTRACT_NUM, 0);
			}
			CHSDPADlg* pMainWnd=(CHSDPADlg *)(AfxGetMainWnd());
			pMainWnd->m_pPhoneBookDlg->nCurSelIndex=0;
			pMainWnd->SendMessage(WM_ADD_CONTACT, wParam, lParam);
			pMainWnd->m_pPhoneBookDlg->InitListCtrl(2);
			pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_ReDrawListCtrl);
        }
		else
		{
			CHSDPADlg* pMainWnd=(CHSDPADlg *)(AfxGetMainWnd());
			pMainWnd->SendMessage(WM_ADD_CONTACT, wParam, lParam);
			pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_ReDrawListCtrl);
		}
		
        
    }

    delete PbBookTemp;

    CDialog::OnOK();
}

BOOL CContactDlg::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
    
    // TODO: Add extra initialization here
//     m_bFirst = TRUE;
// 
// 	SetSkin(g_SetData.Setup_nSkinStyle);

    UpdateData(FALSE);
    
    if(iPos > 0 && iPos < 4)
	{
		switch(iPos)
		{
			case 1:
			{			
				if(!CompareChar(strNumber))
				{
					return false;
				}
				m_strMobile = strNumber;

				break;
			}
			case 2:
			{
				if(!CompareChar(strNumber))
				{
					return false;
				}
				m_strHome = strNumber;

				break;
			}
			case 3:
			{
				if(!CompareChar(strNumber))
				{
					return false;
				}
				m_strCompany = strNumber;

				break;
			}
			default:
			{
				break;
			}
		}
	}		
	if(m_bSimCardFlag)
	{
		m_ComboGroup.EnableWindow(FALSE);
		m_EditHome.EnableWindow(FALSE);
		m_EditCompany.EnableWindow(FALSE);
		m_EditMark.EnableWindow(FALSE);
		m_EditEMail.EnableWindow(FALSE);
		GetDlgItem(IDC_LOADPIC)->EnableWindow(FALSE);		
	}
	else
	{
		for(int i = 0; i < PB_GROUP_MAX; i++)
		{
			if(-1 != m_pPbData->PbGroups[i].index)
			{
				m_ComboGroup.AddString(m_pPbData->PbGroups[i].strGroupName);
			}			
		}
	}	
	UpdateData(false);	

	if(iInsertIndex == CONTACTDLG_TYPE_MODIFY)
	{
		int index;
		index=m_ComboGroup.FindStringExact(0,m_strGroup);
		m_ComboGroup.SetCurSel(index);
	}
	else if(iInsertIndex == CONTACTDLG_TYPE_ADD)
	{
		int index;
		index=m_ComboGroup.FindStringExact(0,m_strGroup);
		m_ComboGroup.SetCurSel(index);
	}
	else if(iInsertIndex == CONTACTDLG_TYPE_NORMAL) //view
	{
		int index;
		index=m_ComboGroup.FindStringExact(0,m_strGroup);
		m_ComboGroup.SetCurSel(index);
		GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MOBILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_LOADPIC)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);	
	}
	UpdateData(TRUE);

	m_EditMobile.m_bFlag  = true;
	m_EditHome.m_bFlag    = true;
	m_EditCompany.m_bFlag = true;

	//����ͼƬ�Ĵ洢��Ϣ,�Ƚ���Ҫ��ͼƬ������ָ��Ŀ¼��Ȼ�����ļ���ʾ�ڿؼ���
	CFile FileTemp;

	BOOL BRes = FileTemp.Open(m_strPicPath, CFile::modeRead);
	
	/*δѡ��ͼƬ��ͼƬ�Ѿ������ڵļ�¼�����Զ�ȡĬ��ͼƬ*/
	if ((m_strPicPath == _T(""))||!BRes)
	{
		//��ȡĬ��ͼƬ��·��
		m_strPicPath = GetPicDefPath();
	}
	if(BRes)
	{
		FileTemp.Close();	
	}
	if(!DisplayPicture(m_strPicPath))
	{
		
		TCHAR fPathTemp[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH, fPathTemp);
		CString strDefPath;
		strDefPath=fPathTemp;
		if(!CopyFile(strDefPath+_T("\\PIC\\default.jpg"),strDefPath+_T("\\default.jpg"),false))
		{
			//AfxMessageBox("Copy file error!"); //DEBUG��Ϣ
			CString strLoadErr;
			strLoadErr.LoadString(IDS_PB_ADDPICERR);
			AfxMessageBox(strLoadErr); 
			return FALSE;
		}
	//	CString strLoadErr;
	//	strLoadErr.LoadString(IDS_PB_ADDPICERR);
	//	AfxMessageBox(strLoadErr); 
	}	
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactDlg::OnCancel() 
{
    if(GetParent()->GetParent() != NULL)
        GetParent()->PostMessage(WM_ABSTRACT_NUM, 1);

    CBaseDialog::OnCancel();
    
}

BOOL CContactDlg::PreTranslateMessage(MSG* pMsg) 
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_RBUTTONDOWN)
    {
        return true;
    }
    return CBaseDialog::PreTranslateMessage(pMsg);
}

void CContactDlg::OnUpdateEditCompany() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	CString strCompanyTemp = m_strCompany;
	UpdateData();
	int nIndex = m_strCompany.Find('+',1);
	if ((1 <= nIndex)&&(1 < m_strCompany.GetLength()))
	{
		m_strCompany = strCompanyTemp;
		UpdateData(FALSE);
		if (nIndex == m_strCompany.GetLength())
		{
			m_EditCompany.SetSel(m_strCompany.GetLength(), m_strCompany.GetLength());//���������		
		}
		else
		{
			m_EditCompany.SetSel(nIndex, nIndex);		
		}
	}
}

void CContactDlg::OnUpdateEditMobile() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	CString strMobileTemp = m_strMobile;
	UpdateData();
	int nIndex = m_strMobile.Find('+',1);
	if ((1 <= nIndex)&&(1 < m_strMobile.GetLength()))
	{
		m_strMobile = strMobileTemp;
		UpdateData(FALSE);
		if (nIndex == m_strMobile.GetLength())
		{
			m_EditMobile.SetSel(m_strMobile.GetLength(), m_strMobile.GetLength());//���������		
		}
		else
		{
			m_EditMobile.SetSel(nIndex, nIndex);		
		}
	}
	// TODO: Add your control notification handler code here
	
}

void CContactDlg::OnUpdateEditHome() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	CString strHomeTemp = m_strHome;
	UpdateData();
	int nIndex = m_strHome.Find('+',1);
	if ((1 <= nIndex)&&(1 < m_strHome.GetLength()))
	{
		m_strHome = strHomeTemp;
		UpdateData(FALSE);
		if (nIndex == m_strHome.GetLength())
		{
			m_EditHome.SetSel(m_strHome.GetLength(), m_strHome.GetLength());//���������		
		}
		else
		{
			m_EditHome.SetSel(nIndex, nIndex);		
		}
	}
	// TODO: Add your control notification handler code here	
}


void CContactDlg::OnLoadPic()
{
	//CString 		strPathName;
	//CFileDialogEx dlg(TRUE);	 //Modifyed by Zhou Bin 2008.12.27
	CFileDialog dlg(TRUE);	
	TCHAR fDefault[MAX_PATH];
	
	//���ô��ļ���·��
	::GetCurrentDirectory(MAX_PATH, fDefault);
	CString strDefFolder;
	strDefFolder = fDefault;

	
		//��ʾ2000�µĴ��ļ��Ի���-------------
	//------�������Ŀǰ�Ĳ���ϵͳ�İ汾��Ϣ
// 	int nStructSize = 0;
// 	DWORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;
// 	dwVersion = GetVersion();
// 	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
// 	dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
// 	if(dwVersion < 0x80000000)
// 	{
// 		nStructSize = 88;   //2k,xp???????
// 	}
// 	else
// 	{
// 		nStructSize = 76;  //98??????
// 	}
// 	dlg.m_ofn.lStructSize = nStructSize;  //------????????????????
	
	if(dlg.DoModal() == IDOK)
	{
		//ͼƬ���º󱣴浱ǰ��ʾ��ͼƬ·��
		CString strPathName;
		strPathName	= dlg.GetPathName();
		if(strPathName.CompareNoCase(m_strPicPath))
		{
			bLoadPic = true;
			m_strPicOldPath = m_strPicPath;
			m_strPicPath = strPathName;
		}
		
		if(!DisplayPicture(m_strPicPath))
		{
			CString strLoadErr;
			strLoadErr.LoadString(IDS_PB_ADDPICERR);
			AfxMessageBox(strLoadErr); 
		}
	}
	else
	{
		return;
	}
	if(strDefFolder != _T(""))
	{
		::SetCurrentDirectory(strDefFolder);
	}
	else
	{
		AfxMessageBox(_T("Path error!")); //debug info
	}
}

/*�Ӹ�����·���н�ͼƬ��ʾ��IDC_CONTACT_PICS�ؼ���*/
bool CContactDlg::DisplayPicture(CString strPath)
{
	RECT rect;

	HWND  hWnd = ::GetActiveWindow();
	HDC hDC = ::GetDC(GetDlgItem(IDC_CONTACT_PICS)->GetSafeHwnd());        //Get the DC for the CPicture Box
	GetDlgItem(IDC_CONTACT_PICS)->GetClientRect(&rect);

	CPicture  pic;
	TCHAR szFile[MAX_PATH];
	
	wcscpy(szFile,strPath);
	//Try to load the Image..if not then we should return
	if(!pic.LoadPicture(szFile))
	{
		return false;
	}
	
	//Draw the image in the picture box
	//(could be updated to maintain aspect ratio)
	pic.DrawPicture(hDC, 0, 0, rect.right - rect.left ,rect.bottom - rect.top );
	::ReleaseDC(::GetDlgItem(hWnd, IDC_CONTACT_PICS), hDC);
	return true;
}

CString CContactDlg::GetPicDefPath() 
{
	CString strDefPath;
	TCHAR fPathTemp[MAX_PATH];

	::GetCurrentDirectory(MAX_PATH, fPathTemp);
	strDefPath = fPathTemp;
	strDefPath = strDefPath + _T("\\default.jpg");
	return strDefPath;
}

CString CContactDlg::GetPicNewPath() 
{
	TCHAR fPathTemp[MAX_PATH];
	CString strNewPath;
	
	::GetCurrentDirectory(MAX_PATH, fPathTemp);
	strNewPath = fPathTemp;
	strNewPath.Insert(strNewPath.GetLength() + 1,_T("\\pic\\"));
	
	CString strRes = GetSysTime();
	strNewPath = strNewPath + strRes;
	strNewPath.Insert(strNewPath.GetLength() + 1,_T(".jpg"));
	return strNewPath;
}

//11-09-2006 09:01:09
CString CContactDlg::GetSysTime() 
{
	CString strTime;
	COleDateTime dateTest;	
	dateTest = COleDateTime::GetCurrentTime();
	strTime = dateTest.Format(_T("%m-%d-%Y %H:%M:%S"));

	strTime.TrimLeft();
	strTime.TrimRight();
	int nPos = strTime.Find('-',0);
	strTime.Delete(nPos,1);
	nPos = strTime.Find('-',0);
	strTime.Delete(nPos,1);
	nPos = strTime.Find(' ',0);
	strTime.Delete(nPos,1);
	nPos = strTime.Find(':',0);
	strTime.Delete(nPos,1);
	nPos = strTime.Find(':',0);
	strTime.Delete(nPos,1);

	return strTime;
}

BOOL CContactDlg::CopyPicFile(CString strNowPath,CString strNewPath) 
{
	if(!CopyFile(strNowPath,strNewPath,false))
	{
		//AfxMessageBox("Copy file error!"); //DEBUG��Ϣ
		CString strLoadErr;
		strLoadErr.LoadString(IDS_PB_ADDPICERR);
		AfxMessageBox(strLoadErr); 
		return FALSE;
	}
	CFileStatus  filestatus;
	if(CFile::GetStatus(strNewPath, filestatus))
	{
		SetFileAttributes(strNewPath, FILE_ATTRIBUTE_ARCHIVE);
	}
	return TRUE;
}

HBRUSH CContactDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	
	// TODO: Change any attributes of the DC here
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);			//wyw_0408 modify
	// TODO: Return a different brush if the default is not desired
	//HDC hDC = ::GetDC(::GetDlgItem(GetSafeHwnd(),IDC_CONTACT_PICS));
	 switch(nCtlColor)
	 {   
		case CTLCOLOR_STATIC:   
		{
			if(pWnd->GetDlgCtrlID() == IDC_CONTACT_PICS)   
			{
				if(!DisplayPicture(m_strPicPath))
				{
					CString strLoadErr;
					strLoadErr.LoadString(IDS_PB_ADDPICERR);
					AfxMessageBox(strLoadErr); 
				}			
			}
			HGDIOBJ  Obj = GetStockObject(NULL_BRUSH);
			return(HBRUSH)Obj;
		}
		default:
			return hbr;
	  }  
}  

bool CContactDlg::CompareChar(const TCHAR* chSource)
{
	while(*chSource)
	{ 
		UINT nChar = *chSource;
		if((nChar >= '0' && nChar <= '9' || nChar == 8) || nChar == '*' 
			|| nChar == '#' || nChar == '+')
		{			
		}
		else
		{
			CString strTemp;
			strTemp.LoadString(IDS_PB_IMPORTTIP);
			AfxMessageBox(strTemp);
			return false;
		}
		
		chSource++;
	}
	return true;
	
} 
