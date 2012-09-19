#ifndef USERDEFID_H
#define USERDEFID_H

#include "Constants.h"
//自定义消息ID
typedef enum
{
    WM_CUSBASELINE = WM_USER,    /*not used*/
    WM_HIST_MENUITEM_CLICK,
    WM_LOG_MENUITEM_CLICK,
    UM_TRAYNOTIFY,
    IDT_TABED_SWITCHME,                        
    IDT_CALL_DIAL,                            
    IDT_TABED_SWITCHTO,                            
	WM_SMS_UPSMSDLGTREE,//add by liub
    WM_SMS_MENUITEM_CLICK,                        
    WM_SMS_SAVE_MSG,                            
	WM_SMS_SAVE_MSG_ANSWER_INCALL,//add by liub
	WM_SMS_READ_FLASHSMS,//add by liub for CFT
    WM_SMS_READ_MSG, 
    WM_SMS_VIEW_MSG, 
    WM_SMS_SEND_PROC,
    WM_SMS_TRANSFER_PROC,
	WM_SMS_OPERATE_PROC, 
    WM_READSMS_UPDATE,
    WM_HIST_SAVE_MSG,
	WM_HIST_SAVE_MSG_CALLDLG,//add by liub
    WM_CHANGE_SKIN,                            
    WM_SEL_CONTACT,                            
    WM_SEL_CONTACT_INDEX,                    
    WM_ADD_CONTACT,                            
    WM_SEL_POSITION,                            
    WM_INTERNET_INITCONNPARAM,                
    WM_ABSTRACT_NUM,
    WM_PB_ReDrawListCtrl,
    WM_CREATE_PROGRESS,
	WM_PB_ProgressOpen,
	WM_Wait_ProgressSet,
	WM_ICON_UPDATE,
    WM_SETUP_UPDATE_POWERDLG,
    WM_POPDLG_DESTROY,
    WM_CALL_UPDATE_ITEM, 
    WM_SMS_FULL_NOTIFY,
	WM_SETUP_UPDATE_GENERALDLG,	//Update General dlg
	WM_SETUP_UPDATE_NWSELDLG,	//Update NWSel dlg
    WM_SMS_INIT_NOTIFY, 
	WM_SETUP_UPDATE_INTERNETDLG, 
    WM_DIAGNOSTICS_NOTIFY, 
    WM_CLICK_INTERNET_BUTTON, 
	WM_WINDOWS_MAX,     //windows max
	WM_WINDOWS_RESTORE,  //windows restore
	WM_DELETE_DCRECORD,
	WM_SEARCH_CONTACT,
	WM_SEARCH_GETTOTALCOUNT, 
	WM_HANDSET_ANSWER_INCOMINGCALL,//add by liub for HeadSet answer incomingcall
	// @@@@@@
    //WM_INTERNET_CONN_TIP, 
	WM_INIT_SMSANDPB,
	WM_UPDATE_CONNECT_STATE,
	WM_REFRESH_GROUP,
	WM_PB_REDRAWTREECTRL,
	WM_USAGE_RESETNETSTATISTICS,
	WM_SETUP_UPDATE_NWSELDLG_MODE,//Update NWSelMode dlg//liub_add
	WM_PB_UPDATECONTACTNUMBER,//更新群组中联系人数量
	WM_CREATE_INTERNET,
	WM_MODEM_IN_OUT,
	WM_NO_MODEM_FOR_SMS,//未插数据卡时SMS模块的处理方式
	WM_UPDATA_SENDSMS_LMS,//flexi发送长短信时刷新进度条的消息
} EnUserMsgType;

//自定义定时器ID
typedef enum
{
        TIMER_ID_ATRESP_TIMEOUT = 1,
        IDT_CHV0_TIMEOUT,        
        IDT_RING_TIMEOUT,   
        IDT_SMS_TIMEOUT,
        IDT_QREAD_TIMEOUT,//added by lly
        IDT_QCPMS_TIMEOUT,        
        IDT_QCMGR_TIMEOUT,        
        IDT_QCMGW_TIMEOUT,        
        IDT_QCMGD_TIMEOUT,        
        IDT_QCMGS_TIMEOUT,    
        IDT_QCMTI_TIMEOUT,
        IDT_QCSMP_TIMEOUT,
		IDT_QHMSGL_TIMEOUT,//add by liub for CDMA2000
		IDT_QHMSGP_TIMEOUT,//add by liub for CDMA2000
		IDT_QHCSMP_TIMEOUT,//add by liub for CDMA2000
		IDT_CVMR_TIMEOUT,//add by liub for TATA voicemail
		IDT_CVMD_TIMEOUT,//add by liub
        IDT_QCSCA_TIMEOUT, 
        IDT_QCMMS_TIMEOUT, //add by jcx for WCDMA
		IDT_PHMOD_TIMEROUT,
        TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE, 
        TIMER_ID_ATRESP_TIMEOUT_CPBR_CHECK_TOTAL, 
        TIMER_ID_ATRESP_TIMEOUT_CPBR_READ_ALL,
        TIMER_ID_ATRESP_TIMEOUT_CPBF, 
        TIMER_ID_ATRESP_TIMEOUT_CPBW,
        IDT_TIMER_FLASHDIALOG,                        
        IDT_TIMER_MOVEDIALOG,                    
        IDT_TIMER_TIP,                            
        IDT_TIMER_TIP_DOWN,                        
        IDT_TICK_TIMER,                                
        IDT_TIMER_DATASERVICEDURATION,
        IDT_USB_CONN_DETECT,
        TIMER_ID_ATRESP_TIMEOUT_CPBR_SET_STORE,
        TIMER_ID_ATRESP_TIMEOUT_CPBW_WRITE_RECORD,
		TIMER_ID_ATRESP_TIMEOUT_CPBW_DELETE_RECORD,
        //added by lgz 2006-06-19
        //添加电源设置超时定时器ID
        IDT_CFUN_TIMEOUT,
        IDT_TIMER_DATASERVICECONNECT,
		//CGDCONT
		IDT_CGDCONT_TIMEOUT,
		//CSCA
		IDT_CSCA_TIMEOUT,
		//COPS
		IDT_COPS_TIMEOUT,
		//PHPREF
		IDT_PHPREF_TIMEOUT,
		IDT_RCVCONCSMS_TIMEOUT_BEGIN,
		IDT_RCVCONCSMS_TIMEOUT_END = (IDT_RCVCONCSMS_TIMEOUT_BEGIN + SMS_RCVCONCBUF_MAX),
		IDT_COPS_MSG,
		IDT_CPMS_TIMEOUT, 
        IDT_CNMI_TIMEOUT, 
        IDT_ATD_TIMEOUT, 
        IDT_TIMER_DATANDISSERVICEDURATION,
		IDT_TIMER_COPSINIT,//查询cops运营商名称 
		IDT_TIMER_VOICEMAIL,//add by liub for voicemail
		IDT_TIMER_CONNECTIMEOUT,
} EnTimerType;
#endif