/*---------------------------------------------------------------------------------------------------*/
/*									以下是ADS环境的代码	(VC)同											 */
/*---------------------------------------------------------------------------------------------------*/
//#ifndef VISUAL_CPLUSPLUS		

#ifndef _A_PINYIN_H_POS_
#define _A_PINYIN_H_POS_
#include "stdlib.h"

#define MAX_KEYSTROKE_LEN	10
#define MAX_USER_KEYSTROKE_LEN	60
//#define USER_PHRASE_LIFE	100
//#define MODULE_ID_SIZE		20

#define VERLEN			20	/* Version buffer size */
#define ENCLEN			15	/* Encoding name buffer size */
#define CIN_ENAME_LENGTH	20      /* English name buffer size */
#define CIN_CNAME_LENGTH	20      /* Chinese name buffer size */
#define N_KEYCODE              50      /* # of valid keys 0-9, a-z, .... */
#define SELECT_KEY_LENGTH       15
#define END_KEY_LENGTH         10      /* max # of end keys */
#define N_CCODE_RULE           5       /* # of rules of encoding */
#define ICODE_MODE2		2	/* two icode & one icode_idx */
#define N_ASCII_KEY            95      /* Num of printable ASCII char */

typedef unsigned int	icode_t;
typedef unsigned short	icode_idx_t;
typedef icode_idx_t 	ichar_t;
typedef unsigned char ubyte_t;

typedef signed char byte_t;
typedef unsigned int xmode_t;
typedef unsigned short xtype_t;

#define HANZI_SIZE		27854//7664
#define PINYIN_SIZE     478  //471//415

#define WCH_SIZE  2

typedef union { 
    unsigned char s[WCH_SIZE];
    /*wchar_t*/unsigned short wch;
} wch_t;


typedef struct {
    char keystroke[MAX_KEYSTROKE_LEN+1];
    wch_t wch;
} kremap_t;

typedef struct {
    short n;
    ubyte_t begin[N_CCODE_RULE], end[N_CCODE_RULE];
} charcode_t;


typedef struct {
    short n;
    ubyte_t begin[N_CCODE_RULE], end[N_CCODE_RULE];
    short num[N_CCODE_RULE], ac_num[N_CCODE_RULE];
    short total_num;
    unsigned long base;
} ccode_t;

typedef struct {
    const icode_t *ic1;		/* icode & idx for different memory models */    
    const icode_idx_t *icidx;
} gen_inp_conf_t;

typedef struct {
    char objname[50];
    char encoding[50];
    char objloadname[100];
} objenc_t;

#ifdef __cplusplus
extern "C"
{ 
#endif
int match_keystroke_normal(unsigned char *inpinfo, unsigned int inpinfo_size,wchar_t *outbuff,unsigned int *outsize);

#ifdef __cplusplus
}
#endif

//#endif
#endif
