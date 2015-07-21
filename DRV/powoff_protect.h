/*! \file   powoff_protect.h
   \brief   掉电保护驱动
   \author   
 */

#ifndef POWER_OFF_DRIVER_H
#define POWER_OFF_DRIVER_H

#include <stdlib.h>
#include <signal.h> 
#include <errno.h>     
 
#include  <stdio.h>
#include  <sys/ioctl.h>
#include  <sys/timeb.h>
#include  <unistd.h>
#include  <fcntl.h>
#include  <termios.h>

#include "DRVModuleHeader.h"


#if(USE_POWOFF_PROTECT_DRV !=0)	
/*IOCTLs define*/
#define POWOFF_TRANSMIT_PID		_IOW('p', 1, unsigned int)
#define POWOFF_DOWN			_IOW('p', 2, unsigned int)
#define DISABLE_POWOFF_IRQ		_IOW('p', 3, unsigned int)
#define ENABLE_POWOFF_IRQ		_IOW('p', 4, unsigned int)
#define DISABLE_ALL_IRQ			_IOW('p', 5, unsigned int)
#define ENABLE_ALL_IRQ			_IOW('p', 6, unsigned int)
#endif


#define POWOFF_DIR    "./back_powerdata"


#if(USE_POWOFF_PROTECT_DRV !=0)
extern sigset_t block_pwr, block_alarm, old_block;
extern int mask_alarm_flag;
#define	POWOFF_DISABLE()		MASK_SIGNAL()
#define	POWOFF_ENABLE()			UN_MASK_SIGNAL()
#else
#define	POWOFF_DISABLE()	
#define	POWOFF_ENABLE()		
#endif


/** @brief 阻塞掉电信号  */
#define MASK_SIGNAL() { \
sigprocmask(SIG_SETMASK, &block_pwr, NULL);\
}

/** @brief 放开被阻塞的掉电信号  */
#define  UN_MASK_SIGNAL() { \
sigprocmask(SIG_UNBLOCK,&block_pwr,NULL);\
}

/** @brief 阻塞ALARM信号（阻塞ALARM与原状态的并集）  */
#if(USE_POWOFF_PROTECT_DRV !=0)
#define MASK_ALARM_SIGNAL() { \
if (!mask_alarm_flag){\
	sigemptyset(&old_block);\
	sigprocmask(SIG_BLOCK, &block_alarm, &old_block);\
	mask_alarm_flag++;\
	}\
}
#else
#define MASK_ALARM_SIGNAL() 
#endif

/** @brief 恢复阻塞ALARM信号前的状态  */
#if(USE_POWOFF_PROTECT_DRV !=0)
#define UN_MASK_ALARM_SIGNAL() { \
if (mask_alarm_flag){\
	sigprocmask(SIG_SETMASK, &old_block, NULL);\
	mask_alarm_flag--;\
	}\
}
#else
#define UN_MASK_ALARM_SIGNAL() 
#endif

/*!
@brief		掉电信号处理
@param[in]  x 进程号
*/
extern void pw_handler(int x);

/*!
@brief 初始化掉电驱动层  
*/
void powOff_init_drv();

/*!
@brief 释放掉电驱动层
*/
void powOff_release_drv();

#endif

