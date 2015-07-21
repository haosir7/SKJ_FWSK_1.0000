/****************************************************************************
文件名           ：DeclareBusinessFunc.h
功能             ：awe4000r抄报业务功能流程函数。
起始日期         ：
作者             ： 
****************************************************************************/

#ifndef DECLARE_BUSINESS_FUNC_H
#define DECLARE_BUSINESS_FUNC_H

#include "YWXMLGY.h"

class CDeclareBusinessFunc
{
public:
	CDeclareBusinessFunc();
	~CDeclareBusinessFunc();

public:

	/*!
	@brief 网络抄报
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 NetDeclareProc(CYWXML_GY &ywxml_gy, string &strErr);

	/*!
	@brief 介质抄报（抄报时提示选择金税盘或者报税盘）
	@param[in] jzlx 介质类型  1：报税盘 2：金税盘 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 DeclareProc(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr);

	/*!
	@brief 网络清卡
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 NetUpdateTaxProc(CYWXML_GY &ywxml_gy, string &strErr);

	/*!
	@brief 报税盘清卡
	@param[in] 
	@param[out] strErr   错误信息
	@return  1 SUCCESS； 0  FAILURE
	*/
	INT32 UpdateTaxProc(CYWXML_GY &ywxml_gy, string &strErr);

};


#endif


