#ifndef _AFX_FEATURECUSTOM_H_
#define _AFX_FEATURECUSTOM_H_

/* AT响应结束符开关 */
//#define FEATURE_ATAMOI

/* USB拔除自动关机特性开关 */
#define FEATURE_AUTOQUIT

/* 程序启动和关闭时自动设置RF电源特性开关 */
#define FEATURE_AUTOSETRF

/* SMS使用PDU模式特性开关 */
//#define FEATURE_SMS_PDUMODE

/* 支持Concatenated Short Messages特性开关 */
#define FEATURE_SMS_CONCATENATE

//不插卡运行测试UI特性
//#define FEATURE_UI_TEST

//GCF测试时回复SMS使用原消息的消息中心号码
//#define FEATURE_GCF_SMSREPLY

//GCF测试时通话中可以切换到其他菜单
//#define FEATURE_GCF_CALLCANSWITCH

//南非版本 (基于Telit)
//#define FEATURE_VERSION_NASHUA

// @@
//灰色系界面，目前用于NASHUA和Itelco
#define FEATURE_UICOLOR_GREY

//意大利Telit版本 (基于Itelco+TIM)
//#define FEATURE_VERSION_ITELCO

//马来西亚H01项目(BlueCube 9508)
//#define FEATURE_VERSION_BLUECUBE

//巴西9508A项目(多个运营商合作，分辩不同的sim卡显示对应的LOGO)
//#define FEATURE_VERSION_BRAZIL

//摩洛哥项目(Telcel 9508)
//#define FEATURE_VERSION_TELCEL


#define FEATURE_NDIS_SUPPORT



//#define FEATURE_ATTEST_SUPPORT


//机卡一体版本
//#define FEATURE_VERSION_NOSIM

//无菜单版本，目前适用于伊拉克阿拉伯版
#define FEATURE_VERSION_NOMENU

//语言自右向左显示，目前适用于伊拉克阿拉伯版
//#define FEATURE_LANGUAGE_RTOL

//主界面button标题为空，目前适用于TATA和Odutel
//#define FEATURE_BUTTON_NOCAPTION

//短信模版
//#define FEATURE_BUTTON_TEMPLATE

//紧急号码
#define FEATURE_EMERGENCY_NUMBER


//语音加密
//#define FEATURE_CALL_PRIVACY

//Haier 适配
#ifndef FEATURE_HAIER
#define FEATURE_HAIER

//电话本适配
#define FEATURE_HAIER_PHONEBOOK
#define FEATURE_HAIER_PINMANAGE
#define FEATURE_HAIER_DSI
#define FEATURE_HAIER_ADAPTER
#define FEATURE_HAIER_SMS
#define FEATURE_HAIER_CM
#define FEATURE_HAIER_INTERNET
#endif

#endif //#ifndef _AFX_FEATURECUSTOM_H_