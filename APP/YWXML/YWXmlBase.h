#ifndef __YW_XML_BASE_H
#define __YW_XML_BASE_H

#include <string>
#include <cstring>
#include "IncludeMe.h"
#include "xmlbase.h"
#include "pos_debug.h"
#include "YWXMLGY.h"


#define KJ_XML_REQUEST_FILE		"KJXmlReqFile.xml"
#define KJ_XML_RESPONSE_FILE	"KJXmlRspFile.xml"


#define MAX_SEND_BUF_LEN		1024*5

#define XML_BUSINESS		"business"
#define XML_BUSINESS_ID		"id"


#define MAX_TAX_NUM			10
#define INVVOL_MAX_NUM  100            //发票卷最大条数20150519linzihao


#define BUSINESS_YWID_SKSBBH		"SKSBBH"			//税控设备编号查询
#define BUSINESS_YWID_SKPXXCX		"SKPXXCX"			//税控设备信息查询
#define BUSINESS_YWID_SKSBQTYXXCX	"SKSBQTYXXCX"		//税控设备其他信息查询
#define BUSINESS_YWID_BSPXXCX		"BSPXXCX"			//报税盘信息查询
#define BUSINESS_YWID_JKSJCX		"JKSJCX"			//监控信息查询
#define BUSINESS_YWID_SQSLCX		"SQSLCX"			//授权税率查询
#define BUSINESS_YWID_GPXXCX		"GPXXCX"			//购票信息查询
#define BUSINESS_YWID_GPXXXP		"GPXXXP"			//购票信息写盘
#define BUSINESS_YWID_GPXXXPJS		"GPXXXPJS"			//购票信息写盘解锁
#define BUSINESS_YWID_KLBG			"KLBG"				//口令变更
#define BUSINESS_YWID_FPKJ			"FPKJ"				//发票开具
#define BUSINESS_YWID_FPZF			"FPZF"				//发票作废
#define BUSINESS_YWID_FPCX			"FPCX"				//发票查询
#define BUSINESS_YWID_SKPBSP		"SKPBSP"			//税控设备报税盘组合操作
#define BUSINESS_YWID_BSPFPCX		"BSPFPCX"			//报税盘发票查询
#define BUSINESS_YWID_BSPFPFF		"BSPFPFF"			//发票分发
#define BUSINESS_YWID_BSPFPSH		"BSPFPSH"			//发票收回
#define BUSINESS_YWID_SJCB			"SJCB"				//数据抄报
#define BUSINESS_YWID_JKHC			"JKHC"				//监控回传
#define BUSINESS_YWID_TJXXCX		"TJXXCX"			//发票统计信息查询
#define BUSINESS_YWID_LXXXXP		"LXXXXP"			//离线信息写盘
#define BUSINESS_YWID_FPSC			"FPSC"				//发票上传
#define BUSINESS_YWID_FPGX			"FPGX"				//发票更新
#define BUSINESS_YWID_FPDR			"FPDR"				//发票读入
#define BUSINESS_YWID_FPBL			"FPBL"				//发票补录
#define BUSINESS_YWID_FJTH			"FJTH"				//分机退回
#define BUSINESS_YWID_BGZSKL        "BGZSKL"            //变更证书口令

#define BUSINESS_NETID_FPSC			"FPSSSC"			//发票实时上传
#define BUSINESS_NETID_FPSCJGHQ		"FPSCJGHQ"			//发票上传结果获取
#define BUSINESS_NETID_WLCB			"WLCB"				//网络抄报
#define BUSINESS_NETID_QLJS			"QLJS"				//清零解锁
#define BUSINESS_NETID_LXXXSC		"LXXXSC"			//离线信息上传
#define BUSINESS_NETID_WLLQFP		"WLLQFP"			//网络领取发票
#define BUSINESS_NETID_WLLQFPJGQR	"WLLQFPJGQR"		//网络领取发票结果确认
#define BUSINESS_NETID_QYCSXZKJ		"qycsxz_kj"			//企业参数下载


#define XML_SUCCESS				0
#define XML_FAILURE				-1
#define XML_INTERNAL_ERR_NO		-110

class CReturnInfo
{
public:
	string m_retCode;
	string m_retMsg;
public:
	CReturnInfo(){m_retCode="0";m_retMsg="成功";}
};

class CYWXmlBase  
{
	
public:
	CYWXmlBase(CYWXML_GY &ywxml_gy);
	~CYWXmlBase();

	INT32 YWXml_Proc(string &strErr);
	INT32 NETXml_Proc(string &strErr);
	void SetServInfo(string ip, string port, string path);

	CYWXML_GY &m_ywxml_gy;

protected:
	XMLConstruct m_pXmlConstruct;
	XMLParse m_pXmlParse;
	CReturnInfo m_retInfo;
	string m_ywlx;

	string m_servip;
	string m_servport;
	string m_servpath;
	string m_pin;
	string m_Err;
	string m_nsrsbh;
	string m_servId;

	virtual INT32 XmlBuild()=0;
	virtual INT32 XmlParse()=0;
	
private:
	INT32 BuildProc();
	INT32 ParseProc();

};


#endif

