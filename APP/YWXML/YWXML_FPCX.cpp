/*! \file     YWXML_FPCX.cpp
   \brief    款机调用的中间件接口 业务: 发票查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#include "YWXML_FPCX.h"
#include "base64.h"
#include "SysMacDef.h"
#include "arithmetic.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"




//-------------------------------------------------------------------------------------------
//构造函数
//-------------------------------------------------------------------------------------------
CFpcx::CFpcx(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, CInvHead &InvInfo, UINT32 &InvCOunt):m_cxfs(cxfs), m_cxtj(cxtj), m_InvInfo(InvInfo), m_InvCOunt(InvCOunt), CYWXmlBase(ywxml_gy)
{
	
}

//-------------------------------------------------------------------------------------------
//析构函数
//-------------------------------------------------------------------------------------------
CFpcx::~CFpcx()
{

}


INT32 CFpcx::XmlBuild()
{

	INT8 Buf[64];

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
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "sksbkl");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_sksbkl);	
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "fplxdm");
	m_pXmlConstruct.AddText(m_ywxml_gy.m_fplxdm);	
	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "cxfs");
	memset(Buf, 0, sizeof(Buf));
	sprintf(Buf, "%u", m_cxfs);
	m_pXmlConstruct.AddText(Buf);
	DBG_PRINT(("Fpcx::cxfs : %s", Buf));

	
	m_pXmlConstruct.AddNode(m_pXmlConstruct.m_parentElement[2], "cxtj");//查询方式
	m_pXmlConstruct.AddText(m_cxtj);	
	DBG_PRINT(("Fpcx::cxtj : %s", m_cxtj.c_str()));

	return XML_SUCCESS;
}

INT32 CFpcx::XmlParse()
{
	UINT8 i = 0;
	UINT8 j = 0;
	UINT8 fpzt = 0;

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

	m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "fpxx");
	m_pXmlParse.m_parentElement[3] = m_pXmlParse.m_Child;
	m_InvCOunt = atoi(m_pXmlParse.GetAttr("count").c_str());

	for (i=0; i<m_InvCOunt; i++)
	{
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[3], "group", i);
		m_pXmlParse.m_parentElement[4] = m_pXmlParse.m_Child;

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fpdm");
		m_InvInfo.m_fpdm = m_pXmlParse.GetText();
		DBG_PRINT(("m_InvInfo.m_fpdm : %s", m_InvInfo.m_fpdm.c_str()));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fphm");
		m_InvInfo.m_fphm = atoi(m_pXmlParse.GetText().c_str());
		DBG_PRINT(("m_InvInfo.m_fphm : %u", m_InvInfo.m_fphm));

		//发票状态
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "fpzt");
		fpzt = atoi(m_pXmlParse.GetText().c_str());
		switch(fpzt)
		{
		case 0:
			m_InvInfo.m_kplx = NORMAL_INV;
			break;
		case 1:
			m_InvInfo.m_kplx = RETURN_INV;
			break;
		case 2:
			m_InvInfo.m_kplx = WASTE_INV;
			break;
		case 3:
			m_InvInfo.m_kplx = WASTE_NOR;
			break;
		case 4:
			m_InvInfo.m_kplx = WASTE_RET;
			break;
		}
		
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "scbz");
		m_InvInfo.m_scbz = (UINT8)atoi(m_pXmlParse.GetText().c_str());
		DBG_PRINT(("m_InvInfo.m_scbz : %u", m_InvInfo.m_scbz));
		
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "kprq");	
		m_InvInfo.m_kprq = atoi(m_pXmlParse.GetText().c_str());
		DBG_PRINT(("m_InvInfo.m_kprq : %u", m_InvInfo.m_kprq));		
	
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "kpsj");	
		m_InvInfo.m_kpsj = atoi(m_pXmlParse.GetText().c_str());
		DBG_PRINT(("m_InvInfo.m_kpsj : %u", m_InvInfo.m_kpsj));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "skm");	
		m_InvInfo.m_fwm = m_pXmlParse.GetText();
		DBG_PRINT(("m_InvInfo.m_fwm : %s", m_InvInfo.m_fwm.c_str()));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "sksbbh");	//税控设备编号
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "skdwmc");	//收款单位名称
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "xhdwdm");	//销货单位识别号
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "xhdwmc");	//销货单位名称

		//购货单位识别号
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "ghdwdm");	
		m_InvInfo.m_fkdwsh = m_pXmlParse.GetText();
		DBG_PRINT(("m_InvInfo.m_fkdwsh : %s",m_InvInfo.m_fkdwsh.c_str()));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "ghdwmc");	
		m_InvInfo.m_fkdw = m_pXmlParse.GetText();
		DBG_PRINT(("m_InvInfo.m_fkdw : %s",m_InvInfo.m_fkdw.c_str()));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "sfxm");
		m_pXmlParse.m_parentElement[5] = m_pXmlParse.m_Child;
		m_InvInfo.m_sphsl = atoi(m_pXmlParse.GetAttr("count").c_str());
		DBG_PRINT(("m_InvInfo.m_sphsl : %u", m_InvInfo.m_sphsl));
	
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "skr");
		m_InvInfo.m_sky = m_pXmlParse.GetText();//中间件工程中,skr、kpr、zfr全用m_sky组装
		DBG_PRINT(("m_InvInfo.m_sky : %s", m_InvInfo.m_sky.c_str()));
		
		CInvDet *pInvDet = NULL;

		for (j=0; j<m_InvInfo.m_sphsl; j++)
		{
			pInvDet = pInvDet->GetNewInvDet();	//从静态数组获取一个可用的CInvDet对象
			DBG_ASSERT_EXIT((pInvDet != NULL), (" pInvDet == NULL!"));

			pInvDet->m_fpdm = m_InvInfo.m_fpdm;
			pInvDet->m_fphm = m_InvInfo.m_fphm;
			pInvDet->m_kprq = m_InvInfo.m_kprq;
			pInvDet->m_kpsj = m_InvInfo.m_kpsj;
			pInvDet->m_kplx = m_InvInfo.m_kplx;
			pInvDet->m_sky = m_InvInfo.m_sky;
			pInvDet->m_sphxh = j+1;

			m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[5], "group", j);
			m_pXmlParse.m_parentElement[6] = m_pXmlParse.m_Child;

			m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[6], "xm");;
			pInvDet->m_spmc = m_pXmlParse.GetText();
			DBG_PRINT(("pInvDet->m_spmc : %s", pInvDet->m_spmc.c_str()));
			
			m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[6], "dj");
			pInvDet->m_dj = atof(m_pXmlParse.GetText().c_str());//单价不含税
			DBG_PRINT(("pInvDet->m_dj : %.3lf", pInvDet->m_dj));

			m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[6], "sl");
			pInvDet->m_spsl = atof(m_pXmlParse.GetText().c_str());//数量
			DBG_PRINT(("pInvDet->m_spdj : %.3lf", pInvDet->m_spsl));
				
			m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[6], "je");
			pInvDet->m_je = double2int(atof(m_pXmlParse.GetText().c_str())*SUM_EXTENSION);//金额不含税
			DBG_PRINT(("pInvDet->m_spje : %lld", pInvDet->m_je));

			m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[6], "zsl");
			pInvDet->m_sl = atof(m_pXmlParse.GetText().c_str());//税率
			DBG_PRINT(("pInvDet->m_sl : %.2lf", pInvDet->m_sl));
	
			m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[6], "se");
			pInvDet->m_spse = double2int(atof(m_pXmlParse.GetText().c_str())*SUM_EXTENSION);//税额
			DBG_PRINT(("pInvDet->m_spse : %lld", pInvDet->m_spse));

			m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[6], "hsdj");
			pInvDet->m_spdj = atof(m_pXmlParse.GetText().c_str());//含税单价
			DBG_PRINT(("pInvDet->m_spdj : %.3lf", pInvDet->m_spdj));

			m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[6], "hsje");
			pInvDet->m_spje = double2int(atof(m_pXmlParse.GetText().c_str())*SUM_EXTENSION);//含税金额
			DBG_PRINT(("pInvDet->m_spje : %lld", pInvDet->m_spje));

			m_InvInfo.InsertNode(pInvDet);

			pInvDet =NULL;
		}

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "hjje");
		m_InvInfo.m_kpje = double2int(atof(m_pXmlParse.GetText().c_str())*SUM_EXTENSION);
		DBG_PRINT(("m_InvInfo.m_kpje : %lld", m_InvInfo.m_kpje));

		
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "hjse");
		m_InvInfo.m_kpse = double2int(atof(m_pXmlParse.GetText().c_str())*SUM_EXTENSION);
		DBG_PRINT(("m_InvInfo.m_kpse : %lld", m_InvInfo.m_kpse));
		
		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "jshj");
		m_InvInfo.m_kphjje = double2int(atof(m_pXmlParse.GetText().c_str())*SUM_EXTENSION);
		DBG_PRINT(("m_InvInfo.m_kphjje : %lld", m_InvInfo.m_kphjje));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "bz");
		m_InvInfo.m_backup1 = m_pXmlParse.GetText();
		DBG_PRINT(("m_InvInfo.m_backup1 : %s", m_InvInfo.m_backup1.c_str()));

// 		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "kpr");
// 		m_InvInfo.m_sky = m_pXmlParse.GetText();
// 		DBG_PRINT(("m_InvInfo.m_sky : %s", m_InvInfo.m_sky.c_str()));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "yfpdm");
		m_InvInfo.m_yfpdm = m_pXmlParse.GetText();
		DBG_PRINT(("m_InvInfo.m_yfpdm : %s", m_InvInfo.m_yfpdm.c_str()));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "yfphm");
		m_InvInfo.m_yfphm = atoi(m_pXmlParse.GetText().c_str());


		//作废日期
// 		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "zfrq");
// 		memset(Buf, 0, sizeof(Buf));
// 		sprintf(Buf, "%u",m_InvInfo.zfrq);
// 		m_pXmlParse.GetText(Buf);
// 		DBG_PRINT(("buf : %s", Buf));
		
		//作废人
// 		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "zfr");
// 		m_pXmlParse.GetText(m_InvInfo.m_yfpdm);
// 		DBG_PRINT(("m_InvInfo.m_yfpdm : %s", m_InvInfo.m_yfpdm.c_str()));

		//已开负数金额
// 		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[4], "ykfsje");	
// 		memset(Buf, 0, sizeof(Buf));
// 		sprintf(Buf, "%ld", m_InvInfo.m_kphjje);
// 		m_pXmlParse.GetText(Buf);
// 		DBG_PRINT(("buf : %s", Buf));

		m_pXmlParse.LocateNodeByName(m_pXmlParse.m_parentElement[2], "qmz");
		m_InvInfo.m_casign = m_pXmlParse.GetText();
		DBG_PRINT(("m_InvInfo.m_casign : %s", m_InvInfo.m_casign.c_str()));		
	}

	return XML_SUCCESS;

}





