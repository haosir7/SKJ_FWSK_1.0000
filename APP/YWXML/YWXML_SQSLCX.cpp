/*! \file    YWXML_SQSLCX.cpp
   \brief    款机调用的中间件接口 业务: 授权税率查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#include "YWXML_SQSLCX.h"
#include "base64.h"
#include "SysMacDef.h"
#include "arithmetic.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CTaxInfo::CTaxInfo()
{
	m_TaxCoun = 0;
// 	for (UINT8 i=0; i<MAX_TAX_NUM; i++)
// 	{
// 		memset(&m_Tax[i], 0, sizeof(m_Tax));
// 	}
}

CTaxInfo::~CTaxInfo()
{

}



//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CSqslcx::CSqslcx(CYWXML_GY &ywxml_gy, CTaxInfo &TaxInfo):m_TaxInfo(TaxInfo), CYWXmlBase(ywxml_gy)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CSqslcx::~CSqslcx()
{

}


INT32 CSqslcx::XmlBuild()
{
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_RootElement, "body");
	m_pXmlConstruct.m_parentElement[1] = m_pXmlConstruct.m_NewElement;
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[1], "input");
	m_pXmlConstruct.m_parentElement[2] = m_pXmlConstruct.m_NewElement;

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "jqbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_jqbh);
	DBG_PRINT(("m_ywxml_gy.m_jqbh : %s", m_ywxml_gy.m_jqbh.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "nsrsbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_nsrsbh);	
	DBG_PRINT(("m_ywxml_gy.m_nsrsbh : %s", m_ywxml_gy.m_nsrsbh.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbbh);
	DBG_PRINT(("m_ywxml_gy.m_sksbbh : %s", m_ywxml_gy.m_sksbbh.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbkl);	
	DBG_PRINT(("m_ywxml_gy.m_sksbkl : %s", m_ywxml_gy.m_sksbkl.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fplxdm");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_fplxdm);	
	DBG_PRINT(("m_ywxml_gy.m_fplxdm : %s", m_ywxml_gy.m_fplxdm.c_str()));
	
	return XML_SUCCESS;
}


INT32 CSqslcx::XmlParse()
{
	INT8 buf[64];
	UINT8 count = 0;
	UINT8 slNum = 0;

	//添加body节点
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "fplxdm");
	m_ywxml_gy.m_fplxdm = m_pXmlParse.GetText();
	DBG_PRINT(("m_ywxml_gy.m_fplxdm : %s", m_ywxml_gy.m_fplxdm.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "sqslxx");
	m_pXmlParse.m_parentElement[3] = m_pXmlParse.m_Child;
	slNum = (UINT8)atoi(m_pXmlParse.GetAttr("count").c_str());
	m_TaxInfo.m_TaxCoun = slNum;
	DBG_PRINT(("m_TaxInfo.m_TaxCoun : %u", m_TaxInfo.m_TaxCoun));

	for (count=0; count<slNum; count++)
	{
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[3], "group", count);
		m_pXmlParse.m_parentElement[4] = m_pXmlParse.m_Child;

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "sl");
		DBG_PRINT(("m_TaxInfo.m_Tax[%u].m_sl : %s", count, m_pXmlParse.GetText().c_str()));
		m_TaxInfo.m_Tax[count].m_sl = double2int(atof(m_pXmlParse.GetText().c_str())*SUM_EXTENSION);
		DBG_PRINT(("m_TaxInfo.m_Tax[%u].m_sl : %u", count, m_TaxInfo.m_Tax[count].m_sl));

	}
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();
	DBG_PRINT(("returncode : %s", m_retInfo.m_retCode.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();
	DBG_PRINT(("returnmsg : %s", m_retInfo.m_retMsg.c_str()));
	
	return XML_SUCCESS;

}






