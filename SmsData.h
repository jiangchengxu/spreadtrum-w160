// SmsData.h: interface for the CSmsData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMSDATA_H__703C1B3D_B269_44DC_B9AE_B2C35BBCE177__INCLUDED_)
#define AFX_SMSDATA_H__703C1B3D_B269_44DC_B9AE_B2C35BBCE177__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSmsData : public CObject
{
public:
	CSmsData();
    //删除含有指定标记的所有记录
	BOOL DelallRecordbyFlag(EnSmsType type, BYTE flag);

    //获得未读SMS条数
	WORD GetUnreadSmsNum();

    //编辑SMS记录
    BOOL EditSmsRecord(EnSmsType type, WORD nIndex, const StSmsRecord &record);

    //增加SMS记录
    BOOL AddSmsRecord(EnSmsType type, StSmsRecord record, BOOL bAutoReplace = TRUE);

    //清除SMS标记
    BOOL ClearSmsFlag(EnSmsType type, BYTE Clearflag, BOOL bAll = TRUE);
    DECLARE_SERIAL(CSmsData)
    CSmsData(const TCHAR *fileName);
    virtual ~CSmsData();

    //加载所有数据
    void LoadSmsData();

    //返回指定位置的一条记录
    StSmsRecord ReadSmsRecord(EnSmsType type, WORD nIndex);

    //添加一条记录
    BOOL AddSmsRecord(EnSmsType type, EnSmsState nState, LPCTSTR pNumber,
                      const COleDateTime &time, LPCTSTR pContent, LPCTSTR pSCNum =_T(""),
                      BYTE flag = 0, USHORT nRefCnt = 0, BYTE nSeqCnt = 0, BYTE nTotalCnt = 0,
                      BOOL bAutoReplace = TRUE);
    
    //删除一条记录
    BOOL DeleteSmsRecord(EnSmsType type, WORD nIndex);

    //删除所有记录
    BOOL DelallSmsRecord(EnSmsType type);

    //返回当前记录数目
    WORD GetSmsNum(EnSmsType type) const;

    //判断当前记录数目为空
    BOOL IsEmpty(EnSmsType type) const;

    //判断当前记录数目已满
    BOOL IsFull(EnSmsType type) const;
    WORD m_SmsNum[SMS_TYPE_ALL];//added by lly 2007.12.28
protected:
#ifdef FEATURE_OBJECT_SERIAL
    //数据串行化
    void Serialize(CArchive &ar);
#endif
    
    //从文件中读取数据
    BOOL ReadDataFromFile();

    //将数据写入文件
    BOOL WriteDataToFile();

    //清除所有数据
    void ResetSmsData();

private:
#ifdef FEATURE_TEXT_MODE
    void TranslateRecordToString(const StSmsRecord &record, char *str);
    void TranslateStringToRecord(const char *str, StSmsRecord *record);
#endif

private:
    //记录满时删除最早的一条记录
	void DeleteWhenFull(EnSmsType type);
    CString m_strSmsFileName;                               //短信息文件名
    //WORD m_SmsNum[SMS_TYPE_ALL];                           //当前记录数目
    StSmsRecord m_SmsRecord[SMS_TYPE_ALL][SMS_RECORD_MAX]; //当前记录条目
};

#endif // !defined(AFX_SMSDATA_H__703C1B3D_B269_44DC_B9AE_B2C35BBCE177__INCLUDED_)
