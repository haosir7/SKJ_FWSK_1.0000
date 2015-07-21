/*! \file    rsaFunc.c
   \brief    Rsa加解密接口函数
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rsaFunc.h"

#if (USE_RSA_FUNC!=0)


int encryptByPubKey(int inlen, unsigned char *from, int *outlen, unsigned char *to, char *pubFileName)
{
	RSA* key=NULL;
	FILE *fp=NULL;
	int encrypt_size=0;
	int err=0;

	fp = fopen(pubFileName, "r");
	if (fp == NULL) 
	{
		err = RSA_OPEN_FILE_ER;
		goto encryptByPubKey_err;
	}
	if (!PEM_read_RSA_PUBKEY(fp, &key, NULL, NULL)) 
	{
		err = RSA_READ_PUB_KEY_ER;
		goto encryptByPubKey_err;
	}                                                         
	fclose(fp);
	
	encrypt_size = RSA_public_encrypt(inlen, from, to, key, RSA_PKC_MODE_DEF);
	if (encrypt_size <= 0)
	{
		err = RSA_ENCRYPT_ER;
		goto encryptByPubKey_err;
	}
	else if (encrypt_size>*outlen)
	{
		err = RSA_OUT_BUFF_OVERFLOW;
		goto encryptByPubKey_err;
	}
	
	*outlen = encrypt_size;
	
	RSA_free(key);
	return RSA_SUCCESS;

encryptByPubKey_err:
	RSA_free(key);
	return err;
}


int decryptByPriKey(int inlen, unsigned char *from, int *outlen, unsigned char *to, char *prifileName)
{
	RSA* key=NULL;
	FILE *fp=NULL;
	int decrypt_size=0;
	int err=0;

	fp = fopen(prifileName, "r");
	if (fp == NULL) 
	{
		err = RSA_OPEN_FILE_ER;
		goto decryptByPriKey_err;
	}
	if (!PEM_read_RSAPrivateKey(fp, &key, NULL, NULL)) 
	{
		err = RSA_READ_PRIV_KEY_ER;
		goto decryptByPriKey_err;
	}                                                         
	fclose(fp);
	
	decrypt_size = RSA_private_decrypt(inlen, from, to, key, RSA_PKC_MODE_DEF);
	if (decrypt_size <= 0)
	{
		err = RSA_DECRYPT_ER;
		goto decryptByPriKey_err;
	}
	else if (decrypt_size>*outlen)
	{
		err = RSA_OUT_BUFF_OVERFLOW;
		goto decryptByPriKey_err;
	}
	
	*outlen = decrypt_size;
	
	RSA_free(key);
	return RSA_SUCCESS;

decryptByPriKey_err:
	RSA_free(key);
	return err;		
}

int encryptByPriKey(int inlen, unsigned char *from, int *outlen, unsigned char *to, char *prifileName)
{
	RSA* key=NULL;
	FILE *fp=NULL;
	int encrypt_size=0;
	int err=0;

	fp = fopen(prifileName, "r");
	if (fp == NULL) 
	{
		err = RSA_OPEN_FILE_ER;
		goto encryptByPriKey_err;
	}
	if (!PEM_read_RSAPrivateKey(fp, &key, NULL, NULL)) 
	{
		err = RSA_READ_PRIV_KEY_ER;
		goto encryptByPriKey_err;
	}                                                         
	fclose(fp);
	
	encrypt_size = RSA_private_encrypt(inlen, from, to, key, RSA_PKC_MODE_DEF);
	if (encrypt_size == 0)
	{
		err = RSA_ENCRYPT_ER;
		goto encryptByPriKey_err;
	}
	else if (encrypt_size>*outlen)
	{
		err = RSA_OUT_BUFF_OVERFLOW;
		goto encryptByPriKey_err;
	}
	
	*outlen = encrypt_size;
	
	RSA_free(key);
	return RSA_SUCCESS;

encryptByPriKey_err:
	RSA_free(key);
	return err;	
}

int decryptByPubKey(int inlen, unsigned char *from, int *outlen, unsigned char *to, char *pubfileName)
{
	RSA* key=NULL;
	FILE *fp=NULL;
	int decrypt_size=0;
	int err=0;

	fp = fopen(pubfileName, "r");
	if (fp == NULL) 
	{
		err = RSA_OPEN_FILE_ER;
		goto decryptByPubKey_err;
	}
	if (!PEM_read_RSA_PUBKEY(fp, &key, NULL, NULL)) 
	{
		err = RSA_READ_PUB_KEY_ER;
		goto decryptByPubKey_err;
	}                                                         
	fclose(fp);
	
	decrypt_size = RSA_public_decrypt(inlen, from, to, key, RSA_PKC_MODE_DEF);
	if (decrypt_size == 0)
	{
		err = RSA_DECRYPT_ER;
		goto decryptByPubKey_err;
	}
	else if (decrypt_size>*outlen)
	{
		err = RSA_OUT_BUFF_OVERFLOW;
		goto decryptByPubKey_err;
	}
	
	*outlen = decrypt_size;
	
	RSA_free(key);
	return RSA_SUCCESS;

decryptByPubKey_err:
	RSA_free(key);
	return err;		
}



#define	NID_md5			4

int rsaSign(int inlen, unsigned char *from, int *siglen, unsigned char *sigbuff, char *prifileName)
{
	RSA* key=NULL;
	FILE *fp=NULL;
	int ret=0;
	int err=0;
	int tmplen=0;
	int i=0;

	fp = fopen(prifileName, "r");
	if (fp == NULL) 
	{
		err = RSA_OPEN_FILE_ER;
		goto sign_err;
	}
	if (!PEM_read_RSAPrivateKey(fp, &key, NULL, NULL)) 
	{
		err = RSA_READ_PRIV_KEY_ER;
		goto sign_err;
	}                                                         
	fclose(fp);
	
	ret = RSA_sign(NID_md5, from, inlen, sigbuff, siglen, key);
	if (ret == 0)
	{
		err = RSA_ENCRYPT_ER;
		goto sign_err;
	}
		
	RSA_free(key);
	return RSA_SUCCESS;

sign_err:
	RSA_free(key);
	return err;	
}

int rsaVerify(int inlen, unsigned char *from, int siglen, unsigned char *sigbuff, char *pubfileName)
{
	RSA* key=NULL;
	FILE *fp=NULL;
	int decrypt_size=0;
	int err=0;
	int ret=0;
	
	fp = fopen(pubfileName, "r");
	if (fp == NULL) 
	{
		err = RSA_OPEN_FILE_ER;
		goto verify_err;
	}
	if (!PEM_read_RSA_PUBKEY(fp, &key, NULL, NULL)) 
	{
		err = RSA_READ_PUB_KEY_ER;
		goto verify_err;
	}                                                         
	fclose(fp);
	
	ret = RSA_verify(NID_md5, from, inlen, sigbuff, siglen, key);
	if (ret == 0)
	{
		err = RSA_ENCRYPT_ER;
		goto verify_err;
	}
	
	
	
	RSA_free(key);
	return RSA_SUCCESS;

verify_err:
	RSA_free(key);
	return err;		
}

#endif		//USE_RSA_FUNC


