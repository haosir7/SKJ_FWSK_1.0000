/*! \file    YWXML_TJXXCX.cpp
   \brief    款机调用的中间件接口 业务2.19统计信息查询
   \author   lzh
   \version  1.0
   \date     2015
 */

#include "YWXML_TJXXCX.h"
#include "arithmetic.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CTjxxcx::CTjxxcx(CYWXML_GY &ywxml_gy, CTjxxhz &tjxxhz):CYWXmlBase(ywxml_gy), m_tjxxhz(tjxxhz)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CTjxxcx::~CTjxxcx()
{

}

INT32 CTjxxcx::XmlBuild( )
{
	INT8 buf[256];
	memset(buf,0,sizeof(buf));
	
	//添加body节点
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_RootElement, "body");
	m_pXmlConstruct.m_parentElement[1] = m_pXmlConstruct.m_NewElement;

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[1], "input");
	m_pXmlConstruct.m_parentElement[2] = m_pXmlConstruct.m_NewElement;

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "jqbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_jqbh);	//税控收款机编号
	DBG_PRINT(("SKJ_TJXXCX: jpbh : %s", m_ywxml_gy.m_jqbh.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "nsrsbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_nsrsbh);	//纳税人识别号
	DBG_PRINT(("SKJ_TJXXCX: nsrsbh : %s", m_ywxml_gy.m_nsrsbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbbh);	//税控设备编号
	DBG_PRINT(("SKJ_TJXXCX: sksbbh : %s", m_ywxml_gy.m_sksbbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbkl);	//税控设备口令
	DBG_PRINT(("SKJ_TJXXCX: sksbkl : %s", m_ywxml_gy.m_sksbkl.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fplxdm");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_fplxdm);	//发票类型代码
	DBG_PRINT(("SKJ_TJXXCX: fplxdm : %s", m_ywxml_gy.m_fplxdm.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "qsrq");
	memset(buf,0,sizeof(buf));
	sprintf(buf, "%u", m_tjxxhz.m_Qsrq);
	m_pXmlConstruct.AddText(buf);	//起始日期
	DBG_PRINT(("SKJ_TJXXCX: qsrq : %s", buf));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "zzrq");
	memset(buf,0,sizeof(buf));
	sprintf(buf, "%u", m_tjxxhz.m_Jzrq);
	m_pXmlConstruct.AddText(buf);	//终止日期
	DBG_PRINT(("SKJ_TJXXCX: zzrq : %s", buf));
	
	return XML_SUCCESS;
}

INT32 CTjxxcx::XmlParse( )
{
	INT32 temp_count = 0;//记录统计信息条数
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "fplxdm");
	m_ywxml_gy.m_fplxdm = m_pXmlParse.GetText();	//发票类型代码
	DBG_PRINT(("SKJ_TJXXCX: fplxdm = %s", m_ywxml_gy.m_fplxdm.c_str()));	

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "tjxx");
	m_pXmlParse.m_parentElement[3] = m_pXmlParse.m_Child;
	temp_count = atoi(m_pXmlParse.GetAttr("count").c_str());//统计信息条数
	DBG_PRINT(("SKJ_TJXXCX: tjxx_count = %d", temp_count));

	for (INT32 temp_i=0; temp_i<temp_count; temp_i++)
	{
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[3], "group",temp_i);
		m_pXmlParse.m_parentElement[4] = m_pXmlParse.m_Child;
		
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "qsrq");
		m_tjxxhz.m_Qsrq = atoi(m_pXmlParse.GetText().c_str());	//起始日期
		DBG_PRINT(("SKJ_TJXXCX: qsrq = %u", m_tjxxhz.m_Qsrq));	

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "jzrq");
		m_tjxxhz.m_Jzrq = atoi(m_pXmlParse.GetText().c_str());	//截止日期
		DBG_PRINT(("SKJ_TJXXCX: jzrq = %u", m_tjxxhz.m_Jzrq));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "qckcfs");
		m_tjxxhz.m_Qckcfs = atoi(m_pXmlParse.GetText().c_str());	//期初库存份数
		DBG_PRINT(("SKJ_TJXXCX: qckcfs = %u", m_tjxxhz.m_Qckcfs));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "lgfpfs");
		m_tjxxhz.m_Lgfpfs = atoi(m_pXmlParse.GetText().c_str());	//领购发票份数
		DBG_PRINT(("SKJ_TJXXCX: lgfpfs = %u", m_tjxxhz.m_Lgfpfs));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "thfpfs");
		m_tjxxhz.m_Thfpfs = atoi(m_pXmlParse.GetText().c_str());	//退回发票份数
		DBG_PRINT(("SKJ_TJXXCX: thfpfs = %u", m_tjxxhz.m_Thfpfs));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "zsfpfs");
		m_tjxxhz.m_Zsfpfs = atoi(m_pXmlParse.GetText().c_str());	//正数发票份数
		DBG_PRINT(("SKJ_TJXXCX: zsfpfs = %u", m_tjxxhz.m_Zsfpfs));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "zffpfs");
		m_tjxxhz.m_Zffpfs = atoi(m_pXmlParse.GetText().c_str());	//正废发票份数
		DBG_PRINT(("SKJ_TJXXCX: zffpfs = %u", m_tjxxhz.m_Zffpfs));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fsfpfs");
		m_tjxxhz.m_Fsfpfs = atoi(m_pXmlParse.GetText().c_str());	//负数发票份数
		DBG_PRINT(("SKJ_TJXXCX: fsfpfs = %u", m_tjxxhz.m_Fsfpfs));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fffpfs");
		m_tjxxhz.m_Fffpfs = atoi(m_pXmlParse.GetText().c_str());	//负废发票份数
		DBG_PRINT(("SKJ_TJXXCX: fffpfs = %u", m_tjxxhz.m_Fffpfs));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "kffpfs");
		m_tjxxhz.m_Kffpfs = atoi(m_pXmlParse.GetText().c_str());	//空废发票份数
		DBG_PRINT(("SKJ_TJXXCX: kffpfs = %u", m_tjxxhz.m_Kffpfs));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "qmkcfs");
		m_tjxxhz.m_Qmkcfs = atoi(m_pXmlParse.GetText().c_str());	//期末库存份数
		DBG_PRINT(("SKJ_TJXXCX: qmkcfs = %u", m_tjxxhz.m_Qmkcfs));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "zsfpljje");
		m_tjxxhz.m_Zsfpljje = double2int(atof( m_pXmlParse.GetText().c_str() ) * SUM_EXTENSION);	//正数发票累计金额
		DBG_PRINT(("SKJ_TJXXCX: zsfpljje = %lld", m_tjxxhz.m_Zsfpljje));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "zsfpljse");
		m_tjxxhz.m_Zsfpljse = double2int(atof( m_pXmlParse.GetText().c_str() ) * SUM_EXTENSION);	//正数发票累计税额
		DBG_PRINT(("SKJ_TJXXCX: zsfpljse = %lld", m_tjxxhz.m_Zsfpljse));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "zffpljje");
		m_tjxxhz.m_Zffpljje = double2int(atof( m_pXmlParse.GetText().c_str() ) * SUM_EXTENSION);	//正废发票累计金额
		DBG_PRINT(("SKJ_TJXXCX: zffpljje = %lld", m_tjxxhz.m_Zffpljje));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "zffpljse");
		m_tjxxhz.m_Zffpljse = double2int(atof( m_pXmlParse.GetText().c_str() ) * SUM_EXTENSION);	//正废发票累计税额
		DBG_PRINT(("SKJ_TJXXCX: zffpljse = %lld", m_tjxxhz.m_Zffpljse));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fsfpljje");
		m_tjxxhz.m_Fsfpljje = double2int(atof( m_pXmlParse.GetText().c_str() ) * SUM_EXTENSION);	//负数发票累计金额
		DBG_PRINT(("SKJ_TJXXCX: fsfpljje = %lld", m_tjxxhz.m_Fsfpljje));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fsfpljse");
		m_tjxxhz.m_Fsfpljse = double2int(atof( m_pXmlParse.GetText().c_str() ) * SUM_EXTENSION);	//负数发票累计税额
		DBG_PRINT(("SKJ_TJXXCX: fsfpljse = %lld", m_tjxxhz.m_Fsfpljse));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fffpljje");
		m_tjxxhz.m_Fffpljje = double2int(atof( m_pXmlParse.GetText().c_str() ) * SUM_EXTENSION);	//负废发票累计金额
		DBG_PRINT(("SKJ_TJXXCX: fffpljje = %lld", m_tjxxhz.m_Fffpljje));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fffpljse");
		m_tjxxhz.m_Fffpljse = double2int(atof( m_pXmlParse.GetText().c_str() ) * SUM_EXTENSION);	//负废发票累计税额
		DBG_PRINT(("SKJ_TJXXCX: fffpljse = %lld", m_tjxxhz.m_Fffpljse));
		
	}
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();		//返回代码
	DBG_PRINT(("SKJ_TJXXCX: returncode = %s", m_retInfo.m_retCode.c_str()));	

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();		//返回信息
	DBG_PRINT(("SKJ_TJXXCX: returnmsg = %s", m_retInfo.m_retMsg.c_str()));	
	
	return XML_SUCCESS;
}


