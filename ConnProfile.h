// ConnProfile.h: interface for the CConnProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNPROFILE_H__86D9E6DA_ABE5_4FAF_A705_F85F4ED9F4CB__INCLUDED_)
#define AFX_CONNPROFILE_H__86D9E6DA_ABE5_4FAF_A705_F85F4ED9F4CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConnProfile : public CObject
{
public:
    WORD GetDefaultConnProfileIndex();
    BOOL IsExistent(LPCTSTR pEntryName);
    BOOL EditConnProfile(WORD nIndex, const StConnProfile &profile);
    BOOL AddConnProfile(const StConnProfile &profile);
    DECLARE_SERIAL(CConnProfile)
    CConnProfile();
    CConnProfile(CString strFileName);
    virtual ~CConnProfile();

    //������������
    void LoadConnProfile();

    //����ָ��λ�õ�һ����¼
    StConnProfile ReadConnProfile(WORD nIndex);

    //ɾ��һ����¼
    BOOL DeleteConnProfile(WORD nIndex);

    //ɾ�����м�¼
    BOOL DelallConnProfile();

    //���ص�ǰ��¼��Ŀ
    WORD GetConnNum() const;

    //���ص�ǰNdis��¼��Ŀ
    WORD GetNdisConnNum() const;

    ////���ص�ǰRas��¼��Ŀ
    WORD GetRasConnNum() const;

    //�жϵ�ǰ��¼��ĿΪ��
    BOOL IsEmpty() const;

    //�жϵ�ǰ��¼��Ŀ����
    BOOL IsFull() const;

protected:
    //���ļ��ж�ȡ����
    BOOL ReadDataFromFile();

    //������д���ļ�
    BOOL WriteDataToFile();

    //���ܼ���
    CString DeorEncrypt(const TCHAR* strData);

private:
    CString m_strConnProfileFileName;                    //�����ļ���
    WORD m_ConnNum;                                      //��ǰ������Ŀ
    StConnProfile m_ConnProfile[CONNECTION_PROFILE_MAX]; //��ǰ������Ŀ
};

#endif // !defined(AFX_CONNPROFILE_H__86D9E6DA_ABE5_4FAF_A705_F85F4ED9F4CB__INCLUDED_)
