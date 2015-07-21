
#include "DeclareBusinessFunc.h"
#include "BusinessBase.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CDeclareBusinessFunc::CDeclareBusinessFunc()
{

}

CDeclareBusinessFunc::~CDeclareBusinessFunc()
{
	
}


INT32 CDeclareBusinessFunc::NetDeclareProc(CYWXML_GY &ywxml_gy, string &strErr)
{
	INT32 ret = SUCCESS;
	string strSQ("");
	string strFphz("");
	UINT8 xxlx = SKSBQTYXXCX_XXLX_WLCB;
	string strSksbxx("");
	UINT8 jzlx = 1;//1：网络（税控盘与金税盘）

	ret = g_pBusBase->SKSBQTYXXCX_Business(ywxml_gy, xxlx, strSksbxx, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	if(ret != SUCCESS)
	{
		return FAILURE;
	}
	
	ret = g_pBusBase->SJCB_Business(ywxml_gy, jzlx, strSQ, strFphz, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	if(ret != SUCCESS)
	{
		return FAILURE;
	}

	ret = g_pBusBase->WLCB_Business(ywxml_gy, strSQ, strFphz, strSksbxx, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);

	return ret;
}

INT32 CDeclareBusinessFunc::DeclareProc(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr)
{
	INT32 ret = SUCCESS;
	string strSQ("");
	string strFphz("");

	if(jzlx == SJCB_JZLX_JSP)		//金税盘报税
	{
	ret = g_pBusBase->SJCB_Business(ywxml_gy, jzlx, strSQ, strFphz, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	}
	else if(jzlx == SJCB_JZLX_BSP)	//报税盘报税
	{
		UINT8 czlx = SKPBSP_CZLX_SJCB;
		ret = g_pBusBase->SKPBSP_Business(ywxml_gy, czlx, strErr);
		ret = g_pBusBase->ErrParse(ret, strErr);
	}
	else
	{
		strErr = "介质类型错误!";
		return FAILURE;
	}

	return ret;
}

INT32 CDeclareBusinessFunc::NetUpdateTaxProc(CYWXML_GY &ywxml_gy, string &strErr)
{
	INT32 ret = SUCCESS;
	UINT8 xxlx = SKSBQTYXXCX_XXLX_QLJS;
	string strSksbxx("");
	string strFpjkmw("");
	
	ret = g_pBusBase->SKSBQTYXXCX_Business(ywxml_gy, xxlx, strSksbxx, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	if(ret != SUCCESS)
	{
		return FAILURE;
	}
	
	ret = g_pBusBase->QLJS_Business(ywxml_gy, strSksbxx, strFpjkmw, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	if(ret != SUCCESS)
	{
		return FAILURE;
	}

	ret = g_pBusBase->JKHC_Business(ywxml_gy, strFpjkmw, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);

	return ret;
}

INT32 CDeclareBusinessFunc::UpdateTaxProc(CYWXML_GY &ywxml_gy, string &strErr)
{
	INT32 ret = SUCCESS;
	UINT8 czlx = SKPBSP_CZLX_QLJS;

	ret = g_pBusBase->SKPBSP_Business(ywxml_gy, czlx, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);

	return ret;
}

