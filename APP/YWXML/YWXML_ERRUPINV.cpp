/*! \file    YWXML_ERRUPINV.h
   \brief    款机调用的中间件接口 业务 获取上传出错的发票信息
   \author   myf
   \version  1.0
   \date     2015 
*/

#include "YWXML_ERRUPINV.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"

CErrInvInfo::CErrInvInfo()
{
	m_ErrUpNum = 0;	
}

CErrInvInfo::~CErrInvInfo()
{
	
}


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CErrUpInv::CErrUpInv(CYWXML_GY &ywxml_gy, CErrInvInfo &ErrInvInfo):CYWXmlBase(ywxml_gy), m_ErrInvInfo(ErrInvInfo)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CErrUpInv::~CErrUpInv()
{

}

INT32 CErrUpInv::XmlBuild( )
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

	return XML_SUCCESS;
}

INT32 CErrUpInv::XmlParse( )
{
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "fpzs");
	m_pXmlParse.m_parentElement[3] = m_pXmlParse.m_Child;
	m_ErrInvInfo.m_ErrUpNum = atoi(m_pXmlParse.GetText().c_str());//出错发票张数
	DBG_PRINT(("SKJ_ERRUPINV: m_ErrInvInfo.m_ErrUpNum = %u", m_ErrInvInfo.m_ErrUpNum));

	for (INT32 temp_i=0; temp_i<m_ErrInvInfo.m_ErrUpNum; temp_i++)
	{
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[3], "fpxx", temp_i);
		m_pXmlParse.m_parentElement[4] = m_pXmlParse.m_Child;
		
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fpdm");
		m_ErrInvInfo.InvUpFailInfo[temp_i].m_fpdm = m_pXmlParse.GetText();	//发票代码
		DBG_PRINT(("SKJ_ERRUPINV: InvUpFailInfo[%d].m_fpdm = %s", temp_i,m_ErrInvInfo.InvUpFailInfo[temp_i].m_fpdm.c_str()));
		
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fphm");
		m_ErrInvInfo.InvUpFailInfo[temp_i].m_fphm = atoi(m_pXmlParse.GetText().c_str());	//发票号码
		DBG_PRINT(("SKJ_ERRUPINV: InvUpFailInfo[%d].m_fphm = %u", temp_i, m_ErrInvInfo.InvUpFailInfo[temp_i].m_fphm));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "cwms");
		m_ErrInvInfo.InvUpFailInfo[temp_i].m_errMsg = m_pXmlParse.GetText();	//发票代码
		DBG_PRINT(("SKJ_ERRUPINV: InvUpFailInfo[%d].m_errMsg = %s", temp_i,m_ErrInvInfo.InvUpFailInfo[temp_i].m_errMsg.c_str()));
	}

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();		//返回代码
	DBG_PRINT(("SKJ_ERRUPINV: returncode = %s", m_retInfo.m_retCode.c_str()));	

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();		//返回信息
	DBG_PRINT(("SKJ_ERRUPINV: returnmsg = %s", m_retInfo.m_retMsg.c_str()));	
	
	return XML_SUCCESS;
}

