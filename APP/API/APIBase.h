/****************************************************************************
文件名           ：APIBase.h
功能             ：awe4000r业务流程抽象类
起始日期         ：
作者             ： 
****************************************************************************/

#ifndef API_BASE_H
#define API_BASE_H

#include "YWXMLGY.h"
#include "YWXmlBase.h"
#include "CUserInfo.h"
#include "CInvHead.h"
#include "CInvKind.h"
#include "CInvVol.h"
#include "CNetPara.h"
#include "CTax.h"
#include "CTjxxhz.h"
#include "CInvServ.h"

#include "CNetPara.h"


class CAPIBase;
extern CAPIBase *g_pAPIBase;


class CAPIBase
{
public:

	static void InitAPI();

	/*!
	@brief 获取税控设备编号		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 GetSksbbh_API(CYWXML_GY &ywxml_gy, string &sksbbh, string &bspbh, string &qtxx, string &strErr)=0;

	/*!
	@brief 获取税控设备信息		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 GetTaxpayerInfo_API(CYWXML_GY &ywxml_gy, CUserInfo &UserInfo, string &strCurTime, string &strErr)=0;
	
	/*!
	@brief 获取报税盘信息		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 GetBSPInfo_API(CYWXML_GY &ywxml_gy, CUserInfo &userInfo, string &strErr)=0;

	/*!
	@brief 获取监控数据		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 GetJKSJ_API(CYWXML_GY &ywxml_gy, CInvKind &InvKind, string &strErr)=0;

	/*!
	@brief 获取税率信息		
	@param[in] pTax 税率类数组
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 GetTaxRateInfo_API(CYWXML_GY &ywxml_gy, CTax *pTax, UINT8 &taxNum, string &strErr)=0;

	/*!
	@brief 金税盘口令变更
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 SKPKLBG_API(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr)=0;


	/*!
	@brief 变更证书口令
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 BGZSKL_API(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr)=0;

	/*!
	@brief 修改金税盘时钟
	@param[in]  szxx     时钟信息
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 XGJSPSZ_API(CYWXML_GY &ywxml_gy, string szxx, string &strErr)=0;


	/*!
	@brief 更新离线信息		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 LXXXUpdatePro_API(CYWXML_GY &ywxml_gy, string &strErr)=0;


	/*!
	@brief 联机	
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 OnLine(string &strErr) = 0;

	/*!
	@brief 设置波特率	
	@param[in] 波特率
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 SetBaudRate(UINT32 btl, string &strErr) = 0;

	/*!
	@brief 网络参数维护
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 NetParaManage_API(CNetPara *pNetPara, string &strErr)=0;
	
	/*!
	@brief 金税盘发票查询（发票段信息查询）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 BSPFPCXPro_API(CYWXML_GY &ywxml_gy, UINT32 &InvCount, CInvVol *pInvVol, string &strErr)=0;


	/*!
	@brief 发票分发（主机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 ZJInvDistribute_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr)=0;

	/*!
	@brief 发票收回（主机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/

	virtual INT32 ZJInvRecover_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr)=0;

	/*!
	@brief 发票退回（分机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 FJInvReturn_API(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr)=0;

	/*!
	@brief 统计信息查询
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 TJXXCXPro_API(CYWXML_GY &ywxml_gy, CTjxxhz *pTjxxhz, string &strErr)=0;
	
	/*!
	@brief 介质购票(发票读入)
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 InvReadIn_API(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr)=0;

	/*!
	@brief 网络领取发票（主机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 WLLQFPPro_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)=0;

	/*!
	@brief 网络领取发票结果确认（主机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 WLLQFPJGQRPro_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)=0;

	/*!
	@brief 购票信息查询（获取当前票号）
	@param[in] 
	@param[out] pInvVol  发票信息类
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 GetCurInvInfo_API(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)=0;
	
	/*!
	@brief 发票开具（包含正票、红票）		
	@param[in] invhead			发票头信息
	@param[out] taxCode   	税控码（20字节）
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 MakeNormalInv_API(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, string &strErr)=0;
	
	/*!
	@brief 发票作废（空白废票、已开票作废）
	@param[in] invhead	发票头信息
	@param[in] zflx   作废类型	0：空白发票作废	1：已开发票作废
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 MakeWasteInv_API(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, UINT8 zflx, string &strErr)=0;
	
	/*!
	@brief 发票查询
	@param[in] cxfs	查询方式
	@param[in] cxtj	查询条件
	@param[out] invNum  发票张数
	@param[in/out] invhead	发票头信息
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 GetInvHeadInfo_API(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, UINT32 &invNum, CInvHead *pInvhead, string &strErr)=0;
	
	/*!
	@brief 发票实时上传
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 InvoiceUpload_API(CYWXML_GY &ywxml_gy, string &strErr)=0;

		/*!
	@brief 网络抄报
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 NetDeclareProc_API(CYWXML_GY &ywxml_gy, string &strErr)=0;

	/*!
	@brief 介质抄报（抄报时提示选择金税盘或者报税盘）
	@param[in] jzlx 介质类型 2：金税盘 3：报税盘 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 DeclareProc_API(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr)=0;

	/*!
	@brief 网络清卡
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 NetUpdateTaxProc_API(CYWXML_GY &ywxml_gy, string &strErr)=0;

	/*!
	@brief 报税盘清卡
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 UpdateTaxProc_API(CYWXML_GY &ywxml_gy, string &strErr)=0;

	/*!
	@brief  发票补录
	@param[in] ywxml_gy	公共参数
	@param[in] bsqsrq	报税起始日期
	@param[in] bszhrq	报税终止日期
	@param[out] strErr	错误信息
	*/
	virtual INT32 Fpbl_API(CYWXML_GY &ywxml_gy, UINT32 bsqsrq, UINT32 bszzrq, string &strErr)=0;

	/*!
	@brief  获取离线相关数据
	@param[out] wscfpzs		未上传发票张数
	@param[out] wscfpsj		未上传发票时间
	@param[out] wscfpljje	未上传发票累计金额
	@param[out] sczs		上传张数
	@param[out] scsjjg		上传时间间隔
	@param[out] strErr		错误信息
	*/
	virtual INT32 Hqlxsj_API(CYWXML_GY &ywxml_gy, string &wscfpzs, string &wscfpsj, string &wscfpljje, string &sczs, string &scsjjg, string &strErr)=0;
	
	/*!
	@brief 转换器或者中间件清库
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	virtual INT32 LibClearDepot(string &strErr)=0;

	/*!
	@brief  获取上传出错发票信息
	@param[in] ywxml_gy
	@param[out] ErrInvNum		出错的发票张数
	@param[out] pInvUpFailInfo	出错的发票信息
	@param[out] strErr			错误信息
	*/
	virtual INT32 InvUpFailInfo_API(CYWXML_GY &ywxml_gy, CDataInvServ *pDataInvServ, UINT32 &nCount, string &strErr)=0;

	/*!
	@brief 网络连接测试
	@param[in] ywxml_gy
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	//virtual INT32 ServerTest_API(CYWXML_GY &ywxml_gy, string &strErr)=0;
};


#endif


