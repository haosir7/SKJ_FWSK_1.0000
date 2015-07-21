
/*! \file   vfd.c
   \brief   客显驱动
   \author 
*/

#include  <sys/ioctl.h>
#include  <unistd.h>
#include  <sys/stat.h>
#include  <fcntl.h>
#include  <termios.h>
#include "vfd.h"
#include "DRVModuleHeader.h"


#if (USE_VFD_DRV != 0)

static int fd;
static int SPI_status = 0; //是否已经初始化

void SPI_Init(void)
{
#ifndef WIN32
	if (SPI_status != 1)
	{
		fd = open("/dev/LED_VFD",O_RDONLY);
		if(fd<0)
		{
			printf("File LED_VFD open error.\n");
			SPI_status = 0;
			return;
		}
		SPI_status = 1;
	}
#endif
	return;
}

void SPI_SET_SPEED(int spd)
{
#ifndef WIN32
	if (SPI_status == 1)
	{
	    ioctl(fd, VFD_SET_SPEED, &spd);
	}
#endif	
   return;
}

void SPI_WRITE_STR(char *str)
{
#ifndef WIN32
	if (SPI_status == 1)
	{
		ioctl(fd, VFD_WRITE_STR, str);
	}
#endif
	
   return;
}

void SPI_WRITE_BYTE(char data)
{
#ifndef WIN32
	if (SPI_status == 1)
	{
		ioctl(fd, VFD_WRITE_BYTE, data);
	}
#endif
	
   return;
}

void SPI_Release(void)
{
#ifndef WIN32
	if (SPI_status == 1)
	{
		close(fd);
	}
#endif	
   return;
}

#else

void SPI_Init(void)
{
	return;
}

void SPI_WRITE_STR(char *str)
{
   return;
}

void SPI_Release(void)
{

   return;
}
#endif //USE_VFD_DRV
