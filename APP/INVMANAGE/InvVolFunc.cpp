/*! \file    InvVolFunc.cpp
   \brief    票源管理应用函数
   \author   zcy
   \version  1.0
   \date     2015-05-07
 */

#include "InvVolFunc.h" 
#include "CaMsgBox.h"
#include "YWXmlBase.h"
#include "CInvVol.h"

#include "TDateTime.h"
#include "CGlobalArg.h"
#include "SysMacDef.h"
#include "InvBusinessFunc.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include "APIBase.h"


UINT8 INV_GetInvVol(UINT32 &uNum,string &strErr)
{
	INT32 Ret=SUCCESS;
	CInvVol invvol[INVVOL_MAX_NUM];
	uNum =INVVOL_MAX_NUM;

	for (int i=0; i<uNum ;i++)
	{
		invvol[i].ResetVol();
	}

	Ret =g_pAPIBase->BSPFPCXPro_API(*g_YwXmlArg, uNum, invvol, strErr);
   
	if ( Ret !=SUCCESS)
	{
		DBG_PRINT(("发票信息获取失败"));
		return FAILURE;
	}
	DBG_PRINT(("发票信息获取成功"));


	//保存获取发票信息
	Ret= invvol[0].Delete();//清空CInvVol表
    DBG_PRINT(("Ret= %d",Ret));
	DBG_PRINT(("uNum= %u",uNum));

	DBG_ASSERT_EXIT(invvol != NULL, ("invvol must not be NULL!"));

    for (int i=0; i<uNum; i++)
	{	
		DBG_PRINT(("发票信息存储条数 = %d",i))
		Ret= invvol[i].AddNew();
		DBG_PRINT(("Ret= %d",Ret))
		if (Ret != SQLITE_OK)
		{
			strErr = "发票信息存储失败";
			DBG_PRINT(("发票信息存储失败"));
			return FAILURE;
		}	
	}
	DBG_PRINT(("发票信息存储成功"));
	
	return SUCCESS;
}

UINT8 INV_NetInvVol(string strCode,UINT32 uStartNo,UINT32 uNum, string &strErr)
{
	//网络购票
	INT32 Ret=SUCCESS;

	CInvVol invvol;
	invvol.m_code = strCode;
	invvol.m_isno = uStartNo;
	invvol.m_fpzfs = uNum;
	invvol.m_ieno =uStartNo +uNum-1;


	Ret =g_pAPIBase->WLLQFPPro_API(*g_YwXmlArg, &invvol, strErr);
   
	if ( Ret !=SUCCESS)
	{
		return FAILURE;
	}

	return SUCCESS;
}

// UINT8 INV_NetInvVolVerify(string strCode,UINT32 uStartNo,UINT32 uEndNo, string &strErr)
// {
// 	//网络购票
// 	INT32 Ret=SUCCESS;
// 	
// 	CInvVol invvol;
// 	invvol.m_code = strCode;
// 	invvol.m_isno = uStartNo;
// 	invvol.m_ieno = uEndNo;
// 	
// 	Ret =g_pAPIBase->WLLQFPJGQRPro_API(*g_YwXmlArg, &invvol, strErr);
// 	
// 	if ( Ret !=SUCCESS)
// 	{
// 		return FAILURE;
// 	}
// 	
// 	return SUCCESS;
// }


UINT8 INV_ReadInvVol(UINT8 uJZlx,string &strErr)
{
	DBG_PRINT(("INV_ReadInvVol"));
	INT32 Ret=SUCCESS;
	
	Ret =g_pAPIBase->InvReadIn_API(*g_YwXmlArg, uJZlx, strErr);
    DBG_PRINT(("Ret= %d",Ret));
	if ( Ret !=SUCCESS)
	{
		return FAILURE;
	}
	return SUCCESS;
}


UINT8 INV_HostToInvVol(UINT8 uJZlx,string strCode,UINT32 uNo,UINT32 uNum,string &strErr)
{
	INT32 Ret=SUCCESS;

    CInvVol invvol;
	invvol.m_code = strCode;
	invvol.m_isno = uNo;
	invvol.m_fpzfs = uNum;

	Ret =g_pAPIBase->ZJInvDistribute_API(*g_YwXmlArg, &invvol,uJZlx, strErr);
	DBG_PRINT(("Ret= %d",Ret));
	if ( Ret !=SUCCESS)
	{
		return FAILURE;
	}

	return SUCCESS;
}


UINT8 INV_RetToInvVol(UINT8 uJZlx,string strCode,UINT32 uNo,UINT32 uNum,string &strErr)
{
	INT32 Ret=SUCCESS;

    CInvVol invvol;	
	invvol.m_code = strCode;
	invvol.m_isno = uNo;
	invvol.m_fpzfs = uNum;

	DBG_PRINT(("g_YwXmlArg->m_fplxdm : %s", g_YwXmlArg->m_fplxdm.c_str()));

	Ret =g_pAPIBase->ZJInvRecover_API(*g_YwXmlArg, &invvol,uJZlx, strErr);
   
	if ( Ret !=SUCCESS)
	{
		return FAILURE;
	}

	return SUCCESS;
}



UINT8 INV_HostGetInvVol(UINT8 uJZlx,string &strErr)
{
	INT32 Ret=SUCCESS;

	DBG_PRINT(("g_YwXmlArg->m_fplxdm : %s", g_YwXmlArg->m_fplxdm.c_str()));
   
	Ret =g_pAPIBase->FJInvReturn_API(*g_YwXmlArg,uJZlx, strErr);
   
	if ( Ret !=SUCCESS)
	{
		return FAILURE;
	}

	return SUCCESS;
}

UINT8 INV_MediumType(UINT8 &uJZlx)
{
	INT32 status;
	CaMsgBox msgBox("介质(1报税盘2金税盘):",CaMsgBox::MB_INPUT);
	msgBox.SetInputType(CaInput::aINT);
	msgBox.SetMaxInputLen(1);
	
	msgBox.ShowBox();
	status = msgBox.m_iStatus;
	if (status != OK_PRESSED)
	{
		//strErr= "请输入介质类型";
		return FAILURE;
	}
	uJZlx = atoi((INT8 *)msgBox.GetInputContent());
	DBG_PRINT(("uJZlx = %d", uJZlx));
	if( (uJZlx > 2)||(uJZlx == 0) )
	{
		CaMsgBox::ShowMsg("请输入正确的介质类型");
		return FAILURE;
	}

	return SUCCESS;
}

UINT8 INV_InvoiceUpload(string &strErr)
{
	INT32 Ret=SUCCESS;
	
	DBG_PRINT(("g_YwXmlArg->m_fplxdm : %s", g_YwXmlArg->m_fplxdm.c_str()));	
	Ret =g_pAPIBase->InvoiceUpload_API(*g_YwXmlArg, strErr);
	
	if ( Ret !=SUCCESS)
	{
		return FAILURE;
	}
	
	return SUCCESS;
}
