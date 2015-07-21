/****************************************************************************
文件名           ：APIskjBase.h
功能             ：awe4000r业务流程抽血类
起始日期         ：
作者             ： 
****************************************************************************/

#ifndef API_SKJ_PROC_H
#define API_SKJ_PROC_H

#include "APIBase.h"

#if TYPE_MODE == SKJ_MODE

class CAPIskjProc : public CAPIBase
{
public:

	CAPIskjProc();
	~CAPIskjProc();

	/*!
	@brief 获取税控设备编号		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetSksbbh_API(CYWXML_GY &ywxml_gy, string &sksbbh, string &bspbh, string &qtxx, string &strErr);

	/*!
	@brief 获取税控设备信息		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetTaxpayerInfo_API(CYWXML_GY &ywxml_gy, CUserInfo &UserInfo, string &strCurTime, string &strErr);
	
	/*!
	@brief 获取报税盘信息		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetBSPInfo_API(CYWXML_GY &ywxml_gy, CUserInfo &userInfo, string &strErr);

	/*!
	@brief 获取监控数据		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetJKSJ_API(CYWXML_GY &ywxml_gy, CInvKind &InvKind, string &strErr);

	/*!
	@brief 获取税率信息		
	@param[in] pTax 税率类数组
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetTaxRateInfo_API(CYWXML_GY &ywxml_gy, CTax *pTax, UINT8 &taxNum, string &strErr);

	/*!
	@brief 金税盘口令变更
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 SKPKLBG_API(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr);

	/*!
	@brief 变更证书口令
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 BGZSKL_API(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr);

	/*!
	@brief 更新离线信息		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 LXXXUpdatePro_API(CYWXML_GY &ywxml_gy, string &strErr);

	/*!
	@brief 联机	
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 OnLine(string &strErr);

	/*!
	@brief 设置波特率	
	@param[in] 波特率
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 SetBaudRate(UINT32 btl, string &strErr);

	/*!
	@brief 网络参数维护
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 NetParaManage_API(CNetPara *pNetPara, string &strErr);

	/*!
	@brief 金税盘发票查询（发票段信息查询）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 BSPFPCXPro_API(CYWXML_GY &ywxml_gy, UINT32 &InvCount, CInvVol *pInvVol, string &strErr);


	/*!
	@brief 发票分发（主机将发票分发给分机，需保证主分税控设备同时连接款机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 ZJInvDistribute_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr);

	/*!
	@brief 发票退回（分机将发票由票源退回给报税区））
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 ZJInvRecover_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr);

	/*!
	@brief 发票收回（主机将分机报税区的发票读至自己的票源区，需保证主分税控设备同时连接款机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 FJInvReturn_API(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr);

	/*!
	@brief 统计信息查询
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 TJXXCXPro_API(CYWXML_GY &ywxml_gy, CTjxxhz *pTjxxhz, string &strErr);

	/*!
	@brief 介质购票(发票读入)
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 InvReadIn_API(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr);

	/*!
	@brief 网络领取发票（主机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 WLLQFPPro_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr);

	/*!
	@brief 网络领取发票结果确认（主机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 WLLQFPJGQRPro_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr);

	/*!
	@brief 购票信息查询（获取当前票号）
	@param[in] 
	@param[out] pInvVol  发票信息类
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetCurInvInfo_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr);
	
	/*!
	@brief 发票开具（包含正票、红票）		
	@param[in] invhead			发票头信息
	@param[out] taxCode   	税控码（20字节）
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 MakeNormalInv_API(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, string &strErr);
	
	/*!
	@brief 发票作废（空白废票、已开票作废）
	@param[in] invhead	发票头信息
	@param[in] zflx   作废类型	0：空白发票作废	1：已开发票作废
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 MakeWasteInv_API(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, UINT8 zflx, string &strErr);
	
	/*!
	@brief 发票查询
	@param[in] cxfs	查询方式
	@param[in] cxtj	查询条件
	@param[out] invNum  发票张数
	@param[in/out] invhead	发票头信息
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetInvHeadInfo_API(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, UINT32 &invNum, CInvHead *pInvhead, string &strErr);
	
	/*!
	@brief 发票实时上传
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 InvoiceUpload_API(CYWXML_GY &ywxml_gy, string &strErr);

		/*!
	@brief 网络抄报
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 NetDeclareProc_API(CYWXML_GY &ywxml_gy, string &strErr);

	/*!
	@brief 介质抄报（抄报时提示选择金税盘或者报税盘）
	@param[in] jzlx 介质类型 1：报税盘 2：金税盘  
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 DeclareProc_API(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr);

	/*!
	@brief 网络清卡
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 NetUpdateTaxProc_API(CYWXML_GY &ywxml_gy, string &strErr);

	/*!
	@brief 报税盘清卡
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 UpdateTaxProc_API(CYWXML_GY &ywxml_gy, string &strErr);

	/*!
	@brief  发票补录
	@param[in] ywxml_gy	公共参数
	@param[in] bsqsrq	报税起始日期
	@param[in] bszhrq	报税终止日期
	@param[out] strErr	错误信息
	*/
	INT32 Fpbl_API(CYWXML_GY &ywxml_gy, UINT32 bsqsrq, UINT32 bszzrq, string &strErr);

	/*!
	@brief  获取离线相关数据
	@param[out] wscfpzs		未上传发票张数
	@param[out] wscfpsj		未上传发票时间
	@param[out] wscfpljje	未上传发票累计金额
	@param[out] sczs		上传张数
	@param[out] scsjjg		上传时间间隔
	@param[out] strErr		错误信息
	*/
	INT32 Hqlxsj_API(string &wscfpzs, string &wscfpsj, string &wscfpljje, string &sczs, string &scsjjg, string &strErr);
};

#endif
#endif


