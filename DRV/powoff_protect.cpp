
/*! \file   powoff_protect.cpp
   \brief   掉电保护驱动
   \author   
 */

#include "IncludeMe.h"
#include "powoff_protect.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"



#if(USE_POWOFF_PROTECT_DRV !=0)
sigset_t block_pwr, block_alarm, old_block;
int mask_alarm_flag;
#endif

volatile static int fd_powoff;

void powOff_init_drv( )
{
#if (USE_POWOFF_PROTECT_DRV != 0)
	
	//初始化掉电驱动，电容电路打开
	if( ( fd_powoff = open("/dev/powoff",O_RDONLY) ) == -1 )
	{
		DBG_PRINT(("open pow_off err"));
		exit(-1);
	}

	//初始化linux信号阻塞全局变量
	sigemptyset(&block_pwr);
	sigaddset(&block_pwr, SIGUSR1);
	sigaddset(&block_pwr, SIGALRM);
	
	sigemptyset(&block_alarm);
	sigaddset(&block_alarm, SIGALRM);

	sigemptyset(&old_block);

	
	//声明掉电消息函数
	ioctl(fd_powoff, POWOFF_TRANSMIT_PID, getpid());
	if (signal(SIGUSR1, pw_handler) == SIG_ERR)
	{
        DBG_PRINT((" signal failed!!, check you code"));
		exit(-1);
	}
	
#endif //USE_POWOFF_PROTECT_DRV
	
}


void powOff_release_drv()
{
#if (USE_POWOFF_PROTECT_DRV != 0)

	if( close(fd_powoff) ==-1 )
	{		
		DBG_PRINT(("close pow_off err!"));
//		exit(-1);
	}

#endif //USE_POWOFF_PROTECT_DRV
	
}

