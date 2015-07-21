/*! \file    TemplatePrintInv.h
\brief    发票打印接口文件
\author   zfj
 */

#ifndef TEMPLATE_PRINT_INV_H
#define TEMPLATE_PRINT_INV_H


#include "comdatatype.h"
#include "TemplateDataDesign.h"
#include "TPLModuleHeader.h"



#ifdef __cplusplus
extern "C"
{
#endif

#define QR_CODE 2
#define DM_CODE 3
#define HX_CODE 1

extern UINT8 bidirection ;
extern UINT8  *m_RemainLineCount;             /**< 掉电剩余打印行个数 */
extern UINT8  *m_BeginPrintFlag;              /**< 标记掉电时是否已经开始打印数据 */

/*!
@brief 打印发票表头
@param[in] invData  发票信息
@return  SUCCESS-成功；FAILURE-出错 
*/
INT8 print_invoice_head( TPrnInvoiceInfo *invData);

/*!
@brief 打印商品明细,不定长版
@param[in] invData  发票信息
@return  SUCCESS-成功；FAILURE-出错 
*/
INT8 print_invoice_data(TPrnInvoiceInfo *invData);

/*!
@brief 循环打印商品明细行
@param[in] invData  发票信息
@param[in] unfixedFlag  不定长标志，主要适用在打不定长存根和退票
@return  SUCCESS-成功；FAILURE-出错 
*/
INT8 print_invoice_data_cycle(TPrnInvoiceInfo *invData,UINT8 unfixedFlag);

/*!
@brief 打印发票票尾
@param[in] invData  发票信息
@return  SUCCESS-成功；FAILURE-出错 
*/
INT8 print_invoice_tail(TPrnInvoiceInfo *invData);

/*!
@brief 真正的打印程序
@param[in] pPrnLineInfo  打印行数据结构
@return  SUCCESS-成功；FAILURE-出错 
*/
INT8 print_invoice_content(TPrnLineInfo *pPrnLineInfo);


/*!
@brief 打印二维码
@param[in] bufbiznfo  二维码字符串
@return  SUCCESS-成功；FAILURE-出错 
*/
#if (PRINT_BIZNFO!=0)
INT8 print_biznfo(UINT8 *bufbiznfo);
#endif


/*!
@brief 设置掉电保护参数实体
@param[in] remanCount  掉电剩余打印行个数
@param[in] beginFlag  标记掉电时是否已经开始打印数据
@return  SUCCESS-成功；FAILURE-出错 
*/
void set_power_pointer(UINT8 *remanCount, UINT8 *beginFlag);

/*!
@brief 设置掉电剩余打印行个数
@param[in] count  掉电剩余打印行个数
@return  SUCCESS-成功；FAILURE-出错 
*/
INT8 set_power_count(UINT8 count);

/*!
@brief 设置掉电时是否已经开始打印数据
@param[in] flag  标记掉电时是否已经开始打印数据
@return  SUCCESS-成功；FAILURE-出错 
*/
INT8 set_power_flag(UINT8 flag);

/*!
@brief 判断打印机当前有否有纸
@return  -1——失败，0——成功
*/
INT8 isPaper(void);


#ifdef __cplusplus
}
#endif

#endif
