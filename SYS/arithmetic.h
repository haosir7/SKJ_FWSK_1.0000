/*! \file    arithmetic.h
   \brief    类型转换、数学计算一类的函数定义
   \author   zl
 */

#ifndef LIB_ARITHMETHIC_H
#define LIB_ARITHMETHIC_H

#include "comdatatype.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*!
@brief 将16进制HEX字串转换为可视的ASC字串。
@note 例：0x12,0x45,0xbc  --->   "1245bc"
@param[in] bin 原16进制HEX串
@param[in] length 原16进制HEX串的长度
@param[out] asc 输出的ASC字串
@warning 目标ASC字串长度将比原字串扩大一倍；
		 目标ASC字串没有在末尾自动添加'\0'字节。
*/
void bin2char(unsigned char *bin,char *asc,UINT32 length);

/*!
@brief 将ASC字串转换为16进制HEX字串。
@note 例："1245bc"  --->   0x12,0x45,0xbc
@param[in] asc 原ASC字串
@param[in] length 转换后的HEX串的长度（不是ASC字串长度！）
@param[out] bin 输出的HEX串
@warning ASC字串中不能含有0～9、a～f、A～F以外的字符；
		 ASC字串长度必须是2的倍数。
*/
void char2bin(unsigned char *bin, const char *asc, UINT32 length);

/*!
@brief 将BCD形式的10进制字串转换为10进制整形值。
@note 例：  0x12,0x45   --->   1245
@param[in] bin  原BCD串
@param[in] length  BCD串的长度
@return  整形值
@warning 转换后的值不能超出整形变量上限
*/
INT32 bin2int(unsigned char *bin, UINT32 length);

/*!
@brief 将10进制整形值转换为BCD形式的10进制字串。
@note 例：  1245   --->   0x12,0x45
@param[in] int_value  10进制整形值
@param[in] length  转换后的BCD串长度
@param[out] bin  输出的BCD串
*/
void int2bin(unsigned char *bin,INT32 int_value,UINT32 length);

/*!
@brief 将网络字节序的HEX串转换为整形值（主机字节序）。
@note 例：  0x03,0xE8   --->   1000 
@param[in] hex  原hex串
@param[in] length  hex串长度
@return  整形值
@warning 转换后的值不能超出整形变量上限
*/
INT32 hex2int(unsigned char *hex,UINT32 length);



/*!
@brief 将整形值（主机字节序）转换为网络字节序的HEX串。
@note 例：  1000    --->   0x03,0xE8
@param[in] int_value  整形值（主机字节序）
@param[in] length  转换后的HEX串长度
@param[out] hex  输出的HEX串
*/
void int2hex(unsigned char *hex,UINT32 int_value,UINT32 length);

/*!
@brief 同bin2char函数。
@param[in] bin 原16进制HEX串
@param[in] length 原16进制HEX串的长度
@param[out] str 输出的ASC字串
@see bin2char
*/
void Hex2Char(unsigned char *bin,char *str,unsigned int length);

/*!
@brief 同char2bin函数。
@param[in] str 原ASC字串
@param[in] length 转换后的HEX串的长度（不是ASC字串长度！）
@param[out] bin 输出的HEX串
@see char2bin
*/
void Char2Hex(unsigned char *bin,const char *str,unsigned int length);


/*!
@brief 以字节为单位进行异或计算
@param[in] databuff 字节流数据
@param[in] size 字节流的有效长度
@return  异或计算的结果 
*/
UINT8 bcc(unsigned char *databuff, UINT32 size);

/*!
@brief 异或计算，与bcc函数完全相同
@param[in] length 字节流的有效长度
@param[in] buffer 字节流数据
@return  异或计算的结果 
@see bcc
*/
unsigned char yihuo(UINT32 length, unsigned char *buffer);

/*!
@brief 计算crc16
@param[in] ptr 待计算的字节流数据
@param[in] len 字节流的有效长度
@return  crc16的计算结果 
*/
unsigned short int cal_crc(UINT8 *ptr, UINT32 len);

/*!
@brief 计算crc16，计算结果与cal_crc函数相同
@param[in] length 字节流的有效长度
@param[in] buffer 待计算的字节流数据
@param[out]  crc  双字节的crc计算结果
@see cal_crc
*/
void crcxdata(unsigned int length, unsigned char * buffer, unsigned char *crc);

/*!
@brief 税控码转换，将8字节HEX格式的税控码转成20字节ASC格式的税控码
@param[in] hex HEX格式税控码
@param[out] asc ASC格式的税控码
*/
void get_fiscal_code(unsigned char *hex, unsigned char *asc);

/*!
@brief 税控码转换，将20字节ASC格式的税控码转成8字节HEX格式的税控码
@param[out] hex HEX格式税控码
@param[in] asc ASC格式的税控码
*/
void fiscal_code_hex(unsigned char *hex, unsigned char *asc);

/*!
@brief 对计算或输入的数量做四舍五入处理
@param[in,out] f 需要做处理的浮点数
@return -1：该数量太大，无法存储； >=0 表示小数点后面有效位数
*/
INT32 AmountRound(double *f);

/*!
@brief 将浮点数数量类型转换为国标要求的整型类型
@param[in] dotnum 小数点后有效位数 需要做处理的浮点数
@param[in] f 需要转换的浮点数
@return 转换后的整型
*/
UINT32 FormatAmount(UINT32 dotnum, double f);

/*!
@brief 检查浮点数的小数位数
@param[in] f 需要检查小数位数的浮点数
@return >=0 表示小数点后面有效位数
*/
INT32 CheckFloatBit(double f);

/*!
@brief 将数据由double类型转换成INT64类型
@param[in] input 待转换的数据
@return  INT64类型的数据
*/
INT64 double2int(double input);


void getBytes(short data, UINT8* bytes);

short getShort(UINT8* bytes);

#ifdef __cplusplus
}
#endif

#endif	//ARITHMETHIC_H
