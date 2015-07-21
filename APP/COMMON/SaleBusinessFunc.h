/****************************************************************************
文件名           ：SaleBusinessFunc.h
功能             ：awe4000r销售业务功能流程函数。
起始日期         ：
作者             ： 
****************************************************************************/

#ifndef SALE_BUSINESS_FUNC_H
#define SALE_BUSINESS_FUNC_H

#include "YWXMLGY.h"
#include "CInvVol.h"
#include "CInvHead.h"

#define	MAX_ERR_NUM		1//10


class CSaleBusinessFunc
{
public:
	CSaleBusinessFunc();
	~CSaleBusinessFunc();


public:

	
	/*!
	@brief 购票信息查询（获取当前票号）
	@param[in] 
	@param[out] pInvVol  发票信息类
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetCurInvInfo(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr);
	
	/*!
	@brief 发票开具（包含正票、红票）		
	@param[in] invhead			发票头信息
	@param[out] taxCode   	税控码（20字节）
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 MakeNormalInv(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, string &strErr);
	
	/*!
	@brief 发票作废（空白废票、已开票作废）
	@param[in] invhead	发票头信息
	@param[in] zflx   作废类型	0：空白发票作废	1：已开发票作废
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 MakeWasteInv(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, UINT8 zflx, string &strErr);
	
	/*!
	@brief 发票查询
	@param[in] cxfs	查询方式
	@param[in] cxtj	查询条件
	@param[out] invNum  发票张数
	@param[in/out] invhead	发票头信息
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 GetInvHeadInfo(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, UINT32 &invNum, CInvHead *pInvhead, string &strErr);
	
	/*!
	@brief 发票实时上传
	@param[out] strErr   		错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 InvoiceUpload(CYWXML_GY &ywxml_gy, string &strErr);
	
	/*!
	@brief 发票补录
	@param[in] nsrsbh	纳税人识别号
	@param[in] sksbbh   税控设备编号
	@param[in] SDate   报税起始日期
	@param[in] EDate	报税截止日期
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 FpblProStart(CYWXML_GY &ywxml_gy, UINT32 SDate, UINT32 EDate, string &strErr);

};


#endif


