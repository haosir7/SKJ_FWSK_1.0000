/*! \file    AutoIssueFunc.h
   \brief    awe4000r发票管理的自动开票函数
   \author   Yu Yan 
   \version  1.0
   \date     2008-5-21 
 */

#ifndef AUTO_ISSUE_FUNC_H
#define AUTO_ISSUE_FUNC_H

#include "IncludeMe.h"
#include "SysMacDef.h"
#include "SaleData.h"
#include "CRtInv.h"
#include "CaMsgBox.h"
#include "CInvVol.h"

/*!
@brief 内部容量测试
@param[out] nIfInvSum  是否执行了卷汇总操作，1：是；0：否
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 SimpleAutoIssue(UINT8 &nIfInvSum);

/*!
@brief 开一张正票	
@param[in] tmpSaleData  销售类对象的引用
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 SimpleMakeNormalInv(SaleData &tmpSaleData,string &strErr);

/*!
@brief 开一张红票	
@param[in] tmpSaleData  销售类对象的引用
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 SimpleMakeReturnInv(SaleData &tmpSaleData);

/*!
@brief 开一张废票
@param[in] tmpSaleData  销售类对象的引用	
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 SimpleMakeWasteInv(SaleData &tmpSaleData);

/*!
@brief 获取当前发票号码（若当前无发票，则自动导入已分发的发票卷）	
@param[out] invNo      当前发票号码 
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 GetInvNo(SaleData &tmpSaleData, UINT32 &invNo);

/*!
@brief 检查按键，判断是否退出自动开票程序	
@param[out] nKeyTime      连续按确认键的次数 
@return  1 是(SUCCESS)； 0 否(FAILURE)
*/
UINT8 CheckKey2(UINT8 &nKeyTime);

/*!
@brief 计算开票金额	
@param[in] date    日期
@param[in] No      税目序号 
@return  金额（已扩大100倍，个位数对应“分”）
*/
UINT32 ComputerMoney(UINT32 date, UINT8 No);

/*!
@brief 计算从申报起始日期起X个月后的申报截止日期	
@param[in] BeginDate    申报起始日期
@param[in] nMonths      月份数 
@return  申报截止日期
*/
UINT32 ThreeMonthsEndDay(UINT32 BeginDate, UINT8 nMonths);

/*!
@brief 判断某日期是否本月最后一天	
@param[in] nDate     待判断的日期
@return  1 是(SUCCESS)； 0 否(FAILURE)
*/
UINT8 IsLastDay(UINT32 nDate);

/*!
@brief 15所容量测试	
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 ComplexAutoIssue();


/*!
@brief 开一张正票	
@param[in] tmpSaleData  销售类对象的引用
@param[in] nTaxId  税目号
@param[in] nInvNo  发票号码
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 MakeNormalInv(SaleData &tmpSaleData, UINT8 nTaxId, UINT32 nInvNo);

/*!
@brief 开一张红票	
@param[in] tmpSaleData  销售类对象的引用
@param[in] nTaxId  税目号
@param[in] nInvNo  发票号码
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 MakeReturnInv(SaleData &tmpSaleData, UINT8 nTaxId, UINT32 nInvNo);


/*!
@brief 开一张废票	
@param[in] tmpSaleData  销售类对象的引用
@param[in] nTaxId  税目号
@param[in] nInvNo  发票号码
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 MakeWasteInv(SaleData &tmpSaleData);

/*!
@brief 正常一天的开票过程	
@param[in] tmpSaleData  销售类对象的引用
@param[in] nInvNum  正常一天开的发票总数量
@param[in] IssuedNum  当天已经开过的发票数量
@param[out] nKeyTime  按“确认”键的次数
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 NormalDayIssue(SaleData &tmpSaleData, UINT32 nInvNum, UINT32 IssuedNum, UINT8 &nKeyTime);


/*!
@brief 月末一天的开票过程	
@param[in] tmpSaleData  销售类对象的引用
@param[in] nInvNum  正常一天开的发票总数量
@param[in] IssuedNum  当天已经开过的发票数量
@param[out] nKeyTime  按“确认”键的次数
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 LastDayIssue(SaleData &tmpSaleData, UINT32 nInvNum, UINT32 IssuedNum, UINT8 &nKeyTime);


/*!
@brief 容量测试总接口	
@param[out] nIfInvSum  是否执行了卷汇总操作，1：是；0：否
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 AutoIssue(UINT8 &nIfInvSum);

/*!
@brief 判断发票张数（INV_HEAD表）是否要滚动，并显示消息框	
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 IfInvHeadRoll();

/*!
@brief 判断卷汇总（INV_SUM表）是否要滚动，并显示消息框	
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 IfInvSumRoll();

/*!
@brief 判断卷汇总（RT_INV表）是否要滚动，并显示消息框	
@return  1 SUCCESS； 0 FAILURE
*/
UINT8 IfRtInvRoll();

/*!
@brief 添加PLU表记录（直到表可容纳记录数的上限）	
@return  1 SUCCESS； 0 FAILURE
*/
//UINT8 WritePlu(void);

/*!
@brief 添加PLU表记录（直到表可容纳记录数的上限）	
@return  1 SUCCESS； 0 FAILURE
*/
//UINT8 WriteSysLog(void);

//--------------------bx
UINT8 NetMakeNormalInv(SaleData &tmpSaleData, CDept *pDept);
UINT8 NetAutoIssue(UINT8 &nIfInvSum);

#endif
