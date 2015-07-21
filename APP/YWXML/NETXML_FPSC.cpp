/*! \file     NETXML_FPSC.cpp
   \brief    款机调用的中间件接口 业务: 发票上传
   \author   myf
   \version  1.0
   \date     2015 
*/

#include "NETXML_FPSC.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CFpsc::CFpsc(CYWXML_GY &ywxml_gy,const string &Fpmx, UINT32 Fpzs, string &Slxlh):m_Fpmx(Fpmx), m_Fpzs(Fpzs),m_Slxlh(Slxlh), CYWXmlBase(ywxml_gy)
{
	m_FpCount = 1;
	m_Count = 1;
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CFpsc::~CFpsc()
{

}


INT32 CFpsc::XmlBuild()
{

	UINT8 i = 0;
	INT8 Buf[64];

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_RootElement, "body");
	m_pXmlConstruct.m_parentElement[1] = m_pXmlConstruct.m_NewElement;
	memset(Buf, 0, sizeof(Buf));
	sprintf(Buf, "%u", m_FpCount);
	m_pXmlConstruct.AddAttr("count", Buf);
	DBG_PRINT(("m_FpCount : %s", Buf));

	m_pXmlConstruct.AddAttr("skph", m_ywxml_gy.m_sksbbh);	
	m_pXmlConstruct.AddAttr("nsrsbh", m_ywxml_gy.m_nsrsbh);
	m_pXmlConstruct.AddAttr("kpjh", m_ywxml_gy.m_kpjh);

	DBG_PRINT(("skph : %s", m_ywxml_gy.m_sksbbh.c_str()));
	DBG_PRINT(("nsrsbh : %s", m_ywxml_gy.m_nsrsbh.c_str()));
	DBG_PRINT(("kpjh : %s", m_ywxml_gy.m_kpjh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[1], "input");
	m_pXmlConstruct.m_parentElement[2] = m_pXmlConstruct.m_NewElement;

	for (i=1; i<=m_FpCount; i++)
	{
		//得到group节点
		m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "group");
		m_pXmlConstruct.m_parentElement[3] = m_pXmlConstruct.m_NewElement;
		memset(Buf, 0, sizeof(Buf));
		sprintf(Buf, "%u", i);
		m_pXmlConstruct.AddAttr("xh", Buf);
		DBG_PRINT(("group_xh : %s", Buf));

		m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[3], "fplxdm");
		m_pXmlConstruct.AddText(m_ywxml_gy.m_fplxdm);	
		DBG_PRINT(("fplxdm : %s", m_ywxml_gy.m_fplxdm.c_str()));

		m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[3], "fpzs");
		memset(Buf, 0, sizeof(Buf));
		sprintf(Buf, "%u", m_Fpzs);
		m_pXmlConstruct.AddText(Buf);	
		DBG_PRINT(("fpzs : %s", Buf));
	
		m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[3], "fpmx");
		m_pXmlConstruct.AddText(m_Fpmx, XML_SET_TEXT_CDATA);	
		//DBG_PRINT(("Fpmx : %s", m_Fpmx.c_str()));
	
	}

	return XML_SUCCESS;
}

INT32 CFpsc::XmlParse()
{
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;
	
	m_Count = (UINT8)atoi(m_pXmlParse.GetAttr("count").c_str());
	DBG_PRINT(("m_Count : %u", m_Count));
// 	m_ywxml_gy.m_sksbbh = m_pXmlParse.GetAttr("skph");
// 	m_ywxml_gy.m_nsrsbh = m_pXmlParse.GetAttr("nsrsbh");
// 	DBG_PRINT(("skph : %s", m_ywxml_gy.m_sksbbh.c_str()));
// 	DBG_PRINT(("nsrsbh : %s", m_ywxml_gy.m_nsrsbh.c_str()));

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;
	
	for (UINT8 i=0; i<m_Count; i++)
	{
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "group", i);
		m_pXmlParse.m_parentElement[3] = m_pXmlParse.m_Child;

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[3], "fplxdm");
		m_ywxml_gy.m_fplxdm = m_pXmlParse.GetText();
		DBG_PRINT(("fplxdm : %s", m_ywxml_gy.m_fplxdm.c_str()));
			
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[3], "returncode");
		m_retInfo.m_retCode = m_pXmlParse.GetText();
		DBG_PRINT(("returncode : %s", m_retInfo.m_retCode.c_str()));
		
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[3], "returnmsg");
		m_retInfo.m_retMsg = m_pXmlParse.GetText();
		DBG_PRINT(("returnmsg : %s", m_retInfo.m_retMsg.c_str()));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[3], "slxlh");
		m_Slxlh = m_pXmlParse.GetText();
		DBG_PRINT(("slxlh : %s", m_Slxlh.c_str()));

	}
	
	return XML_SUCCESS;

}





