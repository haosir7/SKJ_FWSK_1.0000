 /*! \file    YWXML_SKPXXCX.cpp
   \brief    款机调用的中间件接口 业务2.2.税控设备信息查询
   \author   YY
   \version  1.0
   \date     2015
 */

#include "YWXML_SKPXXCX.h"
//#include "JSKManageProc.h"
#include "TDateTime.h"
#include "CUserInfo.h"
#include "CInvKind.h"



#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CSkpxxcx::CSkpxxcx(CYWXML_GY &ywxml_gy, CUserInfo &UserInfo, string &strCurTime):m_UserInfo(UserInfo), m_strCurTime(strCurTime), CYWXmlBase(ywxml_gy)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CSkpxxcx::~CSkpxxcx()
{

}

INT32 CSkpxxcx::XmlBuild()
{
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_RootElement, "body");
	m_pXmlConstruct.m_parentElement[1] = m_pXmlConstruct.m_NewElement;
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[1], "input");
	m_pXmlConstruct.m_parentElement[2] = m_pXmlConstruct.m_NewElement;
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "jqbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_jqbh);
	DBG_PRINT(("m_ywxml_gy.m_jqbh : %s", m_ywxml_gy.m_jqbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbkl);
	DBG_PRINT(("m_ywxml_gy.m_sksbkl == %s", m_ywxml_gy.m_sksbkl.c_str()));			
	
	return XML_SUCCESS;
}

INT32 CSkpxxcx::XmlParse()
{
	INT8 tmpBuff[128];
	//添加body节点
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "sksbbh");
	m_UserInfo.m_Jspsbh = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Jspsbh : %s", m_UserInfo.m_Jspsbh.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "nsrsbh");
	m_UserInfo.m_Nsrsbh = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Nsrsbh : %s", m_UserInfo.m_Nsrsbh.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "nsrmc");
	m_UserInfo.m_Nsrmc = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Nsrmc : %s", m_UserInfo.m_Nsrmc.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "swjgdm");
	m_UserInfo.m_Swjgdm = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Swjgdm : %s", m_UserInfo.m_Swjgdm.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "swjgmc");
	m_UserInfo.m_Swjgmc = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_Swjgmc : %s", m_UserInfo.m_Swjgmc.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "fplxdm");
	m_UserInfo.m_fplxdm = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_fplxdm : %s", m_UserInfo.m_fplxdm.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "dqsz");//当前时钟
	m_strCurTime = m_pXmlParse.GetText();
	DBG_PRINT(("m_strCurTime : %s", m_strCurTime.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "qysj");
	m_UserInfo.m_qysj = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_qysj : %s", m_UserInfo.m_qysj.c_str()));

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

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "qtkzxx");//其它扩展信息
	m_UserInfo.m_qtkzxx = m_pXmlParse.GetText();
	DBG_PRINT(("m_UserInfo.m_qtkzxx : %s", m_UserInfo.m_qtkzxx.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();
	DBG_PRINT(("returncode : %s", m_retInfo.m_retCode.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();
	DBG_PRINT(("returnmsg : %s", m_retInfo.m_retMsg.c_str()));
	
	return XML_SUCCESS;

}






