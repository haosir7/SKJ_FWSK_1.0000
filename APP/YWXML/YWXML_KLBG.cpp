/*! \file    YWXML_KLBG.cpp
   \brief    款机调用的中间件接口 业务: 报税盘信息查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#include "YWXML_KLBG.h"
//#include "JSKManageProc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CKlbg::CKlbg(CYWXML_GY &ywxml_gy, string ykl, string xkl): m_ykl(ykl), m_xkl(xkl), CYWXmlBase(ywxml_gy)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CKlbg::~CKlbg()
{

}


INT32 CKlbg::XmlBuild()
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
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbbh);	
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "ykl");
	m_pXmlConstruct.AddText(m_ykl);	
	DBG_PRINT(("m_ykl : %s", m_ykl.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "xkl");
	m_pXmlConstruct.AddText(m_xkl);	
	DBG_PRINT(("m_xkl : %s", m_xkl.c_str()));

	return XML_SUCCESS;
}

INT32 CKlbg::XmlParse()
{
	//添加body节点
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();
	DBG_PRINT(("returncode : %s", m_retInfo.m_retCode.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();
	DBG_PRINT(("returnmsg : %s", m_retInfo.m_retMsg.c_str()));
	
	return XML_SUCCESS;
	
}






