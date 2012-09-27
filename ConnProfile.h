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

    //加载所有数据
    void LoadConnProfile();

    //返回指定位置的一条记录
    StConnProfile ReadConnProfile(WORD nIndex);

    //删除一条记录
    BOOL DeleteConnProfile(WORD nIndex);

    //删除所有记录
    BOOL DelallConnProfile();

    //返回当前记录数目
    WORD GetConnNum() const;

    //返回当前Ndis记录数目
    WORD GetNdisConnNum() const;

    ////返回当前Ras记录数目
    WORD GetRasConnNum() const;

    //判断当前记录数目为空
    BOOL IsEmpty() const;

    //判断当前记录数目已满
    BOOL IsFull() const;

protected:
    //从文件中读取数据
    BOOL ReadDataFromFile();

    //将数据写入文件
    BOOL WriteDataToFile();

    //解密加密
    CString DeorEncrypt(const TCHAR* strData);

private:
    CString m_strConnProfileFileName;                    //连接文件名
    WORD m_ConnNum;                                      //当前连接数目
    StConnProfile m_ConnProfile[CONNECTION_PROFILE_MAX]; //当前连接条目
};

#endif // !defined(AFX_CONNPROFILE_H__86D9E6DA_ABE5_4FAF_A705_F85F4ED9F4CB__INCLUDED_)
