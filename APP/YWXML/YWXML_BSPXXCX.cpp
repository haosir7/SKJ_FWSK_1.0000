/*! \file    YWXML_BSPXXCX.cpp
   \brief    款机调用的中间件接口 业务: 报税盘信息查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#include "YWXML_BSPXXCX.h"
//#include "JSKManageProc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CBspxxcx::CBspxxcx(CYWXML_GY &ywxml_gy, CUserInfo &userInfo):m_UserInfo(userInfo),CYWXmlBase(ywxml_gy)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CBspxxcx::~CBspxxcx()
{

}


INT32 CBspxxcx::XmlBuild()
{
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_RootElement, "body");
	m_pXmlConstruct.m_parentElement[1] = m_pXmlConstruct.m_NewElement;
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[1], "input");
	m_pXmlConstruct.m_parentElement[2] = m_pXmlConstruct.m_NewElement;

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "jqbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_jqbh);
	DBG_PRINT(("m_ywxml_gy.m_jqbh : %s", m_ywxml_gy.m_jqbh.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "bspkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_bspkl);
	DBG_PRINT(("m_ywxml_gy.m_bspkl == %s", m_ywxml_gy.m_bspkl.c_str()));			
	
	return XML_SUCCESS;
}


INT32 CBspxxcx::XmlParse()
{
	INT8 tmpbuff[64];
	
	//添加body节点
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "bspbh");
	m_UserInfo.m_bspbh = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_bspbh : %s", m_UserInfo.m_bspbh.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "nsrsbh");
	m_UserInfo.m_Nsrsbh = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Nsrsbh : %s", m_UserInfo.m_Nsrsbh.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "nsrmc");
	m_UserInfo.m_Nsrmc  = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Nsrmc : %s", m_UserInfo.m_Nsrmc.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "swjgdm");
	m_UserInfo.m_Swjgdm = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Swjgdm : %s", m_UserInfo.m_Swjgdm.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "swjgmc");
	m_UserInfo.m_Swjgmc = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Swjgmc : %s", m_UserInfo.m_Swjgmc.c_str()));

// 	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "dqsz");
// 	m_dqsz = m_pXmlParse.GetText();
// 	DBG_PRINT(("m_dqsz : %s", m_dqsz.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "qysj");
	m_UserInfo.m_qysj = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_qysj : %s", m_UserInfo.m_qysj.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "ffbz");
	m_UserInfo.m_ffbz = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_ffbz : %s", m_UserInfo.m_ffbz.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "bbh");
	m_UserInfo.m_bbh = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_bbh : %s", m_UserInfo.m_bbh.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "kpjh");
	m_UserInfo.m_Kpjhm = atoi(m_pXmlParse.GetText().c_str());
	DBG_PRINT(("m_UserInfo.m_Kpjhm : %u", m_UserInfo.m_Kpjhm));	

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "qylx");
	m_UserInfo.m_Qylx = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Qylx : %s", m_UserInfo.m_Qylx.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "blxx");
	m_UserInfo.m_blxx = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_blxx : %s", m_UserInfo.m_blxx.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();
	DBG_PRINT(("returncode : %s", m_retInfo.m_retCode.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();
	DBG_PRINT(("returnmsg : %s", m_retInfo.m_retMsg.c_str()));
	
	return XML_SUCCESS;

}






