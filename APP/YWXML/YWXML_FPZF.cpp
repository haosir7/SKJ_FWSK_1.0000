/*! \file     YWXML_FPZF.cpp
   \brief    款机调用的中间件接口 业务: 发票作废
   \author   myf
   \version  1.0
   \date     2015 
*/

#include "YWXML_FPZF.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CFpzf::CFpzf(CYWXML_GY &ywxml_gy, UINT8 zflx, CInvHead &InvInfo):m_zflx(zflx), m_InvInfo(InvInfo), CYWXmlBase(ywxml_gy)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CFpzf::~CFpzf()
{

}


INT32 CFpzf::XmlBuild()
{
	UINT8 i = 0;
	INT8 Buf[64];
	//添加到发票明细
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_RootElement, "body");
	m_pXmlConstruct.m_parentElement[1] = m_pXmlConstruct.m_NewElement;
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[1], "input");
	m_pXmlConstruct.m_parentElement[2] = m_pXmlConstruct.m_NewElement;

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "jqbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_jqbh);
	DBG_PRINT(("m_ywxml_gy.m_jqbh : %s", m_ywxml_gy.m_jqbh.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "nsrsbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_nsrsbh);	
	DBG_PRINT(("m_ywxml_gy.nsrsbh : %s", m_ywxml_gy.m_nsrsbh.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "nsrmc");//纳税人名称
	m_pXmlConstruct.AddText(m_ywxml_gy.m_nsrmc);	
	DBG_PRINT(("m_ywxml_gy.nsrmc : %s", m_ywxml_gy.m_nsrmc.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbbh);	
	DBG_PRINT(("m_ywxml_gy.sksbbh : %s", m_ywxml_gy.m_sksbbh.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbkl);	
	DBG_PRINT(("m_ywxml_gy.sksbkl : %s", m_ywxml_gy.m_sksbkl.c_str()));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "zskl");
	m_pXmlConstruct.AddText(m_InvInfo.m_zskl);	
	DBG_PRINT(("m_InvInfo.m_zskl : %s", m_InvInfo.m_zskl.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fplxdm");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_fplxdm);	
	DBG_PRINT(("m_ywxml_gy.fplxdm : %s", m_ywxml_gy.m_fplxdm.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "zflx");
	memset(Buf, 0, sizeof(Buf));
	sprintf(Buf, "%u", m_zflx);
	m_pXmlConstruct.AddText(Buf);	
	DBG_PRINT(("m_InvInfo.zflx : %s", Buf));


	if(m_zflx == 0)
	{
		m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fpdm");
		m_pXmlConstruct.AddText("");	
		
		m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fphm");
		m_pXmlConstruct.AddText("");
	}
	else
	{
		m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fpdm");
		m_pXmlConstruct.AddText(m_InvInfo.m_fpdm);	
		DBG_PRINT(("m_InvInfo.m_fpdm == %s", m_InvInfo.m_fpdm.c_str()));
		
		m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fphm");
		memset(Buf, 0, sizeof(Buf));
		sprintf(Buf, "%u", m_InvInfo.m_fphm);
		m_pXmlConstruct.AddText(Buf);
		DBG_PRINT(("m_InvInfo.m_fphm : %s", Buf));

	}

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "zfr");	//作废人
	m_pXmlConstruct.AddText(m_InvInfo.m_sky);	

	return XML_SUCCESS;
}

INT32 CFpzf::XmlParse()
{
	string tmpStr("");

	//添加body节点
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "fplxdm");
	m_InvInfo.m_fplb = (UINT8)atoi(m_pXmlParse.GetText().c_str());
	DBG_PRINT(("m_InvInfo.m_fplb : %u", m_InvInfo.m_fplb));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "fpdm");
	m_InvInfo.m_fpdm = m_pXmlParse.GetText();
	DBG_PRINT(("m_InvInfo.m_fpdm : %s", m_InvInfo.m_fpdm.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "fphm");
	m_InvInfo.m_fphm = atoi(m_pXmlParse.GetText().c_str());
	DBG_PRINT(("m_InvInfo.m_fhdm : %u", m_InvInfo.m_fphm));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "zfrq");
	tmpStr = m_pXmlParse.GetText();
	if(tmpStr.length() >= 14)
	{
		string DateTemp = tmpStr.substr(0, 8);
		string TimeTemp = tmpStr.substr(8, 6);
		m_InvInfo.m_zfrq = atoi(DateTemp.c_str());
		m_InvInfo.m_kprq = atoi(DateTemp.c_str());
		m_InvInfo.m_zfsj = atoi(TimeTemp.c_str());
		m_InvInfo.m_kpsj = atoi(TimeTemp.c_str());
	}
	DBG_PRINT(("m_InvInfo.m_zfrq : %u", m_InvInfo.m_zfrq));
	DBG_PRINT(("m_InvInfo.m_zfsj : %u", m_InvInfo.m_zfsj));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "qmz");
	m_InvInfo.m_casign = m_pXmlParse.GetText();
	DBG_PRINT(("m_InvInfo.m_casign : %s", m_InvInfo.m_casign.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();
	DBG_PRINT(("returncode : %s", m_retInfo.m_retCode.c_str()));
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();
	DBG_PRINT(("returnmsg : %s", m_retInfo.m_retMsg.c_str()));
	
	return XML_SUCCESS;

}






