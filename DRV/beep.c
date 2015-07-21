/*! \file   beep.c
   \brief    ·åÃùÆ÷Çý¶¯
   \author   
 */


#ifndef WIN32
#include  <sys/ioctl.h>
#include  <unistd.h>
#include  <sys/stat.h>
#include  <fcntl.h>
#include  <termios.h>
#endif
#include "beep.h"

static int fd;

void Beep_On()
{
#ifndef WIN32
   ioctl(fd, BUZZER_ALARM);
#endif
	
   return;
}

void Beep_Ini()
{
#ifndef WIN32
    int freq, time;
    
    fd = open("/dev/BUZZER",O_RDONLY);
    if (fd < 0) 
    {
     	//printf("Buzzer open error\n");
        return;
    }
    
    freq = 2000;
    time = 1000;
    
    ioctl(fd, BUZZER_INIT);
    ioctl(fd, BUZZER_SET_FREQUENCE, &freq);
    ioctl(fd, BUZZER_SET_TIMES, &time);
#endif

	MoneyBox_Init();

   return;
}


void Beep_Close()
{
#ifndef WIN32
   close(fd);
#endif
   return;
}

void MoneyBox_Init()
{
#ifndef WIN32
    ioctl(fd, MONEYBOX_INIT);
#endif
	
   return;
}

void MoneyBox_Open()
{
#ifndef WIN32
    ioctl(fd, MONEYBOX_OPEN);
#endif
	
   return;
}

