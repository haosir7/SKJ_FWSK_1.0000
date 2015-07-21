/*! \file    ReportFunc.h
   \brief    awe4000r报表打印的应用函数。
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */


#ifndef REPORT_FUNC_H
#define REPORT_FUNC_H

#include "IncludeMe.h"
#include "SysMacDef.h"
#include "CaMsgBox.h"
#include "CSysLog.h"
#include "printer.h"
//#include "CPlu.h"
#include "CClient.h"
#include "CDept.h"
#include "COperator.h"
#include "CTjxxhz.h"

extern INT8 *pGeneralPrnBuff;

/*!****************************************************************************
<PRE>
结构名称  : _TCheckOld
功能描述  : 发票验旧结构
作者	  : zcy
日期	  : 2011-06-28
</PRE>
*******************************************************************************/
typedef struct _TCheckOld 
{
	string taxPayerFileNo;			//纳税人电子账单号
	string taxPayerID;				//纳税人识别号
	string invKindCode;            	//发票种类代码
	string invKindName;				//发票种类名称
	string invCode;					//发票代码
	string invName;					//发票名称
	string invSNo;					//发票起始号
	string invENo;					//发票终止号
	int	   num;				    	//份数
	string checkDate;				//购买日期
	string searchNo;				//查询号
}TCheckOld;

/*!****************************************************************************
<PRE>
结构名称  : _TCheckOldInfo
功能描述  : 发票验旧结构内容
作者	  : zcy
日期	  : 2011-06-30
</PRE>
*******************************************************************************/
typedef struct _TCheckOldInfo 
{
	string taxPayerFileNo;			//纳税人电子账单号
	string taxPayerID;				//纳税人识别号
	string invKindCode;				//发票种类代码
	string invCode;					//发票代码
	string checkDate;				//购买日期
	int	   num;				    	//份数
	string invSNo;					//发票起始号
	string invENo;					//发票终止号
	string makeDate;                 //开票日期
	string money;                    //填开金额
	string taxMoney;				//填开税额
   	string remarks;					//备注
    string resCode;					//验旧结果代码
	string resContent;				//验旧结果描述
	string makeSDate;				//开票起始日期
	string makeEDate;				//开票截止日期
	string inDate;					//录入日期
	string modifyDate;				//修改日期
	string searchNo;				//查询号

}TCheckOldInfo;

/*!
@brief yes/no消息提示框	
@param[in] strInfo   提示框显示的字符串
@return  1: SUCCESS（是）; 0: FAILURE（否）
*/
UINT8 YesNoMsgBox(string strInfo);

/*!
@brief 获取日汇总表某时间段的记录数	
@param[in] nStartDate   起始日期
@param[in] nStartDate   截止日期
@param[out] nNum   记录数
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 GetDaySumCount(UINT32 nStartDate, UINT32 nEndDate, UINT32 &nNum);

/*!
@brief 获取月汇总表某时间段的记录数	
@param[in] pTjxxhz   统计汇总表
@param[out] pInvsum  数据库表
@param[out] strErr   记录数
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 GetMonthSumCount(CTjxxhz *pTjxxhz, string &strErr);

/*!
@brief 判断给定时间是否合法， 把时间组织成格式为HHMMSS无符号整型作为
@param[in] hhmmss － 格式为hhmmss的无符号整型
@return  1: SUCCESS（是）; 0: FAILURE（否）
*/
UINT8 IsTimeValid(UINT32 hhmmss);

/*!
@brief 打印某时间段的系统日志的报表头部
@param[in] nStartDate 时间段的起始日期
@param[in] nEndDate   时间段的结束日期
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnSysLogHead(UINT32 nStartDate, UINT32 nEndDate);

/*!
@brief 打印系统日志的某条记录
@param[in] pSysLog 系统日志记录明细的指针
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnSysLogDetail(const CSysLog *pSysLog);

/*!
@brief 打印PLU的报表头部
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnPLUHead();

/*!
@brief 打印某条PLU信息
@param[in] pPlu PLU信息的指针
@return  1: SUCCESS; 0: FAILURE
*/
//UINT8 PrnPLUDetail(const CPlu *pPlu);

/*!
@brief 打印客户的报表头部
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnClientHead();

/*!
@brief 打印某条客户信息
@param[in] pClient 客户信息的指针
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnClientDetail(const CClient *pClient);

/*!
@brief 打印部类的报表头部
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnDeptHead();

/*!
@brief 打印部类信息
@param[in] pDept 部类信息的指针
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnDeptDetail(const CDept *pDept);

/*!
@brief 打印操作员的报表头部
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnOperatorHead();

/*!
@brief 打印操作员信息
@param[in] pOperator 操作员信息的指针
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnOperatorDetail(const COperator *pOperator);

/*!
@brief 打印某时间段销售员销售统计报表头部
@param[in] nStartDate 起始日期
@param[in] nStartTime 起始时间
@param[in] nEndDate 截止日期
@param[in] nEndTime 截止时间
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnSalerSumHead(UINT32 nStartDate, UINT32 nStartTime, UINT32 nEndDate, UINT32 nEndTime);

/*!
@brief 打印操作员名称
@param[in] pSaler 操作员名称
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnSaler(const string *pSaler);

/*!
@brief 打印销售员销售的部类商品信息
@param[in] pDeptName 部类名称
@param[in] nSum 部类销售金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnSalerDeptSum(const string *pDeptName, INT64 nSum);

/*!
@brief 打印销售员的销售总量信息
@param[in] nInvCount 该销售员的开票数量
@param[in] nSum 该销售员的销售金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnSalerSum(UINT32 nInvCount, INT64 nSum);

/*!
@brief 打印某时间段销售统计报表头部
@param[in] nStartDate 起始日期
@param[in] nStartTime 起始时间
@param[in] nEndDate 截止日期
@param[in] nEndTime 截止时间
@param[in] nIfDay 是日或月销售统计，1：日销售统计， 0：月销售统计
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnSaleSumHead(UINT32 nStartDate, UINT32 nStartTime, UINT32 nEndDate, UINT32 nEndTime, UINT8 nIfDay);

/*!
@brief 打印被统计的日期
@param[in] strIssueDate 日期（字符串）
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnDate(const string *strIssueDate);

/*!
@brief 打印某日销售的部类商品信息
@param[in] pDeptName 部类名称
@param[in] nSum 部类销售金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnDayDeptSum(const string *pDeptName, INT64 nSum);

/*!
@brief 打印某日的销售总量信息
@param[in] nInvCount 该日开票数量
@param[in] nSum 该日的销售金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnDaySaleSum(UINT32 nInvCount, INT64 nSum);

/*!
@brief 打印日销售统计的总计
@param[in] nInvCount 总计开票数量
@param[in] nSum 总计销售金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnAllDaySaleSum(UINT32 nInvCount, INT64 nSum);

/*!
@brief 统计并打印某时间段的部类销售信息
@param[out] sumMoney 部类累计销售金额
@param[in] strFilter 某时间段条件（sql语句）
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnDeptMoney(INT64 &sumMoney, const string *strFilter);

/*!
@brief 打印某时间段部类销售金额
@param[out] sumMoney 部类累计销售金额
@param[in] nStartDate 起始日期
@param[in] nStartTime 起始时间
@param[in] nEndDate 截止日期
@param[in] nEndTime 截止时间
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnPeriodDeptMoney(INT64 &sumMoney, UINT32 nStartDate, UINT32 nStartTime, UINT32 nEndDate, UINT32 nEndTime);

//月销售统计
/*!
@brief 打印统计日期所属的月份
@param[in] sumDate 日期
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnMonth(UINT32 sumDate);

/*!
@brief 打印某月销售的部类商品信息
@param[in] pDeptName 部类名称
@param[in] nSum 部类销售金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnMonthSaleSum(UINT32 nInvCount, INT64 nSum);

/*!
@brief 打印某时间段开票统计报表头部
@param[in] nStartDate 起始日期
@param[in] nStartTime 起始时间
@param[in] nEndDate 截止日期
@param[in] nEndTime 截止时间
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnIssueSumHead(UINT32 nStartDate, UINT32 nStartTime, UINT32 nEndDate, UINT32 nEndTime);

/*!
@brief 打印某时间段开票信息
@param[in] nInvNo 发票号
@param[in] nInvType 发票类型
@param[in] nInvMoney 开票金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnIssueSumCode(string fpdm);
UINT8 PrnIssueSumBody(UINT32 nInvNo, UINT8 nInvType, INT64 nInvMoney);

/*!
@brief 打印某时间段开票统计报表尾部
@param[in] nNormalNum 正票份数
@param[in] nReturnNum 红票份数
@param[in] nWasteNum  废票份数
@param[in] nNormalNum 正票金额
@param[in] nReturnNum 红票金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnIssueSumEnd(UINT32 nNormalNum, UINT32 nReturnNum, UINT32 nWasteNum, INT64 norMoney, INT64 retMoney);

/*!
@brief 打印累计金额报表
@param[in] normalSum 申报期正票累计金额
@param[in] returnSum 申报期红票累计金额
@param[in] normalVolSum  发票卷正票累计金额
@param[in] returnVolSum 发票卷红票累计金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnMoneyInfo(INT64  normalSum, INT64  returnSum, INT64  normalVolSum, INT64  returnVolSum);

/*!
@brief 打印某发票号段销售统计报表头部
@param[in] nStartDate 起始发票号
@param[in] nStartTime 截止发票号
@param[in] strpCode 发票代码指针
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnInvSaleSumHead(UINT32 nStartNo, UINT32 nEndNo, const string *strpCode);

/*!
@brief 打印某发票号段销售统计报表尾部
@param[in] nInvCount 总计开票数量
@param[in] nSum 总计销售金额
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnInvSaleSumEnd(UINT32 nInvCount, INT64 nSum);

/*!
@brief 打印发票验旧统计报表头
@param[in] nsrmc 纳税人名称
@param[in] swdjzh 税务登记证号
@param[in] checkoldHead 发票结存结构体指针
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnInvCheckOldHead(string nsrmc, string swdjzh, TCheckOld* checkoldHead);
/*!
@brief 打印发票验旧统计报表实体
@param[in] checkoldHead 发票结存结构体指针
@param[in] &nHeadNum 发票结存引用
@param[in] checkoldInfo 发票验旧结构体指针
@param[in] &nInfoNum 发票验旧引用
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnInvCheckOldBody(TCheckOld* checkoldHead, int &nHeadNum, TCheckOldInfo* checkoldInfo, int &nInfoNum);

/*!
@brief 打印发票票种信息报表
@param[in] bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标后开始打印
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 print_invkindifo(/*struct TInvKindInfoPrnData *invkindData, */ UINT8 bFindBlackMark);

INT8 print_taxiteminfo(UINT8 bFindBlackMark);

/*!
@brief 统计并打印某时间段的部类销售金额
@param[out] sumMoney 部类累计销售金额
@param[in] strFilter 某时间段条件（sql语句）
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrnTimeDeptMoney(INT64 &sumMoney,INT64 &norMoney,INT64 &retMoney, const string *strFilter);


//打印系统消息
//UINT8 PrnMessageDetail(const CMessage *message);

//打印一行虚线
INT8 PrintSeparator();
INT8 PrintDoubleSeparator();

//打印抬头
INT8 PrintTitle();

void DivideCNStr(string &str1, INT32 len, string &str2);
int GetHalfHZCount(unsigned char *str,int len);

#endif