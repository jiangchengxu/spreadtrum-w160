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
    //ɾ������ָ����ǵ����м�¼
	BOOL DelallRecordbyFlag(EnSmsType type, BYTE flag);

    //���δ��SMS����
	WORD GetUnreadSmsNum();

    //�༭SMS��¼
    BOOL EditSmsRecord(EnSmsType type, WORD nIndex, const StSmsRecord &record);

    //����SMS��¼
    BOOL AddSmsRecord(EnSmsType type, StSmsRecord record, BOOL bAutoReplace = TRUE);

    //���SMS���
    BOOL ClearSmsFlag(EnSmsType type, BYTE Clearflag, BOOL bAll = TRUE);
    DECLARE_SERIAL(CSmsData)
    CSmsData(const TCHAR *fileName);
    virtual ~CSmsData();

    //������������
    void LoadSmsData();

    //����ָ��λ�õ�һ����¼
    StSmsRecord ReadSmsRecord(EnSmsType type, WORD nIndex);

    //���һ����¼
    BOOL AddSmsRecord(EnSmsType type, EnSmsState nState, LPCTSTR pNumber,
                      const COleDateTime &time, LPCTSTR pContent, LPCTSTR pSCNum =_T(""),
                      BYTE flag = 0, USHORT nRefCnt = 0, BYTE nSeqCnt = 0, BYTE nTotalCnt = 0,
                      BOOL bAutoReplace = TRUE);
    
    //ɾ��һ����¼
    BOOL DeleteSmsRecord(EnSmsType type, WORD nIndex);

    //ɾ�����м�¼
    BOOL DelallSmsRecord(EnSmsType type);

    //���ص�ǰ��¼��Ŀ
    WORD GetSmsNum(EnSmsType type) const;

    //�жϵ�ǰ��¼��ĿΪ��
    BOOL IsEmpty(EnSmsType type) const;

    //�жϵ�ǰ��¼��Ŀ����
    BOOL IsFull(EnSmsType type) const;
    WORD m_SmsNum[SMS_TYPE_ALL];//added by lly 2007.12.28
protected:
#ifdef FEATURE_OBJECT_SERIAL
    //���ݴ��л�
    void Serialize(CArchive &ar);
#endif
    
    //���ļ��ж�ȡ����
    BOOL ReadDataFromFile();

    //������д���ļ�
    BOOL WriteDataToFile();

    //�����������
    void ResetSmsData();

private:
#ifdef FEATURE_TEXT_MODE
    void TranslateRecordToString(const StSmsRecord &record, char *str);
    void TranslateStringToRecord(const char *str, StSmsRecord *record);
#endif

private:
    //��¼��ʱɾ�������һ����¼
	void DeleteWhenFull(EnSmsType type);
    CString m_strSmsFileName;                               //����Ϣ�ļ���
    //WORD m_SmsNum[SMS_TYPE_ALL];                           //��ǰ��¼��Ŀ
    StSmsRecord m_SmsRecord[SMS_TYPE_ALL][SMS_RECORD_MAX]; //��ǰ��¼��Ŀ
};

#endif // !defined(AFX_SMSDATA_H__703C1B3D_B269_44DC_B9AE_B2C35BBCE177__INCLUDED_)
