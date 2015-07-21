/*! \file    gZip.h
   \brief    gz格式的压缩与解压接口定义
   \author   zl
 */

#ifndef __GZIP_H_
#define __GZIP_H_

#include "SYSModuleHeader.h"
#include "zlib.h"

#if (USE_GZIP_LIB != 0)


#ifdef __cplusplus
extern "C"
{
#endif

/*!
@brief 将文件压缩为gz文件
@param[in] infilename  原始文件名
@param[in] outfilename  指定压缩后的文件名
@return  0-压缩成功；<0出错 
*/
int compress_one_file(const char *infilename, char *outfilename);

/*!
@brief 将gz格式的文件解压
@param[in] infilename  gz文件名
@param[in] outfilename  指定解压后的文件名
@return  0-压缩成功；<0出错 
*/
int decompress_one_file(const char *infilename, const char *outfilename);



/*!
@brief 将一段字节流中的内容压缩为zlib形式
@param[in] source  原始字节流
@param[in] sourceLen  原始字节流的长度
@param[out] dest   输出压缩后的字节流
@param[in,out] destLen  输入dest允许的最大长度，输出实际压缩后的长度
@return  0-成功；其他 - 出错 
*/
int compress_zlib(unsigned char *dest, unsigned long *destLen, 
				  const unsigned char *source, unsigned int sourceLen);

/*!
@brief 将zlib形式的字节流解压
@param[in] source  原始字节流
@param[in] sourceLen  原始字节流的长度
@param[out] dest   输出解压后的字节流
@param[in,out] destLen  输入dest允许的最大长度，输出实际解压后的长度
@return  0-成功；其他 - 出错 
*/
int uncompress_zlib(unsigned char *dest, unsigned long *destLen, 
					const unsigned char *source, unsigned int sourceLen);

/*!
@brief 预估zlib压缩后的长度
@param[in] sourceLen  原始字节流的长度
@return  压缩后长度的估计值
*/
unsigned int compressBound_zlib(unsigned int sourceLen);


#ifdef __cplusplus
}
#endif


#endif	//USE_GZIP_LIB

#endif	//__GZIP_H_
