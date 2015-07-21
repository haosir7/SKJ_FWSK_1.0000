/****************************************************************************
文件名           ：InvBusinessFunc.h
功能             ：awe4000r发票业务功能流程函数。（主、分机发票管理）
起始日期         ：
作者             ： 
****************************************************************************/

#ifndef INV_BUSINESS_FUNC_H
#define INV_BUSINESS_FUNC_H

#include "YWXMLGY.h"
#include "CInvVol.h"
#include "CTjxxhz.h"

class CInvBusinessFunc
{
public:
	CInvBusinessFunc();
	~CInvBusinessFunc();


	/*!
	@brief 金税盘发票查询（发票段信息查询）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 BSPFPCXPro(CYWXML_GY &ywxml_gy, UINT32 &InvCount, CInvVol *pInvVol, string &strErr);


	/*!
	@brief 发票分发（主机将发票分发给分机，需保证主分税控设备同时连接款机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 ZJInvDistribute(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr);

	/*!
	@brief 发票退回（分机将发票由票源退回给报税区）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 ZJInvRecover(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr);

	/*!
	@brief 发票收回（主机将分机报税区的发票读至自己的票源区，需保证主分税控设备同时连接款机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 FJInvReturn(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr);

	/*!
	@brief 统计信息查询
	@param[in/out] pTjxxhz 统计信息汇总类
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 TJXXCXPro(CYWXML_GY &ywxml_gy, INT32 &monthcount, CTjxxhz *pTjxxhz, string &strErr);


	/*!
	@brief 介质购票(发票读入)
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 InvReadIn(CYWXML_GY &ywxml_gy, UINT8 strJzlx, string &strErr);

	/*!
	@brief 网络领取发票（主机）（网络购票后，调用介质购票读入）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 WLLQFPPro(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr);

	/*!
	@brief 网络领取发票结果确认（主机）
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 WLLQFPJGQRPro(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr);


};


#endif


