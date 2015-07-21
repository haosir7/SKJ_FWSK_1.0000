/*! \file   rtc.h
   \brief   RTC驱动
   \author  
 */

#ifndef __RTC_I2C_H
#define __RTC_I2C_H


#ifdef __cplusplus
extern "C"
{
#endif

#define   M41T80_SLAVE	  0xD0              /* 定义slave器件地址，并默认为transmit模式 */


/* define ioctl command */
#define I2C_IOC_MAGIC				'i'
#define I2C_IOC_MAXNR				 3

#define I2C_IOC_SET_DEV_ADDRESS			_IOW(I2C_IOC_MAGIC, 0, int)
#define I2C_IOC_SET_SUB_ADDRESS			_IOW(I2C_IOC_MAGIC, 1, int)
#define I2C_IOC_SET_SPEED			_IOW(I2C_IOC_MAGIC, 2, int)
#define I2C_IOC_GET_LAST_ERROR			_IOR(I2C_IOC_MAGIC, 3, int)

/* ioctl中用于传递子地址的结构体 */
struct sub_address{
	char sub_addr_len;
	unsigned int sub_addr;
};

/*!
@brief      I2C初始化
@details    包括初始化其中断为向量IRQ中断 
*/
void  I2C_Init(void);

/*!
@brief       设置年月日
@param[in]   data  存放year,month,data的指针
*/
unsigned char SetRTCData(unsigned char *data);//BCD FORMAT:YEAR_MONTH_DATA     EX:041105-->2004.11.05

/*!
@brief     设置时分
@param[in] data  存放hour,minute,second的指针 
*/
unsigned char SetRTCTime(unsigned char *data);//BCD FORMAT:HOUR_MINUTE_SECOND  EX:143002-->14:30:02

/*!
@brief		获取当前时间 
@param[out] data 返回存放年月日的指针 
*/
unsigned char GetRTCData(unsigned char *data);

/*!
@brief		获取当前时间 
@param[out] data 返回存放小时分的指针  
*/
unsigned char GetRTCTime(unsigned char *data);


#ifdef __cplusplus
}
#endif

#endif
