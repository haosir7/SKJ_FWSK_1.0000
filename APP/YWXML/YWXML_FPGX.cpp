 /*! \file    YWXML_FPGX.cpp
   \brief    款机调用的中间件接口 业务2.22.发票更新
   \author   YY
   \version  1.0
   \date     2015
 */

#include "YWXML_FPGX.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CFpgx::CFpgx(CYWXML_GY &ywxml_gy, string strScqrbw):CYWXmlBase(ywxml_gy), m_strScqrbw(strScqrbw)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CFpgx::~CFpgx()
{

}

INT32 CFpgx::XmlBuild( )
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
	DBG_PRINT(("SKJ_FPGX: jpbh : %s", m_ywxml_gy.m_jqbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "nsrsbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_nsrsbh);	//纳税人识别号
	DBG_PRINT(("SKJ_FPGX: nsrsbh : %s", m_ywxml_gy.m_nsrsbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbbh);	//税控设备编号
	DBG_PRINT(("SKJ_FPGX: sksbbh : %s", m_ywxml_gy.m_sksbbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbkl);	//税控设备口令
	DBG_PRINT(("SKJ_FPGX: sksbkl : %s", m_ywxml_gy.m_sksbkl.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fplxdm");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_fplxdm);	//发票类型代码
	DBG_PRINT(("SKJ_FPGX: fplxdm : %s", m_ywxml_gy.m_fplxdm.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "scqrbw");
	m_pXmlConstruct.AddText(m_strScqrbw);	//上传成功确认报文
	DBG_PRINT(("SKJ_FPGX: scqrbw : %s", m_strScqrbw.c_str()));
	
	return XML_SUCCESS;
}

INT32 CFpgx::XmlParse( )
{
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();		//返回代码
	DBG_PRINT(("SKJ_FPGX: returncode = %s", m_retInfo.m_retCode.c_str()));	

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();		//返回信息
	DBG_PRINT(("SKJ_FPGX: returnmsg = %s", m_retInfo.m_retMsg.c_str()));	

	
	return XML_SUCCESS;
}

