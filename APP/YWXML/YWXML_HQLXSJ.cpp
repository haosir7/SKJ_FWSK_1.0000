/*! \file    YWXML_HQLXSJ.h
   \brief    款机调用的中间件接口 业务 2.28.获取离线相关数据
   \author   myf
   \version  1.0
   \date     2015 
*/

#include "YWXML_HQLXSJ.h"
#include "SysMacDef.h"
#include "arithmetic.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CGetOffLineData::CGetOffLineData(CYWXML_GY &ywxml_gy, UINT32 &wscfpzs, string &wscfpsj, INT64 &wscfpljje):CYWXmlBase(ywxml_gy), m_wscfpzs(wscfpzs), m_wscfpsj(wscfpsj), m_wscfpljje(wscfpljje)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CGetOffLineData::~CGetOffLineData()
{

}

INT32 CGetOffLineData::XmlBuild( )
{
	//添加body节点
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_RootElement, "body");
	m_pXmlConstruct.m_parentElement[1] = m_pXmlConstruct.m_NewElement;

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[1], "input");
	m_pXmlConstruct.m_parentElement[2] = m_pXmlConstruct.m_NewElement;
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "jqbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_jqbh);	//税控收款机编号
	DBG_PRINT(("SKJ_ERRUPINV: jpbh : %s", m_ywxml_gy.m_jqbh.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "nsrsbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_nsrsbh);	//纳税人识别号
	DBG_PRINT(("SKJ_ERRUPINV: nsrsbh : %s", m_ywxml_gy.m_nsrsbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbbh);	//税控设备编号
	DBG_PRINT(("SKJ_ERRUPINV: sksbbh : %s", m_ywxml_gy.m_sksbbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbkl);	//税控设备口令
	DBG_PRINT(("SKJ_ERRUPINV: sksbkl : %s", m_ywxml_gy.m_sksbkl.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fplxdm");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_fplxdm);	//发票类型代码
	DBG_PRINT(("SKJ_LXXXXP: fplxdm : %s", m_ywxml_gy.m_fplxdm.c_str()));

	return XML_SUCCESS;
}

INT32 CGetOffLineData::XmlParse( )
{
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "wscfpzs");
	m_wscfpzs = atoi(m_pXmlParse.GetText().c_str());//未上传发票张数
	DBG_PRINT(("SKJ_HQLXSJ: m_wscfpzs = %u", m_wscfpzs));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "wscfpsj");
	m_wscfpsj = m_pXmlParse.GetText();		//未上传发票累计时间
	DBG_PRINT(("SKJ_HQLXSJ: m_wscfpsj = %s", m_wscfpsj.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "wscfpljje");
	m_wscfpljje = double2int(atof(m_pXmlParse.GetText().c_str())*SUM_EXTENSION);//未上传发票累计金额
	DBG_PRINT(("SKJ_HQLXSJ: m_wscfpljje = %ld", m_wscfpljje));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();		//返回代码
	DBG_PRINT(("SKJ_HQLXSJ: returncode = %s", m_retInfo.m_retCode.c_str()));	

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();		//返回信息
	DBG_PRINT(("SKJ_HQLXSJ: returnmsg = %s", m_retInfo.m_retMsg.c_str()));	
	
	return XML_SUCCESS;
}

