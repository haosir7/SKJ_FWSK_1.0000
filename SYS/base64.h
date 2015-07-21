/*! \file    base64.h
   \brief    base64编码解码函数的定义文件
   \author   zl
 */

#ifndef __BASE64_H_
#define __BASE64_H_

#ifdef __cplusplus
extern "C"
{
#endif


/*!
@brief 经过修改的Base64编码函数，编码前后使用同一段BUFF以节省内存。
@param[in,out] buf  输入原始数据，输出Base64编码后的数据
@param[in] vlen  原始数据长度
@warning 编码之后数据长度会增加约4/3倍，务必在调用前先确认给定BUF足够大。
*/
void Base64_Encode_2(unsigned char *buf, int vlen);


/*!
@brief 经过修改的Base64解码函数，解码前后使用同一段BUFF以节省内存。
@param[in,out] value  输入Base64编码的数据，输出解码后的数据
@param[in] rlen  解码后的数据长度
*/
void Base64_Decode_2(char *value, int *rlen);	


/*!
@brief 原始的Base64编码函数。
@param[in] value  原始数据
@param[in] vlen  原始数据长度
@param[out] pcBase64Value  Base64编码后的输出数据
@note 由于内存限制，华邦平台的业务层代码未使用此函数。
*/
void Base64_Encode(unsigned char *value, int vlen, char *pcBase64Value);  

/*!
@brief 原始的Base64解码函数。
@param[in] value  Base64编码的数据
@param[out] result  Base64解码后的输出数据
@param[out] rlen  Base64编码后的输出数据长度
@note 由于内存限制，华邦平台的业务层代码未使用此函数。
*/
void Base64_Decode(char *value, unsigned char *result, int *rlen);  



#ifdef __cplusplus
}
#endif

#endif	//__BASE64_H_


