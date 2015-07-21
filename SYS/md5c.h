/*! \file    md5c.h
   \brief    生成md5签名
 */

#ifndef __MD5C_H
#define __MD5C_H

#include <stdio.h>
#include "comdatatype.h"
#include "SYSModuleHeader.h"

#if (USE_MD5_FUNC!=0)


#ifdef __cplusplus
extern "C"
{
#endif

/* MD5 context. */
typedef struct 
{
	UINT32 state[4];                                   	/* state (ABCD) */
	UINT32 count[2];        							/* number of bits, modulo 2^64 (lsb first) */
	UINT8  buffer[64];                        		 	/* input buffer */
} MD5_CTX;


/*!
@brief MD5算法初始化。
@param[out] MD5_CTX*  输出md5操作句柄
*/
void MD5Init (MD5_CTX *);

/*!
@brief 生成MD5签名。
@param[in] MD5_CTX*  md5操作句柄
@param[in] UINT8*  被签名的字串
@param[in] UINT32  被签名串的长度
*/
void MD5Update (MD5_CTX *, UINT8 *, UINT32);

/*!
@brief MD5签名结果输出。
@param[out] UINT8*  输出MD5签名结果
@param[in] MD5_CTX*  md5操作句柄
*/
void MD5Final (UINT8 *, MD5_CTX *);


extern MD5_CTX *g_MD5_CTX;			/**< 可用于md5操作句柄的全局变量	*/


#ifdef __cplusplus
}
#endif


#endif	//USE_MD5_FUNC

#endif	//__MD5C_H

