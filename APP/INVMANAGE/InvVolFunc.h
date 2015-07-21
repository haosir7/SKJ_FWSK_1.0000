/*! \file    InvVolFun.cpp
\brief    票源管理应用函数
\author   zcy
\version  1.0
\date     2015-05-07
*/

#ifndef _INVVOL_FUN_H
#define _INVVOL_FUN_H

#include "IncludeMe.h"
#include <string>
using namespace std;


/*!
@brief 获取票源段信息，并保存到inv_vol表
@brief 对应直连规范里的报税盘发票查询，即发票卷查询
@param[out] uNum    票源段信息	
@param[out] strErr  消息框显示信息 
@return  1：SUCCESS；0：FAILURE
*/
UINT8 INV_GetInvVol(UINT32 &uNum,string &strErr);


/*!
@brief 网络购票，并把票源读入
@param[in] strCode  发票代码 
@param[in] uStartNo  发票起始号码
@param[in] uNum  发票总份数 
@param[out] strErr  消息框显示信息 
@return  1：SUCCESS；0：FAILURE
*/
UINT8 INV_NetInvVol(string strCode,UINT32 uStartNo,UINT32 uNum, string &strErr);

/*!
@brief 网络购票结果确认
@param[in] strCode  发票代码 
@param[in] uStartNo  发票起始号码
@param[in] uEndNo  发票总份数 
@param[out] strErr  消息框显示信息 
@return  1：SUCCESS；0：FAILURE
*/
// UINT8 INV_NetInvVolVerify(string strCode,UINT32 uStartNo,UINT32 uEndNo, string &strErr);

/*!
@brief 把金税盘中票源读取票源区	
@param[in] uJZlx   介质类型
@param[out] strErr  消息框显示信息 
@return  1：SUCCESS；0：FAILURE
*/
UINT8 INV_ReadInvVol(UINT8 uJZlx,string &strErr);


/*!
@brief 主机分票
@param[in] uJZlx   介质类型	
@param[in] string strCode 发票代码
@param[in] UINT32 uNo 发票起始号
@param[in] UINT32 uNum 发票张数
@param[out] strErr  消息框显示信息 
@return  1：SUCCESS；0：FAILURE
*/
UINT8 INV_HostToInvVol(UINT8 uJZlx,string strCode,UINT32 uNo,UINT32 uNum,string &strErr);

/*!
@brief 发票退回	
@param[in] uJZlx   介质类型
@param[in] string strCode 发票代码
@param[in] UINT32 uNo 发票起始号
@param[in] UINT32 uNum 发票张数
@param[out] strErr  消息框显示信息 
@return  1：SUCCESS；0：FAILURE
*/
UINT8 INV_RetToInvVol(UINT8 uJZlx,string strCode,UINT32 uNo,UINT32 uNum,string &strErr);


/*!
@brief 主机读入分机退回发票
@param[out] strErr  消息框显示信息 
@return  1：SUCCESS；0：FAILURE
*/
UINT8 INV_HostGetInvVol(UINT8 uJZlx,string &strErr);


/*!
@brief 选择介质类型
@param[out] uJZlx   介质类型
@param[out] strErr  消息框显示信息 
@return  1：SUCCESS；0：FAILURE
*/
UINT8 INV_MediumType(UINT8 &uJZlx,string &strErr);

/*!
@brief 选择介质类型
@param[out] strErr  消息框显示信息 
@return  1：SUCCESS；0：FAILURE
*/
UINT8 INV_InvoiceUpload(string &strErr);

#endif
