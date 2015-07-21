 /*! \file    YWXML_SKSBBHCX.cpp
   \brief    款机调用的中间件接口 业务2.1 税控设备编号查询
   \author   YY
   \version  1.0
   \date     2015
 */

#include "YWXML_SKSBBHCX.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CSksbbhcx::CSksbbhcx(CYWXML_GY &ywxml_gy, string &sksbbh, string &bspbh, string &qtxx):
					m_sksbbh(sksbbh), m_bspbh(bspbh), m_qtxx(qtxx),CYWXmlBase(ywxml_gy)
{
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CSksbbhcx::~CSksbbhcx()
{

}

INT32 CSksbbhcx::XmlBuild()
{
	//添加body节点
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_RootElement, "body");
	m_pXmlConstruct.m_parentElement[1] = m_pXmlConstruct.m_NewElement;

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[1], "input");
	m_pXmlConstruct.m_parentElement[2] = m_pXmlConstruct.m_NewElement;

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "jqbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_jqbh);
	DBG_PRINT(("m_ywxml_gy.m_jqbh : %s", m_ywxml_gy.m_jqbh.c_str()));
	
	return XML_SUCCESS;
}

INT32 CSksbbhcx::XmlParse()
{
	//添加body节点
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "sksbbh");
	m_sksbbh = m_pXmlParse.GetText();
	DBG_PRINT(("sksbbh : %s", m_sksbbh.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "bspbh");
	m_bspbh = m_pXmlParse.GetText();
	DBG_PRINT(("bspbh : %s", m_bspbh.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "qtxx");
	m_qtxx = m_pXmlParse.GetText();
	DBG_PRINT(("m_qtxx : %s", m_qtxx.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();
	DBG_PRINT(("returncode : %s", m_retInfo.m_retCode.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();
	DBG_PRINT(("returnmsg : %s", m_retInfo.m_retMsg.c_str()));
	
	return XML_SUCCESS;
}



