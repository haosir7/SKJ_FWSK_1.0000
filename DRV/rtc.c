
/*! \file   rtc.c
   \brief   RTC驱动
   \author  
 */
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h> 
#include "rtc.h"
#include "DRVModuleHeader.h"

#if (USE_RTC_DRV !=0)

static unsigned char set_data_time[8];	//设置时间用BUFFER
static unsigned char RTC_data_buf[8];	//读取时间用BUFFER
static int fd_i2c;		//设备文件号

static unsigned char RTC_Start()
{
#ifndef WIN32
	unsigned char reg;
	int ret;
	struct sub_address addr;

	// HT bit reset to 0
	addr.sub_addr = 0x0c;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, (unsigned long)&addr); 

	if ((ret = read(fd_i2c, &reg, 1)) < 0)
   	{
		printf("i2c read err! errNo = %d\n", ret);
		return 1;
	}
	reg &= 0xbf;
	addr.sub_addr = 0x0c;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, (unsigned long)&addr);
	
	if ((ret = write(fd_i2c, &reg, 1)) < 0)
	{
		printf("i2c write err! errNo = %d\n", ret);
		return 1;
	}

	// ST bit reset to 0
	addr.sub_addr = 0x01;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, (unsigned long)&addr); 

	if ((ret = read(fd_i2c, &reg, 1)) < 0)
   	{
		printf("i2c read err! errNo = %d\n", ret);
		return 1;
	}
	reg &= 0x7f;
	addr.sub_addr = 0x01;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, (unsigned long)&addr); 
	if ((ret = write(fd_i2c, &reg, 1)) < 0)
	{
		printf("i2c write err! errNo = %d\n", ret);
		return 1;
	}

	// Juge the OF bit
	addr.sub_addr = 0x0f;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, (unsigned long)&addr); 

	if ((ret = read(fd_i2c, &reg, 1)) < 0)
	{
		printf("i2c read err! errNo = %d\n", ret);
		return 1;
	}
	
	if((reg & 0x04) == 0x04)
	{
		// OF bit is 1,then to restart the oscillator
		// ST bit set to 1
		addr.sub_addr = 0x01;
		addr.sub_addr_len = 1;
		ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, (unsigned long)&addr); 
	
		if ((ret = read(fd_i2c, &reg, 1)) < 0)
   		{
    			printf("i2c read err! errNo = %d\n", ret);
        		return 1;
    		}
		reg |= 0x80;
		addr.sub_addr = 0x01;
		addr.sub_addr_len = 1;
		ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, (unsigned long)&addr); 
		if ((ret = write(fd_i2c, &reg, 1)) < 0)
    		{
    			printf("i2c write err! errNo = %d\n", ret);
        		return 1;
    		}
		
		// ST bit reset to 0
		addr.sub_addr = 0x01;
		addr.sub_addr_len = 1;
		ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, (unsigned long)&addr); 
		if ((ret = read(fd_i2c, &reg, 1)) < 0)
   		{
    			printf("i2c read err! errNo = %d\n", ret);
        		return 1;
    		}
		reg &= 0x7f;
		addr.sub_addr = 0x01;
		addr.sub_addr_len = 1;
		ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, (unsigned long)&addr); 
		if ((ret = write(fd_i2c, &reg, 1)) < 0)
    		{
    			printf("i2c write err! errNo = %d\n", ret);
      	  	return 1;
    		}
	}
#endif
	return 0;
}


void  I2C_Init(void)
{  
#ifndef WIN32
	int arg;
	
	if( ( fd_i2c = open("/dev/i2c",O_RDWR) ) == -1 ){
		printf("open i2c err\n");
		exit(-1);
	}
	
	//设置器件slave地址，必须右移一位
	arg = M41T80_SLAVE>>1;
	if (ioctl(fd_i2c, I2C_IOC_SET_DEV_ADDRESS, arg) == -1)
	{
		printf("ioctl SET_DEV_ADDRESS err\n");
		exit(-1);		
	}
	
	//设制时钟速度，400或100
	arg = 100;
        if (ioctl(fd_i2c, I2C_IOC_SET_SPEED, arg) == -1)
	{
		printf("ioctl I2C_IOC_SET_SPEED err\n");
		exit(-1);		
	}

	//RTC第一次上电后用此函数使其运行
	if ( RTC_Start() == 0 )
		printf("RTC_Start OK\n");
	else
		printf("RTC_Start ERR\n");
#endif
	
}


void  I2C_Close(void)
{  
#ifndef WIN32
	if( close(fd_i2c) ==-1 )
	{		
		printf("close i2c err!\n");
		exit(-1);
	}
#endif
	
}


unsigned char SetRTCData(unsigned char *data)
{
	
#ifndef WIN32
	int ret;
	struct sub_address addr;

	addr.sub_addr = 0x00;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, &addr); 

	if ( (ret = read(fd_i2c, RTC_data_buf, sizeof(RTC_data_buf))) < 0 )
        {
                printf("i2c read err! errNo = %d\n", ret);
              //  exit(-1);               
        }  		

	set_data_time[0]=RTC_data_buf[0];set_data_time[1]=RTC_data_buf[1];
	set_data_time[2]=RTC_data_buf[2];set_data_time[3]=RTC_data_buf[3];set_data_time[4]=RTC_data_buf[4];
	set_data_time[5]=*(data+2);set_data_time[6]=*(data+1);set_data_time[7]=*data;

	addr.sub_addr = 0x00;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, &addr); 

	if ( (ret = write(fd_i2c, set_data_time, sizeof(set_data_time))) < 0 )
        {
                printf("Set RTC Data err! errNo = %d\n", ret);
                exit(-1);               
        }  		
#endif

	return 0;
  
}


unsigned char SetRTCTime(unsigned char *data)
{

#ifndef WIN32
	int ret;
	struct sub_address addr;
	
	addr.sub_addr = 0x00;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, &addr); 

	if ( (ret = read(fd_i2c, RTC_data_buf, sizeof(RTC_data_buf))) < 0 )
        {
                printf("i2c read err! errNo = %d\n", ret);
              //  exit(-1);               
        }  
        		
	set_data_time[1]=*(data+2);set_data_time[2]=*(data+1);set_data_time[3]=*data;
	set_data_time[0]=RTC_data_buf[0];
	set_data_time[4]=RTC_data_buf[4];set_data_time[5]=RTC_data_buf[5];
	set_data_time[6]=RTC_data_buf[6];set_data_time[7]=RTC_data_buf[7];

	addr.sub_addr = 0x00;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, &addr); 

	if ( (ret = write(fd_i2c, set_data_time, sizeof(set_data_time))) < 0 )
        {
                printf("Set RTC Time err! errNo = %d\n", ret);
                exit(-1);               
        }  	
	
#endif
  
	return 0;
}


unsigned char  GetRTCData(unsigned char *data)
{
#ifndef WIN32
	int ret,i;
	struct sub_address addr;

	addr.sub_addr = 0x00;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, &addr);

	if ( (ret = read(fd_i2c, RTC_data_buf, sizeof(RTC_data_buf))) < 0 )
        {
                printf("i2c read err! errNo = %d\n", ret);
                exit(-1);               
        }  		
	
	*data=RTC_data_buf[7];
	*(data+1)=RTC_data_buf[6];
	*(data+2)=RTC_data_buf[5]; 

#endif
	return 0;
}


unsigned char  GetRTCTime(unsigned char *data)
{
#ifndef WIN32
	int ret;
	struct sub_address addr;

	addr.sub_addr = 0x00;
	addr.sub_addr_len = 1;
	ioctl(fd_i2c, I2C_IOC_SET_SUB_ADDRESS, &addr); 

	if ( (ret = read(fd_i2c, RTC_data_buf, sizeof(RTC_data_buf))) < 0 )
        {
                printf("i2c read err! errNo = %d\n", ret);
                exit(-1);               
        }  		

	*data=RTC_data_buf[3];
	*(data+1)=RTC_data_buf[2];
	*(data+2)=RTC_data_buf[1];

#endif
	return 0;
}

#else

void  I2C_Init(void)
{
	return;
}
unsigned char SetRTCTime(unsigned char *data)
{
	return 0;
}
unsigned char SetRTCData(unsigned char *data)
{
	return 0;
}

#endif //USE_RTC_DRV