/*! \file    rsaFunc.h
   \brief    Rsa加解密接口定义
 */
#ifndef __APE_RSA__FUNC_H
#define __APE_RSA__FUNC_H

#include "SYSModuleHeader.h"
#include "rsa.h"

#if (USE_RSA_FUNC!=0)


#ifdef __cplusplus
extern "C"
{
#endif



#define	RSA_PRI_KEY_FILE_DEF		"private.pem"
#define	RSA_PUB_KEY_FILE_DEF		"public.pem"

#define	RSA_PKC_MODE_DEF		RSA_PKCS1_PADDING

#define RSA_SUCCESS				0
#define RSA_OPEN_FILE_ER		-1
#define RSA_READ_PRIV_KEY_ER	-2
#define RSA_READ_PUB_KEY_ER		-3
#define RSA_ENCRYPT_ER			-4
#define RSA_DECRYPT_ER			-5
#define RSA_OUT_BUFF_OVERFLOW	-6



/*!
@brief Rsa公钥加密。
@param[in] inlen  原文长度
@param[in] from  原文
@param[in,out] outlen  输入密文允许长度，输出密文实际长度
@param[out] to  输出密文
@param[in] pubfileName  公钥所在文件名
*/
int encryptByPubKey(int inlen, unsigned char *from, int *outlen, unsigned char *to, char *pubfileName);

/*!
@brief Rsa私钥解密。
@param[in] inlen  密文长度
@param[in] from  密文
@param[in,out] outlen  输入原文允许长度，输出原文实际长度
@param[out] to  输出原文
@param[in] prifileName  私钥所在文件名
*/
int decryptByPriKey(int inlen, unsigned char *from, int *outlen, unsigned char *to, char *prifileName);

/*!
@brief Rsa私钥加密。
@param[in] inlen  原文长度
@param[in] from  原文
@param[in,out] outlen  输入密文允许长度，输出密文实际长度
@param[out] to  输出密文
@param[in] prifileName  私钥所在文件名
*/
int encryptByPriKey(int inlen, unsigned char *from, int *outlen, unsigned char *to, char *prifileName);

/*!
@brief Rsa公钥解密。
@param[in] inlen  密文长度
@param[in] from  密文
@param[in,out] outlen  输入原文允许长度，输出原文实际长度
@param[out] to  输出原文
@param[in] pubfileName  公钥所在文件名
*/
int decryptByPubKey(int inlen, unsigned char *from, int *outlen, unsigned char *to, char *pubfileName);



/*!
@brief Rsa签名。
@param[in] inlen  原文长度
@param[in] from  原文
@param[in,out] siglen  输入签名允许长度，输出签名实际长度
@param[out] sigbuff  输出签名
@param[in] prifileName  私钥所在文件名
*/
int rsaSign(int inlen, unsigned char *from, int *siglen, unsigned char *sigbuff, char *prifileName);


/*!
@brief Rsa验签。
@param[in] inlen  密文长度
@param[in] from  密文
@param[in] siglen  签名长度
@param[out] sigbuff  签名内容
@param[in] pubfileName  公钥所在文件名
*/
int rsaVerify(int inlen, unsigned char *from, int siglen, unsigned char *sigbuff, char *pubfileName);



#ifdef __cplusplus
}
#endif


#endif		//USE_RSA_FUNC


#endif		//__APE_RSA__FUNC_H


