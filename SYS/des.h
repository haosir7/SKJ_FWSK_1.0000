/*! \file    des.h
   \brief    des加密
 */
#ifndef __DES_H
#define __DES_H

#include "comdatatype.h"
#include "SYSModuleHeader.h"

#if (USE_DES_FUNC!=0)


#ifdef __cplusplus
extern "C"
{
#endif
	
#define	DES_ECB_PADDING					1
#define	DES_ZERO_PADDING				2

#define	DES_PADDING_MODE			DES_ECB_PADDING	
	

INT32 des(UINT8 *source, UINT8 * dest, UINT8 * inkey, INT32 flg);


/*!
@brief 执行3Des加密。
@param[in] in  原文数据
@param[in] in_len  原文长度
@param[out] out  加密后的输出串
@param[in] flag  填充方式：DES_ECB_PADDING或DES_ZERO_PADDING
@param[in] key  密钥
*/
int data_convert(unsigned char *in,int in_len,unsigned char * out,char flag,unsigned char *key);



#ifdef __cplusplus
}
#endif


#endif		//USE_DES_FUNC

#endif		//__DES_H


