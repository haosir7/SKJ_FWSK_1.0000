 /*! \file    YWXML_BSPFPSH.cpp
   \brief    款机调用的中间件接口 业务2.16发票收回
   \author   lzh
   \version  1.0
   \date     2015
 */

#include "YWXML_BSPFPSH.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "base64.h"


//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CBspfpsh::CBspfpsh(CYWXML_GY &ywxml_gy, CInvVol &invvol, UINT8 strJzlx):CYWXmlBase(ywxml_gy), m_invvol(invvol), m_strJzlx(strJzlx) 
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CBspfpsh::~CBspfpsh()
{

}

INT32 CBspfpsh::XmlBuild( )
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
	DBG_PRINT(("SKJ_BSPFPSH: jpbh : %s", m_ywxml_gy.m_jqbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "nsrsbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_nsrsbh);	//纳税人识别号
	DBG_PRINT(("SKJ_BSPFPSH: nsrsbh : %s", m_ywxml_gy.m_nsrsbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "jzlx");
	memset(buf,0,sizeof(buf));
	sprintf(buf, "%u", m_strJzlx);
	m_pXmlConstruct.AddText(buf);


	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "bspbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_bspbh);	//报税盘编号
	DBG_PRINT(("SKJ_BSPFPSH: bspbh : %s", m_ywxml_gy.m_bspbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "bspkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_bspkl);	//报税盘口令
	DBG_PRINT(("SKJ_BSPFPSH: bspkl : %s", m_ywxml_gy.m_bspkl.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbbh");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbbh);	//税控设备编号
	DBG_PRINT(("SKJ_BSPFPSH: sksbbh : %s", m_ywxml_gy.m_sksbbh.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbkl);	//税控设备口令
	DBG_PRINT(("SKJ_BSPFPSH: sksbkl : %s", m_ywxml_gy.m_sksbkl.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fplxdm");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_fplxdm);	//发票类型代码
	DBG_PRINT(("SKJ_BSPFPSH: fplxdm : %s", m_ywxml_gy.m_fplxdm.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fpdm");
	m_pXmlConstruct.AddText(m_invvol.m_code);	//发票代码
	DBG_PRINT(("SKJ_BSPFPSH: fpdm : %s", m_invvol.m_code.c_str()));

	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "qshm");
	memset(buf,0,sizeof(buf));
	sprintf(buf, "%u", m_invvol.m_isno);
	m_pXmlConstruct.AddText(buf);			//发票起始号码
	DBG_PRINT(("SKJ_BSPFPSH: qshm : %s",buf));
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fpfs");
	memset(buf,0,sizeof(buf));
	sprintf(buf, "%u", m_invvol.m_fpzfs);
	m_pXmlConstruct.AddText(buf);			//发票份数
	DBG_PRINT(("SKJ_BSPFPSH: fpfs : %s",buf));
	return XML_SUCCESS;
}

INT32 CBspfpsh::XmlParse( )
{	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_RootElement, "body");
	m_pXmlParse.m_parentElement[1] = m_pXmlParse.m_Child;
	
	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[1], "output");
	m_pXmlParse.m_parentElement[2] = m_pXmlParse.m_Child;

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returncode");
	m_retInfo.m_retCode = m_pXmlParse.GetText();		//返回代码
	DBG_PRINT(("SKJ_BSPFPSH: returncode = %s", m_retInfo.m_retCode.c_str()));	

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "returnmsg");
	m_retInfo.m_retMsg = m_pXmlParse.GetText();		//返回信息
	DBG_PRINT(("SKJ_BSPFPSH: returnmsg = %s", m_retInfo.m_retMsg.c_str()));	

	
	return XML_SUCCESS;
}


