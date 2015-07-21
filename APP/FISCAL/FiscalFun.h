/*! \file    FiscalFun.h
   \brief    几个税控通用函数
   \author   zl
   \version  1.0
   \date     2008-01-01
 */

#ifndef _FISCAL_FUNC_H_
#define _FISCAL_FUNC_H_

#include "IncludeMe.h"
#include "SysMacDef.h"
#include "comdatatype.h"
#include "arithmetic.h"


/**
 * @brief  将日期时间转换为hex格式
 */
void FSC_InvDateHex(UINT32 date, UINT32 time, UINT8 *hexInvDate);

/*!
@brief 初始化入口函数
@param[in]  strOldPsw  原口令
@param[in]  strNewPsw  新口令
@param[out] strErr 错误信息，既包括调用业务二接口返回的信息也包括该函数运行时自身产生的错误信息
@return  1： 成功； 其它：失败
*/
UINT8 FSC_InitProc(const string &strOldPsw, const string &strNewPsw, string &strErr);


/*!
@brief 从金税盘获取纳税人基本信息并保存
@param[out]  strErr  错误信息
@return 1  成功， 0  失败
*/
UINT8 FSC_GetNsrInfo(string &strErr);

/*!
@brief 从金税盘获取授权税率并保存
@param[out]  strErr  错误信息
@return 1  成功， 0  失败
*/
UINT8 FSC_GetTaxInfo(string &strErr);

/*!
@brief 从金税盘获取离线控制信息并保存
@param[out]  strErr  错误信息
@return 1  成功， 0  失败
*/
UINT8 FSC_GetInvKindInfo(string &strErr);

/*!
@brief 获取税盘和报税盘参数信息（包括同步金税盘时钟）并保存金
@param[out]  strErr  错误信息
@return 1  成功， 0  失败
*/
UINT8 FSC_GetJSKInfo(string &strErr);

/*!
@brief 登录时更新纳税人信息和离线控制信息，并存入款机数据库
@param[out]  strErr  错误信息
@return 1  成功， 0  失败
*/
UINT8 FSC_InfoUpdate(string &strErr);


/*!
@brief 网络申报
@return 
*/
void FSC_NetDeclare();

/*!
@brief 检查介质申报条件
@1.发行当日不允许报税
@2.当日不允许抄报两次
@return 1  成功， 0  失败
*/
// UINT8 FSC_CheckDeclare(string &strErr);

/*!
@brief 介质申报
@return 
*/
void FSC_DiskDeclare(UINT8 uJZlx);

/*!
@brief 网络清卡
@return 
*/
void FSC_NetUpdateTax();

/*!
@brief 清零解锁（报税盘）
@return 
*/
void  FSC_DiskUpdateTax();

/*!
@brief 修改金税盘口令
@param[in]  strOldPsw  旧口令
@param[in]  strNewPsw  新口令
@return 1  成功， 0  失败
*/
UINT8 FSC_ChangeDiskPsw(string strOldPsw, string strNewPsw,string  &strErr);

/*!
@brief 发票补录
@param[in]  SDate  起始日期
@param[in]  EDate  结束日期
@return 1  成功， 0  失败
*/
UINT8 FSC_FpblProc(UINT32 SDate, UINT32 EDate, string &strErr);

#endif //_COMMON_H_
