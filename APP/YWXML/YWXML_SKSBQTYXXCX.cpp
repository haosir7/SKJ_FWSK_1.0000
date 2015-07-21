/*! \file     YWXML_SKSBQTYXXCX.cpp
   \brief    款机调用的中间件接口 业务: 税控设备其他信息查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#include "YWXML_SKSBQTYXXCX.h"
// #include "JSKMakeInvoice.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CSksbqtxxcx::CSksbqtxxcx(CYWXML_GY &ywxml_gy, UINT8 xxlx, string &sksbxx):m_xxlx(xxlx), m_sksbxx(sksbxx), CYWXmlBase(ywxml_gy)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CSksbqtxxcx::~CSksbqtxxcx()
{

}

INT32 CSksbqtxxcx::XmlBuild()
{
	INT8 Buf[64];
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_RootElement, "body");
	m_pXmlConstruct.m_parentElement[1] = m_pXmlConstruct.m_NewElement;
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[1], "input");
	m_pXmlConstruct.m_parentElement[2] = m_pXmlConstruct.m_NewElement;

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "jqbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_jqbh);
	DBG_PRINT(("m_ywxml_gy.m_jqbh : %s", m_ywxml_gy.m_jqbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbkl);	
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "xxlx");
	memset(Buf, 0, sizeof(Buf));
	sprintf(Buf, "%u", m_xxlx);
	m_pXmlConstruct.AddText(Buf);	
	
	return XML_SUCCESS;
}

INT32 CSksbqtxxcx::XmlParse()
{
	INT8 Buf[64];
	//添加body节点
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "sksbxx");
	m_sksbxx = m_pXmlParse.GetText();
	DBG_PRINT(("m_sksbxx : %s", m_sksbxx.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();
	DBG_PRINT(("returncode : %s", m_retInfo.m_retCode.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();
	DBG_PRINT(("returnmsg : %s", m_retInfo.m_retMsg.c_str()));
	
	return XML_SUCCESS;

}




