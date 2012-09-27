#include "stdafx.h"
#include "Ds.h"
#include "HSDPADlg.h"
#include "HSDPA.h"

#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

const EnDsatResMode gc_dsatmode = DSAT_MODE_STRING;

const char gc_dsatResCodeTbl[DSAT_MAX][DSAT_MODE_MAX][30] = {
    "0", "OK",
    "4", "ERROR",
    "2", "RING",
    "3", "NO CARRIER",
    "6", "NO DIALTONE",
    "+CME ERROR:", "+CME ERROR:",
    "+CMS ERROR:", "+CMS ERROR:",
    "+CMS ERROR: 500", "+CMS ERROR: 500",
    "+CMTI: ", "+CMTI: ",
    "+CMT: ", "+CMT: ",
    "+RVMFB: ", "+RVMFB: ", //add by liub
    "+RVMFBUPDATE: ", "+RVMFBUPDATE: ", //TATA
    "+RSSI: ", "+RSSI: ",
    "*SPREADY: ", "*SPREADY: ",
    "+CLIP: ", "+CLIP: ",
    "Network status: ", "Network status: ",
    "Network Service: ", "Network Service: ",
    "Roaming status: ", "Roaming status: ",
    "HANDSET_NOTIFY: ", "HANDSET_NOTIFY: ",
    "SIM_READY: ", "SIM_READY: ",
    "Current PLMN: ", "Current PLMN: ",
    "Srv status: ", "Srv status: ",
    "+CDS: ", "+CDS: ",
    "+CUSD: ", "+CUSD: ",
    "+QCPDSI:", "+QCPDSI:",
    "+HVPRIV", "+HVPRIV",
    "+SIDLOCK: ", "+SIDLOCK: ",
    "+SIND: ", "+SIND: ",
    "^MODE: ", "^MODE: ",
    "^SYSINFO: ", "^SYSINFO: ",
    "+CREG: ", "+CREG: ",
    "+CGREG: ", "+CGREG: ",
    "+ECIND: ", "+ECIND: ",
};

static StAtResp g_AtRespArr[ATRESP_MAX];
static HANDLE g_DsEventArr[DSEVENT_ARRNUM];

HANDLE g_AtRespEvent; //用于DS和串口字符传送同步
HANDLE g_EndEvent;  //结束DS任务

//用于APP与DS运行时注册普通命令响应同步
HANDLE g_AtRegEvt;

//用于APP与DS启动时注册主动消息响应同步
HANDLE g_AppRegEvt;

//用于APP普通命令超时处理
HANDLE g_AtTimeout;

//解析结果字符串缓冲
static BYTE g_DsatStrArr[DSAT_STRING_ROW][DSAT_STRING_COL];
//解析结果字符串个数
static WORD g_DsatStrNum;
//解析结果指针
static BYTE *g_DsatPtr = &g_DsatStrArr[0][0];
//当前解析状态
static EnDsatState g_DsatState = STATE_START;
//解析得到的结果码
static EnDsatResCode g_DsatResCode = DSAT_MAX;


HANDLE g_BGPassEvt;
HANDLE g_BGReadNewSmsEvt;
HANDLE g_BGEvtArr[BGEVT_ARRNUM];

StAtResp BGSmsResp;
StAtResp BGCallResp;
StAtResp BGClipResp;

//解析结果字符串缓冲
static BYTE g_BGSmsStrArr[BG_STRING_ROW][BG_STRING_COL];
static BYTE g_BGCallStrArr[BG_STRING_ROW][BG_STRING_COL];
static BYTE g_BGClipStrArr[BG_STRING_ROW][BG_STRING_COL];

//解析结果字符串个数
static WORD g_BGSmsStrNum;
static WORD g_BGCallStrNum;
static WORD g_BGClipStrNum;

//短消息CMTI通知缓存队列
static StBGSmsQueue g_BGSmsQueue;

static const uinetwk_network_info_s_type uinetwk_network_table[] = {
    {001 , 1 , UI_NETWK_TYPE_GSM_900 , "Test Net 001 01" , "Test PLMN 1-1"},
    {001 , 2 , UI_NETWK_TYPE_GSM_900 , "Test Net 001 02" , "Test PLMN 1-2"},
    {002 , 1 , UI_NETWK_TYPE_GSM_900 , "Test2-1" , "Test PLMN 2-1"},
    {202 , 1 , UI_NETWK_TYPE_DCS_1800 , "C-OTE" , "COSMOTE"},
    {202 , 5 , UI_NETWK_TYPE_GSM_900 , "vodafone GR" , "Vodafone"},
    {202 , 9 , UI_NETWK_TYPE_DCS_1800 , "Q-TELCOM" , "Info Quest - Commercial & Industrial SA"},
    {202 , 10 , UI_NETWK_TYPE_GSM_900 , "TIM" , "STET HELLAS"},
    {204 , 4 , UI_NETWK_TYPE_GSM_900 , "voda NL" , "Vodafone Libertel N.V"},
    {204 , 4 , UI_NETWK_TYPE_DCS_1800 , "voda NL" , "Vodafone Libertel N.V"},
    {204 , 8 , UI_NETWK_TYPE_GSM_900 , "NL KPN" , "KPN Mobile The Netherlands BV"},
    {204 , 8 , UI_NETWK_TYPE_DCS_1800 , "NL KPN" , "KPN Mobile The Netherlands BV"},
    {204 , 12 , UI_NETWK_TYPE_DCS_1800 , "NL Tlfrt" , "Telfort Mobiel B.V."},
    {204 , 16 , UI_NETWK_TYPE_DCS_1800 , "T-Mobile NL" , "Ben Netherlands B.V"},
    {204 , 20 , UI_NETWK_TYPE_DCS_1800 , "Orange" , "Dutchtone N.V"},
    {206 , 1 , UI_NETWK_TYPE_GSM_900 , "PROXI" , "Belgacom Mobile"},
    {206 , 1 , UI_NETWK_TYPE_DCS_1800 , "PROXI" , "Belgacom Mobile"},
    {206 , 10 , UI_NETWK_TYPE_GSM_900 , "mobil*" , "Mobistar S.A."},
    {206 , 20 , UI_NETWK_TYPE_DCS_1800 , "BASE" , "KPN Orange Belgium NV/SA"},
    {208 , 1 , UI_NETWK_TYPE_GSM_900 , "Orange F" , "Orange France"},
    {208 , 1 , UI_NETWK_TYPE_DCS_1800 , "Orange F" , "Orange France"},
    {208 , 10 , UI_NETWK_TYPE_GSM_900 , "SFR" , "SFR"},
    {208 , 10 , UI_NETWK_TYPE_DCS_1800 , "SFR" , "SFR"},
    {208 , 20 , UI_NETWK_TYPE_GSM_900 , "BYTEL" , "Bouygues Telecom"},
    {208 , 20 , UI_NETWK_TYPE_DCS_1800 , "BYTEL" , "Bouygues Telecom"},
    {213 , 3 , UI_NETWK_TYPE_GSM_900 , "M-AND" , "Mobiland"},
    {214 , 1 , UI_NETWK_TYPE_GSM_900 , "voda ES" , "Airtel Movil S.A"},
    {214 , 1 , UI_NETWK_TYPE_DCS_1800 , "voda ES" , "Airtel Movil S.A"},
    {214 , 3 , UI_NETWK_TYPE_GSM_900 , "AMENA" , "Retevision Movil S.A"},
    {214 , 3 , UI_NETWK_TYPE_DCS_1800 , "AMENA" , "Retevision Movil S.A"},
    {214 , 4 , UI_NETWK_TYPE_UMTS , "Xfera" , "Xfera Moviles SA"},
    {214 , 7 , UI_NETWK_TYPE_GSM_900 , "MSTAR" , "Telefonica Moviles"},
    {214 , 7 , UI_NETWK_TYPE_DCS_1800 , "MSTAR" , "Telefonica Moviles"},
    {216 , 1 , UI_NETWK_TYPE_GSM_900 , "PANNON" , "Pannon GSM Telecoms Plc"},
    {216 , 1 , UI_NETWK_TYPE_DCS_1800 , "PANNON" , "Pannon GSM Telecoms Plc"},
    {216 , 30 , UI_NETWK_TYPE_GSM_900 , "TMO H" , "Westel Mobile Telecommunications Company Ltd"},
    {216 , 30 , UI_NETWK_TYPE_DCS_1800 , "TMO H" , "Westel Mobile Telecommunications Company Ltd"},
    {216 , 70 , UI_NETWK_TYPE_GSM_900 , "vodafone HU" , "V.R.A.M Telecommunication Co. Ltd"},
    {216 , 70 , UI_NETWK_TYPE_DCS_1800 , "vodafone HU" , "V.R.A.M Telecommunication Co. Ltd"},
    {218 , 3 , UI_NETWK_TYPE_GSM_900 , "BA-ERONET" , "Eronet Mobile Communications Ltd"},
    {218 , 5 , UI_NETWK_TYPE_GSM_900 , "MOBI'S" , "JODP for Telecomms of Republic Srpska"},
    {218 , 90 , UI_NETWK_TYPE_GSM_900 , "GSMBIH" , "PE PTT BIH"},
    {219 , 1 , UI_NETWK_TYPE_GSM_900 , "T-Mobile HR" , "Croatian Telecom"},
    {219 , 10 , UI_NETWK_TYPE_GSM_900 , "HR VIP" , "Vip-NET GSM d.o.o."},
    {220 , 1 , UI_NETWK_TYPE_GSM_900 , "MOBTEL" , "MOBTEL"},
    {220 , 2 , UI_NETWK_TYPE_GSM_900 , "ProMonte" , "ProMonte GSM"},
    {220 , 3 , UI_NETWK_TYPE_GSM_900 , "SCGTS" , "Telekom Srbija a.d."},
    {220 , 4 , UI_NETWK_TYPE_GSM_900 , "MONET" , "Monet D.O.O"},
    {222 , 1 , UI_NETWK_TYPE_GSM_900 , "TIM" , "Telecom Italia Mobile"},
    {222 , 1 , UI_NETWK_TYPE_DCS_1800 , "TIM" , "Telecom Italia Mobile"},
    {222 , 10 , UI_NETWK_TYPE_GSM_900 , "vodafone IT" , "Omnitel Pronto Italia"},
    {222 , 10 , UI_NETWK_TYPE_DCS_1800 , "vodafone IT" , "Omnitel Pronto Italia"},
    {222 , 88 , UI_NETWK_TYPE_GSM_900 , "WIND" , "Wind Telecomunicazioni SpA"},
    {222 , 88 , UI_NETWK_TYPE_DCS_1800 , "WIND" , "Wind Telecomunicazioni SpA"},
    {222 , 98 , UI_NETWK_TYPE_DCS_1800 , "IT Blu" , "Blu SpA"},
    {222 , 99 , UI_NETWK_TYPE_DCS_1800 , "3ITA" , "3ITA"},
    {226 , 1 , UI_NETWK_TYPE_GSM_900 , "CONNEX" , "MobiFon S.A"},
    {226 , 3 , UI_NETWK_TYPE_GSM_900 , "Cosmorom" , "Cosmorom"},
    {226 , 3 , UI_NETWK_TYPE_DCS_1800 , "Cosmorom" , "Cosmorom"},
    {226 , 10 , UI_NETWK_TYPE_GSM_900 , "RO ORANGE" , "Orange Romania SA"},
    {228 , 1 , UI_NETWK_TYPE_GSM_900 , "Swisscom" , "Swisscom Mobile Ltd"},
    {228 , 1 , UI_NETWK_TYPE_DCS_1800 , "Swisscom" , "Swisscom Mobile Ltd"},
    {228 , 2 , UI_NETWK_TYPE_GSM_900 , "sunrise" , "TDC Switzerland AG"},
    {228 , 2 , UI_NETWK_TYPE_DCS_1800 , "sunrise" , "TDC Switzerland AG"},
    {228 , 3 , UI_NETWK_TYPE_DCS_1800 , "Orange" , "Orange Communications S.A"},
    {230 , 1 , UI_NETWK_TYPE_GSM_900 , "TMO CZ" , "RadioMobil"},
    {230 , 2 , UI_NETWK_TYPE_GSM_900 , "ET - CZ" , "EuroTel Praha Spol. sro"},
    {230 , 3 , UI_NETWK_TYPE_GSM_900 , "OSKAR" , "Cesky Mobil a.s"},
    {230 , 3 , UI_NETWK_TYPE_DCS_1800 , "OSKAR" , "Cesky Mobil a.s"},
    {231 , 1 , UI_NETWK_TYPE_GSM_900 , "Orange" , "Globtel GSM a.s"},
    {231 , 1 , UI_NETWK_TYPE_DCS_1800 , "Orange" , "Globtel GSM a.s"},
    {231 , 2 , UI_NETWK_TYPE_GSM_900 , "ET-SK" , "EuroTel Bratislava a.s"},
    {231 , 2 , UI_NETWK_TYPE_DCS_1800 , "ET-SK" , "EuroTel Bratislava a.s"},
    {232 , 1 , UI_NETWK_TYPE_GSM_900 , "A1" , "MobilKom Austria AG"},
    {232 , 1 , UI_NETWK_TYPE_DCS_1800 , "A1" , "MobilKom Austria AG"},
    {232 , 3 , UI_NETWK_TYPE_GSM_900 , "TMO A" , "max.mobil. Telekommunikation Service GmbH"},
    {232 , 3 , UI_NETWK_TYPE_DCS_1800 , "TMO A" , "max.mobil. Telekommunikation Service GmbH"},
    {232 , 5 , UI_NETWK_TYPE_DCS_1800 , "one" , "Connect Austria"},
    {232 , 7 , UI_NETWK_TYPE_DCS_1800 , "telering" , "Tele.ring Telekom Service GmbH & Co KG"},
    {232 , 10 , UI_NETWK_TYPE_UMTS , "3AT" , " Hutchison 3G Austria GmbH"},
    {234 , 10 , UI_NETWK_TYPE_GSM_900 , "O2 -UK" , "BTCellnet"},
    {234 , 10 , UI_NETWK_TYPE_DCS_1800 , "O2 -UK" , "BTCellnet"},
    {234 , 15 , UI_NETWK_TYPE_GSM_900 , "Voda UK" , "Vodafone Ltd"},
    {234 , 15 , UI_NETWK_TYPE_DCS_1800 , "Voda UK" , "Vodafone Ltd"},
    {234 , 30 , UI_NETWK_TYPE_DCS_1800 , "TMO UK" , "One 2 One Personal Communications Limited"},
    {234 , 33 , UI_NETWK_TYPE_DCS_1800 , "Orange" , "Orange PCS Ltd"},
    {234 , 50 , UI_NETWK_TYPE_GSM_900 , "JT-Wave" , "Jersey Telecoms"},
    {234 , 50 , UI_NETWK_TYPE_GSM_900 , "JT-Wave" , "Jersey Telecommunications"},
    {234 , 55 , UI_NETWK_TYPE_GSM_900 , "C&W" , "Guernsey Telecoms Limited"},
    {234 , 55 , UI_NETWK_TYPE_GSM_900 , "C&W" , "Guernsey Telecoms"},
    {234 , 58 , UI_NETWK_TYPE_GSM_900 , "Manx Pronto" , "Manx Telecom"},
    {234 , 58 , UI_NETWK_TYPE_GSM_900 , "Manx Pronto" , "Manx Telecom Pronto GSM"},
    {238 , 1 , UI_NETWK_TYPE_GSM_900 , "TDC MOBIL" , "Tele-Danmark Mobil A/S"},
    {238 , 1 , UI_NETWK_TYPE_DCS_1800 , "TDC MOBIL" , "Tele-Danmark Mobil A/S"},
    {238 , 2 , UI_NETWK_TYPE_GSM_900 , "SONO" , "Sonofon"},
    {238 , 2 , UI_NETWK_TYPE_DCS_1800 , "SONO" , "Sonofon"},
    {238 , 20 , UI_NETWK_TYPE_DCS_1800 , "TELIA DK" , "Telia A/S Denmark"},
    {238 , 30 , UI_NETWK_TYPE_GSM_900 , "Orange" , "Orange A/S"},
    {238 , 30 , UI_NETWK_TYPE_DCS_1800 , "Orange" , "Orange A/S"},
    {240 , 1 , UI_NETWK_TYPE_GSM_900 , "TELIA" , "Telia AB"},
    {240 , 3 , UI_NETWK_TYPE_UMTS , "Orange" , "Orange Sverige AB"},
    {240 , 7 , UI_NETWK_TYPE_GSM_900 , "S COMVIQ" , "Tele 2 AB"},
    {240 , 7 , UI_NETWK_TYPE_DCS_1800 , "S COMVIQ" , "Tele 2 AB"},
    {240 , 8 , UI_NETWK_TYPE_GSM_900 , "voda SE" , "Europolitan AB"},
    {240 , 8 , UI_NETWK_TYPE_DCS_1800 , "voda SE" , "Europolitan AB"},
    {242 , 1 , UI_NETWK_TYPE_GSM_900 , "TELENOR" , "Telenor Mobil AS"},
    {242 , 1 , UI_NETWK_TYPE_DCS_1800 , "TELENOR" , "Telenor Mobil AS"},
    {242 , 2 , UI_NETWK_TYPE_GSM_900 , "NetCom" , "NETCOM AS"},
    {242 , 2 , UI_NETWK_TYPE_DCS_1800 , "NetCom" , "NETCOM AS"},
    {244 , 3 , UI_NETWK_TYPE_DCS_1800 , "FINNET" , "Telia Mobile AB"},
    {244 , 5 , UI_NETWK_TYPE_GSM_900 , "FI elisa" , "Radiolinja Origo Oy"},
    {244 , 5 , UI_NETWK_TYPE_DCS_1800 , "FI elisa" , "Radiolinja Origo Oy"},
    {244 , 9 , UI_NETWK_TYPE_DCS_1800 , "Finnet" , "Finnet Group"},
    {244 , 12 , UI_NETWK_TYPE_GSM_900 , "FINNET" , "Suomen 2G Oy"},
    {244 , 12 , UI_NETWK_TYPE_DCS_1800 , "FINNET" , "Suomen 2G Oy"},
    {244 , 14 , UI_NETWK_TYPE_GSM_900 , "FI AMT" , "Alands Mobiltelefon A.B"},
    {244 , 91 , UI_NETWK_TYPE_GSM_900 , "SONERA" , "Sonera Corporation"},
    {244 , 91 , UI_NETWK_TYPE_DCS_1800 , "SONERA" , "Sonera Corporation"},
    {246 , 1 , UI_NETWK_TYPE_GSM_900 , "OMNITEL LT" , "Omnitel"},
    {246 , 2 , UI_NETWK_TYPE_GSM_900 , "LT BITE GSM" , "UAB Bite GSM"},
    {246 , 2 , UI_NETWK_TYPE_DCS_1800 , "LT BITE GSM" , "UAB Bite GSM"},
    {246 , 3 , UI_NETWK_TYPE_GSM_900 , "TELE2" , "UAB TELE2"},
    {246 , 3 , UI_NETWK_TYPE_DCS_1800 , "TELE2" , "UAB TELE2"},
    {247 , 1 , UI_NETWK_TYPE_GSM_900 , "LMT GSM" , "Latvian Mobile Tel. Co."},
    {247 , 1 , UI_NETWK_TYPE_DCS_1800 , "LMT GSM" , "Latvian Mobile Tel. Co."},
    {247 , 2 , UI_NETWK_TYPE_GSM_900 , "TELE2" , "TELE2"},
    {247 , 2 , UI_NETWK_TYPE_DCS_1800 , "TELE2" , "TELE2"},
    {248 , 1 , UI_NETWK_TYPE_GSM_900 , "EE EMT GSM" , "AS EMT"},
    {248 , 1 , UI_NETWK_TYPE_DCS_1800 , "EE EMT GSM" , "AS EMT"},
    {248 , 2 , UI_NETWK_TYPE_GSM_900 , "EE elisa" , "Radiolinja Eesti AS"},
    {248 , 2 , UI_NETWK_TYPE_DCS_1800 , "EE elisa" , "Radiolinja Eesti AS"},
    {248 , 3 , UI_NETWK_TYPE_GSM_900 , "TELE2" , "Tele2 Eesti AS"},
    {250 , 1 , UI_NETWK_TYPE_GSM_900 , "MTS-RUS" , "Mobile Telesystems"},
    {250 , 1 , UI_NETWK_TYPE_GSM_900 , "MTS-RUS" , "Tambovskaya Electrosuyaz"},
    {250 , 1 , UI_NETWK_TYPE_GSM_900 , "MTS-RUS" , "TAIF-Telcom PSC"},
    {250 , 2 , UI_NETWK_TYPE_GSM_900 , "MegaFon" , "North-West GSM"},
    {250 , 2 , UI_NETWK_TYPE_DCS_1800 , "MegaFon" , "North-West GSM"},
    {250 , 2 , UI_NETWK_TYPE_GSM_900 , "MegaFon" , "JSC Sonic Duo"},
    {250 , 2 , UI_NETWK_TYPE_DCS_1800 , "MegaFon" , "JSC Sonic Duo"},
    {250 , 3 , UI_NETWK_TYPE_GSM_900 , "RUS NCC" , "Nizhegorodskaya Cellular Communications"},
    {250 , 4 , UI_NETWK_TYPE_GSM_900 , "RUS_SCN" , "SIBCHALLENGE LTD"},
    {250 , 5 , UI_NETWK_TYPE_GSM_900 , "RUS 05" , "Mobile Comms Systems JSC"},
    {250 , 5 , UI_NETWK_TYPE_GSM_900 , "RUS 05" , "Siberian Cellular Systems-900"},
    {250 , 5 , UI_NETWK_TYPE_GSM_900 , "RUS 05" , "Yeniseitelecom"},
    {250 , 5 , UI_NETWK_TYPE_GSM_900 , "RUS 05" , "Tomsk Cellular Communication Ltd"},
    {250 , 7 , UI_NETWK_TYPE_GSM_900 , "BMT" , "BM Telecom Limited"},
    {250 , 7 , UI_NETWK_TYPE_GSM_900 , "BMT" , "Zao Smarts"},
    {250 , 7 , UI_NETWK_TYPE_DCS_1800 , "BMT" , "Zao Smarts"},
    {250 , 10 , UI_NETWK_TYPE_GSM_900 , "RUS DTC" , "Don Telecom"},
    {250 , 11 , UI_NETWK_TYPE_GSM_900 , "ORENSOT" , "Orensot"},
    {250 , 12 , UI_NETWK_TYPE_GSM_900 , "RUS 12" , "Far Eastern Cellular Systems - 900"},
    {250 , 12 , UI_NETWK_TYPE_DCS_1800 , "RUS 12" , "JSC Sakhalin Telecom Mobile"},
    {250 , 12 , UI_NETWK_TYPE_GSM_900 , "RUS 12" , "Sibintertelecom"},
    {250 , 13 , UI_NETWK_TYPE_GSM_900 , "KUGSM" , "JSC Kuban-GSM"},
    {250 , 16 , UI_NETWK_TYPE_GSM_900 , "NTC" , "New Telephone Company"},
    {250 , 17 , UI_NETWK_TYPE_GSM_900 , "ERMAK" , "Ermak RMS"},
    {250 , 28 , UI_NETWK_TYPE_GSM_900 , "Bee Line" , "JSC Extel"},
    {250 , 39 , UI_NETWK_TYPE_GSM_900 , "Uraltel" , "South Ural Cellular Telephone"},
    {250 , 39 , UI_NETWK_TYPE_GSM_900 , "Uraltel" , "Uraltel Ltd"},
    {250 , 44 , UI_NETWK_TYPE_GSM_900 , "NC-GSM" , "StavTeleSot"},
    {250 , 92 , UI_NETWK_TYPE_GSM_900 , "Primtel" , "Primtelefone"},
    {250 , 92 , UI_NETWK_TYPE_DCS_1800 , "Primtel" , "Primtelefone"},
    {250 , 93 , UI_NETWK_TYPE_GSM_900 , "RUS 93" , "Telecom XXI JSC"},
    {250 , 93 , UI_NETWK_TYPE_DCS_1800 , "RUS 93" , "Telecom XXI JSC"},
    {250 , 99 , UI_NETWK_TYPE_GSM_900 , "Bee Line" , "KB Impuls"},
    {250 , 99 , UI_NETWK_TYPE_DCS_1800 , "Bee Line" , "KB Impuls"},
    {255 , 1 , UI_NETWK_TYPE_GSM_900 , "UA UMC" , "Ukrainian Mobile Comms"},
    {255 , 1 , UI_NETWK_TYPE_DCS_1800 , "UA UMC" , "Ukrainian Mobile Comms"},
    {255 , 2 , UI_NETWK_TYPE_GSM_900 , "WellCOM" , "Ukrainian Radio Systems"},
    {255 , 3 , UI_NETWK_TYPE_GSM_900 , "UA-KS" , "Kyivstar GSM JSC"},
    {255 , 3 , UI_NETWK_TYPE_DCS_1800 , "UA-KS" , "Kyivstar GSM JSC"},
    {255 , 5 , UI_NETWK_TYPE_DCS_1800 , "UA-GT" , "Golden Telecom LLC"},
    {257 , 1 , UI_NETWK_TYPE_GSM_900 , "VELCOM" , "Mobile Digital Communications"},
    {259 , 1 , UI_NETWK_TYPE_GSM_900 , "VOXTEL" , "Voxtel S.A"},
    {259 , 2 , UI_NETWK_TYPE_GSM_900 , "MDCELL" , "Moldcell SA"},
    {260 , 1 , UI_NETWK_TYPE_GSM_900 , "Plus GSM" , "Polkomtel S.A"},
    {260 , 1 , UI_NETWK_TYPE_DCS_1800 , "Plus GSM" , "Polkomtel S.A"},
    {260 , 2 , UI_NETWK_TYPE_GSM_900 , "Era" , "Polska Telefonia Cyfrowa"},
    {260 , 2 , UI_NETWK_TYPE_DCS_1800 , "Era" , "Polska Telefonia Cyfrowa"},
    {260 , 3 , UI_NETWK_TYPE_GSM_900 , "IDEA" , "PTK Centertel"},
    {260 , 3 , UI_NETWK_TYPE_DCS_1800 , "IDEA" , "PTK Centertel"},
    {262 , 1 , UI_NETWK_TYPE_GSM_900 , "TMO D" , "T-Mobile Deutschland GmbH"},
    {262 , 1 , UI_NETWK_TYPE_DCS_1800 , "TMO D" , "T-Mobile Deutschland GmbH"},
    {262 , 2 , UI_NETWK_TYPE_GSM_900 , "Vodafone" , "Vodafone D2 GmbH"},
    {262 , 2 , UI_NETWK_TYPE_DCS_1800 , "Vodafone" , "Vodafone D2 GmbH"},
    {262 , 3 , UI_NETWK_TYPE_DCS_1800 , "E-Plus" , "E-Plus Mobilfunk GmbH"},
    {262 , 7 , UI_NETWK_TYPE_DCS_1800 , "o2 - de" , "Viag Interkom"},
    {262 , 13 , UI_NETWK_TYPE_UMTS , "Mobilcom" , "Mobilcom Multimedia GMBH"},
    {262 , 14 , UI_NETWK_TYPE_UMTS , "Group3G" , "Group 3G UMTS GmbH"},
    {266 , 1 , UI_NETWK_TYPE_GSM_900 , "GIBTEL" , "Gibraltar Telecoms Int'l"},
    {268 , 1 , UI_NETWK_TYPE_GSM_900 , "voda P" , "Vodafone Telecel"},
    {268 , 1 , UI_NETWK_TYPE_DCS_1800 , "voda P" , "Vodafone Telecel"},
    {268 , 3 , UI_NETWK_TYPE_GSM_900 , "OPTIM" , "Optimus Telecomunicacoes, S.A"},
    {268 , 3 , UI_NETWK_TYPE_DCS_1800 , "OPTIM" , "Optimus Telecomunicacoes, S.A"},
    {268 , 6 , UI_NETWK_TYPE_GSM_900 , "P TMN" , "Telecomunicacoes Moveis Nacionais S.A"},
    {268 , 6 , UI_NETWK_TYPE_DCS_1800 , "P TMN" , "Telecomunicacoes Moveis Nacionais S.A"},
    {270 , 1 , UI_NETWK_TYPE_GSM_900 , "LUXGSM" , "P+T Luxembourg"},
    {270 , 1 , UI_NETWK_TYPE_DCS_1800 , "LUXGSM" , "P+T Luxembourg"},
    {270 , 77 , UI_NETWK_TYPE_GSM_900 , "L TANGO" , "Tango S.A"},
    {270 , 77 , UI_NETWK_TYPE_DCS_1800 , "L TANGO" , "Tango S.A"},
    {272 , 1 , UI_NETWK_TYPE_GSM_900 , "vodafone IE" , "Vodafone Ireland Plc"},
    {272 , 2 , UI_NETWK_TYPE_GSM_900 , "02 -IRL" , "Digifone MM02 Ltd"},
    {272 , 2 , UI_NETWK_TYPE_DCS_1800 , "02 -IRL" , "Digifone MM02 Ltd"},
    {272 , 3 , UI_NETWK_TYPE_GSM_900 , "IRL-METEOR" , "METEOR"},
    {272 , 3 , UI_NETWK_TYPE_DCS_1800 , "IRL-METEOR" , "METEOR"},
    {274 , 1 , UI_NETWK_TYPE_GSM_900 , "IS SIMINN" , "Iceland Telecom Ltd"},
    {274 , 1 , UI_NETWK_TYPE_DCS_1800 , "IS SIMINN" , "Iceland Telecom Ltd"},
    {274 , 2 , UI_NETWK_TYPE_GSM_900 , "Vodafone" , "TAL hf"},
    {274 , 2 , UI_NETWK_TYPE_DCS_1800 , "Vodafone" , "TAL hf"},
    {274 , 3 , UI_NETWK_TYPE_GSM_900 , "Vodafone" , "Islandssimi GSM ehf"},
    {274 , 3 , UI_NETWK_TYPE_DCS_1800 , "Vodafone" , "Islandssimi GSM ehf"},
    {274 , 4 , UI_NETWK_TYPE_DCS_1800 , "Viking" , "IMC Island ehf"},
    {276 , 2 , UI_NETWK_TYPE_GSM_900 , "voda AL" , "Vodafone Albania"},
    {276 , 2 , UI_NETWK_TYPE_DCS_1800 , "voda AL" , "Vodafone Albania"},
    {278 , 1 , UI_NETWK_TYPE_GSM_900 , "voda MT" , "Vodafone Malta Limited"},
    {278 , 21 , UI_NETWK_TYPE_DCS_1800 , "go mobile" , "Mobisle Communications Limited"},
    {280 , 1 , UI_NETWK_TYPE_GSM_900 , "CY CYTAGSM" , "CYTAGSM"},
    {282 , 1 , UI_NETWK_TYPE_GSM_900 , "GCELL" , "Geocell Limited"},
    {282 , 2 , UI_NETWK_TYPE_GSM_900 , "MAGTI" , "Magti Com"},
    {282 , 2 , UI_NETWK_TYPE_DCS_1800 , "MAGTI" , "Magti Com"},
    {283 , 1 , UI_NETWK_TYPE_GSM_900 , "RA-ARMGSM" , "ARMGSM"},
    {284 , 1 , UI_NETWK_TYPE_GSM_900 , "M-TEL" , "MobilTEL AD"},
    {284 , 5 , UI_NETWK_TYPE_GSM_900 , "BG GLOBUL " , "Cosmo Bulgaria Mobile EAD"},
    {284 , 5 , UI_NETWK_TYPE_DCS_1800 , "BG GLOBUL " , "Cosmo Bulgaria Mobile EAD"},
    {286 , 1 , UI_NETWK_TYPE_GSM_900 , "TCELL" , "Turkcell"},
    {286 , 2 , UI_NETWK_TYPE_GSM_900 , "TELSIM GSM" , "TELSIM Mobil Telekomuniksyon Hiz.A.S"},
    {286 , 3 , UI_NETWK_TYPE_DCS_1800 , "AVEA" , "IS-TIM Telekomunikasyon Hizmetleri Anonim Sirketi"},
    {286 , 4 , UI_NETWK_TYPE_DCS_1800 , "AYCELL" , "Turk Telekomunikasyon A.S."},
    {288 , 1 , UI_NETWK_TYPE_GSM_900 , "FT-GSM" , "Faroese Telecom GSM 900"},
    {288 , 2 , UI_NETWK_TYPE_GSM_900 , "KALL" , "P/F Kall (Kall Telecom)"},
    {290 , 1 , UI_NETWK_TYPE_GSM_900 , "TELE GRL" , "Tele Greenland A/S"},
    {293 , 40 , UI_NETWK_TYPE_GSM_900 , "SI voda" , "SI.Mobil d.d"},
    {293 , 40 , UI_NETWK_TYPE_DCS_1800 , "SI voda" , "SI.Mobil d.d"},
    {293 , 41 , UI_NETWK_TYPE_GSM_900 , "MOBITEL" , "Mobitel d.d"},
    {293 , 70 , UI_NETWK_TYPE_DCS_1800 , "VEGA 070" , "Western Wireless International d.o.o."},
    {294 , 1 , UI_NETWK_TYPE_GSM_900 , "MOBI-M" , "MOBIMAK AD"},
    {295 , 1 , UI_NETWK_TYPE_GSM_900 , "FL GSM" , "Telecom FL AG"},
    {295 , 2 , UI_NETWK_TYPE_DCS_1800 , "Orange FL" , "VIAG EuroPlatform AG"},
    {295 , 5 , UI_NETWK_TYPE_DCS_1800 , "FL1" , "Mobilkom (Liechtenstein) AG"},
    {295 , 77 , UI_NETWK_TYPE_GSM_900 , "LI TANGO " , "Tele 2 Aktiengesellschaft"},
    {302 , 37 , UI_NETWK_TYPE_PCS_1900 , "MICROCELL" , "Microcell Telecom Inc"},
    {302 , 370 , UI_NETWK_TYPE_PCS_1900 , "Fido" , "Microcell Telecom Inc"},
    {302 , 720 , UI_NETWK_TYPE_PCS_1900 , "CAN72" , "Rogers Wireless"},
    {310 , 1 , UI_NETWK_TYPE_GSM_900 , "Pacific" , "Pacific Mobile Comms"},
    {310 , 11 , UI_NETWK_TYPE_PCS_1900 , "WTTCKy" , "Third Kentucky Cellular Corporation"},
    {310 , 15 , UI_NETWK_TYPE_PCS_1900 , "Cingular" , "Cingular Wireless"},
    {310 , 16 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 17 , UI_NETWK_TYPE_PCS_1900 , "Cingular" , "Cingular Wireless"},
    {310 , 20 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 21 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 22 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 23 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 24 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 25 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 26 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 27 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Powertel PCS Inc"},
    {310 , 27 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 31 , UI_NETWK_TYPE_PCS_1900 , "FARMERS" , "Voicestream Wireless Corporation"},
    {310 , 34 , UI_NETWK_TYPE_PCS_1900 , "WestLink" , "High Plains/Midwest LLC"},
    {310 , 46 , UI_NETWK_TYPE_PCS_1900 , "SIMMETRY " , "TMP Corp"},
    {310 , 53 , UI_NETWK_TYPE_PCS_1900 , "WVW" , "Key Communications, LLC"},
    {310 , 58 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "PCS One Inc."},
    {310 , 63 , UI_NETWK_TYPE_PCS_1900 , "Choice" , "Choice Wireless L.C."},
    {310 , 64 , UI_NETWK_TYPE_PCS_1900 , "ARDGMC" , "Airadigm Communications"},
    {310 , 110 , UI_NETWK_TYPE_PCS_1900 , "WTTCKy" , "Third Kentucky Cellular Corporation"},
    {310 , 150 , UI_NETWK_TYPE_PCS_1900 , "Cingular" , "Cingular"},
    {310 , 160 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 170 , UI_NETWK_TYPE_PCS_1900 , "Cingular" , "Cingular"},
    {310 , 200 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 210 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 220 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 230 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 240 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 250 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 260 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 270 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Powertel PCS Inc"},
    {310 , 270 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 310 , UI_NETWK_TYPE_PCS_1900 , "VSTREAM" , "Voicestream Wireless Corporation"},
    {310 , 340 , UI_NETWK_TYPE_PCS_1900 , "WestLink" , "High Plains/Midwest LLC"},
    {310 , 380 , UI_NETWK_TYPE_PCS_1900 , "Cingular" , "Cingular"},
    {310 , 410 , UI_NETWK_TYPE_PCS_1900 , "Cingular" , "Cingular"},
    {310 , 460 , UI_NETWK_TYPE_PCS_1900 , "ONELINK" , "TMP Corp"},
    {310 , 530 , UI_NETWK_TYPE_PCS_1900 , "W V W" , "Key Communications, LLC"},
    {310 , 580 , UI_NETWK_TYPE_PCS_1900 , "VSTREAM" , "PCS One Inc."},
    {310 , 630 , UI_NETWK_TYPE_PCS_1900 , "AMERLINK" , "Choice Wireless L.C."},
    {310 , 640 , UI_NETWK_TYPE_PCS_1900 , "ARDGMC" , "Airadigm Communications"},
    {310 , 660 , UI_NETWK_TYPE_PCS_1900 , "TMO" , "Voicestream Wireless Corporation"},
    {310 , 680 , UI_NETWK_TYPE_PCS_1900 , "NPI" , "NPI Wireless"},
    {310 , 690 , UI_NETWK_TYPE_PCS_1900 , "IMMIX" , "Conestoga Wireless Company"},
    {310 , 740 , UI_NETWK_TYPE_PCS_1900 , "TLXT" , "Wireless Telecommunications Company"},
    {310 , 770 , UI_NETWK_TYPE_PCS_1900 , "IWS" , "Iowa Wireless Services LP"},
    {310 , 790 , UI_NETWK_TYPE_PCS_1900 , "Pinpoint" , "PinPoint Wireless Inc."},
    {310 , 800 , UI_NETWK_TYPE_PCS_1900 , "VSTREAM" , "Voicestream Wireless Corporation"},
    {310 , 980 , UI_NETWK_TYPE_PCS_1900 , "Cingular" , "Cingular"},
    {311 , 180 , UI_NETWK_TYPE_PCS_1900 , "Cingular" , "Cingular"},
    {334 , 20 , UI_NETWK_TYPE_PCS_1900 , "TELCEL" , "Radiomovil dipsa SA de CV (TELCEL)"},
    {338 , 5 , UI_NETWK_TYPE_GSM_900 , "DIGICEL" , "Mossel (Jamaica) Ltd"},
    {340 , 1 , UI_NETWK_TYPE_GSM_900 , "F-Orange" , "ORANGE CARAIBE"},
    {340 , 20 , UI_NETWK_TYPE_GSM_900 , "BOUYGTEL-C" , "Bouygues Telecom Caraibe"},
    {340 , 20 , UI_NETWK_TYPE_DCS_1800 , "BOUYGTEL-C" , "Bouygues Telecom Caraibe"},
    {344 , 30 , UI_NETWK_TYPE_PCS_1900 , "APUA-PCS" , "APUA PCS"},
    {350 , 1 , UI_NETWK_TYPE_PCS_1900 , "Telecom" , "Telecommunications (Bermuda & West Indies) Ltd"},
    {362 , 51 , UI_NETWK_TYPE_GSM_900 , "telcell" , "Telcell N.V."},
    {362 , 69 , UI_NETWK_TYPE_GSM_900 , "CT GSM" , "Curacao Telecom N.V."},
    {362 , 69 , UI_NETWK_TYPE_DCS_1800 , "CT GSM" , "Curacao Telecom N.V."},
    {362 , 91 , UI_NETWK_TYPE_GSM_900 , "UTS" , "Setel NV"},
    {368 , 1 , UI_NETWK_TYPE_GSM_900 , "C_Com" , "C_Com"},
    {370 , 1 , UI_NETWK_TYPE_PCS_1900 , "orange" , "France Telecom Dominicana, S.A."},
    {374 , 12 , UI_NETWK_TYPE_DCS_1800 , "TSTT" , "Telcommunications Services of Trinidad and Tobago Ltd"},
    {400 , 1 , UI_NETWK_TYPE_GSM_900 , "ACELL" , "Azercell Telekom B.M."},
    {400 , 2 , UI_NETWK_TYPE_GSM_900 , "BAKCELL GSM 2000" , "Bakcell"},
    {401 , 1 , UI_NETWK_TYPE_GSM_900 , "K-MOBILE" , "KaR-TeL LLC"},
    {401 , 2 , UI_NETWK_TYPE_GSM_900 , "KZ KCELL" , "GSM Kazakhstan JSC 'Kazakhtelecom'"},
    {404 , 0 , UI_NETWK_TYPE_GSM_900 , "BNSL" , "Bharat Sanchar Nigam Ltd."},
    {404 , 1 , UI_NETWK_TYPE_GSM_900 , "Hutch " , "Aircel Digilink India Ltd"},
    {404 , 5 , UI_NETWK_TYPE_GSM_900 , "Hutch" , "Fascel Limited"},
    {404 , 7 , UI_NETWK_TYPE_GSM_900 , "TATA" , "Birla Tata AT&T Limited - Andhra Pradesh"},
    {404 , 9 , UI_NETWK_TYPE_GSM_900 , "RELIANCE" , "Reliance Telecom Private Ltd"},
    {404 , 10 , UI_NETWK_TYPE_GSM_900 , "AirTel" , "Bharti Cellular Ltd - Delhi"},
    {404 , 11 , UI_NETWK_TYPE_GSM_900 , "Hutch" , "Sterling Cellular Ltd"},
    {404 , 12 , UI_NETWK_TYPE_GSM_900 , "IDEA" , "Escotel Mobile Communications Ltd, Haryana"},
    {404 , 13 , UI_NETWK_TYPE_GSM_900 , "Hutch" , "M/s Barakhamba Sales & Services - Andhra Pradesh"},
    {404 , 14 , UI_NETWK_TYPE_GSM_900 , "SPICE" , "Spice Communications Limited - Punjab"},
    {404 , 15 , UI_NETWK_TYPE_GSM_900 , "Hutch" , "Aircel Digilink India Ltd"},
    {404 , 18 , UI_NETWK_TYPE_GSM_900 , "RELIANCE" , "Reliance Telecom Private Ltd"},
    {404 , 19 , UI_NETWK_TYPE_GSM_900 , "IDEA" , "Escotel Mobile Communications Ltd, Kerala"},
    {404 , 20 , UI_NETWK_TYPE_GSM_900 , "Orange" , "Hutchison Max Telecom Ltd"},
    {404 , 21 , UI_NETWK_TYPE_GSM_900 , "BPL MOBILE" , "BPL Mobile Communications Ltd - Mumbai"},
    {404 , 22 , UI_NETWK_TYPE_GSM_900 , "IDEA" , "Birla Tata AT&T Limited - Maharashtra/Goa"},
    {404 , 24 , UI_NETWK_TYPE_GSM_900 , "IDEA" , "Birla Tata AT&T Limited - Gujarat"},
    {404 , 27 , UI_NETWK_TYPE_GSM_900 , "BPL MOBILE" , "BPL Cellular Limited - Maharshtra/Goa"},
    {404 , 30 , UI_NETWK_TYPE_GSM_900 , "Hutch" , "Usha Martin Telekom Ltd"},
    {404 , 31 , UI_NETWK_TYPE_GSM_900 , "AirTel" , "BHARTI MOBITEL LIMITED"},
    {404 , 36 , UI_NETWK_TYPE_GSM_900 , "RELIANCE" , "Reliance Telecom Private Ltd"},
    {404 , 40 , UI_NETWK_TYPE_GSM_900 , "AIRTEL" , "Bharti Mobinet Limited"},
    {404 , 41 , UI_NETWK_TYPE_GSM_900 , "INA RPG" , "RPG Cellular Services"},
    {404 , 42 , UI_NETWK_TYPE_GSM_900 , "AIRCEL" , "Srinivas Cellcom Limited"},
    {404 , 43 , UI_NETWK_TYPE_GSM_900 , "BPL MOBILE" , "BPL Cellular Limited - Tamil Nadu/Pondicherry"},
    {404 , 44 , UI_NETWK_TYPE_GSM_900 , "SPICE" , "Spice Communications Limited - Karnataka"},
    {404 , 45 , UI_NETWK_TYPE_GSM_900 , "AirTel" , "Bharti Mobile Ltd - Karnataka"},
    {404 , 46 , UI_NETWK_TYPE_GSM_900 , "BPL MOB" , "BPL Cellular Limited - Kerala"},
    {404 , 49 , UI_NETWK_TYPE_GSM_900 , "AirTel" , "Bharti Mobile Ltd - Andhra Pradesh"},
    {404 , 50 , UI_NETWK_TYPE_GSM_900 , "RELIANCE" , "Reliance Telecom Private Ltd"},
    {404 , 52 , UI_NETWK_TYPE_GSM_900 , "RELIANCE" , "Reliance Telecom Private Ltd"},
    {404 , 56 , UI_NETWK_TYPE_GSM_900 , "IDEA" , "Escotel Mobile Communications Ltd, UP(W)"},
    {404 , 67 , UI_NETWK_TYPE_GSM_900 , "RELIANCE" , "Reliance Telecom Private Ltd"},
    {404 , 70 , UI_NETWK_TYPE_GSM_900 , "Oasis" , "Hexacom India Limited"},
    {404 , 85 , UI_NETWK_TYPE_GSM_900 , "Reliance" , "Reliance Telecom Private Ltd"},
    {404 , 86 , UI_NETWK_TYPE_DCS_1800 , "Hutch" , "M/s Barakhamba Sales & Services - Karnataka"},
    {410 , 1 , UI_NETWK_TYPE_GSM_900 , "Mobilink" , "Mobilink"},
    {410 , 3 , UI_NETWK_TYPE_GSM_900 , "PK-UFONE" , "Pak Telecom Mobile Ltd"},
    {413 , 2 , UI_NETWK_TYPE_GSM_900 , "DIALOG" , "MTN Networks (Pvt) Ltd"},
    {413 , 3 , UI_NETWK_TYPE_GSM_900 , "CELLTEL" , "Celltel Lanka Limited"},
    {414 , 1 , UI_NETWK_TYPE_GSM_900 , "MPTGSM" , "Myanmar Posts and Telecommunications"},
    {415 , 1 , UI_NETWK_TYPE_GSM_900 , "alfa" , "FTML Service Cellulaire"},
    {415 , 3 , UI_NETWK_TYPE_GSM_900 , "MTC LIBAN" , "LibanCell"},
    {416 , 1 , UI_NETWK_TYPE_GSM_900 , "FSTLNK" , "Jordan M.T.S"},
    {416 , 77 , UI_NETWK_TYPE_GSM_900 , "JO MobCom" , "MobileCom"},
    {417 , 2 , UI_NETWK_TYPE_GSM_900 , "areeba" , "Spacetel Syria"},
    {417 , 2 , UI_NETWK_TYPE_DCS_1800 , "areeba" , "Spacetel Syria"},
    {417 , 9 , UI_NETWK_TYPE_GSM_900 , "MOBILE" , "Syrian Telecom Est."},
    {417 , 93 , UI_NETWK_TYPE_GSM_900 , "Syriatel" , "Syriatel Holdings S.A"},
    {417 , 93 , UI_NETWK_TYPE_DCS_1800 , "Syriatel" , "Syriatel Holdings S.A"},
    {418 , 66 , UI_NETWK_TYPE_GSM_900 , "Newroz Telecom", "Newroz Telecom"},
    {418 , 66 , UI_NETWK_TYPE_DCS_1800 , "Newroz Telecom", "Newroz Telecom"},
    {419 , 2 , UI_NETWK_TYPE_GSM_900 , "MTC" , "Mobile Telecommunications Company"},
    {419 , 2 , UI_NETWK_TYPE_DCS_1800 , "MTC" , "Mobile Telecommunications Company"},
    {419 , 3 , UI_NETWK_TYPE_GSM_900 , "KT WATANYA" , "National Mobile Telecommunications Co"},
    {419 , 3 , UI_NETWK_TYPE_DCS_1800 , "KT WATANYA" , "National Mobile Telecommunications Co"},
    {420 , 1 , UI_NETWK_TYPE_GSM_900 , "KSA" , "Saudi Telecom Company"},
    {420 , 7 , UI_NETWK_TYPE_GSM_900 , "EAE" , "Electronics App' Est."},
    {421 , 2 , UI_NETWK_TYPE_GSM_900 , "Spacetel" , "Spacetel - Yemen"},
    {422 , 2 , UI_NETWK_TYPE_GSM_900 , "OMAN" , "General Telecoms"},
    {424 , 2 , UI_NETWK_TYPE_GSM_900 , "ETSLT" , "ETISALAT"},
    {425 , 1 , UI_NETWK_TYPE_GSM_900 , "IL ORANGE" , "Partner Communications Company Ltd"},
    {425 , 2 , UI_NETWK_TYPE_DCS_1800 , "Cellcom" , "Cellcom Israel Ltd"},
    {425 , 5 , UI_NETWK_TYPE_GSM_900 , "JAWWAL" , "Palestine Telecoms Co Plc"},
    {426 , 1 , UI_NETWK_TYPE_GSM_900 , "BATELCO" , "Batelco"},
    {427 , 1 , UI_NETWK_TYPE_GSM_900 , "Q-NET" , "Q-Tel"},
    {428 , 99 , UI_NETWK_TYPE_GSM_900 , "MobiCom" , "MobiCom"},
    {429 , 1 , UI_NETWK_TYPE_GSM_900 , "Nepal Telcom" , "Nepal Telecommunications Corporation"},
    {432 , 11 , UI_NETWK_TYPE_GSM_900 , "432 11" , "TCI"},
    {432 , 14 , UI_NETWK_TYPE_GSM_900 , "KIFZO" , "Payam Kish Co."},
    {434 , 1 , UI_NETWK_TYPE_GSM_900 , "Buztel" , "Buztel"},
    {434 , 2 , UI_NETWK_TYPE_GSM_900 , "UZMGSM" , "JV Uzmacom"},
    {434 , 4 , UI_NETWK_TYPE_GSM_900 , "DW-GSM" , "DAEWOO Unitel Company"},
    {434 , 4 , UI_NETWK_TYPE_DCS_1800 , "DW-GSM" , "DAEWOO Unitel Company"},
    {434 , 5 , UI_NETWK_TYPE_GSM_900 , "COSCOM" , "Coscom"},
    {437 , 1 , UI_NETWK_TYPE_GSM_900 , "BITEL" , "Bitel Limited"},
    {438 , 1 , UI_NETWK_TYPE_GSM_900 , "BCTI" , "Barash Communication Technologies Inc."},
    {452 , 1 , UI_NETWK_TYPE_GSM_900 , "VMS" , "Vietnam Mobile Telecom Service"},
    {452 , 2 , UI_NETWK_TYPE_GSM_900 , "GPC" , "Vietnam Telecoms Services Co (GPC)"},
    {454 , 0 , UI_NETWK_TYPE_GSM_900 , "CSL" , "Hong Kong CSL Limited GSM 900/1800"},
    {454 , 0 , UI_NETWK_TYPE_DCS_1800 , "CSL" , "Hong Kong CSL Limited GSM 900/1800"},
    {454 , 4 , UI_NETWK_TYPE_GSM_900 , "3(2G)" , "Hutchison Telecom (HK) Ltd"},
    {454 , 4 , UI_NETWK_TYPE_DCS_1800 , "3(2G)" , "Hutchison Telecom (HK) Ltd"},
    {454 , 6 , UI_NETWK_TYPE_GSM_900 , "SMC-Voda" , "SmarTone Mobile Comms Ltd"},
    {454 , 6 , UI_NETWK_TYPE_DCS_1800 , "SMC-Voda" , "SmarTone Mobile Comms Ltd"},
    {454 , 10 , UI_NETWK_TYPE_DCS_1800 , "NWPCS" , "New World PCS Ltd"},
    {454 , 12 , UI_NETWK_TYPE_DCS_1800 , "PEOPLES" , "Peoples Telephone Co Ltd"},
    {454 , 16 , UI_NETWK_TYPE_DCS_1800 , "SUNDAY" , "Mandarin Communications"},
    {454 , 18 , UI_NETWK_TYPE_DCS_1800 , "CSL" , "Pacific Link"},
    {454 , 22 , UI_NETWK_TYPE_DCS_1800 , "HK P PLUS" , "P Plus Communicatoins Ltd"},
    {455 , 0 , UI_NETWK_TYPE_GSM_900 , "SmarTone" , "SmarTone Mobile Communications (Macau) Ltd"},
    {455 , 0 , UI_NETWK_TYPE_DCS_1800 , "SmarTone" , "SmarTone Mobile Communications (Macau) Ltd"},
    {455 , 1 , UI_NETWK_TYPE_GSM_900 , "CTM" , "C.T.M."},
    {455 , 1 , UI_NETWK_TYPE_DCS_1800 , "CTM" , "C.T.M."},
    {455 , 3 , UI_NETWK_TYPE_GSM_900 , "HT Macau" , "Hutchison Telephone (Macau) Company Limited"},
    {455 , 3 , UI_NETWK_TYPE_DCS_1800 , "HT Macau" , "Hutchison Telephone (Macau) Company Limited"},
    {456 , 1 , UI_NETWK_TYPE_GSM_900 , "MT-KHM" , "CamGSM"},
    {456 , 2 , UI_NETWK_TYPE_GSM_900 , "KHM-SM" , "Cambodia Samart Communication Co Ltd"},
    {456 , 18 , UI_NETWK_TYPE_DCS_1800 , "CAMSHIN" , "Cambodia Shinawatra"},
    {457 , 1 , UI_NETWK_TYPE_GSM_900 , "LAO GSM" , "Lao Shinawatra Telecom"},
    {457 , 2 , UI_NETWK_TYPE_GSM_900 , "ETLMNW" , "Enterprise of Telecommunications Lao (ETL)"},
    {460 , 0 , UI_NETWK_TYPE_GSM_900 , "CHN MOBILE" , "China Mobile Communication Corp."},
    {460 , 1 , UI_NETWK_TYPE_GSM_900 , "CHN-CUGSM" , "China Unicom"},
    {460,  3,  UI_NETWK_TYPE_GSM_900 , "China Telecom", "China Telecom"},
    {466 , 1 , UI_NETWK_TYPE_GSM_900 , "Far EasTone" , "Far EasTone Telecommunications GSM 900/1800"},
    {466 , 1 , UI_NETWK_TYPE_DCS_1800 , "Far EasTone" , "Far EasTone Telecommunications GSM 900/1800"},
    {466 , 68 , UI_NETWK_TYPE_GSM_SAT , "ACeS" , "ACeS International Limited (AIL) - TAIWAN"},
    {466 , 88 , UI_NETWK_TYPE_DCS_1800 , "KG Telecom" , "KG Telecom"},
    {466 , 92 , UI_NETWK_TYPE_GSM_900 , "Chunghwa" , "Chunghwa Telecom LDM"},
    {466 , 92 , UI_NETWK_TYPE_DCS_1800 , "Chunghwa" , "Chunghwa Telecom LDM"},
    {466 , 93 , UI_NETWK_TYPE_GSM_900 , "MobiTai" , "Mobitai Communications Corp."},
    {466 , 97 , UI_NETWK_TYPE_DCS_1800 , "TW Mobile" , "Taiwan Cellular Corporation"},
    {466 , 99 , UI_NETWK_TYPE_GSM_900 , "TransAsia" , "TransAsia Telecoms"},
    {470 , 1 , UI_NETWK_TYPE_GSM_900 , "BGD-GP" , "Grameen Phone Ltd"},
    {470 , 2 , UI_NETWK_TYPE_GSM_900 , "BGD AKTEL" , "TM International (Bangladesh) Ltd"},
    {470 , 19 , UI_NETWK_TYPE_GSM_900 , "M2K" , "Sheba Telecom (PVT) Ltd"},
    {472 , 1 , UI_NETWK_TYPE_GSM_900 , "D-MOBILE" , "Dhivehi Raajjeyge Gulhun Private Ltd GSM 900"},
    {502 , 12 , UI_NETWK_TYPE_GSM_900 , "MY MAXIS" , "Maxis Mobile Sdn Bhd"},
    {502 , 13 , UI_NETWK_TYPE_GSM_900 , "CELCOM" , "Telekom Cellular Sdn Bhd"},
    {502 , 16 , UI_NETWK_TYPE_DCS_1800 , "DiGi" , "DiGi Telecommunications Sdn Bhd"},
    {502 , 17 , UI_NETWK_TYPE_DCS_1800 , "MY ADAM" , "TIMECel Sdn Bhd"},
    {502 , 19 , UI_NETWK_TYPE_GSM_900 , "CELCOM" , "Celcom (Malaysia) Sdn Bhd"},
    {505 , 1 , UI_NETWK_TYPE_GSM_900 , "Telstra Mobile" , "Telstra Mobile Comms"},
    {505 , 1 , UI_NETWK_TYPE_DCS_1800 , "Telstra Mobile" , "Telstra Mobile Comms"},
    {505 , 2 , UI_NETWK_TYPE_GSM_900 , "OPTUS" , "Singtel Optus Limited"},
    {505 , 3 , UI_NETWK_TYPE_GSM_900 , "voda AU" , "Vodafone Pacific Pty Ltd"},
    {505 , 8 , UI_NETWK_TYPE_DCS_1800 , "One.Tel" , "One.Tel Networks"},
    {510 , 0 , UI_NETWK_TYPE_GSM_SAT , "ACeS" , "ACeS International Limited (AIL) - INDONESIA"},
    {510 , 1 , UI_NETWK_TYPE_GSM_900 , "IND INDOSAT" , "PT. Satelit Palapa Indonesia"},
    {510 , 8 , UI_NETWK_TYPE_DCS_1800 , "LIPPO TEL" , "PT Natrindo Telepon Seluler"},
    {510 , 10 , UI_NETWK_TYPE_GSM_900 , "T-SEL" , "Telkomsel"},
    {510 , 11 , UI_NETWK_TYPE_GSM_900 , "proXL" , "PT. Excelcomindo"},
    {510 , 11 , UI_NETWK_TYPE_DCS_1800 , "proXL" , "PT. Excelcomindo"},
    {510 , 21 , UI_NETWK_TYPE_DCS_1800 , "INDOSAT" , "PT Indosat - Indonesia"},
    {515 , 1 , UI_NETWK_TYPE_GSM_900 , "ISLACOM" , "Isla Comms Co. Inc."},
    {515 , 2 , UI_NETWK_TYPE_GSM_900 , "GLOBE" , "Globe Telecom, GMCR Inc."},
    {515 , 3 , UI_NETWK_TYPE_GSM_900 , "SMART" , "Smart Communications Inc."},
    {515 , 3 , UI_NETWK_TYPE_DCS_1800 , "SMART" , "Smart Communications Inc."},
    {515 , 5 , UI_NETWK_TYPE_DCS_1800 , "SUN" , "Digital Telecommunications Phils, Inc"},
    {515 , 11 , UI_NETWK_TYPE_GSM_SAT , "ACeS" , "ACeS International Limited (AIL) - PHILIPPINES"},
    {520 , 1 , UI_NETWK_TYPE_GSM_900 , "TH GSM" , "Advanced Info Service Plc"},
    {520 , 10 , UI_NETWK_TYPE_DCS_1800 , "TH WCS" , "Wireless Comm. Services Co."},
    {520 , 15 , UI_NETWK_TYPE_PCS_1900 , "ACT-1900" , "ACT Mobile Company, Limited"},
    {520 , 18 , UI_NETWK_TYPE_DCS_1800 , "TH-DTAC" , "Total Access Comms Co."},
    {520 , 20 , UI_NETWK_TYPE_GSM_SAT , "ACeS" , "ACeS International Limited (AIL) - THAILAND"},
    {520 , 23 , UI_NETWK_TYPE_DCS_1800 , "GSM 1800" , "Digital Phone Co Ltd"},
    {520 , 99 , UI_NETWK_TYPE_DCS_1800 , "Orange TH" , "TA Orange Company Ltd"},
    {525 , 1 , UI_NETWK_TYPE_GSM_900 , "SingTel" , "Singapore Telecom GSM 900"},
    {525 , 2 , UI_NETWK_TYPE_DCS_1800 , "SingTel" , "Singapore Telecom GSM 1800"},
    {525 , 3 , UI_NETWK_TYPE_GSM_900 , "M1-3GSM" , "MobileOne (Asia) Pte Ltd"},
    {525 , 3 , UI_NETWK_TYPE_DCS_1800 , "M1-3GSM" , "MobileOne (Asia) Pte Ltd"},
    {525 , 5 , UI_NETWK_TYPE_DCS_1800 , "STARHUB" , "StarHub Pte Ltd"},
    {528 , 11 , UI_NETWK_TYPE_GSM_900 , "DSTCom" , "DST Communications Sdn Bhd"},
    {530 , 1 , UI_NETWK_TYPE_GSM_900 , "voda NZ" , "Vodafone New Zealand Limited"},
    {539 , 1 , UI_NETWK_TYPE_GSM_900 , "U-CALL" , "Tonga Communications Corporation"},
    {541 , 0 , UI_NETWK_TYPE_GSM_SAT , "ACeS" , "ACeS International Limited (AIL) - REGIONAL"},
    {541 , 1 , UI_NETWK_TYPE_GSM_900 , "SMILE" , "Telecom Vanuatu Ltd"},
    {542 , 1 , UI_NETWK_TYPE_GSM_900 , "VODAFONE" , "Vodafone Fiji Limited"},
    {544 , 11 , UI_NETWK_TYPE_PCS_1900 , "Blue Sky" , "Blue Sky"},
    {546 , 1 , UI_NETWK_TYPE_GSM_900 , "MOBNCL" , "OPT New Caledonia"},
    {547 , 20 , UI_NETWK_TYPE_GSM_900 , "F-VINI" , "Tikiphone S.A"},
    {550 , 1 , UI_NETWK_TYPE_GSM_900 , "FSMTC" , "FSM Telecommunications Corporation"},
    {602 , 1 , UI_NETWK_TYPE_GSM_900 , "MobiNiL" , "The Egyptian Company for Mobile Services"},
    {602 , 2 , UI_NETWK_TYPE_GSM_900 , "voda EG" , "Misrfone Telecommunications Co."},
    {603 , 1 , UI_NETWK_TYPE_GSM_900 , "Mobilis" , "AMN"},
    {603 , 2 , UI_NETWK_TYPE_GSM_900 , "Djezzy" , "Djezzy"},
    {603 , 2 , UI_NETWK_TYPE_DCS_1800 , "Djezzy" , "Djezzy"},
    {604 , 0 , UI_NETWK_TYPE_GSM_900 , "MEDITEL" , "Medi Telecom"},
    {604 , 1 , UI_NETWK_TYPE_GSM_900 , "IAM" , "Itissalat Al-Maghrib S.A"},
    {605 , 2 , UI_NETWK_TYPE_GSM_900 , "TUNTEL " , "Tunisie Telecom"},
    {607 , 1 , UI_NETWK_TYPE_GSM_900 , "GAMCEL" , "Gambia Telecommunications Cellular Company Ltd"},
    {608 , 1 , UI_NETWK_TYPE_GSM_900 , "ALIZE" , "Sonatel"},
    {608 , 2 , UI_NETWK_TYPE_GSM_900 , "SENTEL" , "Sentel GSM"},
    {610 , 1 , UI_NETWK_TYPE_GSM_900 , "MALITEL ML" , "Malitel-SA"},
    {611 , 1 , UI_NETWK_TYPE_GSM_900 , "Mobilis" , "Spacetel Guinee SA"},
    {611 , 2 , UI_NETWK_TYPE_GSM_900 , "LAGUI" , "Sotelgui SA"},
    {612 , 1 , UI_NETWK_TYPE_GSM_900 , "CORA" , "CORA de COMSTAR"},
    {612 , 3 , UI_NETWK_TYPE_GSM_900 , "Orange CI" , "S.I.M."},
    {612 , 5 , UI_NETWK_TYPE_GSM_900 , "TELECEL" , "Loteny Telecom (SA)"},
    {613 , 1 , UI_NETWK_TYPE_GSM_900 , "Onatel" , "Onatel"},
    {613 , 2 , UI_NETWK_TYPE_GSM_900 , "celtel" , "Celtel Burkina Faso"},
    {614 , 2 , UI_NETWK_TYPE_GSM_900 , "CELTEL" , "Celtel Niger"},
    {615 , 1 , UI_NETWK_TYPE_GSM_900 , "TGCELL" , "Togo Cellulaire"},
    {616 , 1 , UI_NETWK_TYPE_GSM_900 , "LIBERCOM" , "LIBERCOM"},
    {616 , 2 , UI_NETWK_TYPE_GSM_900 , "TLCL-BEN" , "Telecel Benin Ltd"},
    {616 , 3 , UI_NETWK_TYPE_GSM_900 , "BENCELL" , "Spacetel-Benin"},
    {617 , 1 , UI_NETWK_TYPE_GSM_900 , "CELL +" , "Cellplus Mobile Comms"},
    {617 , 10 , UI_NETWK_TYPE_GSM_900 , "EMTEL" , "Emtel Ltd"},
    {618 , 1 , UI_NETWK_TYPE_GSM_900 , "LoneStar" , "Lonestar Communications Corporation"},
    {620 , 1 , UI_NETWK_TYPE_GSM_900 , "SPACE" , "ScanCom Ltd"},
    {620 , 2 , UI_NETWK_TYPE_GSM_900 , "GH ONEtouch" , "Ghana Telecommunications Company Ltd"},
    {620 , 3 , UI_NETWK_TYPE_GSM_900 , "mobitel" , "Millicom Ghana Limited"},
    {621 , 20 , UI_NETWK_TYPE_GSM_900 , "VMOBILE" , "Econet Wireless Nigeria Ltd"},
    {621 , 20 , UI_NETWK_TYPE_DCS_1800 , "VMOBILE" , "Econet Wireless Nigeria Ltd"},
    {621 , 30 , UI_NETWK_TYPE_GSM_900 , "MTN-NG" , "MTN Nigeria Communications Limited"},
    {621 , 30 , UI_NETWK_TYPE_DCS_1800 , "MTN-NG" , "MTN Nigeria Communications Limited"},
    {621 , 40 , UI_NETWK_TYPE_GSM_900 , "Mtel" , "Nigerian Telecommunications Limited (NITEL)"},
    {621 , 40 , UI_NETWK_TYPE_DCS_1800 , "Mtel" , "Nigerian Telecommunications Limited (NITEL)"},
    {622 , 2 , UI_NETWK_TYPE_GSM_900 , "LIBERTIS" , "Tchad Mobile SA"},
    {624 , 1 , UI_NETWK_TYPE_GSM_900 , "62401" , "MTN Cameroon Ltd"},
    {624 , 2 , UI_NETWK_TYPE_GSM_900 , "Orange" , "Societe Camerounaise de Mobiles"},
    {625 , 1 , UI_NETWK_TYPE_GSM_900 , "CPV MOVEL" , "Cabo Verde Telecom"},
    {625 , 1 , UI_NETWK_TYPE_GSM_900 , "CPV MOVEL" , "Cabo Verde Telecom"},
    {628 , 1 , UI_NETWK_TYPE_GSM_900 , "LIBERTIS" , "Libertis S.A."},
    {628 , 2 , UI_NETWK_TYPE_GSM_900 , "TELECEL" , "Telecel Gabon SA"},
    {628 , 3 , UI_NETWK_TYPE_GSM_900 , "CELTEL GA" , "Celtel Gabon SA"},
    {629 , 1 , UI_NETWK_TYPE_GSM_900 , "CELTEL" , "Libertis Telecom"},
    {630 , 1 , UI_NETWK_TYPE_GSM_900 , "VODACOM CD" , "Vodacom Congo (RDC) sprl"},
    {630 , 2 , UI_NETWK_TYPE_GSM_900 , "CELTEL" , "CelTel Congo SA"},
    {630 , 4 , UI_NETWK_TYPE_GSM_900 , "CELLCO" , "CELLCO Sarl"},
    {630 , 89 , UI_NETWK_TYPE_GSM_900 , "OASIS" , "SAIT Telecom SPRL"},
    {631 , 1 , UI_NETWK_TYPE_GSM_900 , "BARAKAAT" , "BARAKAAT GLOBETELCOMPANY"},
    {631 , 82 , UI_NETWK_TYPE_GSM_900 , "telsom" , "Telsom Mobile Somalia"},
    {633 , 1 , UI_NETWK_TYPE_GSM_900 , "C&W SEY" , "Cable & Wireless (Seychelles) Ltd"},
    {633 , 10 , UI_NETWK_TYPE_GSM_900 , "AIRTEL" , "Telecom (Seychelles Limited)"},
    {634 , 1 , UI_NETWK_TYPE_GSM_900 , "MobiTel" , "Sudanese Mobile Telephone Company Ltd"},
    {635 , 10 , UI_NETWK_TYPE_GSM_900 , "RCELL" , "Rwandacell SARL"},
    {636 , 1 , UI_NETWK_TYPE_GSM_900 , "ET-MTN" , "Ethiopian Telecoms Auth."},
    {639 , 2 , UI_NETWK_TYPE_GSM_900 , "SAF-COM" , "Safaricom Limited"},
    {639 , 3 , UI_NETWK_TYPE_GSM_900 , "CELTEL" , "Kencell Communications Ltd"},
    {640 , 1 , UI_NETWK_TYPE_GSM_900 , "TRITEL-TZ" , "Tritel (T) Ltd"},
    {640 , 2 , UI_NETWK_TYPE_GSM_900 , "MOBITEL" , "MIC Tanzania Ltd"},
    {640 , 3 , UI_NETWK_TYPE_GSM_900 , "ZANTEL" , "Zanzibar Telecom"},
    {640 , 3 , UI_NETWK_TYPE_DCS_1800 , "ZANTEL" , "Zanzibar Telecom"},
    {640 , 4 , UI_NETWK_TYPE_GSM_900 , "VodaCom" , "Vodacom Tanzania Ltd"},
    {640 , 4 , UI_NETWK_TYPE_DCS_1800 , "VodaCom" , "Vodacom Tanzania Ltd"},
    {640 , 5 , UI_NETWK_TYPE_GSM_900 , "CELTEL" , "Celtel Tanzania Limited"},
    {640 , 5 , UI_NETWK_TYPE_DCS_1800 , "CELTEL" , "Celtel Tanzania Limited"},
    {641 , 1 , UI_NETWK_TYPE_GSM_900 , "CELTEL" , "Clovergem Celtel"},
    {641 , 10 , UI_NETWK_TYPE_GSM_900 , "MTN-UG" , "MTN Uganda Ltd"},
    {641 , 10 , UI_NETWK_TYPE_DCS_1800 , "MTN-UG" , "MTN Uganda Ltd"},
    {641 , 11 , UI_NETWK_TYPE_GSM_900 , "UTL-Mango" , "Uganda Telecom Ltd"},
    {642 , 1 , UI_NETWK_TYPE_GSM_900 , "SPACETEL" , "Spacetel - Burundi"},
    {642 , 2 , UI_NETWK_TYPE_GSM_900 , "SAFARIS" , "Africell PLC Company"},
    {643 , 1 , UI_NETWK_TYPE_GSM_900 , "MOZ-mCel" , "T.D.M GSM 900/1800"},
    {643 , 1 , UI_NETWK_TYPE_DCS_1800 , "MOZ-mCel" , "T.D.M GSM 900/1800"},
    {645 , 1 , UI_NETWK_TYPE_GSM_900 , "CELTEL" , "Celtel Zambia"},
    {645 , 2 , UI_NETWK_TYPE_GSM_900 , "TELECEL" , "Telecel Zambia Limited"},
    {646 , 1 , UI_NETWK_TYPE_GSM_900 , "Madacom" , "Madacom SA"},
    {646 , 2 , UI_NETWK_TYPE_GSM_900 , "MG ANTARIS" , "SMM"},
    {646 , 3 , UI_NETWK_TYPE_GSM_900 , "Life" , "Life"},
    {647 , 0 , UI_NETWK_TYPE_GSM_900 , "Orange" , "Orange Reunion"},
    {647 , 0 , UI_NETWK_TYPE_DCS_1800 , "Orange" , "Orange Reunion"},
    {647 , 2 , UI_NETWK_TYPE_DCS_1800 , "Outremer Telecom" , "Outremer Telecom"},
    {647 , 10 , UI_NETWK_TYPE_GSM_900 , "SFR RU" , "Societe Reunionnaise"},
    {648 , 1 , UI_NETWK_TYPE_GSM_900 , "NETONE" , "P + T Corporation"},
    {648 , 3 , UI_NETWK_TYPE_GSM_900 , "TELECEL" , "Telecel Zimbabwe (PVT) Ltd"},
    {648 , 4 , UI_NETWK_TYPE_GSM_900 , "ECONET" , "Econet Wireless"},
    {649 , 1 , UI_NETWK_TYPE_GSM_900 , "MTCNAM" , "Mobile Telecommunications Limited"},
    {650 , 1 , UI_NETWK_TYPE_GSM_900 , "MW CP 900" , "Telekom Network Ltd"},
    {650 , 10 , UI_NETWK_TYPE_GSM_900 , "CELTEL" , "CelTel Limited"},
    {651 , 1 , UI_NETWK_TYPE_GSM_900 , "Vodacom-LS" , "Vodacom Lesotho (Pty) Ltd"},
    {651 , 2 , UI_NETWK_TYPE_GSM_900 , "EZI-CEL" , "Tele-Com Mobile (Pty) Ltd"},
    {652 , 1 , UI_NETWK_TYPE_GSM_900 , "MASCOM" , "Mascom Wireless"},
    {652 , 2 , UI_NETWK_TYPE_GSM_900 , "Orange" , "Vista Cellular (Pty) Limited"},
    {653 , 10 , UI_NETWK_TYPE_GSM_900 , "SwaziMTN" , "Swazi MTN (Ltd)"},
    {655 , 1 , UI_NETWK_TYPE_GSM_900 , "VODA" , "Vodacom (Pty) Ltd"},
    {655 , 7 , UI_NETWK_TYPE_DCS_1800 , "Cell C" , "Cell C (Pty) Ltd"},
    {655 , 10 , UI_NETWK_TYPE_GSM_900 , "MTN-SA" , "MTN (Pty) Ltd"},
    {706 , 1 , UI_NETWK_TYPE_PCS_1900 , "PERSONAL" , "CTE Telecom Personal SA de CV"},
    {706 , 2 , UI_NETWK_TYPE_GSM_900 , "Digicel" , "DIGICEL, S.A. de C.V."},
    {716 , 10 , UI_NETWK_TYPE_PCS_1900 , "Claro" , "TIM Peru S.A.C."},
    {722 , 7 , UI_NETWK_TYPE_PCS_1900 , "UNIFON" , "UNIFON"},
    {722 , 34 , UI_NETWK_TYPE_PCS_1900 , "AR TP" , "Telecom Personal S.A."},
    {722 , 35 , UI_NETWK_TYPE_GSM_900 , "P-HABLE" , "PORT-HABLE"},
    {724 , 2, UI_NETWK_TYPE_DCS_1800 , "TIM BRASIL", "TIM"},
    {724 , 3,   UI_NETWK_TYPE_DCS_1800 , "TIM BRASIL", "TIM"},
    {724 , 4,   UI_NETWK_TYPE_DCS_1800 , "TIM BRASIL", "TIM"},
    {724 , 5,   UI_NETWK_TYPE_DCS_1800 , "Claro", "Claro"},
    {724 , 15, UI_NETWK_TYPE_DCS_1800 , "BRA SCTL", "SCTL"},
    {724 , 16, UI_NETWK_TYPE_DCS_1800 , "Brasil Telecom Celular", "BrTCel"},
    {724 , 23, UI_NETWK_TYPE_DCS_1800 , "Telemig Celular S/A", "TELEMIGC"},
    {724 , 24, UI_NETWK_TYPE_DCS_1800 , "Amazonia Celular S/A", "AMAZONIA"},
    {724 , 31, UI_NETWK_TYPE_DCS_1800 , "TNL PCS S.A.", "Oi"},
    {724 , 32, UI_NETWK_TYPE_DCS_1800 , "CTBC Cellular", "CTBC"},
    {724 , 33, UI_NETWK_TYPE_DCS_1800 , "CTBC Cellular", "CTBC"},
    {724 , 34, UI_NETWK_TYPE_DCS_1800 , "CTBC Cellular", "CTBC"},
    {730 , 1 , UI_NETWK_TYPE_PCS_1900 , "ENTEL" , "Entel Telefonia Movil S.A"},
    {730 , 10 , UI_NETWK_TYPE_PCS_1900 , "ENTEL" , "Entel PCS Telecomunicaciones S.A"},
    {734 , 1 , UI_NETWK_TYPE_GSM_900 , "INFONT" , "Infonet, Redes De Informacion C.A"},
    {734 , 2 , UI_NETWK_TYPE_GSM_900 , "DIGITEL" , "Corporacion Digitel C.A"},
    {736 , 1 , UI_NETWK_TYPE_PCS_1900 , "NUEVATEL" , "Nuevatel PCS De Bolivia SA"},
    {736 , 2 , UI_NETWK_TYPE_PCS_1900 , "EMOVIL" , "Entel SA"},
    {744 , 1 , UI_NETWK_TYPE_GSM_900 , "HPGYSA" , "Hola Paraguay S.A"},
    {744 , 2 , UI_NETWK_TYPE_GSM_900 , "Porth" , "COPESA (Comunicaciones Personales S.A.)"},
    {746 , 1 , UI_NETWK_TYPE_GSM_900 , "ICMS" , "NV ICMS"},
    {901 , 5 , UI_NETWK_TYPE_GSM_SAT , "Thuraya" , "Thuraya Satellite Telecommunications Company"},
};

void BGEvtSms(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    ASSERT(wStrNum <= BG_STRING_ROW);

    if (wStrNum > BG_STRING_ROW) {

        return;
    }

    ::EnterCriticalSection(&g_BGSmsQueue.cs);
    if (g_BGSmsQueue.wRxCount < BG_SMS_ARRNUM) {
        g_BGSmsQueue.StrNum[g_BGSmsQueue.nWriteIndex] = wStrNum;
        for (WORD cnt = 0; cnt < wStrNum; cnt++)
            strncpy((char*)g_BGSmsQueue.StrArr[g_BGSmsQueue.nWriteIndex][cnt], (char*)strArr[cnt], DSAT_STRING_COL);
        g_BGSmsQueue.nWriteIndex++;
        if (g_BGSmsQueue.nWriteIndex >= BG_SMS_ARRNUM)
            g_BGSmsQueue.nWriteIndex = 0;
        g_BGSmsQueue.wRxCount++;
    }
    ::LeaveCriticalSection(&g_BGSmsQueue.cs);

    ::SetEvent(g_BGEvtArr[BGEVT_SMS]);
}

void BGEvtCall(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    ASSERT(wStrNum <= BG_STRING_ROW);

    if (wStrNum > BG_STRING_ROW)
        return;

    g_BGCallStrNum = 0;
    memset(g_BGCallStrArr, 0x00, sizeof(g_BGCallStrArr));

    g_BGCallStrNum = wStrNum;
    for (WORD cnt = 0; cnt < wStrNum; cnt++)
        strcpy((char*)g_BGCallStrArr[cnt], (char*)strArr[cnt]);

    ::SetEvent(g_BGEvtArr[BGEVT_CALL]);
}

void BGEvtClip(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    ASSERT(wStrNum <= BG_STRING_ROW);

    if (wStrNum > BG_STRING_ROW)
        return;

    g_BGClipStrNum = 0;
    memset(g_BGClipStrArr, 0x00, sizeof(g_BGClipStrArr));

    g_BGClipStrNum = wStrNum;
    for (WORD cnt = 0; cnt < wStrNum; cnt++)
        strcpy((char*)g_BGClipStrArr[cnt], (char*)strArr[cnt]);

    ::SetEvent(g_BGEvtArr[BGEVT_CLIP]);
}

//处理来电，来短信事件的后台线程
UINT BGThreadProc(LPVOID pParam)
{
    CHSDPADlg *pMainDlg = (CHSDPADlg*)pParam;
    ASSERT(pMainDlg);

    memset(&g_BGSmsQueue, 0x00, sizeof(StBGSmsQueue));
    ::InitializeCriticalSection(&g_BGSmsQueue.cs);

    g_BGEvtArr[BGEVT_SMS]   = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_BGEvtArr[BGEVT_CALL]  = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_BGEvtArr[BGEVT_CLIP]  = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_BGEvtArr[BGEVT_END]   = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_BGPassEvt = ::CreateEvent(NULL, TRUE, TRUE, NULL);
    g_BGReadNewSmsEvt = ::CreateEvent(NULL, FALSE, TRUE, NULL);

    DWORD dwEvent;
    while (true) {
        dwEvent = ::WaitForMultipleObjects(BGEVT_ARRNUM, g_BGEvtArr, FALSE, INFINITE);
        dwEvent -= WAIT_OBJECT_0;

        switch (dwEvent) {
        case BGEVT_SMS:
            //pMainDlg->m_bInComSms = TRUE;
            ::WaitForSingleObject(g_BGPassEvt, INFINITE);

            while (true) {
                g_BGSmsStrNum = 0;
                memset(g_BGSmsStrArr, 0x00, sizeof(g_BGSmsStrArr));

                ::EnterCriticalSection(&g_BGSmsQueue.cs);
                if (g_BGSmsQueue.wRxCount > 0) {
                    g_BGSmsStrNum = g_BGSmsQueue.StrNum[g_BGSmsQueue.nReadIndex];
                    for (WORD cnt = 0; cnt < g_BGSmsStrNum; cnt++)
                        strncpy((char*)g_BGSmsStrArr[cnt], (char*)g_BGSmsQueue.StrArr[g_BGSmsQueue.nReadIndex][cnt], DSAT_STRING_COL);
                    g_BGSmsQueue.nReadIndex++;
                    if (g_BGSmsQueue.nReadIndex >= BG_SMS_ARRNUM)
                        g_BGSmsQueue.nReadIndex = 0;
                    g_BGSmsQueue.wRxCount--;
                    ::LeaveCriticalSection(&g_BGSmsQueue.cs);
                    //Sleep(100);
                    (BGSmsResp.m_AtRespFunc)(BGSmsResp.m_pWnd, g_BGSmsStrArr, g_BGSmsStrNum);
                } else {
                    ::LeaveCriticalSection(&g_BGSmsQueue.cs);
                    break;
                }
            }
            break;
        case BGEVT_CALL:
            pMainDlg->m_bInComCall = TRUE;
            if (WAIT_OBJECT_0 == ::WaitForSingleObject(g_BGPassEvt, INFINITE))
                (BGCallResp.m_AtRespFunc)(BGCallResp.m_pWnd, g_BGCallStrArr, g_BGCallStrNum);
            break;
        case BGEVT_CLIP:
            pMainDlg->m_bInComCall = TRUE;
            if (WAIT_OBJECT_0 == ::WaitForSingleObject(g_BGPassEvt, INFINITE))
                (BGClipResp.m_AtRespFunc)(BGClipResp.m_pWnd, g_BGClipStrArr, g_BGClipStrNum);
            break;
        case BGEVT_END:
            for (BYTE i = BGEVT_SMS; i < BGEVT_ARRNUM; i++)
                ::CloseHandle(g_BGEvtArr[i]);
            ::CloseHandle(g_BGPassEvt);
            ::CloseHandle(g_BGReadNewSmsEvt);
            ::DeleteCriticalSection(&g_BGSmsQueue.cs);
            AfxEndThread(0);
            break;
        }
    }

    return 0;
}

//注册AT响应处理函数
void RegisterAtRespFunc(EnAtRespFuncType type, AtRespFunc func, LPVOID pWnd)
{
    ASSERT(type >= ATRESP_RING && type < ATRESP_MAX);
    ASSERT(func != NULL && pWnd != NULL);

    if (type == ATRESP_RING) {
        BGCallResp.m_AtRespFunc = func;
        BGCallResp.m_pWnd = pWnd;
        g_AtRespArr[type].m_AtRespFunc = BGEvtCall;
        g_AtRespArr[type].m_pWnd = NULL;
    } else if (type == ATRESP_CLIP) {
        BGClipResp.m_AtRespFunc = func;
        BGClipResp.m_pWnd = pWnd;
        g_AtRespArr[type].m_AtRespFunc = BGEvtClip;
        g_AtRespArr[type].m_pWnd = NULL;
    } else if (type == ATRESP_CMTI) {
        BGSmsResp.m_AtRespFunc = func;
        BGSmsResp.m_pWnd = pWnd;
        g_AtRespArr[type].m_AtRespFunc = BGEvtSms;
        g_AtRespArr[type].m_pWnd = NULL;
    } else {
        g_AtRespArr[type].m_AtRespFunc = func;
        g_AtRespArr[type].m_pWnd = pWnd;
    }

    if (type == ATRESP_GENERAL_AT)
        ::SetEvent(g_AtRegEvt);
}

//注销AT响应处理函数
void DeRegisterAtRespFunc(EnAtRespFuncType type)
{
    ASSERT(type >= ATRESP_RING && type <= ATRESP_MAX);

    if (type == ATRESP_GENERAL_AT)
        ::ResetEvent(g_AtRegEvt);

    if (type == ATRESP_MAX)
        memset(g_AtRespArr, 0, sizeof(g_AtRespArr));
    else
        memset(&g_AtRespArr[type], 0, sizeof(StAtResp));
}

//AT超时通知函数
void AtTimeoutNotify()
{
    ::SetEvent(g_AtTimeout);
    AfxMessageBox(IDS_USB_PORT_TIMEOUT);
}

CHSDPADlg *pDsMainDlg = NULL;

//执行AT响应处理函数
static void CallAtRespFunc(EnAtRespFuncType type)
{
    ASSERT(type >= ATRESP_RING && type < ATRESP_MAX);

    if (type == ATRESP_GENERAL_AT) {
        if (WAIT_OBJECT_0 != ::WaitForSingleObject(g_AtRegEvt, 30000)) {
            ASSERT(FALSE);
        }
    }


//    ASSERT(g_AtRespArr[type].m_AtRespFunc != NULL && g_AtRespArr[type].m_pWnd != NULL);
    ASSERT(g_AtRespArr[type].m_AtRespFunc != NULL);

    if (g_AtRespArr[type].m_AtRespFunc == NULL)
        return;

    if (type != ATRESP_GENERAL_AT) {
        //下位机在上位机初始化阶段不主动发送RSSI,NETWORK,ROAM变化通知
#if 0
        if (type == ATRESP_RSSI || type == ATRESP_NWCHG || type == ATRESP_ROAMCHG
                || type == ATRESP_RING || type == ATRESP_CMTI) {
            if (pDsMainDlg) {
                BOOL bSyncInitMask;
                EnterCriticalSection(&pDsMainDlg->m_csSyncInitMask);
                bSyncInitMask = pDsMainDlg->m_bSyncInitMask;
                LeaveCriticalSection(&pDsMainDlg->m_csSyncInitMask);
                if (bSyncInitMask)
                    return;
            }
        }
#endif

        (*g_AtRespArr[type].m_AtRespFunc)(g_AtRespArr[type].m_pWnd, g_DsatStrArr, g_DsatStrNum);
    } else {
        StAtResp lastAtResp = g_AtRespArr[type];
        DeRegisterAtRespFunc(type);
        (*lastAtResp.m_AtRespFunc)(lastAtResp.m_pWnd, g_DsatStrArr, g_DsatStrNum);
    }
}

//判断是否找到了AT响应的结果码
static EnDsatResCode FindEndCode(const char *str)
{
    ASSERT(str != NULL);

    int i;
    EnDsatResCode begCode;
    EnDsatResCode divCode;


    begCode = DSAT_OK;
    divCode = DSAT_CME_ERROR;

    for (i = begCode; i < divCode; i++) {
        if (strcmp(str, gc_dsatResCodeTbl[i][gc_dsatmode]) == 0)
            return (EnDsatResCode)(i);
    }
    for (; i < DSAT_MAX; i++) {
        if (memcmp(str, gc_dsatResCodeTbl[i][gc_dsatmode], strlen(gc_dsatResCodeTbl[i][gc_dsatmode])) == 0)
            return (EnDsatResCode)(i);
    }

    return DSAT_MAX;
}

//重置解析缓冲和相关变量
static void ResetParseData()
{
    memset(g_DsatStrArr, 0, sizeof(g_DsatStrArr));
    g_DsatStrNum = 0;
    g_DsatPtr = &g_DsatStrArr[0][0];
    g_DsatState = STATE_START;
    g_DsatResCode = DSAT_MAX;
}

//AT响应解析函数
static void AtRespParse(CSerialPort *pComm)
{
    ASSERT(pComm != NULL);

#ifdef FEATURE_ATTEST_SUPPORT
    //输出，in
    CStdioFile file;
    if (file.Open("AtDebug.log", CFile::modeReadWrite)) {
        CString str;
        str.Format("%s", (char *)&pComm->m_RxQueueCtrl.RxBuffer[0]);
        str.Insert(str.GetLength(), "\n");
        DWORD dwActual = file.SeekToEnd();
        file.WriteString(str);
        file.Close();
    }
#endif

    BYTE ch = 0;
    while (true) {
        EnterCriticalSection(&pComm->m_csRxQueue);
        if (pComm->m_RxQueueCtrl.wRxCount > 0) {
            ch = *pComm->m_RxQueueCtrl.pRead++;
            if (pComm->m_RxQueueCtrl.pRead >= &pComm->m_RxQueueCtrl.RxBuffer[SERIAL_RXBUFFERSIZE])
                pComm->m_RxQueueCtrl.pRead = &pComm->m_RxQueueCtrl.RxBuffer[0];
            pComm->m_RxQueueCtrl.wRxCount--;
            LeaveCriticalSection(&pComm->m_csRxQueue);
        } else {
            LeaveCriticalSection(&pComm->m_csRxQueue);
            break;
        }

        ASSERT(g_DsatStrNum <= DSAT_STRING_ROW);

        switch (g_DsatState) {
        case STATE_START:
            if (ch == AT_FLAG_S3) {
                if ((g_DsatResCode == DSAT_CMT)
#ifndef FEATURE_SMS_PDUMODE
                        //becase spreadtrum send double 0d0a before sms pdu which
// followed  by +CMT: 24, remove the expression for check  the 0d0a again
                        || (g_DsatResCode == DSAT_CDS)
#endif
                   )
                    g_DsatState = STATE_RESCODE_S3;
                else
                    g_DsatState = STATE_HEAD_S3;
            } else if (g_DsatStrNum > 0) {
                *g_DsatPtr++ = ch;
                g_DsatState = STATE_FIND_CONTENT;
            }
            break;
        case STATE_HEAD_S3:
            if (ch == AT_FLAG_S4)
                g_DsatState = STATE_HEAD_S4;
            else if (ch != AT_FLAG_S3) //error
                g_DsatState = STATE_START;
            break;
        case STATE_HEAD_S4:
            if (ch == AT_FLAG_MR) {
                *g_DsatPtr++ = ch;
                g_DsatStrNum++;
                g_DsatState = STATE_FIND_MR;
            } else if (ch != AT_FLAG_S3) {
                *g_DsatPtr++ = ch;
                g_DsatState = STATE_FIND_CONTENT;
            } else {
                //g_DsatState = STATE_TAIL_S3;
                g_DsatState = STATE_HEAD_S3;
            }
            break;
        case STATE_FIND_CONTENT:
            if (ch != AT_FLAG_S3) {
                *g_DsatPtr++ = ch;
            } else {
                if ((g_DsatResCode != DSAT_CMT) && (g_DsatResCode != DSAT_CDS)) {
                    g_DsatResCode = FindEndCode((char *)g_DsatStrArr[g_DsatStrNum++]);
                    if (g_DsatResCode == DSAT_MAX || g_DsatResCode == DSAT_CMT  || g_DsatResCode == DSAT_CDS) {
                        g_DsatState = STATE_TAIL_S3;
                    } else
                        g_DsatState = STATE_RESCODE_S3;
                    g_DsatPtr = &g_DsatStrArr[g_DsatStrNum][0];
                } else {
                    g_DsatStrNum++;
                    g_DsatState = STATE_RESCODE_S3;
                }
            }
            break;
        case STATE_TAIL_S3:
            if (ch == AT_FLAG_S4) {
                g_DsatState = STATE_START;
                /*if (g_DsatResCode == DSAT_MAX) {
                    //ignore unsupported AT command response ,espally unsolicated at command
                    memset(&g_DsatStrArr[--g_DsatStrNum][0], 0, (DSAT_STRING_COL));
                    g_DsatPtr = &g_DsatStrArr[g_DsatStrNum][0];
                }*/
            } else if (ch != AT_FLAG_S3) { //error
                memset(&g_DsatStrArr[--g_DsatStrNum][0], 0, (DSAT_STRING_COL));
                g_DsatPtr = &g_DsatStrArr[g_DsatStrNum][0];
                g_DsatResCode = DSAT_MAX;
                g_DsatState = STATE_START;
            }
            break;
        case STATE_RESCODE_S3:
            if (ch == AT_FLAG_S4) {
                g_DsatState = STATE_END;
            } else if (ch != AT_FLAG_S3) { //error
                memset(&g_DsatStrArr[--g_DsatStrNum][0], 0, (DSAT_STRING_COL));
                g_DsatPtr = &g_DsatStrArr[g_DsatStrNum][0];
                g_DsatResCode = DSAT_MAX;
                g_DsatState = STATE_START;
            }
            break;
        case STATE_FIND_MR:
            if (ch == AT_FLAG_SP) {
                *g_DsatPtr++ = ch;
                g_DsatResCode = DSAT_OK;
                g_DsatState = STATE_END;
            } else if (ch != AT_FLAG_MR) { //error
                memset(&g_DsatStrArr[--g_DsatStrNum][0], 0, (DSAT_STRING_COL));
                g_DsatPtr = &g_DsatStrArr[g_DsatStrNum][0];
                g_DsatResCode = DSAT_MAX;
                g_DsatState = STATE_START;
            }
            break;
        }

        //解析到一条完整的AT命令响应，调用AT处理回调函数
        if (g_DsatState == STATE_END && g_DsatResCode != DSAT_MAX) {
            if (g_DsatResCode == DSAT_RING) {
                if (g_SetData.Main_nCall)
                    CallAtRespFunc(ATRESP_RING);
            } else if (g_DsatResCode == DSAT_NO_CARRIER) {
                CallAtRespFunc(ATRESP_NO_CARRIER);
            } else if (g_DsatResCode == DSAT_DIALTONE) {
                if (g_SetData.Main_nCall)
                    CallAtRespFunc(ATRESP_DIALTONE);
            } else if (g_DsatResCode == DSAT_CMTI) {
                CallAtRespFunc(ATRESP_CMTI);
            } else if (g_DsatResCode == DSAT_CMT) {
                CallAtRespFunc(ATRESP_CMT);
            } else if (g_DsatResCode == DSAT_RVMFB) { //TATA 需求 add by liub
                CallAtRespFunc(ATRESP_RVMFB);
            } else if (g_DsatResCode == DSAT_RVMFBUPDATE) { //TATA 需求 add by liub
                CallAtRespFunc(ATRESP_RVMFBUPDATA);
            } else if (g_DsatResCode == DSAT_RSSI) {
                CallAtRespFunc(ATRESP_RSSI);
            } else if (g_DsatResCode == DSAT_SPREADY) {
                CallAtRespFunc(ATRESP_SPREADY);
            } else if (g_DsatResCode == DSAT_CLIP) {
                if (g_SetData.Main_nCall)
                    CallAtRespFunc(ATRESP_CLIP);
            } else if (g_DsatResCode == DSAT_NWCHG) {
                CallAtRespFunc(ATRESP_NWCHG);
            } else if (g_DsatResCode == DSAT_NWSRVCHG) {
                CallAtRespFunc(ATRESP_NWSRVCHG);
            } else if (g_DsatResCode == DSAT_ROAMCHG) {
                CallAtRespFunc(ATRESP_ROAMCHG);
            } else if (g_DsatResCode == DSAT_HANDSET) {
                CallAtRespFunc(ATRESP_HANDSETCHG);
            } else if (g_DsatResCode == DSAT_SIMREADY) {
                CallAtRespFunc(ATRESP_SIMREADY);
            } else if (g_DsatResCode == DSAT_PLMNCHG) {
                CallAtRespFunc(ATRESP_PLMNCHG);
            } else if (g_DsatResCode == DSAT_SRVSTATUSCHG) {
                CallAtRespFunc(ATRESP_SRVSTATUSCHG);
            } else if (g_DsatResCode == DSAT_CDS) {
                CallAtRespFunc(ATRESP_CDS);
            } else if (g_DsatResCode == DSAT_CUSD) {
                CallAtRespFunc(ATRESP_CUSD);
            } else if (g_DsatResCode == DSAT_PS) {
                CallAtRespFunc(ATRESP_PS);
            } else if (g_DsatResCode == DSAT_HVPRIV) {
                CallAtRespFunc(ATRESP_HVPRIV);
            } else if (g_DsatResCode == DSAT_SIDLOCK) {
                CallAtRespFunc(ATRESP_SIDLOCK);
            } else if (g_DsatResCode == DSAT_SIND) {
                CallAtRespFunc(ATRESP_SIND);
            } else if (g_DsatResCode == DSAT_MODE) {
                CallAtRespFunc(ATRESP_MODE);
            } else if (g_DsatResCode == DSAT_SYSINFO) {
                CallAtRespFunc(ATRESP_SYSINFO);
            } else if (g_DsatResCode == DSAT_CREG || g_DsatResCode ==
                       DSAT_CGREG) {
                CallAtRespFunc(ATRESP_CREG);
            } else if(g_DsatResCode == DSAT_ECIND) {
                CallAtRespFunc(ATRESP_ECIND);
            }

            else if (g_DsatResCode == DSAT_OK || g_DsatResCode == DSAT_ERROR
                     || g_DsatResCode == DSAT_CME_ERROR || g_DsatResCode == DSAT_CMS_ERROR) {

                pComm->SetSerialState(SERIAL_STATE_CMD);
                CallAtRespFunc(ATRESP_GENERAL_AT);
            }
            ResetParseData();
        }
    } //while
}

//普通AT命令超时处理
static void AtTimeoutProc(CSerialPort *pComm)
{
    ASSERT(pComm != NULL);

    pComm->Purge(PURGE_RXCLEAR);

    ::EnterCriticalSection(&pComm->m_csRxQueue);
    memset(&pComm->m_RxQueueCtrl, 0x00, sizeof(StRxQueueCtrl));
    ::LeaveCriticalSection(&pComm->m_csRxQueue);

    ResetParseData();

    DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
    pComm->SetSerialState(SERIAL_STATE_CMD);
}

void DsEventCreate()
{
    g_AtRespEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_EndEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);


    g_AtRegEvt = ::CreateEvent(NULL, TRUE, FALSE, NULL);


    g_AppRegEvt = ::CreateEvent(NULL, TRUE, FALSE, NULL);

    g_AtTimeout = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    g_DsEventArr[DSEVENT_ATRESP] = g_AtRespEvent;
    g_DsEventArr[DSEVENT_ATTIMEOUT] = g_AtTimeout;
    g_DsEventArr[DSEVENT_END] = g_EndEvent;
}

void DsEventDestroy()
{
    for (BYTE i = DSEVENT_ATRESP; i < DSEVENT_ARRNUM; i++)
        ::CloseHandle(g_DsEventArr[i]);
    ::CloseHandle(g_AtRegEvt);
    ::CloseHandle(g_AppRegEvt);
}

//Ds任务
UINT DsThreadProc(LPVOID pParam)
{
    CSerialPort *pComm = (CSerialPort*)pParam;

    ResetParseData();
    DeRegisterAtRespFunc(ATRESP_MAX);

    ResetSmsRcvConcBuf(SMS_RCVCONCBUF_MAX);

    ::WaitForSingleObject(g_AppRegEvt, INFINITE);

    DWORD dwEvent = 0;
    while (true) {
        dwEvent = ::WaitForMultipleObjects(DSEVENT_ARRNUM, g_DsEventArr, FALSE, INFINITE);
        dwEvent -= WAIT_OBJECT_0;

        switch (dwEvent) {
        case DSEVENT_ATRESP:
            AtRespParse(pComm);
            break;
        case DSEVENT_ATTIMEOUT:
            AtTimeoutProc(pComm);
            break;
        case DSEVENT_END:
            DsEventDestroy();
            AfxEndThread(0);
            break;
        }
    }

    return 0;
}

//Sms At related variables and functions
EnSmsState GetSmsStateFromStr(LPCSTR str)
{
    BYTE cnt;
    for (cnt = 0; cnt < SMS_STATE_MAX; cnt++) {
        if (strcmp(str, gcstrSmsState[cnt][gSmsMode]) == 0)
            break;
    }
    return (EnSmsState)cnt;
}

LPCSTR GetSmsStrFromState(EnSmsState state)
{
    ASSERT(state >= SMS_STATE_MT_NOT_READ && state < SMS_STATE_MAX);
    return (gcstrSmsState[state][gSmsMode]);
}

//for gsm/wcdma format
//"年/月/日,时:分:秒" //"6/3/20,13:22:34" -cdma
//"06/05/23,09:19:55+00" -gsm/wcdma
BOOL GetTimeFromStr(LPCSTR str, COleDateTime &time)
{
    int Date[3];
    int Time[3];
    int Zone = 0;
    char *pDate, *pTime, *pZone = 0;
    char *p, *p1, *p2;
    char WorkStr[30] = {0};

    strncpy(WorkStr, str, sizeof(WorkStr));

    pDate = WorkStr;
    pTime = NULL;
    p = WorkStr;

    while (*p) {
        if (*p == ',') {
            *p++ = 0;
            pTime = p;
            break;
        } else if (*p == '+') {
            *p++ = 0;
            pZone = p;
            Zone = atoi(pZone);
            break;
        }
        p++;
    }

    if (pTime == NULL || *pTime == 0)
        return FALSE;

    int cnt = 0;
    p1 = p2 = pDate;
    while (cnt < 3) {
        if (*p2 == '/') {
            *p2++ = 0;
            Date[cnt++] = atoi(p1);
            p1 = p2;
        } else if (*p2 == 0) {
            Date[cnt++] = atoi(p1);
            break;
        } else
            p2++;
    }

    if (cnt != 3)
        return FALSE;

    Date[0] += 2000;

    cnt = 0;
    p1 = p2 = pTime;
    while (cnt < 3) {
        if (*p2 == ':') {
            *p2++ = 0;
            Time[cnt++] = atoi(p1);
            p1 = p2;
        } else if (*p2 == 0) {
            Time[cnt++] = atoi(p1);
            break;
        } else
            p2++;
    }

    if (cnt != 3)
        return FALSE;

    if ((Date[1] >= 1 && Date[1] <= 12) && (Date[2] >= 1 && Date[2] <= 31)
            && (Time[0] >= 0 && Time[0] <= 23) && (Time[1] >= 0 && Time[1] <= 59) && (Time[2] >= 0 && Time[2] <= 59)) {
        time = COleDateTime(Date[0], Date[1], Date[2], Time[0], Time[1], Time[2]);
        return TRUE;
    } else
        return FALSE;
}

BOOL IsValidCallNumChar(UINT nChar)
{
    if ((nChar >= '0' && nChar <= '9')
            || nChar == 8 || nChar == '*'
            || nChar == '#' || nChar == '+')
        return TRUE;
    else
        return FALSE;
}

BOOL IsValidSmsNumChar(UINT nChar)
{
    if ((nChar >= '0' && nChar <= '9')
            || nChar == 8 || nChar == 22 || nChar == 3 || nChar == ';' || nChar == '+')
        return TRUE;
    else
        return FALSE;
}

//GB转UCS2处理函数
CString GBToUCS2(const char *strGb)
{
    ASSERT(strGb);

    CString strUcs2;
    CString strTmp;
    wchar_t* wszUnicode = NULL;    //Unicode编码的字符
    int iLen = 0;                  //需要转换的字符数

    if (strlen(strGb) == 0)
        return strUcs2;

    //计算转换的字符数
    iLen = MultiByteToWideChar(CP_ACP, 0, strGb, -1, NULL, 0);

    //给wszUnicode分配内存
    wszUnicode = new wchar_t[iLen+1];

    //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
    MultiByteToWideChar(CP_ACP, 0, strGb, -1, wszUnicode, iLen);

    for (int i = 0; i < iLen - 1; i++) {
        strTmp.Format(_T("%04X"), wszUnicode[i]);
        strUcs2 += strTmp;
    }

    //释放内存
    delete []wszUnicode;
    return strUcs2;
}

//GB转UCS2处理函数
CString BTToUCS2(const TCHAR *strGb)
{
    ASSERT(strGb);

    CString strUcs2;
    CString strTmp;
    // wchar_t* wszUnicode = NULL;    //Unicode编码的字符
    int iLen = 0;                  //需要转换的字符数

    if (wcslen(strGb) == 0)
        return strUcs2;

    //计算转换的字符数
    //  iLen = MultiByteToWideChar(CP_ACP, 0, strGb, -1, NULL, 0);
    iLen = wcslen(strGb);
    //给wszUnicode分配内存
    //  wszUnicode = new wchar_t[iLen+1];



    //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
    // MultiByteToWideChar(CP_ACP, 0, strGb, -1, wszUnicode, iLen);

    for (int i = 0; i < iLen; i++) {
        strTmp.Format(_T("%04X"), strGb[i]);
        strUcs2 += strTmp;
    }

    //释放内存
    // delete []wszUnicode;
    return strUcs2;
}

//UCS2转UCS2处理函数
int  UCS2ToUCS2(CString strGb, CString strUcs2)
{
    ASSERT(strGb);

    CString strTmp;
    // wchar_t* wszUnicode = NULL;    //Unicode编码的字符
    int iLen = 0;                  //需要转换的字符数

    if (wcslen(strGb) == 0)
        return 0;

    iLen = wcslen(strGb);

    wchar_t* wszUnicode = new wchar_t[iLen*2+1];
    memset(wszUnicode, 0, iLen * 2 + 1);


    //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
    // MultiByteToWideChar(CP_ACP, 0, strGb, -1, wszUnicode, iLen);
    int j = 0;
    for (int i = 0; i < iLen ;) {
        wszUnicode[j] = (strGb[i] >> 8) & 0x00FF;
        wszUnicode[j + 1] = strGb[i] & 0x00FF;
        j += 2;
        i += 1;

    }

    for (int num = 0 ; num < j ; num++) {

        strTmp.Format(_T("%x"), wszUnicode[num]);
        strUcs2 += strTmp;
    }

    //释放内存
    delete []wszUnicode;
    return j;
}
//UCS2转GB处理函数
CString UCS2ToGB(const CString &strUcs2)
{
    CString strGb;
    wchar_t* wszUnicode = NULL; //Unicode编码的字符
    char* pszGb = NULL; //Gb编码的繁体字符
    int iLen = 0; //需要转换的字符数

    iLen = strUcs2.GetLength() / 4;
    wszUnicode = new wchar_t[iLen+1];
    CString str;
    wchar_t wData;

    for (int i = 0; i < iLen; i++) {
        str = strUcs2.Mid(i * 4, 4);
        swscanf(str, _T("%x"), &wData);
        wszUnicode[i] = wData;
    }

    wszUnicode[i] = 0;

    //计算转换的字符数
    iLen = WideCharToMultiByte(CP_ACP, 0, (PWSTR) wszUnicode, -1, NULL, 0, NULL, NULL);

    //给pszGbt分配内存
    pszGb = new char[iLen+1];

    //转换Unicode码到Gb码繁体，使用API函数WideCharToMultiByte
    WideCharToMultiByte(CP_ACP, 0, (PWSTR) wszUnicode, -1, pszGb, iLen, NULL, NULL);

    strGb = pszGb;

    //释放内存
    delete [] wszUnicode;
    delete [] pszGb;

    return strGb;
}

void ASCToUCS2(const char* unicode, TCHAR* WChar)
{

    int len = strlen(unicode);
    int j = 0;
    for (int i = 0; i < len;) {
        WChar[j] = unicode[i];
        WChar[j] = WChar[j] << 8;
        WChar[j] = WChar[j] | (unicode[i + 1] & 0x00FF);
        i += 2;
        j += 1;
        if (unicode[i] == 0x00 && unicode[i + 1 ] == 0x00)
            break;
    }
    WChar[j] = 0;
}

BOOL ASCHEXToWchar(const char* ascii, TCHAR* WChar)
{
    int len = strlen(ascii);
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (i % 2 == 0) {
            WChar[j++] = '0';
            WChar[j++] = '0';
        }
        WChar[j++] = ascii[i];
    }

    return TRUE;
}

int WCharToUnicode(const TCHAR *WChar, char* unicode)
{
    int j = 0;
    if (WChar == NULL) {
        unicode[0] = '\0';
        return j;
    }
    int len = _tcslen(WChar);
    for (int i = 0; i < len; i ++) {
        unicode[j] = (char)(WChar[i] >> 8);
        unicode[j + 1] = (char)(WChar[i] & 0x00FF);
        if (unicode[j] == 0x00 && unicode[j + 1] == 0x00)
            break;
        j += 2;
    }
    return j;
}

int WCharToChar(const TCHAR *wchar, char* cstr)
{
    char unicode[2];
    if (wchar == NULL) {
        memset(cstr, 0, strlen(cstr));
        return 0;
    }
    int len = _tcslen(wchar);
    for (int i = 0; i < len; i ++) {
        unicode[0] = (char)(wchar[i] & 0x00FF);
        unicode[1] = (char)(wchar[i] >> 8);
        if (unicode[0] == 0x00 && unicode[1] == 0x00) {
            cstr[i] = 0x00;
            break;
        }
        if (unicode[1] == 0x00)
            cstr[i] = unicode[0];
    }
    return len;
}
BOOL IsGsm7bitChar(const wchar_t uc)
{
    int i, j;

    for (i = 0; i < GSM0338_EXTENSIONS; i++)
        for (j = 0; j < GSM0338_ALPHABET_SIZE; j++) {
            if (uc == GSM0338ToUCS2Array[i][j])
                return TRUE;
        }
    return FALSE;
}
BOOL UnicodeIsHighStr(TCHAR *wchar)
{
    BOOL result = FALSE;
    TCHAR wstr;
    char str;
    INT len = _tcslen(wchar);
    for (int i = 0; i < len; i ++) {
        wstr = wchar[i];
        str = (char)(wstr >> 8);
        if (str != 0x00) {
            result = TRUE;
            break;
        }
    }
    return result;
}
BOOL CStringisHighStr(CString str)
{
    BOOL result;
    LPTSTR lptstr = str.GetBuffer(str.GetLength());
    result = UnicodeIsHighStr(lptstr);
    str.ReleaseBuffer();
    return result;
}

char * strtrim(char *p)
{
    int len = 0;
    if (*p == NULL) {
        return NULL;
    }

    while (*p != '\0') {
        if (*p != ' ' && *p != '\t') {
            break;
        }
        p++;
    }

    len = strlen(p);
    while (len--) {
        if (*(p + len) != ' ' && *(p + len) != '\t') {
            break;
        }
        *(p + len) = '\0';
    }
    return p;
}

//判断文本中的字符是否全部在GSM-7bit编码表中
BOOL IsAlphabetUnicode(const TCHAR *str)
{
    ASSERT(str);

    wchar_t* wszUnicode = NULL;
    int iLen = 0;
    int i = 0;

    if (wcslen(str) == 0)
        return TRUE;

    //计算转换的字符数
    //iLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    iLen = wcslen(str);

    //给wszUnicode分配内存
    wszUnicode = new wchar_t[iLen+1];
    wcscpy(wszUnicode, str);

    //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
    //MultiByteToWideChar(CP_ACP, 0, str, -1, wszUnicode, iLen);

    for (i = 0; i < iLen; i++) {
        if (!IsGsm7bitChar(wszUnicode[i])) {
            delete []wszUnicode;               //wyw_0326 add

            return FALSE;
        }
    }

    //释放内存
    delete []wszUnicode;
    return TRUE;
}
BOOL IsAlphabet(const char *str)
{
    ASSERT(str);

    wchar_t* wszUnicode = NULL;
    int iLen = 0;
    int i = 0;

    if (strlen(str) == 0)
        return TRUE;

    //计算转换的字符数
    iLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);

    //给wszUnicode分配内存
    wszUnicode = new wchar_t[iLen+1];

    //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
    MultiByteToWideChar(CP_ACP, 0, str, -1, wszUnicode, iLen);

    for (i = 0; i < iLen - 1; i++) {
        if (!IsGsm7bitChar(wszUnicode[i]))
            return FALSE;
    }

    //释放内存
    delete []wszUnicode;
    return TRUE;
}

//获得双字节字符集文本串的字符个数
USHORT GetUnicodeCharNum(const TCHAR *str)
{
    USHORT len;
    len = wcslen(str);
    /*  num = 0;
      char * strTemp;
    strTemp=new char[len*2];
    WCharToUnicode(str, strTemp);
      for(i=0;i<len*2;i++)
      {
          num++;


          if(IsDBCSLeadByte((BYTE)strTemp[i]))
              i++;
      }
    delete []strTemp;*/
    return len;
}


USHORT GetACSIICharNum(const TCHAR *str)
{
    USHORT len, num, i;
    len = wcslen(str);
    num = 0;
    char * strTemp;
    strTemp = new char[len];
    WCharToChar(str, strTemp);
    for (i = 0; i < len; i++) {
        num++;
        if (IsDBCSLeadByte((BYTE)strTemp[i]))
            i++;
    }
    delete []strTemp;
    return num;
}

void AtRespDummy(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    return;
}

#ifdef FEATURE_SMS_PDUMODE
#define SMS_MASK_MTI    0X03
#define SMS_MASK_MTI_DT 0X00
#define SMS_MASK_MTI_SP 0X02
#define SMS_MASK_MTI_MO 0X01
#define SMS_MASK_UDHI   0X40
#define SMS_MASK_SRR    0X20
#define SMS_MASK_SRI    0X20
#define SMS_MASK_VP     0X18
#define SMS_MASK_VP_NP  0X00    //VP not present
#define SMS_MASK_VP_RF  0X10    //vp relative format
#define SMS_MASK_VP_EF  0X08    //VP enhance format
#define SMS_MASK_VP_AF  0X18    //VP absolute format


void DecodeFOFormSmsPDU(StSmsRecord *pRecord,  char *fo)
{
    USHORT ifo = 0;
    if(fo == NULL)
        return;

    ifo = strtol(fo, NULL, 16);

    if(ifo & SMS_MASK_UDHI) {
        pRecord->flag |= SMS_RECORD_FLAG_CONCATENATE_SEGE;
    }
    pRecord->tp_mti = ifo & SMS_MASK_MTI;
}

int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int nSrc;        // 源字符串的计数值
    int nDst;        // 目标编码串的计数值
    int nChar;       // 当前正在处理的组内字符字节的序号，范围是0-7
    unsigned char nLeft;    // 上一字节残余的数据
    // 计数值初始化
    nSrc = 0;
    nDst = 0;
    // 将源串每8个字节分为一组，压缩成7个字节
    // 循环该处理过程，直至源串被处理完
    // 如果分组不到8字节，也能正确处理
    while (nSrc <= nSrcLength) {
        // 取源字符串的计数值的最低3位
        nChar = nSrc & 7;
        // 处理源串的每个字节
        if (nChar == 0) {
            // 组内第一个字节，只是保存起来，待处理下一个字节时使用
            nLeft = *pSrc;
        } else {
            unsigned char cChar[3] = {0};
            unsigned char iChar;
            // 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节
            iChar = (*pSrc << (8 - nChar)) | nLeft;
            sprintf((char *)cChar,"%02X", iChar);
            strcat((char *)pDst, (char *)cChar);
            // 将该字节剩下的左边部分，作为残余数据保存起来
            nLeft = *pSrc >> nChar;
            // 修改目标串的指针和计数值
            pDst+=2;
            nDst++;
        }
        // 修改源串的指针和计数值
        pSrc++;
        nSrc++;
    }
    // 返回目标串长度
    return nDst;
}

// 7-bit解码
// pSrc: 源编码串指针
// pDst: 目标字符串指针
// nSrcLength: 源编码串长度
// 返回: 目标字符串长度

int gsmDecode7bit(const char* pSrc, char* pDst, int nSrcLength)
{
    int Data, cnt;
    char str[3] = {0};
    unsigned char *ascSrc = new unsigned char[nSrcLength];
    int nSrc;        // 源字符串的计数值
    int nDst;        // 目标解码串的计数值
    int nByte;       // 当前正在处理的组内字节的序号，范围是0-6
    unsigned char nLeft;    // 上一字节残余的数据
    // 计数值初始化
    nSrc = 0;
    nDst = 0;
    // 组内字节序号和残余数据初始化
    nByte = 0;
    nLeft = 0;
    for(cnt = 0; cnt < nSrcLength/2; cnt++) {
        strncpy(str, pSrc+2*cnt, 2);
        sscanf(str, "%x", &Data);
        ascSrc[cnt] = Data;
    }
    ascSrc[cnt] = 0;
    // 将源数据每7个字节分为一组，解压缩成8个字节
    // 循环该处理过程，直至源数据被处理完
    // 如果分组不到7字节，也能正确处理
    while (nSrc < cnt) {
        // 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
        *pDst = ((*ascSrc << nByte) | nLeft) & 0x7f;
        // 将该字节剩下的左边部分，作为残余数据保存起来
        nLeft = *ascSrc >> (7 - nByte);
        // 修改目标串的指针和计数值
        pDst++;
        nDst++;
        // 修改字节计数值
        nByte++;
        // 到了一组的最后一个字节
        if (nByte == 7) {
            // 额外得到一个目标解码字节
            *pDst = nLeft;
            // 修改目标串的指针和计数值
            pDst++;
            nDst++;
            // 组内字节序号和残余数据初始化
            nByte = 0;
            nLeft = 0;
        }
        // 修改源串的指针和计数值
        ascSrc++;
        nSrc++;
    }
    *pDst = 0;
    // 返回目标串长度
    //delete [] ascSrc;         //release ascSrc will occur fatal error!
    return nDst;
}

//对fo后的pdu进行解析
void DecodeDeliverySms(const char *pdu,   StSmsRecord *pRecord)
{
    ASSERT(pdu != NULL && strlen(pdu) > 0);
    ASSERT(pRecord != NULL);

    USHORT numlen = 0;
    USHORT contentlen = 0;
    BYTE pid = 0;
    BYTE code = 0;
    char buf[20];
    char pdunum[50];
    char pdutime[30];
    char pduContent[500];
    const char *p = pdu;

    memset(pdunum, 0x00, sizeof(pdunum));
    memset(pdutime, 0x00, sizeof(pdutime));
    memset(pduContent, 0x00, sizeof(pduContent));

    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    numlen = strtol(buf, NULL, 16);

    p += 2;
    strncpy(pdunum, p, numlen + 2 +numlen%2);	//奇数时加F补足
    DecodeNumFormSmsPDU(pdunum, (char*)pRecord->szNumber);

    p += (numlen + 2 + numlen%2);
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    pid = strtol(buf, NULL, 16);

    p += 2;
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    code = strtol(buf, NULL, 16);

    p += 2;
    strncpy(pdutime, p, 14);
    DecodeTimeFormSmsPDU(pdutime, &pRecord->timestamp);

    p += 14;
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    contentlen = strtol(buf, NULL, 16);

    p += 2;         //skip content len
    if(pRecord->flag & SMS_RECORD_FLAG_CONCATENATE_SEGE) {
        memset(buf, 0x00, sizeof(buf));
        strncpy(buf, p, 14);
        int udhl = DecodeUDHFormSmsPDU(pRecord, buf);
        p += udhl * 2;

        if(udhl == 0) {
            pRecord->flag &= ~SMS_RECORD_FLAG_CONCATENATE_SEGE;
        }
    }

    strncpy(pduContent, p, contentlen * 2);
    DecodeContentFromSmsPDU(pduContent, code, (char*)pRecord->szContent);
}

//对fo后的pdu进行解析
void DecodeSubmitSms(const char *pdu,   StSmsRecord *pRecord)
{
    ASSERT(pdu != NULL && strlen(pdu) > 0);
    ASSERT(pRecord != NULL);

    USHORT numlen = 0;
    USHORT contentlen = 0;
    BYTE pid = 0;
    BYTE code = 0;
    char buf[20];
    char pdunum[50];
    char pdutime[30];
    char pduContent[500];
    const char *p = pdu;

    memset(pdunum, 0x00, sizeof(pdunum));
    memset(pdutime, 0x00, sizeof(pdutime));
    memset(pduContent, 0x00, sizeof(pduContent));

    p += 2; //skip tp-mr
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    numlen = strtol(buf, NULL, 16);

    p += 2;
    strncpy(pdunum, p, numlen + 2 +numlen%2);	//奇数时加F补足
    DecodeNumFormSmsPDU(pdunum, (char*)pRecord->szNumber);

    p += (numlen + 2 + numlen%2);
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    pid = strtol(buf, NULL, 16);

    p += 2;
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    code = strtol(buf, NULL, 16);

    p += 2;     //skip tp-vp

    p += 2;
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    contentlen = strtol(buf, NULL, 16);

    p += 2;         //skip content len
    if(pRecord->flag & SMS_RECORD_FLAG_CONCATENATE_SEGE) {
        memset(buf, 0x00, sizeof(buf));
        strncpy(buf, p, 14);
        int udhl = DecodeUDHFormSmsPDU(pRecord, buf);
        p += udhl * 2;

        if(udhl == 0) {
            pRecord->flag &= ~SMS_RECORD_FLAG_CONCATENATE_SEGE;
        }
    }

    strncpy(pduContent, p, contentlen * 2);
    DecodeContentFromSmsPDU(pduContent, code, (char*)pRecord->szContent);
}

//对fo后的pdu进行解析
void DecodeSReportSms(const char *pdu,   StSmsRecord *pRecord)
{
    ASSERT(pdu != NULL && strlen(pdu) > 0);
    ASSERT(pRecord != NULL);

    USHORT numlen = 0;
    USHORT status = 0;
    BYTE pid = 0;
    BYTE code = 0;
    char buf[20];
    char pdunum[50];
    char pdutime[30];
    const char *p = pdu;

    memset(pdunum, 0x00, sizeof(pdunum));
    memset(pdutime, 0x00, sizeof(pdutime));

    p += 2; //skip tp-mr
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    numlen = strtol(buf, NULL, 16);

    p += 2;
    strncpy(pdunum, p, numlen + 2 +numlen%2);	//奇数时加F补足
    DecodeNumFormSmsPDU(pdunum, (char*)pRecord->szNumber);

    p += (numlen + 2 + numlen%2);

    strncpy(pdutime, p, 14);
    DecodeTimeFormSmsPDU(pdutime, &pRecord->timestamp);
    p += 14;

    p += 14; //skip tp discharge time

    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    status = strtol(buf, NULL, 16);

    switch(status) {
    case 0x00:
        strcpy(pRecord->szContent, "Message received on handset");
        break;
    default:
        break;
    }
}

int DecodeUDHFormSmsPDU(StSmsRecord *pRecord,  char *para)
{
    int udhl = 0;
    char *ptr = para;
    char p[5] = {0};
    char* udh1 = "050003";
    char* udh2 = "060804";

    if(strncmp(udh1, para, strlen(udh1)) == 0) {
        ptr = para + strlen(udh1);
        udhl = 6;
        strncpy(p, ptr, 2);
        pRecord->nRefCnt = strtol(p, NULL, 16);;

        ptr += 2;
        memset(p, 0x00, 5);
        strncpy(p, ptr, 2);
        pRecord->nTotalCnt = strtol(p, NULL, 16);;

        ptr +=2;
        memset(p, 0x00, 5);
        strncpy(p, ptr, 2);
        pRecord->nSeqCnt = strtol(p, NULL, 16);;
    } else if(strncmp(udh2, para, strlen(udh2)) == 0) {
        ptr = para + strlen(udh2);
        udhl = 7;
        strncpy(p, ptr, 4);
        pRecord->nRefCnt = strtol(p, NULL, 16);;

        ptr += 4;
        memset(p, 0x00, 5);
        strncpy(p, ptr, 2);
        pRecord->nTotalCnt = strtol(p, NULL, 16);;

        ptr +=2;
        memset(p, 0x00, 5);
        strncpy(p, ptr, 2);
        pRecord->nSeqCnt = strtol(p, NULL, 16);;
    }

    return udhl;
}

//长度，类型，号码
/*0D91683129413485F7*/
void DecodeNumFormSmsPDU(const char *pdunum, char *pOutNum)
{
    ASSERT(pdunum && pOutNum);

    const char *p = pdunum;
    const char *q = p;
    USHORT numtype = 0;
    char buf[10];

    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    numtype = strtol(buf, NULL, 16);

    switch (numtype) {
    case 0x91:
        pOutNum[0] = '+';
        break;
    }

    p += 2;
    while (*p) {
        q = p + 1;
        if (*q) {
            pOutNum[strlen(pOutNum)] = *q;
        }

        if (UPCASE(*p) != 'F') {
            pOutNum[strlen(pOutNum)] = *p;
            p += 2;
        } else {
            pOutNum[strlen(pOutNum)] = '\0';
            break;
        }
    }
}

//年，月，日，时，分，秒，时区
/*60804041235100*/
void DecodeTimeFormSmsPDU(const char *pdutime, COleDateTime *pOutTime)
{
    ASSERT(pdutime && pOutTime);

    int time[7];
    int cnt;
    const char *p, *q;

    for (cnt = 0; cnt < 7; cnt++)
        time[cnt] = 0;

    p = q = pdutime;
    cnt = 0;

    while (*p && cnt < 7) {
        q = p + 1;
        time[cnt] = (*q - 48) * 10 + (*p - 48);
        cnt++;
        p += 2;
    }

    *pOutTime = COleDateTime(time[0] + 2000, time[1], time[2], time[3], time[4], time[5]);
}

//内容
/*963F62C9*/
void DecodeContentFromSmsPDU(const char *pduContent, const BYTE codeType, char *pContent)
{
    ASSERT(pduContent && pContent);

    //ASCII
    if(codeType == 0x00) {
        gsmDecode7bit(pduContent, pContent, strlen(pduContent));
    } else if (codeType == 0x04) {
        const char *p = pduContent;
        char buf[10];
        char c;

        while (*p) {
            memset(buf, 0x00, sizeof(buf));
            strncpy(buf, p, 2);
            c = strtol(buf, NULL, 16);
            pContent[strlen(pContent)] = c;
            p += 2;
        }
    }
    //UCS2
    else if (codeType == 0x08) {
        CString strGb = UCS2ToGB((CString)pduContent);
        USES_CONVERSION;
        strncpy(pContent, W2A((LPCTSTR)strGb), SMS_CHAR_MAX * 2);
    } else
        ASSERT(false);
}

/* +CMGR: 1,,24
0891683108200505F0240D91683129413485F700086080404123510004963F62C9
*/
void DecodeSmsPDU(const char *pdu, const USHORT len, StSmsRecord *pRecord)
{
    ASSERT(pdu != NULL && strlen(pdu) > 0);
    ASSERT(len > 0);
    ASSERT(pRecord != NULL);

    const char *p = pdu;
    char buf[20];
    USHORT numlen = 0;

    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 2);
    numlen = strtol(buf, NULL, 16);

    //skip SC Number
    p = p + 2 * (numlen + 1);

    if (*p) {
        memset(buf, 0x00, sizeof(buf));
        strncpy(buf, p, 2);
        DecodeFOFormSmsPDU(pRecord, buf);

        p+=2;
        switch(pRecord->tp_mti) {
        case 0X00:  //SMS_DELIVERY or SMS DELIVERY REPORT
            DecodeDeliverySms(p, pRecord);
            break;
        case 0x01:  //SMS_SUBMIT or SMS SUBMIT REPORT
            DecodeSubmitSms(p, pRecord);
            break;
        case 0x02:  //SMS_STATUS_REPORT or SMS_COMMAND
            DecodeSReportSms(p, pRecord);
            break;
        case 0x03:  //RESERVED
            break;
        default:
            break;
        }
    }
}

int EncodeNumForSmsPDU(const char *pnum, char *pOutNum)
{
    ASSERT(pnum && pOutNum);

    const char *p = pnum;
    const char *q = p;

    while (*p) {
        q = p + 1;
        if (*q) {
            pOutNum[strlen(pOutNum)] = *q;
        } else {
            pOutNum[strlen(pOutNum)] = 'F';
        }
        pOutNum[strlen(pOutNum)] = *p;

        if(*q == '\0') {
            break;
        }
        p+=2;
    }
    pOutNum[strlen(pOutNum)] = '\0';
    return strlen(pOutNum);
}

//encode smscenter number, return the whole tpdu length
int EncodeSCNumberForSmsPDU(char *sbuffer)
{
    char pTemp[30] = {0};
    char pDeScn[50] = {0};
    char *pDes = pDeScn;
    int iscLen = 0;
    USES_CONVERSION;
    char *pSCNumber = W2A(gSmsCentreNum);
    int cnt = 0;

    strcpy(pDeScn, "00");
    pDes += 2;

    //TON of SMS Center number
    if(pSCNumber[0] == '+') {
        strcat(pDeScn, "91");       //145
        pSCNumber++;
    } else {
        strcat(pDeScn, "A1");       //161
    }
    pDes += 2;

    iscLen = EncodeNumForSmsPDU(pSCNumber, pDes) + 2;

    sprintf(pTemp, "%02X", iscLen/2);
    strncpy(pDeScn, pTemp, 2);

    if(sbuffer != NULL) {
        strcpy(sbuffer, pDeScn);
    }
    return strlen(pDeScn);
}

int EnCodeFOForSmsPDU()
{
    int fo = 0;
    //MO FO Format
    //|     7      |        6       |       5       |   4       3   |   2       |   1      0   |
    //| TP-RP   |   TP-UDHI |TP-SRR    |   TP-VPF   |TP-RD  |TP-MTI     |

    //set tp-mti
    fo |= SMS_MASK_MTI_MO;

    //set tp-udhi
    if(gSmsIsConcatenate) {
        fo |= SMS_MASK_UDHI;
    }

    //set tp-srr
    if(g_SetData.Messages_nDeliReport == 1) {
        fo |= SMS_MASK_SRR;
    }

    //set tp-vp
    fo |= SMS_MASK_VP_RF;

    return fo;
}

int  EncodeSmsPDU(char *pduOut, CString da, CString context)
{
    int ifo = 0;
    char *pOA = NULL;
    int udhl = 0;
    char udh[20] = {0};
    TCHAR wudh[20] = {0};
    char temp[20] = {0};
    char odtmp[20] = {0};
    int contextLen = 0;
    int numLen = 0;
    CString pDA = da;
    char szAscBuf[1600] = {0};

    ASSERT(pduOut);
    EncodeSCNumberForSmsPDU(pduOut);

    USES_CONVERSION;
    //encode fo
    ifo = EnCodeFOForSmsPDU();
    sprintf(temp, "%02XFF", ifo);
    strcat(pduOut, temp);

    memset(temp, 0x00, sizeof(temp));
    if(da.Left(1).Compare(_T("+")) == 0) {
        strcat(temp, "91");
        pDA = da.Mid(1);
    } else {
        strcat(temp, "A1");
    }
    pOA = W2A(pDA);
    numLen = strlen(pOA);
    sprintf(odtmp, "%02X", numLen);
    EncodeNumForSmsPDU(W2A(pDA),&temp[strlen(temp)]);
    strcat(pduOut, odtmp);
    strcat(pduOut, temp);

    strcat(pduOut, "00");
    //TP-PID

    if(IsAlphabetUnicode(context)) {
        strcat(pduOut, "00");    //TP-DCS
        contextLen = context.GetLength();
        gsmEncode7bit(W2A(context), (unsigned char *)szAscBuf, contextLen);
    } else {
        strcat(pduOut, "08");    //TP-DCS
        CString strUC = BTToUCS2((CString)context);
        contextLen =  WCharToChar(strUC, szAscBuf)/2;
    }

    if((ifo & SMS_MASK_VP) == SMS_MASK_VP_RF) {
        int vp = gc_sms_validity_period[g_SetData.Messages_nValPeriod];
        memset(temp, 0x00, sizeof(temp));
        sprintf(temp, "%02X", vp);
        strcat(pduOut, temp);
    } else if((ifo & SMS_MASK_VP) == SMS_MASK_VP_NP) {
        //not present vp
    }/*TP-VP*/

    if(ifo & SMS_MASK_UDHI) {
        if(SetConcatenateSmsPara((TCHAR *)wudh,  gSmsRefCnt, gSmsCurSege+1,
                                 gSmsTotalSege)) {
            WCharToChar(wudh, udh);
            udhl = strlen(udh)/2;
        }
    }/*TP-UDH*/
    memset(temp, 0x00, sizeof(temp));
    contextLen += udhl;
    sprintf(temp,"%02X",contextLen);
    strcat(pduOut, temp);
    strcat(pduOut, udh);

    strcat(pduOut, szAscBuf);

    return strlen(pduOut);
}
#endif

//提取长短信的参数
//输入："nRefCnt/nSeqCnt/nTotalCnt","1/1/2"
//输出：长短信标识号,长短信当前分段号,长短信总分段号
BOOL ExtractConcatenateSmsPara(char *Para, USHORT *nRefCnt, BYTE *nSeqCnt, BYTE *nTotalCnt)
{
    ASSERT(Para && strlen(Para) > 0);

    BOOL ret = TRUE;
    char *p = Para;
    char *ptr[3];
    int cnt = 1;
    ptr[0] = p;
    ptr[1] = ptr[2] = 0;

    while (*p && cnt < 3) {
        if (*p == '/') {
            ptr[cnt++] = p + 1;
            *p = 0;
        }
        p++;
    }

    if (cnt < 3 || ptr[1] == 0 || ptr[2] == 0)
        ret = FALSE;
    else {
        *nRefCnt = atoi(ptr[0]);
        *nSeqCnt = atoi(ptr[1]);
        *nTotalCnt = atoi(ptr[2]);

        if (*nSeqCnt == 0 || *nTotalCnt == 0 || *nSeqCnt > *nTotalCnt)
            ret = FALSE;
    }

    if (!ret)
        *nRefCnt = *nSeqCnt = *nTotalCnt = 0;
    return ret;
}

//flexi 版本，第一个参数修改为短信内容
//提取长短信的参数
//输入："nRefCnt/nSeqCnt/nTotalCnt","1/1/2"
//输出：长短信标识号,长短信当前分段号,长短信总分段号
BOOL ExtractConcatenateSmsPara_Flexi(CString Para, TCHAR *nRefCnt, BYTE *nSeqCnt, BYTE *nTotalCnt)
{
    ASSERT(Para && wcslen(Para) > 0);

    BOOL ret = TRUE;

    TCHAR* szMsg = new TCHAR[100];
    //其参数为CString字符串的长度
    szMsg = Para.GetBuffer(Para.GetLength());
    Para.ReleaseBuffer();
//  delete []szMsg;
//  szMsg = NULL;

//  TCHAR *p = szMsg;
//  p++;
//  int cnt;

//  TCHAR *ptr[5];
//     for (cnt =0; cnt < 5; cnt++)
//     {
//      *ptr[cnt] = szMsg[cnt+1];
//     }
//  delete []szMsg;
//      szMsg = NULL;

//      if(cnt < 5 || ptr[1] == 0 || ptr[2] == 0)
//          ret = FALSE;

//  TCHAR nRefCnt_flexi;


    TCHAR STR_SeqCnt1[2] = {0};
    STR_SeqCnt1[0] = szMsg[1];

    TCHAR STR_SeqCnt2[2] = {0};
    STR_SeqCnt2[0] = szMsg[2];

    TCHAR STR_nTotalCnt1[2] = {0};
    STR_nTotalCnt1[0] = szMsg[4];

    TCHAR STR_nTotalCnt2[2] = {0};
    STR_nTotalCnt2[0] = szMsg[5];



    *nRefCnt = szMsg[3];
    *nSeqCnt = (BYTE)((_wtoi(STR_SeqCnt1) * 10) + (_wtoi(STR_SeqCnt2)));
    *nTotalCnt = (BYTE)((_wtoi(STR_nTotalCnt1) * 10) + (_wtoi(STR_nTotalCnt2)));

    if (*nSeqCnt == 0 || *nTotalCnt == 0 || *nSeqCnt > *nTotalCnt)
        ret = FALSE;

    if (!ret)
        *nRefCnt = *nSeqCnt = *nTotalCnt = 0;
    return ret;

//     BOOL ret = TRUE;
//     char *p = Para;
//     char *ptr[3];
//     int cnt = 1;
//     ptr[0] = p; ptr[1] = ptr[2] = 0;
//
//     while(*p && cnt < 3)
//     {
//         if(*p == '/')
//         {
//             ptr[cnt++] = p+1;
//             *p = 0;
//         }
//         p++;
//     }
//
//     if(cnt < 3 || ptr[1] == 0 || ptr[2] == 0)
//         ret = FALSE;
//     else
//     {
//         *nRefCnt = atoi(ptr[0]);
//         *nSeqCnt = atoi(ptr[1]);
//         *nTotalCnt = atoi(ptr[2]);
//
//         if(*nSeqCnt == 0 || *nTotalCnt == 0 || *nSeqCnt > *nTotalCnt)
//             ret = FALSE;
//     }
//
//     if(!ret)
//         *nRefCnt = *nSeqCnt = *nTotalCnt = 0;
//     return ret;
}

//显示长短信参数
//输入：长短信标识号,长短信当前分段号,长短信总分段号
//输出："(nRefCnt/nSeqCnt/nTotalCnt)","(1/1/2)"
//Modified by Zhou Bin 2008.12.30
//BOOL DspConcatenateSmsPara(TCHAR *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt)
BOOL DspConcatenateSmsPara(char *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt)
{
    ASSERT(DspBuf);

    if (nSeqCnt == 0 || nTotalCnt == 0 || nSeqCnt > nTotalCnt)
        return FALSE;
    else {
#if 0
        //Modified by Zhou Bin 2008.12.30
        //swprintf(DspBuf, _T("(%d/%d/%d)"), nRefCnt, nSeqCnt, nTotalCnt);
        sprintf(DspBuf, "(%d/%d/%d)", nRefCnt, nSeqCnt, nTotalCnt);
#else
        //swprintf(DspBuf,_T("(%d/%d)"), nSeqCnt, nTotalCnt);
        sprintf(DspBuf, "(%d/%d)", nSeqCnt, nTotalCnt);

#endif
        return TRUE;
    }
}

//设置写发长短信参数
//输入：长短信标识号,长短信总分段号,长短信当前分段号
//输出：CONCAT_8: "050003050201", CONCAT_16: "06080400030201"
BOOL SetConcatenateSmsPara(TCHAR *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt)
{
    ASSERT(DspBuf);

    if (nSeqCnt == 0 || nTotalCnt == 0 || nSeqCnt > nTotalCnt)
        return FALSE;
    else {
        if (nRefCnt <= 0x00FF)
            swprintf(DspBuf, _T("050003%02X%02X%02X"), nRefCnt, nTotalCnt, nSeqCnt);
        else
            swprintf(DspBuf, _T("060804%04X%02X%02X"), nRefCnt, nTotalCnt, nSeqCnt);
        return TRUE;
    }
}

BOOL SetConcatenateSmsParaA(char *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt, char MinMaxChar)
{
    ASSERT(DspBuf);

    if (nSeqCnt == 0 || nTotalCnt == 0 || nSeqCnt > nTotalCnt)
        return FALSE;
    else {
        if (nRefCnt <= 0x00FF) {
            if (nSeqCnt < 10 && nTotalCnt < 10) {
                //sprintf(DspBuf, "050003%02X%02X%02X", nRefCnt, nTotalCnt, nSeqCnt);
                sprintf(DspBuf, "(0%d%c0%d)", nSeqCnt, MinMaxChar, nTotalCnt); //FLEXI modify by liub
            } else if (nSeqCnt < 10) {
                sprintf(DspBuf, "(0%d%c%d)", nSeqCnt, MinMaxChar, nTotalCnt); //FLEXI modify by liub
            } else if (nTotalCnt < 10) {
                sprintf(DspBuf, "(%d%c0%d)", nSeqCnt, MinMaxChar, nTotalCnt); //FLEXI modify by liub
            } else {
                sprintf(DspBuf, "(%d%c%d)", nSeqCnt, MinMaxChar, nTotalCnt); //FLEXI modify by liub
            }
        }

        else {
            if (nSeqCnt < 10 && nTotalCnt < 10) {
                //sprintf(DspBuf, "050003%02X%02X%02X", nRefCnt, nTotalCnt, nSeqCnt);
                sprintf(DspBuf, "(0%d%c0%d)", nSeqCnt, MinMaxChar, nTotalCnt); //FLEXI modify by liub
            } else if (nSeqCnt < 10) {
                sprintf(DspBuf, "(0%d%c%d)", nSeqCnt, MinMaxChar, nTotalCnt); //FLEXI modify by liub
            } else if (nTotalCnt < 10) {
                sprintf(DspBuf, "(%d%c0%d)", nSeqCnt, MinMaxChar, nTotalCnt); //FLEXI modify by liub
            } else {
                sprintf(DspBuf, "(%d%c%d)", nSeqCnt, MinMaxChar, nTotalCnt); //FLEXI modify by liub
            }
        }
        return TRUE;
    }
}

//判断当前记录是否是合法的长短信
BOOL IsConcatenateSms(StSmsRecord *pSmsRecord)
{
    ASSERT(pSmsRecord);

    BOOL ret = FALSE;

    if (pSmsRecord->flag & SMS_RECORD_FLAG_CONCATENATE_SEGE) {
        if (pSmsRecord->nSeqCnt == 0 || pSmsRecord->nTotalCnt == 0
                || pSmsRecord->nSeqCnt > pSmsRecord->nTotalCnt)
            ret = FALSE;
        else
            ret = TRUE;
    }

    if (!ret) {
        pSmsRecord->flag &= (BYTE)~SMS_RECORD_FLAG_CONCATENATE_SEGE;
        pSmsRecord->nRefCnt = pSmsRecord->nSeqCnt = pSmsRecord->nTotalCnt = 0;
    }

    return ret;
}

#ifdef FEATURE_SMS_CONCATENATE
BYTE gSmsRefCnt = -1;
BOOL gSmsIsConcatenate = FALSE;
BOOL gSmsIsAsciiCode = TRUE;
BYTE gSmsTotalSege;
BYTE gSmsCurSege;
//Modified by Zhou Bin 2008.12.30
TCHAR gszSmsSege[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX + 1];
//TCHAR gszSmsSege[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX + 1];

void ResetSmsConcatenateData()
{
    gSmsIsConcatenate = FALSE;
    gSmsIsAsciiCode = TRUE;
    gSmsCurSege = 0;
    gSmsTotalSege = 0;
    memset(gszSmsSege, 0x00, sizeof(gszSmsSege));
}

BOOL DivideSmsConcatenate(const TCHAR *szRawContent)
{
    ASSERT(szRawContent);

    ResetSmsConcatenateData();

    int l = GetUnicodeCharNum(szRawContent);

    if (l == 0 || l > (SMS_CONCAT_SEGMENT_MAX * SMS_CONCAT_ASCII_MAX))
        return FALSE;

    gSmsIsAsciiCode = IsAlphabetUnicode(szRawContent);

    if (gSmsIsAsciiCode) {
        if (GetACSIICharNum(szRawContent) > SMS_CHAR_MAX)
            gSmsIsConcatenate = TRUE;
    } else {
        if (GetUnicodeCharNum(szRawContent) > SMS_CHINESE_CHAR_MAX)
            gSmsIsConcatenate = TRUE;
    }

    if (gSmsIsConcatenate) {
        gSmsRefCnt++;

        if (gSmsIsAsciiCode) {
            USHORT i, j;
            USHORT GbNum = 0;
            USHORT len = wcslen(szRawContent);

            gSmsTotalSege = 0;

            for (i = 0, j = 0; i < SMS_CONCAT_ASCII_MAX && j < len;) {
                // gszSmsSege[gSmsTotalSege][i++] = szRawContent[j];

                //     if((IsDBCSLeadByte((BYTE)(char)(szRawContent[j]  & 0x00FF))) && (j < len))
                //   {
                gszSmsSege[gSmsTotalSege][i] = szRawContent[j];
                //  }
                i++;
                j++;
                GbNum++;

                if (GbNum == SMS_CONCAT_ASCII_MAX && j < len && gSmsTotalSege < SMS_CONCAT_SEGMENT_MAX - 1) {
                    i = 0;
                    GbNum = 0;
                    gSmsTotalSege++;
                }
            }
            gSmsTotalSege++;
        } else {
            USHORT i, j;
            USHORT GbNum = 0;
            USHORT len = wcslen(szRawContent);

            gSmsTotalSege = 0;
            for (i = 0, j = 0; i < SMS_CONCAT_GB_MAX && j < len;) {
                //  gszSmsSege[gSmsTotalSege][i++] = szRawContent[j];

                //  if((IsDBCSLeadByte((BYTE)(char)(szRawContent[j] >> 8))) && (j < len))
                //  {
                gszSmsSege[gSmsTotalSege][i] = szRawContent[j];
                // }
                i++;
                j++;
                GbNum++;

                if (GbNum == SMS_CONCAT_GB_MAX && j < len && gSmsTotalSege < SMS_CONCAT_SEGMENT_MAX - 1) {
                    i = 0;
                    GbNum = 0;
                    gSmsTotalSege++;
                }
            }
            gSmsTotalSege++;
        }
    }

    return TRUE;
}
#endif //FEATURE_SMS_CONCATENATE

//国家码列表
const char szCountryCodeArr[][10] = {
    "+65",
    "+852",
    "+86",
    "+886",
    "+1",
    "+27",
    "+30",
    "+31",
    "+351",
    "+359",
    "+385",
    "+39",
    "+40",
    "+44",
    "+46",
    "+49",
    "+504",
    "+53",
    "+55",
    "+61",
    "+62",
    "+64",
    "+90",
    "+91",
    "+94",
    "+961",
    "", //end flag
};
#ifdef FEATURE_SMS_PDUMODE
BOOL SmsAtCMGRRspProc(BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum, StSmsRecord &record, const EnSmsKind kind)
{
    int cnt = 0;
    char buf[9], *p;
    p = (char *)&strArr[0][strlen(gcstrResSms[AT_SMS_QCMGR])];

    ASSERT(wStrNum == 3 || wStrNum == 2);
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, p, 1);
    record.state = (EnSmsState)atoi(buf);
    if (record.state > SMS_STATE_ALL || record.state < SMS_STATE_MT_NOT_READ) {
        return FALSE;
    }

    //state test begin
    if (kind == SMS_KIND_MT) {
        if (record.state != SMS_STATE_MT_NOT_READ
                && record.state != SMS_STATE_MT_READ) {
            return FALSE;
        }
    } else if (kind == SMS_KIND_MO) {
        if (record.state != SMS_STATE_MO_NOT_SENT
                && record.state != SMS_STATE_MO_SENT) {
            return FALSE;
        }
    }

    if (record.state == SMS_STATE_MO_NOT_SENT || record.state == SMS_STATE_MO_SENT) {
        record.state = SMS_STATE_MT_READ;
    }

    if(wStrNum == 3) {
        p = (char *)&strArr[1];
        DecodeSmsPDU(p, strlen(p), &record);
    }

    return TRUE;
}
#else
BOOL SmsAtCMGRRspProc(BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum, StSmsRecord &record, const EnSmsKind kind)
{
    int cnt = 0;
    char *ptr[9], *p;
    p = (char *)&strArr[0][strlen(gcstrResSms[AT_SMS_QCMGR])];
#ifdef FEATURE_ATTEST_SUPPORT
    CStdioFile file;
    if (file.Open("Sms.log", CFile::modeReadWrite)) {
        CString str;
        str.Format("%s", p);
        str.Insert(str.GetLength(), "\n");
        DWORD dwActual = file.SeekToEnd();
        file.WriteString(str);
        file.Close();
    }
#endif
    ptr[0] = ptr[1] = ptr[2] = ptr[3] = ptr[4] = ptr[5] = ptr[6] = ptr[7] = ptr[8] = 0;

    BOOL bOutQuot = TRUE;//判断是否在双引号内
    BOOL bNo_See_Comma = TRUE;//判断是否遇到逗号

    while (*p != '\0') {
        if (bNo_See_Comma) {
            ptr[cnt++] = p;
            bNo_See_Comma = FALSE;

            //  p++;
        }


        if (*p == ',' && bOutQuot == TRUE && cnt < 9) {

            *p++ = '\0';
            bNo_See_Comma = TRUE;
        } else if (*p == '\"') {
            bOutQuot = !bOutQuot;
            p++;
        } else
            p++;
    }


    for (int i = 0; i < 9; i++) {
        if (ptr[i] != 0) {
            if (*(ptr[i] + strlen(ptr[i]) - 1) == '\"')
                *(ptr[i] + strlen(ptr[i]) - 1) = '\0';
            if (*ptr[i] == '\"')
                ptr[i]++;
        }
    }

    if ((record.state = GetSmsStateFromStr(ptr[0])) == SMS_STATE_MAX) {
        return FALSE;
    }

    //state test begin
    if (kind == SMS_KIND_MT) {
        if (record.state != SMS_STATE_MT_NOT_READ
                && record.state != SMS_STATE_MT_READ) {
            return FALSE;
        }
    } else if (kind == SMS_KIND_MO) {
        if (record.state != SMS_STATE_MO_NOT_SENT
                && record.state != SMS_STATE_MO_SENT) {
            return FALSE;
        }
    }
    //state test end
    if (!(ptr[1] && *ptr[1])) {
        memset(record.szNumber, 0x00, PB_NUM_MAX);
    } else {
        CString szNumTemp = (char *)ptr[1];
        wcscpy((TCHAR *)record.szNumber, szNumTemp);
    }

    int time, scnum, concatenate, ascii_or_unicode;
    time = scnum = concatenate = ascii_or_unicode = -1;

    if (record.state == SMS_STATE_MT_NOT_READ || record.state == SMS_STATE_MT_READ) {
        if (!wcsicmp(g_SetData.Setup_sz3GType, _T("CDMA2000"))) {
            time = 3;
            scnum = 2;
            concatenate = -1;
            ascii_or_unicode = 5;
        } else {
            time = 3;
            scnum = 2;
            concatenate = -1;
            ascii_or_unicode = 7;
        }
    } else if (record.state == SMS_STATE_MO_NOT_SENT || record.state == SMS_STATE_MO_SENT) {
        if (!wcsicmp(g_SetData.Setup_sz3GType, _T("CDMA2000"))) {
            time = 3;
            scnum = 2;
            concatenate = -1;
            ascii_or_unicode = 4;
        } else {
            time = 3;
            scnum = 2;
            concatenate = -1;
            ascii_or_unicode = 4;
        }
        record.state = SMS_STATE_MT_READ;
    }

    if (time == -1 || !(ptr[time] && *ptr[time]))
        record.timestamp = COleDateTime::GetCurrentTime();
    else {
        if (!GetTimeFromStr((LPCSTR)ptr[time], record.timestamp))
            record.timestamp = COleDateTime::GetCurrentTime();
    }
    CString szSCNumTemp = (char*)ptr[scnum];
    if (scnum == -1 || !(ptr[scnum] && *ptr[scnum]))
        memset(record.szSCNumber, 0x00, SMS_SC_NUM_MAX);
    else {
        wcsncpy((TCHAR*)record.szSCNumber, szSCNumTemp, SMS_SC_NUM_MAX);
    }

    if (concatenate != -1 && ptr[concatenate] && *ptr[concatenate]) {
        if (ExtractConcatenateSmsPara(ptr[concatenate], &record.nRefCnt, &record.nSeqCnt, &record.nTotalCnt))
            record.flag |= SMS_RECORD_FLAG_CONCATENATE_SEGE;
    }
    //modified by wk end on 2006-8-22

    USES_CONVERSION;
    if (wStrNum == 3) {
        //modify by lijl 2009.4.13 以ascii码形式存储短信内容
        if(ptr[ascii_or_unicode] != NULL) {
            if (*ptr[ascii_or_unicode] == '0') {
                strncpy(record.szContent, (char*)strArr[1], SMS_CHAR_MAX);

            } else {
                CString strGb = UCS2ToGB((CString)((char*)strArr[1]));
                strncpy(record.szContent, W2A((LPCTSTR)strGb), SMS_CHAR_MAX * 2);
            }
        }
    } else
        memset((TCHAR *)record.szContent, 0, SMS_CHAR_MAX * 2);
#ifdef FEATURE_ATTEST_SUPPORT
    if (file.Open("Sms.log", CFile::modeReadWrite)) {
        CString str;
        str.Format("%s", (CString)((char*)strArr[1]));
        str.Insert(str.GetLength(), "\n");
        DWORD dwActual = file.SeekToEnd();
        file.WriteString(str);
        file.Close();
    }
#endif

    return TRUE;
}
#endif

const wchar_t GSM0338ToUCS2Array[GSM0338_EXTENSIONS][GSM0338_ALPHABET_SIZE] = {
    /* GSM 7 bit default alphabet */
    {
        0x0040,     /* 00 */
        0x00A3,
        0x0024,
        0x00A5,
        0x00E8,
        0x00E9,
        0x00F9,
        0x00EC,
        0x00F2,
        0x00C7,
        0x000A,
        0x00D8,
        0x00F8,
        0x000D,
        0x00C5,
        0x00E5,     /* 0F */

        0x0394,     /* 10 */
        0x005F,
        0x03A6,
        0x0393,
        0x039B,
        0x03A9,
        0x03A0,
        0x03A8,
        0x03A3,
        0x0398,
        0x039E,
        0x0020,
        0x00C6,
        0x00E6,
        0x00DF,
        0x00C9,     /* 1F */

        0x0020,     /* 20 */
        0x0021,
        0x0022,
        0x0023,
        0x00A4,
        0x0025,
        0x0026,
        0x0027,
        0x0028,
        0x0029,
        0x002A,
        0x002B,
        0x002C,
        0x002D,
        0x002E,
        0x002F,     /* 2F */

        0x0030,     /* 30 */
        0x0031,
        0x0032,
        0x0033,
        0x0034,
        0x0035,
        0x0036,
        0x0037,
        0x0038,
        0x0039,
        0x003A,
        0x003B,
        0x003C,
        0x003D,
        0x003E,
        0x003F,     /* 3F */

        0x00A1,     /* 40 */
        0x0041,
        0x0042,
        0x0043,
        0x0044,
        0x0045,
        0x0046,
        0x0047,
        0x0048,
        0x0049,
        0x004A,
        0x004B,
        0x004C,
        0x004D,
        0x004E,
        0x004F,     /* 4F */

        0x0050,     /* 50 */
        0x0051,
        0x0052,
        0x0053,
        0x0054,
        0x0055,
        0x0056,
        0x0057,
        0x0058,
        0x0059,
        0x005A,
        0x00C4,
        0x00D6,
        0x00D1,
        0x00DC,
        0x00A7,     /* 5F */

        0x00BF,     /* 60 */
        0x0061,
        0x0062,
        0x0063,
        0x0064,
        0x0065,
        0x0066,
        0x0067,
        0x0068,
        0x0069,
        0x006A,
        0x006B,
        0x006C,
        0x006D,
        0x006E,
        0x006F,     /* 6F */

        0x0070,     /* 70 */
        0x0071,
        0x0072,
        0x0073,
        0x0074,
        0x0075,
        0x0076,
        0x0077,
        0x0078,
        0x0079,
        0x007A,
        0x00E4,
        0x00F6,
        0x00F1,
        0x00FC,
        0x00E0,     /* 7F */

#if 0
        0x0040      /* @ sign exception */
#endif
    },

    /* GSM 7 bit default alphabet extension table */
    {
        0x0040,             /* 00 */
        0x00A3,
        0x0024,
        0x00A5,
        0x00E8,
        0x00E9,
        0x00F9,
        0x00EC,
        0x00F2,
        0x00C7,
        0x000A,     /* this should be a Page Break, otherwise Line Feed if not handled */
        0x00D8,
        0x00F8,
        0x000D,
        0x00C5,
        0x00E5,             /* 0F */

        0x0394,             /* 10 */
        0x005F,
        0x03A6,
        0x0393,
        0x005E,     /* ^ */
        0x03A9,
        0x03A0,
        0x03A8,
        0x03A3,
        0x0398,
        0x039E,
        0x0020,
        0x00C6,
        0x00E6,
        0x00DF,
        0x00C9,             /* 1F */

        0x0020,             /* 20 */
        0x0021,
        0x0022,
        0x0023,
        0x00A4,
        0x0025,
        0x0026,
        0x0027,
        0x007B,     /* { */
        0x007D,     /* } */
        0x002A,
        0x002B,
        0x002C,
        0x002D,
        0x002E,
        0x005C,     /* \ */ /* 2F */

        0x0030,             /* 30 */
        0x0031,
        0x0032,
        0x0033,
        0x0034,
        0x0035,
        0x0036,
        0x0037,
        0x0038,
        0x0039,
        0x003A,
        0x003B,
        0x005B,     /* [ */
        0x007E,     /* ~ */
        0x005D,     /* ] */
        0x003F,             /* 3F */

        0x007C,     /* | */ /* 40 */
        0x0041,
        0x0042,
        0x0043,
        0x0044,
        0x0045,
        0x0046,
        0x0047,
        0x0048,
        0x0049,
        0x004A,
        0x004B,
        0x004C,
        0x004D,
        0x004E,
        0x004F,             /* 4F */

        0x0050,             /* 50 */
        0x0051,
        0x0052,
        0x0053,
        0x0054,
        0x0055,
        0x0056,
        0x0057,
        0x0058,
        0x0059,
        0x005A,
        0x00C4,
        0x00D6,
        0x00D1,
        0x00DC,
        0x00A7,             /* 5F */

        0x00BF,             /* 60 */
        0x0061,
        0x0062,
        0x0063,
        0x0064,
        0x20AC,     /* Euro currency symbol */
        0x0066,
        0x0067,
        0x0068,
        0x0069,
        0x006A,
        0x006B,
        0x006C,
        0x006D,
        0x006E,
        0x006F,             /* 6F */

        0x0070,             /* 70 */
        0x0071,
        0x0072,
        0x0073,
        0x0074,
        0x0075,
        0x0076,
        0x0077,
        0x0078,
        0x0079,
        0x007A,
        0x00E4,
        0x00F6,
        0x00F1,
        0x00FC,
        0x00E0,             /* 7F */

#if 0
        0x0040      /* @ sign exception */
#endif
    }
};

StRcvConcSmsRecord gSmsRcvConcBUf[SMS_RCVCONCBUF_MAX];

void ResetSmsRcvConcBuf(BYTE nIndex)
{
    ASSERT(nIndex <= SMS_RCVCONCBUF_MAX);

    BYTE cnt;
    if (nIndex == SMS_RCVCONCBUF_MAX) {
        for (cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++) {
            memset(&gSmsRcvConcBUf[cnt], 0x00, sizeof(StRcvConcSmsRecord));
            gSmsRcvConcBUf[cnt].nTimerID = IDT_RCVCONCSMS_TIMEOUT_BEGIN + cnt;
        }
    } else {
        memset(&gSmsRcvConcBUf[nIndex], 0x00, sizeof(StRcvConcSmsRecord));
        gSmsRcvConcBUf[nIndex].nTimerID = IDT_RCVCONCSMS_TIMEOUT_BEGIN + nIndex;
    }
}

static void SaveSmsSegeToRcvConcBuf(const StSmsRecord &sege, const BYTE cnt)
{
    gSmsRcvConcBUf[cnt].bInuse = TRUE;
    gSmsRcvConcBUf[cnt].nSeqFlag[sege.nSeqCnt-1] = TRUE;
    gSmsRcvConcBUf[cnt].nRefCnt = sege.nRefCnt;
    gSmsRcvConcBUf[cnt].nTotalCnt = sege.nTotalCnt;
    gSmsRcvConcBUf[cnt].timestamp = sege.timestamp;
    wcsncpy((TCHAR*)gSmsRcvConcBUf[cnt].szNumber, (const TCHAR*)sege.szNumber, PB_NUM_MAX);
    wcsncpy((TCHAR*)gSmsRcvConcBUf[cnt].szSCNumber, (const TCHAR*)sege.szSCNumber, SMS_SC_NUM_MAX);
    wcsncpy((TCHAR*)gSmsRcvConcBUf[cnt].szContent[sege.nSeqCnt-1], (const TCHAR*)sege.szContent, SMS_CONCAT_ASCII_MAX * 2);
}

int SaveSmsSegeToRcvConcBuf(const StSmsRecord &sege)
{
    BYTE cnt;

    if (sege.nSeqCnt == 0 || sege.nTotalCnt == 0 || sege.nSeqCnt > sege.nTotalCnt
            || sege.nSeqCnt > SMS_CONCAT_SEGMENT_MAX || sege.nTotalCnt > SMS_CONCAT_SEGMENT_MAX)
        return -1;

    for (cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++) {
        if (gSmsRcvConcBUf[cnt].bInuse && gSmsRcvConcBUf[cnt].nRefCnt == sege.nRefCnt
                && gSmsRcvConcBUf[cnt].nTotalCnt == sege.nTotalCnt
                && !gSmsRcvConcBUf[cnt].nSeqFlag[sege.nSeqCnt-1]) {
            if (wcscmp((const TCHAR*)gSmsRcvConcBUf[cnt].szNumber, (const TCHAR*)sege.szNumber) == 0) {
                SaveSmsSegeToRcvConcBuf(sege, cnt);
                return cnt;
            }
        }
    }

    for (cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++) {
        if (!gSmsRcvConcBUf[cnt].bInuse) {
            SaveSmsSegeToRcvConcBuf(sege, cnt);
            return cnt;
        }
    }

    return -1;
}

int MergeConcSmsSegeTogether(StSmsRecord &record)
{
    BYTE i;
    BYTE cnt;

    memset(&record, 0x00, sizeof(StSmsRecord));

    for (cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++) {
        if (gSmsRcvConcBUf[cnt].bInuse && gSmsRcvConcBUf[cnt].nTotalCnt > 0) {
            for (i = 0; i < gSmsRcvConcBUf[cnt].nTotalCnt; i++) {
                if (!gSmsRcvConcBUf[cnt].nSeqFlag[i])
                    break;
            }

            //Ready to merge
            if (i == gSmsRcvConcBUf[cnt].nTotalCnt) {
                record.flag = SMS_RECORD_FLAG_NEW | SMS_RECORD_FLAG_CONCATENATE_FULL;
                record.state = SMS_STATE_MT_NOT_READ;
                record.nRefCnt = gSmsRcvConcBUf[cnt].nRefCnt;
                record.timestamp = gSmsRcvConcBUf[cnt].timestamp;
                strncpy((char*)record.szNumber, (const char*)gSmsRcvConcBUf[cnt].szNumber, PB_NUM_MAX);
                record.m_NoATRspCDS = TRUE;
                //  wcsncpy((TCHAR*)record.szSCNumber, (const TCHAR*)gSmsRcvConcBUf[cnt].szSCNumber, SMS_SC_NUM_MAX);//(WCDMA使用)

                for (i = 0; i < gSmsRcvConcBUf[cnt].nTotalCnt; i++)
                    strncat((char*)record.szContent, (const char*)gSmsRcvConcBUf[cnt].szContent[i], SMS_CONCAT_ASCII_MAX * 2);

                ResetSmsRcvConcBuf(cnt);
                return cnt;
            }
        }
    }
    return -1;
}

BYTE gSmsTranRefCnt = -1;
BOOL gSmsTranIsConcatenate = FALSE;
BOOL gSmsTranIsAsciiCode = TRUE;
BYTE gSmsTranTotalSege;
BYTE gSmsTranCurSege;
TCHAR gszSmsTranSege[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX + 1];

void ResetSmsTranConcatenateData()
{
    gSmsTranIsConcatenate = FALSE;
    gSmsTranIsAsciiCode = TRUE;
    gSmsTranCurSege = 0;
    gSmsTranTotalSege = 0;
    memset(gszSmsTranSege, 0x00, sizeof(gszSmsTranSege));
}

BOOL DivideSmsTranConcatenate(const TCHAR *szRawContent)
{
    ASSERT(szRawContent);

    ResetSmsTranConcatenateData();

    int l = GetUnicodeCharNum(szRawContent);

    if (l == 0 || l > (SMS_CONCAT_SEGMENT_MAX * SMS_CONCAT_ASCII_MAX))
        return FALSE;

    gSmsTranIsAsciiCode = IsAlphabetUnicode(szRawContent);

    if (gSmsTranIsAsciiCode) {
        if (GetACSIICharNum(szRawContent) > SMS_CHAR_MAX)
            gSmsTranIsConcatenate = TRUE;
    } else {
        if (GetUnicodeCharNum(szRawContent) > SMS_CHINESE_CHAR_MAX)
            gSmsTranIsConcatenate = TRUE;
    }

    if (gSmsTranIsConcatenate) {
        gSmsTranRefCnt++;

        if (gSmsTranIsAsciiCode) {
            USHORT i, j;
            USHORT GbNum = 0;
            USHORT len = wcslen(szRawContent);

            gSmsTranTotalSege = 0;

            for (i = 0, j = 0; i < SMS_CONCAT_ASCII_MAX && j < len;) {
                //gszSmsTranSege[gSmsTranTotalSege][i++] = szRawContent[j];

                //if((IsDBCSLeadByte((BYTE)szRawContent[j++])) && (j < len))
                {
                    gszSmsTranSege[gSmsTranTotalSege][i++] = szRawContent[j++];
                }

                GbNum++;

                if (GbNum == SMS_CONCAT_ASCII_MAX && j < len && gSmsTranTotalSege < SMS_CONCAT_SEGMENT_MAX - 1) {
                    i = 0;
                    GbNum = 0;
                    gSmsTranTotalSege++;
                }
            }
            gSmsTranTotalSege++;
        } else {
            USHORT i, j;
            USHORT GbNum = 0;
            USHORT len = wcslen(szRawContent);

            gSmsTranTotalSege = 0;
            for (i = 0, j = 0; i < SMS_CONCAT_GB_MAX && j < len;) {
                // gszSmsTranSege[gSmsTranTotalSege][i++] = szRawContent[j];

                //  if((IsDBCSLeadByte((BYTE)szRawContent[j++])) && (j < len))
                {
                    gszSmsTranSege[gSmsTranTotalSege][i++] = szRawContent[j++];
                }

                GbNum++;

                if (GbNum == SMS_CONCAT_GB_MAX && j < len && gSmsTranTotalSege < SMS_CONCAT_SEGMENT_MAX - 1) {
                    i = 0;
                    GbNum = 0;
                    gSmsTranTotalSege++;
                }
            }
            gSmsTranTotalSege++;
        }
    }

    return TRUE;
}

const BYTE gc_sms_validity_period[] = {
    11, 71, 167, 169, 173, 244
};

TCHAR gSmsCentreNum[SMS_SC_NUM_MAX+2];

BYTE gSmsExtractNumCnt;
//TCHAR gSmsExtractNumBuf[SMS_EXTRACTNUM_MAX][PB_NUM_MAX+2];
char gSmsExtractNumBuf[SMS_EXTRACTNUM_MAX][PB_NUM_MAX+2];

BYTE ExtractNumFromSmsContent(const char *szRawContent)
{
#define SMS_EXTRACTNUM_FROM 5
#define SMS_EXTRACTNUM_TO PB_NUM_MAX

    gSmsExtractNumCnt = 0;
    memset(gSmsExtractNumBuf, 0x00, sizeof(gSmsExtractNumBuf));

    if (szRawContent != NULL && strlen(szRawContent) > 0) {
        BOOL bFound = FALSE;
        USHORT i, j, k, l, m;
        USHORT len = strlen(szRawContent);

        for (i = 0, j = 0, k = 0; k < len && gSmsExtractNumCnt < SMS_EXTRACTNUM_MAX; k++) {
            if (IsDBCSLeadByte((BYTE)szRawContent[k])) {
                if (bFound) {
                    j = k;

                    if ((j - i) >= SMS_EXTRACTNUM_FROM && (j - i) <= SMS_EXTRACTNUM_TO) {
                        for (l = 0, m = i; m < j; l++, m++)
                            gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
                        gSmsExtractNumCnt++;
                    }

                    bFound = FALSE;
                    i = j = 0;
                }
                k++;
            } else if (szRawContent[k] >= '0' && szRawContent[k] <= '9') {
                if (!bFound) {
                    i = k;
                    bFound = TRUE;
                } else {
                    if (k == len - 1) {
                        j = k + 1;

                        if ((j - i) >= SMS_EXTRACTNUM_FROM && (j - i) <= SMS_EXTRACTNUM_TO) {
                            for (l = 0, m = i; m < j; l++, m++)
                                gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
                            gSmsExtractNumCnt++;
                        }
                    }
                }
            } else {
                if (bFound) {
                    j = k;

                    if ((j - i) >= SMS_EXTRACTNUM_FROM && (j - i) <= SMS_EXTRACTNUM_TO) {
                        for (l = 0, m = i; m < j; l++, m++)
                            gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
                        gSmsExtractNumCnt++;
                    }

                    bFound = FALSE;
                    i = j = 0;
                }
            }
        }
    }

    return gSmsExtractNumCnt;
}
// BYTE ExtractNumFromSmsContent(const TCHAR *szRawContent)
// {
// #define SMS_EXTRACTNUM_FROM 5
// #define SMS_EXTRACTNUM_TO PB_NUM_MAX
//
//     gSmsExtractNumCnt = 0;
//     memset(gSmsExtractNumBuf, 0x0000, sizeof(gSmsExtractNumBuf));
//
//     if(szRawContent != NULL && wcslen(szRawContent) > 0)
//     {
//         BOOL bFound = FALSE;
//         USHORT i, j, k, l, m;
//         USHORT len = wcslen(szRawContent);
//
//         for(i=0,j=0,k=0; k < len && gSmsExtractNumCnt < SMS_EXTRACTNUM_MAX; k++)
//         {
//             if(IsDBCSLeadByte((BYTE)szRawContent[k]))
//             {
//                 if(bFound)
//                 {
//                     j = k;
//
//                     if((j-i) >= SMS_EXTRACTNUM_FROM && (j-i) <= SMS_EXTRACTNUM_TO)
//                     {
//                         for(l=0,m=i; m<j; l++,m++)
//                             gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
//                         gSmsExtractNumCnt++;
//                     }
//
//                     bFound = FALSE;
//                     i = j = 0;
//                 }
//                 k++;
//             }
//             else if(szRawContent[k] >= '0' && szRawContent[k] <= '9')
//             {
//                 if(!bFound)
//                 {
//                     i = k;
//                     bFound = TRUE;
//                 }
//                 else
//                 {
//                     if(k == len - 1)
//                     {
//                         j = k + 1;
//
//                         if((j-i) >= SMS_EXTRACTNUM_FROM && (j-i) <= SMS_EXTRACTNUM_TO)
//                         {
//                             for(l=0,m=i; m<j; l++,m++)
//                                 gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
//                             gSmsExtractNumCnt++;
//                         }
//                     }
//                 }
//             }
//             else
//             {
//                 if(bFound)
//                 {
//                     j = k;
//
//                     if((j-i) >= SMS_EXTRACTNUM_FROM && (j-i) <= SMS_EXTRACTNUM_TO)
//                     {
//                         for(l=0,m=i; m<j; l++,m++)
//                             gSmsExtractNumBuf[gSmsExtractNumCnt][l] = szRawContent[m];
//                         gSmsExtractNumCnt++;
//                     }
//
//                     bFound = FALSE;
//                     i = j = 0;
//                 }
//             }
//         }
//     }
//
//     return gSmsExtractNumCnt;
// }

BOOL g_bSmsSyncComm;
USHORT g_ME_SmsCnt;
USHORT g_ME_SmsNum;
USHORT g_ME_SmsMax;
USHORT g_SM_SmsCnt;
USHORT g_SM_SmsNum;
USHORT g_SM_SmsMax;
StSmsCardRecord *g_ME_SmsRecord;
StSmsCardRecord *g_SM_SmsRecord;
StSmsRecord g_RcvSmsRecord;

void UE_InitSmsBuf()
{
    g_bSmsSyncComm = FALSE;
    g_ME_SmsCnt = 0;
    g_ME_SmsNum = 0;
    g_ME_SmsMax = 0;
    g_SM_SmsCnt = 0;
    g_SM_SmsNum = 0;
    g_SM_SmsMax = 0;
    g_ME_SmsRecord = NULL;
    g_SM_SmsRecord = NULL;

    g_ME_SmsRecord = new StSmsCardRecord[SMS_ME_NUM_MAX];
    g_SM_SmsRecord = new StSmsCardRecord[SMS_SM_NUM_MAX];

    ASSERT(g_ME_SmsRecord);
    ASSERT(g_SM_SmsRecord);

    memset(g_ME_SmsRecord, 0x00, sizeof(StSmsCardRecord)*SMS_ME_NUM_MAX);
    memset(g_SM_SmsRecord, 0x00, sizeof(StSmsCardRecord)*SMS_SM_NUM_MAX);
}

void UE_ReleaseSmsBuf()
{
    delete []g_ME_SmsRecord;
    delete []g_SM_SmsRecord;
}

StSmsCardRecord UE_ReadSmsCardRecord(EnLocType type, WORD nIndex)
{
    ASSERT(type == LOC_ME || type == LOC_UIM);

    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;

    if (type == LOC_ME) {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
    } else {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
    }

    ASSERT(pSmsRecord && pSmsNum && nIndex < *pSmsNum);

    return pSmsRecord[nIndex];
}

void UE_EditSmsCardRecord(EnLocType type, WORD nIndex, const StSmsRecord &record)
{
    ASSERT(type == LOC_ME || type == LOC_UIM);

    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;

    if (type == LOC_ME) {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
    } else {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
    }

    ASSERT(pSmsRecord && pSmsNum && nIndex < *pSmsNum);

    pSmsRecord[nIndex].record = record;
}

BOOL UE_DeleteSmsCardRecord(EnLocType type, WORD nIndex)
{
    ASSERT(type == LOC_ME || type == LOC_UIM);

    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;

    if (type == LOC_ME) {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
    } else {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
    }

    ASSERT(pSmsRecord && pSmsNum);

    if (nIndex < *pSmsNum) {
        for (WORD i = nIndex; i < *pSmsNum - 1; i++)
            pSmsRecord[i] = pSmsRecord[i+1];

        memset(&pSmsRecord[i], 0x00, sizeof(StSmsCardRecord));
        (*pSmsNum)--;
        return TRUE;
    } else
        return FALSE;
}

BOOL UE_AddSmsCardRecord(EnLocType type, const StSmsCardRecord &record)
{
    ASSERT(type == LOC_ME || type == LOC_UIM);

    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;
    USHORT *pSmsMax = NULL;

    if (type == LOC_ME) {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
        pSmsMax = &g_ME_SmsMax;
    } else {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
        pSmsMax = &g_SM_SmsMax;
    }

    ASSERT(pSmsRecord && pSmsNum && pSmsMax);

    if (*pSmsNum < *pSmsMax) {
        pSmsRecord[*pSmsNum] = record;
        (*pSmsNum)++;
        return TRUE;
    } else
        return FALSE;
}

BOOL UE_AddSmsCardRecord(EnLocType type, const StSmsRecord &record, const WORD &nIndex)
{
    StSmsCardRecord CardRecord;
    memset(&CardRecord, 0x00, sizeof(StSmsCardRecord));

    CardRecord.index = nIndex;
    CardRecord.record = record;

    return UE_AddSmsCardRecord(type, CardRecord);
}

WORD UE_GetUnreadSmsNum(EnLocType loctype)
{
    ASSERT(loctype == LOC_ME || loctype == LOC_UIM || loctype == LOC_MAX);

    if (!g_bSmsSyncComm)
        return 0;

    WORD i;
    WORD nUnreadNum = 0;

    if (loctype == LOC_ME || loctype == LOC_MAX) {
        for (i = 0; i < g_ME_SmsNum; i++) {
            if (g_ME_SmsRecord[i].record.state == SMS_STATE_MT_NOT_READ)
                nUnreadNum++;
        }
    }

    if (loctype == LOC_UIM || loctype == LOC_MAX) {
        for (i = 0; i < g_SM_SmsNum; i++) {
            if (g_SM_SmsRecord[i].record.state == SMS_STATE_MT_NOT_READ)
                nUnreadNum++;
        }
    }

    return nUnreadNum;
}

BOOL UE_ClearSmsFlag(EnLocType loctype, BYTE Clearflag, BOOL bAll)
{
    ASSERT(loctype == LOC_ME || loctype == LOC_UIM || loctype == LOC_MAX);

    if (!g_bSmsSyncComm)
        return FALSE;

    BOOL ret = FALSE;
    WORD i;

    if (loctype == LOC_ME || loctype == LOC_MAX) {
        for (i = 0; i < g_ME_SmsNum; i++) {
            if (g_ME_SmsRecord[i].record.flag & Clearflag) {
                ret = TRUE;
                g_ME_SmsRecord[i].record.flag &= (BYTE)~Clearflag;
                if (!bAll)
                    break;
            }
        }
    }

    if (loctype == LOC_UIM || loctype == LOC_MAX) {
        for (i = 0; i < g_SM_SmsNum; i++) {
            if (g_SM_SmsRecord[i].record.flag & Clearflag) {
                ret = TRUE;
                g_SM_SmsRecord[i].record.flag &= (BYTE)~Clearflag;
                if (!bAll)
                    break;
            }
        }
    }

    return ret;
}

BOOL UE_SmsIsFull(EnLocType loctype)
{
    ASSERT(loctype == LOC_ME || loctype == LOC_UIM || loctype == LOC_MAX);

    if (loctype == LOC_ME)
        return (g_ME_SmsNum == g_ME_SmsMax);
    else if (loctype == LOC_UIM)
        return (g_SM_SmsNum == g_SM_SmsMax);
    else
        return ((g_ME_SmsNum == g_ME_SmsMax) && (g_SM_SmsNum == g_SM_SmsMax));
}

int UE_SmsFindCardRecord(EnLocType loctype, WORD nIndex)
{
    ASSERT(loctype == LOC_ME || loctype == LOC_UIM);

    if (!g_bSmsSyncComm)
        return -1;

    StSmsCardRecord *pSmsRecord = NULL;
    USHORT *pSmsNum = NULL;
    USHORT *pSmsMax = NULL;
    WORD cnt = 0;

    if (loctype == LOC_ME) {
        pSmsRecord = g_ME_SmsRecord;
        pSmsNum = &g_ME_SmsNum;
        pSmsMax = &g_ME_SmsMax;
    } else {
        pSmsRecord = g_SM_SmsRecord;
        pSmsNum = &g_SM_SmsNum;
        pSmsMax = &g_SM_SmsMax;
    }

    ASSERT(pSmsRecord && pSmsNum && pSmsMax && nIndex < *pSmsMax);

    for (cnt = 0; cnt < *pSmsNum; cnt++) {
        if (pSmsRecord[cnt].index == nIndex)
            return cnt;
    }

    return -1;
}

BOOL get_network_info
(
    unsigned int       mcc,
    unsigned int       mnc,
    uinetwk_network_info_s_type  *info_ptr
    /* The returned network info   */
)
{
    const int          number_of_entries = sizeof(uinetwk_network_table) /
                                           sizeof(uinetwk_network_info_s_type);
    int                i                 = 0;
    BOOL            continue_search = TRUE;
    BOOL            found             = FALSE;

    info_ptr->mcc   = mcc;
    info_ptr->mnc   = mnc;

    /*
    ** Search the table for the MCC and MNC.  For now, a linear search
    ** is performed.  Future enhancements may want to incorporate more
    ** sophisticated algorithms.
    */
    while ((continue_search == TRUE) && (i < number_of_entries)) {
        if (mcc == uinetwk_network_table[ i ].mcc) {
            if (mnc == uinetwk_network_table[ i ].mnc) {
                info_ptr->full_name_ptr    = uinetwk_network_table[ i ].full_name_ptr;
                info_ptr->short_name_ptr   = uinetwk_network_table[ i ].short_name_ptr;
                info_ptr->network_type     = uinetwk_network_table[ i ].network_type;
                found                      = TRUE;
                continue_search            = FALSE;
            } else if (mnc < uinetwk_network_table[ i ].mnc) {
                /*
                ** Terminate the search because the MNCs are stored in ascending
                ** order in the table and the MNC being searched is less than the
                ** current MNC in the table.
                */
                continue_search = FALSE;
            }
        } /* if ( mcc == uinetwk_network_table[ i ].mcc ) */
        else if (mcc < uinetwk_network_table[ i ].mcc) {
            /*
            ** Terminate the search because the MCCs are stored in ascending
            ** order in the table and the MCC being searched is less than the
            ** current MCC in the table.
            */
            continue_search = FALSE;
        }

        i++;

    } /* while ( continue_search == TRUE ) */

    if (found == FALSE) {
        info_ptr->full_name_ptr    = NULL;
        info_ptr->short_name_ptr   = NULL;
        info_ptr->network_type     = UI_NETWK_TYPE_UNKNOWN_TYPE;
    }

    return found;

}

int g_DataCardTotalNum = 0; //DATACARD存储器支持的总条数
int g_USimTotalNum = 0;     //USIM存储器支持的总条数
int g_DataCardUsedNum = 0; //DATACARD存储器的已用条数
int g_USimUsedNum = 0;     //USIM存储器的已用条数
StPbRecord* pUSIMPbRecord = NULL; //存储从USIM存储器中读取出来的电话薄记录
StPbRecord* pDataCardPbRecord = NULL;//存储从DATACARD存储器中读取出来的电话薄记录
bool gbPcCardInit = false;  /*PCCARD是否已读取数据至内存中，真为已读取*/
bool gbSimInit = false;     /*SIM是否已读取数据至内存中，真为已读取*/

BOOL g_bDataService = FALSE; //数据连接标志
BYTE g_nDataConnType = 0; //数据连接类型
BOOL g_bConnLimitTip = TRUE; //数据连接限制提示标记
CString    m_str_sms_Prompt = "Reading RUIM SMS,Waiting";
int gCurAutoPref = 0;
