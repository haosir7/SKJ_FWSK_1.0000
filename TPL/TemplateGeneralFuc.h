/*! \file    TemplateGeneralFuc.h
\brief    打印模块通用函数
\author   Xiao Pengkun   2005-07-20
 */

#ifndef TEMPLATE_GENERAL_FUN_H
#define TEMPLATE_GENERAL_FUN_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "TemplateDataDesign.h"

//----------------------------------------------------
//商品取消标志，应用于不定长销售中，打印取消商品行需要
extern UINT8 DetailCancelFlag;
extern UINT8 g_cungen;
extern UINT8 is_gb_4byte(unsigned char *str);
extern UINT32 HZCount(UINT8 *str, UINT32 len, 
			   UINT8 *num_asc, UINT8 *num_hz2B, UINT8 *num_hz4B);
//----------------------------------------------------


/*!
@brief 判断半个汉字的个数
@param[in] str	需要判断的字符串
@param[in] len	字符串长度
@note 判断字符串中半个汉字的个数，如果为奇数，则需要进行处理
@return  半个汉字的个数
*/
UINT32 GetHalfHZCount(UINT8 *str,UINT32 len);


/*!
@brief 获取打印模板（文件）缓存
@param[in] p	
@note 输入指针，获得缓存，输入指针指向申请到的缓存。
@return  
*/
void GetPrintTemplateBuffer(void **p); 
/*!
@brief 获取最终打印行缓存
@param[in] p	
@note 输入指针，获得缓存，输入指针指向申请到的缓存。
@return  
*/
void GetPrintLineBuffer(void **p);


/*!
@brief 获取打印模板每一行缓存
@param[in] p	
@note 输入指针，获得缓存，输入指针指向申请到的缓存。
@return  
*/
void GetTemplateLineBuffer(void **p);


/*!
@brief 获取每一行模板数据俩个~之间的数据缓存
@param[in] p	
@note 输入指针，获得缓存，输入指针指向申请到的缓存。
@return  
*/
void GetTemplateLineFieldBuffer(void **p);


/*!
@brief 获取用来匹配俩个~之间的数据标签的缓存
@param[in] p	
@note 输入指针，获得缓存，输入指针指向申请到的缓存。
@return  
*/
void GetMatchFieldLabelBuffer(void **p);

/*!
@brief 初始化最终打印行
@param[in] prnLineInfo	
@note 将打印行数据结构初始化，属性chContent内容为空格，结尾为'\0'。
@return  
*/
void InitPrinLineInfo(TPrnLineInfo *prnLineInfo);


/*!
@brief 将chContent中的'\0'替换为空格' ',除了结尾
@param[in] pPrnLineInfo	
@note 将最终输出打印行中的结束符替换为空格，使得内容可以打印出来。
@return  
*/
void ScanReplaceSpace(TPrnLineInfo *pPrnLineInfo);

#ifdef __cplusplus
}
#endif

#endif

