
#include "APIzhqProc.h"

#include <sys/timeb.h>
#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

#if TYPE_MODE == ZHQ_MODE

CAPIzhqProc::CAPIzhqProc()
{
	
}

CAPIzhqProc::~CAPIzhqProc()
{
	
}


INT32 CAPIzhqProc::OnLine(string &strErr)
{
	UINT8 ret = SUCCESS;
	
	UINT64 fTime, fTime2;
	struct timeb tp;
	ftime(&tp);
	
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT(("1 联机测试起始时间, fTime = %llu ", fTime));

	ret = m_SerialProc.onLine_Serial(strErr);
	
	ftime(&tp);
	fTime2 = tp.time * 1000 + tp.millitm;
	DBG_PRINT(("2 联机测试所用时间  fTime2 - fTime = %llu ", (fTime2 - fTime)));
	

	return ret;
}

INT32 CAPIzhqProc::SetBaudRate(UINT32 btl, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.setBaudRate_Serial(btl, strErr);
	
	return ret;
}

/*!
获取税控设备编号		
*/
INT32  CAPIzhqProc::GetSksbbh_API(CYWXML_GY &ywxml_gy, string &sksbbh, string &bspbh, string &qtxx, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.skpbhcx_Serial(ywxml_gy, sksbbh, strErr);
	if (ret != SUCCESS)
	{
		return ret;
	}
	
	ret = m_SerialProc.bspbhcx_Serial(ywxml_gy, bspbh, strErr);
	return ret;
}

/*!
获取税控设备信息		
*/
INT32  CAPIzhqProc::GetTaxpayerInfo_API(CYWXML_GY &ywxml_gy, CUserInfo &UserInfo, string &strCurTime, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.skpxxcx_Serial(ywxml_gy, UserInfo, strCurTime, strErr);
	
	return ret;
}

/*!
获取报税盘信息		
*/
INT32  CAPIzhqProc::GetBSPInfo_API(CYWXML_GY &ywxml_gy, CUserInfo &userInfo, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.bspxxcx_Serial(ywxml_gy, userInfo,  strErr);
	
	return ret;
}

/*!
获取监控数据		
*/
INT32  CAPIzhqProc::GetJKSJ_API(CYWXML_GY &ywxml_gy, CInvKind &InvKind, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.jksjcx_Serial(ywxml_gy, InvKind,  strErr);
	
	return ret;
}

/*!
获取税率信息		
*/
INT32  CAPIzhqProc::GetTaxRateInfo_API(CYWXML_GY &ywxml_gy, CTax *pTax, UINT8 &taxNum, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.sqslcx_Serial(ywxml_gy, pTax, taxNum, strErr);
	
	return ret;
}

/*!
金税盘口令变更
*/
INT32  CAPIzhqProc::SKPKLBG_API(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.sksbklgg_Serial(ywxml_gy, ykl, xkl, strErr);
	
	return ret;
}
/*!
@brief 变更证书口令
*/
INT32 CAPIzhqProc::BGZSKL_API(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr)
{
	INT32 ret = SUCCESS;
	
	
	ret= m_SerialProc.zsklgg_Serial(ywxml_gy, ykl, xkl, strErr);
	
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}
	
	return ret;
	
}
/*!
更新离线信息		
*/
INT32  CAPIzhqProc::LXXXUpdatePro_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.lxqxgx_Serial(ywxml_gy, strErr);
	
	return ret;
}


/*!
网络参数维护
*/
INT32  CAPIzhqProc::NetParaManage_API(CNetPara *pNetPara, string &strErr)
{
	UINT8 ret = SUCCESS;

	ret = m_SerialProc.wlcswh_Serial(pNetPara, strErr);

	return ret;
}


/*!
金税盘发票查询（发票段信息查询）
*/
INT32  CAPIzhqProc::BSPFPCXPro_API(CYWXML_GY &ywxml_gy, UINT32 &InvCount, CInvVol *pInvVol, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	DBG_PRINT(("获取待传入的InvCount = %u", InvCount));
	ret = m_SerialProc.bspfpcx_Serial(ywxml_gy, InvCount, pInvVol, strErr);
	
	return ret;
}

/*!
发票分发（主机）
*/
INT32  CAPIzhqProc::ZJInvDistribute_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	DBG_PRINT(("ywxml_gy.m_fplxdm : %s", ywxml_gy.m_fplxdm.c_str()));
	
	ret = m_SerialProc.fpff_Serial(ywxml_gy, strJzlx, pInvVol, strErr);
	
	return ret;
}

/*!
发票回收（主机）
*/

INT32  CAPIzhqProc::ZJInvRecover_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.fphs_Serial(ywxml_gy, pInvVol, strJzlx, strErr);
	
	return ret;
}

/*!
发票退回（分机）
*/
INT32  CAPIzhqProc::FJInvReturn_API(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.fjth_Serial(ywxml_gy, strJzlx, strErr);
	
	return ret;
}

/*!
统计信息查询
*/
INT32 CAPIzhqProc::TJXXCXPro_API(CYWXML_GY &ywxml_gy, CTjxxhz *pTjxxhz, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.fptjcx_Serial(ywxml_gy, pTjxxhz, strErr);
	
	return ret;
}

/*!
介质购票(发票读入)
*/
INT32  CAPIzhqProc::InvReadIn_API(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.fpdr_Serial(ywxml_gy, strJzlx, strErr);
	
	return ret;
}

/*!
网络领取发票（主机）
*/
INT32  CAPIzhqProc::WLLQFPPro_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.wlgp_Serial(ywxml_gy, pInvVol, strErr);
	
	return ret;
}

/*!
网络领取发票结果确认（主机）
*/
INT32  CAPIzhqProc::WLLQFPJGQRPro_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	UINT8 ret = SUCCESS;
	//网络领取发票WLLQFPPro_API 内部已经实现，转换器没有提供此接口，款机UI待确认是否使用此接口
	
	//	ret = m_SerialProc.wlgp_Serial(ywxml_gy, pInvVol, strErr);
	
	return ret;
}

/*!
购票信息查询（获取当前票号）
*/
INT32  CAPIzhqProc::GetCurInvInfo_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.gpxxcx_Serial(ywxml_gy, pInvVol, strErr);
	
	return ret;
}

/*!
发票开具（包含正票、红票）		
*/
INT32  CAPIzhqProc::MakeNormalInv_API(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, string &strErr)
{
	UINT8 ret = SUCCESS;
	

	UINT64 fTime, fTime2;
	struct timeb tp;
	ftime(&tp);
	
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT(("1 开票起始时间, fTime = %llu ", fTime));
	
	ret = m_SerialProc.fpkj_Serial(ywxml_gy, pInvHead, strErr);
	
	ftime(&tp);
	fTime2 = tp.time * 1000 + tp.millitm;
	DBG_PRINT(("2 开票所用时间  fTime2 - fTime = %llu ", (fTime2 - fTime)));

	return ret;
}

/*!
发票作废（空白废票、已开票作废）
*/
INT32  CAPIzhqProc::MakeWasteInv_API(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, UINT8 zflx, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.fpzf_Serial(ywxml_gy, pInvHead, zflx, strErr);
	
	return ret;
}

/*!
发票查询
*/
INT32  CAPIzhqProc::GetInvHeadInfo_API(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, UINT32  &invNum, CInvHead *pInvhead, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.fpcx_Serial(ywxml_gy, cxfs, cxtj, invNum, pInvhead, strErr);
	
	return ret;
}

/*!
发票实时上传
*/
INT32  CAPIzhqProc::InvoiceUpload_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	UINT8 ret = SUCCESS;
	UINT8 czlx = 0; //0:未上传发票上传
	UINT32 fpzs = 1; //每次上传一张发票
	ret = m_SerialProc.fpsssc_Serial(ywxml_gy, czlx, fpzs, strErr);
	
	return ret;
}

/*!
网络抄报
*/
INT32  CAPIzhqProc::NetDeclareProc_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	UINT8 ret = SUCCESS;
	UINT32 jzlx = 1; //1：网络（税控盘与金税盘）
	//	2：税控设备（金税盘专用）
	
	
	ret = m_SerialProc.sjcb_Serial(ywxml_gy, jzlx, strErr);
	return ret;
}

/*!
介质抄报（抄报时提示选择金税盘或者报税盘）
*/
INT32  CAPIzhqProc::DeclareProc_API(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	DBG_PRINT(("ywxml_gy.m_nsrsbh : %s", ywxml_gy.m_nsrsbh.c_str()));
	DBG_PRINT(("ywxml_gy.m_sksbbh : %s", ywxml_gy.m_sksbbh.c_str()));
	DBG_PRINT(("ywxml_gy.m_sksbkl : %s", ywxml_gy.m_sksbkl.c_str()));
	DBG_PRINT(("ywxml_gy.m_fplxdm : %s", ywxml_gy.m_fplxdm.c_str()));
	DBG_PRINT(("ywxml_gy.m_jqbh : %s", ywxml_gy.m_jqbh.c_str()));
	if (jzlx ==2)
	{
		ret = m_SerialProc.sjcb_Serial(ywxml_gy, jzlx, strErr);  //金税盘
	}
	else if(jzlx == 1)
	{
		UINT8 czlx=1;	//1：数据抄报
						//2：清零解锁
						//3：反写购票信息
						//4：校准税控设备时钟
		ret =m_SerialProc.skpbspzhcz_Serial(ywxml_gy, czlx, strErr); //报税盘
	}
	
	
	return ret;
}

/*!
网络清卡
*/
INT32  CAPIzhqProc::NetUpdateTaxProc_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.wljkhc_Serial(ywxml_gy, strErr);
	
	return ret;
}


/*!
报税盘清卡
*/
INT32  CAPIzhqProc::UpdateTaxProc_API(CYWXML_GY &ywxml_gy, string &strErr)
{
	UINT8 ret = SUCCESS;
	UINT8 czlx = 2;		//1：数据抄报
						//2：清零解锁
						//3：反写购票信息
						//4：校准税控设备时钟

	ret = m_SerialProc.skpbspzhcz_Serial(ywxml_gy, czlx, strErr);
	
	return ret;
}

/*!
发票补录
*/
INT32 CAPIzhqProc::Fpbl_API(CYWXML_GY &ywxml_gy, UINT32 bsqsrq, UINT32 bszzrq, string &strErr)
{
	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.fpbl_Serial(ywxml_gy, bsqsrq, bszzrq, strErr);
	
	return ret;
}


/*!
 获取离线数据
*/
INT32 CAPIzhqProc::Hqlxsj_API(string &wscfpzs, string &wscfpsj, string &wscfpljje, string &sczs, string &scsjjg, string &strErr)
{

	UINT8 ret = SUCCESS;
	
	ret = m_SerialProc.hqlxsj_Serial(wscfpzs, wscfpsj, wscfpljje, sczs, scsjjg, strErr);
	
	return ret;
}

#endif

