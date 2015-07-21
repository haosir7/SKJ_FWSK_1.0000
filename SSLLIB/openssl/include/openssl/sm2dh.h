/*added by aisino gxg 201301
*/
#ifndef HEADER_SM2CDH_H
#define HEADER_SM2CDH_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_ECDH
#error SM2DH is disabled.
#endif

#include <openssl/ec.h>
#include <openssl/ossl_typ.h>
#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

int SM2_KDF(const EVP_MD *md, const unsigned char *share, size_t sharelen, 
			unsigned char *outkey, size_t* keylen);


static void __printBN(const BIGNUM *x, const char *c)
{
    char *str = BN_bn2hex(x);
    printf("\n%s:%s\n", c, str);
    OPENSSL_free(str);
}

#define printBN(x) __printBN(x, #x)


typedef int KDF(const EVP_MD *md, const unsigned char *share, 
					size_t sharelen, unsigned char *outkey, size_t* keylen);

const SM2DH_METHOD *SM2DH_OpenSSL(void);

void  SM2DH_set_default_method(const SM2DH_METHOD *);
const SM2DH_METHOD *SM2DH_get_default_method(void);
int	SM2DH_set_method(EC_KEY *, const SM2DH_METHOD *);

int SM2DH_compute_R(EC_KEY *eckey, unsigned char* outr, size_t outlen, const BIGNUM *r);

int SM2DH_compute_key(void *out, size_t* outlen, unsigned char* S, size_t* Slen, 
	unsigned char* S1, size_t* S1len, int isA, const BIGNUM *r, const EC_POINT *pub_key,
	EC_KEY *eckey, const EC_POINT *pub_R, const unsigned char* Za, size_t Zalen,  unsigned char* Zb, size_t Zblen,  
	int (*KDF)(const EVP_MD *md, const unsigned char *share, 
					size_t sharelen, unsigned char *outkey, size_t* keylen));

//ws 201401

void ERR_load_SM2DH_strings(void);
/* Function codes. */
#define SM2DH_F_SM2DH_CHECK				 102
#define SM2DH_F_SM2DH_COMPUTE_KEY				 100
#define SM2DH_F_SM2DH_DATA_NEW_METHOD			 101

/* Reason codes. */
#define SM2DH_R_KDF_FAILED				 102
#define SM2DH_R_NON_FIPS_METHOD				 103
#define SM2DH_R_NO_PRIVATE_VALUE				 100
#define SM2DH_R_POINT_ARITHMETIC_FAILURE			 101

#define SM2DSA_F_SM2DSA_DO_SIGN				 101
//end 
#ifdef  __cplusplus
}
#endif
#endif
