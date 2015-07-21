/*! \file    InvManageFun.h
   \brief    awe4000r发票管理的应用函数
   \author   Yu Yan 
   \version  1.0
   \date     2008-01-31
 */

#ifndef INV_MANAGE_FUNC_H
#define INV_MANAGE_FUNC_H

#include "IncludeMe.h"
#include "SysMacDef.h"
#include "SaleData.h"
#include "CRtInv.h"


//----发票作废
/*!
@brief 发票作废主过程	
@return  1 成功； 非1 错误代码
*/
UINT8 INVM_InvWasteProc();
/*!
@brief 发票作废	
@param[in] nNum      作废发票份数 
@param[in] pSaleData      销售数据类对象指针 
@param[out] nIfInvSum  是否执行了卷汇总操作，1：是；0：否
@return  1 成功； 0 失败
*/
UINT8 INVM_InvWaste(UINT32 nNum, SaleData *pSaleData, UINT8 &nIfInvSum);

/*!
@brief 发票作废具体操作	
@param[in] pSaleData      销售数据类对象指针 
@return  1 成功； 0 失败
*/
UINT8 INVM_InvWasteHandle(SaleData *pSaleData);

/*!
@brief 构造作废发票	
@param[in] pInvHead      作废发票的对象指针 
@return  
*/
void INVM_BuildWasteInv(CInvHead *pInvHead);


/*!
@brief 废已开发票(核心过程)	
@param[in] strInvCode     作废发票的对象指针 
@param[in] invNo     发票号码 
@param[out] strErr     错误信息
@return  1 成功； 0 失败
*/
//UINT8	WasteHasMakeInv(string& strInvCode, UINT32 invNo, string& strErr);

/*!
@brief 作废已开发票	
@param[in] pInvHead     作废发票的对象指针 
@param[out] strErr     错误信息
@return  1 成功； 0 失败
*/
UINT8 INVM_WstInv(CInvHead* pInvHead, string &strErr);

//-----发票退回

/*!
@brief 检验发票是否存在
@param[in]  发票代码，发票号码
@param[out] pInvHead 查询发票信息
@return  1 成功； 非1  错误ID
*/
UINT8 INVM_CheckIsNoDB(string invCode,UINT32 invNo,CInvHead *pInvHead);

/*!
@brief 检验发票是否可退	
@param[in] pInvHead   所退发票整体信息指针
@return  1 成功； 非1  错误代码
*/
UINT8 INVM_CheckIsNoRet(CInvHead *pInvHead);


/*!
@brief 发票退回过程(退本机发票)
@param[in] pInvHead   所退发票整体信息指针
@return  1: SUCCESS; 非1  错误代码
*/
UINT8 INVM_InvRetProc(CInvHead *pInvHead);

/*!
@brief 退本机发票
@param[in] pInvHead   所退发票整体信息指针
@param[out] 
@return  1 成功； 非1  错误代码
*/
UINT8  INVM_InvReturn(SaleData *pSale, CInvHead *pInvHead);

/*!
@brief 退非本机发票
@param[out] pInvHead   所退发票整体信息指针
@param[in] codeNo      所退发票的发票代码 
@param[in] invNo       所退发票的发票号码  
@return  1 成功； 非1  错误代码
*/
// UINT8 ReturnGoods(SaleData *pSale, string codeNo, UINT32 invNo);

// UINT8 InvReturnSpe(SaleData *pSale);

/*!
@brief 查询发票代码对应的发票代码关联号
@param[in] code      发票代码 
@return  0  错误；非0 发票代码关联号
*/
// UINT8 CheckInvCode(INT8 *code);

//--------分发查询
/*!
@brief yes/no消息提示框	
@param[in] nNum   未导入的发票卷数 
@return  1: SUCCESS（是）; 0: FAILURE（否）
*/
//UINT8 YesNoBox(UINT32 nNum);
UINT8 YesNoBox(string strMsg);

/*!
@brief 统计未导入的发票卷数，并保存到nNum里。	
@param[out] nNum   未导入的发票卷数 
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 INVM_GetVolNum(UINT32 &nNum);

/*!
@brief 统计发票存根数	
@param[in] nCodeNo   发票代码关联号
@param[in] nStartNo   发票起始号码
@param[in] nEndNo   发票截止号码
@param[out] nNum   发票存根数 
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 INVM_GetInvNum(string nCode, UINT32 nStartNo, UINT32 nEndNo, UINT32 &nNum);

/*!
@brief 打印发票存根	
@param[in] smallInvInfo   发票代码关联号
@param[in] IfPrnChar   是否打印“电子存根”四字，1：打印，0：不打印
@return  1: SUCCESS; 非1: 错误代码
*/
UINT8 PrnInvStub(CInvHead *smallInvInfo, UINT8 IfPrnChar);

/*!
@brief 发票整卷作废	
@return  1: SUCCESS; 0: FAILURE
*/
//UINT8 WasteInvVol(void);

/*!
@brief 是否本月	
@return  1: SUCCESS; 0: FAILURE
*/
INT32 CmpCurMonth(UINT32 nDate, UINT32 nTime);

/*!
@brief 按号码段查询发票明细
@param[in] 发票代码，发票号码	
@return  1: SUCCESS; 非1: 错误信息ID
*/
UINT8 INVM_InvDetailNOQuery(string invCode,UINT32 invNO);

/*!
@brief 时间段发票查询
@param[in] 发票开具	年月
@return  1: SUCCESS; 0: FAILURE
*/
//UINT8 INVM_DownLoadInvDate(string invDate);
/*!
@brief 错误信息返回
@param[in] ret 错误ID	
@return  1: SUCCESS; 0: FAILURE
*/
void INVM_ErrMsgBox(UINT8 ret);



#endif
