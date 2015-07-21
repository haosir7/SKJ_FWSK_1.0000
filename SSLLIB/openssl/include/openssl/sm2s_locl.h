/*added by aisino gxg 201301
*/

#ifndef HEADER_SM2S_LOCL_H
#define HEADER_SM2S_LOCL_H

#include <openssl/sm2dsa.h>

#ifdef  __cplusplus
extern "C" {
#endif

struct sm2dsa_method 
{
	const char *name;
	SM2DSA_SIG *(*sm2dsa_do_sign)(const unsigned char *dgst, int dgst_len, 
			const BIGNUM *inv, const BIGNUM *rp, SM2_KEY *eckey);
	int (*sm2dsa_sign_setup)(SM2_KEY *eckey, BN_CTX *ctx, const unsigned char *dgst, int dgst_len, BIGNUM **kinv, 
			BIGNUM **r);
	int (*sm2dsa_do_verify)(const unsigned char *dgst, int dgst_len, 
			const SM2DSA_SIG *sig, SM2_KEY *eckey);
//ws 201401
	int (*sm2_pub_enc)(const unsigned char *from,int flen,
			   unsigned char *to,int *tlen,
			   SM2_KEY *sm2key);
	int (*sm2_pri_dec)( const unsigned char *from,int flen,
				 unsigned char *to,int *olen,
				SM2_KEY *sm2);
	int (*sm2_keygen)(SM2_KEY *sm2key);
//end
#if 0
	int (*init)(SM2_KEY *eckey);
	int (*finish)(SM2_KEY *eckey);
#endif
	int flags;
	char *app_data;
} ;

typedef struct sm2_data_st {
	/* EC_KEY_METH_DATA part */
	int (*init)(SM2_KEY *);
	/* method (SM2DSA) specific part */
	ENGINE	*engine;
	int	flags;
	const SM2_METHOD *meth;
	CRYPTO_EX_DATA ex_data;
} SM2_DATA;

/** ecdsa_check
 * checks whether ECKEY->meth_data is a pointer to a SM2_DATA structure
 * and if not it removes the old meth_data and creates a SM2_DATA structure.
 * \param  eckey pointer to a SM2_KEY object
 * \return pointer to a SM2_DATA structure
 */
SM2_DATA *sm2dsa_check(SM2_KEY *eckey);

#ifdef  __cplusplus
}
#endif

#endif /* HEADER_SM2S_LOCL_H */
