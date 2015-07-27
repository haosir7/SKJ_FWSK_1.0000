
#include "APIskjProc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

#if TYPE_MODE == SKJ_MODE

#include "CGlobalArg.h"
#include "ManageBusinessFunc.h"
#include "InvBusinessFunc.h"
#include "SaleBusinessFunc.h"
#include "DeclareBusinessFunc.h"

#define  TJXXCX_MONTHMAX 6

CAPIskjProc::CAPIskjProc()
{
	
}

CAPIskjProc::~CAPIskjProc()
{
	
}

/*!
 获取税控设备编号		
*/
INT32  CAPIskjProc::GetSksbbh_API(CYWXML_GY &ywxml_gy, string &sksbbh, string &bspbh, string &qtxx, string &strErr)
{
	INT32 ret = SUCCESS;
	
	CManageBusinessFunc managBusFun;

	ret= managBusFun.GetSksbbh(ywxml_gy, sksbbh, bspbh, qtxx, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 获取税控设备信息		
*/
INT32  CAPIskjProc::GetTaxpayerInfo_API(CYWXML_GY &ywxml_gy, CUserInfo &UserInfo, string &strCurTime, string &strErr)
{
	DBG_PRINT(("进入CAPIskjProc::GetTaxpayerInfo_API函数"));

	INT32 ret = SUCCESS;

	CManageBusinessFunc managBusFun;
	
	ret= managBusFun.GetTaxpayerInfo(ywxml_gy, UserInfo, strCurTime, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	DBG_PRINT(("退出CAPIskjProc::GetTaxpayerInfo_API函数"));
	
	return ret;
}

/*!
 获取报税盘信息		
*/
INT32  CAPIskjProc::GetBSPInfo_API(CYWXML_GY &ywxml_gy, CUserInfo &userInfo, string &strErr)
{
	INT32 ret = SUCCESS;

	CManageBusinessFunc managBusFun;
	
	ret= managBusFun.GetBSPInfo(ywxml_gy, userInfo, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 获取监控数据		
*/
INT32  CAPIskjProc::GetJKSJ_API(CYWXML_GY &ywxml_gy, CInvKind &InvKind, string &strErr)
{
	INT32 ret = SUCCESS;

	CManageBusinessFunc managBusFun;
	
	ret= managBusFun.GetJKSJ(ywxml_gy, InvKind, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 获取税率信息		
*/
INT32  CAPIskjProc::GetTaxRateInfo_API(CYWXML_GY &ywxml_gy, CTax *pTax, UINT8 &taxNum, string &strErr)
{
	INT32 ret = SUCCESS;

	CManageBusinessFunc managBusFun;
	
	ret= managBusFun.GetTaxRateInfo(ywxml_gy, pTax, taxNum, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 金税盘口令变更
*/
INT32  CAPIskjProc::SKPKLBG_API(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr)
{
	INT32 ret = SUCCESS;

	CManageBusinessFunc managBusFun;
	
	ret= managBusFun.SKPKLBG(ywxml_gy, ykl, xkl, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
	@brief 变更证书口令
*/
INT32 CAPIskjProc::BGZSKL_API(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr)
{
	INT32 ret = SUCCESS;
	
	CManageBusinessFunc managBusFun;
	
	ret= managBusFun.BGZSKLProc(ywxml_gy, ykl, xkl, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}
	
	return ret;

}

/*!
 更新离线信息		
*/
INT32  CAPIskjProc::LXXXUpdatePro_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	INT32 ret = SUCCESS;

	CManageBusinessFunc managBusFun;
	
	ret= managBusFun.LXXXUpdatePro(ywxml_gy, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}


INT32 CAPIskjProc::OnLine(string &strErr)
{
	UINT8 ret = SUCCESS;
	return ret;
}

INT32 CAPIskjProc::SetBaudRate(UINT32 btl, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	return ret;
}

/*!
 网络参数维护
*/
INT32  CAPIskjProc::NetParaManage_API(CNetPara *pNetPara, string &strErr)
{
	INT32 ret = SUCCESS;


	return ret;
}


/*!
 金税盘发票查询（发票段信息查询）
*/
INT32  CAPIskjProc::BSPFPCXPro_API(CYWXML_GY &ywxml_gy, UINT32 &InvCount, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = SUCCESS;

	CInvBusinessFunc invBusFun;
	
	ret= invBusFun.BSPFPCXPro(ywxml_gy, InvCount, pInvVol, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 发票分发（主机）
*/
INT32  CAPIskjProc::ZJInvDistribute_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr)
{
	INT32 ret = SUCCESS;

	CInvBusinessFunc invBusFun;
	
	ret= invBusFun.ZJInvDistribute(ywxml_gy, pInvVol, strJzlx, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 发票退回（分机）
*/

INT32  CAPIskjProc::ZJInvRecover_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr)
{
	INT32 ret = SUCCESS;

	CInvBusinessFunc invBusFun;
	
	ret= invBusFun.ZJInvRecover(ywxml_gy, pInvVol, strJzlx, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 发票收回（主机）
*/
INT32  CAPIskjProc::FJInvReturn_API(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr)
{
	INT32 ret = SUCCESS;

	CInvBusinessFunc invBusFun;
	
	ret= invBusFun.FJInvReturn(ywxml_gy, strJzlx, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 统计信息查询
*/
INT32  CAPIskjProc::TJXXCXPro_API(CYWXML_GY &ywxml_gy, CTjxxhz *pTjxxhz, string &strErr)
{
	INT32 ret = SUCCESS;

	CInvBusinessFunc invBusFun;
	INT32 monthcount = TJXXCX_MONTHMAX;
	DBG_PRINT(("ywxml_gy.m_fplxdm : %s", ywxml_gy.m_fplxdm.c_str()));

	ret= invBusFun.TJXXCXPro(ywxml_gy, monthcount, pTjxxhz, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 介质购票(发票读入)
*/
INT32  CAPIskjProc::InvReadIn_API(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr)
{
	INT32 ret = SUCCESS;

	CInvBusinessFunc invBusFun;
	
	ret= invBusFun.InvReadIn(ywxml_gy, strJzlx, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 网络领取发票（主机）
*/
INT32  CAPIskjProc::WLLQFPPro_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = SUCCESS;

	CInvBusinessFunc invBusFun;
	
	ret= invBusFun.WLLQFPPro(ywxml_gy, pInvVol, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 网络领取发票结果确认（主机）
*/
INT32  CAPIskjProc::WLLQFPJGQRPro_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = SUCCESS;

	CInvBusinessFunc invBusFun;
	
	ret= invBusFun.WLLQFPJGQRPro(ywxml_gy, pInvVol, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 购票信息查询（获取当前票号）
*/
INT32  CAPIskjProc::GetCurInvInfo_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = SUCCESS;

    CSaleBusinessFunc  saleBusFun;

	ret= saleBusFun.GetCurInvInfo(ywxml_gy, pInvVol, strErr);

	if (ret !=SUCCESS)
	{
		return FAILURE;
	}
	
	return ret;
}

/*!
 发票开具（包含正票、红票）		
*/
INT32  CAPIskjProc::MakeNormalInv_API(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, string &strErr)
{
	INT32 ret = SUCCESS;
    CSaleBusinessFunc  saleBusFun;

	ret= saleBusFun.MakeNormalInv(ywxml_gy, pInvHead, strErr);

	if (ret !=SUCCESS)
	{
		return FAILURE;
	}
	
	return ret;
}

/*!
 发票作废（空白废票、已开票作废）
*/
INT32  CAPIskjProc::MakeWasteInv_API(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, UINT8 zflx, string &strErr)
{
	INT32 ret = SUCCESS;

	CSaleBusinessFunc  saleBusFun;
	ret= saleBusFun.MakeWasteInv(ywxml_gy, pInvHead, zflx, strErr);

	if (ret !=SUCCESS)
	{
		return FAILURE;
	}
	
	return ret;
}

/*!
 发票查询
*/
INT32  CAPIskjProc::GetInvHeadInfo_API(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, UINT32 &invNum, CInvHead *pInvhead, string &strErr)
{
	INT32 ret = SUCCESS;

	CSaleBusinessFunc  saleBusFun;
	ret= saleBusFun.GetInvHeadInfo(ywxml_gy, cxfs, cxtj, invNum, pInvhead, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}
	
	return ret;
}

/*!
 发票实时上传
*/
INT32  CAPIskjProc::InvoiceUpload_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	INT32 ret = SUCCESS;

#if TYPE_MODE ==  SKJ_MODE
//	CSaleBusinessFunc  saleBusFun;
//	do 
//	{
//	ret= saleBusFun.InvoiceUpload(ywxml_gy, strErr);
//	} while(ret == SUCCESS);

// 	ret= saleBusFun.InvoiceUpload(ywxml_gy, strErr);
// 	
// 	if (ret !=SUCCESS)
// 	{
// 		return FAILURE;
// 	}
	g_globalArg->m_pthreadNsrsbh = ywxml_gy.m_nsrsbh;
	g_globalArg->m_pthreadSksbbh = ywxml_gy.m_sksbbh;
	g_globalArg->m_pthreadSksbkl = ywxml_gy.m_sksbkl;
	g_globalArg->m_pthreadFplxdm = ywxml_gy.m_fplxdm;
	g_globalArg->m_pthreadJqbh = ywxml_gy.m_jqbh;
	g_globalArg->m_pthreadKpjh = ywxml_gy.m_kpjh;
	g_globalArg->m_pthreadZskl = ywxml_gy.m_zskl;

	if(	g_globalArg->m_pthreadFlag == 0)
	{
		g_globalArg->m_pthreadFlag = 1;
	}
	else
	{
		g_globalArg->m_pthreadFlag = 0;
	}
	DBG_PRINT(("g_globalArg->m_pthreadFlag = %u", g_globalArg->m_pthreadFlag));
#endif
	return ret;
}

/*!
 网络抄报
*/
INT32  CAPIskjProc::NetDeclareProc_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	INT32 ret = SUCCESS;

	CDeclareBusinessFunc  declareBusFun;
	ret= declareBusFun.NetDeclareProc(ywxml_gy, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 介质抄报（抄报时提示选择金税盘或者报税盘）
*/
INT32  CAPIskjProc::DeclareProc_API(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr)
{
	INT32 ret = SUCCESS;

	CDeclareBusinessFunc  declareBusFun;
	ret= declareBusFun.DeclareProc(ywxml_gy, jzlx, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

/*!
 网络清卡
*/
INT32  CAPIskjProc::NetUpdateTaxProc_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	INT32 ret = SUCCESS;

	CDeclareBusinessFunc  declareBusFun;
	ret= declareBusFun.NetUpdateTaxProc(ywxml_gy, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}


/*!
 报税盘清卡
*/
INT32  CAPIskjProc::UpdateTaxProc_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	INT32 ret = SUCCESS;

	CDeclareBusinessFunc  declareBusFun;
	ret= declareBusFun.UpdateTaxProc(ywxml_gy, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}

	return ret;
}

INT32 CAPIskjProc::Fpbl_API(CYWXML_GY &ywxml_gy, UINT32 bsqsrq, UINT32 bszzrq, string &strErr)
{

	INT32 ret = SUCCESS;
	
	CSaleBusinessFunc  saleBusFun;
	ret = saleBusFun.FpblProStart(ywxml_gy, bsqsrq, bszzrq, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}
	
	return ret;
}

INT32 CAPIskjProc::Hqlxsj_API(string &wscfpzs, string &wscfpsj, string &wscfpljje, string &sczs, string &scsjjg, string &strErr)
{
	return SUCCESS;
}

INT32 CAPIskjProc::LibClearDepot(string &strErr)
{
	return SUCCESS;
}

#endif

