/*! \file    iconv.h
   \brief    将iconv标准库中GBK与UTF8转换的相关代码提取后做成新库。
			 业务层不必直接调用本文件中的接口，可调用commonFunc.h中的code_convert函数实现转换
   \author   ZL
   \version  1.0
   \date     2012-4-05
 */

#ifndef _ICONV_GBK_UTF8_H__
#define _ICONV_GBK_UTF8_H__

#include "iconv_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif


                              
/*!
@brief 指定字符集转换的形式并获取操作句柄
@param[in] from_charset 原字符集类型（如"gbk"）
@param[in] to_charset 目标字符集类型（如"utf-8"）
@return  操作句柄
*/
iconv_t iconv_open(ICONV_CONST char* tocode, ICONV_CONST char* fromcode);

/*!
@brief 转换过程
@param[in] icd 操作句柄
@param[in] inbuf 原字串
@param[in] inbytesleft 原字串长
@param[out] outbuf 输出字串
@param[in,out] outbytesleft 传入输出最大允许长度，传出实际长度
*/                                    
size_t iconv (iconv_t icd,
              ICONV_CONST char* * inbuf, size_t *inbytesleft,
              char* * outbuf, size_t *outbytesleft);
              

/*!
@brief 释放操作句柄
@param[in] icd 操作句柄
*/
int iconv_close (iconv_t icd);
    
    
    
    
#ifdef __cplusplus
}
#endif

    
#endif
