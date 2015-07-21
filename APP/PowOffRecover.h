/****************************************************************************
文件名           ：PowOffRecover.h
功能             ：awe4000r销售中的应用函数。
起始日期         ：2008-01-31
作者             ：Yu Yan     
****************************************************************************/

#ifndef POWER_OFF_RECOVER_H
#define POWER_OFF_RECOVER_H

#include "powoff_protect.h"
#include "SaleData.h"

//------销售流程的掉电恢复

/*!
@brief 填充打印发票结构体的发票头
@param[in] invFullInfo 发票头信息结构体指针
@param[out] smallInvInfo 发票整体信息指针
@return  1： 成功； 其它：失败
*/
UINT8 InvStuctToClass(struct TInvFullInfo *invFullInfo, CInvHead *smallInvInfo);

/*!
@brief 开票恢复
@param[in] invFullInfo 发票头信息结构体指针
@param[in] nChildId 掉电恢复子过程号
@return  1： 成功； 0：失败
*/
UINT8 MakeInvRecover(struct TInvFullInfo *invFullInfo, UINT8 nChildId);

/*!
@brief 滚动恢复
@return  1： 成功； 0：失败
*/
UINT8 RollRecover(void);



//-----总掉电恢复

/*!
@brief 掉电恢复
@return  1： 成功； 0：失败
*/
UINT8 poweroff_recover();

/*!
@brief 掉电恢复进度显示
@param[in] strInfo 显示信息
*/
void ShowMsg(string strInfo);

#endif
