/*! \file    SaleFunc.h
   \brief    awe4000r销售中的应用函数
   \author   Yu Yan
   \version  1.0
   \date     2008-01-31
 */


#ifndef SALE_FUNC_H
#define SALE_FUNC_H

#include "IncludeMe.h"
#include "SysMacDef.h"

#include "CMachine.h"
//#include "CPlu.h"
#include "CDept.h"
//#include "CInvSum.h"
#include "CInvHead.h"
#include "CInvDet.h"
#include "CInvVol.h"
#include "CUserInfo.h"
//#include "CInvcodeLink.h"

#include "TemplateDataDesign.h"
#include "TemplateGeneralFuc.h"
#include "TemplateIndep.h"
#include "TemplateInterface.h"
#include "TemplateMacro.h"
#include "TemplatePrintInv.h"
#include "arithmetic.h"
#include "commonFunc.h"

#include "PrintDriver.h"
#include "TDateTime.h"
#include "CGlobalArg.h"


/*!
@brief 获取当前发票号
@param[out] CInvVol *pInvVol 当前发票号码，代码和类型，剩余发票份数
@param[out] string &strErr 错误信息
@return  1： 成功； 其它：失败
*/
UINT8 SALE_GetCurInv(CInvVol *pInvVol,string &strErr);


/*!
@brief 发票开具 (正票，负票,空白作废)
@param[in] CInvHead *pInvHead 发票开具信息 
@param[out] CInvHead *pInvHead 发票税控码
@param[out] string &strErr 错误信息
@return  1： 成功； 其它：失败
*/
UINT8 SALE_MakeInvHand(CInvHead *pInvHead,string &strErr);

//----发票打印

/*!
@brief 填充打印发票结构体的发票头
@param[out] pPrnInvInfo 发票打印数据结构体指针
@param[in] smallInvInfo 发票整体信息指针
@return  1： 成功； 其它：失败
*/
UINT8 FillPrnInvHead(TPrnInvoiceInfo *pPrnInvInfo, CInvHead *smallInvInfo);

/*!
@brief 填充打印发票结构体的发票明细
@param[out] pPrnInvInfo 发票打印数据结构体指针
@param[in] smallInvInfo 发票整体信息指针
@return  1： 成功； 其它：失败
*/
UINT8 FillPrnInvDetail(TPrnInvoiceInfo *pPrnInvInfo, CInvHead *smallInvInfo);

/*!
@brief 填充打印发票结构体的发票尾
@param[out] pPrnInvInfo 发票打印数据结构体指针
@param[in] smallInvInfo 发票整体信息指针
@return  1： 成功； 其它：失败
*/			
UINT8 FillPrnInvTail(TPrnInvoiceInfo *pPrnInvInfo, CInvHead *smallInvInfo);


/*!
@brief 若需要滚动，检查是否记录未申报，导致无法滚动
@param[in] nInvHeadNum INV_HEAD表的记录数量
@param[in] nInvSumNum INV_SUM表的记录数量
@return  0： 可以滚动； 1：无法滚动，须申报
*/
UINT8 CheckIfFull(INT32 nInvHeadNum, INT32 nInvSumNum);

/*!
@brief 进纸
@return  1 成功；0  没纸了
*/
UINT8 ForwardPaper(void);

/*!
@brief 不找黑标进纸
@return  1 成功；0  没纸了
*/
UINT8 NoMarkForwardPaper(void);

/*!
@brief 退纸
@return  1 成功；0  没纸了
*/
UINT8 BackwardPaper(void);	


/**
 * brief 添加开票的系统日志记录
 * @param[in]  nInvType 开票类型
 * @param[in]  EventResult  事件结果
 * @param[in]  Backup 备注信息
 * @return 1: SUCCESS, 0: FAILURE
 */
UINT8 MakeInvLog(UINT8 nInvType, UINT32 EventResult, string Backup = "");

/**
 * brief 查询未上传张数
 * @param[in]  uNum 未上传张数
 * @param[in]  strErr  错误信息
 * @return 1: SUCCESS, 0: FAILURE
 */
UINT8 GetServNum(UINT32 &uNum,string &strErr);

#ifndef WIN32
//------线程相关--------//
void * NetCommunicate(void *arg);

//上传核心函数
UINT8 UploadInvProc();
#endif

#endif
