/*! \file    MaintenanceFunc.h
   \brief    awe4000r销售程序中系统维护的函数
   \author   Yu Yan
   \version  1.0
   \date     2008-3-19
 */

#ifndef MAINTENANCE_FUNC_H
#define MAINTENANCE_FUNC_H

#include "IncludeMe.h"
#include "SysMacDef.h"
#include "CaMsgBox.h"

/*!
@brief yes/no消息提示框	
@param[in] strInfo   提示框显示的字符串
@return  1: SUCCESS（是）; 0: FAILURE（否）
*/
UINT8 YesNoMsBox(string strInfo);


//-------机器自检--------//
/*!
@brief yes/no消息提示框	
@param[in] strInfo   提示框显示的字符串
@return  1: SUCCESS（是）; 0: FAILURE（否）
*/
void PrintStr(INT8 *str);

/*!
@brief CPU自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 CPUEvent(UINT8 IfForward);

/*!
@brief Beep自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 BeepEvent(UINT8 IfForward);

/*!
@brief 钱箱自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 BoxEvent(UINT8 IfForward);

/*!
@brief FLash自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 FlashEvent(UINT8 IfForward);

/*!
@brief RAM自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 RAMEvent(UINT8 IfForward);

/*!
@brief 串口0自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 COM0Event(UINT8 IfForward);

/*!
@brief 串口2自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 COM2Event(UINT8 IfForward);

/*!
@brief 打印机自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 PrinterEvent(UINT8 IfForward);

/*!
@brief LCD自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 LCDEvent(UINT8 IfForward);

/*!
@brief 键盘自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 KeyboardEvent(UINT8 IfForward);

/*!
@brief USB自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 USBEvent(UINT8 IfForward);
UINT8 USBEvent2(UINT8 IfForward);

/*!
@brief IC卡自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 ICCardEvent(UINT8 IfForward);

/*!
@brief 机器连续自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 ContinuousTestEvent(void);

/*!
@brief 机器定时自检过程	
@param[in] IfForward   是否进纸 1=进纸， 0=不进
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 MachineAutoCheck(void);

//-----------编码下载
/*!
@brief 机器编码下载	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 DownloadFCRCode(void);

//-----------MAC下载
/*!
@brief MAC地址下载
@param[in] nMacLen   存放mac地址的缓冲区长度 （不小于18）
@param[out] pMacAddr   存放mac地址的缓冲区首地址	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 DownloadMAC(INT8* pMacAddr, INT32 nMacLen);


/*!
@brief 键盘按键	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 CheckKey(UINT8 &nKeyTime);

#endif
