/*! \file   USBMount.h
   \brief   USB接口驱动
   \author  zl
*/
#ifndef __USB_MOUNT_H
#define __USB_MOUNT_H

#ifdef __cplusplus
extern "C"
{
#endif

/*!
@brief  加载USB盘
@param[in]  设备符 默认为NULL
@return true 成功 false 失败
*/
unsigned char UsbDiskMount(char* dev);

/*!
@brief  卸载USB盘
@return true 成功 false 失败
*/
unsigned char UsbDiskUnMount();

/*!
@brief  加载第二个USB盘
@param[in]  设备符 默认为NULL
@return true 成功 false 失败
*/
unsigned char UsbDiskMount2(char* dev);

/*!
@brief  卸载第二个USB盘
@return true 成功 false 失败
*/
unsigned char UsbDiskUnMount2();


#ifdef __cplusplus
}
#endif

#endif
