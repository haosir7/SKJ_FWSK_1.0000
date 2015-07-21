/****************************************************************************
文件名           ：ManageBusinessFunc.h
功能             ：awe4000r管理业务功能流程函数。（税控设备信息读写）
起始日期         ：
作者             ： 
****************************************************************************/

#ifndef MANAGE_BUSINESS_FUNC_H
#define MANAGE_BUSINESS_FUNC_H

#include "YWXMLGY.h"
#include "CTax.h"
#include "CUserInfo.h"
#include "CInvKind.h"
#include "BusinessBase.h"

#if BUSINESS_TYPE_MODE == BUSINESS_JSK_MODE
#include "CNetPara.h"
#endif

class CManageBusinessFunc
{
public:
	CManageBusinessFunc();
	~CManageBusinessFunc();

public:

	/*!
	@brief 获取税控设备编号		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetSksbbh(CYWXML_GY &ywxml_gy, string &sksbbh, string &bspbh, string &qtxx, string &strErr);

	/*!
	@brief 获取税控设备信息		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetTaxpayerInfo(CYWXML_GY &ywxml_gy, CUserInfo &UserInfo, string &strCurTime, string &strErr);
	
	/*!
	@brief 获取报税盘信息		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetBSPInfo(CYWXML_GY &ywxml_gy, CUserInfo &userInfo, string &strErr);

	/*!
	@brief 获取监控数据		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetJKSJ(CYWXML_GY &ywxml_gy, CInvKind &InvKind, string &strErr);

	/*!
	@brief 获取税率信息		
	@param[in] pTax 税率类数组
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetTaxRateInfo(CYWXML_GY &ywxml_gy, CTax *pTax, UINT8 &taxNum, string &strErr);

	/*!
	@brief 金税盘口令变更
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 SKPKLBG(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr);

	/*!
	@brief 更新离线信息		
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 LXXXUpdatePro(CYWXML_GY &ywxml_gy, string &strErr);

	/*!
	@brief 变更证书口令
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 BGZSKLProc(CYWXML_GY &ywxml_gy, string yzskl, string xzskl, string &strErr);

#if BUSINESS_TYPE_MODE == BUSINESS_JSK_MODE
	/*!
	@brief 网络参数维护
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 NetParaManage(CYWXML_GY &ywxml_gy, CNetPara *pNetPara, string &strErr);


	/*!
	@brief 获取离线相关数据
	@param[in] 
	@param[out] fpzs   未上传的发票张数
	@param[out] fpsj   未上传发票时间（第一张未上传发票的开票日期YYYYMMDDHHMMSS）
	@param[out] fpljje 未上传发票累计金额（保留两位小数）
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetOffLineInvInfo(UINT32 &wscfpzs, string &wscfpsj, INT64 &wscfpljje, string &strErr);
#endif

};


#endif


