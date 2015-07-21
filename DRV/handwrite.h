
/*! \file   handwrite.h 
   \brief   手写板驱动
   \author   
 */

#ifndef HAND_WRITER_H
#define HAND_WRITER_H


#define	MAX_WAIT_TIME	1000000

#define HW_SUCCESS						1
#define HW_FAILURE						0

#define SCREEN_HW_TYPE		1	/**< 0:胜创通带屏 1:Aisino带屏 */

#ifdef __cplusplus
extern "C"
{
#endif

enum RESOLVE_STATUS
{
	STATE_RESET = 0,
	STATE_PACKHEAD,
	STATE_CMDNO,
	STATE_LENGTH,
	STATE_PARA_END,
	STATE_CRC_OK
};

struct RevCmdData
{
	unsigned char CmdNo;
	unsigned char CmdParaLen;
	unsigned char CmdPara[30];
	unsigned char CmdCRC;
	
};

/*!
@brief 初始化胜创通无屏手写板
@return  HW_SUCCESS	－ 初始化正确；HW_FAILURE－ 初始化错误；
*/
int HwInit(void);

/*!
@brief 初始化带屏胜创通手写板
@return  HW_SUCCESS	－ 初始化正确；HW_FAILURE－ 初始化错误；
*/
int ScreenHwInit(void);

/*!
@brief 释放通讯设备
@return  HW_SUCCESS	－ 初始化正确；HW_FAILURE－ 初始化错误；
*/
int HwRelease(void);

/*!
@brief 等待接收汉字
@param[out] hz_buf：备选汉字
@param[out] num：	汉字个数
@return  HW_SUCCESS	－ 初始化正确；HW_FAILURE－ 初始化错误；
*/
int HwRevHANZI(unsigned char *hz_buf, unsigned char *num);

/*!
@brief 询问接收设备当前收到的字节数
@param[in] HwPort：端口号
@return  字节个数
*/
int get_rev_count(int HwPort);

/*!
@brief 获取并解析手写数据
@param[in] chHWbuf  接收的buf
@param[in] nHwBufLen 接收的buf长度
@return  解析手写数据大小
*/
int HwDataAdjust(unsigned char *chHWbuf, int nHwBufLen);

#ifdef __cplusplus
}
#endif

#endif
