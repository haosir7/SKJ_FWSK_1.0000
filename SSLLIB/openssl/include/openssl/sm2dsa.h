/*added by aisino gxg 201301
*/
#ifndef HEADER_SM2DSA_H
#define HEADER_SM2DSA_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_SM2DSA
#error SM2DSA is disabled.
#endif

#include <openssl/ec.h>
#include <openssl/sm2.h>
#include <openssl/ossl_typ.h>
#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SM2DSA_SIG_st
	{
	BIGNUM *r;
	BIGNUM *s;
	} SM2DSA_SIG;

/** SM2DSA_SIG *SM2DSA_SIG_new(void)
 * allocates and initialize a SM2DSA_SIG structure
 * \return pointer to a SM2DSA_SIG structure or NULL if an error occurred
 */
SM2DSA_SIG *SM2DSA_SIG_new(void);

/** SM2DSA_SIG_free
 * frees a SM2DSA_SIG structure
 * \param a pointer to the SM2DSA_SIG structure
 */
void	  SM2DSA_SIG_free(SM2DSA_SIG *a);

/** i2d_SM2DSA_SIG
 * DER encode content of SM2DSA_SIG object (note: this function modifies *pp
 * (*pp += length of the DER encoded signature)).
 * \param a  pointer to the SM2DSA_SIG object
 * \param pp pointer to a unsigned char pointer for the output or NULL
 * \return the length of the DER encoded SM2DSA_SIG object or 0 
 */
int	  i2d_SM2DSA_SIG(const SM2DSA_SIG *a, unsigned char **pp);

/** d2i_SM2DSA_SIG
 * decodes a DER encoded SM2DSA signature (note: this function changes *pp
 * (*pp += len)). 
 * \param v pointer to SM2DSA_SIG pointer (may be NULL)
 * \param pp buffer with the DER encoded signature
 * \param len bufferlength
 * \return pointer to the decoded SM2DSA_SIG structure (or NULL)
 */
SM2DSA_SIG *d2i_SM2DSA_SIG(SM2DSA_SIG **v, const unsigned char **pp, long len);

/** SM2DSA_do_sign
 * computes the SM2DSA signature of the given hash value using
 * the supplied private key and returns the created signature.
 * \param dgst pointer to the hash value
 * \param dgst_len length of the hash value
 * \param eckey pointer to the SM2_KEY object containing a private EC key
 * \return pointer to a SM2DSA_SIG structure or NULL
 */
SM2DSA_SIG *SM2DSA_do_sign(const unsigned char *dgst,int dgst_len,SM2_KEY *eckey);

/** SM2DSA_do_sign_ex
 * computes SM2DSA signature of a given hash value using the supplied
 * private key (note: sig must point to SM2DSA_size(eckey) bytes of memory).
 * \param dgst pointer to the hash value to sign
 * \param dgstlen length of the hash value
 * \param kinv optional pointer to a pre-computed inverse k
 * \param rp optional pointer to the pre-computed rp value (see 
 *        SM2DSA_sign_setup
 * \param eckey pointer to the SM2_KEY object containing a private EC key
 * \return pointer to a SM2DSA_SIG structure or NULL
 */
SM2DSA_SIG *SM2DSA_do_sign_ex(const unsigned char *dgst, int dgstlen, 
		const BIGNUM *kinv, const BIGNUM *rp, SM2_KEY *eckey);

/** SM2DSA_do_verify
 * verifies that the supplied signature is a valid SM2DSA
 * signature of the supplied hash value using the supplied public key.
 * \param dgst pointer to the hash value
 * \param dgst_len length of the hash value
 * \param sig  pointer to the SM2DSA_SIG structure
 * \param eckey pointer to the SM2_KEY object containing a public EC key
 * \return 1 if the signature is valid, 0 if the signature is invalid and -1 on error
 */
int	  SM2DSA_do_verify(const unsigned char *dgst, int dgst_len,
		const SM2DSA_SIG *sig, SM2_KEY* eckey);

const SM2_METHOD *SM2_OpenSSL(void);

/** SM2DSA_set_default_method
 * sets the default SM2DSA method
 * \param meth the new default SM2_METHOD
 */
void	  SM2DSA_set_default_method(const SM2_METHOD *meth);

/** SM2DSA_get_default_method
 * returns the default SM2DSA method
 * \return pointer to SM2_METHOD structure containing the default method
 */
const SM2_METHOD *SM2DSA_get_default_method(void);

/** SM2DSA_set_method
 * sets method to be used for the SM2DSA operations
 * \param eckey pointer to the SM2_KEY object
 * \param meth  pointer to the new method
 * \return 1 on success and 0 otherwise 
 */
int 	  SM2DSA_set_method(SM2_KEY *eckey, const SM2_METHOD *meth);

/** SM2DSA_size
 * returns the maximum length of the DER encoded signature
 * \param  eckey pointer to a SM2_KEY object
 * \return numbers of bytes required for the DER encoded signature
 */
int	  SM2DSA_size(const SM2_KEY *eckey);

/** SM2DSA_sign_setup
 * precompute parts of the signing operation. 
 * \param eckey pointer to the SM2_KEY object containing a private EC key
 * \param ctx  pointer to a BN_CTX object (may be NULL)
 * \param kinv pointer to a BIGNUM pointer for the inverse of k
 * \param rp   pointer to a BIGNUM pointer for x coordinate of k * generator
 * \return 1 on success and 0 otherwise
 */
int   SM2DSA_sign_setup(SM2_KEY *eckey, BN_CTX *ctx, const unsigned char *dgst, int dgst_len, BIGNUM **kinv, 
		BIGNUM **rp);

/** SM2DSA_sign
 * computes SM2DSA signature of a given hash value using the supplied
 * private key (note: sig must point to SM2DSA_size(eckey) bytes of memory).
 * \param type this parameter is ignored
 * \param dgst pointer to the hash value to sign
 * \param dgstlen length of the hash value
 * \param sig buffer to hold the DER encoded signature
 * \param siglen pointer to the length of the returned signature
 * \param eckey pointer to the SM2_KEY object containing a private EC key
 * \return 1 on success and 0 otherwise
 */
int	  SM2DSA_sign(int type, const unsigned char *dgst, int dgstlen, 
		unsigned char *sig, unsigned int *siglen, SM2_KEY *eckey);


/** SM2DSA_sign_ex
 * computes SM2DSA signature of a given hash value using the supplied
 * private key (note: sig must point to SM2DSA_size(eckey) bytes of memory).
 * \param type this parameter is ignored
 * \param dgst pointer to the hash value to sign
 * \param dgstlen length of the hash value
 * \param sig buffer to hold the DER encoded signature
 * \param siglen pointer to the length of the returned signature
 * \param kinv optional pointer to a pre-computed inverse k
 * \param rp optional pointer to the pre-computed rp value (see 
 *        SM2DSA_sign_setup
 * \param eckey pointer to the SM2_KEY object containing a private EC key
 * \return 1 on success and 0 otherwise
 */
int	  SM2DSA_sign_ex(int type, const unsigned char *dgst, int dgstlen, 
		unsigned char *sig, unsigned int *siglen, const BIGNUM *kinv,
		const BIGNUM *rp, SM2_KEY *eckey);

/** SM2DSA_verify
 * verifies that the given signature is valid SM2DSA signature
 * of the supplied hash value using the specified public key.
 * \param type this parameter is ignored
 * \param dgst pointer to the hash value 
 * \param dgstlen length of the hash value
 * \param sig  pointer to the DER encoded signature
 * \param siglen length of the DER encoded signature
 * \param eckey pointer to the SM2_KEY object containing a public EC key
 * \return 1 if the signature is valid, 0 if the signature is invalid and -1 on error
 */
int 	  SM2DSA_verify(int type, const unsigned char *dgst, int dgstlen, 
		const unsigned char *sig, int siglen, SM2_KEY *eckey);

/* the standard ex_data functions */
int 	  SM2DSA_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new 
		*new_func, CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func);
int 	  SM2DSA_set_ex_data(SM2_KEY *d, int idx, void *arg);
void 	  *SM2DSA_get_ex_data(SM2_KEY *d, int idx);

//ws 201401
int SM2DSA_256_Za(const char* id, const EVP_MD *md, EC_KEY* ek, unsigned char* odigest, int* dlen);
//end
/* BEGIN ERROR CODES */
/* The following lines are auto generated by the script mkerr.pl. Any changes
 * made after this point may be overwritten when the script is next run.
 */
void ERR_load_SM2DSA_strings(void);

/* Error codes for the SM2DSA functions. */

/* Function codes. */
#define SM2DSA_F_SM2_DATA_NEW_METHOD			 100
#define SM2DSA_F_SM2DSA_DO_SIGN				 101
#define SM2DSA_F_SM2DSA_DO_VERIFY				 102
#define SM2DSA_F_SM2DSA_SIGN_SETUP			 103

/* Reason codes. */
#define SM2DSA_R_BAD_SIGNATURE				 100
#define SM2DSA_R_DATA_TOO_LARGE_FOR_KEY_SIZE		 101
#define SM2DSA_R_ERR_EC_LIB				 102
#define SM2DSA_R_MISSING_PARAMETERS			 103
#define SM2DSA_R_NEED_NEW_SETUP_VALUES			 106
#define SM2DSA_R_RANDOM_NUMBER_GENERATION_FAILED		 104
#define SM2DSA_R_SIGNATURE_MALLOC_FAILED			 105

#ifdef  __cplusplus
}
#endif
#endif
