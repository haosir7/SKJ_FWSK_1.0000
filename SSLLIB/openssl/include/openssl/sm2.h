/*added by aisino gxg 201301
*/

#ifndef HEADER_SM2_H
#define HEADER_SM2_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_SM2
#error SM2 is disabled.
#endif

#include <openssl/asn1.h>
#include <openssl/symhacks.h>
#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#endif

#ifdef  __cplusplus
extern "C" {
#elif defined(__SUNPRO_C)
# if __SUNPRO_C >= 0x520
# pragma error_messages (off,E_ARRAY_OF_INCOMPLETE_NONAME,E_ARRAY_OF_INCOMPLETE)
# endif
#endif

#define SM2_KEY EC_KEY

//ws 201401
/*
struct sm2_meth_st
{
	const char *name;
	int (*sm2_pub_enc)(BN_CTX* ctx_in, int flen,const unsigned char *from,
			   unsigned char *to,
			   SM2_KEY *sm2key);
	int (*sm2_pri_dec)(BN_CTX* ctx_in, int flen, unsigned char *from,
				const unsigned char *to,
				SM2_KEY *sm2);
	
	int (*init)(SM2_KEY *sm2key);		// called at new 
	int (*finish)(SM2_KEY *sm2key);	// called at free 
	int flags;			// SM2_METHOD_FLAG_* things 
	char *app_data;			// may be needed! 

	int (*sm2_sign)(const unsigned char *m, unsigned int m_length,
		unsigned char *sigret, unsigned int *siglen, const SM2_KEY *sm2key);
	int (*sm2_verify)(const unsigned char *m, unsigned int m_length,
		unsigned char *sigbuf, unsigned int siglen, const SM2_KEY *sm2key);

	int (*sm2_keygen)(SM2_KEY *sm2key);
};

typedef struct sm2_meth_st SM2_METHOD;
*/
//end

int SM2_public_encrypt(  const unsigned char *from,int flen,
	unsigned char *out,int * olen, SM2_KEY *sm2);
int SM2_private_decrypt(  const unsigned char *from,int flen,
	unsigned char *out,int * olen, SM2_KEY *sm2);
#ifdef  __cplusplus
}
#endif
#endif
