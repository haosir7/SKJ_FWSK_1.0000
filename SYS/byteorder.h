/*! \file    byteorder.h
   \brief    将一变量的字节序调转。以宏定义的方式实现。
 */

#ifndef __BYTEORDER_SWAB_H
#define __BYTEORDER_SWAB_H

#include "comdatatype.h"


static UINT16 __x16;
static UINT32 __x32;
static UINT64 __x64;


#define ___swab16(x) \
({ \
	__x16 = (x); \
	((UINT16)( \
		(((UINT16)(__x16) & (UINT16)0x00ffU) << 8) | \
		(((UINT16)(__x16) & (UINT16)0xff00U) >> 8) )); \
})

#define ___swab32(x) \
({ \
	__x32 = (x); \
	((UINT32)( \
		(((UINT32)(__x32) & (UINT32)0x000000ffUL) << 24) | \
		(((UINT32)(__x32) & (UINT32)0x0000ff00UL) <<  8) | \
		(((UINT32)(__x32) & (UINT32)0x00ff0000UL) >>  8) | \
		(((UINT32)(__x32) & (UINT32)0xff000000UL) >> 24) )); \
})

#define ___swab64(x) \
({ \
	__x64 = (x); \
	((UINT64)( \
		(UINT64)(((UINT64)(__x64) & (UINT64)0x00000000000000ffULL) << 56) | \
		(UINT64)(((UINT64)(__x64) & (UINT64)0x000000000000ff00ULL) << 40) | \
		(UINT64)(((UINT64)(__x64) & (UINT64)0x0000000000ff0000ULL) << 24) | \
		(UINT64)(((UINT64)(__x64) & (UINT64)0x00000000ff000000ULL) <<  8) | \
	    (UINT64)(((UINT64)(__x64) & (UINT64)0x000000ff00000000ULL) >>  8) | \
		(UINT64)(((UINT64)(__x64) & (UINT64)0x0000ff0000000000ULL) >> 24) | \
		(UINT64)(((UINT64)(__x64) & (UINT64)0x00ff000000000000ULL) >> 40) | \
		(UINT64)(((UINT64)(__x64) & (UINT64)0xff00000000000000ULL) >> 56) )); \
})

#define ___constant_swab16(x) \
	((UINT16)( \
		(((UINT16)(x) & (UINT16)0x00ffU) << 8) | \
		(((UINT16)(x) & (UINT16)0xff00U) >> 8) ))
#define ___constant_swab32(x) \
	((UINT32)( \
		(((UINT32)(x) & (UINT32)0x000000ffUL) << 24) | \
		(((UINT32)(x) & (UINT32)0x0000ff00UL) <<  8) | \
		(((UINT32)(x) & (UINT32)0x00ff0000UL) >>  8) | \
		(((UINT32)(x) & (UINT32)0xff000000UL) >> 24) ))
#define ___constant_swab64(x) \
	((UINT64)( \
		(UINT64)(((UINT64)(x) & (UINT64)0x00000000000000ffULL) << 56) | \
		(UINT64)(((UINT64)(x) & (UINT64)0x000000000000ff00ULL) << 40) | \
		(UINT64)(((UINT64)(x) & (UINT64)0x0000000000ff0000ULL) << 24) | \
		(UINT64)(((UINT64)(x) & (UINT64)0x00000000ff000000ULL) <<  8) | \
	    (UINT64)(((UINT64)(x) & (UINT64)0x000000ff00000000ULL) >>  8) | \
		(UINT64)(((UINT64)(x) & (UINT64)0x0000ff0000000000ULL) >> 24) | \
		(UINT64)(((UINT64)(x) & (UINT64)0x00ff000000000000ULL) >> 40) | \
		(UINT64)(((UINT64)(x) & (UINT64)0xff00000000000000ULL) >> 56) ))
	
	

/*!
@def swab16(x)
@brief 字节序调转（16位）
@param[in,out] x  输入一16位整形变量，调转其字节序并输出
 */
#define swab16(x) do { __x16 = x; __x16 = ___constant_swab16(__x16); x = __x16; } while (0)

/*! 
@def swab32(x)
@brief 字节序调转（32位）
@param[in,out] x  输入一32位整形变量，调转其字节序并输出
 */
#define swab32(x) do { __x32 = x; __x32 = ___constant_swab32(__x32); x = __x32; } while (0)

/*! 
@def swab64(x)
@brief 字节序调转（64位）
@param[in,out] x  输入一64位整形变量，调转其字节序并输出
 */
#define swab64(x) do { __x64 = x; __x64 = ___constant_swab64(__x64); x = __x64; }; while (0)



/*!
@def swab16p(x)
@brief 指针形式的字节序调转（16位）
@param[in,out] x  输入一16位整形变量的指针，调转其指向内容的字节序
 */
#define swab16p(x) do { __x16 = *(x); __x16 = ___constant_swab16(__x16); *(x) = __x16; } while (0)

/*! 
@def swab32p(x)
@brief 指针形式的字节序调转（32位）
@param[in,out] x  输入一32位整形变量的指针，调转其指向内容的字节序
 */
#define swab32p(x) do { __x32 = *(x); __x32 = ___constant_swab32(__x32); *(x) = __x32; } while (0)

/*! 
@def swab64p(x)
@brief 指针形式的字节序调转（64位）
@param[in,out] x  输入一64位整形变量的指针，调转其指向内容的字节序
 */
#define swab64p(x) do  {__x64 = *(x); __x64 = ___constant_swab64(__x64); *(x) = __x64; } while (0)




#endif /* __BYTEORDER_SWAB_H */
