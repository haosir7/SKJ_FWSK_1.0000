
#include "SaleBusinessFunc.h"
#include "BusinessBase.h"
#include "commonFunc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CSaleBusinessFunc::CSaleBusinessFunc()
{

}

CSaleBusinessFunc::~CSaleBusinessFunc()
{

}


INT32 CSaleBusinessFunc::GetCurInvInfo(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = SUCCESS;
	ret = g_pBusBase->GPXXCX_Business(ywxml_gy, pInvVol, strErr);

	return g_pBusBase->ErrParse(ret, strErr);
}

INT32 CSaleBusinessFunc::MakeNormalInv(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, string &strErr)
{
	INT32 ret = SUCCESS;
	ret = g_pBusBase->FPKJ_Business(ywxml_gy, pInvHead, strErr);

	DBG_PRINT(("fpdm = %s", pInvHead->m_fpdm.c_str()));
	DBG_PRINT(("fphm = %08u", pInvHead->m_fphm));
	DBG_PRINT(("fwm = %s", pInvHead->m_fwm.c_str()));
	DBG_PRINT(("kprq= %u",pInvHead->m_kprq));

	return g_pBusBase->ErrParse(ret, strErr);
}

INT32 CSaleBusinessFunc::MakeWasteInv(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, UINT8 zflx, string &strErr)
{
	INT32 ret = SUCCESS;

	pInvHead->m_xfsh = ywxml_gy.m_nsrsbh;
	pInvHead->m_xfmc = ywxml_gy.m_nsrmc;

	ret = g_pBusBase->FPZF_Business(ywxml_gy, pInvHead, zflx, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	
	DBG_PRINT(("fpdm = %s", pInvHead->m_fpdm.c_str()));
	DBG_PRINT(("fphm = %08u", pInvHead->m_fphm));
	DBG_PRINT(("m_kprq = %u", pInvHead->m_kprq));
	DBG_PRINT(("m_kpsj = %u", pInvHead->m_kpsj));
	DBG_PRINT(("fwm = %s", pInvHead->m_fwm.c_str()));
	
	return ret;
}

INT32 CSaleBusinessFunc::GetInvHeadInfo(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, UINT32 &invNum, CInvHead *pInvhead, string &strErr)
{
	INT32 ret = SUCCESS;
	ret = g_pBusBase->FPCX_Business(ywxml_gy, cxfs, cxtj, invNum, pInvhead, strErr);
	
	return g_pBusBase->ErrParse(ret, strErr);
}

INT32 CSaleBusinessFunc::InvoiceUpload(CYWXML_GY &ywxml_gy, string &strErr)
{
	INT32 ret = SUCCESS;
	UINT8 upFlag = 0;
	UINT8 czlx = 0;
	UINT32 fpzx = 1;	//一次只上传一张发票
	string strInvInfo("");
	string strMxjgmw("");
	UINT8 nCount = 0;
	UINT32 nTime = 1000;
	UINT8 xxlx = SKSBQTYXXCX_XXLX_FPSCJGHQ;
	string strSksbxx("");
	string slxlh("");

	//从金税盘获取上传发票信息
	ret = g_pBusBase->FPSC_Business(ywxml_gy, czlx, fpzx, strInvInfo, strErr);
	DBG_PRINT(("ret = %d", ret));
	ret = g_pBusBase->ErrParse(ret, strErr);
	if(ret != SUCCESS)
	{
		return ret;
	}

	while(1)
	{
		if(nCount >= MAX_ERR_NUM)
		{
			break;
		}

		if(upFlag == 0)
		{
			//调用网络上传接口，上传发票信息
			ret = g_pBusBase->NETFPSC_Business(ywxml_gy, strInvInfo, fpzx, slxlh, strErr);
			DBG_PRINT(("ret = %d", ret));
			ret = g_pBusBase->ErrParse(ret, strErr);
			if(ret != SUCCESS)
			{
				nCount++;
				CommonSleep(nTime);
				continue;
			}
			CommonSleep(3*nTime);
			upFlag = 1;
			nCount = 0;
		}
		else
		{
			ret = g_pBusBase->SKSBQTYXXCX_Business(ywxml_gy, xxlx, strSksbxx, strErr);
			ret = g_pBusBase->ErrParse(ret, strErr);
			if(ret != SUCCESS)
			{
				return FAILURE;
			}
			//获取上传结果密文
			ret = g_pBusBase->FPSCJGHQ_Business(ywxml_gy, strSksbxx, strMxjgmw, strErr);
			ret = g_pBusBase->ErrParse(ret, strErr);
			if( ret != SUCCESS )
			{
				nCount++;
				CommonSleep(3*nTime);
				continue;
			}

			//将上传结果密文更新到金税盘
			ret = g_pBusBase->FPGX_Business(ywxml_gy, strMxjgmw, strErr);
			ret = g_pBusBase->ErrParse(ret, strErr);
			break;
		}

		CommonSleep(nTime);
	}

	return ret;
}

INT32 CSaleBusinessFunc::FpblProStart(CYWXML_GY &ywxml_gy, UINT32 SDate, UINT32 EDate, string &strErr)
{
	INT32 ret = SUCCESS;
	
	ret = g_pBusBase->FPBL_Business(ywxml_gy, SDate, EDate, strErr);
	ret = g_pBusBase->ErrParse(ret, strErr);
	
	return ret;
}

