/*! \file    commonFunc.h
   \brief    通用函数的定义文件
   \author   zl
 */

#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include "comdatatype.h"
#include "SYSModuleHeader.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*!
@brief 把小写金额转换成大写金额
@param[in] Amount 输入金额，单位为分
@param[in] outBuffLen 指定输出缓冲区outBuff的大小，不可小于64
@param[out] outBuff 保存大写金额,不小于64字节
@return  NULL-失败，否则成功，返回指向大写金额的句柄
*/
char *ChineseAmount(INT64 Amount, char *outBuff,UINT8 outBuffLen);


/*!
@brief 延时函数。使用nanosleep，被alarm打断后会继续剩余的延时
@param[in] ms 延时长度，单位毫秒
*/
void CommonSleep(int ms);


#if (USE_ICONV_LIB!=0)
/*!
@brief 字符集转换函数
@param[in] from_charset 原字符集类型（如"gbk"）
@param[in] to_charset 目标字符集类型（如"utf-8"）
@param[in] inStr 输入的字符串
@param[in] inlen 输入字符串的长度
@param[out] **outStr 传出字符串，需传入指针的引用
@return 0 - 成功；-1 - open错误；-2 - 转换错误
*/
int code_convert(char *from_charset, char *to_charset, 
				 const char *inStr, int inlen, char **outStr);

#endif	//USE_ICONV_LIB


#ifdef __cplusplus
}
#endif

#endif	//COMMON_FUNCTION_H
