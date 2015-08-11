
#include "InvBusinessFunc.h"
#include "commonFunc.h"

#include "YWXML_FPDR.h"
#include "YWXML_BSPFPFF.h"
#include "YWXML_BSPFPSH.h"
#include "YWXML_FJTH.h"
#include "YWXML_GPXXXP.h"
#include "YWXML_GPXXXPJS.h"
#include "YWXML_BSPFPCX.h"
#include "BusinessBase.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CInvBusinessFunc::CInvBusinessFunc()
{

}

CInvBusinessFunc::~CInvBusinessFunc()
{

}

INT32 CInvBusinessFunc::BSPFPCXPro(CYWXML_GY &ywxml_gy,  UINT32 &InvCount, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = SUCCESS;
	ret = g_pBusBase->BSPFPCX_Business(ywxml_gy, InvCount, pInvVol, strErr);
	
	return g_pBusBase->ErrParse(ret, strErr);;

}


INT32 CInvBusinessFunc::InvReadIn(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr)
{
	INT32 ret = SUCCESS;
	ret = g_pBusBase->FPDR_Business(ywxml_gy, strJzlx, strErr);
	
	return g_pBusBase->ErrParse(ret, strErr);;
}

INT32 CInvBusinessFunc::ZJInvDistribute(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr)
{
	INT32 ret = SUCCESS;
	ret = g_pBusBase->BSPFPFF_Business(ywxml_gy, strJzlx, pInvVol, strErr);
	
	return g_pBusBase->ErrParse(ret, strErr);;
}

INT32 CInvBusinessFunc::ZJInvRecover(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr)
{
	INT32 ret = SUCCESS;
	ret = g_pBusBase->BSPFPSH_Business(ywxml_gy, strJzlx, pInvVol, strErr);
	
	return g_pBusBase->ErrParse(ret, strErr);;
}

INT32 CInvBusinessFunc::FJInvReturn(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr)
{
	INT32 ret = SUCCESS;
	ret = g_pBusBase->FJTH_Business(ywxml_gy, strJzlx, strErr);
	
	return g_pBusBase->ErrParse(ret, strErr);;
}


INT32 CInvBusinessFunc::TJXXCXPro(CYWXML_GY &ywxml_gy, INT32 &monthcount, CTjxxhz *pTjxxhz, string &strErr)
{
	INT32 ret = SUCCESS;
	ret = g_pBusBase->TJXXCX_Business(ywxml_gy, monthcount, pTjxxhz, strErr);
	
	return g_pBusBase->ErrParse(ret, strErr);
}

INT32 CInvBusinessFunc::WLLQFPPro(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = SUCCESS;
	UINT8 xxlx = SKSBQTYXXCX_XXLX_WLLQFPJGQR;
	string sksbxx("");

	ret = g_pBusBase->SKSBQTYXXCX_Business(ywxml_gy, xxlx, sksbxx, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	if(ret != SUCCESS)
	{
		return FAILURE;
	}

	if(sksbxx.length() > 0)
	{
		DBG_PRINT(("here"));
		ret = g_pBusBase->WLLQFPJGQR_Business(ywxml_gy, pInvVol, sksbxx, strErr);
		ret = g_pBusBase->ErrParse(ret, strErr);
		if(ret != SUCCESS)
		{
			return FAILURE;
		}
		
		ret = g_pBusBase->GPXXXPJS_Business(ywxml_gy, pInvVol->m_fpjjsmw, strErr);
		ret = g_pBusBase->ErrParse(ret, strErr);
		if(ret != SUCCESS)
		{
			return FAILURE;
		}
	}
	else
	{
		DBG_PRINT(("here"));
		xxlx = SKSBQTYXXCX_XXLX_WLLQFP;
		ret = g_pBusBase->SKSBQTYXXCX_Business(ywxml_gy, xxlx, sksbxx, strErr);
		ret = g_pBusBase->ErrParse(ret, strErr);
		if(ret != SUCCESS)
		{
			return FAILURE;
		}
		
	ret = g_pBusBase->WLLQFP_Business(ywxml_gy, pInvVol, sksbxx, strErr);
		ret = g_pBusBase->ErrParse(ret, strErr);
		if(ret != SUCCESS)
		{
			return FAILURE;
		}
		
		ret = g_pBusBase->GPXXXP_Business(ywxml_gy, pInvVol->m_fpjmw, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	if(ret != SUCCESS)
	{
		return FAILURE;
	}

		CommonSleep(1000);
		
		ret = WLLQFPJGQRPro(ywxml_gy, pInvVol, strErr);
	}

	return ret;

}

INT32 CInvBusinessFunc::WLLQFPJGQRPro(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = SUCCESS;
	UINT8 xxlx = SKSBQTYXXCX_XXLX_WLLQFPJGQR;
	string sksbxx("");
	
	ret = g_pBusBase->SKSBQTYXXCX_Business(ywxml_gy, xxlx, sksbxx, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	if(ret != SUCCESS)
	{
		return FAILURE;
	}
	
	ret = g_pBusBase->WLLQFPJGQR_Business(ywxml_gy, pInvVol, sksbxx, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	if(ret != SUCCESS)
	{
		return FAILURE;
	}
	
	ret = g_pBusBase->GPXXXPJS_Business(ywxml_gy, pInvVol->m_fpjjsmw, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);

	return ret;
}


