/*! \file    TemplateIndep.h
\brief    打印模块接口。
\author   Xiao Pengkun   2005-07-20
 */


#ifndef   TEMPLATE_INDEP_H
#define   TEMPLATE_INDEP_H

#include "TemplateDataDesign.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern UINT8  templateBuffer[PRINT_TEMPLATE_BUF_LEN];
extern TPrnInvoiceInfo g_invPrnData;



#include "TemplateDataDesign.h"


/*!
@brief 完成对模板行的解析，输出最终打印行数据。
@param[in] pInv  指向发票数据结构的指针
@param[out] pPrnLine  返回值为指向打印行结构的指针
@note 输入发票数据结构，通过解析模板行，将对应的属性数据添加到相应的结构
		 体中，输出为指向最终打印数据行的指针，循环调用此函数，完成对打印模板的解析和
		 业务数据的添加，每调用一次输出模板行和业务数据结合生成的最终打印行。
@return  0 - 成功
*/
UINT8  ParseTempLine(TPrnInvoiceInfo *pInv,TPrnLineInfo **pPrnLine);


/*!
@brief 检验模板的有效性
@note 调用解析模板行函数，如果返回0则成功，如果错误则重置缓冲区
*/
UINT8  CheckTempValid(void);


/*!
@brief 完成对打印模板所需缓冲区的重置,保证下一次调用时的正确性
@note 主要完成对打印模板指针prnTempBuf的重置，如果调用模板出现任何错误，
		 都必须掉用此函数进行重置
*/
void ResetTempBuf(void );


/*!
@brief 返回模板属性
@note 将模板行的相关属性存储，结构体为SaveTemplateInfo
@return  模板属性
*/
TSaveTemplateInfo * SaveTemplate(void);

/**
 * brief 读取打印模板
 * @param UINT8 TemplateNo 模板号
 * @param UINT32 len 模板长度
 * @return static UINT8 
 */
UINT8 ReadPrintTemplate(UINT8 TemplateNo);

/**
 * brief 加载打印模板
 * @param UINT8 TemplateNo 模板号
 * @param UINT32 len 模板长度
 * @return static UINT8 
 */
UINT8 LoadPrintTemplate(UINT8 TemplateNo);

/**
 * brief 设置开机工作模式
* @param workMode    WORK_MODE-正常销售  TRAINING_MODE-训练模式
 */
void  SetTplInfoWorkMode(UINT8 workMode);
/**
 * brief 获取开机工作模式
* @return 工作模式
 */
UINT8 GetTplInfoWorkMode();
/**
 * brief 获取模板黑标方式
* @return 
 */
UINT8 GetTplInfoMarkFlag();
/**
 * brief 获取最大商品明细数量
* @return 
 */
UINT8 GetTplInfoMaxGoodsCount();
/**
 * brief 获取最大商品名称数
* @return 
 */
UINT8 GetTplInfoMaxSPMCCharacter();
/**
 * brief 获取打印结束后走纸距离
* @return 
 */
UINT16 GetTplInfoEndForwardPoint();


#ifdef __cplusplus
}
#endif

#endif







