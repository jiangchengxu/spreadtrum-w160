#ifndef _AFX_FEATURECUSTOM_H_
#define _AFX_FEATURECUSTOM_H_

/* AT��Ӧ���������� */
//#define FEATURE_ATAMOI

/* USB�γ��Զ��ػ����Կ��� */
#define FEATURE_AUTOQUIT

/* ���������͹ر�ʱ�Զ�����RF��Դ���Կ��� */
#define FEATURE_AUTOSETRF

/* SMSʹ��PDUģʽ���Կ��� */
//#define FEATURE_SMS_PDUMODE

/* ֧��Concatenated Short Messages���Կ��� */
#define FEATURE_SMS_CONCATENATE

//���忨���в���UI����
//#define FEATURE_UI_TEST

//GCF����ʱ�ظ�SMSʹ��ԭ��Ϣ����Ϣ���ĺ���
//#define FEATURE_GCF_SMSREPLY

//GCF����ʱͨ���п����л��������˵�
//#define FEATURE_GCF_CALLCANSWITCH

//�Ϸǰ汾 (����Telit)
//#define FEATURE_VERSION_NASHUA

// @@
//��ɫϵ���棬Ŀǰ����NASHUA��Itelco
#define FEATURE_UICOLOR_GREY

//�����Telit�汾 (����Itelco+TIM)
//#define FEATURE_VERSION_ITELCO

//��������H01��Ŀ(BlueCube 9508)
//#define FEATURE_VERSION_BLUECUBE

//����9508A��Ŀ(�����Ӫ�̺������ֱ粻ͬ��sim����ʾ��Ӧ��LOGO)
//#define FEATURE_VERSION_BRAZIL

//Ħ�����Ŀ(Telcel 9508)
//#define FEATURE_VERSION_TELCEL


#define FEATURE_NDIS_SUPPORT



//#define FEATURE_ATTEST_SUPPORT


//����һ��汾
//#define FEATURE_VERSION_NOSIM

//�޲˵��汾��Ŀǰ�����������˰�������
#define FEATURE_VERSION_NOMENU

//��������������ʾ��Ŀǰ�����������˰�������
//#define FEATURE_LANGUAGE_RTOL

//������button����Ϊ�գ�Ŀǰ������TATA��Odutel
//#define FEATURE_BUTTON_NOCAPTION

//����ģ��
//#define FEATURE_BUTTON_TEMPLATE

//��������
#define FEATURE_EMERGENCY_NUMBER


//��������
//#define FEATURE_CALL_PRIVACY

//Haier ����
#ifndef FEATURE_HAIER
#define FEATURE_HAIER

//�绰������
#define FEATURE_HAIER_PHONEBOOK
#define FEATURE_HAIER_PINMANAGE
#define FEATURE_HAIER_DSI
#define FEATURE_HAIER_ADAPTER
#define FEATURE_HAIER_SMS
#define FEATURE_HAIER_CM
#define FEATURE_HAIER_INTERNET
#endif

#endif //#ifndef _AFX_FEATURECUSTOM_H_